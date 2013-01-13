/********************************
 * Author: ideawu
 * Link: http://www.ideawu.net/
 ********************************/

grammar Expr;

options {
	language=Python;
	output=AST;
	ASTLabelType=CommonTree;
}

tokens{
	NOP; EMPTY_LINE; ID_LIST;
	NEW; THROW; TRY; CATCH; FINALLY;
	PRINT; PRINTF; SPRINTF;
	CALL; BLOCK; EXPR_LIST;
	IMPORT; MEMBER; MODULE;
	ARRAY; INDEX; SLICE; OBJECT;
	CLASS; FUNCTION; PARAMS;
	PRE_INC; PRE_DEC; POST_INC; POST_DEC;
	IF; ELSE; ELSE_IF; WHILE; DO_WHILE;
	SWITCH; CASE; DEFAULT;
	BREAK; CONTINUE; RETURN;
	FOR; FOREACH; EACH; EACH_VAL;
	CLASS; VAR; CONSTRUCTOR;
	ASSIGN; OP_ASSIGN;
	EXEC_STMT; EXEC_LIST;
	NEGATIVE;
}

prog
	: EOF -> NOP
	| stmt*
	;

stmt
	: ';' ->
	| exec_stmt
	| import_stmt
	| print_stmt | printf_stmt
	| break_stmt
	| continue_stmt
	| return_stmt
	| if_stmt
	| while_stmt
	| do_while_stmt
	| switch_stmt
	| for_stmt
	| foreach_stmt
	| throw_stmt
	| try_stmt
	| func_decl
	| class_decl
	;

/***** statements *****/
block
	: '{' stmt* '}'
		-> ^(BLOCK stmt*)
	;

import_stmt
	: 'import' module_path (',' module_path)* ';'
		-> ^(IMPORT module_path+)
	;
module_path
	: module
	| module '.*'
	;

printf_stmt
	: 'printf' '(' expr (',' expr_list)? ')' ';'
		-> ^(PRINTF expr expr_list?)
	;
// echo: no newline
print_stmt
	//: ('print')  expr (',' expr)* ';'
	//	-> ^(PRINT expr+)
	: ('print') expr_list ';'
		-> ^(PRINT expr_list)
	;

break_stmt
	: 'break' ';'
		-> BREAK
	;
continue_stmt
	: 'continue' ';'
		-> CONTINUE
	;
return_stmt
	: 'return' expr? ';'
		-> ^(RETURN expr?)
	;

if_stmt
	: if_clause else_if_clause* else_clause?
	;
if_clause
	: 'if' '(' expr ')' block
		-> ^(IF expr block)
	;
else_if_clause
	: 'else' if_clause
		-> ^(ELSE_IF if_clause)
	;
else_clause
	: 'else' block
		-> ^(ELSE block)
	;

while_stmt
	: 'while' '(' expr ')' block
		-> ^(WHILE expr block)
	;

do_while_stmt
	: 'do' block 'while' '(' expr ')' ';'
		-> ^(DO_WHILE block expr)
	;

switch_stmt
	: 'switch' '(' expr ')' case_block
		-> ^(SWITCH expr case_block)
	;
case_block
	: '{' (case_clause)+ (default_clause)? '}'
	;
case_clause
	: case_test+ stmt* break_stmt
		-> ^(CASE case_test+ stmt* break_stmt)
	;
case_test
	: 'case' expr ':'
		-> ^(CASE expr)
	;
default_clause
	: 'default' ':' stmt*
		-> ^(DEFAULT stmt*)
	;

for_stmt
	: 'for' '(' a=exec_list? ';' expr ';' b=exec_list? ')' block
		-> ^(FOR $a? expr block $b?)
	;
// for in 是一种 trackback 结构, 而 foreach as 不是
foreach_stmt
	: 'foreach' '(' expr 'as' each ')' block
		-> ^(FOREACH expr each block)
	;
each
	: each_val
		-> ^(EACH each_val)
	| ID '=>' each_val
		-> ^(EACH ID each_val)
	;
each_val
	: ID (',' ID)*
		-> ^(EACH_VAL ID+)
	;


throw_stmt
	: 'throw' expr ';'
		-> ^(THROW expr)
	;
try_stmt
	: 'try' block catch_clause+ finally_clause?
		-> ^(TRY block catch_clause+ finally_clause?)
	;
catch_clause
	: 'catch' '(' module ID? ')' block
		-> ^(CATCH module ID? block)
	;
finally_clause
	: 'finally' block
		-> ^(FINALLY block)
	;


func_decl
	: 'function' ID params block
		-> ^(FUNCTION ID params block)
	;
params
	: '(' param_decl? (',' param_decl)* ')'
		-> ^(PARAMS param_decl*)
	;
param_decl
	: ID ('=' atom)?
	;

class_decl
	: 'class' ID ('extends' ID)?
		'{' class_element* '}'
		-> ^(CLASS ID ID? class_element*)
	;
class_element
	: var_def | constructor | func_decl
	;
var_def
	: 'public' ID ('=' expr)? ';'
		-> ^(VAR ID expr?)
	| 'public' 'static' ID ('=' expr)? ';'
		-> ^(VAR 'static' ID expr?)
	;
constructor
	: 'function' 'init' params block
		-> ^(CONSTRUCTOR params block)
	;



/***** expressions *****/
member_expr
	: primary ('.' primary)*
		-> ^(MEMBER primary+)
	;
primary
	: ID index_expr* call_expr?
	;
call_expr
	: '(' expr_list? ')'
		-> ^(CALL expr_list?)
	;
index_expr
	options{
		backtrack = true;
	}
	: '[' expr ']'
		-> ^(INDEX expr)
	| '[' expr '..' expr? ']'
		-> ^(SLICE expr expr?)
	;


exec_list
	: exec_expr (',' exec_expr)*
		-> ^(EXEC_LIST exec_expr+)
	;
member_list
	: member_expr (',' member_expr)*
	;
exec_expr
	: member_expr
		(assign_op expr
			-> ^(ASSIGN member_expr assign_op expr)
		| '++'
			-> ^(POST_INC member_expr)
		| '--'
			-> ^(POST_DEC member_expr)
		|
			-> member_expr
		)
	| '++' member_expr
		-> ^(PRE_INC member_expr)
	| '--' member_expr
		-> ^(PRE_DEC member_expr)
	;
assign_op
	: '='|'+='|'-='|'*='|'/='|'%='|'&='|'^='|'|='
	;
exec_stmt
	: exec_list ';'
		-> ^(EXEC_STMT exec_list)
	;



expr_list
	: expr (',' expr)* ','?
		-> ^(EXPR_LIST expr+)
	;
expr
	: logic_or_expr
	;
logic_or_expr
	: logic_and_expr ('||'^ logic_and_expr)*
	;
logic_and_expr
	: bitwise_or_expr ('&&'^ bitwise_or_expr)*
	;
bitwise_or_expr
	: bitwise_xor_expr ('|'^ bitwise_xor_expr)*
	;
bitwise_xor_expr
	: bitwise_and_expr ('^'^ bitwise_and_expr)*
	;
bitwise_and_expr
	: relation_expr ('&'^ relation_expr)*
	;
relation_expr
	: add_expr (('<'|'>'|'<='|'>='|'=='|'!=')^ add_expr)?
	;
add_expr
	: mul_expr (('+'|'-')^ mul_expr)*
	;
mul_expr
	: not_expr (('*'|'/'|'%')^ not_expr)*
	;
not_expr
	: op='!'? negative_expr
		-> {$op != None}?
			^('!' negative_expr)
			-> negative_expr
	;
negative_expr
	: (op='-')? atom
		-> {$op != None}?
			^(NEGATIVE atom)
			-> atom
	;

atom
	: literal
	| member_expr
	| array_decl
	| object_decl
	| new_clause
	| sprintf
	| '(' expr ')' -> expr
	;
literal
	: BOOL | NULL | INT | FLOAT | STRING
	;

new_clause
	: 'new' module call_expr
		-> ^(NEW module call_expr)
	;
module
	: ID ('.' ID)*
		-> ^(MODULE ID+)
	;


array_decl
	: '[' expr_list? ']'
		-> ^(ARRAY expr_list?)
	;

object_decl
	: '{' property? (',' property)* ','? '}'
		-> ^(OBJECT property*)
	;
property
	: (ID | STRING | INT) ':' expr
	;


sprintf
	: 'sprintf' '(' expr (',' expr_list)? ')'
		-> ^(SPRINTF expr expr_list?)
	;

/***** tokens *****/

NULL
	: 'null'
	;
BOOL
	: 'true' | 'false'
	;
ID
	: (ALPHA | '_' | '$') (ALPHA | '_' | DIGIT)*
	;

INT
	: DIGIT+
	;
FLOAT
	: INT '.' DIGIT*
	;
fragment ALPHA
	: 'a'..'z' |'A'..'Z'
	;
fragment DIGIT
	: '0'..'9'
	;
// TODO: 字符串拼接 "$a$b"
STRING
	: '"' DOUBLE_QUOTE_CHARS* '"'
	| '\'' SINGLE_QUOTE_CHARS* '\''
	;
fragment DOUBLE_QUOTE_CHARS
	: ~('"')
	// 应该是 '\\"' 吧?
	| '\\' '"'
	;
fragment SINGLE_QUOTE_CHARS
	: ~('\'')
	| '\\' '\''
	;

fragment NEWLINE
	: '\r'? '\n'
	;

WS
	: (' '|'\t'|'\r'|'\n')+ {$channel=HIDDEN;}
	;
COMMENT
	: '/*' (options {greedy=false;}:.)* '*/' {$channel=HIDDEN;}
	;
LINECOMMENT
	: ('//'|'#') ~('\r'|'\n')* NEWLINE {$channel=HIDDEN;}
	;

