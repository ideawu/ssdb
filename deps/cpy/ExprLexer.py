# $ANTLR 3.5 Expr.g 2013-04-12 19:22:24

import sys
from antlr3 import *
from antlr3.compat import set, frozenset



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


class ExprLexer(Lexer):

    grammarFileName = "Expr.g"
    api_version = 1

    def __init__(self, input=None, state=None):
        if state is None:
            state = RecognizerSharedState()
        super(ExprLexer, self).__init__(input, state)

        self.delegates = []

        self.dfa15 = self.DFA15(
            self, 15,
            eot = self.DFA15_eot,
            eof = self.DFA15_eof,
            min = self.DFA15_min,
            max = self.DFA15_max,
            accept = self.DFA15_accept,
            special = self.DFA15_special,
            transition = self.DFA15_transition
            )






    # $ANTLR start "T__68"
    def mT__68(self, ):
        try:
            _type = T__68
            _channel = DEFAULT_CHANNEL

            # Expr.g:7:7: ( '!' )
            # Expr.g:7:9: '!'
            pass 
            self.match(33)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__68"



    # $ANTLR start "T__69"
    def mT__69(self, ):
        try:
            _type = T__69
            _channel = DEFAULT_CHANNEL

            # Expr.g:8:7: ( '!=' )
            # Expr.g:8:9: '!='
            pass 
            self.match("!=")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__69"



    # $ANTLR start "T__70"
    def mT__70(self, ):
        try:
            _type = T__70
            _channel = DEFAULT_CHANNEL

            # Expr.g:9:7: ( '%' )
            # Expr.g:9:9: '%'
            pass 
            self.match(37)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__70"



    # $ANTLR start "T__71"
    def mT__71(self, ):
        try:
            _type = T__71
            _channel = DEFAULT_CHANNEL

            # Expr.g:10:7: ( '%=' )
            # Expr.g:10:9: '%='
            pass 
            self.match("%=")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__71"



    # $ANTLR start "T__72"
    def mT__72(self, ):
        try:
            _type = T__72
            _channel = DEFAULT_CHANNEL

            # Expr.g:11:7: ( '&&' )
            # Expr.g:11:9: '&&'
            pass 
            self.match("&&")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__72"



    # $ANTLR start "T__73"
    def mT__73(self, ):
        try:
            _type = T__73
            _channel = DEFAULT_CHANNEL

            # Expr.g:12:7: ( '&' )
            # Expr.g:12:9: '&'
            pass 
            self.match(38)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__73"



    # $ANTLR start "T__74"
    def mT__74(self, ):
        try:
            _type = T__74
            _channel = DEFAULT_CHANNEL

            # Expr.g:13:7: ( '&=' )
            # Expr.g:13:9: '&='
            pass 
            self.match("&=")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__74"



    # $ANTLR start "T__75"
    def mT__75(self, ):
        try:
            _type = T__75
            _channel = DEFAULT_CHANNEL

            # Expr.g:14:7: ( '(' )
            # Expr.g:14:9: '('
            pass 
            self.match(40)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__75"



    # $ANTLR start "T__76"
    def mT__76(self, ):
        try:
            _type = T__76
            _channel = DEFAULT_CHANNEL

            # Expr.g:15:7: ( ')' )
            # Expr.g:15:9: ')'
            pass 
            self.match(41)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__76"



    # $ANTLR start "T__77"
    def mT__77(self, ):
        try:
            _type = T__77
            _channel = DEFAULT_CHANNEL

            # Expr.g:16:7: ( '*' )
            # Expr.g:16:9: '*'
            pass 
            self.match(42)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__77"



    # $ANTLR start "T__78"
    def mT__78(self, ):
        try:
            _type = T__78
            _channel = DEFAULT_CHANNEL

            # Expr.g:17:7: ( '*=' )
            # Expr.g:17:9: '*='
            pass 
            self.match("*=")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__78"



    # $ANTLR start "T__79"
    def mT__79(self, ):
        try:
            _type = T__79
            _channel = DEFAULT_CHANNEL

            # Expr.g:18:7: ( '+' )
            # Expr.g:18:9: '+'
            pass 
            self.match(43)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__79"



    # $ANTLR start "T__80"
    def mT__80(self, ):
        try:
            _type = T__80
            _channel = DEFAULT_CHANNEL

            # Expr.g:19:7: ( '++' )
            # Expr.g:19:9: '++'
            pass 
            self.match("++")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__80"



    # $ANTLR start "T__81"
    def mT__81(self, ):
        try:
            _type = T__81
            _channel = DEFAULT_CHANNEL

            # Expr.g:20:7: ( '+=' )
            # Expr.g:20:9: '+='
            pass 
            self.match("+=")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__81"



    # $ANTLR start "T__82"
    def mT__82(self, ):
        try:
            _type = T__82
            _channel = DEFAULT_CHANNEL

            # Expr.g:21:7: ( ',' )
            # Expr.g:21:9: ','
            pass 
            self.match(44)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__82"



    # $ANTLR start "T__83"
    def mT__83(self, ):
        try:
            _type = T__83
            _channel = DEFAULT_CHANNEL

            # Expr.g:22:7: ( '-' )
            # Expr.g:22:9: '-'
            pass 
            self.match(45)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__83"



    # $ANTLR start "T__84"
    def mT__84(self, ):
        try:
            _type = T__84
            _channel = DEFAULT_CHANNEL

            # Expr.g:23:7: ( '--' )
            # Expr.g:23:9: '--'
            pass 
            self.match("--")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__84"



    # $ANTLR start "T__85"
    def mT__85(self, ):
        try:
            _type = T__85
            _channel = DEFAULT_CHANNEL

            # Expr.g:24:7: ( '-=' )
            # Expr.g:24:9: '-='
            pass 
            self.match("-=")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__85"



    # $ANTLR start "T__86"
    def mT__86(self, ):
        try:
            _type = T__86
            _channel = DEFAULT_CHANNEL

            # Expr.g:25:7: ( '.' )
            # Expr.g:25:9: '.'
            pass 
            self.match(46)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__86"



    # $ANTLR start "T__87"
    def mT__87(self, ):
        try:
            _type = T__87
            _channel = DEFAULT_CHANNEL

            # Expr.g:26:7: ( '.*' )
            # Expr.g:26:9: '.*'
            pass 
            self.match(".*")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__87"



    # $ANTLR start "T__88"
    def mT__88(self, ):
        try:
            _type = T__88
            _channel = DEFAULT_CHANNEL

            # Expr.g:27:7: ( '..' )
            # Expr.g:27:9: '..'
            pass 
            self.match("..")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__88"



    # $ANTLR start "T__89"
    def mT__89(self, ):
        try:
            _type = T__89
            _channel = DEFAULT_CHANNEL

            # Expr.g:28:7: ( '/' )
            # Expr.g:28:9: '/'
            pass 
            self.match(47)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__89"



    # $ANTLR start "T__90"
    def mT__90(self, ):
        try:
            _type = T__90
            _channel = DEFAULT_CHANNEL

            # Expr.g:29:7: ( '/=' )
            # Expr.g:29:9: '/='
            pass 
            self.match("/=")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__90"



    # $ANTLR start "T__91"
    def mT__91(self, ):
        try:
            _type = T__91
            _channel = DEFAULT_CHANNEL

            # Expr.g:30:7: ( ':' )
            # Expr.g:30:9: ':'
            pass 
            self.match(58)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__91"



    # $ANTLR start "T__92"
    def mT__92(self, ):
        try:
            _type = T__92
            _channel = DEFAULT_CHANNEL

            # Expr.g:31:7: ( ';' )
            # Expr.g:31:9: ';'
            pass 
            self.match(59)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__92"



    # $ANTLR start "T__93"
    def mT__93(self, ):
        try:
            _type = T__93
            _channel = DEFAULT_CHANNEL

            # Expr.g:32:7: ( '<' )
            # Expr.g:32:9: '<'
            pass 
            self.match(60)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__93"



    # $ANTLR start "T__94"
    def mT__94(self, ):
        try:
            _type = T__94
            _channel = DEFAULT_CHANNEL

            # Expr.g:33:7: ( '<=' )
            # Expr.g:33:9: '<='
            pass 
            self.match("<=")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__94"



    # $ANTLR start "T__95"
    def mT__95(self, ):
        try:
            _type = T__95
            _channel = DEFAULT_CHANNEL

            # Expr.g:34:7: ( '=' )
            # Expr.g:34:9: '='
            pass 
            self.match(61)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__95"



    # $ANTLR start "T__96"
    def mT__96(self, ):
        try:
            _type = T__96
            _channel = DEFAULT_CHANNEL

            # Expr.g:35:7: ( '==' )
            # Expr.g:35:9: '=='
            pass 
            self.match("==")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__96"



    # $ANTLR start "T__97"
    def mT__97(self, ):
        try:
            _type = T__97
            _channel = DEFAULT_CHANNEL

            # Expr.g:36:7: ( '=>' )
            # Expr.g:36:9: '=>'
            pass 
            self.match("=>")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__97"



    # $ANTLR start "T__98"
    def mT__98(self, ):
        try:
            _type = T__98
            _channel = DEFAULT_CHANNEL

            # Expr.g:37:7: ( '>' )
            # Expr.g:37:9: '>'
            pass 
            self.match(62)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__98"



    # $ANTLR start "T__99"
    def mT__99(self, ):
        try:
            _type = T__99
            _channel = DEFAULT_CHANNEL

            # Expr.g:38:7: ( '>=' )
            # Expr.g:38:9: '>='
            pass 
            self.match(">=")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__99"



    # $ANTLR start "T__100"
    def mT__100(self, ):
        try:
            _type = T__100
            _channel = DEFAULT_CHANNEL

            # Expr.g:39:8: ( '[' )
            # Expr.g:39:10: '['
            pass 
            self.match(91)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__100"



    # $ANTLR start "T__101"
    def mT__101(self, ):
        try:
            _type = T__101
            _channel = DEFAULT_CHANNEL

            # Expr.g:40:8: ( ']' )
            # Expr.g:40:10: ']'
            pass 
            self.match(93)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__101"



    # $ANTLR start "T__102"
    def mT__102(self, ):
        try:
            _type = T__102
            _channel = DEFAULT_CHANNEL

            # Expr.g:41:8: ( '^' )
            # Expr.g:41:10: '^'
            pass 
            self.match(94)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__102"



    # $ANTLR start "T__103"
    def mT__103(self, ):
        try:
            _type = T__103
            _channel = DEFAULT_CHANNEL

            # Expr.g:42:8: ( '^=' )
            # Expr.g:42:10: '^='
            pass 
            self.match("^=")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__103"



    # $ANTLR start "T__104"
    def mT__104(self, ):
        try:
            _type = T__104
            _channel = DEFAULT_CHANNEL

            # Expr.g:43:8: ( 'as' )
            # Expr.g:43:10: 'as'
            pass 
            self.match("as")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__104"



    # $ANTLR start "T__105"
    def mT__105(self, ):
        try:
            _type = T__105
            _channel = DEFAULT_CHANNEL

            # Expr.g:44:8: ( 'break' )
            # Expr.g:44:10: 'break'
            pass 
            self.match("break")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__105"



    # $ANTLR start "T__106"
    def mT__106(self, ):
        try:
            _type = T__106
            _channel = DEFAULT_CHANNEL

            # Expr.g:45:8: ( 'case' )
            # Expr.g:45:10: 'case'
            pass 
            self.match("case")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__106"



    # $ANTLR start "T__107"
    def mT__107(self, ):
        try:
            _type = T__107
            _channel = DEFAULT_CHANNEL

            # Expr.g:46:8: ( 'catch' )
            # Expr.g:46:10: 'catch'
            pass 
            self.match("catch")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__107"



    # $ANTLR start "T__108"
    def mT__108(self, ):
        try:
            _type = T__108
            _channel = DEFAULT_CHANNEL

            # Expr.g:47:8: ( 'class' )
            # Expr.g:47:10: 'class'
            pass 
            self.match("class")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__108"



    # $ANTLR start "T__109"
    def mT__109(self, ):
        try:
            _type = T__109
            _channel = DEFAULT_CHANNEL

            # Expr.g:48:8: ( 'continue' )
            # Expr.g:48:10: 'continue'
            pass 
            self.match("continue")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__109"



    # $ANTLR start "T__110"
    def mT__110(self, ):
        try:
            _type = T__110
            _channel = DEFAULT_CHANNEL

            # Expr.g:49:8: ( 'default' )
            # Expr.g:49:10: 'default'
            pass 
            self.match("default")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__110"



    # $ANTLR start "T__111"
    def mT__111(self, ):
        try:
            _type = T__111
            _channel = DEFAULT_CHANNEL

            # Expr.g:50:8: ( 'do' )
            # Expr.g:50:10: 'do'
            pass 
            self.match("do")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__111"



    # $ANTLR start "T__112"
    def mT__112(self, ):
        try:
            _type = T__112
            _channel = DEFAULT_CHANNEL

            # Expr.g:51:8: ( 'else' )
            # Expr.g:51:10: 'else'
            pass 
            self.match("else")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__112"



    # $ANTLR start "T__113"
    def mT__113(self, ):
        try:
            _type = T__113
            _channel = DEFAULT_CHANNEL

            # Expr.g:52:8: ( 'extends' )
            # Expr.g:52:10: 'extends'
            pass 
            self.match("extends")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__113"



    # $ANTLR start "T__114"
    def mT__114(self, ):
        try:
            _type = T__114
            _channel = DEFAULT_CHANNEL

            # Expr.g:53:8: ( 'finally' )
            # Expr.g:53:10: 'finally'
            pass 
            self.match("finally")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__114"



    # $ANTLR start "T__115"
    def mT__115(self, ):
        try:
            _type = T__115
            _channel = DEFAULT_CHANNEL

            # Expr.g:54:8: ( 'for' )
            # Expr.g:54:10: 'for'
            pass 
            self.match("for")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__115"



    # $ANTLR start "T__116"
    def mT__116(self, ):
        try:
            _type = T__116
            _channel = DEFAULT_CHANNEL

            # Expr.g:55:8: ( 'foreach' )
            # Expr.g:55:10: 'foreach'
            pass 
            self.match("foreach")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__116"



    # $ANTLR start "T__117"
    def mT__117(self, ):
        try:
            _type = T__117
            _channel = DEFAULT_CHANNEL

            # Expr.g:56:8: ( 'function' )
            # Expr.g:56:10: 'function'
            pass 
            self.match("function")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__117"



    # $ANTLR start "T__118"
    def mT__118(self, ):
        try:
            _type = T__118
            _channel = DEFAULT_CHANNEL

            # Expr.g:57:8: ( 'if' )
            # Expr.g:57:10: 'if'
            pass 
            self.match("if")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__118"



    # $ANTLR start "T__119"
    def mT__119(self, ):
        try:
            _type = T__119
            _channel = DEFAULT_CHANNEL

            # Expr.g:58:8: ( 'import' )
            # Expr.g:58:10: 'import'
            pass 
            self.match("import")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__119"



    # $ANTLR start "T__120"
    def mT__120(self, ):
        try:
            _type = T__120
            _channel = DEFAULT_CHANNEL

            # Expr.g:59:8: ( 'init' )
            # Expr.g:59:10: 'init'
            pass 
            self.match("init")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__120"



    # $ANTLR start "T__121"
    def mT__121(self, ):
        try:
            _type = T__121
            _channel = DEFAULT_CHANNEL

            # Expr.g:60:8: ( 'new' )
            # Expr.g:60:10: 'new'
            pass 
            self.match("new")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__121"



    # $ANTLR start "T__122"
    def mT__122(self, ):
        try:
            _type = T__122
            _channel = DEFAULT_CHANNEL

            # Expr.g:61:8: ( 'print' )
            # Expr.g:61:10: 'print'
            pass 
            self.match("print")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__122"



    # $ANTLR start "T__123"
    def mT__123(self, ):
        try:
            _type = T__123
            _channel = DEFAULT_CHANNEL

            # Expr.g:62:8: ( 'printf' )
            # Expr.g:62:10: 'printf'
            pass 
            self.match("printf")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__123"



    # $ANTLR start "T__124"
    def mT__124(self, ):
        try:
            _type = T__124
            _channel = DEFAULT_CHANNEL

            # Expr.g:63:8: ( 'public' )
            # Expr.g:63:10: 'public'
            pass 
            self.match("public")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__124"



    # $ANTLR start "T__125"
    def mT__125(self, ):
        try:
            _type = T__125
            _channel = DEFAULT_CHANNEL

            # Expr.g:64:8: ( 'return' )
            # Expr.g:64:10: 'return'
            pass 
            self.match("return")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__125"



    # $ANTLR start "T__126"
    def mT__126(self, ):
        try:
            _type = T__126
            _channel = DEFAULT_CHANNEL

            # Expr.g:65:8: ( 'sprintf' )
            # Expr.g:65:10: 'sprintf'
            pass 
            self.match("sprintf")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__126"



    # $ANTLR start "T__127"
    def mT__127(self, ):
        try:
            _type = T__127
            _channel = DEFAULT_CHANNEL

            # Expr.g:66:8: ( 'static' )
            # Expr.g:66:10: 'static'
            pass 
            self.match("static")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__127"



    # $ANTLR start "T__128"
    def mT__128(self, ):
        try:
            _type = T__128
            _channel = DEFAULT_CHANNEL

            # Expr.g:67:8: ( 'switch' )
            # Expr.g:67:10: 'switch'
            pass 
            self.match("switch")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__128"



    # $ANTLR start "T__129"
    def mT__129(self, ):
        try:
            _type = T__129
            _channel = DEFAULT_CHANNEL

            # Expr.g:68:8: ( 'throw' )
            # Expr.g:68:10: 'throw'
            pass 
            self.match("throw")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__129"



    # $ANTLR start "T__130"
    def mT__130(self, ):
        try:
            _type = T__130
            _channel = DEFAULT_CHANNEL

            # Expr.g:69:8: ( 'try' )
            # Expr.g:69:10: 'try'
            pass 
            self.match("try")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__130"



    # $ANTLR start "T__131"
    def mT__131(self, ):
        try:
            _type = T__131
            _channel = DEFAULT_CHANNEL

            # Expr.g:70:8: ( 'while' )
            # Expr.g:70:10: 'while'
            pass 
            self.match("while")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__131"



    # $ANTLR start "T__132"
    def mT__132(self, ):
        try:
            _type = T__132
            _channel = DEFAULT_CHANNEL

            # Expr.g:71:8: ( '{' )
            # Expr.g:71:10: '{'
            pass 
            self.match(123)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__132"



    # $ANTLR start "T__133"
    def mT__133(self, ):
        try:
            _type = T__133
            _channel = DEFAULT_CHANNEL

            # Expr.g:72:8: ( '|' )
            # Expr.g:72:10: '|'
            pass 
            self.match(124)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__133"



    # $ANTLR start "T__134"
    def mT__134(self, ):
        try:
            _type = T__134
            _channel = DEFAULT_CHANNEL

            # Expr.g:73:8: ( '|=' )
            # Expr.g:73:10: '|='
            pass 
            self.match("|=")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__134"



    # $ANTLR start "T__135"
    def mT__135(self, ):
        try:
            _type = T__135
            _channel = DEFAULT_CHANNEL

            # Expr.g:74:8: ( '||' )
            # Expr.g:74:10: '||'
            pass 
            self.match("||")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__135"



    # $ANTLR start "T__136"
    def mT__136(self, ):
        try:
            _type = T__136
            _channel = DEFAULT_CHANNEL

            # Expr.g:75:8: ( '}' )
            # Expr.g:75:10: '}'
            pass 
            self.match(125)



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "T__136"



    # $ANTLR start "NULL"
    def mNULL(self, ):
        try:
            _type = NULL
            _channel = DEFAULT_CHANNEL

            # Expr.g:363:2: ( 'null' )
            # Expr.g:363:4: 'null'
            pass 
            self.match("null")




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "NULL"



    # $ANTLR start "BOOL"
    def mBOOL(self, ):
        try:
            _type = BOOL
            _channel = DEFAULT_CHANNEL

            # Expr.g:364:2: ( 'true' | 'false' )
            alt1 = 2
            LA1_0 = self.input.LA(1)

            if (LA1_0 == 116) :
                alt1 = 1
            elif (LA1_0 == 102) :
                alt1 = 2
            else:
                nvae = NoViableAltException("", 1, 0, self.input)

                raise nvae


            if alt1 == 1:
                # Expr.g:364:4: 'true'
                pass 
                self.match("true")



            elif alt1 == 2:
                # Expr.g:364:13: 'false'
                pass 
                self.match("false")



            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "BOOL"



    # $ANTLR start "ID"
    def mID(self, ):
        try:
            _type = ID
            _channel = DEFAULT_CHANNEL

            # Expr.g:367:2: ( ( ALPHA | '_' | '$' ) ( ALPHA | '_' | DIGIT )* )
            # Expr.g:367:4: ( ALPHA | '_' | '$' ) ( ALPHA | '_' | DIGIT )*
            pass 
            if self.input.LA(1) == 36 or (65 <= self.input.LA(1) <= 90) or self.input.LA(1) == 95 or (97 <= self.input.LA(1) <= 122):
                self.input.consume()
            else:
                mse = MismatchedSetException(None, self.input)
                self.recover(mse)
                raise mse



            # Expr.g:367:24: ( ALPHA | '_' | DIGIT )*
            while True: #loop2
                alt2 = 2
                LA2_0 = self.input.LA(1)

                if ((48 <= LA2_0 <= 57) or (65 <= LA2_0 <= 90) or LA2_0 == 95 or (97 <= LA2_0 <= 122)) :
                    alt2 = 1


                if alt2 == 1:
                    # Expr.g:
                    pass 
                    if (48 <= self.input.LA(1) <= 57) or (65 <= self.input.LA(1) <= 90) or self.input.LA(1) == 95 or (97 <= self.input.LA(1) <= 122):
                        self.input.consume()
                    else:
                        mse = MismatchedSetException(None, self.input)
                        self.recover(mse)
                        raise mse




                else:
                    break #loop2




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "ID"



    # $ANTLR start "INT"
    def mINT(self, ):
        try:
            _type = INT
            _channel = DEFAULT_CHANNEL

            # Expr.g:371:2: ( ( DIGIT )+ )
            # Expr.g:371:4: ( DIGIT )+
            pass 
            # Expr.g:371:4: ( DIGIT )+
            cnt3 = 0
            while True: #loop3
                alt3 = 2
                LA3_0 = self.input.LA(1)

                if ((48 <= LA3_0 <= 57)) :
                    alt3 = 1


                if alt3 == 1:
                    # Expr.g:
                    pass 
                    if (48 <= self.input.LA(1) <= 57):
                        self.input.consume()
                    else:
                        mse = MismatchedSetException(None, self.input)
                        self.recover(mse)
                        raise mse




                else:
                    if cnt3 >= 1:
                        break #loop3

                    eee = EarlyExitException(3, self.input)
                    raise eee

                cnt3 += 1




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "INT"



    # $ANTLR start "FLOAT"
    def mFLOAT(self, ):
        try:
            _type = FLOAT
            _channel = DEFAULT_CHANNEL

            # Expr.g:374:2: ( INT '.' ( DIGIT )* )
            # Expr.g:374:4: INT '.' ( DIGIT )*
            pass 
            self.mINT()


            self.match(46)

            # Expr.g:374:12: ( DIGIT )*
            while True: #loop4
                alt4 = 2
                LA4_0 = self.input.LA(1)

                if ((48 <= LA4_0 <= 57)) :
                    alt4 = 1


                if alt4 == 1:
                    # Expr.g:
                    pass 
                    if (48 <= self.input.LA(1) <= 57):
                        self.input.consume()
                    else:
                        mse = MismatchedSetException(None, self.input)
                        self.recover(mse)
                        raise mse




                else:
                    break #loop4




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "FLOAT"



    # $ANTLR start "ALPHA"
    def mALPHA(self, ):
        try:
            # Expr.g:377:2: ( 'a' .. 'z' | 'A' .. 'Z' )
            # Expr.g:
            pass 
            if (65 <= self.input.LA(1) <= 90) or (97 <= self.input.LA(1) <= 122):
                self.input.consume()
            else:
                mse = MismatchedSetException(None, self.input)
                self.recover(mse)
                raise mse






        finally:
            pass

    # $ANTLR end "ALPHA"



    # $ANTLR start "DIGIT"
    def mDIGIT(self, ):
        try:
            # Expr.g:380:2: ( '0' .. '9' )
            # Expr.g:
            pass 
            if (48 <= self.input.LA(1) <= 57):
                self.input.consume()
            else:
                mse = MismatchedSetException(None, self.input)
                self.recover(mse)
                raise mse






        finally:
            pass

    # $ANTLR end "DIGIT"



    # $ANTLR start "STRING"
    def mSTRING(self, ):
        try:
            _type = STRING
            _channel = DEFAULT_CHANNEL

            # Expr.g:384:2: ( '\"' ( DOUBLE_QUOTE_CHARS )* '\"' | '\\'' ( SINGLE_QUOTE_CHARS )* '\\'' )
            alt7 = 2
            LA7_0 = self.input.LA(1)

            if (LA7_0 == 34) :
                alt7 = 1
            elif (LA7_0 == 39) :
                alt7 = 2
            else:
                nvae = NoViableAltException("", 7, 0, self.input)

                raise nvae


            if alt7 == 1:
                # Expr.g:384:4: '\"' ( DOUBLE_QUOTE_CHARS )* '\"'
                pass 
                self.match(34)

                # Expr.g:384:8: ( DOUBLE_QUOTE_CHARS )*
                while True: #loop5
                    alt5 = 2
                    LA5_0 = self.input.LA(1)

                    if ((0 <= LA5_0 <= 33) or (35 <= LA5_0 <= 65535)) :
                        alt5 = 1


                    if alt5 == 1:
                        # Expr.g:384:8: DOUBLE_QUOTE_CHARS
                        pass 
                        self.mDOUBLE_QUOTE_CHARS()



                    else:
                        break #loop5


                self.match(34)


            elif alt7 == 2:
                # Expr.g:385:4: '\\'' ( SINGLE_QUOTE_CHARS )* '\\''
                pass 
                self.match(39)

                # Expr.g:385:9: ( SINGLE_QUOTE_CHARS )*
                while True: #loop6
                    alt6 = 2
                    LA6_0 = self.input.LA(1)

                    if ((0 <= LA6_0 <= 38) or (40 <= LA6_0 <= 65535)) :
                        alt6 = 1


                    if alt6 == 1:
                        # Expr.g:385:9: SINGLE_QUOTE_CHARS
                        pass 
                        self.mSINGLE_QUOTE_CHARS()



                    else:
                        break #loop6


                self.match(39)


            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "STRING"



    # $ANTLR start "DOUBLE_QUOTE_CHARS"
    def mDOUBLE_QUOTE_CHARS(self, ):
        try:
            # Expr.g:388:2: (~ ( '\"' ) | '\\\\' '\"' | '\\\\' '\\\\' )
            alt8 = 3
            LA8_0 = self.input.LA(1)

            if (LA8_0 == 92) :
                LA8 = self.input.LA(2)
                if LA8 == 34:
                    alt8 = 2
                elif LA8 == 92:
                    alt8 = 3
                else:
                    alt8 = 1

            elif ((0 <= LA8_0 <= 33) or (35 <= LA8_0 <= 91) or (93 <= LA8_0 <= 65535)) :
                alt8 = 1
            else:
                nvae = NoViableAltException("", 8, 0, self.input)

                raise nvae


            if alt8 == 1:
                # Expr.g:388:4: ~ ( '\"' )
                pass 
                if (0 <= self.input.LA(1) <= 33) or (35 <= self.input.LA(1) <= 65535):
                    self.input.consume()
                else:
                    mse = MismatchedSetException(None, self.input)
                    self.recover(mse)
                    raise mse




            elif alt8 == 2:
                # Expr.g:390:4: '\\\\' '\"'
                pass 
                self.match(92)

                self.match(34)


            elif alt8 == 3:
                # Expr.g:391:4: '\\\\' '\\\\'
                pass 
                self.match(92)

                self.match(92)



        finally:
            pass

    # $ANTLR end "DOUBLE_QUOTE_CHARS"



    # $ANTLR start "SINGLE_QUOTE_CHARS"
    def mSINGLE_QUOTE_CHARS(self, ):
        try:
            # Expr.g:394:2: (~ ( '\\'' ) | '\\\\' '\\'' | '\\\\' '\\\\' )
            alt9 = 3
            LA9_0 = self.input.LA(1)

            if (LA9_0 == 92) :
                LA9 = self.input.LA(2)
                if LA9 == 39:
                    alt9 = 2
                elif LA9 == 92:
                    alt9 = 3
                else:
                    alt9 = 1

            elif ((0 <= LA9_0 <= 38) or (40 <= LA9_0 <= 91) or (93 <= LA9_0 <= 65535)) :
                alt9 = 1
            else:
                nvae = NoViableAltException("", 9, 0, self.input)

                raise nvae


            if alt9 == 1:
                # Expr.g:394:4: ~ ( '\\'' )
                pass 
                if (0 <= self.input.LA(1) <= 38) or (40 <= self.input.LA(1) <= 65535):
                    self.input.consume()
                else:
                    mse = MismatchedSetException(None, self.input)
                    self.recover(mse)
                    raise mse




            elif alt9 == 2:
                # Expr.g:395:4: '\\\\' '\\''
                pass 
                self.match(92)

                self.match(39)


            elif alt9 == 3:
                # Expr.g:396:4: '\\\\' '\\\\'
                pass 
                self.match(92)

                self.match(92)



        finally:
            pass

    # $ANTLR end "SINGLE_QUOTE_CHARS"



    # $ANTLR start "NEWLINE"
    def mNEWLINE(self, ):
        try:
            # Expr.g:400:2: ( ( '\\r' )? '\\n' )
            # Expr.g:400:4: ( '\\r' )? '\\n'
            pass 
            # Expr.g:400:4: ( '\\r' )?
            alt10 = 2
            LA10_0 = self.input.LA(1)

            if (LA10_0 == 13) :
                alt10 = 1
            if alt10 == 1:
                # Expr.g:400:4: '\\r'
                pass 
                self.match(13)




            self.match(10)




        finally:
            pass

    # $ANTLR end "NEWLINE"



    # $ANTLR start "WS"
    def mWS(self, ):
        try:
            _type = WS
            _channel = DEFAULT_CHANNEL

            # Expr.g:404:2: ( ( ' ' | '\\t' | '\\r' | '\\n' )+ )
            # Expr.g:404:4: ( ' ' | '\\t' | '\\r' | '\\n' )+
            pass 
            # Expr.g:404:4: ( ' ' | '\\t' | '\\r' | '\\n' )+
            cnt11 = 0
            while True: #loop11
                alt11 = 2
                LA11_0 = self.input.LA(1)

                if ((9 <= LA11_0 <= 10) or LA11_0 == 13 or LA11_0 == 32) :
                    alt11 = 1


                if alt11 == 1:
                    # Expr.g:
                    pass 
                    if (9 <= self.input.LA(1) <= 10) or self.input.LA(1) == 13 or self.input.LA(1) == 32:
                        self.input.consume()
                    else:
                        mse = MismatchedSetException(None, self.input)
                        self.recover(mse)
                        raise mse




                else:
                    if cnt11 >= 1:
                        break #loop11

                    eee = EarlyExitException(11, self.input)
                    raise eee

                cnt11 += 1


            #action start
            _channel=HIDDEN;
            #action end




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "WS"



    # $ANTLR start "COMMENT"
    def mCOMMENT(self, ):
        try:
            _type = COMMENT
            _channel = DEFAULT_CHANNEL

            # Expr.g:407:2: ( '/*' ( options {greedy=false; } : . )* '*/' )
            # Expr.g:407:4: '/*' ( options {greedy=false; } : . )* '*/'
            pass 
            self.match("/*")


            # Expr.g:407:9: ( options {greedy=false; } : . )*
            while True: #loop12
                alt12 = 2
                LA12_0 = self.input.LA(1)

                if (LA12_0 == 42) :
                    LA12_1 = self.input.LA(2)

                    if (LA12_1 == 47) :
                        alt12 = 2
                    elif ((0 <= LA12_1 <= 46) or (48 <= LA12_1 <= 65535)) :
                        alt12 = 1


                elif ((0 <= LA12_0 <= 41) or (43 <= LA12_0 <= 65535)) :
                    alt12 = 1


                if alt12 == 1:
                    # Expr.g:407:34: .
                    pass 
                    self.matchAny()


                else:
                    break #loop12


            self.match("*/")


            #action start
            _channel=HIDDEN;
            #action end




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "COMMENT"



    # $ANTLR start "LINECOMMENT"
    def mLINECOMMENT(self, ):
        try:
            _type = LINECOMMENT
            _channel = DEFAULT_CHANNEL

            # Expr.g:410:2: ( ( '//' | '#' ) (~ ( '\\r' | '\\n' ) )* NEWLINE )
            # Expr.g:410:4: ( '//' | '#' ) (~ ( '\\r' | '\\n' ) )* NEWLINE
            pass 
            # Expr.g:410:4: ( '//' | '#' )
            alt13 = 2
            LA13_0 = self.input.LA(1)

            if (LA13_0 == 47) :
                alt13 = 1
            elif (LA13_0 == 35) :
                alt13 = 2
            else:
                nvae = NoViableAltException("", 13, 0, self.input)

                raise nvae


            if alt13 == 1:
                # Expr.g:410:5: '//'
                pass 
                self.match("//")



            elif alt13 == 2:
                # Expr.g:410:10: '#'
                pass 
                self.match(35)




            # Expr.g:410:15: (~ ( '\\r' | '\\n' ) )*
            while True: #loop14
                alt14 = 2
                LA14_0 = self.input.LA(1)

                if ((0 <= LA14_0 <= 9) or (11 <= LA14_0 <= 12) or (14 <= LA14_0 <= 65535)) :
                    alt14 = 1


                if alt14 == 1:
                    # Expr.g:
                    pass 
                    if (0 <= self.input.LA(1) <= 9) or (11 <= self.input.LA(1) <= 12) or (14 <= self.input.LA(1) <= 65535):
                        self.input.consume()
                    else:
                        mse = MismatchedSetException(None, self.input)
                        self.recover(mse)
                        raise mse




                else:
                    break #loop14


            self.mNEWLINE()


            #action start
            _channel=HIDDEN;
            #action end




            self._state.type = _type
            self._state.channel = _channel
        finally:
            pass

    # $ANTLR end "LINECOMMENT"



    def mTokens(self):
        # Expr.g:1:8: ( T__68 | T__69 | T__70 | T__71 | T__72 | T__73 | T__74 | T__75 | T__76 | T__77 | T__78 | T__79 | T__80 | T__81 | T__82 | T__83 | T__84 | T__85 | T__86 | T__87 | T__88 | T__89 | T__90 | T__91 | T__92 | T__93 | T__94 | T__95 | T__96 | T__97 | T__98 | T__99 | T__100 | T__101 | T__102 | T__103 | T__104 | T__105 | T__106 | T__107 | T__108 | T__109 | T__110 | T__111 | T__112 | T__113 | T__114 | T__115 | T__116 | T__117 | T__118 | T__119 | T__120 | T__121 | T__122 | T__123 | T__124 | T__125 | T__126 | T__127 | T__128 | T__129 | T__130 | T__131 | T__132 | T__133 | T__134 | T__135 | T__136 | NULL | BOOL | ID | INT | FLOAT | STRING | WS | COMMENT | LINECOMMENT )
        alt15 = 78
        alt15 = self.dfa15.predict(self.input)
        if alt15 == 1:
            # Expr.g:1:10: T__68
            pass 
            self.mT__68()



        elif alt15 == 2:
            # Expr.g:1:16: T__69
            pass 
            self.mT__69()



        elif alt15 == 3:
            # Expr.g:1:22: T__70
            pass 
            self.mT__70()



        elif alt15 == 4:
            # Expr.g:1:28: T__71
            pass 
            self.mT__71()



        elif alt15 == 5:
            # Expr.g:1:34: T__72
            pass 
            self.mT__72()



        elif alt15 == 6:
            # Expr.g:1:40: T__73
            pass 
            self.mT__73()



        elif alt15 == 7:
            # Expr.g:1:46: T__74
            pass 
            self.mT__74()



        elif alt15 == 8:
            # Expr.g:1:52: T__75
            pass 
            self.mT__75()



        elif alt15 == 9:
            # Expr.g:1:58: T__76
            pass 
            self.mT__76()



        elif alt15 == 10:
            # Expr.g:1:64: T__77
            pass 
            self.mT__77()



        elif alt15 == 11:
            # Expr.g:1:70: T__78
            pass 
            self.mT__78()



        elif alt15 == 12:
            # Expr.g:1:76: T__79
            pass 
            self.mT__79()



        elif alt15 == 13:
            # Expr.g:1:82: T__80
            pass 
            self.mT__80()



        elif alt15 == 14:
            # Expr.g:1:88: T__81
            pass 
            self.mT__81()



        elif alt15 == 15:
            # Expr.g:1:94: T__82
            pass 
            self.mT__82()



        elif alt15 == 16:
            # Expr.g:1:100: T__83
            pass 
            self.mT__83()



        elif alt15 == 17:
            # Expr.g:1:106: T__84
            pass 
            self.mT__84()



        elif alt15 == 18:
            # Expr.g:1:112: T__85
            pass 
            self.mT__85()



        elif alt15 == 19:
            # Expr.g:1:118: T__86
            pass 
            self.mT__86()



        elif alt15 == 20:
            # Expr.g:1:124: T__87
            pass 
            self.mT__87()



        elif alt15 == 21:
            # Expr.g:1:130: T__88
            pass 
            self.mT__88()



        elif alt15 == 22:
            # Expr.g:1:136: T__89
            pass 
            self.mT__89()



        elif alt15 == 23:
            # Expr.g:1:142: T__90
            pass 
            self.mT__90()



        elif alt15 == 24:
            # Expr.g:1:148: T__91
            pass 
            self.mT__91()



        elif alt15 == 25:
            # Expr.g:1:154: T__92
            pass 
            self.mT__92()



        elif alt15 == 26:
            # Expr.g:1:160: T__93
            pass 
            self.mT__93()



        elif alt15 == 27:
            # Expr.g:1:166: T__94
            pass 
            self.mT__94()



        elif alt15 == 28:
            # Expr.g:1:172: T__95
            pass 
            self.mT__95()



        elif alt15 == 29:
            # Expr.g:1:178: T__96
            pass 
            self.mT__96()



        elif alt15 == 30:
            # Expr.g:1:184: T__97
            pass 
            self.mT__97()



        elif alt15 == 31:
            # Expr.g:1:190: T__98
            pass 
            self.mT__98()



        elif alt15 == 32:
            # Expr.g:1:196: T__99
            pass 
            self.mT__99()



        elif alt15 == 33:
            # Expr.g:1:202: T__100
            pass 
            self.mT__100()



        elif alt15 == 34:
            # Expr.g:1:209: T__101
            pass 
            self.mT__101()



        elif alt15 == 35:
            # Expr.g:1:216: T__102
            pass 
            self.mT__102()



        elif alt15 == 36:
            # Expr.g:1:223: T__103
            pass 
            self.mT__103()



        elif alt15 == 37:
            # Expr.g:1:230: T__104
            pass 
            self.mT__104()



        elif alt15 == 38:
            # Expr.g:1:237: T__105
            pass 
            self.mT__105()



        elif alt15 == 39:
            # Expr.g:1:244: T__106
            pass 
            self.mT__106()



        elif alt15 == 40:
            # Expr.g:1:251: T__107
            pass 
            self.mT__107()



        elif alt15 == 41:
            # Expr.g:1:258: T__108
            pass 
            self.mT__108()



        elif alt15 == 42:
            # Expr.g:1:265: T__109
            pass 
            self.mT__109()



        elif alt15 == 43:
            # Expr.g:1:272: T__110
            pass 
            self.mT__110()



        elif alt15 == 44:
            # Expr.g:1:279: T__111
            pass 
            self.mT__111()



        elif alt15 == 45:
            # Expr.g:1:286: T__112
            pass 
            self.mT__112()



        elif alt15 == 46:
            # Expr.g:1:293: T__113
            pass 
            self.mT__113()



        elif alt15 == 47:
            # Expr.g:1:300: T__114
            pass 
            self.mT__114()



        elif alt15 == 48:
            # Expr.g:1:307: T__115
            pass 
            self.mT__115()



        elif alt15 == 49:
            # Expr.g:1:314: T__116
            pass 
            self.mT__116()



        elif alt15 == 50:
            # Expr.g:1:321: T__117
            pass 
            self.mT__117()



        elif alt15 == 51:
            # Expr.g:1:328: T__118
            pass 
            self.mT__118()



        elif alt15 == 52:
            # Expr.g:1:335: T__119
            pass 
            self.mT__119()



        elif alt15 == 53:
            # Expr.g:1:342: T__120
            pass 
            self.mT__120()



        elif alt15 == 54:
            # Expr.g:1:349: T__121
            pass 
            self.mT__121()



        elif alt15 == 55:
            # Expr.g:1:356: T__122
            pass 
            self.mT__122()



        elif alt15 == 56:
            # Expr.g:1:363: T__123
            pass 
            self.mT__123()



        elif alt15 == 57:
            # Expr.g:1:370: T__124
            pass 
            self.mT__124()



        elif alt15 == 58:
            # Expr.g:1:377: T__125
            pass 
            self.mT__125()



        elif alt15 == 59:
            # Expr.g:1:384: T__126
            pass 
            self.mT__126()



        elif alt15 == 60:
            # Expr.g:1:391: T__127
            pass 
            self.mT__127()



        elif alt15 == 61:
            # Expr.g:1:398: T__128
            pass 
            self.mT__128()



        elif alt15 == 62:
            # Expr.g:1:405: T__129
            pass 
            self.mT__129()



        elif alt15 == 63:
            # Expr.g:1:412: T__130
            pass 
            self.mT__130()



        elif alt15 == 64:
            # Expr.g:1:419: T__131
            pass 
            self.mT__131()



        elif alt15 == 65:
            # Expr.g:1:426: T__132
            pass 
            self.mT__132()



        elif alt15 == 66:
            # Expr.g:1:433: T__133
            pass 
            self.mT__133()



        elif alt15 == 67:
            # Expr.g:1:440: T__134
            pass 
            self.mT__134()



        elif alt15 == 68:
            # Expr.g:1:447: T__135
            pass 
            self.mT__135()



        elif alt15 == 69:
            # Expr.g:1:454: T__136
            pass 
            self.mT__136()



        elif alt15 == 70:
            # Expr.g:1:461: NULL
            pass 
            self.mNULL()



        elif alt15 == 71:
            # Expr.g:1:466: BOOL
            pass 
            self.mBOOL()



        elif alt15 == 72:
            # Expr.g:1:471: ID
            pass 
            self.mID()



        elif alt15 == 73:
            # Expr.g:1:474: INT
            pass 
            self.mINT()



        elif alt15 == 74:
            # Expr.g:1:478: FLOAT
            pass 
            self.mFLOAT()



        elif alt15 == 75:
            # Expr.g:1:484: STRING
            pass 
            self.mSTRING()



        elif alt15 == 76:
            # Expr.g:1:491: WS
            pass 
            self.mWS()



        elif alt15 == 77:
            # Expr.g:1:494: COMMENT
            pass 
            self.mCOMMENT()



        elif alt15 == 78:
            # Expr.g:1:502: LINECOMMENT
            pass 
            self.mLINECOMMENT()








    # lookup tables for DFA #15

    DFA15_eot = DFA.unpack(
        u"\1\uffff\1\52\1\54\1\57\2\uffff\1\61\1\64\1\uffff\1\67\1\72\1\75"
        u"\2\uffff\1\77\1\102\1\104\2\uffff\1\106\15\44\1\uffff\1\144\2\uffff"
        u"\1\145\41\uffff\1\147\5\44\1\156\6\44\1\165\15\44\6\uffff\6\44"
        u"\1\uffff\3\44\1\u008e\2\44\1\uffff\2\44\1\u0093\10\44\1\u009c\3"
        u"\44\1\u00a0\4\44\1\u00a5\3\44\1\uffff\3\44\1\u00ac\1\uffff\1\u00ad"
        u"\7\44\1\uffff\1\u00b5\1\44\1\u00b7\1\uffff\1\u00b8\1\u00b9\2\44"
        u"\1\uffff\4\44\1\u00b5\1\44\2\uffff\1\u00c2\5\44\1\u00c8\1\uffff"
        u"\1\u00c9\3\uffff\6\44\1\u00d0\1\u00d1\1\uffff\1\u00d2\1\u00d3\1"
        u"\44\1\u00d5\1\u00d6\2\uffff\1\44\1\u00d8\1\u00d9\1\u00da\1\u00db"
        u"\1\44\4\uffff\1\u00dd\2\uffff\1\u00de\4\uffff\1\u00df\3\uffff"
        )

    DFA15_eof = DFA.unpack(
        u"\u00e0\uffff"
        )

    DFA15_min = DFA.unpack(
        u"\1\11\2\75\1\46\2\uffff\1\75\1\53\1\uffff\1\55\2\52\2\uffff\3\75"
        u"\2\uffff\1\75\1\163\1\162\1\141\1\145\1\154\1\141\1\146\1\145\1"
        u"\162\1\145\1\160\2\150\1\uffff\1\75\2\uffff\1\56\41\uffff\1\60"
        u"\1\145\1\163\1\141\1\156\1\146\1\60\1\163\1\164\1\156\1\162\1\156"
        u"\1\154\1\60\1\160\1\151\1\167\1\154\1\151\1\142\1\164\1\162\1\141"
        u"\1\151\1\162\1\165\1\151\6\uffff\1\141\1\145\1\143\1\163\1\164"
        u"\1\141\1\uffff\2\145\1\141\1\60\1\143\1\163\1\uffff\1\157\1\164"
        u"\1\60\1\154\1\156\1\154\1\165\1\151\2\164\1\157\1\60\1\145\1\154"
        u"\1\153\1\60\1\150\1\163\1\151\1\165\1\60\1\156\1\154\1\141\1\uffff"
        u"\1\164\1\145\1\162\1\60\1\uffff\1\60\1\164\1\151\1\162\1\156\1"
        u"\151\1\143\1\167\1\uffff\1\60\1\145\1\60\1\uffff\2\60\1\156\1\154"
        u"\1\uffff\1\144\1\154\1\143\1\151\1\60\1\164\2\uffff\1\60\1\143"
        u"\1\156\1\164\1\143\1\150\1\60\1\uffff\1\60\3\uffff\1\165\1\164"
        u"\1\163\1\171\1\150\1\157\2\60\1\uffff\2\60\1\146\2\60\2\uffff\1"
        u"\145\4\60\1\156\4\uffff\1\60\2\uffff\1\60\4\uffff\1\60\3\uffff"
        )

    DFA15_max = DFA.unpack(
        u"\1\175\3\75\2\uffff\2\75\1\uffff\1\75\1\56\1\75\2\uffff\1\75\1"
        u"\76\1\75\2\uffff\1\75\1\163\1\162\2\157\1\170\1\165\1\156\2\165"
        u"\1\145\1\167\1\162\1\150\1\uffff\1\174\2\uffff\1\71\41\uffff\1"
        u"\172\1\145\1\164\1\141\1\156\1\146\1\172\1\163\1\164\1\156\1\162"
        u"\1\156\1\154\1\172\1\160\1\151\1\167\1\154\1\151\1\142\1\164\1"
        u"\162\1\141\1\151\1\162\1\171\1\151\6\uffff\1\141\1\145\1\143\1"
        u"\163\1\164\1\141\1\uffff\2\145\1\141\1\172\1\143\1\163\1\uffff"
        u"\1\157\1\164\1\172\1\154\1\156\1\154\1\165\1\151\2\164\1\157\1"
        u"\172\1\145\1\154\1\153\1\172\1\150\1\163\1\151\1\165\1\172\1\156"
        u"\1\154\1\141\1\uffff\1\164\1\145\1\162\1\172\1\uffff\1\172\1\164"
        u"\1\151\1\162\1\156\1\151\1\143\1\167\1\uffff\1\172\1\145\1\172"
        u"\1\uffff\2\172\1\156\1\154\1\uffff\1\144\1\154\1\143\1\151\1\172"
        u"\1\164\2\uffff\1\172\1\143\1\156\1\164\1\143\1\150\1\172\1\uffff"
        u"\1\172\3\uffff\1\165\1\164\1\163\1\171\1\150\1\157\2\172\1\uffff"
        u"\2\172\1\146\2\172\2\uffff\1\145\4\172\1\156\4\uffff\1\172\2\uffff"
        u"\1\172\4\uffff\1\172\3\uffff"
        )

    DFA15_accept = DFA.unpack(
        u"\4\uffff\1\10\1\11\2\uffff\1\17\3\uffff\1\30\1\31\3\uffff\1\41"
        u"\1\42\16\uffff\1\101\1\uffff\1\105\1\110\1\uffff\1\113\1\114\1"
        u"\116\1\2\1\1\1\4\1\3\1\5\1\7\1\6\1\13\1\12\1\15\1\16\1\14\1\21"
        u"\1\22\1\20\1\24\1\25\1\23\1\27\1\115\1\26\1\33\1\32\1\35\1\36\1"
        u"\34\1\40\1\37\1\44\1\43\33\uffff\1\103\1\104\1\102\1\111\1\112"
        u"\1\45\6\uffff\1\54\6\uffff\1\63\30\uffff\1\60\4\uffff\1\66\10\uffff"
        u"\1\77\3\uffff\1\47\4\uffff\1\55\6\uffff\1\65\1\106\7\uffff\1\107"
        u"\1\uffff\1\46\1\50\1\51\10\uffff\1\67\5\uffff\1\76\1\100\6\uffff"
        u"\1\64\1\70\1\71\1\72\1\uffff\1\74\1\75\1\uffff\1\53\1\56\1\57\1"
        u"\61\1\uffff\1\73\1\52\1\62"
        )

    DFA15_special = DFA.unpack(
        u"\u00e0\uffff"
        )


    DFA15_transition = [
        DFA.unpack(u"\2\47\2\uffff\1\47\22\uffff\1\47\1\1\1\46\1\50\1\44"
        u"\1\2\1\3\1\46\1\4\1\5\1\6\1\7\1\10\1\11\1\12\1\13\12\45\1\14\1"
        u"\15\1\16\1\17\1\20\2\uffff\32\44\1\21\1\uffff\1\22\1\23\1\44\1"
        u"\uffff\1\24\1\25\1\26\1\27\1\30\1\31\2\44\1\32\4\44\1\33\1\44\1"
        u"\34\1\44\1\35\1\36\1\37\2\44\1\40\3\44\1\41\1\42\1\43"),
        DFA.unpack(u"\1\51"),
        DFA.unpack(u"\1\53"),
        DFA.unpack(u"\1\55\26\uffff\1\56"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\60"),
        DFA.unpack(u"\1\62\21\uffff\1\63"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\65\17\uffff\1\66"),
        DFA.unpack(u"\1\70\3\uffff\1\71"),
        DFA.unpack(u"\1\74\4\uffff\1\50\15\uffff\1\73"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\76"),
        DFA.unpack(u"\1\100\1\101"),
        DFA.unpack(u"\1\103"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\105"),
        DFA.unpack(u"\1\107"),
        DFA.unpack(u"\1\110"),
        DFA.unpack(u"\1\111\12\uffff\1\112\2\uffff\1\113"),
        DFA.unpack(u"\1\114\11\uffff\1\115"),
        DFA.unpack(u"\1\116\13\uffff\1\117"),
        DFA.unpack(u"\1\123\7\uffff\1\120\5\uffff\1\121\5\uffff\1\122"),
        DFA.unpack(u"\1\124\6\uffff\1\125\1\126"),
        DFA.unpack(u"\1\127\17\uffff\1\130"),
        DFA.unpack(u"\1\131\2\uffff\1\132"),
        DFA.unpack(u"\1\133"),
        DFA.unpack(u"\1\134\3\uffff\1\135\2\uffff\1\136"),
        DFA.unpack(u"\1\137\11\uffff\1\140"),
        DFA.unpack(u"\1\141"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\142\76\uffff\1\143"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\146\1\uffff\12\45"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\1\150"),
        DFA.unpack(u"\1\151\1\152"),
        DFA.unpack(u"\1\153"),
        DFA.unpack(u"\1\154"),
        DFA.unpack(u"\1\155"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\1\157"),
        DFA.unpack(u"\1\160"),
        DFA.unpack(u"\1\161"),
        DFA.unpack(u"\1\162"),
        DFA.unpack(u"\1\163"),
        DFA.unpack(u"\1\164"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\1\166"),
        DFA.unpack(u"\1\167"),
        DFA.unpack(u"\1\170"),
        DFA.unpack(u"\1\171"),
        DFA.unpack(u"\1\172"),
        DFA.unpack(u"\1\173"),
        DFA.unpack(u"\1\174"),
        DFA.unpack(u"\1\175"),
        DFA.unpack(u"\1\176"),
        DFA.unpack(u"\1\177"),
        DFA.unpack(u"\1\u0080"),
        DFA.unpack(u"\1\u0082\3\uffff\1\u0081"),
        DFA.unpack(u"\1\u0083"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u0084"),
        DFA.unpack(u"\1\u0085"),
        DFA.unpack(u"\1\u0086"),
        DFA.unpack(u"\1\u0087"),
        DFA.unpack(u"\1\u0088"),
        DFA.unpack(u"\1\u0089"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u008a"),
        DFA.unpack(u"\1\u008b"),
        DFA.unpack(u"\1\u008c"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\4\44\1\u008d"
        u"\25\44"),
        DFA.unpack(u"\1\u008f"),
        DFA.unpack(u"\1\u0090"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u0091"),
        DFA.unpack(u"\1\u0092"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\1\u0094"),
        DFA.unpack(u"\1\u0095"),
        DFA.unpack(u"\1\u0096"),
        DFA.unpack(u"\1\u0097"),
        DFA.unpack(u"\1\u0098"),
        DFA.unpack(u"\1\u0099"),
        DFA.unpack(u"\1\u009a"),
        DFA.unpack(u"\1\u009b"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\1\u009d"),
        DFA.unpack(u"\1\u009e"),
        DFA.unpack(u"\1\u009f"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\1\u00a1"),
        DFA.unpack(u"\1\u00a2"),
        DFA.unpack(u"\1\u00a3"),
        DFA.unpack(u"\1\u00a4"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\1\u00a6"),
        DFA.unpack(u"\1\u00a7"),
        DFA.unpack(u"\1\u00a8"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u00a9"),
        DFA.unpack(u"\1\u00aa"),
        DFA.unpack(u"\1\u00ab"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u""),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\1\u00ae"),
        DFA.unpack(u"\1\u00af"),
        DFA.unpack(u"\1\u00b0"),
        DFA.unpack(u"\1\u00b1"),
        DFA.unpack(u"\1\u00b2"),
        DFA.unpack(u"\1\u00b3"),
        DFA.unpack(u"\1\u00b4"),
        DFA.unpack(u""),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\1\u00b6"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u""),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\1\u00ba"),
        DFA.unpack(u"\1\u00bb"),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u00bc"),
        DFA.unpack(u"\1\u00bd"),
        DFA.unpack(u"\1\u00be"),
        DFA.unpack(u"\1\u00bf"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\1\u00c0"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\5\44\1\u00c1"
        u"\24\44"),
        DFA.unpack(u"\1\u00c3"),
        DFA.unpack(u"\1\u00c4"),
        DFA.unpack(u"\1\u00c5"),
        DFA.unpack(u"\1\u00c6"),
        DFA.unpack(u"\1\u00c7"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u""),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u00ca"),
        DFA.unpack(u"\1\u00cb"),
        DFA.unpack(u"\1\u00cc"),
        DFA.unpack(u"\1\u00cd"),
        DFA.unpack(u"\1\u00ce"),
        DFA.unpack(u"\1\u00cf"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u""),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\1\u00d4"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\u00d7"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u"\1\u00dc"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\12\44\7\uffff\32\44\4\uffff\1\44\1\uffff\32\44"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"")
    ]

    # class definition for DFA #15

    class DFA15(DFA):
        pass


 



def main(argv, stdin=sys.stdin, stdout=sys.stdout, stderr=sys.stderr):
    from antlr3.main import LexerMain
    main = LexerMain(ExprLexer)

    main.stdin = stdin
    main.stdout = stdout
    main.stderr = stderr
    main.execute(argv)



if __name__ == '__main__':
    main(sys.argv)
