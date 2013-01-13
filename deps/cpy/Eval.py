# $ANTLR 3.4 Eval.g 2012-12-09 16:07:29

import sys
from antlr3 import *
from antlr3.tree import *

from antlr3.compat import set, frozenset

       
from engine import CpyBuilder



# for convenience in actions
HIDDEN = BaseRecognizer.HIDDEN

# token types
EOF=-1
T__68=68
T__69=69
T__70=70
T__71=71
T__72=72
T__73=73
T__74=74
T__75=75
T__76=76
T__77=77
T__78=78
T__79=79
T__80=80
T__81=81
T__82=82
T__83=83
T__84=84
T__85=85
T__86=86
T__87=87
T__88=88
T__89=89
T__90=90
T__91=91
T__92=92
T__93=93
T__94=94
T__95=95
T__96=96
T__97=97
T__98=98
T__99=99
T__100=100
T__101=101
T__102=102
T__103=103
T__104=104
T__105=105
T__106=106
T__107=107
T__108=108
T__109=109
T__110=110
T__111=111
T__112=112
T__113=113
T__114=114
T__115=115
T__116=116
T__117=117
T__118=118
T__119=119
T__120=120
T__121=121
T__122=122
T__123=123
T__124=124
T__125=125
T__126=126
T__127=127
T__128=128
T__129=129
T__130=130
T__131=131
T__132=132
T__133=133
T__134=134
T__135=135
T__136=136
ALPHA=4
ARRAY=5
ASSIGN=6
BLOCK=7
BOOL=8
BREAK=9
CALL=10
CASE=11
CATCH=12
CLASS=13
COMMENT=14
CONSTRUCTOR=15
CONTINUE=16
DEFAULT=17
DIGIT=18
DOUBLE_QUOTE_CHARS=19
DO_WHILE=20
EACH=21
EACH_VAL=22
ELSE=23
ELSE_IF=24
EMPTY_LINE=25
EXEC_LIST=26
EXEC_STMT=27
EXPR_LIST=28
FINALLY=29
FLOAT=30
FOR=31
FOREACH=32
FUNCTION=33
ID=34
ID_LIST=35
IF=36
IMPORT=37
INDEX=38
INT=39
LINECOMMENT=40
MEMBER=41
MODULE=42
NEGATIVE=43
NEW=44
NEWLINE=45
NOP=46
NULL=47
OBJECT=48
OP_ASSIGN=49
PARAMS=50
POST_DEC=51
POST_INC=52
PRE_DEC=53
PRE_INC=54
PRINT=55
PRINTF=56
RETURN=57
SINGLE_QUOTE_CHARS=58
SLICE=59
SPRINTF=60
STRING=61
SWITCH=62
THROW=63
TRY=64
VAR=65
WHILE=66
WS=67

# token names
tokenNames = [
    "<invalid>", "<EOR>", "<DOWN>", "<UP>",
    "ALPHA", "ARRAY", "ASSIGN", "BLOCK", "BOOL", "BREAK", "CALL", "CASE", 
    "CATCH", "CLASS", "COMMENT", "CONSTRUCTOR", "CONTINUE", "DEFAULT", "DIGIT", 
    "DOUBLE_QUOTE_CHARS", "DO_WHILE", "EACH", "EACH_VAL", "ELSE", "ELSE_IF", 
    "EMPTY_LINE", "EXEC_LIST", "EXEC_STMT", "EXPR_LIST", "FINALLY", "FLOAT", 
    "FOR", "FOREACH", "FUNCTION", "ID", "ID_LIST", "IF", "IMPORT", "INDEX", 
    "INT", "LINECOMMENT", "MEMBER", "MODULE", "NEGATIVE", "NEW", "NEWLINE", 
    "NOP", "NULL", "OBJECT", "OP_ASSIGN", "PARAMS", "POST_DEC", "POST_INC", 
    "PRE_DEC", "PRE_INC", "PRINT", "PRINTF", "RETURN", "SINGLE_QUOTE_CHARS", 
    "SLICE", "SPRINTF", "STRING", "SWITCH", "THROW", "TRY", "VAR", "WHILE", 
    "WS", "'!'", "'!='", "'%'", "'%='", "'&&'", "'&'", "'&='", "'('", "')'", 
    "'*'", "'*='", "'+'", "'++'", "'+='", "','", "'-'", "'--'", "'-='", 
    "'.'", "'.*'", "'..'", "'/'", "'/='", "':'", "';'", "'<'", "'<='", "'='", 
    "'=='", "'=>'", "'>'", "'>='", "'['", "']'", "'^'", "'^='", "'as'", 
    "'break'", "'case'", "'catch'", "'class'", "'continue'", "'default'", 
    "'do'", "'else'", "'extends'", "'finally'", "'for'", "'foreach'", "'function'", 
    "'if'", "'import'", "'init'", "'new'", "'print'", "'printf'", "'public'", 
    "'return'", "'sprintf'", "'static'", "'switch'", "'throw'", "'try'", 
    "'while'", "'{'", "'|'", "'|='", "'||'", "'}'"
]




class Eval(TreeParser):
    grammarFileName = "Eval.g"
    api_version = 1
    tokenNames = tokenNames

    def __init__(self, input, state=None, *args, **kwargs):
        if state is None:
            state = RecognizerSharedState()

        super(Eval, self).__init__(input, state, *args, **kwargs)

        self.dfa4 = self.DFA4(
            self, 4,
            eot = self.DFA4_eot,
            eof = self.DFA4_eof,
            min = self.DFA4_min,
            max = self.DFA4_max,
            accept = self.DFA4_accept,
            special = self.DFA4_special,
            transition = self.DFA4_transition
            )



             


        self.delegates = []






    # $ANTLR start "prog"
    # Eval.g:21:1: prog[cpy] : ( stmt )* ;
    def prog(self, cpy):
              
        self.cpy = cpy
        	
        try:
            try:
                # Eval.g:28:2: ( ( stmt )* )
                # Eval.g:28:4: ( stmt )*
                pass 
                # Eval.g:28:4: ( stmt )*
                while True: #loop1
                    alt1 = 2
                    LA1_0 = self.input.LA(1)

                    if (LA1_0 == BREAK or LA1_0 == CLASS or LA1_0 == CONTINUE or LA1_0 == DO_WHILE or LA1_0 == EXEC_STMT or (FOR <= LA1_0 <= FUNCTION) or (IF <= LA1_0 <= IMPORT) or (PRINT <= LA1_0 <= RETURN) or (SWITCH <= LA1_0 <= TRY) or LA1_0 == WHILE) :
                        alt1 = 1


                    if alt1 == 1:
                        # Eval.g:28:4: stmt
                        pass 
                        self._state.following.append(self.FOLLOW_stmt_in_prog69)
                        self.stmt()

                        self._state.following.pop()


                    else:
                        break #loop1




                #action start
                       
                self.cpy.close()
                	
                #action end


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "prog"



    # $ANTLR start "stmt"
    # Eval.g:31:1: stmt : ( import_stmt | exec_stmt | print_stmt | printf_stmt | break_stmt | continue_stmt | return_stmt | if_stmt | while_stmt | do_while_stmt | switch_stmt | throw_stmt | try_stmt | func_decl | class_decl | for_stmt | foreach_stmt );
    def stmt(self, ):
        try:
            try:
                # Eval.g:32:2: ( import_stmt | exec_stmt | print_stmt | printf_stmt | break_stmt | continue_stmt | return_stmt | if_stmt | while_stmt | do_while_stmt | switch_stmt | throw_stmt | try_stmt | func_decl | class_decl | for_stmt | foreach_stmt )
                alt2 = 17
                LA2 = self.input.LA(1)
                if LA2 == IMPORT:
                    alt2 = 1
                elif LA2 == EXEC_STMT:
                    alt2 = 2
                elif LA2 == PRINT:
                    alt2 = 3
                elif LA2 == PRINTF:
                    alt2 = 4
                elif LA2 == BREAK:
                    alt2 = 5
                elif LA2 == CONTINUE:
                    alt2 = 6
                elif LA2 == RETURN:
                    alt2 = 7
                elif LA2 == IF:
                    alt2 = 8
                elif LA2 == WHILE:
                    alt2 = 9
                elif LA2 == DO_WHILE:
                    alt2 = 10
                elif LA2 == SWITCH:
                    alt2 = 11
                elif LA2 == THROW:
                    alt2 = 12
                elif LA2 == TRY:
                    alt2 = 13
                elif LA2 == FUNCTION:
                    alt2 = 14
                elif LA2 == CLASS:
                    alt2 = 15
                elif LA2 == FOR:
                    alt2 = 16
                elif LA2 == FOREACH:
                    alt2 = 17
                else:
                    nvae = NoViableAltException("", 2, 0, self.input)

                    raise nvae


                if alt2 == 1:
                    # Eval.g:32:4: import_stmt
                    pass 
                    self._state.following.append(self.FOLLOW_import_stmt_in_stmt81)
                    self.import_stmt()

                    self._state.following.pop()


                elif alt2 == 2:
                    # Eval.g:33:4: exec_stmt
                    pass 
                    self._state.following.append(self.FOLLOW_exec_stmt_in_stmt86)
                    self.exec_stmt()

                    self._state.following.pop()


                elif alt2 == 3:
                    # Eval.g:34:4: print_stmt
                    pass 
                    self._state.following.append(self.FOLLOW_print_stmt_in_stmt91)
                    self.print_stmt()

                    self._state.following.pop()


                elif alt2 == 4:
                    # Eval.g:34:17: printf_stmt
                    pass 
                    self._state.following.append(self.FOLLOW_printf_stmt_in_stmt95)
                    self.printf_stmt()

                    self._state.following.pop()


                elif alt2 == 5:
                    # Eval.g:35:4: break_stmt
                    pass 
                    self._state.following.append(self.FOLLOW_break_stmt_in_stmt100)
                    self.break_stmt()

                    self._state.following.pop()


                elif alt2 == 6:
                    # Eval.g:36:4: continue_stmt
                    pass 
                    self._state.following.append(self.FOLLOW_continue_stmt_in_stmt105)
                    self.continue_stmt()

                    self._state.following.pop()


                elif alt2 == 7:
                    # Eval.g:37:4: return_stmt
                    pass 
                    self._state.following.append(self.FOLLOW_return_stmt_in_stmt110)
                    self.return_stmt()

                    self._state.following.pop()


                elif alt2 == 8:
                    # Eval.g:38:4: if_stmt
                    pass 
                    self._state.following.append(self.FOLLOW_if_stmt_in_stmt115)
                    self.if_stmt()

                    self._state.following.pop()


                elif alt2 == 9:
                    # Eval.g:39:4: while_stmt
                    pass 
                    self._state.following.append(self.FOLLOW_while_stmt_in_stmt120)
                    self.while_stmt()

                    self._state.following.pop()


                elif alt2 == 10:
                    # Eval.g:40:4: do_while_stmt
                    pass 
                    self._state.following.append(self.FOLLOW_do_while_stmt_in_stmt125)
                    self.do_while_stmt()

                    self._state.following.pop()


                elif alt2 == 11:
                    # Eval.g:41:4: switch_stmt
                    pass 
                    self._state.following.append(self.FOLLOW_switch_stmt_in_stmt130)
                    self.switch_stmt()

                    self._state.following.pop()


                elif alt2 == 12:
                    # Eval.g:42:4: throw_stmt
                    pass 
                    self._state.following.append(self.FOLLOW_throw_stmt_in_stmt135)
                    self.throw_stmt()

                    self._state.following.pop()


                elif alt2 == 13:
                    # Eval.g:43:4: try_stmt
                    pass 
                    self._state.following.append(self.FOLLOW_try_stmt_in_stmt140)
                    self.try_stmt()

                    self._state.following.pop()


                elif alt2 == 14:
                    # Eval.g:44:4: func_decl
                    pass 
                    self._state.following.append(self.FOLLOW_func_decl_in_stmt145)
                    self.func_decl()

                    self._state.following.pop()


                elif alt2 == 15:
                    # Eval.g:45:4: class_decl
                    pass 
                    self._state.following.append(self.FOLLOW_class_decl_in_stmt150)
                    self.class_decl()

                    self._state.following.pop()


                elif alt2 == 16:
                    # Eval.g:46:4: for_stmt
                    pass 
                    self._state.following.append(self.FOLLOW_for_stmt_in_stmt155)
                    self.for_stmt()

                    self._state.following.pop()


                elif alt2 == 17:
                    # Eval.g:47:4: foreach_stmt
                    pass 
                    self._state.following.append(self.FOLLOW_foreach_stmt_in_stmt160)
                    self.foreach_stmt()

                    self._state.following.pop()



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "stmt"



    # $ANTLR start "block"
    # Eval.g:52:1: block : ^( BLOCK ( stmt )* ) ;
    def block(self, ):
              
        self.cpy.block_enter()
        	
        try:
            try:
                # Eval.g:59:2: ( ^( BLOCK ( stmt )* ) )
                # Eval.g:59:4: ^( BLOCK ( stmt )* )
                pass 
                self.match(self.input, BLOCK, self.FOLLOW_BLOCK_in_block185)

                if self.input.LA(1) == DOWN:
                    self.match(self.input, DOWN, None)
                    # Eval.g:59:12: ( stmt )*
                    while True: #loop3
                        alt3 = 2
                        LA3_0 = self.input.LA(1)

                        if (LA3_0 == BREAK or LA3_0 == CLASS or LA3_0 == CONTINUE or LA3_0 == DO_WHILE or LA3_0 == EXEC_STMT or (FOR <= LA3_0 <= FUNCTION) or (IF <= LA3_0 <= IMPORT) or (PRINT <= LA3_0 <= RETURN) or (SWITCH <= LA3_0 <= TRY) or LA3_0 == WHILE) :
                            alt3 = 1


                        if alt3 == 1:
                            # Eval.g:59:12: stmt
                            pass 
                            self._state.following.append(self.FOLLOW_stmt_in_block187)
                            self.stmt()

                            self._state.following.pop()


                        else:
                            break #loop3


                    self.match(self.input, UP, None)





                #action start
                       
                self.cpy.block_leave()
                	
                #action end


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "block"



    # $ANTLR start "import_stmt"
    # Eval.g:62:1: import_stmt : ^( IMPORT (a= module |b= module '.*' )+ ) ;
    def import_stmt(self, ):
        a = None

        b = None


        try:
            try:
                # Eval.g:63:2: ( ^( IMPORT (a= module |b= module '.*' )+ ) )
                # Eval.g:63:4: ^( IMPORT (a= module |b= module '.*' )+ )
                pass 
                self.match(self.input, IMPORT, self.FOLLOW_IMPORT_in_import_stmt201)

                self.match(self.input, DOWN, None)
                # Eval.g:64:3: (a= module |b= module '.*' )+
                cnt4 = 0
                while True: #loop4
                    alt4 = 3
                    alt4 = self.dfa4.predict(self.input)
                    if alt4 == 1:
                        # Eval.g:64:5: a= module
                        pass 
                        self._state.following.append(self.FOLLOW_module_in_import_stmt209)
                        a = self.module()

                        self._state.following.pop()

                        #action start
                        self.cpy.op_import(a, None)
                        #action end



                    elif alt4 == 2:
                        # Eval.g:66:5: b= module '.*'
                        pass 
                        self._state.following.append(self.FOLLOW_module_in_import_stmt222)
                        b = self.module()

                        self._state.following.pop()

                        self.match(self.input, 87, self.FOLLOW_87_in_import_stmt224)

                        #action start
                        self.cpy.op_import(b, '*')
                        #action end



                    else:
                        if cnt4 >= 1:
                            break #loop4

                        eee = EarlyExitException(4, self.input)
                        raise eee

                    cnt4 += 1


                self.match(self.input, UP, None)





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "import_stmt"



    # $ANTLR start "exec_stmt"
    # Eval.g:72:1: exec_stmt : ^( EXEC_STMT exec_list ) ;
    def exec_stmt(self, ):
        exec_list1 = None


        try:
            try:
                # Eval.g:73:2: ( ^( EXEC_STMT exec_list ) )
                # Eval.g:73:4: ^( EXEC_STMT exec_list )
                pass 
                self.match(self.input, EXEC_STMT, self.FOLLOW_EXEC_STMT_in_exec_stmt250)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_exec_list_in_exec_stmt252)
                exec_list1 = self.exec_list()

                self._state.following.pop()

                self.match(self.input, UP, None)


                #action start
                self.cpy.stmt(exec_list1)
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "exec_stmt"



    # $ANTLR start "exec_expr"
    # Eval.g:76:1: exec_expr returns [text] : ( member_expr | ^( ASSIGN member_expr op= ( '=' | '+=' | '-=' | '*=' | '/=' | '%=' | '&=' | '^=' | '|=' ) expr ) | ^( POST_INC member_expr ) | ^( POST_DEC member_expr ) | ^( PRE_INC member_expr ) | ^( PRE_DEC member_expr ) );
    def exec_expr(self, ):
        text = None


        op = None
        member_expr2 = None

        member_expr3 = None

        expr4 = None

        member_expr5 = None

        member_expr6 = None

        member_expr7 = None

        member_expr8 = None


        try:
            try:
                # Eval.g:77:2: ( member_expr | ^( ASSIGN member_expr op= ( '=' | '+=' | '-=' | '*=' | '/=' | '%=' | '&=' | '^=' | '|=' ) expr ) | ^( POST_INC member_expr ) | ^( POST_DEC member_expr ) | ^( PRE_INC member_expr ) | ^( PRE_DEC member_expr ) )
                alt5 = 6
                LA5 = self.input.LA(1)
                if LA5 == MEMBER:
                    alt5 = 1
                elif LA5 == ASSIGN:
                    alt5 = 2
                elif LA5 == POST_INC:
                    alt5 = 3
                elif LA5 == POST_DEC:
                    alt5 = 4
                elif LA5 == PRE_INC:
                    alt5 = 5
                elif LA5 == PRE_DEC:
                    alt5 = 6
                else:
                    nvae = NoViableAltException("", 5, 0, self.input)

                    raise nvae


                if alt5 == 1:
                    # Eval.g:77:4: member_expr
                    pass 
                    self._state.following.append(self.FOLLOW_member_expr_in_exec_expr270)
                    member_expr2 = self.member_expr()

                    self._state.following.pop()

                    #action start
                    text = member_expr2
                    #action end



                elif alt5 == 2:
                    # Eval.g:79:4: ^( ASSIGN member_expr op= ( '=' | '+=' | '-=' | '*=' | '/=' | '%=' | '&=' | '^=' | '|=' ) expr )
                    pass 
                    self.match(self.input, ASSIGN, self.FOLLOW_ASSIGN_in_exec_expr280)

                    self.match(self.input, DOWN, None)
                    self._state.following.append(self.FOLLOW_member_expr_in_exec_expr282)
                    member_expr3 = self.member_expr()

                    self._state.following.pop()

                    op = self.input.LT(1)

                    if self.input.LA(1) == 71 or self.input.LA(1) == 74 or self.input.LA(1) == 78 or self.input.LA(1) == 81 or self.input.LA(1) == 85 or self.input.LA(1) == 90 or self.input.LA(1) == 95 or self.input.LA(1) == 103 or self.input.LA(1) == 134:
                        self.input.consume()
                        self._state.errorRecovery = False


                    else:
                        mse = MismatchedSetException(None, self.input)
                        raise mse



                    self._state.following.append(self.FOLLOW_expr_in_exec_expr306)
                    expr4 = self.expr()

                    self._state.following.pop()

                    self.match(self.input, UP, None)


                    #action start
                    text = self.cpy.op_assign(member_expr3, expr4, op.text)
                    #action end



                elif alt5 == 3:
                    # Eval.g:81:4: ^( POST_INC member_expr )
                    pass 
                    self.match(self.input, POST_INC, self.FOLLOW_POST_INC_in_exec_expr317)

                    self.match(self.input, DOWN, None)
                    self._state.following.append(self.FOLLOW_member_expr_in_exec_expr319)
                    member_expr5 = self.member_expr()

                    self._state.following.pop()

                    self.match(self.input, UP, None)


                    #action start
                    text = self.cpy.op_inc(member_expr5)
                    #action end



                elif alt5 == 4:
                    # Eval.g:83:4: ^( POST_DEC member_expr )
                    pass 
                    self.match(self.input, POST_DEC, self.FOLLOW_POST_DEC_in_exec_expr330)

                    self.match(self.input, DOWN, None)
                    self._state.following.append(self.FOLLOW_member_expr_in_exec_expr332)
                    member_expr6 = self.member_expr()

                    self._state.following.pop()

                    self.match(self.input, UP, None)


                    #action start
                    text = self.cpy.op_dec(member_expr6)
                    #action end



                elif alt5 == 5:
                    # Eval.g:85:4: ^( PRE_INC member_expr )
                    pass 
                    self.match(self.input, PRE_INC, self.FOLLOW_PRE_INC_in_exec_expr343)

                    self.match(self.input, DOWN, None)
                    self._state.following.append(self.FOLLOW_member_expr_in_exec_expr345)
                    member_expr7 = self.member_expr()

                    self._state.following.pop()

                    self.match(self.input, UP, None)


                    #action start
                    text = self.cpy.op_inc(member_expr7)
                    #action end



                elif alt5 == 6:
                    # Eval.g:87:4: ^( PRE_DEC member_expr )
                    pass 
                    self.match(self.input, PRE_DEC, self.FOLLOW_PRE_DEC_in_exec_expr356)

                    self.match(self.input, DOWN, None)
                    self._state.following.append(self.FOLLOW_member_expr_in_exec_expr358)
                    member_expr8 = self.member_expr()

                    self._state.following.pop()

                    self.match(self.input, UP, None)


                    #action start
                    text = self.cpy.op_dec(member_expr8)
                    #action end




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "exec_expr"



    # $ANTLR start "exec_list"
    # Eval.g:90:1: exec_list returns [text] : ^( EXEC_LIST ( exec_expr )+ ) ;
    def exec_list(self, ):
        text = None


        exec_expr9 = None


        ps = []
        try:
            try:
                # Eval.g:92:2: ( ^( EXEC_LIST ( exec_expr )+ ) )
                # Eval.g:92:4: ^( EXEC_LIST ( exec_expr )+ )
                pass 
                self.match(self.input, EXEC_LIST, self.FOLLOW_EXEC_LIST_in_exec_list382)

                self.match(self.input, DOWN, None)
                # Eval.g:92:16: ( exec_expr )+
                cnt6 = 0
                while True: #loop6
                    alt6 = 2
                    LA6_0 = self.input.LA(1)

                    if (LA6_0 == ASSIGN or LA6_0 == MEMBER or (POST_DEC <= LA6_0 <= PRE_INC)) :
                        alt6 = 1


                    if alt6 == 1:
                        # Eval.g:92:17: exec_expr
                        pass 
                        self._state.following.append(self.FOLLOW_exec_expr_in_exec_list385)
                        exec_expr9 = self.exec_expr()

                        self._state.following.pop()

                        #action start
                        ps.append(exec_expr9)
                        #action end



                    else:
                        if cnt6 >= 1:
                            break #loop6

                        eee = EarlyExitException(6, self.input)
                        raise eee

                    cnt6 += 1


                self.match(self.input, UP, None)


                #action start
                text = ', '.join(ps)
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "exec_list"



    # $ANTLR start "printf_stmt"
    # Eval.g:96:1: printf_stmt : ^( PRINTF expr ( expr_list )? ) ;
    def printf_stmt(self, ):
        expr10 = None

        expr_list11 = None


        try:
            try:
                # Eval.g:97:2: ( ^( PRINTF expr ( expr_list )? ) )
                # Eval.g:97:4: ^( PRINTF expr ( expr_list )? )
                pass 
                self.match(self.input, PRINTF, self.FOLLOW_PRINTF_in_printf_stmt408)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_expr_in_printf_stmt410)
                expr10 = self.expr()

                self._state.following.pop()

                # Eval.g:97:18: ( expr_list )?
                alt7 = 2
                LA7_0 = self.input.LA(1)

                if (LA7_0 == EXPR_LIST) :
                    alt7 = 1
                if alt7 == 1:
                    # Eval.g:97:18: expr_list
                    pass 
                    self._state.following.append(self.FOLLOW_expr_list_in_printf_stmt412)
                    expr_list11 = self.expr_list()

                    self._state.following.pop()




                self.match(self.input, UP, None)


                #action start
                self.cpy.op_printf(expr10, expr_list11)
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "printf_stmt"



    # $ANTLR start "print_stmt"
    # Eval.g:100:1: print_stmt : ^( PRINT expr_list ) ;
    def print_stmt(self, ):
        expr_list12 = None


        try:
            try:
                # Eval.g:101:2: ( ^( PRINT expr_list ) )
                # Eval.g:101:4: ^( PRINT expr_list )
                pass 
                self.match(self.input, PRINT, self.FOLLOW_PRINT_in_print_stmt429)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_expr_list_in_print_stmt431)
                expr_list12 = self.expr_list()

                self._state.following.pop()

                self.match(self.input, UP, None)


                #action start
                self.cpy.op_print(expr_list12)
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "print_stmt"



    # $ANTLR start "break_stmt"
    # Eval.g:107:1: break_stmt : BREAK ;
    def break_stmt(self, ):
        try:
            try:
                # Eval.g:108:2: ( BREAK )
                # Eval.g:108:4: BREAK
                pass 
                self.match(self.input, BREAK, self.FOLLOW_BREAK_in_break_stmt451)

                #action start
                self.cpy.op_break()
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "break_stmt"



    # $ANTLR start "continue_stmt"
    # Eval.g:111:1: continue_stmt : CONTINUE ;
    def continue_stmt(self, ):
        try:
            try:
                # Eval.g:112:2: ( CONTINUE )
                # Eval.g:112:4: CONTINUE
                pass 
                self.match(self.input, CONTINUE, self.FOLLOW_CONTINUE_in_continue_stmt465)

                #action start
                self.cpy.op_continue()
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "continue_stmt"



    # $ANTLR start "return_stmt"
    # Eval.g:115:1: return_stmt : ^( RETURN ( expr )? ) ;
    def return_stmt(self, ):
        expr13 = None


        try:
            try:
                # Eval.g:116:2: ( ^( RETURN ( expr )? ) )
                # Eval.g:116:4: ^( RETURN ( expr )? )
                pass 
                self.match(self.input, RETURN, self.FOLLOW_RETURN_in_return_stmt480)

                if self.input.LA(1) == DOWN:
                    self.match(self.input, DOWN, None)
                    # Eval.g:116:13: ( expr )?
                    alt8 = 2
                    LA8_0 = self.input.LA(1)

                    if (LA8_0 == ARRAY or LA8_0 == BOOL or LA8_0 == FLOAT or LA8_0 == INT or LA8_0 == MEMBER or (NEGATIVE <= LA8_0 <= NEW) or (NULL <= LA8_0 <= OBJECT) or (SPRINTF <= LA8_0 <= STRING) or (68 <= LA8_0 <= 70) or (72 <= LA8_0 <= 73) or LA8_0 == 77 or LA8_0 == 79 or LA8_0 == 83 or LA8_0 == 89 or (93 <= LA8_0 <= 94) or LA8_0 == 96 or (98 <= LA8_0 <= 99) or LA8_0 == 102 or LA8_0 == 133 or LA8_0 == 135) :
                        alt8 = 1
                    if alt8 == 1:
                        # Eval.g:116:13: expr
                        pass 
                        self._state.following.append(self.FOLLOW_expr_in_return_stmt482)
                        expr13 = self.expr()

                        self._state.following.pop()




                    self.match(self.input, UP, None)



                #action start
                self.cpy.op_return(expr13)
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "return_stmt"



    # $ANTLR start "if_stmt"
    # Eval.g:121:1: if_stmt : if_clause ( else_if_clause )* ( else_clause )? ;
    def if_stmt(self, ):
              
        self.cpy.if_enter()
        	
        try:
            try:
                # Eval.g:128:2: ( if_clause ( else_if_clause )* ( else_clause )? )
                # Eval.g:128:4: if_clause ( else_if_clause )* ( else_clause )?
                pass 
                self._state.following.append(self.FOLLOW_if_clause_in_if_stmt510)
                self.if_clause()

                self._state.following.pop()

                # Eval.g:128:14: ( else_if_clause )*
                while True: #loop9
                    alt9 = 2
                    LA9_0 = self.input.LA(1)

                    if (LA9_0 == ELSE_IF) :
                        alt9 = 1


                    if alt9 == 1:
                        # Eval.g:128:14: else_if_clause
                        pass 
                        self._state.following.append(self.FOLLOW_else_if_clause_in_if_stmt512)
                        self.else_if_clause()

                        self._state.following.pop()


                    else:
                        break #loop9


                # Eval.g:128:30: ( else_clause )?
                alt10 = 2
                LA10_0 = self.input.LA(1)

                if (LA10_0 == ELSE) :
                    alt10 = 1
                if alt10 == 1:
                    # Eval.g:128:30: else_clause
                    pass 
                    self._state.following.append(self.FOLLOW_else_clause_in_if_stmt515)
                    self.else_clause()

                    self._state.following.pop()






                #action start
                       
                self.cpy.if_leave()
                	
                #action end


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "if_stmt"



    # $ANTLR start "if_clause"
    # Eval.g:130:1: if_clause : ^( IF expr block ) ;
    def if_clause(self, ):
        expr14 = None


        try:
            try:
                # Eval.g:131:2: ( ^( IF expr block ) )
                # Eval.g:131:4: ^( IF expr block )
                pass 
                self.match(self.input, IF, self.FOLLOW_IF_in_if_clause527)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_expr_in_if_clause529)
                expr14 = self.expr()

                self._state.following.pop()

                #action start
                self.cpy.op_if(expr14)
                #action end


                self._state.following.append(self.FOLLOW_block_in_if_clause533)
                self.block()

                self._state.following.pop()

                self.match(self.input, UP, None)





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "if_clause"



    # $ANTLR start "else_if_clause"
    # Eval.g:133:1: else_if_clause : ^( ELSE_IF if_clause ) ;
    def else_if_clause(self, ):
        try:
            try:
                # Eval.g:134:2: ( ^( ELSE_IF if_clause ) )
                # Eval.g:134:4: ^( ELSE_IF if_clause )
                pass 
                self.match(self.input, ELSE_IF, self.FOLLOW_ELSE_IF_in_else_if_clause545)

                #action start
                self.cpy.op_else_if()
                #action end


                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_if_clause_in_else_if_clause549)
                self.if_clause()

                self._state.following.pop()

                self.match(self.input, UP, None)





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "else_if_clause"



    # $ANTLR start "else_clause"
    # Eval.g:136:1: else_clause : ^( ELSE block ) ;
    def else_clause(self, ):
        try:
            try:
                # Eval.g:137:2: ( ^( ELSE block ) )
                # Eval.g:137:4: ^( ELSE block )
                pass 
                self.match(self.input, ELSE, self.FOLLOW_ELSE_in_else_clause561)

                #action start
                self.cpy.op_else()
                #action end


                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_block_in_else_clause565)
                self.block()

                self._state.following.pop()

                self.match(self.input, UP, None)





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "else_clause"



    # $ANTLR start "while_stmt"
    # Eval.g:141:1: while_stmt : ^( WHILE expr block ) ;
    def while_stmt(self, ):
        expr15 = None


        try:
            try:
                # Eval.g:142:2: ( ^( WHILE expr block ) )
                # Eval.g:142:4: ^( WHILE expr block )
                pass 
                self.match(self.input, WHILE, self.FOLLOW_WHILE_in_while_stmt579)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_expr_in_while_stmt581)
                expr15 = self.expr()

                self._state.following.pop()

                #action start
                self.cpy.op_while(expr15)
                #action end


                self._state.following.append(self.FOLLOW_block_in_while_stmt585)
                self.block()

                self._state.following.pop()

                self.match(self.input, UP, None)





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "while_stmt"



    # $ANTLR start "do_while_stmt"
    # Eval.g:145:1: do_while_stmt : ^( DO_WHILE block expr ) ;
    def do_while_stmt(self, ):
        expr16 = None


        try:
            try:
                # Eval.g:146:2: ( ^( DO_WHILE block expr ) )
                # Eval.g:146:4: ^( DO_WHILE block expr )
                pass 
                self.match(self.input, DO_WHILE, self.FOLLOW_DO_WHILE_in_do_while_stmt598)

                #action start
                self.cpy.op_do_while_enter()
                #action end


                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_block_in_do_while_stmt604)
                self.block()

                self._state.following.pop()

                self._state.following.append(self.FOLLOW_expr_in_do_while_stmt608)
                expr16 = self.expr()

                self._state.following.pop()

                #action start
                self.cpy.op_do_while_leave(expr16)
                #action end


                self.match(self.input, UP, None)





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "do_while_stmt"



    # $ANTLR start "switch_stmt"
    # Eval.g:153:1: switch_stmt : ^( SWITCH expr case_block ) ;
    def switch_stmt(self, ):
        expr17 = None


        try:
            try:
                # Eval.g:154:2: ( ^( SWITCH expr case_block ) )
                # Eval.g:154:4: ^( SWITCH expr case_block )
                pass 
                self.match(self.input, SWITCH, self.FOLLOW_SWITCH_in_switch_stmt627)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_expr_in_switch_stmt629)
                expr17 = self.expr()

                self._state.following.pop()

                #action start
                self.cpy.op_switch_enter(expr17)
                #action end


                self._state.following.append(self.FOLLOW_case_block_in_switch_stmt633)
                self.case_block()

                self._state.following.pop()

                self.match(self.input, UP, None)


                #action start
                self.cpy.op_switch_leave()
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "switch_stmt"



    # $ANTLR start "case_block"
    # Eval.g:157:1: case_block : '{' ( case_clause )+ ( default_clause )? '}' ;
    def case_block(self, ):
        try:
            try:
                # Eval.g:158:2: ( '{' ( case_clause )+ ( default_clause )? '}' )
                # Eval.g:158:4: '{' ( case_clause )+ ( default_clause )? '}'
                pass 
                self.match(self.input, 132, self.FOLLOW_132_in_case_block648)

                # Eval.g:158:8: ( case_clause )+
                cnt11 = 0
                while True: #loop11
                    alt11 = 2
                    LA11_0 = self.input.LA(1)

                    if (LA11_0 == CASE) :
                        alt11 = 1


                    if alt11 == 1:
                        # Eval.g:158:9: case_clause
                        pass 
                        self._state.following.append(self.FOLLOW_case_clause_in_case_block651)
                        self.case_clause()

                        self._state.following.pop()


                    else:
                        if cnt11 >= 1:
                            break #loop11

                        eee = EarlyExitException(11, self.input)
                        raise eee

                    cnt11 += 1


                # Eval.g:158:23: ( default_clause )?
                alt12 = 2
                LA12_0 = self.input.LA(1)

                if (LA12_0 == DEFAULT) :
                    alt12 = 1
                if alt12 == 1:
                    # Eval.g:158:24: default_clause
                    pass 
                    self._state.following.append(self.FOLLOW_default_clause_in_case_block656)
                    self.default_clause()

                    self._state.following.pop()




                self.match(self.input, 136, self.FOLLOW_136_in_case_block660)




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "case_block"



    # $ANTLR start "case_clause"
    # Eval.g:160:1: case_clause : ^( CASE ( case_test )+ ( stmt )* break_stmt ) ;
    def case_clause(self, ):
        self.cpy.op_case_enter()
        try:
            try:
                # Eval.g:162:2: ( ^( CASE ( case_test )+ ( stmt )* break_stmt ) )
                # Eval.g:162:4: ^( CASE ( case_test )+ ( stmt )* break_stmt )
                pass 
                self.match(self.input, CASE, self.FOLLOW_CASE_in_case_clause676)

                self.match(self.input, DOWN, None)
                # Eval.g:162:11: ( case_test )+
                cnt13 = 0
                while True: #loop13
                    alt13 = 2
                    LA13_0 = self.input.LA(1)

                    if (LA13_0 == CASE) :
                        alt13 = 1


                    if alt13 == 1:
                        # Eval.g:162:11: case_test
                        pass 
                        self._state.following.append(self.FOLLOW_case_test_in_case_clause678)
                        self.case_test()

                        self._state.following.pop()


                    else:
                        if cnt13 >= 1:
                            break #loop13

                        eee = EarlyExitException(13, self.input)
                        raise eee

                    cnt13 += 1


                #action start
                self.cpy.op_case()
                #action end


                # Eval.g:162:43: ( stmt )*
                while True: #loop14
                    alt14 = 2
                    LA14_0 = self.input.LA(1)

                    if (LA14_0 == BREAK) :
                        LA14_1 = self.input.LA(2)

                        if (LA14_1 == BREAK or LA14_1 == CLASS or LA14_1 == CONTINUE or LA14_1 == DO_WHILE or LA14_1 == EXEC_STMT or (FOR <= LA14_1 <= FUNCTION) or (IF <= LA14_1 <= IMPORT) or (PRINT <= LA14_1 <= RETURN) or (SWITCH <= LA14_1 <= TRY) or LA14_1 == WHILE) :
                            alt14 = 1


                    elif (LA14_0 == CLASS or LA14_0 == CONTINUE or LA14_0 == DO_WHILE or LA14_0 == EXEC_STMT or (FOR <= LA14_0 <= FUNCTION) or (IF <= LA14_0 <= IMPORT) or (PRINT <= LA14_0 <= RETURN) or (SWITCH <= LA14_0 <= TRY) or LA14_0 == WHILE) :
                        alt14 = 1


                    if alt14 == 1:
                        # Eval.g:162:43: stmt
                        pass 
                        self._state.following.append(self.FOLLOW_stmt_in_case_clause683)
                        self.stmt()

                        self._state.following.pop()


                    else:
                        break #loop14


                self._state.following.append(self.FOLLOW_break_stmt_in_case_clause686)
                self.break_stmt()

                self._state.following.pop()

                self.match(self.input, UP, None)


                #action start
                self.cpy.op_case_leave()
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "case_clause"



    # $ANTLR start "case_test"
    # Eval.g:165:1: case_test : ^( CASE expr ) ;
    def case_test(self, ):
        expr18 = None


        try:
            try:
                # Eval.g:166:2: ( ^( CASE expr ) )
                # Eval.g:166:4: ^( CASE expr )
                pass 
                self.match(self.input, CASE, self.FOLLOW_CASE_in_case_test702)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_expr_in_case_test704)
                expr18 = self.expr()

                self._state.following.pop()

                self.match(self.input, UP, None)


                #action start
                self.cpy.op_case_test(expr18)
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "case_test"



    # $ANTLR start "default_clause"
    # Eval.g:169:1: default_clause : ^( DEFAULT ( stmt )* ) ;
    def default_clause(self, ):
              
        self.cpy.op_default_enter()
        	
        try:
            try:
                # Eval.g:173:2: ( ^( DEFAULT ( stmt )* ) )
                # Eval.g:173:4: ^( DEFAULT ( stmt )* )
                pass 
                self.match(self.input, DEFAULT, self.FOLLOW_DEFAULT_in_default_clause725)

                if self.input.LA(1) == DOWN:
                    self.match(self.input, DOWN, None)
                    # Eval.g:173:14: ( stmt )*
                    while True: #loop15
                        alt15 = 2
                        LA15_0 = self.input.LA(1)

                        if (LA15_0 == BREAK or LA15_0 == CLASS or LA15_0 == CONTINUE or LA15_0 == DO_WHILE or LA15_0 == EXEC_STMT or (FOR <= LA15_0 <= FUNCTION) or (IF <= LA15_0 <= IMPORT) or (PRINT <= LA15_0 <= RETURN) or (SWITCH <= LA15_0 <= TRY) or LA15_0 == WHILE) :
                            alt15 = 1


                        if alt15 == 1:
                            # Eval.g:173:14: stmt
                            pass 
                            self._state.following.append(self.FOLLOW_stmt_in_default_clause727)
                            self.stmt()

                            self._state.following.pop()


                        else:
                            break #loop15


                    self.match(self.input, UP, None)



                #action start
                self.cpy.op_default_leave()
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "default_clause"



    # $ANTLR start "for_stmt"
    # Eval.g:178:1: for_stmt : ^( FOR (a= exec_list )? expr block (b= exec_list )? ) ;
    def for_stmt(self, ):
        a = None

        b = None

        expr19 = None


        try:
            try:
                # Eval.g:179:2: ( ^( FOR (a= exec_list )? expr block (b= exec_list )? ) )
                # Eval.g:179:4: ^( FOR (a= exec_list )? expr block (b= exec_list )? )
                pass 
                self.match(self.input, FOR, self.FOLLOW_FOR_in_for_stmt746)

                self.match(self.input, DOWN, None)
                # Eval.g:179:10: (a= exec_list )?
                alt16 = 2
                LA16_0 = self.input.LA(1)

                if (LA16_0 == EXEC_LIST) :
                    alt16 = 1
                if alt16 == 1:
                    # Eval.g:179:11: a= exec_list
                    pass 
                    self._state.following.append(self.FOLLOW_exec_list_in_for_stmt751)
                    a = self.exec_list()

                    self._state.following.pop()

                    #action start
                    self.cpy.stmt(a)
                    #action end





                self._state.following.append(self.FOLLOW_expr_in_for_stmt759)
                expr19 = self.expr()

                self._state.following.pop()

                #action start
                self.cpy.op_while(expr19)
                #action end


                self._state.following.append(self.FOLLOW_block_in_for_stmt765)
                self.block()

                self._state.following.pop()

                #action start
                self.cpy.block_enter()
                #action end


                # Eval.g:183:3: (b= exec_list )?
                alt17 = 2
                LA17_0 = self.input.LA(1)

                if (LA17_0 == EXEC_LIST) :
                    alt17 = 1
                if alt17 == 1:
                    # Eval.g:183:4: b= exec_list
                    pass 
                    self._state.following.append(self.FOLLOW_exec_list_in_for_stmt776)
                    b = self.exec_list()

                    self._state.following.pop()

                    #action start
                    self.cpy.stmt(b)
                    #action end





                #action start
                self.cpy.block_leave()
                #action end


                self.match(self.input, UP, None)





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "for_stmt"



    # $ANTLR start "foreach_stmt"
    # Eval.g:188:1: foreach_stmt : ^( FOREACH expr ( ^( EACH k= ID v= each_val ) | ^( EACH v= each_val ) ) block ) ;
    def foreach_stmt(self, ):
        k = None
        v = None

        expr20 = None


        try:
            try:
                # Eval.g:189:2: ( ^( FOREACH expr ( ^( EACH k= ID v= each_val ) | ^( EACH v= each_val ) ) block ) )
                # Eval.g:189:4: ^( FOREACH expr ( ^( EACH k= ID v= each_val ) | ^( EACH v= each_val ) ) block )
                pass 
                self.match(self.input, FOREACH, self.FOLLOW_FOREACH_in_foreach_stmt800)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_expr_in_foreach_stmt802)
                expr20 = self.expr()

                self._state.following.pop()

                # Eval.g:190:3: ( ^( EACH k= ID v= each_val ) | ^( EACH v= each_val ) )
                alt18 = 2
                LA18_0 = self.input.LA(1)

                if (LA18_0 == EACH) :
                    LA18_1 = self.input.LA(2)

                    if (LA18_1 == 2) :
                        LA18_2 = self.input.LA(3)

                        if (LA18_2 == ID) :
                            alt18 = 1
                        elif (LA18_2 == EACH_VAL) :
                            alt18 = 2
                        else:
                            nvae = NoViableAltException("", 18, 2, self.input)

                            raise nvae


                    else:
                        nvae = NoViableAltException("", 18, 1, self.input)

                        raise nvae


                else:
                    nvae = NoViableAltException("", 18, 0, self.input)

                    raise nvae


                if alt18 == 1:
                    # Eval.g:190:5: ^( EACH k= ID v= each_val )
                    pass 
                    self.match(self.input, EACH, self.FOLLOW_EACH_in_foreach_stmt809)

                    self.match(self.input, DOWN, None)
                    k = self.match(self.input, ID, self.FOLLOW_ID_in_foreach_stmt813)

                    self._state.following.append(self.FOLLOW_each_val_in_foreach_stmt817)
                    v = self.each_val()

                    self._state.following.pop()

                    self.match(self.input, UP, None)


                    #action start
                    self.cpy.op_foreach(expr20, k.text, v)
                    #action end



                elif alt18 == 2:
                    # Eval.g:192:5: ^( EACH v= each_val )
                    pass 
                    self.match(self.input, EACH, self.FOLLOW_EACH_in_foreach_stmt830)

                    self.match(self.input, DOWN, None)
                    self._state.following.append(self.FOLLOW_each_val_in_foreach_stmt834)
                    v = self.each_val()

                    self._state.following.pop()

                    self.match(self.input, UP, None)


                    #action start
                    self.cpy.op_foreach(expr20, None, v)
                    #action end





                self._state.following.append(self.FOLLOW_block_in_foreach_stmt848)
                self.block()

                self._state.following.pop()

                self.match(self.input, UP, None)





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "foreach_stmt"



    # $ANTLR start "each_val"
    # Eval.g:198:1: each_val returns [text] : ^( EACH_VAL ( ID )+ ) ;
    def each_val(self, ):
        text = None


        ID21 = None

        ps = []
        try:
            try:
                # Eval.g:200:2: ( ^( EACH_VAL ( ID )+ ) )
                # Eval.g:200:4: ^( EACH_VAL ( ID )+ )
                pass 
                self.match(self.input, EACH_VAL, self.FOLLOW_EACH_VAL_in_each_val871)

                self.match(self.input, DOWN, None)
                # Eval.g:200:15: ( ID )+
                cnt19 = 0
                while True: #loop19
                    alt19 = 2
                    LA19_0 = self.input.LA(1)

                    if (LA19_0 == ID) :
                        alt19 = 1


                    if alt19 == 1:
                        # Eval.g:200:16: ID
                        pass 
                        ID21 = self.match(self.input, ID, self.FOLLOW_ID_in_each_val874)

                        #action start
                        ps.append(ID21.text)
                        #action end



                    else:
                        if cnt19 >= 1:
                            break #loop19

                        eee = EarlyExitException(19, self.input)
                        raise eee

                    cnt19 += 1


                self.match(self.input, UP, None)


                #action start
                text = ','.join(ps)
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "each_val"



    # $ANTLR start "throw_stmt"
    # Eval.g:205:1: throw_stmt : ^( THROW expr ) ;
    def throw_stmt(self, ):
        expr22 = None


        try:
            try:
                # Eval.g:206:2: ( ^( THROW expr ) )
                # Eval.g:206:4: ^( THROW expr )
                pass 
                self.match(self.input, THROW, self.FOLLOW_THROW_in_throw_stmt897)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_expr_in_throw_stmt899)
                expr22 = self.expr()

                self._state.following.pop()

                self.match(self.input, UP, None)


                #action start
                self.cpy.op_throw(expr22)
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "throw_stmt"



    # $ANTLR start "try_stmt"
    # Eval.g:209:1: try_stmt : ^( TRY block ( catch_clause )+ ( finally_clause )? ) ;
    def try_stmt(self, ):
        self.cpy.op_try()
        try:
            try:
                # Eval.g:211:2: ( ^( TRY block ( catch_clause )+ ( finally_clause )? ) )
                # Eval.g:211:4: ^( TRY block ( catch_clause )+ ( finally_clause )? )
                pass 
                self.match(self.input, TRY, self.FOLLOW_TRY_in_try_stmt920)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_block_in_try_stmt922)
                self.block()

                self._state.following.pop()

                # Eval.g:211:16: ( catch_clause )+
                cnt20 = 0
                while True: #loop20
                    alt20 = 2
                    LA20_0 = self.input.LA(1)

                    if (LA20_0 == CATCH) :
                        alt20 = 1


                    if alt20 == 1:
                        # Eval.g:211:16: catch_clause
                        pass 
                        self._state.following.append(self.FOLLOW_catch_clause_in_try_stmt924)
                        self.catch_clause()

                        self._state.following.pop()


                    else:
                        if cnt20 >= 1:
                            break #loop20

                        eee = EarlyExitException(20, self.input)
                        raise eee

                    cnt20 += 1


                # Eval.g:211:30: ( finally_clause )?
                alt21 = 2
                LA21_0 = self.input.LA(1)

                if (LA21_0 == FINALLY) :
                    alt21 = 1
                if alt21 == 1:
                    # Eval.g:211:30: finally_clause
                    pass 
                    self._state.following.append(self.FOLLOW_finally_clause_in_try_stmt927)
                    self.finally_clause()

                    self._state.following.pop()




                self.match(self.input, UP, None)





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "try_stmt"



    # $ANTLR start "catch_clause"
    # Eval.g:213:1: catch_clause : ^( CATCH module ( ID )? block ) ;
    def catch_clause(self, ):
        ID24 = None
        module23 = None


        try:
            try:
                # Eval.g:214:2: ( ^( CATCH module ( ID )? block ) )
                # Eval.g:214:4: ^( CATCH module ( ID )? block )
                pass 
                self.match(self.input, CATCH, self.FOLLOW_CATCH_in_catch_clause940)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_module_in_catch_clause942)
                module23 = self.module()

                self._state.following.pop()

                # Eval.g:214:19: ( ID )?
                alt22 = 2
                LA22_0 = self.input.LA(1)

                if (LA22_0 == ID) :
                    alt22 = 1
                if alt22 == 1:
                    # Eval.g:214:19: ID
                    pass 
                    ID24 = self.match(self.input, ID, self.FOLLOW_ID_in_catch_clause944)




                #action start
                self.cpy.op_catch(module23, ID24.text)
                #action end


                self._state.following.append(self.FOLLOW_block_in_catch_clause953)
                self.block()

                self._state.following.pop()

                self.match(self.input, UP, None)





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "catch_clause"



    # $ANTLR start "finally_clause"
    # Eval.g:218:1: finally_clause : ^( FINALLY block ) ;
    def finally_clause(self, ):
        self.cpy.op_finally()
        try:
            try:
                # Eval.g:220:2: ( ^( FINALLY block ) )
                # Eval.g:220:4: ^( FINALLY block )
                pass 
                self.match(self.input, FINALLY, self.FOLLOW_FINALLY_in_finally_clause970)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_block_in_finally_clause972)
                self.block()

                self._state.following.pop()

                self.match(self.input, UP, None)





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "finally_clause"



    # $ANTLR start "func_decl"
    # Eval.g:224:1: func_decl : ^( FUNCTION ID params block ) ;
    def func_decl(self, ):
        ID25 = None
        params26 = None


        try:
            try:
                # Eval.g:225:2: ( ^( FUNCTION ID params block ) )
                # Eval.g:225:4: ^( FUNCTION ID params block )
                pass 
                self.match(self.input, FUNCTION, self.FOLLOW_FUNCTION_in_func_decl986)

                self.match(self.input, DOWN, None)
                ID25 = self.match(self.input, ID, self.FOLLOW_ID_in_func_decl988)

                self._state.following.append(self.FOLLOW_params_in_func_decl990)
                params26 = self.params()

                self._state.following.pop()

                #action start
                self.cpy.op_function(ID25.text, params26)
                #action end


                self._state.following.append(self.FOLLOW_block_in_func_decl998)
                self.block()

                self._state.following.pop()

                self.match(self.input, UP, None)





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "func_decl"



    # $ANTLR start "params"
    # Eval.g:230:1: params returns [text] : ^( PARAMS ( param_decl )* ) ;
    def params(self, ):
        text = None


        param_decl27 = None


        ps = []
        try:
            try:
                # Eval.g:232:2: ( ^( PARAMS ( param_decl )* ) )
                # Eval.g:232:4: ^( PARAMS ( param_decl )* )
                pass 
                self.match(self.input, PARAMS, self.FOLLOW_PARAMS_in_params1021)

                if self.input.LA(1) == DOWN:
                    self.match(self.input, DOWN, None)
                    # Eval.g:232:13: ( param_decl )*
                    while True: #loop23
                        alt23 = 2
                        LA23_0 = self.input.LA(1)

                        if (LA23_0 == ID) :
                            alt23 = 1


                        if alt23 == 1:
                            # Eval.g:232:14: param_decl
                            pass 
                            self._state.following.append(self.FOLLOW_param_decl_in_params1024)
                            param_decl27 = self.param_decl()

                            self._state.following.pop()

                            #action start
                            ps.append(param_decl27)
                            #action end



                        else:
                            break #loop23


                    self.match(self.input, UP, None)



                #action start
                text = ', '.join(ps)
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "params"



    # $ANTLR start "param_decl"
    # Eval.g:235:1: param_decl returns [text] : ID ( '=' atom )? ;
    def param_decl(self, ):
        text = None


        ID28 = None
        atom29 = None


        try:
            try:
                # Eval.g:236:2: ( ID ( '=' atom )? )
                # Eval.g:236:4: ID ( '=' atom )?
                pass 
                ID28 = self.match(self.input, ID, self.FOLLOW_ID_in_param_decl1048)

                #action start
                text = ID28.text
                #action end


                # Eval.g:238:3: ( '=' atom )?
                alt24 = 2
                LA24_0 = self.input.LA(1)

                if (LA24_0 == 95) :
                    alt24 = 1
                if alt24 == 1:
                    # Eval.g:238:4: '=' atom
                    pass 
                    self.match(self.input, 95, self.FOLLOW_95_in_param_decl1057)

                    self._state.following.append(self.FOLLOW_atom_in_param_decl1059)
                    atom29 = self.atom()

                    self._state.following.pop()

                    #action start
                    text += ('=' + atom29)
                    #action end








            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "param_decl"



    # $ANTLR start "class_decl"
    # Eval.g:244:1: class_decl : ( ^( CLASS a= ID ( class_element )* ) | ^( CLASS b= ID c= ID ( class_element )* ) );
    def class_decl(self, ):
        a = None
        b = None
        c = None

        try:
            try:
                # Eval.g:246:2: ( ^( CLASS a= ID ( class_element )* ) | ^( CLASS b= ID c= ID ( class_element )* ) )
                alt27 = 2
                LA27_0 = self.input.LA(1)

                if (LA27_0 == CLASS) :
                    LA27_1 = self.input.LA(2)

                    if (LA27_1 == 2) :
                        LA27_2 = self.input.LA(3)

                        if (LA27_2 == ID) :
                            LA27_3 = self.input.LA(4)

                            if (LA27_3 == ID) :
                                alt27 = 2
                            elif (LA27_3 == 3 or LA27_3 == CONSTRUCTOR or LA27_3 == FUNCTION or LA27_3 == VAR) :
                                alt27 = 1
                            else:
                                nvae = NoViableAltException("", 27, 3, self.input)

                                raise nvae


                        else:
                            nvae = NoViableAltException("", 27, 2, self.input)

                            raise nvae


                    else:
                        nvae = NoViableAltException("", 27, 1, self.input)

                        raise nvae


                else:
                    nvae = NoViableAltException("", 27, 0, self.input)

                    raise nvae


                if alt27 == 1:
                    # Eval.g:246:4: ^( CLASS a= ID ( class_element )* )
                    pass 
                    self.match(self.input, CLASS, self.FOLLOW_CLASS_in_class_decl1087)

                    self.match(self.input, DOWN, None)
                    a = self.match(self.input, ID, self.FOLLOW_ID_in_class_decl1091)

                    #action start
                    self.cpy.op_class_enter(a.text, None)
                    #action end


                    # Eval.g:248:3: ( class_element )*
                    while True: #loop25
                        alt25 = 2
                        LA25_0 = self.input.LA(1)

                        if (LA25_0 == CONSTRUCTOR or LA25_0 == FUNCTION or LA25_0 == VAR) :
                            alt25 = 1


                        if alt25 == 1:
                            # Eval.g:248:3: class_element
                            pass 
                            self._state.following.append(self.FOLLOW_class_element_in_class_decl1100)
                            self.class_element()

                            self._state.following.pop()


                        else:
                            break #loop25


                    self.match(self.input, UP, None)



                elif alt27 == 2:
                    # Eval.g:249:4: ^( CLASS b= ID c= ID ( class_element )* )
                    pass 
                    self.match(self.input, CLASS, self.FOLLOW_CLASS_in_class_decl1108)

                    self.match(self.input, DOWN, None)
                    b = self.match(self.input, ID, self.FOLLOW_ID_in_class_decl1112)

                    c = self.match(self.input, ID, self.FOLLOW_ID_in_class_decl1116)

                    #action start
                    self.cpy.op_class_enter(b.text, c.text)
                    #action end


                    # Eval.g:251:3: ( class_element )*
                    while True: #loop26
                        alt26 = 2
                        LA26_0 = self.input.LA(1)

                        if (LA26_0 == CONSTRUCTOR or LA26_0 == FUNCTION or LA26_0 == VAR) :
                            alt26 = 1


                        if alt26 == 1:
                            # Eval.g:251:3: class_element
                            pass 
                            self._state.following.append(self.FOLLOW_class_element_in_class_decl1125)
                            self.class_element()

                            self._state.following.pop()


                        else:
                            break #loop26


                    self.match(self.input, UP, None)



                #action start
                self.cpy.op_class_leave()
                #action end


            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "class_decl"



    # $ANTLR start "class_element"
    # Eval.g:253:1: class_element : ( var_def | constructor | func_decl );
    def class_element(self, ):
        try:
            try:
                # Eval.g:254:2: ( var_def | constructor | func_decl )
                alt28 = 3
                LA28 = self.input.LA(1)
                if LA28 == VAR:
                    alt28 = 1
                elif LA28 == CONSTRUCTOR:
                    alt28 = 2
                elif LA28 == FUNCTION:
                    alt28 = 3
                else:
                    nvae = NoViableAltException("", 28, 0, self.input)

                    raise nvae


                if alt28 == 1:
                    # Eval.g:254:4: var_def
                    pass 
                    self._state.following.append(self.FOLLOW_var_def_in_class_element1137)
                    self.var_def()

                    self._state.following.pop()


                elif alt28 == 2:
                    # Eval.g:254:14: constructor
                    pass 
                    self._state.following.append(self.FOLLOW_constructor_in_class_element1141)
                    self.constructor()

                    self._state.following.pop()


                elif alt28 == 3:
                    # Eval.g:254:28: func_decl
                    pass 
                    self._state.following.append(self.FOLLOW_func_decl_in_class_element1145)
                    self.func_decl()

                    self._state.following.pop()



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "class_element"



    # $ANTLR start "var_def"
    # Eval.g:256:1: var_def : ( ^( VAR ID ( expr )? ) | ^( VAR 'static' ID ( expr )? ) );
    def var_def(self, ):
        ID30 = None
        ID32 = None
        expr31 = None

        expr33 = None


        try:
            try:
                # Eval.g:257:2: ( ^( VAR ID ( expr )? ) | ^( VAR 'static' ID ( expr )? ) )
                alt31 = 2
                LA31_0 = self.input.LA(1)

                if (LA31_0 == VAR) :
                    LA31_1 = self.input.LA(2)

                    if (LA31_1 == 2) :
                        LA31_2 = self.input.LA(3)

                        if (LA31_2 == ID) :
                            alt31 = 1
                        elif (LA31_2 == 127) :
                            alt31 = 2
                        else:
                            nvae = NoViableAltException("", 31, 2, self.input)

                            raise nvae


                    else:
                        nvae = NoViableAltException("", 31, 1, self.input)

                        raise nvae


                else:
                    nvae = NoViableAltException("", 31, 0, self.input)

                    raise nvae


                if alt31 == 1:
                    # Eval.g:257:4: ^( VAR ID ( expr )? )
                    pass 
                    self.match(self.input, VAR, self.FOLLOW_VAR_in_var_def1156)

                    self.match(self.input, DOWN, None)
                    ID30 = self.match(self.input, ID, self.FOLLOW_ID_in_var_def1158)

                    # Eval.g:257:13: ( expr )?
                    alt29 = 2
                    LA29_0 = self.input.LA(1)

                    if (LA29_0 == ARRAY or LA29_0 == BOOL or LA29_0 == FLOAT or LA29_0 == INT or LA29_0 == MEMBER or (NEGATIVE <= LA29_0 <= NEW) or (NULL <= LA29_0 <= OBJECT) or (SPRINTF <= LA29_0 <= STRING) or (68 <= LA29_0 <= 70) or (72 <= LA29_0 <= 73) or LA29_0 == 77 or LA29_0 == 79 or LA29_0 == 83 or LA29_0 == 89 or (93 <= LA29_0 <= 94) or LA29_0 == 96 or (98 <= LA29_0 <= 99) or LA29_0 == 102 or LA29_0 == 133 or LA29_0 == 135) :
                        alt29 = 1
                    if alt29 == 1:
                        # Eval.g:257:13: expr
                        pass 
                        self._state.following.append(self.FOLLOW_expr_in_var_def1160)
                        expr31 = self.expr()

                        self._state.following.pop()




                    self.match(self.input, UP, None)


                    #action start
                    self.cpy.op_var_def(False, ID30.text, expr31)
                    #action end



                elif alt31 == 2:
                    # Eval.g:259:4: ^( VAR 'static' ID ( expr )? )
                    pass 
                    self.match(self.input, VAR, self.FOLLOW_VAR_in_var_def1172)

                    self.match(self.input, DOWN, None)
                    self.match(self.input, 127, self.FOLLOW_127_in_var_def1174)

                    ID32 = self.match(self.input, ID, self.FOLLOW_ID_in_var_def1176)

                    # Eval.g:259:22: ( expr )?
                    alt30 = 2
                    LA30_0 = self.input.LA(1)

                    if (LA30_0 == ARRAY or LA30_0 == BOOL or LA30_0 == FLOAT or LA30_0 == INT or LA30_0 == MEMBER or (NEGATIVE <= LA30_0 <= NEW) or (NULL <= LA30_0 <= OBJECT) or (SPRINTF <= LA30_0 <= STRING) or (68 <= LA30_0 <= 70) or (72 <= LA30_0 <= 73) or LA30_0 == 77 or LA30_0 == 79 or LA30_0 == 83 or LA30_0 == 89 or (93 <= LA30_0 <= 94) or LA30_0 == 96 or (98 <= LA30_0 <= 99) or LA30_0 == 102 or LA30_0 == 133 or LA30_0 == 135) :
                        alt30 = 1
                    if alt30 == 1:
                        # Eval.g:259:22: expr
                        pass 
                        self._state.following.append(self.FOLLOW_expr_in_var_def1178)
                        expr33 = self.expr()

                        self._state.following.pop()




                    self.match(self.input, UP, None)


                    #action start
                    self.cpy.op_var_def(True, ID32.text, expr33)
                    #action end




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "var_def"



    # $ANTLR start "constructor"
    # Eval.g:262:1: constructor : ^( CONSTRUCTOR params block ) ;
    def constructor(self, ):
        params34 = None


        try:
            try:
                # Eval.g:263:2: ( ^( CONSTRUCTOR params block ) )
                # Eval.g:263:4: ^( CONSTRUCTOR params block )
                pass 
                self.match(self.input, CONSTRUCTOR, self.FOLLOW_CONSTRUCTOR_in_constructor1195)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_params_in_constructor1197)
                params34 = self.params()

                self._state.following.pop()

                #action start
                self.cpy.op_construct(params34)
                #action end


                self._state.following.append(self.FOLLOW_block_in_constructor1205)
                self.block()

                self._state.following.pop()

                self.match(self.input, UP, None)





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return 

    # $ANTLR end "constructor"



    # $ANTLR start "module"
    # Eval.g:270:1: module returns [text] : ^( MODULE ( ID )+ ) ;
    def module(self, ):
        text = None


        ID35 = None

        ps = []
        try:
            try:
                # Eval.g:272:2: ( ^( MODULE ( ID )+ ) )
                # Eval.g:272:4: ^( MODULE ( ID )+ )
                pass 
                self.match(self.input, MODULE, self.FOLLOW_MODULE_in_module1229)

                self.match(self.input, DOWN, None)
                # Eval.g:272:13: ( ID )+
                cnt32 = 0
                while True: #loop32
                    alt32 = 2
                    LA32_0 = self.input.LA(1)

                    if (LA32_0 == ID) :
                        alt32 = 1


                    if alt32 == 1:
                        # Eval.g:272:14: ID
                        pass 
                        ID35 = self.match(self.input, ID, self.FOLLOW_ID_in_module1232)

                        #action start
                        ps.append(ID35.text)
                        #action end



                    else:
                        if cnt32 >= 1:
                            break #loop32

                        eee = EarlyExitException(32, self.input)
                        raise eee

                    cnt32 += 1


                self.match(self.input, UP, None)


                #action start
                text = '.'.join(ps)
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "module"



    # $ANTLR start "member_expr"
    # Eval.g:276:1: member_expr returns [text] : ^( MEMBER ( primary )+ ) ;
    def member_expr(self, ):
        text = None


        primary36 = None


        ps = []
        try:
            try:
                # Eval.g:278:2: ( ^( MEMBER ( primary )+ ) )
                # Eval.g:278:4: ^( MEMBER ( primary )+ )
                pass 
                self.match(self.input, MEMBER, self.FOLLOW_MEMBER_in_member_expr1263)

                self.match(self.input, DOWN, None)
                # Eval.g:278:13: ( primary )+
                cnt33 = 0
                while True: #loop33
                    alt33 = 2
                    LA33_0 = self.input.LA(1)

                    if (LA33_0 == ID) :
                        alt33 = 1


                    if alt33 == 1:
                        # Eval.g:278:14: primary
                        pass 
                        self._state.following.append(self.FOLLOW_primary_in_member_expr1266)
                        primary36 = self.primary()

                        self._state.following.pop()

                        #action start
                        ps.append(primary36)
                        #action end



                    else:
                        if cnt33 >= 1:
                            break #loop33

                        eee = EarlyExitException(33, self.input)
                        raise eee

                    cnt33 += 1


                self.match(self.input, UP, None)


                #action start
                text = '.'.join(ps)
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "member_expr"



    # $ANTLR start "primary"
    # Eval.g:281:1: primary returns [text] : ID ( index_expr )* ( call_expr )? ;
    def primary(self, ):
        text = None


        ID39 = None
        index_expr37 = None

        call_expr38 = None


        a=''
        try:
            try:
                # Eval.g:283:2: ( ID ( index_expr )* ( call_expr )? )
                # Eval.g:283:4: ID ( index_expr )* ( call_expr )?
                pass 
                ID39 = self.match(self.input, ID, self.FOLLOW_ID_in_primary1295)

                # Eval.g:283:7: ( index_expr )*
                while True: #loop34
                    alt34 = 2
                    LA34_0 = self.input.LA(1)

                    if (LA34_0 == INDEX or LA34_0 == SLICE) :
                        alt34 = 1


                    if alt34 == 1:
                        # Eval.g:283:8: index_expr
                        pass 
                        self._state.following.append(self.FOLLOW_index_expr_in_primary1298)
                        index_expr37 = self.index_expr()

                        self._state.following.pop()

                        #action start
                        a += index_expr37
                        #action end



                    else:
                        break #loop34


                # Eval.g:284:3: ( call_expr )?
                alt35 = 2
                LA35_0 = self.input.LA(1)

                if (LA35_0 == CALL) :
                    alt35 = 1
                if alt35 == 1:
                    # Eval.g:284:3: call_expr
                    pass 
                    self._state.following.append(self.FOLLOW_call_expr_in_primary1305)
                    call_expr38 = self.call_expr()

                    self._state.following.pop()




                #action start
                  
                b = call_expr38
                if b == None: b = ''
                text = ID39.text + a + b
                		
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "primary"



    # $ANTLR start "call_expr"
    # Eval.g:291:1: call_expr returns [text] : ^( CALL ( expr_list )? ) ;
    def call_expr(self, ):
        text = None


        expr_list40 = None


        try:
            try:
                # Eval.g:292:2: ( ^( CALL ( expr_list )? ) )
                # Eval.g:292:4: ^( CALL ( expr_list )? )
                pass 
                self.match(self.input, CALL, self.FOLLOW_CALL_in_call_expr1324)

                if self.input.LA(1) == DOWN:
                    self.match(self.input, DOWN, None)
                    # Eval.g:292:11: ( expr_list )?
                    alt36 = 2
                    LA36_0 = self.input.LA(1)

                    if (LA36_0 == EXPR_LIST) :
                        alt36 = 1
                    if alt36 == 1:
                        # Eval.g:292:11: expr_list
                        pass 
                        self._state.following.append(self.FOLLOW_expr_list_in_call_expr1326)
                        expr_list40 = self.expr_list()

                        self._state.following.pop()




                    self.match(self.input, UP, None)



                #action start
                  
                s = expr_list40
                if s == None: s = ''
                text = '(' + s + ')'
                		
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "call_expr"



    # $ANTLR start "index_expr"
    # Eval.g:299:1: index_expr returns [text] : ( ^( INDEX expr ) | ^( SLICE a= expr (b= expr )? ) );
    def index_expr(self, ):
        text = None


        a = None

        b = None

        expr41 = None


        try:
            try:
                # Eval.g:300:2: ( ^( INDEX expr ) | ^( SLICE a= expr (b= expr )? ) )
                alt38 = 2
                LA38_0 = self.input.LA(1)

                if (LA38_0 == INDEX) :
                    alt38 = 1
                elif (LA38_0 == SLICE) :
                    alt38 = 2
                else:
                    nvae = NoViableAltException("", 38, 0, self.input)

                    raise nvae


                if alt38 == 1:
                    # Eval.g:300:4: ^( INDEX expr )
                    pass 
                    self.match(self.input, INDEX, self.FOLLOW_INDEX_in_index_expr1346)

                    self.match(self.input, DOWN, None)
                    self._state.following.append(self.FOLLOW_expr_in_index_expr1348)
                    expr41 = self.expr()

                    self._state.following.pop()

                    self.match(self.input, UP, None)


                    #action start
                    text = '[' + expr41 + ']'
                    #action end



                elif alt38 == 2:
                    # Eval.g:302:4: ^( SLICE a= expr (b= expr )? )
                    pass 
                    self.match(self.input, SLICE, self.FOLLOW_SLICE_in_index_expr1359)

                    self.match(self.input, DOWN, None)
                    self._state.following.append(self.FOLLOW_expr_in_index_expr1363)
                    a = self.expr()

                    self._state.following.pop()

                    # Eval.g:302:20: (b= expr )?
                    alt37 = 2
                    LA37_0 = self.input.LA(1)

                    if (LA37_0 == ARRAY or LA37_0 == BOOL or LA37_0 == FLOAT or LA37_0 == INT or LA37_0 == MEMBER or (NEGATIVE <= LA37_0 <= NEW) or (NULL <= LA37_0 <= OBJECT) or (SPRINTF <= LA37_0 <= STRING) or (68 <= LA37_0 <= 70) or (72 <= LA37_0 <= 73) or LA37_0 == 77 or LA37_0 == 79 or LA37_0 == 83 or LA37_0 == 89 or (93 <= LA37_0 <= 94) or LA37_0 == 96 or (98 <= LA37_0 <= 99) or LA37_0 == 102 or LA37_0 == 133 or LA37_0 == 135) :
                        alt37 = 1
                    if alt37 == 1:
                        # Eval.g:302:20: b= expr
                        pass 
                        self._state.following.append(self.FOLLOW_expr_in_index_expr1367)
                        b = self.expr()

                        self._state.following.pop()




                    self.match(self.input, UP, None)


                    #action start
                      
                    s = b
                    if s == None: s = ''
                    text = '[%s : %s]' %(a, s)
                    		
                    #action end




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "index_expr"



    # $ANTLR start "expr_list"
    # Eval.g:311:1: expr_list returns [text] : ^( EXPR_LIST ( expr )+ ) ;
    def expr_list(self, ):
        text = None


        expr42 = None


        ps = []
        try:
            try:
                # Eval.g:313:2: ( ^( EXPR_LIST ( expr )+ ) )
                # Eval.g:313:4: ^( EXPR_LIST ( expr )+ )
                pass 
                self.match(self.input, EXPR_LIST, self.FOLLOW_EXPR_LIST_in_expr_list1394)

                self.match(self.input, DOWN, None)
                # Eval.g:313:16: ( expr )+
                cnt39 = 0
                while True: #loop39
                    alt39 = 2
                    LA39_0 = self.input.LA(1)

                    if (LA39_0 == ARRAY or LA39_0 == BOOL or LA39_0 == FLOAT or LA39_0 == INT or LA39_0 == MEMBER or (NEGATIVE <= LA39_0 <= NEW) or (NULL <= LA39_0 <= OBJECT) or (SPRINTF <= LA39_0 <= STRING) or (68 <= LA39_0 <= 70) or (72 <= LA39_0 <= 73) or LA39_0 == 77 or LA39_0 == 79 or LA39_0 == 83 or LA39_0 == 89 or (93 <= LA39_0 <= 94) or LA39_0 == 96 or (98 <= LA39_0 <= 99) or LA39_0 == 102 or LA39_0 == 133 or LA39_0 == 135) :
                        alt39 = 1


                    if alt39 == 1:
                        # Eval.g:313:17: expr
                        pass 
                        self._state.following.append(self.FOLLOW_expr_in_expr_list1397)
                        expr42 = self.expr()

                        self._state.following.pop()

                        #action start
                        ps.append(expr42)
                        #action end



                    else:
                        if cnt39 >= 1:
                            break #loop39

                        eee = EarlyExitException(39, self.input)
                        raise eee

                    cnt39 += 1


                self.match(self.input, UP, None)


                #action start
                  
                text = ', '.join(ps)
                		
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "expr_list"



    # $ANTLR start "expr"
    # Eval.g:319:1: expr returns [text] : (a= relation_expr |a= logic_or_expr |a= logic_and_expr |a= bitwise_or_expr |a= bitwise_xor_expr |a= bitwise_and_expr |a= add_expr |a= mul_expr |a= not_expr |a= negative_expr |a= atom );
    def expr(self, ):
        text = None


        a = None


        try:
            try:
                # Eval.g:320:2: (a= relation_expr |a= logic_or_expr |a= logic_and_expr |a= bitwise_or_expr |a= bitwise_xor_expr |a= bitwise_and_expr |a= add_expr |a= mul_expr |a= not_expr |a= negative_expr |a= atom )
                alt40 = 11
                LA40 = self.input.LA(1)
                if LA40 == 69 or LA40 == 93 or LA40 == 94 or LA40 == 96 or LA40 == 98 or LA40 == 99:
                    alt40 = 1
                elif LA40 == 135:
                    alt40 = 2
                elif LA40 == 72:
                    alt40 = 3
                elif LA40 == 133:
                    alt40 = 4
                elif LA40 == 102:
                    alt40 = 5
                elif LA40 == 73:
                    alt40 = 6
                elif LA40 == 79 or LA40 == 83:
                    alt40 = 7
                elif LA40 == 70 or LA40 == 77 or LA40 == 89:
                    alt40 = 8
                elif LA40 == 68:
                    alt40 = 9
                elif LA40 == NEGATIVE:
                    alt40 = 10
                elif LA40 == ARRAY or LA40 == BOOL or LA40 == FLOAT or LA40 == INT or LA40 == MEMBER or LA40 == NEW or LA40 == NULL or LA40 == OBJECT or LA40 == SPRINTF or LA40 == STRING:
                    alt40 = 11
                else:
                    nvae = NoViableAltException("", 40, 0, self.input)

                    raise nvae


                if alt40 == 1:
                    # Eval.g:320:4: a= relation_expr
                    pass 
                    self._state.following.append(self.FOLLOW_relation_expr_in_expr1423)
                    a = self.relation_expr()

                    self._state.following.pop()

                    #action start
                    text = a
                    #action end



                elif alt40 == 2:
                    # Eval.g:321:4: a= logic_or_expr
                    pass 
                    self._state.following.append(self.FOLLOW_logic_or_expr_in_expr1432)
                    a = self.logic_or_expr()

                    self._state.following.pop()

                    #action start
                    text = a
                    #action end



                elif alt40 == 3:
                    # Eval.g:322:4: a= logic_and_expr
                    pass 
                    self._state.following.append(self.FOLLOW_logic_and_expr_in_expr1441)
                    a = self.logic_and_expr()

                    self._state.following.pop()

                    #action start
                    text = a
                    #action end



                elif alt40 == 4:
                    # Eval.g:323:4: a= bitwise_or_expr
                    pass 
                    self._state.following.append(self.FOLLOW_bitwise_or_expr_in_expr1450)
                    a = self.bitwise_or_expr()

                    self._state.following.pop()

                    #action start
                    text = a
                    #action end



                elif alt40 == 5:
                    # Eval.g:324:4: a= bitwise_xor_expr
                    pass 
                    self._state.following.append(self.FOLLOW_bitwise_xor_expr_in_expr1459)
                    a = self.bitwise_xor_expr()

                    self._state.following.pop()

                    #action start
                    text = a
                    #action end



                elif alt40 == 6:
                    # Eval.g:325:4: a= bitwise_and_expr
                    pass 
                    self._state.following.append(self.FOLLOW_bitwise_and_expr_in_expr1468)
                    a = self.bitwise_and_expr()

                    self._state.following.pop()

                    #action start
                    text = a
                    #action end



                elif alt40 == 7:
                    # Eval.g:326:4: a= add_expr
                    pass 
                    self._state.following.append(self.FOLLOW_add_expr_in_expr1477)
                    a = self.add_expr()

                    self._state.following.pop()

                    #action start
                    text = a
                    #action end



                elif alt40 == 8:
                    # Eval.g:327:4: a= mul_expr
                    pass 
                    self._state.following.append(self.FOLLOW_mul_expr_in_expr1487)
                    a = self.mul_expr()

                    self._state.following.pop()

                    #action start
                    text = a
                    #action end



                elif alt40 == 9:
                    # Eval.g:328:4: a= not_expr
                    pass 
                    self._state.following.append(self.FOLLOW_not_expr_in_expr1497)
                    a = self.not_expr()

                    self._state.following.pop()

                    #action start
                    text = a
                    #action end



                elif alt40 == 10:
                    # Eval.g:329:4: a= negative_expr
                    pass 
                    self._state.following.append(self.FOLLOW_negative_expr_in_expr1507)
                    a = self.negative_expr()

                    self._state.following.pop()

                    #action start
                    text = a
                    #action end



                elif alt40 == 11:
                    # Eval.g:330:4: a= atom
                    pass 
                    self._state.following.append(self.FOLLOW_atom_in_expr1516)
                    a = self.atom()

                    self._state.following.pop()

                    #action start
                    text = a
                    #action end




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "expr"



    # $ANTLR start "logic_or_expr"
    # Eval.g:332:1: logic_or_expr returns [text] : ^( '||' b= expr c= expr ) ;
    def logic_or_expr(self, ):
        text = None


        b = None

        c = None


        try:
            try:
                # Eval.g:333:2: ( ^( '||' b= expr c= expr ) )
                # Eval.g:333:4: ^( '||' b= expr c= expr )
                pass 
                self.match(self.input, 135, self.FOLLOW_135_in_logic_or_expr1534)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_expr_in_logic_or_expr1538)
                b = self.expr()

                self._state.following.pop()

                self._state.following.append(self.FOLLOW_expr_in_logic_or_expr1542)
                c = self.expr()

                self._state.following.pop()

                self.match(self.input, UP, None)


                #action start
                text = '(' + b + ' or ' + c + ')'
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "logic_or_expr"



    # $ANTLR start "logic_and_expr"
    # Eval.g:336:1: logic_and_expr returns [text] : ^( '&&' b= expr c= expr ) ;
    def logic_and_expr(self, ):
        text = None


        b = None

        c = None


        try:
            try:
                # Eval.g:337:2: ( ^( '&&' b= expr c= expr ) )
                # Eval.g:337:4: ^( '&&' b= expr c= expr )
                pass 
                self.match(self.input, 72, self.FOLLOW_72_in_logic_and_expr1561)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_expr_in_logic_and_expr1565)
                b = self.expr()

                self._state.following.pop()

                self._state.following.append(self.FOLLOW_expr_in_logic_and_expr1569)
                c = self.expr()

                self._state.following.pop()

                self.match(self.input, UP, None)


                #action start
                text = b + ' and ' + c
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "logic_and_expr"



    # $ANTLR start "bitwise_or_expr"
    # Eval.g:340:1: bitwise_or_expr returns [text] : ^( '|' b= expr c= expr ) ;
    def bitwise_or_expr(self, ):
        text = None


        b = None

        c = None


        try:
            try:
                # Eval.g:341:2: ( ^( '|' b= expr c= expr ) )
                # Eval.g:341:4: ^( '|' b= expr c= expr )
                pass 
                self.match(self.input, 133, self.FOLLOW_133_in_bitwise_or_expr1588)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_expr_in_bitwise_or_expr1592)
                b = self.expr()

                self._state.following.pop()

                self._state.following.append(self.FOLLOW_expr_in_bitwise_or_expr1596)
                c = self.expr()

                self._state.following.pop()

                self.match(self.input, UP, None)


                #action start
                text = b + ' | ' + c
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "bitwise_or_expr"



    # $ANTLR start "bitwise_xor_expr"
    # Eval.g:344:1: bitwise_xor_expr returns [text] : ^( '^' b= expr c= expr ) ;
    def bitwise_xor_expr(self, ):
        text = None


        b = None

        c = None


        try:
            try:
                # Eval.g:345:2: ( ^( '^' b= expr c= expr ) )
                # Eval.g:345:4: ^( '^' b= expr c= expr )
                pass 
                self.match(self.input, 102, self.FOLLOW_102_in_bitwise_xor_expr1615)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_expr_in_bitwise_xor_expr1619)
                b = self.expr()

                self._state.following.pop()

                self._state.following.append(self.FOLLOW_expr_in_bitwise_xor_expr1623)
                c = self.expr()

                self._state.following.pop()

                self.match(self.input, UP, None)


                #action start
                text = b + ' ^ ' + c
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "bitwise_xor_expr"



    # $ANTLR start "bitwise_and_expr"
    # Eval.g:348:1: bitwise_and_expr returns [text] : ^( '&' b= expr c= expr ) ;
    def bitwise_and_expr(self, ):
        text = None


        b = None

        c = None


        try:
            try:
                # Eval.g:349:2: ( ^( '&' b= expr c= expr ) )
                # Eval.g:349:4: ^( '&' b= expr c= expr )
                pass 
                self.match(self.input, 73, self.FOLLOW_73_in_bitwise_and_expr1642)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_expr_in_bitwise_and_expr1646)
                b = self.expr()

                self._state.following.pop()

                self._state.following.append(self.FOLLOW_expr_in_bitwise_and_expr1650)
                c = self.expr()

                self._state.following.pop()

                self.match(self.input, UP, None)


                #action start
                text = b + ' & ' + c
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "bitwise_and_expr"



    # $ANTLR start "relation_expr"
    # Eval.g:352:1: relation_expr returns [text] : ^(op= ( '<' | '>' | '<=' | '>=' | '==' | '!=' ) b= expr c= expr ) ;
    def relation_expr(self, ):
        text = None


        op = None
        b = None

        c = None


        try:
            try:
                # Eval.g:353:2: ( ^(op= ( '<' | '>' | '<=' | '>=' | '==' | '!=' ) b= expr c= expr ) )
                # Eval.g:353:4: ^(op= ( '<' | '>' | '<=' | '>=' | '==' | '!=' ) b= expr c= expr )
                pass 
                op = self.input.LT(1)

                if self.input.LA(1) == 69 or (93 <= self.input.LA(1) <= 94) or self.input.LA(1) == 96 or (98 <= self.input.LA(1) <= 99):
                    self.input.consume()
                    self._state.errorRecovery = False


                else:
                    mse = MismatchedSetException(None, self.input)
                    raise mse



                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_expr_in_relation_expr1687)
                b = self.expr()

                self._state.following.pop()

                self._state.following.append(self.FOLLOW_expr_in_relation_expr1691)
                c = self.expr()

                self._state.following.pop()

                self.match(self.input, UP, None)


                #action start
                text = b + op.text + c
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "relation_expr"



    # $ANTLR start "add_expr"
    # Eval.g:356:1: add_expr returns [text] : ^(op= ( '+' | '-' ) b= expr c= expr ) ;
    def add_expr(self, ):
        text = None


        op = None
        b = None

        c = None


        try:
            try:
                # Eval.g:357:2: ( ^(op= ( '+' | '-' ) b= expr c= expr ) )
                # Eval.g:357:4: ^(op= ( '+' | '-' ) b= expr c= expr )
                pass 
                op = self.input.LT(1)

                if self.input.LA(1) == 79 or self.input.LA(1) == 83:
                    self.input.consume()
                    self._state.errorRecovery = False


                else:
                    mse = MismatchedSetException(None, self.input)
                    raise mse



                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_expr_in_add_expr1720)
                b = self.expr()

                self._state.following.pop()

                self._state.following.append(self.FOLLOW_expr_in_add_expr1724)
                c = self.expr()

                self._state.following.pop()

                self.match(self.input, UP, None)


                #action start
                text = '(' + b + ' ' + op.text + ' ' + c + ')'
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "add_expr"



    # $ANTLR start "mul_expr"
    # Eval.g:360:1: mul_expr returns [text] : ^(op= ( '*' | '/' | '%' ) b= expr c= expr ) ;
    def mul_expr(self, ):
        text = None


        op = None
        b = None

        c = None


        try:
            try:
                # Eval.g:361:2: ( ^(op= ( '*' | '/' | '%' ) b= expr c= expr ) )
                # Eval.g:361:4: ^(op= ( '*' | '/' | '%' ) b= expr c= expr )
                pass 
                op = self.input.LT(1)

                if self.input.LA(1) == 70 or self.input.LA(1) == 77 or self.input.LA(1) == 89:
                    self.input.consume()
                    self._state.errorRecovery = False


                else:
                    mse = MismatchedSetException(None, self.input)
                    raise mse



                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_expr_in_mul_expr1755)
                b = self.expr()

                self._state.following.pop()

                self._state.following.append(self.FOLLOW_expr_in_mul_expr1759)
                c = self.expr()

                self._state.following.pop()

                self.match(self.input, UP, None)


                #action start
                text = b + ' ' + op.text + ' ' + c
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "mul_expr"



    # $ANTLR start "not_expr"
    # Eval.g:364:1: not_expr returns [text] : ^( '!' a= expr ) ;
    def not_expr(self, ):
        text = None


        a = None


        try:
            try:
                # Eval.g:365:2: ( ^( '!' a= expr ) )
                # Eval.g:365:4: ^( '!' a= expr )
                pass 
                self.match(self.input, 68, self.FOLLOW_68_in_not_expr1778)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_expr_in_not_expr1782)
                a = self.expr()

                self._state.following.pop()

                self.match(self.input, UP, None)


                #action start
                text = 'not (' + a + ')'
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "not_expr"



    # $ANTLR start "negative_expr"
    # Eval.g:368:1: negative_expr returns [text] : ^( NEGATIVE a= expr ) ;
    def negative_expr(self, ):
        text = None


        a = None


        try:
            try:
                # Eval.g:369:2: ( ^( NEGATIVE a= expr ) )
                # Eval.g:369:4: ^( NEGATIVE a= expr )
                pass 
                self.match(self.input, NEGATIVE, self.FOLLOW_NEGATIVE_in_negative_expr1801)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_expr_in_negative_expr1805)
                a = self.expr()

                self._state.following.pop()

                self.match(self.input, UP, None)


                #action start
                text = '- (' + a + ')'
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "negative_expr"



    # $ANTLR start "sprintf"
    # Eval.g:374:1: sprintf returns [text] : ^( SPRINTF expr (a= expr_list )? ) ;
    def sprintf(self, ):
        text = None


        a = None

        expr43 = None


        try:
            try:
                # Eval.g:375:2: ( ^( SPRINTF expr (a= expr_list )? ) )
                # Eval.g:375:4: ^( SPRINTF expr (a= expr_list )? )
                pass 
                self.match(self.input, SPRINTF, self.FOLLOW_SPRINTF_in_sprintf1826)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_expr_in_sprintf1828)
                expr43 = self.expr()

                self._state.following.pop()

                # Eval.g:375:20: (a= expr_list )?
                alt41 = 2
                LA41_0 = self.input.LA(1)

                if (LA41_0 == EXPR_LIST) :
                    alt41 = 1
                if alt41 == 1:
                    # Eval.g:375:20: a= expr_list
                    pass 
                    self._state.following.append(self.FOLLOW_expr_list_in_sprintf1832)
                    a = self.expr_list()

                    self._state.following.pop()




                self.match(self.input, UP, None)


                #action start
                  
                s = a
                if not s: s=''
                text = expr43 + '%(' + s + ')'
                		
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "sprintf"



    # $ANTLR start "new_clause"
    # Eval.g:383:1: new_clause returns [text] : ^( NEW module call_expr ) ;
    def new_clause(self, ):
        text = None


        module44 = None

        call_expr45 = None


        try:
            try:
                # Eval.g:384:2: ( ^( NEW module call_expr ) )
                # Eval.g:384:4: ^( NEW module call_expr )
                pass 
                self.match(self.input, NEW, self.FOLLOW_NEW_in_new_clause1853)

                self.match(self.input, DOWN, None)
                self._state.following.append(self.FOLLOW_module_in_new_clause1855)
                module44 = self.module()

                self._state.following.pop()

                self._state.following.append(self.FOLLOW_call_expr_in_new_clause1857)
                call_expr45 = self.call_expr()

                self._state.following.pop()

                self.match(self.input, UP, None)


                #action start
                text = module44 + call_expr45
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "new_clause"



    # $ANTLR start "array_decl"
    # Eval.g:388:1: array_decl returns [text] : ^( ARRAY ( expr_list )? ) ;
    def array_decl(self, ):
        text = None


        expr_list46 = None


        try:
            try:
                # Eval.g:389:2: ( ^( ARRAY ( expr_list )? ) )
                # Eval.g:389:4: ^( ARRAY ( expr_list )? )
                pass 
                self.match(self.input, ARRAY, self.FOLLOW_ARRAY_in_array_decl1877)

                if self.input.LA(1) == DOWN:
                    self.match(self.input, DOWN, None)
                    # Eval.g:389:12: ( expr_list )?
                    alt42 = 2
                    LA42_0 = self.input.LA(1)

                    if (LA42_0 == EXPR_LIST) :
                        alt42 = 1
                    if alt42 == 1:
                        # Eval.g:389:12: expr_list
                        pass 
                        self._state.following.append(self.FOLLOW_expr_list_in_array_decl1879)
                        expr_list46 = self.expr_list()

                        self._state.following.pop()




                    self.match(self.input, UP, None)



                #action start
                  
                s = expr_list46
                if s == None: s = ''
                text = '[' + s + ']'
                		
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "array_decl"



    # $ANTLR start "object_decl"
    # Eval.g:396:1: object_decl returns [text] : ^( OBJECT ( property )* ) ;
    def object_decl(self, ):
        text = None


        property47 = None


        s = ''
        try:
            try:
                # Eval.g:398:2: ( ^( OBJECT ( property )* ) )
                # Eval.g:398:4: ^( OBJECT ( property )* )
                pass 
                self.match(self.input, OBJECT, self.FOLLOW_OBJECT_in_object_decl1904)

                if self.input.LA(1) == DOWN:
                    self.match(self.input, DOWN, None)
                    # Eval.g:398:13: ( property )*
                    while True: #loop43
                        alt43 = 2
                        LA43_0 = self.input.LA(1)

                        if (LA43_0 == ID or LA43_0 == INT or LA43_0 == STRING) :
                            alt43 = 1


                        if alt43 == 1:
                            # Eval.g:398:14: property
                            pass 
                            self._state.following.append(self.FOLLOW_property_in_object_decl1907)
                            property47 = self.property()

                            self._state.following.pop()

                            #action start
                            s += property47
                            #action end



                        else:
                            break #loop43


                    self.match(self.input, UP, None)



                #action start
                text = '{' + s + '}'
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "object_decl"



    # $ANTLR start "property"
    # Eval.g:401:1: property returns [text] : a= ( ID | STRING | INT ) ':' expr ;
    def property(self, ):
        text = None


        a = None
        expr48 = None


        try:
            try:
                # Eval.g:402:2: (a= ( ID | STRING | INT ) ':' expr )
                # Eval.g:402:4: a= ( ID | STRING | INT ) ':' expr
                pass 
                a = self.input.LT(1)

                if self.input.LA(1) == ID or self.input.LA(1) == INT or self.input.LA(1) == STRING:
                    self.input.consume()
                    self._state.errorRecovery = False


                else:
                    mse = MismatchedSetException(None, self.input)
                    raise mse



                self.match(self.input, 91, self.FOLLOW_91_in_property1944)

                self._state.following.append(self.FOLLOW_expr_in_property1946)
                expr48 = self.expr()

                self._state.following.pop()

                #action start
                text = a.text + ': ' + expr48 + ','
                #action end





            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "property"



    # $ANTLR start "atom"
    # Eval.g:407:1: atom returns [text] : (a= literal |a= member_expr |a= new_clause |a= array_decl |a= object_decl |a= sprintf );
    def atom(self, ):
        text = None


        a = None


        try:
            try:
                # Eval.g:408:2: (a= literal |a= member_expr |a= new_clause |a= array_decl |a= object_decl |a= sprintf )
                alt44 = 6
                LA44 = self.input.LA(1)
                if LA44 == BOOL or LA44 == FLOAT or LA44 == INT or LA44 == NULL or LA44 == STRING:
                    alt44 = 1
                elif LA44 == MEMBER:
                    alt44 = 2
                elif LA44 == NEW:
                    alt44 = 3
                elif LA44 == ARRAY:
                    alt44 = 4
                elif LA44 == OBJECT:
                    alt44 = 5
                elif LA44 == SPRINTF:
                    alt44 = 6
                else:
                    nvae = NoViableAltException("", 44, 0, self.input)

                    raise nvae


                if alt44 == 1:
                    # Eval.g:408:4: a= literal
                    pass 
                    self._state.following.append(self.FOLLOW_literal_in_atom1967)
                    a = self.literal()

                    self._state.following.pop()

                    #action start
                    text = a
                    #action end



                elif alt44 == 2:
                    # Eval.g:409:4: a= member_expr
                    pass 
                    self._state.following.append(self.FOLLOW_member_expr_in_atom1977)
                    a = self.member_expr()

                    self._state.following.pop()

                    #action start
                    text = a
                    #action end



                elif alt44 == 3:
                    # Eval.g:410:4: a= new_clause
                    pass 
                    self._state.following.append(self.FOLLOW_new_clause_in_atom1986)
                    a = self.new_clause()

                    self._state.following.pop()

                    #action start
                    text = a
                    #action end



                elif alt44 == 4:
                    # Eval.g:411:4: a= array_decl
                    pass 
                    self._state.following.append(self.FOLLOW_array_decl_in_atom1995)
                    a = self.array_decl()

                    self._state.following.pop()

                    #action start
                    text = a
                    #action end



                elif alt44 == 5:
                    # Eval.g:412:4: a= object_decl
                    pass 
                    self._state.following.append(self.FOLLOW_object_decl_in_atom2004)
                    a = self.object_decl()

                    self._state.following.pop()

                    #action start
                    text = a
                    #action end



                elif alt44 == 6:
                    # Eval.g:413:4: a= sprintf
                    pass 
                    self._state.following.append(self.FOLLOW_sprintf_in_atom2013)
                    a = self.sprintf()

                    self._state.following.pop()

                    #action start
                    text = a
                    #action end




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "atom"



    # $ANTLR start "literal"
    # Eval.g:415:1: literal returns [text] : ( NULL | BOOL | INT | FLOAT | STRING );
    def literal(self, ):
        text = None


        BOOL49 = None
        INT50 = None
        FLOAT51 = None
        STRING52 = None

        try:
            try:
                # Eval.g:416:2: ( NULL | BOOL | INT | FLOAT | STRING )
                alt45 = 5
                LA45 = self.input.LA(1)
                if LA45 == NULL:
                    alt45 = 1
                elif LA45 == BOOL:
                    alt45 = 2
                elif LA45 == INT:
                    alt45 = 3
                elif LA45 == FLOAT:
                    alt45 = 4
                elif LA45 == STRING:
                    alt45 = 5
                else:
                    nvae = NoViableAltException("", 45, 0, self.input)

                    raise nvae


                if alt45 == 1:
                    # Eval.g:416:4: NULL
                    pass 
                    self.match(self.input, NULL, self.FOLLOW_NULL_in_literal2029)

                    #action start
                    text = 'None'
                    #action end



                elif alt45 == 2:
                    # Eval.g:417:4: BOOL
                    pass 
                    BOOL49 = self.match(self.input, BOOL, self.FOLLOW_BOOL_in_literal2036)

                    #action start
                    text = BOOL49.text.capitalize()
                    #action end



                elif alt45 == 3:
                    # Eval.g:418:4: INT
                    pass 
                    INT50 = self.match(self.input, INT, self.FOLLOW_INT_in_literal2043)

                    #action start
                    text = INT50.text
                    #action end



                elif alt45 == 4:
                    # Eval.g:419:4: FLOAT
                    pass 
                    FLOAT51 = self.match(self.input, FLOAT, self.FOLLOW_FLOAT_in_literal2050)

                    #action start
                    text = FLOAT51.text
                    #action end



                elif alt45 == 5:
                    # Eval.g:420:4: STRING
                    pass 
                    STRING52 = self.match(self.input, STRING, self.FOLLOW_STRING_in_literal2057)

                    #action start
                    text = STRING52.text
                    #action end




            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)

        finally:
            pass
        return text

    # $ANTLR end "literal"



    # lookup tables for DFA #4

    DFA4_eot = DFA.unpack(
        u"\10\uffff"
        )

    DFA4_eof = DFA.unpack(
        u"\10\uffff"
        )

    DFA4_min = DFA.unpack(
        u"\1\3\1\uffff\1\2\1\42\2\3\2\uffff"
        )

    DFA4_max = DFA.unpack(
        u"\1\52\1\uffff\1\2\2\42\1\127\2\uffff"
        )

    DFA4_accept = DFA.unpack(
        u"\1\uffff\1\3\4\uffff\1\1\1\2"
        )

    DFA4_special = DFA.unpack(
        u"\10\uffff"
        )


    DFA4_transition = [
        DFA.unpack(u"\1\1\46\uffff\1\2"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\3"),
        DFA.unpack(u"\1\4"),
        DFA.unpack(u"\1\5\36\uffff\1\4"),
        DFA.unpack(u"\1\6\46\uffff\1\6\54\uffff\1\7"),
        DFA.unpack(u""),
        DFA.unpack(u"")
    ]

    # class definition for DFA #4

    class DFA4(DFA):
        pass


 

    FOLLOW_stmt_in_prog69 = frozenset([1, 9, 13, 16, 20, 27, 31, 32, 33, 36, 37, 55, 56, 57, 62, 63, 64, 66])
    FOLLOW_import_stmt_in_stmt81 = frozenset([1])
    FOLLOW_exec_stmt_in_stmt86 = frozenset([1])
    FOLLOW_print_stmt_in_stmt91 = frozenset([1])
    FOLLOW_printf_stmt_in_stmt95 = frozenset([1])
    FOLLOW_break_stmt_in_stmt100 = frozenset([1])
    FOLLOW_continue_stmt_in_stmt105 = frozenset([1])
    FOLLOW_return_stmt_in_stmt110 = frozenset([1])
    FOLLOW_if_stmt_in_stmt115 = frozenset([1])
    FOLLOW_while_stmt_in_stmt120 = frozenset([1])
    FOLLOW_do_while_stmt_in_stmt125 = frozenset([1])
    FOLLOW_switch_stmt_in_stmt130 = frozenset([1])
    FOLLOW_throw_stmt_in_stmt135 = frozenset([1])
    FOLLOW_try_stmt_in_stmt140 = frozenset([1])
    FOLLOW_func_decl_in_stmt145 = frozenset([1])
    FOLLOW_class_decl_in_stmt150 = frozenset([1])
    FOLLOW_for_stmt_in_stmt155 = frozenset([1])
    FOLLOW_foreach_stmt_in_stmt160 = frozenset([1])
    FOLLOW_BLOCK_in_block185 = frozenset([2])
    FOLLOW_stmt_in_block187 = frozenset([3, 9, 13, 16, 20, 27, 31, 32, 33, 36, 37, 55, 56, 57, 62, 63, 64, 66])
    FOLLOW_IMPORT_in_import_stmt201 = frozenset([2])
    FOLLOW_module_in_import_stmt209 = frozenset([3, 42])
    FOLLOW_module_in_import_stmt222 = frozenset([87])
    FOLLOW_87_in_import_stmt224 = frozenset([3, 42])
    FOLLOW_EXEC_STMT_in_exec_stmt250 = frozenset([2])
    FOLLOW_exec_list_in_exec_stmt252 = frozenset([3])
    FOLLOW_member_expr_in_exec_expr270 = frozenset([1])
    FOLLOW_ASSIGN_in_exec_expr280 = frozenset([2])
    FOLLOW_member_expr_in_exec_expr282 = frozenset([71, 74, 78, 81, 85, 90, 95, 103, 134])
    FOLLOW_set_in_exec_expr286 = frozenset([5, 8, 30, 39, 41, 43, 44, 47, 48, 60, 61, 68, 69, 70, 72, 73, 77, 79, 83, 89, 93, 94, 96, 98, 99, 102, 133, 135])
    FOLLOW_expr_in_exec_expr306 = frozenset([3])
    FOLLOW_POST_INC_in_exec_expr317 = frozenset([2])
    FOLLOW_member_expr_in_exec_expr319 = frozenset([3])
    FOLLOW_POST_DEC_in_exec_expr330 = frozenset([2])
    FOLLOW_member_expr_in_exec_expr332 = frozenset([3])
    FOLLOW_PRE_INC_in_exec_expr343 = frozenset([2])
    FOLLOW_member_expr_in_exec_expr345 = frozenset([3])
    FOLLOW_PRE_DEC_in_exec_expr356 = frozenset([2])
    FOLLOW_member_expr_in_exec_expr358 = frozenset([3])
    FOLLOW_EXEC_LIST_in_exec_list382 = frozenset([2])
    FOLLOW_exec_expr_in_exec_list385 = frozenset([3, 6, 41, 51, 52, 53, 54])
    FOLLOW_PRINTF_in_printf_stmt408 = frozenset([2])
    FOLLOW_expr_in_printf_stmt410 = frozenset([3, 28])
    FOLLOW_expr_list_in_printf_stmt412 = frozenset([3])
    FOLLOW_PRINT_in_print_stmt429 = frozenset([2])
    FOLLOW_expr_list_in_print_stmt431 = frozenset([3])
    FOLLOW_BREAK_in_break_stmt451 = frozenset([1])
    FOLLOW_CONTINUE_in_continue_stmt465 = frozenset([1])
    FOLLOW_RETURN_in_return_stmt480 = frozenset([2])
    FOLLOW_expr_in_return_stmt482 = frozenset([3])
    FOLLOW_if_clause_in_if_stmt510 = frozenset([1, 23, 24])
    FOLLOW_else_if_clause_in_if_stmt512 = frozenset([1, 23, 24])
    FOLLOW_else_clause_in_if_stmt515 = frozenset([1])
    FOLLOW_IF_in_if_clause527 = frozenset([2])
    FOLLOW_expr_in_if_clause529 = frozenset([7])
    FOLLOW_block_in_if_clause533 = frozenset([3])
    FOLLOW_ELSE_IF_in_else_if_clause545 = frozenset([2])
    FOLLOW_if_clause_in_else_if_clause549 = frozenset([3])
    FOLLOW_ELSE_in_else_clause561 = frozenset([2])
    FOLLOW_block_in_else_clause565 = frozenset([3])
    FOLLOW_WHILE_in_while_stmt579 = frozenset([2])
    FOLLOW_expr_in_while_stmt581 = frozenset([7])
    FOLLOW_block_in_while_stmt585 = frozenset([3])
    FOLLOW_DO_WHILE_in_do_while_stmt598 = frozenset([2])
    FOLLOW_block_in_do_while_stmt604 = frozenset([5, 8, 30, 39, 41, 43, 44, 47, 48, 60, 61, 68, 69, 70, 72, 73, 77, 79, 83, 89, 93, 94, 96, 98, 99, 102, 133, 135])
    FOLLOW_expr_in_do_while_stmt608 = frozenset([3])
    FOLLOW_SWITCH_in_switch_stmt627 = frozenset([2])
    FOLLOW_expr_in_switch_stmt629 = frozenset([132])
    FOLLOW_case_block_in_switch_stmt633 = frozenset([3])
    FOLLOW_132_in_case_block648 = frozenset([11])
    FOLLOW_case_clause_in_case_block651 = frozenset([11, 17, 136])
    FOLLOW_default_clause_in_case_block656 = frozenset([136])
    FOLLOW_136_in_case_block660 = frozenset([1])
    FOLLOW_CASE_in_case_clause676 = frozenset([2])
    FOLLOW_case_test_in_case_clause678 = frozenset([9, 11, 13, 16, 20, 27, 31, 32, 33, 36, 37, 55, 56, 57, 62, 63, 64, 66])
    FOLLOW_stmt_in_case_clause683 = frozenset([9, 13, 16, 20, 27, 31, 32, 33, 36, 37, 55, 56, 57, 62, 63, 64, 66])
    FOLLOW_break_stmt_in_case_clause686 = frozenset([3])
    FOLLOW_CASE_in_case_test702 = frozenset([2])
    FOLLOW_expr_in_case_test704 = frozenset([3])
    FOLLOW_DEFAULT_in_default_clause725 = frozenset([2])
    FOLLOW_stmt_in_default_clause727 = frozenset([3, 9, 13, 16, 20, 27, 31, 32, 33, 36, 37, 55, 56, 57, 62, 63, 64, 66])
    FOLLOW_FOR_in_for_stmt746 = frozenset([2])
    FOLLOW_exec_list_in_for_stmt751 = frozenset([5, 8, 30, 39, 41, 43, 44, 47, 48, 60, 61, 68, 69, 70, 72, 73, 77, 79, 83, 89, 93, 94, 96, 98, 99, 102, 133, 135])
    FOLLOW_expr_in_for_stmt759 = frozenset([7])
    FOLLOW_block_in_for_stmt765 = frozenset([3, 26])
    FOLLOW_exec_list_in_for_stmt776 = frozenset([3])
    FOLLOW_FOREACH_in_foreach_stmt800 = frozenset([2])
    FOLLOW_expr_in_foreach_stmt802 = frozenset([21])
    FOLLOW_EACH_in_foreach_stmt809 = frozenset([2])
    FOLLOW_ID_in_foreach_stmt813 = frozenset([22])
    FOLLOW_each_val_in_foreach_stmt817 = frozenset([3])
    FOLLOW_EACH_in_foreach_stmt830 = frozenset([2])
    FOLLOW_each_val_in_foreach_stmt834 = frozenset([3])
    FOLLOW_block_in_foreach_stmt848 = frozenset([3])
    FOLLOW_EACH_VAL_in_each_val871 = frozenset([2])
    FOLLOW_ID_in_each_val874 = frozenset([3, 34])
    FOLLOW_THROW_in_throw_stmt897 = frozenset([2])
    FOLLOW_expr_in_throw_stmt899 = frozenset([3])
    FOLLOW_TRY_in_try_stmt920 = frozenset([2])
    FOLLOW_block_in_try_stmt922 = frozenset([12])
    FOLLOW_catch_clause_in_try_stmt924 = frozenset([3, 12, 29])
    FOLLOW_finally_clause_in_try_stmt927 = frozenset([3])
    FOLLOW_CATCH_in_catch_clause940 = frozenset([2])
    FOLLOW_module_in_catch_clause942 = frozenset([7, 34])
    FOLLOW_ID_in_catch_clause944 = frozenset([7])
    FOLLOW_block_in_catch_clause953 = frozenset([3])
    FOLLOW_FINALLY_in_finally_clause970 = frozenset([2])
    FOLLOW_block_in_finally_clause972 = frozenset([3])
    FOLLOW_FUNCTION_in_func_decl986 = frozenset([2])
    FOLLOW_ID_in_func_decl988 = frozenset([50])
    FOLLOW_params_in_func_decl990 = frozenset([7])
    FOLLOW_block_in_func_decl998 = frozenset([3])
    FOLLOW_PARAMS_in_params1021 = frozenset([2])
    FOLLOW_param_decl_in_params1024 = frozenset([3, 34])
    FOLLOW_ID_in_param_decl1048 = frozenset([1, 95])
    FOLLOW_95_in_param_decl1057 = frozenset([5, 8, 30, 39, 41, 44, 47, 48, 60, 61])
    FOLLOW_atom_in_param_decl1059 = frozenset([1])
    FOLLOW_CLASS_in_class_decl1087 = frozenset([2])
    FOLLOW_ID_in_class_decl1091 = frozenset([3, 15, 33, 65])
    FOLLOW_class_element_in_class_decl1100 = frozenset([3, 15, 33, 65])
    FOLLOW_CLASS_in_class_decl1108 = frozenset([2])
    FOLLOW_ID_in_class_decl1112 = frozenset([34])
    FOLLOW_ID_in_class_decl1116 = frozenset([3, 15, 33, 65])
    FOLLOW_class_element_in_class_decl1125 = frozenset([3, 15, 33, 65])
    FOLLOW_var_def_in_class_element1137 = frozenset([1])
    FOLLOW_constructor_in_class_element1141 = frozenset([1])
    FOLLOW_func_decl_in_class_element1145 = frozenset([1])
    FOLLOW_VAR_in_var_def1156 = frozenset([2])
    FOLLOW_ID_in_var_def1158 = frozenset([3, 5, 8, 30, 39, 41, 43, 44, 47, 48, 60, 61, 68, 69, 70, 72, 73, 77, 79, 83, 89, 93, 94, 96, 98, 99, 102, 133, 135])
    FOLLOW_expr_in_var_def1160 = frozenset([3])
    FOLLOW_VAR_in_var_def1172 = frozenset([2])
    FOLLOW_127_in_var_def1174 = frozenset([34])
    FOLLOW_ID_in_var_def1176 = frozenset([3, 5, 8, 30, 39, 41, 43, 44, 47, 48, 60, 61, 68, 69, 70, 72, 73, 77, 79, 83, 89, 93, 94, 96, 98, 99, 102, 133, 135])
    FOLLOW_expr_in_var_def1178 = frozenset([3])
    FOLLOW_CONSTRUCTOR_in_constructor1195 = frozenset([2])
    FOLLOW_params_in_constructor1197 = frozenset([7])
    FOLLOW_block_in_constructor1205 = frozenset([3])
    FOLLOW_MODULE_in_module1229 = frozenset([2])
    FOLLOW_ID_in_module1232 = frozenset([3, 34])
    FOLLOW_MEMBER_in_member_expr1263 = frozenset([2])
    FOLLOW_primary_in_member_expr1266 = frozenset([3, 34])
    FOLLOW_ID_in_primary1295 = frozenset([1, 10, 38, 59])
    FOLLOW_index_expr_in_primary1298 = frozenset([1, 10, 38, 59])
    FOLLOW_call_expr_in_primary1305 = frozenset([1])
    FOLLOW_CALL_in_call_expr1324 = frozenset([2])
    FOLLOW_expr_list_in_call_expr1326 = frozenset([3])
    FOLLOW_INDEX_in_index_expr1346 = frozenset([2])
    FOLLOW_expr_in_index_expr1348 = frozenset([3])
    FOLLOW_SLICE_in_index_expr1359 = frozenset([2])
    FOLLOW_expr_in_index_expr1363 = frozenset([3, 5, 8, 30, 39, 41, 43, 44, 47, 48, 60, 61, 68, 69, 70, 72, 73, 77, 79, 83, 89, 93, 94, 96, 98, 99, 102, 133, 135])
    FOLLOW_expr_in_index_expr1367 = frozenset([3])
    FOLLOW_EXPR_LIST_in_expr_list1394 = frozenset([2])
    FOLLOW_expr_in_expr_list1397 = frozenset([3, 5, 8, 30, 39, 41, 43, 44, 47, 48, 60, 61, 68, 69, 70, 72, 73, 77, 79, 83, 89, 93, 94, 96, 98, 99, 102, 133, 135])
    FOLLOW_relation_expr_in_expr1423 = frozenset([1])
    FOLLOW_logic_or_expr_in_expr1432 = frozenset([1])
    FOLLOW_logic_and_expr_in_expr1441 = frozenset([1])
    FOLLOW_bitwise_or_expr_in_expr1450 = frozenset([1])
    FOLLOW_bitwise_xor_expr_in_expr1459 = frozenset([1])
    FOLLOW_bitwise_and_expr_in_expr1468 = frozenset([1])
    FOLLOW_add_expr_in_expr1477 = frozenset([1])
    FOLLOW_mul_expr_in_expr1487 = frozenset([1])
    FOLLOW_not_expr_in_expr1497 = frozenset([1])
    FOLLOW_negative_expr_in_expr1507 = frozenset([1])
    FOLLOW_atom_in_expr1516 = frozenset([1])
    FOLLOW_135_in_logic_or_expr1534 = frozenset([2])
    FOLLOW_expr_in_logic_or_expr1538 = frozenset([5, 8, 30, 39, 41, 43, 44, 47, 48, 60, 61, 68, 69, 70, 72, 73, 77, 79, 83, 89, 93, 94, 96, 98, 99, 102, 133, 135])
    FOLLOW_expr_in_logic_or_expr1542 = frozenset([3])
    FOLLOW_72_in_logic_and_expr1561 = frozenset([2])
    FOLLOW_expr_in_logic_and_expr1565 = frozenset([5, 8, 30, 39, 41, 43, 44, 47, 48, 60, 61, 68, 69, 70, 72, 73, 77, 79, 83, 89, 93, 94, 96, 98, 99, 102, 133, 135])
    FOLLOW_expr_in_logic_and_expr1569 = frozenset([3])
    FOLLOW_133_in_bitwise_or_expr1588 = frozenset([2])
    FOLLOW_expr_in_bitwise_or_expr1592 = frozenset([5, 8, 30, 39, 41, 43, 44, 47, 48, 60, 61, 68, 69, 70, 72, 73, 77, 79, 83, 89, 93, 94, 96, 98, 99, 102, 133, 135])
    FOLLOW_expr_in_bitwise_or_expr1596 = frozenset([3])
    FOLLOW_102_in_bitwise_xor_expr1615 = frozenset([2])
    FOLLOW_expr_in_bitwise_xor_expr1619 = frozenset([5, 8, 30, 39, 41, 43, 44, 47, 48, 60, 61, 68, 69, 70, 72, 73, 77, 79, 83, 89, 93, 94, 96, 98, 99, 102, 133, 135])
    FOLLOW_expr_in_bitwise_xor_expr1623 = frozenset([3])
    FOLLOW_73_in_bitwise_and_expr1642 = frozenset([2])
    FOLLOW_expr_in_bitwise_and_expr1646 = frozenset([5, 8, 30, 39, 41, 43, 44, 47, 48, 60, 61, 68, 69, 70, 72, 73, 77, 79, 83, 89, 93, 94, 96, 98, 99, 102, 133, 135])
    FOLLOW_expr_in_bitwise_and_expr1650 = frozenset([3])
    FOLLOW_set_in_relation_expr1671 = frozenset([2])
    FOLLOW_expr_in_relation_expr1687 = frozenset([5, 8, 30, 39, 41, 43, 44, 47, 48, 60, 61, 68, 69, 70, 72, 73, 77, 79, 83, 89, 93, 94, 96, 98, 99, 102, 133, 135])
    FOLLOW_expr_in_relation_expr1691 = frozenset([3])
    FOLLOW_set_in_add_expr1712 = frozenset([2])
    FOLLOW_expr_in_add_expr1720 = frozenset([5, 8, 30, 39, 41, 43, 44, 47, 48, 60, 61, 68, 69, 70, 72, 73, 77, 79, 83, 89, 93, 94, 96, 98, 99, 102, 133, 135])
    FOLLOW_expr_in_add_expr1724 = frozenset([3])
    FOLLOW_set_in_mul_expr1745 = frozenset([2])
    FOLLOW_expr_in_mul_expr1755 = frozenset([5, 8, 30, 39, 41, 43, 44, 47, 48, 60, 61, 68, 69, 70, 72, 73, 77, 79, 83, 89, 93, 94, 96, 98, 99, 102, 133, 135])
    FOLLOW_expr_in_mul_expr1759 = frozenset([3])
    FOLLOW_68_in_not_expr1778 = frozenset([2])
    FOLLOW_expr_in_not_expr1782 = frozenset([3])
    FOLLOW_NEGATIVE_in_negative_expr1801 = frozenset([2])
    FOLLOW_expr_in_negative_expr1805 = frozenset([3])
    FOLLOW_SPRINTF_in_sprintf1826 = frozenset([2])
    FOLLOW_expr_in_sprintf1828 = frozenset([3, 28])
    FOLLOW_expr_list_in_sprintf1832 = frozenset([3])
    FOLLOW_NEW_in_new_clause1853 = frozenset([2])
    FOLLOW_module_in_new_clause1855 = frozenset([10])
    FOLLOW_call_expr_in_new_clause1857 = frozenset([3])
    FOLLOW_ARRAY_in_array_decl1877 = frozenset([2])
    FOLLOW_expr_list_in_array_decl1879 = frozenset([3])
    FOLLOW_OBJECT_in_object_decl1904 = frozenset([2])
    FOLLOW_property_in_object_decl1907 = frozenset([3, 34, 39, 61])
    FOLLOW_set_in_property1932 = frozenset([91])
    FOLLOW_91_in_property1944 = frozenset([5, 8, 30, 39, 41, 43, 44, 47, 48, 60, 61, 68, 69, 70, 72, 73, 77, 79, 83, 89, 93, 94, 96, 98, 99, 102, 133, 135])
    FOLLOW_expr_in_property1946 = frozenset([1])
    FOLLOW_literal_in_atom1967 = frozenset([1])
    FOLLOW_member_expr_in_atom1977 = frozenset([1])
    FOLLOW_new_clause_in_atom1986 = frozenset([1])
    FOLLOW_array_decl_in_atom1995 = frozenset([1])
    FOLLOW_object_decl_in_atom2004 = frozenset([1])
    FOLLOW_sprintf_in_atom2013 = frozenset([1])
    FOLLOW_NULL_in_literal2029 = frozenset([1])
    FOLLOW_BOOL_in_literal2036 = frozenset([1])
    FOLLOW_INT_in_literal2043 = frozenset([1])
    FOLLOW_FLOAT_in_literal2050 = frozenset([1])
    FOLLOW_STRING_in_literal2057 = frozenset([1])



def main(argv, stdin=sys.stdin, stdout=sys.stdout, stderr=sys.stderr):
    from antlr3.main import WalkerMain
    main = WalkerMain(Eval)

    main.stdin = stdin
    main.stdout = stdout
    main.stderr = stderr
    main.execute(argv)



if __name__ == '__main__':
    main(sys.argv)
