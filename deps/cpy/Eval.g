/********************************
 * Author: ideawu
 * Link: http://www.ideawu.net/
 ********************************/

tree grammar Eval;

options {
    language=Python;
    tokenVocab=Expr;
    ASTLabelType=CommonTree;
}

@header{
	from engine import CpyBuilder
}

@init{
}

prog[cpy]
	@init{
		self.cpy = cpy
	}
	@after{
		self.cpy.close()
	}
	: stmt*
	;

stmt
	: import_stmt
	| exec_stmt
	| print_stmt | printf_stmt
	| break_stmt
	| continue_stmt
	| return_stmt
	| if_stmt
	| while_stmt
	| do_while_stmt
	| switch_stmt
	| throw_stmt
	| try_stmt
	| func_decl
	| class_decl
	| for_stmt
	| foreach_stmt
	;

/***** statements *****/

block
	@init{
		self.cpy.block_enter()
	}
	@after{
		self.cpy.block_leave()
	}
	: ^(BLOCK stmt*)
	;

import_stmt
	: ^(IMPORT
		( a=module
			{self.cpy.op_import($a.text, None)}
		| b=module '.*'
			{self.cpy.op_import($b.text, '*')}
		)+
		)
	;

exec_stmt
	: ^(EXEC_STMT exec_list)
		{self.cpy.stmt($exec_list.text)}
	;
exec_expr returns[text]
	: member_expr
		{$text = $member_expr.text}
	| ^(ASSIGN member_expr op=('='|'+='|'-='|'*='|'/='|'%='|'&='|'^='|'|=') expr)
		{$text = self.cpy.op_assign($member_expr.text, $expr.text, $op.text)}
	| ^(POST_INC member_expr)
		{$text = self.cpy.op_inc($member_expr.text)}
	| ^(POST_DEC member_expr)
		{$text = self.cpy.op_dec($member_expr.text)}
	| ^(PRE_INC member_expr)
		{$text = self.cpy.op_inc($member_expr.text)}
	| ^(PRE_DEC member_expr)
		{$text = self.cpy.op_dec($member_expr.text)}
	;
exec_list returns[text]
	@init{ps = []}
	: ^(EXEC_LIST (exec_expr {ps.append($exec_expr.text)} ) +)
		{$text = ', '.join(ps)}
	;

printf_stmt
	: ^(PRINTF expr expr_list?)
		{self.cpy.op_printf($expr.text, $expr_list.text)}
	;
print_stmt
	: ^(PRINT expr_list)
		{self.cpy.op_print($expr_list.text)}
	//: ^(PRINT (expr {self.cpy.op_print($expr.text)} )+)
	//	{self.cpy.op_print_leave()}
	;

break_stmt
	: BREAK
		{self.cpy.op_break()}
	;
continue_stmt
	: CONTINUE
		{self.cpy.op_continue()}
	;
return_stmt
	: ^(RETURN expr?)
		{self.cpy.op_return($expr.text)}
	;


if_stmt
	@init{
		self.cpy.if_enter()
	}
	@after{
		self.cpy.if_leave()
	}
	: if_clause else_if_clause* else_clause?
	;
if_clause
	: ^(IF expr {self.cpy.op_if($expr.text)} block)
	;
else_if_clause
	: ^(ELSE_IF {self.cpy.op_else_if()} if_clause)
	;
else_clause
	: ^(ELSE {self.cpy.op_else()} block)
	;


while_stmt
	: ^(WHILE expr {self.cpy.op_while($expr.text)} block)
	;

do_while_stmt
	: ^(DO_WHILE {self.cpy.op_do_while_enter()}
		block
		expr {self.cpy.op_do_while_leave($expr.text)}
		)
	;


switch_stmt
	: ^(SWITCH expr {self.cpy.op_switch_enter($expr.text)} case_block)
		{self.cpy.op_switch_leave()}
	;
case_block
	: '{' (case_clause)+ (default_clause)? '}'
	;
case_clause
	@init{self.cpy.op_case_enter()}
	: ^(CASE case_test+ {self.cpy.op_case()} stmt* break_stmt)
		{self.cpy.op_case_leave()}
	;
case_test
	: ^(CASE expr)
		{self.cpy.op_case_test($expr.text)}
	;
default_clause
	@init{
		self.cpy.op_default_enter()
	}
	: ^(DEFAULT stmt*)
		{self.cpy.op_default_leave()}
	;


for_stmt
	: ^(FOR (a=exec_list {self.cpy.stmt($a.text)})?
		expr {self.cpy.op_while($expr.text)}
		block
		{self.cpy.block_enter()}
		(b=exec_list {self.cpy.stmt($b.text)})?
		{self.cpy.block_leave()}
		)
	;
// for in 是一种 trackback 结构, 而 foreach as 不是
foreach_stmt
	: ^(FOREACH expr
		( ^(EACH k=ID v=each_val)
			{self.cpy.op_foreach($expr.text, $k.text, $v.text)}
		| ^(EACH v=each_val)
			{self.cpy.op_foreach($expr.text, None, $v.text)}
		)
		block
		)
	;
each_val returns[text]
	@init{ps = []}
	: ^(EACH_VAL (ID {ps.append($ID.text)} )+)
		{$text = ','.join(ps)}
	;


throw_stmt
	: ^(THROW expr)
		{self.cpy.op_throw($expr.text)}
	;
try_stmt
	@init{self.cpy.op_try()}
	: ^(TRY block catch_clause+ finally_clause?)
	;
catch_clause
	: ^(CATCH module ID?
		{self.cpy.op_catch($module.text, $ID.text)}
		block)
	;
finally_clause
	@init{self.cpy.op_finally()}
	: ^(FINALLY block)
	;


func_decl
	: ^(FUNCTION ID params
		{self.cpy.op_function($ID.text, $params.text)}
		block
		)
	;
params returns[text]
	@init{ps = []}
	: ^(PARAMS (param_decl {ps.append($param_decl.text)} ) *)
		{$text = ', '.join(ps)}
	;
param_decl returns[text]
	: ID
		{$text = $ID.text}
		('=' atom
			{$text += ('=' + $atom.text)}
		)?
	;


class_decl
	@after{self.cpy.op_class_leave()}
	: ^(CLASS a=ID
			{self.cpy.op_class_enter($a.text, None)}
		class_element*)
	| ^(CLASS b=ID c=ID
			{self.cpy.op_class_enter($b.text, $c.text)}
		class_element*)
	;
class_element
	: var_def | constructor | func_decl
	;
var_def
	: ^(VAR ID expr?)
		{self.cpy.op_var_def(False, $ID.text, $expr.text)}
	| ^(VAR 'static' ID expr?)
		{self.cpy.op_var_def(True, $ID.text, $expr.text)}
	;
constructor
	: ^(CONSTRUCTOR params
		{self.cpy.op_construct($params.text)}
		block)
	;


/***** expressions *****/
module returns[text]
	@init{ps = []}
	: ^(MODULE (ID {ps.append($ID.text)} ) +)
		{$text = '.'.join(ps)}
	;

member_expr returns[text]
	@init{ps = []}
	: ^(MEMBER (primary {ps.append($primary.text)} ) +)
		{$text = '.'.join(ps)}
	;
primary returns[text]
	@init{a=''}
	: ID (index_expr{a += $index_expr.text})*
		call_expr?
		{
		b = $call_expr.text
		if b == None: b = ''
		$text = $ID.text + a + b
		}
	;
call_expr returns[text]
	: ^(CALL expr_list?)
		{
		s = $expr_list.text
		if s == None: s = ''
		$text = '(' + s + ')'
		}
	;
index_expr returns[text]
	: ^(INDEX expr)
		{$text = '[' + $expr.text + ']'}
	| ^(SLICE a=expr b=expr?)
		{
		s = $b.text
		if s == None: s = ''
		$text = '[\%s : \%s]' \%($a.text, s)
		}
	;


expr_list returns[text]
	@init{ps = []}
	: ^(EXPR_LIST (expr {ps.append($expr.text)} )+)
		{
		$text = ', '.join(ps)
		}
	;

expr returns[text]
	: a=relation_expr	{$text = $a.text}
	| a=logic_or_expr	{$text = $a.text}
	| a=logic_and_expr	{$text = $a.text}
	| a=bitwise_or_expr	{$text = $a.text}
	| a=bitwise_xor_expr	{$text = $a.text}
	| a=bitwise_and_expr	{$text = $a.text}
	| a=add_expr		{$text = $a.text}
	| a=mul_expr		{$text = $a.text}
	| a=not_expr		{$text = $a.text}
	| a=negative_expr	{$text = $a.text}
	| a=atom			{$text = $a.text}
	;
logic_or_expr returns[text]
	: ^('||' b=expr c=expr)
		{$text = '(' + $b.text + ' or ' + $c.text + ')'}
	;
logic_and_expr returns[text]
	: ^('&&' b=expr c=expr)
		{$text = $b.text + ' and ' + $c.text}
	;
bitwise_or_expr returns[text]
	: ^('|' b=expr c=expr)
		{$text = $b.text + ' | ' + $c.text}
	;
bitwise_xor_expr returns[text]
	: ^('^' b=expr c=expr)
		{$text = $b.text + ' ^ ' + $c.text}
	;
bitwise_and_expr returns[text]
	: ^('&' b=expr c=expr)
		{$text = $b.text + ' & ' + $c.text}
	;
relation_expr returns[text]
	: ^(op=('<'|'>'|'<='|'>='|'=='|'!=') b=expr c=expr)
		{$text = $b.text + $op.text + $c.text}
	;
add_expr returns[text]
	: ^(op=('+'|'-') b=expr c=expr)
		{$text = '(' + $b.text + ' ' + $op.text + ' ' + $c.text + ')'}
	;
mul_expr returns[text]
	: ^(op=('*'|'/'|'%') b=expr c=expr)
		{$text = $b.text + ' ' + $op.text + ' ' + $c.text}
	;
not_expr returns[text]
	: ^('!' a=expr)
		{$text = 'not (' + $a.text + ')'}
	;
negative_expr returns[text]
	: ^(NEGATIVE a=expr)
		{$text = '- (' + $a.text + ')'}
	;


sprintf returns[text]
	: ^(SPRINTF expr a=expr_list?)
		{
		s = $a.text
		if not s: s=''
		$text = $expr.text + '\%(' + s + ')'
		}
	;

new_clause returns[text]
	: ^(NEW module call_expr)
		{$text = $module.text + $call_expr.text}
	;

array_decl returns[text]
	: ^(ARRAY expr_list?)
		{
		s = $expr_list.text
		if s == None: s = ''
		$text = '[' + s + ']'
		}
	;
object_decl returns[text]
	@init{s = ''}
	: ^(OBJECT (property {s += $property.text} )*)
		{$text = '{' + s + '}'}
	;
property returns[text]
	: a=(ID | STRING | INT) ':' expr
		{$text = $a.text + ': ' + $expr.text + ','}
	;


atom returns[text]
	: a=literal		{$text = $a.text}
	| a=member_expr	{$text = $a.text}
	| a=new_clause	{$text = $a.text}
	| a=array_decl	{$text = $a.text}
	| a=object_decl	{$text = $a.text}
	| a=sprintf		{$text = $a.text}
	;
literal returns[text]
	: NULL {$text = 'None'}
	| BOOL {$text = $BOOL.text.capitalize()}
	| INT {$text = $INT.text}
	| FLOAT {$text = $FLOAT.text}
	| STRING {$text = $STRING.text}
	;
