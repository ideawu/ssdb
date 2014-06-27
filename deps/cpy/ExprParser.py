# $ANTLR 3.5 Expr.g 2013-04-12 19:22:24

import sys
from antlr3 import *
from antlr3.compat import set, frozenset

from antlr3.tree import *




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




class ExprParser(Parser):
    grammarFileName = "Expr.g"
    api_version = 1
    tokenNames = tokenNames

    def __init__(self, input, state=None, *args, **kwargs):
        if state is None:
            state = RecognizerSharedState()

        super(ExprParser, self).__init__(input, state, *args, **kwargs)

        self.dfa6 = self.DFA6(
            self, 6,
            eot = self.DFA6_eot,
            eof = self.DFA6_eof,
            min = self.DFA6_min,
            max = self.DFA6_max,
            accept = self.DFA6_accept,
            special = self.DFA6_special,
            transition = self.DFA6_transition
            )




        self.delegates = []

	self._adaptor = None
	self.adaptor = CommonTreeAdaptor()



    def getTreeAdaptor(self):
        return self._adaptor

    def setTreeAdaptor(self, adaptor):
        self._adaptor = adaptor

    adaptor = property(getTreeAdaptor, setTreeAdaptor)


    class prog_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.prog_return, self).__init__()

            self.tree = None





    # $ANTLR start "prog"
    # Expr.g:33:1: prog : ( EOF -> NOP | ( stmt )* );
    def prog(self, ):
        retval = self.prog_return()
        retval.start = self.input.LT(1)


        root_0 = None

        EOF1 = None
        stmt2 = None

        EOF1_tree = None
        stream_EOF = RewriteRuleTokenStream(self._adaptor, "token EOF")

        try:
            try:
                # Expr.g:34:2: ( EOF -> NOP | ( stmt )* )
                alt2 = 2
                LA2_0 = self.input.LA(1)

                if (LA2_0 == EOF) :
                    LA2_1 = self.input.LA(2)

                    if (LA2_1 == EOF) :
                        alt2 = 1
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed


                        nvae = NoViableAltException("", 2, 1, self.input)

                        raise nvae


                elif (LA2_0 == ID or LA2_0 == 80 or LA2_0 == 84 or LA2_0 == 92 or LA2_0 == 105 or (108 <= LA2_0 <= 109) or LA2_0 == 111 or (115 <= LA2_0 <= 119) or (122 <= LA2_0 <= 123) or LA2_0 == 125 or (128 <= LA2_0 <= 131)) :
                    alt2 = 2
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    nvae = NoViableAltException("", 2, 0, self.input)

                    raise nvae


                if alt2 == 1:
                    # Expr.g:34:4: EOF
                    pass 
                    EOF1 = self.match(self.input, EOF, self.FOLLOW_EOF_in_prog211) 
                    if self._state.backtracking == 0:
                        stream_EOF.add(EOF1)


                    # AST Rewrite
                    # elements: 
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 34:8: -> NOP
                        self._adaptor.addChild(root_0, 
                        self._adaptor.createFromType(NOP, "NOP")
                        )




                        retval.tree = root_0




                elif alt2 == 2:
                    # Expr.g:35:4: ( stmt )*
                    pass 
                    root_0 = self._adaptor.nil()


                    # Expr.g:35:4: ( stmt )*
                    while True: #loop1
                        alt1 = 2
                        LA1_0 = self.input.LA(1)

                        if (LA1_0 == ID or LA1_0 == 80 or LA1_0 == 84 or LA1_0 == 92 or LA1_0 == 105 or (108 <= LA1_0 <= 109) or LA1_0 == 111 or (115 <= LA1_0 <= 119) or (122 <= LA1_0 <= 123) or LA1_0 == 125 or (128 <= LA1_0 <= 131)) :
                            alt1 = 1


                        if alt1 == 1:
                            # Expr.g:35:4: stmt
                            pass 
                            self._state.following.append(self.FOLLOW_stmt_in_prog220)
                            stmt2 = self.stmt()

                            self._state.following.pop()
                            if self._state.backtracking == 0:
                                self._adaptor.addChild(root_0, stmt2.tree)



                        else:
                            break #loop1



                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "prog"


    class stmt_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.stmt_return, self).__init__()

            self.tree = None





    # $ANTLR start "stmt"
    # Expr.g:38:1: stmt : ( ';' ->| exec_stmt | import_stmt | print_stmt | printf_stmt | break_stmt | continue_stmt | return_stmt | if_stmt | while_stmt | do_while_stmt | switch_stmt | for_stmt | foreach_stmt | throw_stmt | try_stmt | func_decl | class_decl );
    def stmt(self, ):
        retval = self.stmt_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal3 = None
        exec_stmt4 = None
        import_stmt5 = None
        print_stmt6 = None
        printf_stmt7 = None
        break_stmt8 = None
        continue_stmt9 = None
        return_stmt10 = None
        if_stmt11 = None
        while_stmt12 = None
        do_while_stmt13 = None
        switch_stmt14 = None
        for_stmt15 = None
        foreach_stmt16 = None
        throw_stmt17 = None
        try_stmt18 = None
        func_decl19 = None
        class_decl20 = None

        char_literal3_tree = None
        stream_92 = RewriteRuleTokenStream(self._adaptor, "token 92")

        try:
            try:
                # Expr.g:39:2: ( ';' ->| exec_stmt | import_stmt | print_stmt | printf_stmt | break_stmt | continue_stmt | return_stmt | if_stmt | while_stmt | do_while_stmt | switch_stmt | for_stmt | foreach_stmt | throw_stmt | try_stmt | func_decl | class_decl )
                alt3 = 18
                LA3 = self.input.LA(1)
                if LA3 == 92:
                    alt3 = 1
                elif LA3 == ID or LA3 == 80 or LA3 == 84:
                    alt3 = 2
                elif LA3 == 119:
                    alt3 = 3
                elif LA3 == 122:
                    alt3 = 4
                elif LA3 == 123:
                    alt3 = 5
                elif LA3 == 105:
                    alt3 = 6
                elif LA3 == 109:
                    alt3 = 7
                elif LA3 == 125:
                    alt3 = 8
                elif LA3 == 118:
                    alt3 = 9
                elif LA3 == 131:
                    alt3 = 10
                elif LA3 == 111:
                    alt3 = 11
                elif LA3 == 128:
                    alt3 = 12
                elif LA3 == 115:
                    alt3 = 13
                elif LA3 == 116:
                    alt3 = 14
                elif LA3 == 129:
                    alt3 = 15
                elif LA3 == 130:
                    alt3 = 16
                elif LA3 == 117:
                    alt3 = 17
                elif LA3 == 108:
                    alt3 = 18
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    nvae = NoViableAltException("", 3, 0, self.input)

                    raise nvae


                if alt3 == 1:
                    # Expr.g:39:4: ';'
                    pass 
                    char_literal3 = self.match(self.input, 92, self.FOLLOW_92_in_stmt232) 
                    if self._state.backtracking == 0:
                        stream_92.add(char_literal3)


                    # AST Rewrite
                    # elements: 
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 39:8: ->
                        root_0 = None



                        retval.tree = root_0




                elif alt3 == 2:
                    # Expr.g:40:4: exec_stmt
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_exec_stmt_in_stmt239)
                    exec_stmt4 = self.exec_stmt()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, exec_stmt4.tree)



                elif alt3 == 3:
                    # Expr.g:41:4: import_stmt
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_import_stmt_in_stmt244)
                    import_stmt5 = self.import_stmt()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, import_stmt5.tree)



                elif alt3 == 4:
                    # Expr.g:42:4: print_stmt
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_print_stmt_in_stmt249)
                    print_stmt6 = self.print_stmt()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, print_stmt6.tree)



                elif alt3 == 5:
                    # Expr.g:42:17: printf_stmt
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_printf_stmt_in_stmt253)
                    printf_stmt7 = self.printf_stmt()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, printf_stmt7.tree)



                elif alt3 == 6:
                    # Expr.g:43:4: break_stmt
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_break_stmt_in_stmt258)
                    break_stmt8 = self.break_stmt()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, break_stmt8.tree)



                elif alt3 == 7:
                    # Expr.g:44:4: continue_stmt
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_continue_stmt_in_stmt263)
                    continue_stmt9 = self.continue_stmt()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, continue_stmt9.tree)



                elif alt3 == 8:
                    # Expr.g:45:4: return_stmt
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_return_stmt_in_stmt268)
                    return_stmt10 = self.return_stmt()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, return_stmt10.tree)



                elif alt3 == 9:
                    # Expr.g:46:4: if_stmt
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_if_stmt_in_stmt273)
                    if_stmt11 = self.if_stmt()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, if_stmt11.tree)



                elif alt3 == 10:
                    # Expr.g:47:4: while_stmt
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_while_stmt_in_stmt278)
                    while_stmt12 = self.while_stmt()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, while_stmt12.tree)



                elif alt3 == 11:
                    # Expr.g:48:4: do_while_stmt
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_do_while_stmt_in_stmt283)
                    do_while_stmt13 = self.do_while_stmt()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, do_while_stmt13.tree)



                elif alt3 == 12:
                    # Expr.g:49:4: switch_stmt
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_switch_stmt_in_stmt288)
                    switch_stmt14 = self.switch_stmt()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, switch_stmt14.tree)



                elif alt3 == 13:
                    # Expr.g:50:4: for_stmt
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_for_stmt_in_stmt293)
                    for_stmt15 = self.for_stmt()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, for_stmt15.tree)



                elif alt3 == 14:
                    # Expr.g:51:4: foreach_stmt
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_foreach_stmt_in_stmt298)
                    foreach_stmt16 = self.foreach_stmt()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, foreach_stmt16.tree)



                elif alt3 == 15:
                    # Expr.g:52:4: throw_stmt
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_throw_stmt_in_stmt303)
                    throw_stmt17 = self.throw_stmt()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, throw_stmt17.tree)



                elif alt3 == 16:
                    # Expr.g:53:4: try_stmt
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_try_stmt_in_stmt308)
                    try_stmt18 = self.try_stmt()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, try_stmt18.tree)



                elif alt3 == 17:
                    # Expr.g:54:4: func_decl
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_func_decl_in_stmt313)
                    func_decl19 = self.func_decl()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, func_decl19.tree)



                elif alt3 == 18:
                    # Expr.g:55:4: class_decl
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_class_decl_in_stmt318)
                    class_decl20 = self.class_decl()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, class_decl20.tree)



                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "stmt"


    class block_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.block_return, self).__init__()

            self.tree = None





    # $ANTLR start "block"
    # Expr.g:59:1: block : '{' ( stmt )* '}' -> ^( BLOCK ( stmt )* ) ;
    def block(self, ):
        retval = self.block_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal21 = None
        char_literal23 = None
        stmt22 = None

        char_literal21_tree = None
        char_literal23_tree = None
        stream_132 = RewriteRuleTokenStream(self._adaptor, "token 132")
        stream_136 = RewriteRuleTokenStream(self._adaptor, "token 136")
        stream_stmt = RewriteRuleSubtreeStream(self._adaptor, "rule stmt")
        try:
            try:
                # Expr.g:60:2: ( '{' ( stmt )* '}' -> ^( BLOCK ( stmt )* ) )
                # Expr.g:60:4: '{' ( stmt )* '}'
                pass 
                char_literal21 = self.match(self.input, 132, self.FOLLOW_132_in_block331) 
                if self._state.backtracking == 0:
                    stream_132.add(char_literal21)


                # Expr.g:60:8: ( stmt )*
                while True: #loop4
                    alt4 = 2
                    LA4_0 = self.input.LA(1)

                    if (LA4_0 == ID or LA4_0 == 80 or LA4_0 == 84 or LA4_0 == 92 or LA4_0 == 105 or (108 <= LA4_0 <= 109) or LA4_0 == 111 or (115 <= LA4_0 <= 119) or (122 <= LA4_0 <= 123) or LA4_0 == 125 or (128 <= LA4_0 <= 131)) :
                        alt4 = 1


                    if alt4 == 1:
                        # Expr.g:60:8: stmt
                        pass 
                        self._state.following.append(self.FOLLOW_stmt_in_block333)
                        stmt22 = self.stmt()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_stmt.add(stmt22.tree)



                    else:
                        break #loop4


                char_literal23 = self.match(self.input, 136, self.FOLLOW_136_in_block336) 
                if self._state.backtracking == 0:
                    stream_136.add(char_literal23)


                # AST Rewrite
                # elements: stmt
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 61:3: -> ^( BLOCK ( stmt )* )
                    # Expr.g:61:6: ^( BLOCK ( stmt )* )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(BLOCK, "BLOCK")
                    , root_1)

                    # Expr.g:61:14: ( stmt )*
                    while stream_stmt.hasNext():
                        self._adaptor.addChild(root_1, stream_stmt.nextTree())


                    stream_stmt.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "block"


    class import_stmt_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.import_stmt_return, self).__init__()

            self.tree = None





    # $ANTLR start "import_stmt"
    # Expr.g:64:1: import_stmt : 'import' module_path ( ',' module_path )* ';' -> ^( IMPORT ( module_path )+ ) ;
    def import_stmt(self, ):
        retval = self.import_stmt_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal24 = None
        char_literal26 = None
        char_literal28 = None
        module_path25 = None
        module_path27 = None

        string_literal24_tree = None
        char_literal26_tree = None
        char_literal28_tree = None
        stream_92 = RewriteRuleTokenStream(self._adaptor, "token 92")
        stream_82 = RewriteRuleTokenStream(self._adaptor, "token 82")
        stream_119 = RewriteRuleTokenStream(self._adaptor, "token 119")
        stream_module_path = RewriteRuleSubtreeStream(self._adaptor, "rule module_path")
        try:
            try:
                # Expr.g:65:2: ( 'import' module_path ( ',' module_path )* ';' -> ^( IMPORT ( module_path )+ ) )
                # Expr.g:65:4: 'import' module_path ( ',' module_path )* ';'
                pass 
                string_literal24 = self.match(self.input, 119, self.FOLLOW_119_in_import_stmt358) 
                if self._state.backtracking == 0:
                    stream_119.add(string_literal24)


                self._state.following.append(self.FOLLOW_module_path_in_import_stmt360)
                module_path25 = self.module_path()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_module_path.add(module_path25.tree)


                # Expr.g:65:25: ( ',' module_path )*
                while True: #loop5
                    alt5 = 2
                    LA5_0 = self.input.LA(1)

                    if (LA5_0 == 82) :
                        alt5 = 1


                    if alt5 == 1:
                        # Expr.g:65:26: ',' module_path
                        pass 
                        char_literal26 = self.match(self.input, 82, self.FOLLOW_82_in_import_stmt363) 
                        if self._state.backtracking == 0:
                            stream_82.add(char_literal26)


                        self._state.following.append(self.FOLLOW_module_path_in_import_stmt365)
                        module_path27 = self.module_path()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_module_path.add(module_path27.tree)



                    else:
                        break #loop5


                char_literal28 = self.match(self.input, 92, self.FOLLOW_92_in_import_stmt369) 
                if self._state.backtracking == 0:
                    stream_92.add(char_literal28)


                # AST Rewrite
                # elements: module_path
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 66:3: -> ^( IMPORT ( module_path )+ )
                    # Expr.g:66:6: ^( IMPORT ( module_path )+ )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(IMPORT, "IMPORT")
                    , root_1)

                    # Expr.g:66:15: ( module_path )+
                    if not (stream_module_path.hasNext()):
                        raise RewriteEarlyExitException()

                    while stream_module_path.hasNext():
                        self._adaptor.addChild(root_1, stream_module_path.nextTree())


                    stream_module_path.reset()

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "import_stmt"


    class module_path_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.module_path_return, self).__init__()

            self.tree = None





    # $ANTLR start "module_path"
    # Expr.g:68:1: module_path : ( module | module '.*' );
    def module_path(self, ):
        retval = self.module_path_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal31 = None
        module29 = None
        module30 = None

        string_literal31_tree = None

        try:
            try:
                # Expr.g:69:2: ( module | module '.*' )
                alt6 = 2
                alt6 = self.dfa6.predict(self.input)
                if alt6 == 1:
                    # Expr.g:69:4: module
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_module_in_module_path390)
                    module29 = self.module()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, module29.tree)



                elif alt6 == 2:
                    # Expr.g:70:4: module '.*'
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_module_in_module_path395)
                    module30 = self.module()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, module30.tree)


                    string_literal31 = self.match(self.input, 87, self.FOLLOW_87_in_module_path397)
                    if self._state.backtracking == 0:
                        string_literal31_tree = self._adaptor.createWithPayload(string_literal31)
                        self._adaptor.addChild(root_0, string_literal31_tree)




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "module_path"


    class printf_stmt_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.printf_stmt_return, self).__init__()

            self.tree = None





    # $ANTLR start "printf_stmt"
    # Expr.g:73:1: printf_stmt : 'printf' '(' expr ( ',' expr_list )? ')' ';' -> ^( PRINTF expr ( expr_list )? ) ;
    def printf_stmt(self, ):
        retval = self.printf_stmt_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal32 = None
        char_literal33 = None
        char_literal35 = None
        char_literal37 = None
        char_literal38 = None
        expr34 = None
        expr_list36 = None

        string_literal32_tree = None
        char_literal33_tree = None
        char_literal35_tree = None
        char_literal37_tree = None
        char_literal38_tree = None
        stream_92 = RewriteRuleTokenStream(self._adaptor, "token 92")
        stream_123 = RewriteRuleTokenStream(self._adaptor, "token 123")
        stream_82 = RewriteRuleTokenStream(self._adaptor, "token 82")
        stream_75 = RewriteRuleTokenStream(self._adaptor, "token 75")
        stream_76 = RewriteRuleTokenStream(self._adaptor, "token 76")
        stream_expr = RewriteRuleSubtreeStream(self._adaptor, "rule expr")
        stream_expr_list = RewriteRuleSubtreeStream(self._adaptor, "rule expr_list")
        try:
            try:
                # Expr.g:74:2: ( 'printf' '(' expr ( ',' expr_list )? ')' ';' -> ^( PRINTF expr ( expr_list )? ) )
                # Expr.g:74:4: 'printf' '(' expr ( ',' expr_list )? ')' ';'
                pass 
                string_literal32 = self.match(self.input, 123, self.FOLLOW_123_in_printf_stmt408) 
                if self._state.backtracking == 0:
                    stream_123.add(string_literal32)


                char_literal33 = self.match(self.input, 75, self.FOLLOW_75_in_printf_stmt410) 
                if self._state.backtracking == 0:
                    stream_75.add(char_literal33)


                self._state.following.append(self.FOLLOW_expr_in_printf_stmt412)
                expr34 = self.expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_expr.add(expr34.tree)


                # Expr.g:74:22: ( ',' expr_list )?
                alt7 = 2
                LA7_0 = self.input.LA(1)

                if (LA7_0 == 82) :
                    alt7 = 1
                if alt7 == 1:
                    # Expr.g:74:23: ',' expr_list
                    pass 
                    char_literal35 = self.match(self.input, 82, self.FOLLOW_82_in_printf_stmt415) 
                    if self._state.backtracking == 0:
                        stream_82.add(char_literal35)


                    self._state.following.append(self.FOLLOW_expr_list_in_printf_stmt417)
                    expr_list36 = self.expr_list()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_expr_list.add(expr_list36.tree)





                char_literal37 = self.match(self.input, 76, self.FOLLOW_76_in_printf_stmt421) 
                if self._state.backtracking == 0:
                    stream_76.add(char_literal37)


                char_literal38 = self.match(self.input, 92, self.FOLLOW_92_in_printf_stmt423) 
                if self._state.backtracking == 0:
                    stream_92.add(char_literal38)


                # AST Rewrite
                # elements: expr_list, expr
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 75:3: -> ^( PRINTF expr ( expr_list )? )
                    # Expr.g:75:6: ^( PRINTF expr ( expr_list )? )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(PRINTF, "PRINTF")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_expr.nextTree())

                    # Expr.g:75:20: ( expr_list )?
                    if stream_expr_list.hasNext():
                        self._adaptor.addChild(root_1, stream_expr_list.nextTree())


                    stream_expr_list.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "printf_stmt"


    class print_stmt_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.print_stmt_return, self).__init__()

            self.tree = None





    # $ANTLR start "print_stmt"
    # Expr.g:78:1: print_stmt : ( 'print' ) expr_list ';' -> ^( PRINT expr_list ) ;
    def print_stmt(self, ):
        retval = self.print_stmt_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal39 = None
        char_literal41 = None
        expr_list40 = None

        string_literal39_tree = None
        char_literal41_tree = None
        stream_122 = RewriteRuleTokenStream(self._adaptor, "token 122")
        stream_92 = RewriteRuleTokenStream(self._adaptor, "token 92")
        stream_expr_list = RewriteRuleSubtreeStream(self._adaptor, "rule expr_list")
        try:
            try:
                # Expr.g:81:2: ( ( 'print' ) expr_list ';' -> ^( PRINT expr_list ) )
                # Expr.g:81:4: ( 'print' ) expr_list ';'
                pass 
                # Expr.g:81:4: ( 'print' )
                # Expr.g:81:5: 'print'
                pass 
                string_literal39 = self.match(self.input, 122, self.FOLLOW_122_in_print_stmt452) 
                if self._state.backtracking == 0:
                    stream_122.add(string_literal39)





                self._state.following.append(self.FOLLOW_expr_list_in_print_stmt455)
                expr_list40 = self.expr_list()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_expr_list.add(expr_list40.tree)


                char_literal41 = self.match(self.input, 92, self.FOLLOW_92_in_print_stmt457) 
                if self._state.backtracking == 0:
                    stream_92.add(char_literal41)


                # AST Rewrite
                # elements: expr_list
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 82:3: -> ^( PRINT expr_list )
                    # Expr.g:82:6: ^( PRINT expr_list )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(PRINT, "PRINT")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_expr_list.nextTree())

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "print_stmt"


    class break_stmt_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.break_stmt_return, self).__init__()

            self.tree = None





    # $ANTLR start "break_stmt"
    # Expr.g:85:1: break_stmt : 'break' ';' -> BREAK ;
    def break_stmt(self, ):
        retval = self.break_stmt_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal42 = None
        char_literal43 = None

        string_literal42_tree = None
        char_literal43_tree = None
        stream_92 = RewriteRuleTokenStream(self._adaptor, "token 92")
        stream_105 = RewriteRuleTokenStream(self._adaptor, "token 105")

        try:
            try:
                # Expr.g:86:2: ( 'break' ';' -> BREAK )
                # Expr.g:86:4: 'break' ';'
                pass 
                string_literal42 = self.match(self.input, 105, self.FOLLOW_105_in_break_stmt478) 
                if self._state.backtracking == 0:
                    stream_105.add(string_literal42)


                char_literal43 = self.match(self.input, 92, self.FOLLOW_92_in_break_stmt480) 
                if self._state.backtracking == 0:
                    stream_92.add(char_literal43)


                # AST Rewrite
                # elements: 
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 87:3: -> BREAK
                    self._adaptor.addChild(root_0, 
                    self._adaptor.createFromType(BREAK, "BREAK")
                    )




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "break_stmt"


    class continue_stmt_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.continue_stmt_return, self).__init__()

            self.tree = None





    # $ANTLR start "continue_stmt"
    # Expr.g:89:1: continue_stmt : 'continue' ';' -> CONTINUE ;
    def continue_stmt(self, ):
        retval = self.continue_stmt_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal44 = None
        char_literal45 = None

        string_literal44_tree = None
        char_literal45_tree = None
        stream_109 = RewriteRuleTokenStream(self._adaptor, "token 109")
        stream_92 = RewriteRuleTokenStream(self._adaptor, "token 92")

        try:
            try:
                # Expr.g:90:2: ( 'continue' ';' -> CONTINUE )
                # Expr.g:90:4: 'continue' ';'
                pass 
                string_literal44 = self.match(self.input, 109, self.FOLLOW_109_in_continue_stmt496) 
                if self._state.backtracking == 0:
                    stream_109.add(string_literal44)


                char_literal45 = self.match(self.input, 92, self.FOLLOW_92_in_continue_stmt498) 
                if self._state.backtracking == 0:
                    stream_92.add(char_literal45)


                # AST Rewrite
                # elements: 
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 91:3: -> CONTINUE
                    self._adaptor.addChild(root_0, 
                    self._adaptor.createFromType(CONTINUE, "CONTINUE")
                    )




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "continue_stmt"


    class return_stmt_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.return_stmt_return, self).__init__()

            self.tree = None





    # $ANTLR start "return_stmt"
    # Expr.g:93:1: return_stmt : 'return' ( expr )? ';' -> ^( RETURN ( expr )? ) ;
    def return_stmt(self, ):
        retval = self.return_stmt_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal46 = None
        char_literal48 = None
        expr47 = None

        string_literal46_tree = None
        char_literal48_tree = None
        stream_125 = RewriteRuleTokenStream(self._adaptor, "token 125")
        stream_92 = RewriteRuleTokenStream(self._adaptor, "token 92")
        stream_expr = RewriteRuleSubtreeStream(self._adaptor, "rule expr")
        try:
            try:
                # Expr.g:94:2: ( 'return' ( expr )? ';' -> ^( RETURN ( expr )? ) )
                # Expr.g:94:4: 'return' ( expr )? ';'
                pass 
                string_literal46 = self.match(self.input, 125, self.FOLLOW_125_in_return_stmt514) 
                if self._state.backtracking == 0:
                    stream_125.add(string_literal46)


                # Expr.g:94:13: ( expr )?
                alt8 = 2
                LA8_0 = self.input.LA(1)

                if (LA8_0 == BOOL or LA8_0 == FLOAT or LA8_0 == ID or LA8_0 == INT or LA8_0 == NULL or LA8_0 == STRING or LA8_0 == 68 or LA8_0 == 75 or LA8_0 == 83 or LA8_0 == 100 or LA8_0 == 121 or LA8_0 == 126 or LA8_0 == 132) :
                    alt8 = 1
                if alt8 == 1:
                    # Expr.g:94:13: expr
                    pass 
                    self._state.following.append(self.FOLLOW_expr_in_return_stmt516)
                    expr47 = self.expr()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_expr.add(expr47.tree)





                char_literal48 = self.match(self.input, 92, self.FOLLOW_92_in_return_stmt519) 
                if self._state.backtracking == 0:
                    stream_92.add(char_literal48)


                # AST Rewrite
                # elements: expr
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 95:3: -> ^( RETURN ( expr )? )
                    # Expr.g:95:6: ^( RETURN ( expr )? )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(RETURN, "RETURN")
                    , root_1)

                    # Expr.g:95:15: ( expr )?
                    if stream_expr.hasNext():
                        self._adaptor.addChild(root_1, stream_expr.nextTree())


                    stream_expr.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "return_stmt"


    class if_stmt_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.if_stmt_return, self).__init__()

            self.tree = None





    # $ANTLR start "if_stmt"
    # Expr.g:98:1: if_stmt : if_clause ( else_if_clause )* ( else_clause )? ;
    def if_stmt(self, ):
        retval = self.if_stmt_return()
        retval.start = self.input.LT(1)


        root_0 = None

        if_clause49 = None
        else_if_clause50 = None
        else_clause51 = None


        try:
            try:
                # Expr.g:99:2: ( if_clause ( else_if_clause )* ( else_clause )? )
                # Expr.g:99:4: if_clause ( else_if_clause )* ( else_clause )?
                pass 
                root_0 = self._adaptor.nil()


                self._state.following.append(self.FOLLOW_if_clause_in_if_stmt541)
                if_clause49 = self.if_clause()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    self._adaptor.addChild(root_0, if_clause49.tree)


                # Expr.g:99:14: ( else_if_clause )*
                while True: #loop9
                    alt9 = 2
                    LA9_0 = self.input.LA(1)

                    if (LA9_0 == 112) :
                        LA9_1 = self.input.LA(2)

                        if (LA9_1 == 118) :
                            alt9 = 1




                    if alt9 == 1:
                        # Expr.g:99:14: else_if_clause
                        pass 
                        self._state.following.append(self.FOLLOW_else_if_clause_in_if_stmt543)
                        else_if_clause50 = self.else_if_clause()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            self._adaptor.addChild(root_0, else_if_clause50.tree)



                    else:
                        break #loop9


                # Expr.g:99:30: ( else_clause )?
                alt10 = 2
                LA10_0 = self.input.LA(1)

                if (LA10_0 == 112) :
                    alt10 = 1
                if alt10 == 1:
                    # Expr.g:99:30: else_clause
                    pass 
                    self._state.following.append(self.FOLLOW_else_clause_in_if_stmt546)
                    else_clause51 = self.else_clause()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, else_clause51.tree)







                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "if_stmt"


    class if_clause_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.if_clause_return, self).__init__()

            self.tree = None





    # $ANTLR start "if_clause"
    # Expr.g:101:1: if_clause : 'if' '(' expr ')' block -> ^( IF expr block ) ;
    def if_clause(self, ):
        retval = self.if_clause_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal52 = None
        char_literal53 = None
        char_literal55 = None
        expr54 = None
        block56 = None

        string_literal52_tree = None
        char_literal53_tree = None
        char_literal55_tree = None
        stream_75 = RewriteRuleTokenStream(self._adaptor, "token 75")
        stream_118 = RewriteRuleTokenStream(self._adaptor, "token 118")
        stream_76 = RewriteRuleTokenStream(self._adaptor, "token 76")
        stream_block = RewriteRuleSubtreeStream(self._adaptor, "rule block")
        stream_expr = RewriteRuleSubtreeStream(self._adaptor, "rule expr")
        try:
            try:
                # Expr.g:102:2: ( 'if' '(' expr ')' block -> ^( IF expr block ) )
                # Expr.g:102:4: 'if' '(' expr ')' block
                pass 
                string_literal52 = self.match(self.input, 118, self.FOLLOW_118_in_if_clause557) 
                if self._state.backtracking == 0:
                    stream_118.add(string_literal52)


                char_literal53 = self.match(self.input, 75, self.FOLLOW_75_in_if_clause559) 
                if self._state.backtracking == 0:
                    stream_75.add(char_literal53)


                self._state.following.append(self.FOLLOW_expr_in_if_clause561)
                expr54 = self.expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_expr.add(expr54.tree)


                char_literal55 = self.match(self.input, 76, self.FOLLOW_76_in_if_clause563) 
                if self._state.backtracking == 0:
                    stream_76.add(char_literal55)


                self._state.following.append(self.FOLLOW_block_in_if_clause565)
                block56 = self.block()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_block.add(block56.tree)


                # AST Rewrite
                # elements: block, expr
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 103:3: -> ^( IF expr block )
                    # Expr.g:103:6: ^( IF expr block )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(IF, "IF")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_expr.nextTree())

                    self._adaptor.addChild(root_1, stream_block.nextTree())

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "if_clause"


    class else_if_clause_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.else_if_clause_return, self).__init__()

            self.tree = None





    # $ANTLR start "else_if_clause"
    # Expr.g:105:1: else_if_clause : 'else' if_clause -> ^( ELSE_IF if_clause ) ;
    def else_if_clause(self, ):
        retval = self.else_if_clause_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal57 = None
        if_clause58 = None

        string_literal57_tree = None
        stream_112 = RewriteRuleTokenStream(self._adaptor, "token 112")
        stream_if_clause = RewriteRuleSubtreeStream(self._adaptor, "rule if_clause")
        try:
            try:
                # Expr.g:106:2: ( 'else' if_clause -> ^( ELSE_IF if_clause ) )
                # Expr.g:106:4: 'else' if_clause
                pass 
                string_literal57 = self.match(self.input, 112, self.FOLLOW_112_in_else_if_clause587) 
                if self._state.backtracking == 0:
                    stream_112.add(string_literal57)


                self._state.following.append(self.FOLLOW_if_clause_in_else_if_clause589)
                if_clause58 = self.if_clause()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_if_clause.add(if_clause58.tree)


                # AST Rewrite
                # elements: if_clause
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 107:3: -> ^( ELSE_IF if_clause )
                    # Expr.g:107:6: ^( ELSE_IF if_clause )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(ELSE_IF, "ELSE_IF")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_if_clause.nextTree())

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "else_if_clause"


    class else_clause_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.else_clause_return, self).__init__()

            self.tree = None





    # $ANTLR start "else_clause"
    # Expr.g:109:1: else_clause : 'else' block -> ^( ELSE block ) ;
    def else_clause(self, ):
        retval = self.else_clause_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal59 = None
        block60 = None

        string_literal59_tree = None
        stream_112 = RewriteRuleTokenStream(self._adaptor, "token 112")
        stream_block = RewriteRuleSubtreeStream(self._adaptor, "rule block")
        try:
            try:
                # Expr.g:110:2: ( 'else' block -> ^( ELSE block ) )
                # Expr.g:110:4: 'else' block
                pass 
                string_literal59 = self.match(self.input, 112, self.FOLLOW_112_in_else_clause609) 
                if self._state.backtracking == 0:
                    stream_112.add(string_literal59)


                self._state.following.append(self.FOLLOW_block_in_else_clause611)
                block60 = self.block()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_block.add(block60.tree)


                # AST Rewrite
                # elements: block
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 111:3: -> ^( ELSE block )
                    # Expr.g:111:6: ^( ELSE block )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(ELSE, "ELSE")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_block.nextTree())

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "else_clause"


    class while_stmt_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.while_stmt_return, self).__init__()

            self.tree = None





    # $ANTLR start "while_stmt"
    # Expr.g:114:1: while_stmt : 'while' '(' expr ')' block -> ^( WHILE expr block ) ;
    def while_stmt(self, ):
        retval = self.while_stmt_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal61 = None
        char_literal62 = None
        char_literal64 = None
        expr63 = None
        block65 = None

        string_literal61_tree = None
        char_literal62_tree = None
        char_literal64_tree = None
        stream_131 = RewriteRuleTokenStream(self._adaptor, "token 131")
        stream_75 = RewriteRuleTokenStream(self._adaptor, "token 75")
        stream_76 = RewriteRuleTokenStream(self._adaptor, "token 76")
        stream_block = RewriteRuleSubtreeStream(self._adaptor, "rule block")
        stream_expr = RewriteRuleSubtreeStream(self._adaptor, "rule expr")
        try:
            try:
                # Expr.g:115:2: ( 'while' '(' expr ')' block -> ^( WHILE expr block ) )
                # Expr.g:115:4: 'while' '(' expr ')' block
                pass 
                string_literal61 = self.match(self.input, 131, self.FOLLOW_131_in_while_stmt632) 
                if self._state.backtracking == 0:
                    stream_131.add(string_literal61)


                char_literal62 = self.match(self.input, 75, self.FOLLOW_75_in_while_stmt634) 
                if self._state.backtracking == 0:
                    stream_75.add(char_literal62)


                self._state.following.append(self.FOLLOW_expr_in_while_stmt636)
                expr63 = self.expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_expr.add(expr63.tree)


                char_literal64 = self.match(self.input, 76, self.FOLLOW_76_in_while_stmt638) 
                if self._state.backtracking == 0:
                    stream_76.add(char_literal64)


                self._state.following.append(self.FOLLOW_block_in_while_stmt640)
                block65 = self.block()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_block.add(block65.tree)


                # AST Rewrite
                # elements: expr, block
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 116:3: -> ^( WHILE expr block )
                    # Expr.g:116:6: ^( WHILE expr block )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(WHILE, "WHILE")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_expr.nextTree())

                    self._adaptor.addChild(root_1, stream_block.nextTree())

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "while_stmt"


    class do_while_stmt_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.do_while_stmt_return, self).__init__()

            self.tree = None





    # $ANTLR start "do_while_stmt"
    # Expr.g:119:1: do_while_stmt : 'do' block 'while' '(' expr ')' ';' -> ^( DO_WHILE block expr ) ;
    def do_while_stmt(self, ):
        retval = self.do_while_stmt_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal66 = None
        string_literal68 = None
        char_literal69 = None
        char_literal71 = None
        char_literal72 = None
        block67 = None
        expr70 = None

        string_literal66_tree = None
        string_literal68_tree = None
        char_literal69_tree = None
        char_literal71_tree = None
        char_literal72_tree = None
        stream_92 = RewriteRuleTokenStream(self._adaptor, "token 92")
        stream_111 = RewriteRuleTokenStream(self._adaptor, "token 111")
        stream_131 = RewriteRuleTokenStream(self._adaptor, "token 131")
        stream_75 = RewriteRuleTokenStream(self._adaptor, "token 75")
        stream_76 = RewriteRuleTokenStream(self._adaptor, "token 76")
        stream_block = RewriteRuleSubtreeStream(self._adaptor, "rule block")
        stream_expr = RewriteRuleSubtreeStream(self._adaptor, "rule expr")
        try:
            try:
                # Expr.g:120:2: ( 'do' block 'while' '(' expr ')' ';' -> ^( DO_WHILE block expr ) )
                # Expr.g:120:4: 'do' block 'while' '(' expr ')' ';'
                pass 
                string_literal66 = self.match(self.input, 111, self.FOLLOW_111_in_do_while_stmt663) 
                if self._state.backtracking == 0:
                    stream_111.add(string_literal66)


                self._state.following.append(self.FOLLOW_block_in_do_while_stmt665)
                block67 = self.block()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_block.add(block67.tree)


                string_literal68 = self.match(self.input, 131, self.FOLLOW_131_in_do_while_stmt667) 
                if self._state.backtracking == 0:
                    stream_131.add(string_literal68)


                char_literal69 = self.match(self.input, 75, self.FOLLOW_75_in_do_while_stmt669) 
                if self._state.backtracking == 0:
                    stream_75.add(char_literal69)


                self._state.following.append(self.FOLLOW_expr_in_do_while_stmt671)
                expr70 = self.expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_expr.add(expr70.tree)


                char_literal71 = self.match(self.input, 76, self.FOLLOW_76_in_do_while_stmt673) 
                if self._state.backtracking == 0:
                    stream_76.add(char_literal71)


                char_literal72 = self.match(self.input, 92, self.FOLLOW_92_in_do_while_stmt675) 
                if self._state.backtracking == 0:
                    stream_92.add(char_literal72)


                # AST Rewrite
                # elements: expr, block
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 121:3: -> ^( DO_WHILE block expr )
                    # Expr.g:121:6: ^( DO_WHILE block expr )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(DO_WHILE, "DO_WHILE")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_block.nextTree())

                    self._adaptor.addChild(root_1, stream_expr.nextTree())

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "do_while_stmt"


    class switch_stmt_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.switch_stmt_return, self).__init__()

            self.tree = None





    # $ANTLR start "switch_stmt"
    # Expr.g:124:1: switch_stmt : 'switch' '(' expr ')' case_block -> ^( SWITCH expr case_block ) ;
    def switch_stmt(self, ):
        retval = self.switch_stmt_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal73 = None
        char_literal74 = None
        char_literal76 = None
        expr75 = None
        case_block77 = None

        string_literal73_tree = None
        char_literal74_tree = None
        char_literal76_tree = None
        stream_128 = RewriteRuleTokenStream(self._adaptor, "token 128")
        stream_75 = RewriteRuleTokenStream(self._adaptor, "token 75")
        stream_76 = RewriteRuleTokenStream(self._adaptor, "token 76")
        stream_case_block = RewriteRuleSubtreeStream(self._adaptor, "rule case_block")
        stream_expr = RewriteRuleSubtreeStream(self._adaptor, "rule expr")
        try:
            try:
                # Expr.g:125:2: ( 'switch' '(' expr ')' case_block -> ^( SWITCH expr case_block ) )
                # Expr.g:125:4: 'switch' '(' expr ')' case_block
                pass 
                string_literal73 = self.match(self.input, 128, self.FOLLOW_128_in_switch_stmt698) 
                if self._state.backtracking == 0:
                    stream_128.add(string_literal73)


                char_literal74 = self.match(self.input, 75, self.FOLLOW_75_in_switch_stmt700) 
                if self._state.backtracking == 0:
                    stream_75.add(char_literal74)


                self._state.following.append(self.FOLLOW_expr_in_switch_stmt702)
                expr75 = self.expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_expr.add(expr75.tree)


                char_literal76 = self.match(self.input, 76, self.FOLLOW_76_in_switch_stmt704) 
                if self._state.backtracking == 0:
                    stream_76.add(char_literal76)


                self._state.following.append(self.FOLLOW_case_block_in_switch_stmt706)
                case_block77 = self.case_block()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_case_block.add(case_block77.tree)


                # AST Rewrite
                # elements: case_block, expr
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 126:3: -> ^( SWITCH expr case_block )
                    # Expr.g:126:6: ^( SWITCH expr case_block )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(SWITCH, "SWITCH")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_expr.nextTree())

                    self._adaptor.addChild(root_1, stream_case_block.nextTree())

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "switch_stmt"


    class case_block_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.case_block_return, self).__init__()

            self.tree = None





    # $ANTLR start "case_block"
    # Expr.g:128:1: case_block : '{' ( case_clause )+ ( default_clause )? '}' ;
    def case_block(self, ):
        retval = self.case_block_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal78 = None
        char_literal81 = None
        case_clause79 = None
        default_clause80 = None

        char_literal78_tree = None
        char_literal81_tree = None

        try:
            try:
                # Expr.g:129:2: ( '{' ( case_clause )+ ( default_clause )? '}' )
                # Expr.g:129:4: '{' ( case_clause )+ ( default_clause )? '}'
                pass 
                root_0 = self._adaptor.nil()


                char_literal78 = self.match(self.input, 132, self.FOLLOW_132_in_case_block728)
                if self._state.backtracking == 0:
                    char_literal78_tree = self._adaptor.createWithPayload(char_literal78)
                    self._adaptor.addChild(root_0, char_literal78_tree)



                # Expr.g:129:8: ( case_clause )+
                cnt11 = 0
                while True: #loop11
                    alt11 = 2
                    LA11_0 = self.input.LA(1)

                    if (LA11_0 == 106) :
                        alt11 = 1


                    if alt11 == 1:
                        # Expr.g:129:9: case_clause
                        pass 
                        self._state.following.append(self.FOLLOW_case_clause_in_case_block731)
                        case_clause79 = self.case_clause()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            self._adaptor.addChild(root_0, case_clause79.tree)



                    else:
                        if cnt11 >= 1:
                            break #loop11

                        if self._state.backtracking > 0:
                            raise BacktrackingFailed


                        eee = EarlyExitException(11, self.input)
                        raise eee

                    cnt11 += 1


                # Expr.g:129:23: ( default_clause )?
                alt12 = 2
                LA12_0 = self.input.LA(1)

                if (LA12_0 == 110) :
                    alt12 = 1
                if alt12 == 1:
                    # Expr.g:129:24: default_clause
                    pass 
                    self._state.following.append(self.FOLLOW_default_clause_in_case_block736)
                    default_clause80 = self.default_clause()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, default_clause80.tree)





                char_literal81 = self.match(self.input, 136, self.FOLLOW_136_in_case_block740)
                if self._state.backtracking == 0:
                    char_literal81_tree = self._adaptor.createWithPayload(char_literal81)
                    self._adaptor.addChild(root_0, char_literal81_tree)





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "case_block"


    class case_clause_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.case_clause_return, self).__init__()

            self.tree = None





    # $ANTLR start "case_clause"
    # Expr.g:131:1: case_clause : ( case_test )+ ( stmt )* break_stmt -> ^( CASE ( case_test )+ ( stmt )* break_stmt ) ;
    def case_clause(self, ):
        retval = self.case_clause_return()
        retval.start = self.input.LT(1)


        root_0 = None

        case_test82 = None
        stmt83 = None
        break_stmt84 = None

        stream_case_test = RewriteRuleSubtreeStream(self._adaptor, "rule case_test")
        stream_stmt = RewriteRuleSubtreeStream(self._adaptor, "rule stmt")
        stream_break_stmt = RewriteRuleSubtreeStream(self._adaptor, "rule break_stmt")
        try:
            try:
                # Expr.g:132:2: ( ( case_test )+ ( stmt )* break_stmt -> ^( CASE ( case_test )+ ( stmt )* break_stmt ) )
                # Expr.g:132:4: ( case_test )+ ( stmt )* break_stmt
                pass 
                # Expr.g:132:4: ( case_test )+
                cnt13 = 0
                while True: #loop13
                    alt13 = 2
                    LA13_0 = self.input.LA(1)

                    if (LA13_0 == 106) :
                        alt13 = 1


                    if alt13 == 1:
                        # Expr.g:132:4: case_test
                        pass 
                        self._state.following.append(self.FOLLOW_case_test_in_case_clause750)
                        case_test82 = self.case_test()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_case_test.add(case_test82.tree)



                    else:
                        if cnt13 >= 1:
                            break #loop13

                        if self._state.backtracking > 0:
                            raise BacktrackingFailed


                        eee = EarlyExitException(13, self.input)
                        raise eee

                    cnt13 += 1


                # Expr.g:132:15: ( stmt )*
                while True: #loop14
                    alt14 = 2
                    LA14_0 = self.input.LA(1)

                    if (LA14_0 == 105) :
                        LA14_1 = self.input.LA(2)

                        if (LA14_1 == 92) :
                            LA14_3 = self.input.LA(3)

                            if (LA14_3 == ID or LA14_3 == 80 or LA14_3 == 84 or LA14_3 == 92 or LA14_3 == 105 or (108 <= LA14_3 <= 109) or LA14_3 == 111 or (115 <= LA14_3 <= 119) or (122 <= LA14_3 <= 123) or LA14_3 == 125 or (128 <= LA14_3 <= 131)) :
                                alt14 = 1




                    elif (LA14_0 == ID or LA14_0 == 80 or LA14_0 == 84 or LA14_0 == 92 or (108 <= LA14_0 <= 109) or LA14_0 == 111 or (115 <= LA14_0 <= 119) or (122 <= LA14_0 <= 123) or LA14_0 == 125 or (128 <= LA14_0 <= 131)) :
                        alt14 = 1


                    if alt14 == 1:
                        # Expr.g:132:15: stmt
                        pass 
                        self._state.following.append(self.FOLLOW_stmt_in_case_clause753)
                        stmt83 = self.stmt()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_stmt.add(stmt83.tree)



                    else:
                        break #loop14


                self._state.following.append(self.FOLLOW_break_stmt_in_case_clause756)
                break_stmt84 = self.break_stmt()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_break_stmt.add(break_stmt84.tree)


                # AST Rewrite
                # elements: stmt, case_test, break_stmt
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 133:3: -> ^( CASE ( case_test )+ ( stmt )* break_stmt )
                    # Expr.g:133:6: ^( CASE ( case_test )+ ( stmt )* break_stmt )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(CASE, "CASE")
                    , root_1)

                    # Expr.g:133:13: ( case_test )+
                    if not (stream_case_test.hasNext()):
                        raise RewriteEarlyExitException()

                    while stream_case_test.hasNext():
                        self._adaptor.addChild(root_1, stream_case_test.nextTree())


                    stream_case_test.reset()

                    # Expr.g:133:24: ( stmt )*
                    while stream_stmt.hasNext():
                        self._adaptor.addChild(root_1, stream_stmt.nextTree())


                    stream_stmt.reset();

                    self._adaptor.addChild(root_1, stream_break_stmt.nextTree())

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "case_clause"


    class case_test_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.case_test_return, self).__init__()

            self.tree = None





    # $ANTLR start "case_test"
    # Expr.g:135:1: case_test : 'case' expr ':' -> ^( CASE expr ) ;
    def case_test(self, ):
        retval = self.case_test_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal85 = None
        char_literal87 = None
        expr86 = None

        string_literal85_tree = None
        char_literal87_tree = None
        stream_91 = RewriteRuleTokenStream(self._adaptor, "token 91")
        stream_106 = RewriteRuleTokenStream(self._adaptor, "token 106")
        stream_expr = RewriteRuleSubtreeStream(self._adaptor, "rule expr")
        try:
            try:
                # Expr.g:136:2: ( 'case' expr ':' -> ^( CASE expr ) )
                # Expr.g:136:4: 'case' expr ':'
                pass 
                string_literal85 = self.match(self.input, 106, self.FOLLOW_106_in_case_test782) 
                if self._state.backtracking == 0:
                    stream_106.add(string_literal85)


                self._state.following.append(self.FOLLOW_expr_in_case_test784)
                expr86 = self.expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_expr.add(expr86.tree)


                char_literal87 = self.match(self.input, 91, self.FOLLOW_91_in_case_test786) 
                if self._state.backtracking == 0:
                    stream_91.add(char_literal87)


                # AST Rewrite
                # elements: expr
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 137:3: -> ^( CASE expr )
                    # Expr.g:137:6: ^( CASE expr )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(CASE, "CASE")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_expr.nextTree())

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "case_test"


    class default_clause_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.default_clause_return, self).__init__()

            self.tree = None





    # $ANTLR start "default_clause"
    # Expr.g:139:1: default_clause : 'default' ':' ( stmt )* -> ^( DEFAULT ( stmt )* ) ;
    def default_clause(self, ):
        retval = self.default_clause_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal88 = None
        char_literal89 = None
        stmt90 = None

        string_literal88_tree = None
        char_literal89_tree = None
        stream_110 = RewriteRuleTokenStream(self._adaptor, "token 110")
        stream_91 = RewriteRuleTokenStream(self._adaptor, "token 91")
        stream_stmt = RewriteRuleSubtreeStream(self._adaptor, "rule stmt")
        try:
            try:
                # Expr.g:140:2: ( 'default' ':' ( stmt )* -> ^( DEFAULT ( stmt )* ) )
                # Expr.g:140:4: 'default' ':' ( stmt )*
                pass 
                string_literal88 = self.match(self.input, 110, self.FOLLOW_110_in_default_clause806) 
                if self._state.backtracking == 0:
                    stream_110.add(string_literal88)


                char_literal89 = self.match(self.input, 91, self.FOLLOW_91_in_default_clause808) 
                if self._state.backtracking == 0:
                    stream_91.add(char_literal89)


                # Expr.g:140:18: ( stmt )*
                while True: #loop15
                    alt15 = 2
                    LA15_0 = self.input.LA(1)

                    if (LA15_0 == ID or LA15_0 == 80 or LA15_0 == 84 or LA15_0 == 92 or LA15_0 == 105 or (108 <= LA15_0 <= 109) or LA15_0 == 111 or (115 <= LA15_0 <= 119) or (122 <= LA15_0 <= 123) or LA15_0 == 125 or (128 <= LA15_0 <= 131)) :
                        alt15 = 1


                    if alt15 == 1:
                        # Expr.g:140:18: stmt
                        pass 
                        self._state.following.append(self.FOLLOW_stmt_in_default_clause810)
                        stmt90 = self.stmt()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_stmt.add(stmt90.tree)



                    else:
                        break #loop15


                # AST Rewrite
                # elements: stmt
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 141:3: -> ^( DEFAULT ( stmt )* )
                    # Expr.g:141:6: ^( DEFAULT ( stmt )* )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(DEFAULT, "DEFAULT")
                    , root_1)

                    # Expr.g:141:16: ( stmt )*
                    while stream_stmt.hasNext():
                        self._adaptor.addChild(root_1, stream_stmt.nextTree())


                    stream_stmt.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "default_clause"


    class for_stmt_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.for_stmt_return, self).__init__()

            self.tree = None





    # $ANTLR start "for_stmt"
    # Expr.g:144:1: for_stmt : 'for' '(' (a= exec_list )? ';' expr ';' (b= exec_list )? ')' block -> ^( FOR ( $a)? expr block ( $b)? ) ;
    def for_stmt(self, ):
        retval = self.for_stmt_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal91 = None
        char_literal92 = None
        char_literal93 = None
        char_literal95 = None
        char_literal96 = None
        a = None
        b = None
        expr94 = None
        block97 = None

        string_literal91_tree = None
        char_literal92_tree = None
        char_literal93_tree = None
        char_literal95_tree = None
        char_literal96_tree = None
        stream_115 = RewriteRuleTokenStream(self._adaptor, "token 115")
        stream_92 = RewriteRuleTokenStream(self._adaptor, "token 92")
        stream_75 = RewriteRuleTokenStream(self._adaptor, "token 75")
        stream_76 = RewriteRuleTokenStream(self._adaptor, "token 76")
        stream_block = RewriteRuleSubtreeStream(self._adaptor, "rule block")
        stream_expr = RewriteRuleSubtreeStream(self._adaptor, "rule expr")
        stream_exec_list = RewriteRuleSubtreeStream(self._adaptor, "rule exec_list")
        try:
            try:
                # Expr.g:145:2: ( 'for' '(' (a= exec_list )? ';' expr ';' (b= exec_list )? ')' block -> ^( FOR ( $a)? expr block ( $b)? ) )
                # Expr.g:145:4: 'for' '(' (a= exec_list )? ';' expr ';' (b= exec_list )? ')' block
                pass 
                string_literal91 = self.match(self.input, 115, self.FOLLOW_115_in_for_stmt833) 
                if self._state.backtracking == 0:
                    stream_115.add(string_literal91)


                char_literal92 = self.match(self.input, 75, self.FOLLOW_75_in_for_stmt835) 
                if self._state.backtracking == 0:
                    stream_75.add(char_literal92)


                # Expr.g:145:15: (a= exec_list )?
                alt16 = 2
                LA16_0 = self.input.LA(1)

                if (LA16_0 == ID or LA16_0 == 80 or LA16_0 == 84) :
                    alt16 = 1
                if alt16 == 1:
                    # Expr.g:145:15: a= exec_list
                    pass 
                    self._state.following.append(self.FOLLOW_exec_list_in_for_stmt839)
                    a = self.exec_list()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_exec_list.add(a.tree)





                char_literal93 = self.match(self.input, 92, self.FOLLOW_92_in_for_stmt842) 
                if self._state.backtracking == 0:
                    stream_92.add(char_literal93)


                self._state.following.append(self.FOLLOW_expr_in_for_stmt844)
                expr94 = self.expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_expr.add(expr94.tree)


                char_literal95 = self.match(self.input, 92, self.FOLLOW_92_in_for_stmt846) 
                if self._state.backtracking == 0:
                    stream_92.add(char_literal95)


                # Expr.g:145:41: (b= exec_list )?
                alt17 = 2
                LA17_0 = self.input.LA(1)

                if (LA17_0 == ID or LA17_0 == 80 or LA17_0 == 84) :
                    alt17 = 1
                if alt17 == 1:
                    # Expr.g:145:41: b= exec_list
                    pass 
                    self._state.following.append(self.FOLLOW_exec_list_in_for_stmt850)
                    b = self.exec_list()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_exec_list.add(b.tree)





                char_literal96 = self.match(self.input, 76, self.FOLLOW_76_in_for_stmt853) 
                if self._state.backtracking == 0:
                    stream_76.add(char_literal96)


                self._state.following.append(self.FOLLOW_block_in_for_stmt855)
                block97 = self.block()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_block.add(block97.tree)


                # AST Rewrite
                # elements: expr, block, a, b
                # token labels: 
                # rule labels: retval, b, a
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)

                    if b is not None:
                        stream_b = RewriteRuleSubtreeStream(self._adaptor, "rule b", b.tree)
                    else:
                        stream_b = RewriteRuleSubtreeStream(self._adaptor, "token b", None)

                    if a is not None:
                        stream_a = RewriteRuleSubtreeStream(self._adaptor, "rule a", a.tree)
                    else:
                        stream_a = RewriteRuleSubtreeStream(self._adaptor, "token a", None)


                    root_0 = self._adaptor.nil()
                    # 146:3: -> ^( FOR ( $a)? expr block ( $b)? )
                    # Expr.g:146:6: ^( FOR ( $a)? expr block ( $b)? )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(FOR, "FOR")
                    , root_1)

                    # Expr.g:146:13: ( $a)?
                    if stream_a.hasNext():
                        self._adaptor.addChild(root_1, stream_a.nextTree())


                    stream_a.reset();

                    self._adaptor.addChild(root_1, stream_expr.nextTree())

                    self._adaptor.addChild(root_1, stream_block.nextTree())

                    # Expr.g:146:28: ( $b)?
                    if stream_b.hasNext():
                        self._adaptor.addChild(root_1, stream_b.nextTree())


                    stream_b.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "for_stmt"


    class foreach_stmt_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.foreach_stmt_return, self).__init__()

            self.tree = None





    # $ANTLR start "foreach_stmt"
    # Expr.g:149:1: foreach_stmt : 'foreach' '(' expr 'as' each ')' block -> ^( FOREACH expr each block ) ;
    def foreach_stmt(self, ):
        retval = self.foreach_stmt_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal98 = None
        char_literal99 = None
        string_literal101 = None
        char_literal103 = None
        expr100 = None
        each102 = None
        block104 = None

        string_literal98_tree = None
        char_literal99_tree = None
        string_literal101_tree = None
        char_literal103_tree = None
        stream_116 = RewriteRuleTokenStream(self._adaptor, "token 116")
        stream_104 = RewriteRuleTokenStream(self._adaptor, "token 104")
        stream_75 = RewriteRuleTokenStream(self._adaptor, "token 75")
        stream_76 = RewriteRuleTokenStream(self._adaptor, "token 76")
        stream_block = RewriteRuleSubtreeStream(self._adaptor, "rule block")
        stream_expr = RewriteRuleSubtreeStream(self._adaptor, "rule expr")
        stream_each = RewriteRuleSubtreeStream(self._adaptor, "rule each")
        try:
            try:
                # Expr.g:150:2: ( 'foreach' '(' expr 'as' each ')' block -> ^( FOREACH expr each block ) )
                # Expr.g:150:4: 'foreach' '(' expr 'as' each ')' block
                pass 
                string_literal98 = self.match(self.input, 116, self.FOLLOW_116_in_foreach_stmt886) 
                if self._state.backtracking == 0:
                    stream_116.add(string_literal98)


                char_literal99 = self.match(self.input, 75, self.FOLLOW_75_in_foreach_stmt888) 
                if self._state.backtracking == 0:
                    stream_75.add(char_literal99)


                self._state.following.append(self.FOLLOW_expr_in_foreach_stmt890)
                expr100 = self.expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_expr.add(expr100.tree)


                string_literal101 = self.match(self.input, 104, self.FOLLOW_104_in_foreach_stmt892) 
                if self._state.backtracking == 0:
                    stream_104.add(string_literal101)


                self._state.following.append(self.FOLLOW_each_in_foreach_stmt894)
                each102 = self.each()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_each.add(each102.tree)


                char_literal103 = self.match(self.input, 76, self.FOLLOW_76_in_foreach_stmt896) 
                if self._state.backtracking == 0:
                    stream_76.add(char_literal103)


                self._state.following.append(self.FOLLOW_block_in_foreach_stmt898)
                block104 = self.block()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_block.add(block104.tree)


                # AST Rewrite
                # elements: expr, block, each
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 151:3: -> ^( FOREACH expr each block )
                    # Expr.g:151:6: ^( FOREACH expr each block )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(FOREACH, "FOREACH")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_expr.nextTree())

                    self._adaptor.addChild(root_1, stream_each.nextTree())

                    self._adaptor.addChild(root_1, stream_block.nextTree())

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "foreach_stmt"


    class each_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.each_return, self).__init__()

            self.tree = None





    # $ANTLR start "each"
    # Expr.g:153:1: each : ( each_val -> ^( EACH each_val ) | ID '=>' each_val -> ^( EACH ID each_val ) );
    def each(self, ):
        retval = self.each_return()
        retval.start = self.input.LT(1)


        root_0 = None

        ID106 = None
        string_literal107 = None
        each_val105 = None
        each_val108 = None

        ID106_tree = None
        string_literal107_tree = None
        stream_97 = RewriteRuleTokenStream(self._adaptor, "token 97")
        stream_ID = RewriteRuleTokenStream(self._adaptor, "token ID")
        stream_each_val = RewriteRuleSubtreeStream(self._adaptor, "rule each_val")
        try:
            try:
                # Expr.g:154:2: ( each_val -> ^( EACH each_val ) | ID '=>' each_val -> ^( EACH ID each_val ) )
                alt18 = 2
                LA18_0 = self.input.LA(1)

                if (LA18_0 == ID) :
                    LA18_1 = self.input.LA(2)

                    if (LA18_1 == 97) :
                        alt18 = 2
                    elif (LA18_1 == 76 or LA18_1 == 82) :
                        alt18 = 1
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed


                        nvae = NoViableAltException("", 18, 1, self.input)

                        raise nvae


                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    nvae = NoViableAltException("", 18, 0, self.input)

                    raise nvae


                if alt18 == 1:
                    # Expr.g:154:4: each_val
                    pass 
                    self._state.following.append(self.FOLLOW_each_val_in_each922)
                    each_val105 = self.each_val()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_each_val.add(each_val105.tree)


                    # AST Rewrite
                    # elements: each_val
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 155:3: -> ^( EACH each_val )
                        # Expr.g:155:6: ^( EACH each_val )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(EACH, "EACH")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_each_val.nextTree())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt18 == 2:
                    # Expr.g:156:4: ID '=>' each_val
                    pass 
                    ID106 = self.match(self.input, ID, self.FOLLOW_ID_in_each937) 
                    if self._state.backtracking == 0:
                        stream_ID.add(ID106)


                    string_literal107 = self.match(self.input, 97, self.FOLLOW_97_in_each939) 
                    if self._state.backtracking == 0:
                        stream_97.add(string_literal107)


                    self._state.following.append(self.FOLLOW_each_val_in_each941)
                    each_val108 = self.each_val()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_each_val.add(each_val108.tree)


                    # AST Rewrite
                    # elements: each_val, ID
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 157:3: -> ^( EACH ID each_val )
                        # Expr.g:157:6: ^( EACH ID each_val )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(EACH, "EACH")
                        , root_1)

                        self._adaptor.addChild(root_1, 
                        stream_ID.nextNode()
                        )

                        self._adaptor.addChild(root_1, stream_each_val.nextTree())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "each"


    class each_val_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.each_val_return, self).__init__()

            self.tree = None





    # $ANTLR start "each_val"
    # Expr.g:159:1: each_val : ID ( ',' ID )* -> ^( EACH_VAL ( ID )+ ) ;
    def each_val(self, ):
        retval = self.each_val_return()
        retval.start = self.input.LT(1)


        root_0 = None

        ID109 = None
        char_literal110 = None
        ID111 = None

        ID109_tree = None
        char_literal110_tree = None
        ID111_tree = None
        stream_82 = RewriteRuleTokenStream(self._adaptor, "token 82")
        stream_ID = RewriteRuleTokenStream(self._adaptor, "token ID")

        try:
            try:
                # Expr.g:160:2: ( ID ( ',' ID )* -> ^( EACH_VAL ( ID )+ ) )
                # Expr.g:160:4: ID ( ',' ID )*
                pass 
                ID109 = self.match(self.input, ID, self.FOLLOW_ID_in_each_val963) 
                if self._state.backtracking == 0:
                    stream_ID.add(ID109)


                # Expr.g:160:7: ( ',' ID )*
                while True: #loop19
                    alt19 = 2
                    LA19_0 = self.input.LA(1)

                    if (LA19_0 == 82) :
                        alt19 = 1


                    if alt19 == 1:
                        # Expr.g:160:8: ',' ID
                        pass 
                        char_literal110 = self.match(self.input, 82, self.FOLLOW_82_in_each_val966) 
                        if self._state.backtracking == 0:
                            stream_82.add(char_literal110)


                        ID111 = self.match(self.input, ID, self.FOLLOW_ID_in_each_val968) 
                        if self._state.backtracking == 0:
                            stream_ID.add(ID111)



                    else:
                        break #loop19


                # AST Rewrite
                # elements: ID
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 161:3: -> ^( EACH_VAL ( ID )+ )
                    # Expr.g:161:6: ^( EACH_VAL ( ID )+ )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(EACH_VAL, "EACH_VAL")
                    , root_1)

                    # Expr.g:161:17: ( ID )+
                    if not (stream_ID.hasNext()):
                        raise RewriteEarlyExitException()

                    while stream_ID.hasNext():
                        self._adaptor.addChild(root_1, 
                        stream_ID.nextNode()
                        )


                    stream_ID.reset()

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "each_val"


    class throw_stmt_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.throw_stmt_return, self).__init__()

            self.tree = None





    # $ANTLR start "throw_stmt"
    # Expr.g:165:1: throw_stmt : 'throw' expr ';' -> ^( THROW expr ) ;
    def throw_stmt(self, ):
        retval = self.throw_stmt_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal112 = None
        char_literal114 = None
        expr113 = None

        string_literal112_tree = None
        char_literal114_tree = None
        stream_92 = RewriteRuleTokenStream(self._adaptor, "token 92")
        stream_129 = RewriteRuleTokenStream(self._adaptor, "token 129")
        stream_expr = RewriteRuleSubtreeStream(self._adaptor, "rule expr")
        try:
            try:
                # Expr.g:166:2: ( 'throw' expr ';' -> ^( THROW expr ) )
                # Expr.g:166:4: 'throw' expr ';'
                pass 
                string_literal112 = self.match(self.input, 129, self.FOLLOW_129_in_throw_stmt993) 
                if self._state.backtracking == 0:
                    stream_129.add(string_literal112)


                self._state.following.append(self.FOLLOW_expr_in_throw_stmt995)
                expr113 = self.expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_expr.add(expr113.tree)


                char_literal114 = self.match(self.input, 92, self.FOLLOW_92_in_throw_stmt997) 
                if self._state.backtracking == 0:
                    stream_92.add(char_literal114)


                # AST Rewrite
                # elements: expr
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 167:3: -> ^( THROW expr )
                    # Expr.g:167:6: ^( THROW expr )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(THROW, "THROW")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_expr.nextTree())

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "throw_stmt"


    class try_stmt_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.try_stmt_return, self).__init__()

            self.tree = None





    # $ANTLR start "try_stmt"
    # Expr.g:169:1: try_stmt : 'try' block ( catch_clause )+ ( finally_clause )? -> ^( TRY block ( catch_clause )+ ( finally_clause )? ) ;
    def try_stmt(self, ):
        retval = self.try_stmt_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal115 = None
        block116 = None
        catch_clause117 = None
        finally_clause118 = None

        string_literal115_tree = None
        stream_130 = RewriteRuleTokenStream(self._adaptor, "token 130")
        stream_catch_clause = RewriteRuleSubtreeStream(self._adaptor, "rule catch_clause")
        stream_block = RewriteRuleSubtreeStream(self._adaptor, "rule block")
        stream_finally_clause = RewriteRuleSubtreeStream(self._adaptor, "rule finally_clause")
        try:
            try:
                # Expr.g:170:2: ( 'try' block ( catch_clause )+ ( finally_clause )? -> ^( TRY block ( catch_clause )+ ( finally_clause )? ) )
                # Expr.g:170:4: 'try' block ( catch_clause )+ ( finally_clause )?
                pass 
                string_literal115 = self.match(self.input, 130, self.FOLLOW_130_in_try_stmt1017) 
                if self._state.backtracking == 0:
                    stream_130.add(string_literal115)


                self._state.following.append(self.FOLLOW_block_in_try_stmt1019)
                block116 = self.block()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_block.add(block116.tree)


                # Expr.g:170:16: ( catch_clause )+
                cnt20 = 0
                while True: #loop20
                    alt20 = 2
                    LA20_0 = self.input.LA(1)

                    if (LA20_0 == 107) :
                        alt20 = 1


                    if alt20 == 1:
                        # Expr.g:170:16: catch_clause
                        pass 
                        self._state.following.append(self.FOLLOW_catch_clause_in_try_stmt1021)
                        catch_clause117 = self.catch_clause()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_catch_clause.add(catch_clause117.tree)



                    else:
                        if cnt20 >= 1:
                            break #loop20

                        if self._state.backtracking > 0:
                            raise BacktrackingFailed


                        eee = EarlyExitException(20, self.input)
                        raise eee

                    cnt20 += 1


                # Expr.g:170:30: ( finally_clause )?
                alt21 = 2
                LA21_0 = self.input.LA(1)

                if (LA21_0 == 114) :
                    alt21 = 1
                if alt21 == 1:
                    # Expr.g:170:30: finally_clause
                    pass 
                    self._state.following.append(self.FOLLOW_finally_clause_in_try_stmt1024)
                    finally_clause118 = self.finally_clause()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_finally_clause.add(finally_clause118.tree)





                # AST Rewrite
                # elements: finally_clause, catch_clause, block
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 171:3: -> ^( TRY block ( catch_clause )+ ( finally_clause )? )
                    # Expr.g:171:6: ^( TRY block ( catch_clause )+ ( finally_clause )? )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(TRY, "TRY")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_block.nextTree())

                    # Expr.g:171:18: ( catch_clause )+
                    if not (stream_catch_clause.hasNext()):
                        raise RewriteEarlyExitException()

                    while stream_catch_clause.hasNext():
                        self._adaptor.addChild(root_1, stream_catch_clause.nextTree())


                    stream_catch_clause.reset()

                    # Expr.g:171:32: ( finally_clause )?
                    if stream_finally_clause.hasNext():
                        self._adaptor.addChild(root_1, stream_finally_clause.nextTree())


                    stream_finally_clause.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "try_stmt"


    class catch_clause_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.catch_clause_return, self).__init__()

            self.tree = None





    # $ANTLR start "catch_clause"
    # Expr.g:173:1: catch_clause : 'catch' '(' module ( ID )? ')' block -> ^( CATCH module ( ID )? block ) ;
    def catch_clause(self, ):
        retval = self.catch_clause_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal119 = None
        char_literal120 = None
        ID122 = None
        char_literal123 = None
        module121 = None
        block124 = None

        string_literal119_tree = None
        char_literal120_tree = None
        ID122_tree = None
        char_literal123_tree = None
        stream_107 = RewriteRuleTokenStream(self._adaptor, "token 107")
        stream_ID = RewriteRuleTokenStream(self._adaptor, "token ID")
        stream_75 = RewriteRuleTokenStream(self._adaptor, "token 75")
        stream_76 = RewriteRuleTokenStream(self._adaptor, "token 76")
        stream_module = RewriteRuleSubtreeStream(self._adaptor, "rule module")
        stream_block = RewriteRuleSubtreeStream(self._adaptor, "rule block")
        try:
            try:
                # Expr.g:174:2: ( 'catch' '(' module ( ID )? ')' block -> ^( CATCH module ( ID )? block ) )
                # Expr.g:174:4: 'catch' '(' module ( ID )? ')' block
                pass 
                string_literal119 = self.match(self.input, 107, self.FOLLOW_107_in_catch_clause1051) 
                if self._state.backtracking == 0:
                    stream_107.add(string_literal119)


                char_literal120 = self.match(self.input, 75, self.FOLLOW_75_in_catch_clause1053) 
                if self._state.backtracking == 0:
                    stream_75.add(char_literal120)


                self._state.following.append(self.FOLLOW_module_in_catch_clause1055)
                module121 = self.module()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_module.add(module121.tree)


                # Expr.g:174:23: ( ID )?
                alt22 = 2
                LA22_0 = self.input.LA(1)

                if (LA22_0 == ID) :
                    alt22 = 1
                if alt22 == 1:
                    # Expr.g:174:23: ID
                    pass 
                    ID122 = self.match(self.input, ID, self.FOLLOW_ID_in_catch_clause1057) 
                    if self._state.backtracking == 0:
                        stream_ID.add(ID122)





                char_literal123 = self.match(self.input, 76, self.FOLLOW_76_in_catch_clause1060) 
                if self._state.backtracking == 0:
                    stream_76.add(char_literal123)


                self._state.following.append(self.FOLLOW_block_in_catch_clause1062)
                block124 = self.block()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_block.add(block124.tree)


                # AST Rewrite
                # elements: block, ID, module
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 175:3: -> ^( CATCH module ( ID )? block )
                    # Expr.g:175:6: ^( CATCH module ( ID )? block )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(CATCH, "CATCH")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_module.nextTree())

                    # Expr.g:175:21: ( ID )?
                    if stream_ID.hasNext():
                        self._adaptor.addChild(root_1, 
                        stream_ID.nextNode()
                        )


                    stream_ID.reset();

                    self._adaptor.addChild(root_1, stream_block.nextTree())

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "catch_clause"


    class finally_clause_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.finally_clause_return, self).__init__()

            self.tree = None





    # $ANTLR start "finally_clause"
    # Expr.g:177:1: finally_clause : 'finally' block -> ^( FINALLY block ) ;
    def finally_clause(self, ):
        retval = self.finally_clause_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal125 = None
        block126 = None

        string_literal125_tree = None
        stream_114 = RewriteRuleTokenStream(self._adaptor, "token 114")
        stream_block = RewriteRuleSubtreeStream(self._adaptor, "rule block")
        try:
            try:
                # Expr.g:178:2: ( 'finally' block -> ^( FINALLY block ) )
                # Expr.g:178:4: 'finally' block
                pass 
                string_literal125 = self.match(self.input, 114, self.FOLLOW_114_in_finally_clause1087) 
                if self._state.backtracking == 0:
                    stream_114.add(string_literal125)


                self._state.following.append(self.FOLLOW_block_in_finally_clause1089)
                block126 = self.block()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_block.add(block126.tree)


                # AST Rewrite
                # elements: block
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 179:3: -> ^( FINALLY block )
                    # Expr.g:179:6: ^( FINALLY block )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(FINALLY, "FINALLY")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_block.nextTree())

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "finally_clause"


    class func_decl_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.func_decl_return, self).__init__()

            self.tree = None





    # $ANTLR start "func_decl"
    # Expr.g:183:1: func_decl : 'function' ID params block -> ^( FUNCTION ID params block ) ;
    def func_decl(self, ):
        retval = self.func_decl_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal127 = None
        ID128 = None
        params129 = None
        block130 = None

        string_literal127_tree = None
        ID128_tree = None
        stream_117 = RewriteRuleTokenStream(self._adaptor, "token 117")
        stream_ID = RewriteRuleTokenStream(self._adaptor, "token ID")
        stream_block = RewriteRuleSubtreeStream(self._adaptor, "rule block")
        stream_params = RewriteRuleSubtreeStream(self._adaptor, "rule params")
        try:
            try:
                # Expr.g:184:2: ( 'function' ID params block -> ^( FUNCTION ID params block ) )
                # Expr.g:184:4: 'function' ID params block
                pass 
                string_literal127 = self.match(self.input, 117, self.FOLLOW_117_in_func_decl1111) 
                if self._state.backtracking == 0:
                    stream_117.add(string_literal127)


                ID128 = self.match(self.input, ID, self.FOLLOW_ID_in_func_decl1113) 
                if self._state.backtracking == 0:
                    stream_ID.add(ID128)


                self._state.following.append(self.FOLLOW_params_in_func_decl1115)
                params129 = self.params()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_params.add(params129.tree)


                self._state.following.append(self.FOLLOW_block_in_func_decl1117)
                block130 = self.block()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_block.add(block130.tree)


                # AST Rewrite
                # elements: block, ID, params
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 185:3: -> ^( FUNCTION ID params block )
                    # Expr.g:185:6: ^( FUNCTION ID params block )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(FUNCTION, "FUNCTION")
                    , root_1)

                    self._adaptor.addChild(root_1, 
                    stream_ID.nextNode()
                    )

                    self._adaptor.addChild(root_1, stream_params.nextTree())

                    self._adaptor.addChild(root_1, stream_block.nextTree())

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "func_decl"


    class params_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.params_return, self).__init__()

            self.tree = None





    # $ANTLR start "params"
    # Expr.g:187:1: params : '(' ( param_decl )? ( ',' param_decl )* ')' -> ^( PARAMS ( param_decl )* ) ;
    def params(self, ):
        retval = self.params_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal131 = None
        char_literal133 = None
        char_literal135 = None
        param_decl132 = None
        param_decl134 = None

        char_literal131_tree = None
        char_literal133_tree = None
        char_literal135_tree = None
        stream_82 = RewriteRuleTokenStream(self._adaptor, "token 82")
        stream_75 = RewriteRuleTokenStream(self._adaptor, "token 75")
        stream_76 = RewriteRuleTokenStream(self._adaptor, "token 76")
        stream_param_decl = RewriteRuleSubtreeStream(self._adaptor, "rule param_decl")
        try:
            try:
                # Expr.g:188:2: ( '(' ( param_decl )? ( ',' param_decl )* ')' -> ^( PARAMS ( param_decl )* ) )
                # Expr.g:188:4: '(' ( param_decl )? ( ',' param_decl )* ')'
                pass 
                char_literal131 = self.match(self.input, 75, self.FOLLOW_75_in_params1141) 
                if self._state.backtracking == 0:
                    stream_75.add(char_literal131)


                # Expr.g:188:8: ( param_decl )?
                alt23 = 2
                LA23_0 = self.input.LA(1)

                if (LA23_0 == ID) :
                    alt23 = 1
                if alt23 == 1:
                    # Expr.g:188:8: param_decl
                    pass 
                    self._state.following.append(self.FOLLOW_param_decl_in_params1143)
                    param_decl132 = self.param_decl()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_param_decl.add(param_decl132.tree)





                # Expr.g:188:20: ( ',' param_decl )*
                while True: #loop24
                    alt24 = 2
                    LA24_0 = self.input.LA(1)

                    if (LA24_0 == 82) :
                        alt24 = 1


                    if alt24 == 1:
                        # Expr.g:188:21: ',' param_decl
                        pass 
                        char_literal133 = self.match(self.input, 82, self.FOLLOW_82_in_params1147) 
                        if self._state.backtracking == 0:
                            stream_82.add(char_literal133)


                        self._state.following.append(self.FOLLOW_param_decl_in_params1149)
                        param_decl134 = self.param_decl()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_param_decl.add(param_decl134.tree)



                    else:
                        break #loop24


                char_literal135 = self.match(self.input, 76, self.FOLLOW_76_in_params1153) 
                if self._state.backtracking == 0:
                    stream_76.add(char_literal135)


                # AST Rewrite
                # elements: param_decl
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 189:3: -> ^( PARAMS ( param_decl )* )
                    # Expr.g:189:6: ^( PARAMS ( param_decl )* )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(PARAMS, "PARAMS")
                    , root_1)

                    # Expr.g:189:15: ( param_decl )*
                    while stream_param_decl.hasNext():
                        self._adaptor.addChild(root_1, stream_param_decl.nextTree())


                    stream_param_decl.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "params"


    class param_decl_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.param_decl_return, self).__init__()

            self.tree = None





    # $ANTLR start "param_decl"
    # Expr.g:191:1: param_decl : ID ( '=' atom )? ;
    def param_decl(self, ):
        retval = self.param_decl_return()
        retval.start = self.input.LT(1)


        root_0 = None

        ID136 = None
        char_literal137 = None
        atom138 = None

        ID136_tree = None
        char_literal137_tree = None

        try:
            try:
                # Expr.g:192:2: ( ID ( '=' atom )? )
                # Expr.g:192:4: ID ( '=' atom )?
                pass 
                root_0 = self._adaptor.nil()


                ID136 = self.match(self.input, ID, self.FOLLOW_ID_in_param_decl1174)
                if self._state.backtracking == 0:
                    ID136_tree = self._adaptor.createWithPayload(ID136)
                    self._adaptor.addChild(root_0, ID136_tree)



                # Expr.g:192:7: ( '=' atom )?
                alt25 = 2
                LA25_0 = self.input.LA(1)

                if (LA25_0 == 95) :
                    alt25 = 1
                if alt25 == 1:
                    # Expr.g:192:8: '=' atom
                    pass 
                    char_literal137 = self.match(self.input, 95, self.FOLLOW_95_in_param_decl1177)
                    if self._state.backtracking == 0:
                        char_literal137_tree = self._adaptor.createWithPayload(char_literal137)
                        self._adaptor.addChild(root_0, char_literal137_tree)



                    self._state.following.append(self.FOLLOW_atom_in_param_decl1179)
                    atom138 = self.atom()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, atom138.tree)







                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "param_decl"


    class class_decl_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.class_decl_return, self).__init__()

            self.tree = None





    # $ANTLR start "class_decl"
    # Expr.g:195:1: class_decl : 'class' ID ( 'extends' ID )? '{' ( class_element )* '}' -> ^( CLASS ID ( ID )? ( class_element )* ) ;
    def class_decl(self, ):
        retval = self.class_decl_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal139 = None
        ID140 = None
        string_literal141 = None
        ID142 = None
        char_literal143 = None
        char_literal145 = None
        class_element144 = None

        string_literal139_tree = None
        ID140_tree = None
        string_literal141_tree = None
        ID142_tree = None
        char_literal143_tree = None
        char_literal145_tree = None
        stream_132 = RewriteRuleTokenStream(self._adaptor, "token 132")
        stream_113 = RewriteRuleTokenStream(self._adaptor, "token 113")
        stream_108 = RewriteRuleTokenStream(self._adaptor, "token 108")
        stream_136 = RewriteRuleTokenStream(self._adaptor, "token 136")
        stream_ID = RewriteRuleTokenStream(self._adaptor, "token ID")
        stream_class_element = RewriteRuleSubtreeStream(self._adaptor, "rule class_element")
        try:
            try:
                # Expr.g:196:2: ( 'class' ID ( 'extends' ID )? '{' ( class_element )* '}' -> ^( CLASS ID ( ID )? ( class_element )* ) )
                # Expr.g:196:4: 'class' ID ( 'extends' ID )? '{' ( class_element )* '}'
                pass 
                string_literal139 = self.match(self.input, 108, self.FOLLOW_108_in_class_decl1192) 
                if self._state.backtracking == 0:
                    stream_108.add(string_literal139)


                ID140 = self.match(self.input, ID, self.FOLLOW_ID_in_class_decl1194) 
                if self._state.backtracking == 0:
                    stream_ID.add(ID140)


                # Expr.g:196:15: ( 'extends' ID )?
                alt26 = 2
                LA26_0 = self.input.LA(1)

                if (LA26_0 == 113) :
                    alt26 = 1
                if alt26 == 1:
                    # Expr.g:196:16: 'extends' ID
                    pass 
                    string_literal141 = self.match(self.input, 113, self.FOLLOW_113_in_class_decl1197) 
                    if self._state.backtracking == 0:
                        stream_113.add(string_literal141)


                    ID142 = self.match(self.input, ID, self.FOLLOW_ID_in_class_decl1199) 
                    if self._state.backtracking == 0:
                        stream_ID.add(ID142)





                char_literal143 = self.match(self.input, 132, self.FOLLOW_132_in_class_decl1205) 
                if self._state.backtracking == 0:
                    stream_132.add(char_literal143)


                # Expr.g:197:7: ( class_element )*
                while True: #loop27
                    alt27 = 2
                    LA27_0 = self.input.LA(1)

                    if (LA27_0 == 117 or LA27_0 == 124) :
                        alt27 = 1


                    if alt27 == 1:
                        # Expr.g:197:7: class_element
                        pass 
                        self._state.following.append(self.FOLLOW_class_element_in_class_decl1207)
                        class_element144 = self.class_element()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_class_element.add(class_element144.tree)



                    else:
                        break #loop27


                char_literal145 = self.match(self.input, 136, self.FOLLOW_136_in_class_decl1210) 
                if self._state.backtracking == 0:
                    stream_136.add(char_literal145)


                # AST Rewrite
                # elements: ID, ID, class_element
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 198:3: -> ^( CLASS ID ( ID )? ( class_element )* )
                    # Expr.g:198:6: ^( CLASS ID ( ID )? ( class_element )* )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(CLASS, "CLASS")
                    , root_1)

                    self._adaptor.addChild(root_1, 
                    stream_ID.nextNode()
                    )

                    # Expr.g:198:17: ( ID )?
                    if stream_ID.hasNext():
                        self._adaptor.addChild(root_1, 
                        stream_ID.nextNode()
                        )


                    stream_ID.reset();

                    # Expr.g:198:21: ( class_element )*
                    while stream_class_element.hasNext():
                        self._adaptor.addChild(root_1, stream_class_element.nextTree())


                    stream_class_element.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "class_decl"


    class class_element_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.class_element_return, self).__init__()

            self.tree = None





    # $ANTLR start "class_element"
    # Expr.g:200:1: class_element : ( var_def | constructor | func_decl );
    def class_element(self, ):
        retval = self.class_element_return()
        retval.start = self.input.LT(1)


        root_0 = None

        var_def146 = None
        constructor147 = None
        func_decl148 = None


        try:
            try:
                # Expr.g:201:2: ( var_def | constructor | func_decl )
                alt28 = 3
                LA28_0 = self.input.LA(1)

                if (LA28_0 == 124) :
                    alt28 = 1
                elif (LA28_0 == 117) :
                    LA28_2 = self.input.LA(2)

                    if (LA28_2 == 120) :
                        alt28 = 2
                    elif (LA28_2 == ID) :
                        alt28 = 3
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed


                        nvae = NoViableAltException("", 28, 2, self.input)

                        raise nvae


                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    nvae = NoViableAltException("", 28, 0, self.input)

                    raise nvae


                if alt28 == 1:
                    # Expr.g:201:4: var_def
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_var_def_in_class_element1236)
                    var_def146 = self.var_def()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, var_def146.tree)



                elif alt28 == 2:
                    # Expr.g:201:14: constructor
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_constructor_in_class_element1240)
                    constructor147 = self.constructor()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, constructor147.tree)



                elif alt28 == 3:
                    # Expr.g:201:28: func_decl
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_func_decl_in_class_element1244)
                    func_decl148 = self.func_decl()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, func_decl148.tree)



                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "class_element"


    class var_def_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.var_def_return, self).__init__()

            self.tree = None





    # $ANTLR start "var_def"
    # Expr.g:203:1: var_def : ( 'public' ID ( '=' expr )? ';' -> ^( VAR ID ( expr )? ) | 'public' 'static' ID ( '=' expr )? ';' -> ^( VAR 'static' ID ( expr )? ) );
    def var_def(self, ):
        retval = self.var_def_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal149 = None
        ID150 = None
        char_literal151 = None
        char_literal153 = None
        string_literal154 = None
        string_literal155 = None
        ID156 = None
        char_literal157 = None
        char_literal159 = None
        expr152 = None
        expr158 = None

        string_literal149_tree = None
        ID150_tree = None
        char_literal151_tree = None
        char_literal153_tree = None
        string_literal154_tree = None
        string_literal155_tree = None
        ID156_tree = None
        char_literal157_tree = None
        char_literal159_tree = None
        stream_127 = RewriteRuleTokenStream(self._adaptor, "token 127")
        stream_95 = RewriteRuleTokenStream(self._adaptor, "token 95")
        stream_92 = RewriteRuleTokenStream(self._adaptor, "token 92")
        stream_124 = RewriteRuleTokenStream(self._adaptor, "token 124")
        stream_ID = RewriteRuleTokenStream(self._adaptor, "token ID")
        stream_expr = RewriteRuleSubtreeStream(self._adaptor, "rule expr")
        try:
            try:
                # Expr.g:204:2: ( 'public' ID ( '=' expr )? ';' -> ^( VAR ID ( expr )? ) | 'public' 'static' ID ( '=' expr )? ';' -> ^( VAR 'static' ID ( expr )? ) )
                alt31 = 2
                LA31_0 = self.input.LA(1)

                if (LA31_0 == 124) :
                    LA31_1 = self.input.LA(2)

                    if (LA31_1 == ID) :
                        alt31 = 1
                    elif (LA31_1 == 127) :
                        alt31 = 2
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed


                        nvae = NoViableAltException("", 31, 1, self.input)

                        raise nvae


                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    nvae = NoViableAltException("", 31, 0, self.input)

                    raise nvae


                if alt31 == 1:
                    # Expr.g:204:4: 'public' ID ( '=' expr )? ';'
                    pass 
                    string_literal149 = self.match(self.input, 124, self.FOLLOW_124_in_var_def1254) 
                    if self._state.backtracking == 0:
                        stream_124.add(string_literal149)


                    ID150 = self.match(self.input, ID, self.FOLLOW_ID_in_var_def1256) 
                    if self._state.backtracking == 0:
                        stream_ID.add(ID150)


                    # Expr.g:204:16: ( '=' expr )?
                    alt29 = 2
                    LA29_0 = self.input.LA(1)

                    if (LA29_0 == 95) :
                        alt29 = 1
                    if alt29 == 1:
                        # Expr.g:204:17: '=' expr
                        pass 
                        char_literal151 = self.match(self.input, 95, self.FOLLOW_95_in_var_def1259) 
                        if self._state.backtracking == 0:
                            stream_95.add(char_literal151)


                        self._state.following.append(self.FOLLOW_expr_in_var_def1261)
                        expr152 = self.expr()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_expr.add(expr152.tree)





                    char_literal153 = self.match(self.input, 92, self.FOLLOW_92_in_var_def1265) 
                    if self._state.backtracking == 0:
                        stream_92.add(char_literal153)


                    # AST Rewrite
                    # elements: expr, ID
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 205:3: -> ^( VAR ID ( expr )? )
                        # Expr.g:205:6: ^( VAR ID ( expr )? )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(VAR, "VAR")
                        , root_1)

                        self._adaptor.addChild(root_1, 
                        stream_ID.nextNode()
                        )

                        # Expr.g:205:15: ( expr )?
                        if stream_expr.hasNext():
                            self._adaptor.addChild(root_1, stream_expr.nextTree())


                        stream_expr.reset();

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt31 == 2:
                    # Expr.g:206:4: 'public' 'static' ID ( '=' expr )? ';'
                    pass 
                    string_literal154 = self.match(self.input, 124, self.FOLLOW_124_in_var_def1283) 
                    if self._state.backtracking == 0:
                        stream_124.add(string_literal154)


                    string_literal155 = self.match(self.input, 127, self.FOLLOW_127_in_var_def1285) 
                    if self._state.backtracking == 0:
                        stream_127.add(string_literal155)


                    ID156 = self.match(self.input, ID, self.FOLLOW_ID_in_var_def1287) 
                    if self._state.backtracking == 0:
                        stream_ID.add(ID156)


                    # Expr.g:206:25: ( '=' expr )?
                    alt30 = 2
                    LA30_0 = self.input.LA(1)

                    if (LA30_0 == 95) :
                        alt30 = 1
                    if alt30 == 1:
                        # Expr.g:206:26: '=' expr
                        pass 
                        char_literal157 = self.match(self.input, 95, self.FOLLOW_95_in_var_def1290) 
                        if self._state.backtracking == 0:
                            stream_95.add(char_literal157)


                        self._state.following.append(self.FOLLOW_expr_in_var_def1292)
                        expr158 = self.expr()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_expr.add(expr158.tree)





                    char_literal159 = self.match(self.input, 92, self.FOLLOW_92_in_var_def1296) 
                    if self._state.backtracking == 0:
                        stream_92.add(char_literal159)


                    # AST Rewrite
                    # elements: expr, ID, 127
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 207:3: -> ^( VAR 'static' ID ( expr )? )
                        # Expr.g:207:6: ^( VAR 'static' ID ( expr )? )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(VAR, "VAR")
                        , root_1)

                        self._adaptor.addChild(root_1, 
                        stream_127.nextNode()
                        )

                        self._adaptor.addChild(root_1, 
                        stream_ID.nextNode()
                        )

                        # Expr.g:207:24: ( expr )?
                        if stream_expr.hasNext():
                            self._adaptor.addChild(root_1, stream_expr.nextTree())


                        stream_expr.reset();

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "var_def"


    class constructor_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.constructor_return, self).__init__()

            self.tree = None





    # $ANTLR start "constructor"
    # Expr.g:209:1: constructor : 'function' 'init' params block -> ^( CONSTRUCTOR params block ) ;
    def constructor(self, ):
        retval = self.constructor_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal160 = None
        string_literal161 = None
        params162 = None
        block163 = None

        string_literal160_tree = None
        string_literal161_tree = None
        stream_117 = RewriteRuleTokenStream(self._adaptor, "token 117")
        stream_120 = RewriteRuleTokenStream(self._adaptor, "token 120")
        stream_block = RewriteRuleSubtreeStream(self._adaptor, "rule block")
        stream_params = RewriteRuleSubtreeStream(self._adaptor, "rule params")
        try:
            try:
                # Expr.g:210:2: ( 'function' 'init' params block -> ^( CONSTRUCTOR params block ) )
                # Expr.g:210:4: 'function' 'init' params block
                pass 
                string_literal160 = self.match(self.input, 117, self.FOLLOW_117_in_constructor1321) 
                if self._state.backtracking == 0:
                    stream_117.add(string_literal160)


                string_literal161 = self.match(self.input, 120, self.FOLLOW_120_in_constructor1323) 
                if self._state.backtracking == 0:
                    stream_120.add(string_literal161)


                self._state.following.append(self.FOLLOW_params_in_constructor1325)
                params162 = self.params()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_params.add(params162.tree)


                self._state.following.append(self.FOLLOW_block_in_constructor1327)
                block163 = self.block()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_block.add(block163.tree)


                # AST Rewrite
                # elements: params, block
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 211:3: -> ^( CONSTRUCTOR params block )
                    # Expr.g:211:6: ^( CONSTRUCTOR params block )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(CONSTRUCTOR, "CONSTRUCTOR")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_params.nextTree())

                    self._adaptor.addChild(root_1, stream_block.nextTree())

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "constructor"


    class member_expr_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.member_expr_return, self).__init__()

            self.tree = None





    # $ANTLR start "member_expr"
    # Expr.g:217:1: member_expr : primary ( '.' primary )* -> ^( MEMBER ( primary )+ ) ;
    def member_expr(self, ):
        retval = self.member_expr_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal165 = None
        primary164 = None
        primary166 = None

        char_literal165_tree = None
        stream_86 = RewriteRuleTokenStream(self._adaptor, "token 86")
        stream_primary = RewriteRuleSubtreeStream(self._adaptor, "rule primary")
        try:
            try:
                # Expr.g:218:2: ( primary ( '.' primary )* -> ^( MEMBER ( primary )+ ) )
                # Expr.g:218:4: primary ( '.' primary )*
                pass 
                self._state.following.append(self.FOLLOW_primary_in_member_expr1354)
                primary164 = self.primary()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_primary.add(primary164.tree)


                # Expr.g:218:12: ( '.' primary )*
                while True: #loop32
                    alt32 = 2
                    LA32_0 = self.input.LA(1)

                    if (LA32_0 == 86) :
                        alt32 = 1


                    if alt32 == 1:
                        # Expr.g:218:13: '.' primary
                        pass 
                        char_literal165 = self.match(self.input, 86, self.FOLLOW_86_in_member_expr1357) 
                        if self._state.backtracking == 0:
                            stream_86.add(char_literal165)


                        self._state.following.append(self.FOLLOW_primary_in_member_expr1359)
                        primary166 = self.primary()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_primary.add(primary166.tree)



                    else:
                        break #loop32


                # AST Rewrite
                # elements: primary
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 219:3: -> ^( MEMBER ( primary )+ )
                    # Expr.g:219:6: ^( MEMBER ( primary )+ )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(MEMBER, "MEMBER")
                    , root_1)

                    # Expr.g:219:15: ( primary )+
                    if not (stream_primary.hasNext()):
                        raise RewriteEarlyExitException()

                    while stream_primary.hasNext():
                        self._adaptor.addChild(root_1, stream_primary.nextTree())


                    stream_primary.reset()

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "member_expr"


    class primary_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.primary_return, self).__init__()

            self.tree = None





    # $ANTLR start "primary"
    # Expr.g:221:1: primary : ID ( index_expr )* ( call_expr )? ;
    def primary(self, ):
        retval = self.primary_return()
        retval.start = self.input.LT(1)


        root_0 = None

        ID167 = None
        index_expr168 = None
        call_expr169 = None

        ID167_tree = None

        try:
            try:
                # Expr.g:222:2: ( ID ( index_expr )* ( call_expr )? )
                # Expr.g:222:4: ID ( index_expr )* ( call_expr )?
                pass 
                root_0 = self._adaptor.nil()


                ID167 = self.match(self.input, ID, self.FOLLOW_ID_in_primary1382)
                if self._state.backtracking == 0:
                    ID167_tree = self._adaptor.createWithPayload(ID167)
                    self._adaptor.addChild(root_0, ID167_tree)



                # Expr.g:222:7: ( index_expr )*
                while True: #loop33
                    alt33 = 2
                    LA33_0 = self.input.LA(1)

                    if (LA33_0 == 100) :
                        alt33 = 1


                    if alt33 == 1:
                        # Expr.g:222:7: index_expr
                        pass 
                        self._state.following.append(self.FOLLOW_index_expr_in_primary1384)
                        index_expr168 = self.index_expr()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            self._adaptor.addChild(root_0, index_expr168.tree)



                    else:
                        break #loop33


                # Expr.g:222:19: ( call_expr )?
                alt34 = 2
                LA34_0 = self.input.LA(1)

                if (LA34_0 == 75) :
                    alt34 = 1
                if alt34 == 1:
                    # Expr.g:222:19: call_expr
                    pass 
                    self._state.following.append(self.FOLLOW_call_expr_in_primary1387)
                    call_expr169 = self.call_expr()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, call_expr169.tree)







                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "primary"


    class call_expr_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.call_expr_return, self).__init__()

            self.tree = None





    # $ANTLR start "call_expr"
    # Expr.g:224:1: call_expr : '(' ( expr_list )? ')' -> ^( CALL ( expr_list )? ) ;
    def call_expr(self, ):
        retval = self.call_expr_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal170 = None
        char_literal172 = None
        expr_list171 = None

        char_literal170_tree = None
        char_literal172_tree = None
        stream_75 = RewriteRuleTokenStream(self._adaptor, "token 75")
        stream_76 = RewriteRuleTokenStream(self._adaptor, "token 76")
        stream_expr_list = RewriteRuleSubtreeStream(self._adaptor, "rule expr_list")
        try:
            try:
                # Expr.g:225:2: ( '(' ( expr_list )? ')' -> ^( CALL ( expr_list )? ) )
                # Expr.g:225:4: '(' ( expr_list )? ')'
                pass 
                char_literal170 = self.match(self.input, 75, self.FOLLOW_75_in_call_expr1398) 
                if self._state.backtracking == 0:
                    stream_75.add(char_literal170)


                # Expr.g:225:8: ( expr_list )?
                alt35 = 2
                LA35_0 = self.input.LA(1)

                if (LA35_0 == BOOL or LA35_0 == FLOAT or LA35_0 == ID or LA35_0 == INT or LA35_0 == NULL or LA35_0 == STRING or LA35_0 == 68 or LA35_0 == 75 or LA35_0 == 83 or LA35_0 == 100 or LA35_0 == 121 or LA35_0 == 126 or LA35_0 == 132) :
                    alt35 = 1
                if alt35 == 1:
                    # Expr.g:225:8: expr_list
                    pass 
                    self._state.following.append(self.FOLLOW_expr_list_in_call_expr1400)
                    expr_list171 = self.expr_list()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_expr_list.add(expr_list171.tree)





                char_literal172 = self.match(self.input, 76, self.FOLLOW_76_in_call_expr1403) 
                if self._state.backtracking == 0:
                    stream_76.add(char_literal172)


                # AST Rewrite
                # elements: expr_list
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 226:3: -> ^( CALL ( expr_list )? )
                    # Expr.g:226:6: ^( CALL ( expr_list )? )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(CALL, "CALL")
                    , root_1)

                    # Expr.g:226:13: ( expr_list )?
                    if stream_expr_list.hasNext():
                        self._adaptor.addChild(root_1, stream_expr_list.nextTree())


                    stream_expr_list.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "call_expr"


    class index_expr_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.index_expr_return, self).__init__()

            self.tree = None





    # $ANTLR start "index_expr"
    # Expr.g:228:1: index_expr options {backtrack=true; } : ( '[' expr ']' -> ^( INDEX expr ) | '[' expr '..' ( expr )? ']' -> ^( SLICE expr ( expr )? ) );
    def index_expr(self, ):
        retval = self.index_expr_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal173 = None
        char_literal175 = None
        char_literal176 = None
        string_literal178 = None
        char_literal180 = None
        expr174 = None
        expr177 = None
        expr179 = None

        char_literal173_tree = None
        char_literal175_tree = None
        char_literal176_tree = None
        string_literal178_tree = None
        char_literal180_tree = None
        stream_101 = RewriteRuleTokenStream(self._adaptor, "token 101")
        stream_88 = RewriteRuleTokenStream(self._adaptor, "token 88")
        stream_100 = RewriteRuleTokenStream(self._adaptor, "token 100")
        stream_expr = RewriteRuleSubtreeStream(self._adaptor, "rule expr")
        try:
            try:
                # Expr.g:232:2: ( '[' expr ']' -> ^( INDEX expr ) | '[' expr '..' ( expr )? ']' -> ^( SLICE expr ( expr )? ) )
                alt37 = 2
                LA37_0 = self.input.LA(1)

                if (LA37_0 == 100) :
                    LA37_1 = self.input.LA(2)

                    if (self.synpred1_Expr()) :
                        alt37 = 1
                    elif (True) :
                        alt37 = 2
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed


                        nvae = NoViableAltException("", 37, 1, self.input)

                        raise nvae


                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    nvae = NoViableAltException("", 37, 0, self.input)

                    raise nvae


                if alt37 == 1:
                    # Expr.g:232:4: '[' expr ']'
                    pass 
                    char_literal173 = self.match(self.input, 100, self.FOLLOW_100_in_index_expr1439) 
                    if self._state.backtracking == 0:
                        stream_100.add(char_literal173)


                    self._state.following.append(self.FOLLOW_expr_in_index_expr1441)
                    expr174 = self.expr()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_expr.add(expr174.tree)


                    char_literal175 = self.match(self.input, 101, self.FOLLOW_101_in_index_expr1443) 
                    if self._state.backtracking == 0:
                        stream_101.add(char_literal175)


                    # AST Rewrite
                    # elements: expr
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 233:3: -> ^( INDEX expr )
                        # Expr.g:233:6: ^( INDEX expr )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(INDEX, "INDEX")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_expr.nextTree())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt37 == 2:
                    # Expr.g:234:4: '[' expr '..' ( expr )? ']'
                    pass 
                    char_literal176 = self.match(self.input, 100, self.FOLLOW_100_in_index_expr1458) 
                    if self._state.backtracking == 0:
                        stream_100.add(char_literal176)


                    self._state.following.append(self.FOLLOW_expr_in_index_expr1460)
                    expr177 = self.expr()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_expr.add(expr177.tree)


                    string_literal178 = self.match(self.input, 88, self.FOLLOW_88_in_index_expr1462) 
                    if self._state.backtracking == 0:
                        stream_88.add(string_literal178)


                    # Expr.g:234:18: ( expr )?
                    alt36 = 2
                    LA36_0 = self.input.LA(1)

                    if (LA36_0 == BOOL or LA36_0 == FLOAT or LA36_0 == ID or LA36_0 == INT or LA36_0 == NULL or LA36_0 == STRING or LA36_0 == 68 or LA36_0 == 75 or LA36_0 == 83 or LA36_0 == 100 or LA36_0 == 121 or LA36_0 == 126 or LA36_0 == 132) :
                        alt36 = 1
                    if alt36 == 1:
                        # Expr.g:234:18: expr
                        pass 
                        self._state.following.append(self.FOLLOW_expr_in_index_expr1464)
                        expr179 = self.expr()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_expr.add(expr179.tree)





                    char_literal180 = self.match(self.input, 101, self.FOLLOW_101_in_index_expr1467) 
                    if self._state.backtracking == 0:
                        stream_101.add(char_literal180)


                    # AST Rewrite
                    # elements: expr, expr
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 235:3: -> ^( SLICE expr ( expr )? )
                        # Expr.g:235:6: ^( SLICE expr ( expr )? )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(SLICE, "SLICE")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_expr.nextTree())

                        # Expr.g:235:19: ( expr )?
                        if stream_expr.hasNext():
                            self._adaptor.addChild(root_1, stream_expr.nextTree())


                        stream_expr.reset();

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "index_expr"


    class exec_list_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.exec_list_return, self).__init__()

            self.tree = None





    # $ANTLR start "exec_list"
    # Expr.g:239:1: exec_list : exec_expr ( ',' exec_expr )* -> ^( EXEC_LIST ( exec_expr )+ ) ;
    def exec_list(self, ):
        retval = self.exec_list_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal182 = None
        exec_expr181 = None
        exec_expr183 = None

        char_literal182_tree = None
        stream_82 = RewriteRuleTokenStream(self._adaptor, "token 82")
        stream_exec_expr = RewriteRuleSubtreeStream(self._adaptor, "rule exec_expr")
        try:
            try:
                # Expr.g:240:2: ( exec_expr ( ',' exec_expr )* -> ^( EXEC_LIST ( exec_expr )+ ) )
                # Expr.g:240:4: exec_expr ( ',' exec_expr )*
                pass 
                self._state.following.append(self.FOLLOW_exec_expr_in_exec_list1492)
                exec_expr181 = self.exec_expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_exec_expr.add(exec_expr181.tree)


                # Expr.g:240:14: ( ',' exec_expr )*
                while True: #loop38
                    alt38 = 2
                    LA38_0 = self.input.LA(1)

                    if (LA38_0 == 82) :
                        alt38 = 1


                    if alt38 == 1:
                        # Expr.g:240:15: ',' exec_expr
                        pass 
                        char_literal182 = self.match(self.input, 82, self.FOLLOW_82_in_exec_list1495) 
                        if self._state.backtracking == 0:
                            stream_82.add(char_literal182)


                        self._state.following.append(self.FOLLOW_exec_expr_in_exec_list1497)
                        exec_expr183 = self.exec_expr()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_exec_expr.add(exec_expr183.tree)



                    else:
                        break #loop38


                # AST Rewrite
                # elements: exec_expr
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 241:3: -> ^( EXEC_LIST ( exec_expr )+ )
                    # Expr.g:241:6: ^( EXEC_LIST ( exec_expr )+ )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(EXEC_LIST, "EXEC_LIST")
                    , root_1)

                    # Expr.g:241:18: ( exec_expr )+
                    if not (stream_exec_expr.hasNext()):
                        raise RewriteEarlyExitException()

                    while stream_exec_expr.hasNext():
                        self._adaptor.addChild(root_1, stream_exec_expr.nextTree())


                    stream_exec_expr.reset()

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "exec_list"


    class member_list_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.member_list_return, self).__init__()

            self.tree = None





    # $ANTLR start "member_list"
    # Expr.g:243:1: member_list : member_expr ( ',' member_expr )* ;
    def member_list(self, ):
        retval = self.member_list_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal185 = None
        member_expr184 = None
        member_expr186 = None

        char_literal185_tree = None

        try:
            try:
                # Expr.g:244:2: ( member_expr ( ',' member_expr )* )
                # Expr.g:244:4: member_expr ( ',' member_expr )*
                pass 
                root_0 = self._adaptor.nil()


                self._state.following.append(self.FOLLOW_member_expr_in_member_list1520)
                member_expr184 = self.member_expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    self._adaptor.addChild(root_0, member_expr184.tree)


                # Expr.g:244:16: ( ',' member_expr )*
                while True: #loop39
                    alt39 = 2
                    LA39_0 = self.input.LA(1)

                    if (LA39_0 == 82) :
                        alt39 = 1


                    if alt39 == 1:
                        # Expr.g:244:17: ',' member_expr
                        pass 
                        char_literal185 = self.match(self.input, 82, self.FOLLOW_82_in_member_list1523)
                        if self._state.backtracking == 0:
                            char_literal185_tree = self._adaptor.createWithPayload(char_literal185)
                            self._adaptor.addChild(root_0, char_literal185_tree)



                        self._state.following.append(self.FOLLOW_member_expr_in_member_list1525)
                        member_expr186 = self.member_expr()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            self._adaptor.addChild(root_0, member_expr186.tree)



                    else:
                        break #loop39




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "member_list"


    class exec_expr_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.exec_expr_return, self).__init__()

            self.tree = None





    # $ANTLR start "exec_expr"
    # Expr.g:246:1: exec_expr : ( member_expr ( assign_op expr -> ^( ASSIGN member_expr assign_op expr ) | '++' -> ^( POST_INC member_expr ) | '--' -> ^( POST_DEC member_expr ) | -> member_expr ) | '++' member_expr -> ^( PRE_INC member_expr ) | '--' member_expr -> ^( PRE_DEC member_expr ) );
    def exec_expr(self, ):
        retval = self.exec_expr_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal190 = None
        string_literal191 = None
        string_literal192 = None
        string_literal194 = None
        member_expr187 = None
        assign_op188 = None
        expr189 = None
        member_expr193 = None
        member_expr195 = None

        string_literal190_tree = None
        string_literal191_tree = None
        string_literal192_tree = None
        string_literal194_tree = None
        stream_80 = RewriteRuleTokenStream(self._adaptor, "token 80")
        stream_84 = RewriteRuleTokenStream(self._adaptor, "token 84")
        stream_member_expr = RewriteRuleSubtreeStream(self._adaptor, "rule member_expr")
        stream_expr = RewriteRuleSubtreeStream(self._adaptor, "rule expr")
        stream_assign_op = RewriteRuleSubtreeStream(self._adaptor, "rule assign_op")
        try:
            try:
                # Expr.g:247:2: ( member_expr ( assign_op expr -> ^( ASSIGN member_expr assign_op expr ) | '++' -> ^( POST_INC member_expr ) | '--' -> ^( POST_DEC member_expr ) | -> member_expr ) | '++' member_expr -> ^( PRE_INC member_expr ) | '--' member_expr -> ^( PRE_DEC member_expr ) )
                alt41 = 3
                LA41 = self.input.LA(1)
                if LA41 == ID:
                    alt41 = 1
                elif LA41 == 80:
                    alt41 = 2
                elif LA41 == 84:
                    alt41 = 3
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    nvae = NoViableAltException("", 41, 0, self.input)

                    raise nvae


                if alt41 == 1:
                    # Expr.g:247:4: member_expr ( assign_op expr -> ^( ASSIGN member_expr assign_op expr ) | '++' -> ^( POST_INC member_expr ) | '--' -> ^( POST_DEC member_expr ) | -> member_expr )
                    pass 
                    self._state.following.append(self.FOLLOW_member_expr_in_exec_expr1537)
                    member_expr187 = self.member_expr()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_member_expr.add(member_expr187.tree)


                    # Expr.g:248:3: ( assign_op expr -> ^( ASSIGN member_expr assign_op expr ) | '++' -> ^( POST_INC member_expr ) | '--' -> ^( POST_DEC member_expr ) | -> member_expr )
                    alt40 = 4
                    LA40 = self.input.LA(1)
                    if LA40 == 71 or LA40 == 74 or LA40 == 78 or LA40 == 81 or LA40 == 85 or LA40 == 90 or LA40 == 95 or LA40 == 103 or LA40 == 134:
                        alt40 = 1
                    elif LA40 == 80:
                        alt40 = 2
                    elif LA40 == 84:
                        alt40 = 3
                    elif LA40 == 76 or LA40 == 82 or LA40 == 92:
                        alt40 = 4
                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed


                        nvae = NoViableAltException("", 40, 0, self.input)

                        raise nvae


                    if alt40 == 1:
                        # Expr.g:248:4: assign_op expr
                        pass 
                        self._state.following.append(self.FOLLOW_assign_op_in_exec_expr1542)
                        assign_op188 = self.assign_op()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_assign_op.add(assign_op188.tree)


                        self._state.following.append(self.FOLLOW_expr_in_exec_expr1544)
                        expr189 = self.expr()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_expr.add(expr189.tree)


                        # AST Rewrite
                        # elements: member_expr, expr, assign_op
                        # token labels: 
                        # rule labels: retval
                        # token list labels: 
                        # rule list labels: 
                        # wildcard labels: 
                        if self._state.backtracking == 0:
                            retval.tree = root_0
                            if retval is not None:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                            else:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                            root_0 = self._adaptor.nil()
                            # 249:4: -> ^( ASSIGN member_expr assign_op expr )
                            # Expr.g:249:7: ^( ASSIGN member_expr assign_op expr )
                            root_1 = self._adaptor.nil()
                            root_1 = self._adaptor.becomeRoot(
                            self._adaptor.createFromType(ASSIGN, "ASSIGN")
                            , root_1)

                            self._adaptor.addChild(root_1, stream_member_expr.nextTree())

                            self._adaptor.addChild(root_1, stream_assign_op.nextTree())

                            self._adaptor.addChild(root_1, stream_expr.nextTree())

                            self._adaptor.addChild(root_0, root_1)




                            retval.tree = root_0




                    elif alt40 == 2:
                        # Expr.g:250:5: '++'
                        pass 
                        string_literal190 = self.match(self.input, 80, self.FOLLOW_80_in_exec_expr1565) 
                        if self._state.backtracking == 0:
                            stream_80.add(string_literal190)


                        # AST Rewrite
                        # elements: member_expr
                        # token labels: 
                        # rule labels: retval
                        # token list labels: 
                        # rule list labels: 
                        # wildcard labels: 
                        if self._state.backtracking == 0:
                            retval.tree = root_0
                            if retval is not None:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                            else:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                            root_0 = self._adaptor.nil()
                            # 251:4: -> ^( POST_INC member_expr )
                            # Expr.g:251:7: ^( POST_INC member_expr )
                            root_1 = self._adaptor.nil()
                            root_1 = self._adaptor.becomeRoot(
                            self._adaptor.createFromType(POST_INC, "POST_INC")
                            , root_1)

                            self._adaptor.addChild(root_1, stream_member_expr.nextTree())

                            self._adaptor.addChild(root_0, root_1)




                            retval.tree = root_0




                    elif alt40 == 3:
                        # Expr.g:252:5: '--'
                        pass 
                        string_literal191 = self.match(self.input, 84, self.FOLLOW_84_in_exec_expr1582) 
                        if self._state.backtracking == 0:
                            stream_84.add(string_literal191)


                        # AST Rewrite
                        # elements: member_expr
                        # token labels: 
                        # rule labels: retval
                        # token list labels: 
                        # rule list labels: 
                        # wildcard labels: 
                        if self._state.backtracking == 0:
                            retval.tree = root_0
                            if retval is not None:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                            else:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                            root_0 = self._adaptor.nil()
                            # 253:4: -> ^( POST_DEC member_expr )
                            # Expr.g:253:7: ^( POST_DEC member_expr )
                            root_1 = self._adaptor.nil()
                            root_1 = self._adaptor.becomeRoot(
                            self._adaptor.createFromType(POST_DEC, "POST_DEC")
                            , root_1)

                            self._adaptor.addChild(root_1, stream_member_expr.nextTree())

                            self._adaptor.addChild(root_0, root_1)




                            retval.tree = root_0




                    elif alt40 == 4:
                        # Expr.g:255:4: 
                        pass 
                        # AST Rewrite
                        # elements: member_expr
                        # token labels: 
                        # rule labels: retval
                        # token list labels: 
                        # rule list labels: 
                        # wildcard labels: 
                        if self._state.backtracking == 0:
                            retval.tree = root_0
                            if retval is not None:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                            else:
                                stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                            root_0 = self._adaptor.nil()
                            # 255:4: -> member_expr
                            self._adaptor.addChild(root_0, stream_member_expr.nextTree())




                            retval.tree = root_0







                elif alt41 == 2:
                    # Expr.g:257:4: '++' member_expr
                    pass 
                    string_literal192 = self.match(self.input, 80, self.FOLLOW_80_in_exec_expr1613) 
                    if self._state.backtracking == 0:
                        stream_80.add(string_literal192)


                    self._state.following.append(self.FOLLOW_member_expr_in_exec_expr1615)
                    member_expr193 = self.member_expr()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_member_expr.add(member_expr193.tree)


                    # AST Rewrite
                    # elements: member_expr
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 258:3: -> ^( PRE_INC member_expr )
                        # Expr.g:258:6: ^( PRE_INC member_expr )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(PRE_INC, "PRE_INC")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_member_expr.nextTree())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                elif alt41 == 3:
                    # Expr.g:259:4: '--' member_expr
                    pass 
                    string_literal194 = self.match(self.input, 84, self.FOLLOW_84_in_exec_expr1630) 
                    if self._state.backtracking == 0:
                        stream_84.add(string_literal194)


                    self._state.following.append(self.FOLLOW_member_expr_in_exec_expr1632)
                    member_expr195 = self.member_expr()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_member_expr.add(member_expr195.tree)


                    # AST Rewrite
                    # elements: member_expr
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 260:3: -> ^( PRE_DEC member_expr )
                        # Expr.g:260:6: ^( PRE_DEC member_expr )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(PRE_DEC, "PRE_DEC")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_member_expr.nextTree())

                        self._adaptor.addChild(root_0, root_1)




                        retval.tree = root_0




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "exec_expr"


    class assign_op_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.assign_op_return, self).__init__()

            self.tree = None





    # $ANTLR start "assign_op"
    # Expr.g:262:1: assign_op : ( '=' | '+=' | '-=' | '*=' | '/=' | '%=' | '&=' | '^=' | '|=' );
    def assign_op(self, ):
        retval = self.assign_op_return()
        retval.start = self.input.LT(1)


        root_0 = None

        set196 = None

        set196_tree = None

        try:
            try:
                # Expr.g:263:2: ( '=' | '+=' | '-=' | '*=' | '/=' | '%=' | '&=' | '^=' | '|=' )
                # Expr.g:
                pass 
                root_0 = self._adaptor.nil()


                set196 = self.input.LT(1)

                if self.input.LA(1) == 71 or self.input.LA(1) == 74 or self.input.LA(1) == 78 or self.input.LA(1) == 81 or self.input.LA(1) == 85 or self.input.LA(1) == 90 or self.input.LA(1) == 95 or self.input.LA(1) == 103 or self.input.LA(1) == 134:
                    self.input.consume()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, self._adaptor.createWithPayload(set196))

                    self._state.errorRecovery = False


                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    mse = MismatchedSetException(None, self.input)
                    raise mse





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "assign_op"


    class exec_stmt_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.exec_stmt_return, self).__init__()

            self.tree = None





    # $ANTLR start "exec_stmt"
    # Expr.g:265:1: exec_stmt : exec_list ';' -> ^( EXEC_STMT exec_list ) ;
    def exec_stmt(self, ):
        retval = self.exec_stmt_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal198 = None
        exec_list197 = None

        char_literal198_tree = None
        stream_92 = RewriteRuleTokenStream(self._adaptor, "token 92")
        stream_exec_list = RewriteRuleSubtreeStream(self._adaptor, "rule exec_list")
        try:
            try:
                # Expr.g:266:2: ( exec_list ';' -> ^( EXEC_STMT exec_list ) )
                # Expr.g:266:4: exec_list ';'
                pass 
                self._state.following.append(self.FOLLOW_exec_list_in_exec_stmt1678)
                exec_list197 = self.exec_list()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_exec_list.add(exec_list197.tree)


                char_literal198 = self.match(self.input, 92, self.FOLLOW_92_in_exec_stmt1680) 
                if self._state.backtracking == 0:
                    stream_92.add(char_literal198)


                # AST Rewrite
                # elements: exec_list
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 267:3: -> ^( EXEC_STMT exec_list )
                    # Expr.g:267:6: ^( EXEC_STMT exec_list )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(EXEC_STMT, "EXEC_STMT")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_exec_list.nextTree())

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "exec_stmt"


    class expr_list_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.expr_list_return, self).__init__()

            self.tree = None





    # $ANTLR start "expr_list"
    # Expr.g:272:1: expr_list : expr ( ',' expr )* ( ',' )? -> ^( EXPR_LIST ( expr )+ ) ;
    def expr_list(self, ):
        retval = self.expr_list_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal200 = None
        char_literal202 = None
        expr199 = None
        expr201 = None

        char_literal200_tree = None
        char_literal202_tree = None
        stream_82 = RewriteRuleTokenStream(self._adaptor, "token 82")
        stream_expr = RewriteRuleSubtreeStream(self._adaptor, "rule expr")
        try:
            try:
                # Expr.g:273:2: ( expr ( ',' expr )* ( ',' )? -> ^( EXPR_LIST ( expr )+ ) )
                # Expr.g:273:4: expr ( ',' expr )* ( ',' )?
                pass 
                self._state.following.append(self.FOLLOW_expr_in_expr_list1703)
                expr199 = self.expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_expr.add(expr199.tree)


                # Expr.g:273:9: ( ',' expr )*
                while True: #loop42
                    alt42 = 2
                    LA42_0 = self.input.LA(1)

                    if (LA42_0 == 82) :
                        LA42_1 = self.input.LA(2)

                        if (LA42_1 == BOOL or LA42_1 == FLOAT or LA42_1 == ID or LA42_1 == INT or LA42_1 == NULL or LA42_1 == STRING or LA42_1 == 68 or LA42_1 == 75 or LA42_1 == 83 or LA42_1 == 100 or LA42_1 == 121 or LA42_1 == 126 or LA42_1 == 132) :
                            alt42 = 1




                    if alt42 == 1:
                        # Expr.g:273:10: ',' expr
                        pass 
                        char_literal200 = self.match(self.input, 82, self.FOLLOW_82_in_expr_list1706) 
                        if self._state.backtracking == 0:
                            stream_82.add(char_literal200)


                        self._state.following.append(self.FOLLOW_expr_in_expr_list1708)
                        expr201 = self.expr()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_expr.add(expr201.tree)



                    else:
                        break #loop42


                # Expr.g:273:21: ( ',' )?
                alt43 = 2
                LA43_0 = self.input.LA(1)

                if (LA43_0 == 82) :
                    alt43 = 1
                if alt43 == 1:
                    # Expr.g:273:21: ','
                    pass 
                    char_literal202 = self.match(self.input, 82, self.FOLLOW_82_in_expr_list1712) 
                    if self._state.backtracking == 0:
                        stream_82.add(char_literal202)





                # AST Rewrite
                # elements: expr
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 274:3: -> ^( EXPR_LIST ( expr )+ )
                    # Expr.g:274:6: ^( EXPR_LIST ( expr )+ )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(EXPR_LIST, "EXPR_LIST")
                    , root_1)

                    # Expr.g:274:18: ( expr )+
                    if not (stream_expr.hasNext()):
                        raise RewriteEarlyExitException()

                    while stream_expr.hasNext():
                        self._adaptor.addChild(root_1, stream_expr.nextTree())


                    stream_expr.reset()

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "expr_list"


    class expr_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.expr_return, self).__init__()

            self.tree = None





    # $ANTLR start "expr"
    # Expr.g:276:1: expr : logic_or_expr ;
    def expr(self, ):
        retval = self.expr_return()
        retval.start = self.input.LT(1)


        root_0 = None

        logic_or_expr203 = None


        try:
            try:
                # Expr.g:277:2: ( logic_or_expr )
                # Expr.g:277:4: logic_or_expr
                pass 
                root_0 = self._adaptor.nil()


                self._state.following.append(self.FOLLOW_logic_or_expr_in_expr1734)
                logic_or_expr203 = self.logic_or_expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    self._adaptor.addChild(root_0, logic_or_expr203.tree)




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "expr"


    class logic_or_expr_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.logic_or_expr_return, self).__init__()

            self.tree = None





    # $ANTLR start "logic_or_expr"
    # Expr.g:279:1: logic_or_expr : logic_and_expr ( '||' ^ logic_and_expr )* ;
    def logic_or_expr(self, ):
        retval = self.logic_or_expr_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal205 = None
        logic_and_expr204 = None
        logic_and_expr206 = None

        string_literal205_tree = None

        try:
            try:
                # Expr.g:280:2: ( logic_and_expr ( '||' ^ logic_and_expr )* )
                # Expr.g:280:4: logic_and_expr ( '||' ^ logic_and_expr )*
                pass 
                root_0 = self._adaptor.nil()


                self._state.following.append(self.FOLLOW_logic_and_expr_in_logic_or_expr1744)
                logic_and_expr204 = self.logic_and_expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    self._adaptor.addChild(root_0, logic_and_expr204.tree)


                # Expr.g:280:19: ( '||' ^ logic_and_expr )*
                while True: #loop44
                    alt44 = 2
                    LA44_0 = self.input.LA(1)

                    if (LA44_0 == 135) :
                        alt44 = 1


                    if alt44 == 1:
                        # Expr.g:280:20: '||' ^ logic_and_expr
                        pass 
                        string_literal205 = self.match(self.input, 135, self.FOLLOW_135_in_logic_or_expr1747)
                        if self._state.backtracking == 0:
                            string_literal205_tree = self._adaptor.createWithPayload(string_literal205)
                            root_0 = self._adaptor.becomeRoot(string_literal205_tree, root_0)



                        self._state.following.append(self.FOLLOW_logic_and_expr_in_logic_or_expr1750)
                        logic_and_expr206 = self.logic_and_expr()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            self._adaptor.addChild(root_0, logic_and_expr206.tree)



                    else:
                        break #loop44




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "logic_or_expr"


    class logic_and_expr_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.logic_and_expr_return, self).__init__()

            self.tree = None





    # $ANTLR start "logic_and_expr"
    # Expr.g:282:1: logic_and_expr : bitwise_or_expr ( '&&' ^ bitwise_or_expr )* ;
    def logic_and_expr(self, ):
        retval = self.logic_and_expr_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal208 = None
        bitwise_or_expr207 = None
        bitwise_or_expr209 = None

        string_literal208_tree = None

        try:
            try:
                # Expr.g:283:2: ( bitwise_or_expr ( '&&' ^ bitwise_or_expr )* )
                # Expr.g:283:4: bitwise_or_expr ( '&&' ^ bitwise_or_expr )*
                pass 
                root_0 = self._adaptor.nil()


                self._state.following.append(self.FOLLOW_bitwise_or_expr_in_logic_and_expr1762)
                bitwise_or_expr207 = self.bitwise_or_expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    self._adaptor.addChild(root_0, bitwise_or_expr207.tree)


                # Expr.g:283:20: ( '&&' ^ bitwise_or_expr )*
                while True: #loop45
                    alt45 = 2
                    LA45_0 = self.input.LA(1)

                    if (LA45_0 == 72) :
                        alt45 = 1


                    if alt45 == 1:
                        # Expr.g:283:21: '&&' ^ bitwise_or_expr
                        pass 
                        string_literal208 = self.match(self.input, 72, self.FOLLOW_72_in_logic_and_expr1765)
                        if self._state.backtracking == 0:
                            string_literal208_tree = self._adaptor.createWithPayload(string_literal208)
                            root_0 = self._adaptor.becomeRoot(string_literal208_tree, root_0)



                        self._state.following.append(self.FOLLOW_bitwise_or_expr_in_logic_and_expr1768)
                        bitwise_or_expr209 = self.bitwise_or_expr()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            self._adaptor.addChild(root_0, bitwise_or_expr209.tree)



                    else:
                        break #loop45




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "logic_and_expr"


    class bitwise_or_expr_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.bitwise_or_expr_return, self).__init__()

            self.tree = None





    # $ANTLR start "bitwise_or_expr"
    # Expr.g:285:1: bitwise_or_expr : bitwise_xor_expr ( '|' ^ bitwise_xor_expr )* ;
    def bitwise_or_expr(self, ):
        retval = self.bitwise_or_expr_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal211 = None
        bitwise_xor_expr210 = None
        bitwise_xor_expr212 = None

        char_literal211_tree = None

        try:
            try:
                # Expr.g:286:2: ( bitwise_xor_expr ( '|' ^ bitwise_xor_expr )* )
                # Expr.g:286:4: bitwise_xor_expr ( '|' ^ bitwise_xor_expr )*
                pass 
                root_0 = self._adaptor.nil()


                self._state.following.append(self.FOLLOW_bitwise_xor_expr_in_bitwise_or_expr1780)
                bitwise_xor_expr210 = self.bitwise_xor_expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    self._adaptor.addChild(root_0, bitwise_xor_expr210.tree)


                # Expr.g:286:21: ( '|' ^ bitwise_xor_expr )*
                while True: #loop46
                    alt46 = 2
                    LA46_0 = self.input.LA(1)

                    if (LA46_0 == 133) :
                        alt46 = 1


                    if alt46 == 1:
                        # Expr.g:286:22: '|' ^ bitwise_xor_expr
                        pass 
                        char_literal211 = self.match(self.input, 133, self.FOLLOW_133_in_bitwise_or_expr1783)
                        if self._state.backtracking == 0:
                            char_literal211_tree = self._adaptor.createWithPayload(char_literal211)
                            root_0 = self._adaptor.becomeRoot(char_literal211_tree, root_0)



                        self._state.following.append(self.FOLLOW_bitwise_xor_expr_in_bitwise_or_expr1786)
                        bitwise_xor_expr212 = self.bitwise_xor_expr()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            self._adaptor.addChild(root_0, bitwise_xor_expr212.tree)



                    else:
                        break #loop46




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "bitwise_or_expr"


    class bitwise_xor_expr_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.bitwise_xor_expr_return, self).__init__()

            self.tree = None





    # $ANTLR start "bitwise_xor_expr"
    # Expr.g:288:1: bitwise_xor_expr : bitwise_and_expr ( '^' ^ bitwise_and_expr )* ;
    def bitwise_xor_expr(self, ):
        retval = self.bitwise_xor_expr_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal214 = None
        bitwise_and_expr213 = None
        bitwise_and_expr215 = None

        char_literal214_tree = None

        try:
            try:
                # Expr.g:289:2: ( bitwise_and_expr ( '^' ^ bitwise_and_expr )* )
                # Expr.g:289:4: bitwise_and_expr ( '^' ^ bitwise_and_expr )*
                pass 
                root_0 = self._adaptor.nil()


                self._state.following.append(self.FOLLOW_bitwise_and_expr_in_bitwise_xor_expr1798)
                bitwise_and_expr213 = self.bitwise_and_expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    self._adaptor.addChild(root_0, bitwise_and_expr213.tree)


                # Expr.g:289:21: ( '^' ^ bitwise_and_expr )*
                while True: #loop47
                    alt47 = 2
                    LA47_0 = self.input.LA(1)

                    if (LA47_0 == 102) :
                        alt47 = 1


                    if alt47 == 1:
                        # Expr.g:289:22: '^' ^ bitwise_and_expr
                        pass 
                        char_literal214 = self.match(self.input, 102, self.FOLLOW_102_in_bitwise_xor_expr1801)
                        if self._state.backtracking == 0:
                            char_literal214_tree = self._adaptor.createWithPayload(char_literal214)
                            root_0 = self._adaptor.becomeRoot(char_literal214_tree, root_0)



                        self._state.following.append(self.FOLLOW_bitwise_and_expr_in_bitwise_xor_expr1804)
                        bitwise_and_expr215 = self.bitwise_and_expr()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            self._adaptor.addChild(root_0, bitwise_and_expr215.tree)



                    else:
                        break #loop47




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "bitwise_xor_expr"


    class bitwise_and_expr_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.bitwise_and_expr_return, self).__init__()

            self.tree = None





    # $ANTLR start "bitwise_and_expr"
    # Expr.g:291:1: bitwise_and_expr : relation_expr ( '&' ^ relation_expr )* ;
    def bitwise_and_expr(self, ):
        retval = self.bitwise_and_expr_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal217 = None
        relation_expr216 = None
        relation_expr218 = None

        char_literal217_tree = None

        try:
            try:
                # Expr.g:292:2: ( relation_expr ( '&' ^ relation_expr )* )
                # Expr.g:292:4: relation_expr ( '&' ^ relation_expr )*
                pass 
                root_0 = self._adaptor.nil()


                self._state.following.append(self.FOLLOW_relation_expr_in_bitwise_and_expr1816)
                relation_expr216 = self.relation_expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    self._adaptor.addChild(root_0, relation_expr216.tree)


                # Expr.g:292:18: ( '&' ^ relation_expr )*
                while True: #loop48
                    alt48 = 2
                    LA48_0 = self.input.LA(1)

                    if (LA48_0 == 73) :
                        alt48 = 1


                    if alt48 == 1:
                        # Expr.g:292:19: '&' ^ relation_expr
                        pass 
                        char_literal217 = self.match(self.input, 73, self.FOLLOW_73_in_bitwise_and_expr1819)
                        if self._state.backtracking == 0:
                            char_literal217_tree = self._adaptor.createWithPayload(char_literal217)
                            root_0 = self._adaptor.becomeRoot(char_literal217_tree, root_0)



                        self._state.following.append(self.FOLLOW_relation_expr_in_bitwise_and_expr1822)
                        relation_expr218 = self.relation_expr()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            self._adaptor.addChild(root_0, relation_expr218.tree)



                    else:
                        break #loop48




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "bitwise_and_expr"


    class relation_expr_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.relation_expr_return, self).__init__()

            self.tree = None





    # $ANTLR start "relation_expr"
    # Expr.g:294:1: relation_expr : add_expr ( ( '<' | '>' | '<=' | '>=' | '==' | '!=' ) ^ add_expr )? ;
    def relation_expr(self, ):
        retval = self.relation_expr_return()
        retval.start = self.input.LT(1)


        root_0 = None

        set220 = None
        add_expr219 = None
        add_expr221 = None

        set220_tree = None

        try:
            try:
                # Expr.g:295:2: ( add_expr ( ( '<' | '>' | '<=' | '>=' | '==' | '!=' ) ^ add_expr )? )
                # Expr.g:295:4: add_expr ( ( '<' | '>' | '<=' | '>=' | '==' | '!=' ) ^ add_expr )?
                pass 
                root_0 = self._adaptor.nil()


                self._state.following.append(self.FOLLOW_add_expr_in_relation_expr1834)
                add_expr219 = self.add_expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    self._adaptor.addChild(root_0, add_expr219.tree)


                # Expr.g:295:13: ( ( '<' | '>' | '<=' | '>=' | '==' | '!=' ) ^ add_expr )?
                alt49 = 2
                LA49_0 = self.input.LA(1)

                if (LA49_0 == 69 or (93 <= LA49_0 <= 94) or LA49_0 == 96 or (98 <= LA49_0 <= 99)) :
                    alt49 = 1
                if alt49 == 1:
                    # Expr.g:295:14: ( '<' | '>' | '<=' | '>=' | '==' | '!=' ) ^ add_expr
                    pass 
                    set220 = self.input.LT(1)

                    set220 = self.input.LT(1)

                    if self.input.LA(1) == 69 or (93 <= self.input.LA(1) <= 94) or self.input.LA(1) == 96 or (98 <= self.input.LA(1) <= 99):
                        self.input.consume()
                        if self._state.backtracking == 0:
                            root_0 = self._adaptor.becomeRoot(self._adaptor.createWithPayload(set220), root_0)

                        self._state.errorRecovery = False


                    else:
                        if self._state.backtracking > 0:
                            raise BacktrackingFailed


                        mse = MismatchedSetException(None, self.input)
                        raise mse



                    self._state.following.append(self.FOLLOW_add_expr_in_relation_expr1852)
                    add_expr221 = self.add_expr()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, add_expr221.tree)







                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "relation_expr"


    class add_expr_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.add_expr_return, self).__init__()

            self.tree = None





    # $ANTLR start "add_expr"
    # Expr.g:297:1: add_expr : mul_expr ( ( '+' | '-' ) ^ mul_expr )* ;
    def add_expr(self, ):
        retval = self.add_expr_return()
        retval.start = self.input.LT(1)


        root_0 = None

        set223 = None
        mul_expr222 = None
        mul_expr224 = None

        set223_tree = None

        try:
            try:
                # Expr.g:298:2: ( mul_expr ( ( '+' | '-' ) ^ mul_expr )* )
                # Expr.g:298:4: mul_expr ( ( '+' | '-' ) ^ mul_expr )*
                pass 
                root_0 = self._adaptor.nil()


                self._state.following.append(self.FOLLOW_mul_expr_in_add_expr1864)
                mul_expr222 = self.mul_expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    self._adaptor.addChild(root_0, mul_expr222.tree)


                # Expr.g:298:13: ( ( '+' | '-' ) ^ mul_expr )*
                while True: #loop50
                    alt50 = 2
                    LA50_0 = self.input.LA(1)

                    if (LA50_0 == 79 or LA50_0 == 83) :
                        alt50 = 1


                    if alt50 == 1:
                        # Expr.g:298:14: ( '+' | '-' ) ^ mul_expr
                        pass 
                        set223 = self.input.LT(1)

                        set223 = self.input.LT(1)

                        if self.input.LA(1) == 79 or self.input.LA(1) == 83:
                            self.input.consume()
                            if self._state.backtracking == 0:
                                root_0 = self._adaptor.becomeRoot(self._adaptor.createWithPayload(set223), root_0)

                            self._state.errorRecovery = False


                        else:
                            if self._state.backtracking > 0:
                                raise BacktrackingFailed


                            mse = MismatchedSetException(None, self.input)
                            raise mse



                        self._state.following.append(self.FOLLOW_mul_expr_in_add_expr1874)
                        mul_expr224 = self.mul_expr()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            self._adaptor.addChild(root_0, mul_expr224.tree)



                    else:
                        break #loop50




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "add_expr"


    class mul_expr_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.mul_expr_return, self).__init__()

            self.tree = None





    # $ANTLR start "mul_expr"
    # Expr.g:300:1: mul_expr : not_expr ( ( '*' | '/' | '%' ) ^ not_expr )* ;
    def mul_expr(self, ):
        retval = self.mul_expr_return()
        retval.start = self.input.LT(1)


        root_0 = None

        set226 = None
        not_expr225 = None
        not_expr227 = None

        set226_tree = None

        try:
            try:
                # Expr.g:301:2: ( not_expr ( ( '*' | '/' | '%' ) ^ not_expr )* )
                # Expr.g:301:4: not_expr ( ( '*' | '/' | '%' ) ^ not_expr )*
                pass 
                root_0 = self._adaptor.nil()


                self._state.following.append(self.FOLLOW_not_expr_in_mul_expr1886)
                not_expr225 = self.not_expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    self._adaptor.addChild(root_0, not_expr225.tree)


                # Expr.g:301:13: ( ( '*' | '/' | '%' ) ^ not_expr )*
                while True: #loop51
                    alt51 = 2
                    LA51_0 = self.input.LA(1)

                    if (LA51_0 == 70 or LA51_0 == 77 or LA51_0 == 89) :
                        alt51 = 1


                    if alt51 == 1:
                        # Expr.g:301:14: ( '*' | '/' | '%' ) ^ not_expr
                        pass 
                        set226 = self.input.LT(1)

                        set226 = self.input.LT(1)

                        if self.input.LA(1) == 70 or self.input.LA(1) == 77 or self.input.LA(1) == 89:
                            self.input.consume()
                            if self._state.backtracking == 0:
                                root_0 = self._adaptor.becomeRoot(self._adaptor.createWithPayload(set226), root_0)

                            self._state.errorRecovery = False


                        else:
                            if self._state.backtracking > 0:
                                raise BacktrackingFailed


                            mse = MismatchedSetException(None, self.input)
                            raise mse



                        self._state.following.append(self.FOLLOW_not_expr_in_mul_expr1898)
                        not_expr227 = self.not_expr()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            self._adaptor.addChild(root_0, not_expr227.tree)



                    else:
                        break #loop51




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "mul_expr"


    class not_expr_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.not_expr_return, self).__init__()

            self.tree = None





    # $ANTLR start "not_expr"
    # Expr.g:303:1: not_expr : (op= '!' )? negative_expr -> {$op != None}? ^( '!' negative_expr ) -> negative_expr ;
    def not_expr(self, ):
        retval = self.not_expr_return()
        retval.start = self.input.LT(1)


        root_0 = None

        op = None
        negative_expr228 = None

        op_tree = None
        stream_68 = RewriteRuleTokenStream(self._adaptor, "token 68")
        stream_negative_expr = RewriteRuleSubtreeStream(self._adaptor, "rule negative_expr")
        try:
            try:
                # Expr.g:304:2: ( (op= '!' )? negative_expr -> {$op != None}? ^( '!' negative_expr ) -> negative_expr )
                # Expr.g:304:4: (op= '!' )? negative_expr
                pass 
                # Expr.g:304:6: (op= '!' )?
                alt52 = 2
                LA52_0 = self.input.LA(1)

                if (LA52_0 == 68) :
                    alt52 = 1
                if alt52 == 1:
                    # Expr.g:304:6: op= '!'
                    pass 
                    op = self.match(self.input, 68, self.FOLLOW_68_in_not_expr1912) 
                    if self._state.backtracking == 0:
                        stream_68.add(op)





                self._state.following.append(self.FOLLOW_negative_expr_in_not_expr1915)
                negative_expr228 = self.negative_expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_negative_expr.add(negative_expr228.tree)


                # AST Rewrite
                # elements: negative_expr, negative_expr, 68
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    if op != None:
                        # 305:3: -> {$op != None}? ^( '!' negative_expr )
                        # Expr.g:306:4: ^( '!' negative_expr )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        stream_68.nextNode()
                        , root_1)

                        self._adaptor.addChild(root_1, stream_negative_expr.nextTree())

                        self._adaptor.addChild(root_0, root_1)



                    else: 
                        # 307:4: -> negative_expr
                        self._adaptor.addChild(root_0, stream_negative_expr.nextTree())



                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "not_expr"


    class negative_expr_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.negative_expr_return, self).__init__()

            self.tree = None





    # $ANTLR start "negative_expr"
    # Expr.g:309:1: negative_expr : (op= '-' )? atom -> {$op != None}? ^( NEGATIVE atom ) -> atom ;
    def negative_expr(self, ):
        retval = self.negative_expr_return()
        retval.start = self.input.LT(1)


        root_0 = None

        op = None
        atom229 = None

        op_tree = None
        stream_83 = RewriteRuleTokenStream(self._adaptor, "token 83")
        stream_atom = RewriteRuleSubtreeStream(self._adaptor, "rule atom")
        try:
            try:
                # Expr.g:310:2: ( (op= '-' )? atom -> {$op != None}? ^( NEGATIVE atom ) -> atom )
                # Expr.g:310:4: (op= '-' )? atom
                pass 
                # Expr.g:310:4: (op= '-' )?
                alt53 = 2
                LA53_0 = self.input.LA(1)

                if (LA53_0 == 83) :
                    alt53 = 1
                if alt53 == 1:
                    # Expr.g:310:5: op= '-'
                    pass 
                    op = self.match(self.input, 83, self.FOLLOW_83_in_negative_expr1950) 
                    if self._state.backtracking == 0:
                        stream_83.add(op)





                self._state.following.append(self.FOLLOW_atom_in_negative_expr1954)
                atom229 = self.atom()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_atom.add(atom229.tree)


                # AST Rewrite
                # elements: atom, atom
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    if op != None:
                        # 311:3: -> {$op != None}? ^( NEGATIVE atom )
                        # Expr.g:312:4: ^( NEGATIVE atom )
                        root_1 = self._adaptor.nil()
                        root_1 = self._adaptor.becomeRoot(
                        self._adaptor.createFromType(NEGATIVE, "NEGATIVE")
                        , root_1)

                        self._adaptor.addChild(root_1, stream_atom.nextTree())

                        self._adaptor.addChild(root_0, root_1)



                    else: 
                        # 313:4: -> atom
                        self._adaptor.addChild(root_0, stream_atom.nextTree())



                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "negative_expr"


    class atom_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.atom_return, self).__init__()

            self.tree = None





    # $ANTLR start "atom"
    # Expr.g:316:1: atom : ( literal | member_expr | array_decl | object_decl | new_clause | sprintf | '(' expr ')' -> expr );
    def atom(self, ):
        retval = self.atom_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal236 = None
        char_literal238 = None
        literal230 = None
        member_expr231 = None
        array_decl232 = None
        object_decl233 = None
        new_clause234 = None
        sprintf235 = None
        expr237 = None

        char_literal236_tree = None
        char_literal238_tree = None
        stream_75 = RewriteRuleTokenStream(self._adaptor, "token 75")
        stream_76 = RewriteRuleTokenStream(self._adaptor, "token 76")
        stream_expr = RewriteRuleSubtreeStream(self._adaptor, "rule expr")
        try:
            try:
                # Expr.g:317:2: ( literal | member_expr | array_decl | object_decl | new_clause | sprintf | '(' expr ')' -> expr )
                alt54 = 7
                LA54 = self.input.LA(1)
                if LA54 == BOOL or LA54 == FLOAT or LA54 == INT or LA54 == NULL or LA54 == STRING:
                    alt54 = 1
                elif LA54 == ID:
                    alt54 = 2
                elif LA54 == 100:
                    alt54 = 3
                elif LA54 == 132:
                    alt54 = 4
                elif LA54 == 121:
                    alt54 = 5
                elif LA54 == 126:
                    alt54 = 6
                elif LA54 == 75:
                    alt54 = 7
                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    nvae = NoViableAltException("", 54, 0, self.input)

                    raise nvae


                if alt54 == 1:
                    # Expr.g:317:4: literal
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_literal_in_atom1987)
                    literal230 = self.literal()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, literal230.tree)



                elif alt54 == 2:
                    # Expr.g:318:4: member_expr
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_member_expr_in_atom1992)
                    member_expr231 = self.member_expr()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, member_expr231.tree)



                elif alt54 == 3:
                    # Expr.g:319:4: array_decl
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_array_decl_in_atom1997)
                    array_decl232 = self.array_decl()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, array_decl232.tree)



                elif alt54 == 4:
                    # Expr.g:320:4: object_decl
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_object_decl_in_atom2002)
                    object_decl233 = self.object_decl()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, object_decl233.tree)



                elif alt54 == 5:
                    # Expr.g:321:4: new_clause
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_new_clause_in_atom2007)
                    new_clause234 = self.new_clause()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, new_clause234.tree)



                elif alt54 == 6:
                    # Expr.g:322:4: sprintf
                    pass 
                    root_0 = self._adaptor.nil()


                    self._state.following.append(self.FOLLOW_sprintf_in_atom2012)
                    sprintf235 = self.sprintf()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, sprintf235.tree)



                elif alt54 == 7:
                    # Expr.g:323:4: '(' expr ')'
                    pass 
                    char_literal236 = self.match(self.input, 75, self.FOLLOW_75_in_atom2017) 
                    if self._state.backtracking == 0:
                        stream_75.add(char_literal236)


                    self._state.following.append(self.FOLLOW_expr_in_atom2019)
                    expr237 = self.expr()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_expr.add(expr237.tree)


                    char_literal238 = self.match(self.input, 76, self.FOLLOW_76_in_atom2021) 
                    if self._state.backtracking == 0:
                        stream_76.add(char_literal238)


                    # AST Rewrite
                    # elements: expr
                    # token labels: 
                    # rule labels: retval
                    # token list labels: 
                    # rule list labels: 
                    # wildcard labels: 
                    if self._state.backtracking == 0:
                        retval.tree = root_0
                        if retval is not None:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                        else:
                            stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                        root_0 = self._adaptor.nil()
                        # 323:17: -> expr
                        self._adaptor.addChild(root_0, stream_expr.nextTree())




                        retval.tree = root_0




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "atom"


    class literal_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.literal_return, self).__init__()

            self.tree = None





    # $ANTLR start "literal"
    # Expr.g:325:1: literal : ( BOOL | NULL | INT | FLOAT | STRING );
    def literal(self, ):
        retval = self.literal_return()
        retval.start = self.input.LT(1)


        root_0 = None

        set239 = None

        set239_tree = None

        try:
            try:
                # Expr.g:326:2: ( BOOL | NULL | INT | FLOAT | STRING )
                # Expr.g:
                pass 
                root_0 = self._adaptor.nil()


                set239 = self.input.LT(1)

                if self.input.LA(1) == BOOL or self.input.LA(1) == FLOAT or self.input.LA(1) == INT or self.input.LA(1) == NULL or self.input.LA(1) == STRING:
                    self.input.consume()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, self._adaptor.createWithPayload(set239))

                    self._state.errorRecovery = False


                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    mse = MismatchedSetException(None, self.input)
                    raise mse





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "literal"


    class new_clause_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.new_clause_return, self).__init__()

            self.tree = None





    # $ANTLR start "new_clause"
    # Expr.g:329:1: new_clause : 'new' module call_expr -> ^( NEW module call_expr ) ;
    def new_clause(self, ):
        retval = self.new_clause_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal240 = None
        module241 = None
        call_expr242 = None

        string_literal240_tree = None
        stream_121 = RewriteRuleTokenStream(self._adaptor, "token 121")
        stream_module = RewriteRuleSubtreeStream(self._adaptor, "rule module")
        stream_call_expr = RewriteRuleSubtreeStream(self._adaptor, "rule call_expr")
        try:
            try:
                # Expr.g:330:2: ( 'new' module call_expr -> ^( NEW module call_expr ) )
                # Expr.g:330:4: 'new' module call_expr
                pass 
                string_literal240 = self.match(self.input, 121, self.FOLLOW_121_in_new_clause2062) 
                if self._state.backtracking == 0:
                    stream_121.add(string_literal240)


                self._state.following.append(self.FOLLOW_module_in_new_clause2064)
                module241 = self.module()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_module.add(module241.tree)


                self._state.following.append(self.FOLLOW_call_expr_in_new_clause2066)
                call_expr242 = self.call_expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_call_expr.add(call_expr242.tree)


                # AST Rewrite
                # elements: module, call_expr
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 331:3: -> ^( NEW module call_expr )
                    # Expr.g:331:6: ^( NEW module call_expr )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(NEW, "NEW")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_module.nextTree())

                    self._adaptor.addChild(root_1, stream_call_expr.nextTree())

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "new_clause"


    class module_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.module_return, self).__init__()

            self.tree = None





    # $ANTLR start "module"
    # Expr.g:333:1: module : ID ( '.' ID )* -> ^( MODULE ( ID )+ ) ;
    def module(self, ):
        retval = self.module_return()
        retval.start = self.input.LT(1)


        root_0 = None

        ID243 = None
        char_literal244 = None
        ID245 = None

        ID243_tree = None
        char_literal244_tree = None
        ID245_tree = None
        stream_ID = RewriteRuleTokenStream(self._adaptor, "token ID")
        stream_86 = RewriteRuleTokenStream(self._adaptor, "token 86")

        try:
            try:
                # Expr.g:334:2: ( ID ( '.' ID )* -> ^( MODULE ( ID )+ ) )
                # Expr.g:334:4: ID ( '.' ID )*
                pass 
                ID243 = self.match(self.input, ID, self.FOLLOW_ID_in_module2088) 
                if self._state.backtracking == 0:
                    stream_ID.add(ID243)


                # Expr.g:334:7: ( '.' ID )*
                while True: #loop55
                    alt55 = 2
                    LA55_0 = self.input.LA(1)

                    if (LA55_0 == 86) :
                        alt55 = 1


                    if alt55 == 1:
                        # Expr.g:334:8: '.' ID
                        pass 
                        char_literal244 = self.match(self.input, 86, self.FOLLOW_86_in_module2091) 
                        if self._state.backtracking == 0:
                            stream_86.add(char_literal244)


                        ID245 = self.match(self.input, ID, self.FOLLOW_ID_in_module2093) 
                        if self._state.backtracking == 0:
                            stream_ID.add(ID245)



                    else:
                        break #loop55


                # AST Rewrite
                # elements: ID
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 335:3: -> ^( MODULE ( ID )+ )
                    # Expr.g:335:6: ^( MODULE ( ID )+ )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(MODULE, "MODULE")
                    , root_1)

                    # Expr.g:335:15: ( ID )+
                    if not (stream_ID.hasNext()):
                        raise RewriteEarlyExitException()

                    while stream_ID.hasNext():
                        self._adaptor.addChild(root_1, 
                        stream_ID.nextNode()
                        )


                    stream_ID.reset()

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "module"


    class array_decl_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.array_decl_return, self).__init__()

            self.tree = None





    # $ANTLR start "array_decl"
    # Expr.g:339:1: array_decl : '[' ( expr_list )? ']' -> ^( ARRAY ( expr_list )? ) ;
    def array_decl(self, ):
        retval = self.array_decl_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal246 = None
        char_literal248 = None
        expr_list247 = None

        char_literal246_tree = None
        char_literal248_tree = None
        stream_101 = RewriteRuleTokenStream(self._adaptor, "token 101")
        stream_100 = RewriteRuleTokenStream(self._adaptor, "token 100")
        stream_expr_list = RewriteRuleSubtreeStream(self._adaptor, "rule expr_list")
        try:
            try:
                # Expr.g:340:2: ( '[' ( expr_list )? ']' -> ^( ARRAY ( expr_list )? ) )
                # Expr.g:340:4: '[' ( expr_list )? ']'
                pass 
                char_literal246 = self.match(self.input, 100, self.FOLLOW_100_in_array_decl2118) 
                if self._state.backtracking == 0:
                    stream_100.add(char_literal246)


                # Expr.g:340:8: ( expr_list )?
                alt56 = 2
                LA56_0 = self.input.LA(1)

                if (LA56_0 == BOOL or LA56_0 == FLOAT or LA56_0 == ID or LA56_0 == INT or LA56_0 == NULL or LA56_0 == STRING or LA56_0 == 68 or LA56_0 == 75 or LA56_0 == 83 or LA56_0 == 100 or LA56_0 == 121 or LA56_0 == 126 or LA56_0 == 132) :
                    alt56 = 1
                if alt56 == 1:
                    # Expr.g:340:8: expr_list
                    pass 
                    self._state.following.append(self.FOLLOW_expr_list_in_array_decl2120)
                    expr_list247 = self.expr_list()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_expr_list.add(expr_list247.tree)





                char_literal248 = self.match(self.input, 101, self.FOLLOW_101_in_array_decl2123) 
                if self._state.backtracking == 0:
                    stream_101.add(char_literal248)


                # AST Rewrite
                # elements: expr_list
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 341:3: -> ^( ARRAY ( expr_list )? )
                    # Expr.g:341:6: ^( ARRAY ( expr_list )? )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(ARRAY, "ARRAY")
                    , root_1)

                    # Expr.g:341:14: ( expr_list )?
                    if stream_expr_list.hasNext():
                        self._adaptor.addChild(root_1, stream_expr_list.nextTree())


                    stream_expr_list.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "array_decl"


    class object_decl_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.object_decl_return, self).__init__()

            self.tree = None





    # $ANTLR start "object_decl"
    # Expr.g:344:1: object_decl : '{' ( property )? ( ',' property )* ( ',' )? '}' -> ^( OBJECT ( property )* ) ;
    def object_decl(self, ):
        retval = self.object_decl_return()
        retval.start = self.input.LT(1)


        root_0 = None

        char_literal249 = None
        char_literal251 = None
        char_literal253 = None
        char_literal254 = None
        property250 = None
        property252 = None

        char_literal249_tree = None
        char_literal251_tree = None
        char_literal253_tree = None
        char_literal254_tree = None
        stream_132 = RewriteRuleTokenStream(self._adaptor, "token 132")
        stream_136 = RewriteRuleTokenStream(self._adaptor, "token 136")
        stream_82 = RewriteRuleTokenStream(self._adaptor, "token 82")
        stream_property = RewriteRuleSubtreeStream(self._adaptor, "rule property")
        try:
            try:
                # Expr.g:345:2: ( '{' ( property )? ( ',' property )* ( ',' )? '}' -> ^( OBJECT ( property )* ) )
                # Expr.g:345:4: '{' ( property )? ( ',' property )* ( ',' )? '}'
                pass 
                char_literal249 = self.match(self.input, 132, self.FOLLOW_132_in_object_decl2145) 
                if self._state.backtracking == 0:
                    stream_132.add(char_literal249)


                # Expr.g:345:8: ( property )?
                alt57 = 2
                LA57_0 = self.input.LA(1)

                if (LA57_0 == ID or LA57_0 == INT or LA57_0 == STRING) :
                    alt57 = 1
                if alt57 == 1:
                    # Expr.g:345:8: property
                    pass 
                    self._state.following.append(self.FOLLOW_property_in_object_decl2147)
                    property250 = self.property()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_property.add(property250.tree)





                # Expr.g:345:18: ( ',' property )*
                while True: #loop58
                    alt58 = 2
                    LA58_0 = self.input.LA(1)

                    if (LA58_0 == 82) :
                        LA58_1 = self.input.LA(2)

                        if (LA58_1 == ID or LA58_1 == INT or LA58_1 == STRING) :
                            alt58 = 1




                    if alt58 == 1:
                        # Expr.g:345:19: ',' property
                        pass 
                        char_literal251 = self.match(self.input, 82, self.FOLLOW_82_in_object_decl2151) 
                        if self._state.backtracking == 0:
                            stream_82.add(char_literal251)


                        self._state.following.append(self.FOLLOW_property_in_object_decl2153)
                        property252 = self.property()

                        self._state.following.pop()
                        if self._state.backtracking == 0:
                            stream_property.add(property252.tree)



                    else:
                        break #loop58


                # Expr.g:345:34: ( ',' )?
                alt59 = 2
                LA59_0 = self.input.LA(1)

                if (LA59_0 == 82) :
                    alt59 = 1
                if alt59 == 1:
                    # Expr.g:345:34: ','
                    pass 
                    char_literal253 = self.match(self.input, 82, self.FOLLOW_82_in_object_decl2157) 
                    if self._state.backtracking == 0:
                        stream_82.add(char_literal253)





                char_literal254 = self.match(self.input, 136, self.FOLLOW_136_in_object_decl2160) 
                if self._state.backtracking == 0:
                    stream_136.add(char_literal254)


                # AST Rewrite
                # elements: property
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 346:3: -> ^( OBJECT ( property )* )
                    # Expr.g:346:6: ^( OBJECT ( property )* )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(OBJECT, "OBJECT")
                    , root_1)

                    # Expr.g:346:15: ( property )*
                    while stream_property.hasNext():
                        self._adaptor.addChild(root_1, stream_property.nextTree())


                    stream_property.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "object_decl"


    class property_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.property_return, self).__init__()

            self.tree = None





    # $ANTLR start "property"
    # Expr.g:348:1: property : ( ID | STRING | INT ) ':' expr ;
    def property(self, ):
        retval = self.property_return()
        retval.start = self.input.LT(1)


        root_0 = None

        set255 = None
        char_literal256 = None
        expr257 = None

        set255_tree = None
        char_literal256_tree = None

        try:
            try:
                # Expr.g:349:2: ( ( ID | STRING | INT ) ':' expr )
                # Expr.g:349:4: ( ID | STRING | INT ) ':' expr
                pass 
                root_0 = self._adaptor.nil()


                set255 = self.input.LT(1)

                if self.input.LA(1) == ID or self.input.LA(1) == INT or self.input.LA(1) == STRING:
                    self.input.consume()
                    if self._state.backtracking == 0:
                        self._adaptor.addChild(root_0, self._adaptor.createWithPayload(set255))

                    self._state.errorRecovery = False


                else:
                    if self._state.backtracking > 0:
                        raise BacktrackingFailed


                    mse = MismatchedSetException(None, self.input)
                    raise mse



                char_literal256 = self.match(self.input, 91, self.FOLLOW_91_in_property2193)
                if self._state.backtracking == 0:
                    char_literal256_tree = self._adaptor.createWithPayload(char_literal256)
                    self._adaptor.addChild(root_0, char_literal256_tree)



                self._state.following.append(self.FOLLOW_expr_in_property2195)
                expr257 = self.expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    self._adaptor.addChild(root_0, expr257.tree)




                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "property"


    class sprintf_return(ParserRuleReturnScope):
        def __init__(self):
            super(ExprParser.sprintf_return, self).__init__()

            self.tree = None





    # $ANTLR start "sprintf"
    # Expr.g:353:1: sprintf : 'sprintf' '(' expr ( ',' expr_list )? ')' -> ^( SPRINTF expr ( expr_list )? ) ;
    def sprintf(self, ):
        retval = self.sprintf_return()
        retval.start = self.input.LT(1)


        root_0 = None

        string_literal258 = None
        char_literal259 = None
        char_literal261 = None
        char_literal263 = None
        expr260 = None
        expr_list262 = None

        string_literal258_tree = None
        char_literal259_tree = None
        char_literal261_tree = None
        char_literal263_tree = None
        stream_126 = RewriteRuleTokenStream(self._adaptor, "token 126")
        stream_82 = RewriteRuleTokenStream(self._adaptor, "token 82")
        stream_75 = RewriteRuleTokenStream(self._adaptor, "token 75")
        stream_76 = RewriteRuleTokenStream(self._adaptor, "token 76")
        stream_expr = RewriteRuleSubtreeStream(self._adaptor, "rule expr")
        stream_expr_list = RewriteRuleSubtreeStream(self._adaptor, "rule expr_list")
        try:
            try:
                # Expr.g:354:2: ( 'sprintf' '(' expr ( ',' expr_list )? ')' -> ^( SPRINTF expr ( expr_list )? ) )
                # Expr.g:354:4: 'sprintf' '(' expr ( ',' expr_list )? ')'
                pass 
                string_literal258 = self.match(self.input, 126, self.FOLLOW_126_in_sprintf2207) 
                if self._state.backtracking == 0:
                    stream_126.add(string_literal258)


                char_literal259 = self.match(self.input, 75, self.FOLLOW_75_in_sprintf2209) 
                if self._state.backtracking == 0:
                    stream_75.add(char_literal259)


                self._state.following.append(self.FOLLOW_expr_in_sprintf2211)
                expr260 = self.expr()

                self._state.following.pop()
                if self._state.backtracking == 0:
                    stream_expr.add(expr260.tree)


                # Expr.g:354:23: ( ',' expr_list )?
                alt60 = 2
                LA60_0 = self.input.LA(1)

                if (LA60_0 == 82) :
                    alt60 = 1
                if alt60 == 1:
                    # Expr.g:354:24: ',' expr_list
                    pass 
                    char_literal261 = self.match(self.input, 82, self.FOLLOW_82_in_sprintf2214) 
                    if self._state.backtracking == 0:
                        stream_82.add(char_literal261)


                    self._state.following.append(self.FOLLOW_expr_list_in_sprintf2216)
                    expr_list262 = self.expr_list()

                    self._state.following.pop()
                    if self._state.backtracking == 0:
                        stream_expr_list.add(expr_list262.tree)





                char_literal263 = self.match(self.input, 76, self.FOLLOW_76_in_sprintf2220) 
                if self._state.backtracking == 0:
                    stream_76.add(char_literal263)


                # AST Rewrite
                # elements: expr_list, expr
                # token labels: 
                # rule labels: retval
                # token list labels: 
                # rule list labels: 
                # wildcard labels: 
                if self._state.backtracking == 0:
                    retval.tree = root_0
                    if retval is not None:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "rule retval", retval.tree)
                    else:
                        stream_retval = RewriteRuleSubtreeStream(self._adaptor, "token retval", None)


                    root_0 = self._adaptor.nil()
                    # 355:3: -> ^( SPRINTF expr ( expr_list )? )
                    # Expr.g:355:6: ^( SPRINTF expr ( expr_list )? )
                    root_1 = self._adaptor.nil()
                    root_1 = self._adaptor.becomeRoot(
                    self._adaptor.createFromType(SPRINTF, "SPRINTF")
                    , root_1)

                    self._adaptor.addChild(root_1, stream_expr.nextTree())

                    # Expr.g:355:21: ( expr_list )?
                    if stream_expr_list.hasNext():
                        self._adaptor.addChild(root_1, stream_expr_list.nextTree())


                    stream_expr_list.reset();

                    self._adaptor.addChild(root_0, root_1)




                    retval.tree = root_0





                retval.stop = self.input.LT(-1)


                if self._state.backtracking == 0:
                    retval.tree = self._adaptor.rulePostProcessing(root_0)
                    self._adaptor.setTokenBoundaries(retval.tree, retval.start, retval.stop)



            except RecognitionException, re:
                self.reportError(re)
                self.recover(self.input, re)
                retval.tree = self._adaptor.errorNode(self.input, retval.start, self.input.LT(-1), re)

        finally:
            pass
        return retval

    # $ANTLR end "sprintf"

    # $ANTLR start "synpred1_Expr"
    def synpred1_Expr_fragment(self, ):
        # Expr.g:232:4: ( '[' expr ']' )
        # Expr.g:232:4: '[' expr ']'
        pass 
        root_0 = self._adaptor.nil()


        self.match(self.input, 100, self.FOLLOW_100_in_synpred1_Expr1439)


        self._state.following.append(self.FOLLOW_expr_in_synpred1_Expr1441)
        self.expr()

        self._state.following.pop()


        self.match(self.input, 101, self.FOLLOW_101_in_synpred1_Expr1443)




    # $ANTLR end "synpred1_Expr"




    def synpred1_Expr(self):
        self._state.backtracking += 1
        start = self.input.mark()
        try:
            self.synpred1_Expr_fragment()
        except BacktrackingFailed:
            success = False
        else:
            success = True
        self.input.rewind(start)
        self._state.backtracking -= 1
        return success



    # lookup tables for DFA #6

    DFA6_eot = DFA.unpack(
        u"\6\uffff"
        )

    DFA6_eof = DFA.unpack(
        u"\6\uffff"
        )

    DFA6_min = DFA.unpack(
        u"\1\42\1\122\1\42\2\uffff\1\122"
        )

    DFA6_max = DFA.unpack(
        u"\1\42\1\134\1\42\2\uffff\1\134"
        )

    DFA6_accept = DFA.unpack(
        u"\3\uffff\1\1\1\2\1\uffff"
        )

    DFA6_special = DFA.unpack(
        u"\6\uffff"
        )


    DFA6_transition = [
        DFA.unpack(u"\1\1"),
        DFA.unpack(u"\1\3\3\uffff\1\2\1\4\4\uffff\1\3"),
        DFA.unpack(u"\1\5"),
        DFA.unpack(u""),
        DFA.unpack(u""),
        DFA.unpack(u"\1\3\3\uffff\1\2\1\4\4\uffff\1\3")
    ]

    # class definition for DFA #6

    class DFA6(DFA):
        pass


 

    FOLLOW_EOF_in_prog211 = frozenset([1])
    FOLLOW_stmt_in_prog220 = frozenset([1, 34, 80, 84, 92, 105, 108, 109, 111, 115, 116, 117, 118, 119, 122, 123, 125, 128, 129, 130, 131])
    FOLLOW_92_in_stmt232 = frozenset([1])
    FOLLOW_exec_stmt_in_stmt239 = frozenset([1])
    FOLLOW_import_stmt_in_stmt244 = frozenset([1])
    FOLLOW_print_stmt_in_stmt249 = frozenset([1])
    FOLLOW_printf_stmt_in_stmt253 = frozenset([1])
    FOLLOW_break_stmt_in_stmt258 = frozenset([1])
    FOLLOW_continue_stmt_in_stmt263 = frozenset([1])
    FOLLOW_return_stmt_in_stmt268 = frozenset([1])
    FOLLOW_if_stmt_in_stmt273 = frozenset([1])
    FOLLOW_while_stmt_in_stmt278 = frozenset([1])
    FOLLOW_do_while_stmt_in_stmt283 = frozenset([1])
    FOLLOW_switch_stmt_in_stmt288 = frozenset([1])
    FOLLOW_for_stmt_in_stmt293 = frozenset([1])
    FOLLOW_foreach_stmt_in_stmt298 = frozenset([1])
    FOLLOW_throw_stmt_in_stmt303 = frozenset([1])
    FOLLOW_try_stmt_in_stmt308 = frozenset([1])
    FOLLOW_func_decl_in_stmt313 = frozenset([1])
    FOLLOW_class_decl_in_stmt318 = frozenset([1])
    FOLLOW_132_in_block331 = frozenset([34, 80, 84, 92, 105, 108, 109, 111, 115, 116, 117, 118, 119, 122, 123, 125, 128, 129, 130, 131, 136])
    FOLLOW_stmt_in_block333 = frozenset([34, 80, 84, 92, 105, 108, 109, 111, 115, 116, 117, 118, 119, 122, 123, 125, 128, 129, 130, 131, 136])
    FOLLOW_136_in_block336 = frozenset([1])
    FOLLOW_119_in_import_stmt358 = frozenset([34])
    FOLLOW_module_path_in_import_stmt360 = frozenset([82, 92])
    FOLLOW_82_in_import_stmt363 = frozenset([34])
    FOLLOW_module_path_in_import_stmt365 = frozenset([82, 92])
    FOLLOW_92_in_import_stmt369 = frozenset([1])
    FOLLOW_module_in_module_path390 = frozenset([1])
    FOLLOW_module_in_module_path395 = frozenset([87])
    FOLLOW_87_in_module_path397 = frozenset([1])
    FOLLOW_123_in_printf_stmt408 = frozenset([75])
    FOLLOW_75_in_printf_stmt410 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_expr_in_printf_stmt412 = frozenset([76, 82])
    FOLLOW_82_in_printf_stmt415 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_expr_list_in_printf_stmt417 = frozenset([76])
    FOLLOW_76_in_printf_stmt421 = frozenset([92])
    FOLLOW_92_in_printf_stmt423 = frozenset([1])
    FOLLOW_122_in_print_stmt452 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_expr_list_in_print_stmt455 = frozenset([92])
    FOLLOW_92_in_print_stmt457 = frozenset([1])
    FOLLOW_105_in_break_stmt478 = frozenset([92])
    FOLLOW_92_in_break_stmt480 = frozenset([1])
    FOLLOW_109_in_continue_stmt496 = frozenset([92])
    FOLLOW_92_in_continue_stmt498 = frozenset([1])
    FOLLOW_125_in_return_stmt514 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 92, 100, 121, 126, 132])
    FOLLOW_expr_in_return_stmt516 = frozenset([92])
    FOLLOW_92_in_return_stmt519 = frozenset([1])
    FOLLOW_if_clause_in_if_stmt541 = frozenset([1, 112])
    FOLLOW_else_if_clause_in_if_stmt543 = frozenset([1, 112])
    FOLLOW_else_clause_in_if_stmt546 = frozenset([1])
    FOLLOW_118_in_if_clause557 = frozenset([75])
    FOLLOW_75_in_if_clause559 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_expr_in_if_clause561 = frozenset([76])
    FOLLOW_76_in_if_clause563 = frozenset([132])
    FOLLOW_block_in_if_clause565 = frozenset([1])
    FOLLOW_112_in_else_if_clause587 = frozenset([118])
    FOLLOW_if_clause_in_else_if_clause589 = frozenset([1])
    FOLLOW_112_in_else_clause609 = frozenset([132])
    FOLLOW_block_in_else_clause611 = frozenset([1])
    FOLLOW_131_in_while_stmt632 = frozenset([75])
    FOLLOW_75_in_while_stmt634 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_expr_in_while_stmt636 = frozenset([76])
    FOLLOW_76_in_while_stmt638 = frozenset([132])
    FOLLOW_block_in_while_stmt640 = frozenset([1])
    FOLLOW_111_in_do_while_stmt663 = frozenset([132])
    FOLLOW_block_in_do_while_stmt665 = frozenset([131])
    FOLLOW_131_in_do_while_stmt667 = frozenset([75])
    FOLLOW_75_in_do_while_stmt669 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_expr_in_do_while_stmt671 = frozenset([76])
    FOLLOW_76_in_do_while_stmt673 = frozenset([92])
    FOLLOW_92_in_do_while_stmt675 = frozenset([1])
    FOLLOW_128_in_switch_stmt698 = frozenset([75])
    FOLLOW_75_in_switch_stmt700 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_expr_in_switch_stmt702 = frozenset([76])
    FOLLOW_76_in_switch_stmt704 = frozenset([132])
    FOLLOW_case_block_in_switch_stmt706 = frozenset([1])
    FOLLOW_132_in_case_block728 = frozenset([106])
    FOLLOW_case_clause_in_case_block731 = frozenset([106, 110, 136])
    FOLLOW_default_clause_in_case_block736 = frozenset([136])
    FOLLOW_136_in_case_block740 = frozenset([1])
    FOLLOW_case_test_in_case_clause750 = frozenset([34, 80, 84, 92, 105, 106, 108, 109, 111, 115, 116, 117, 118, 119, 122, 123, 125, 128, 129, 130, 131])
    FOLLOW_stmt_in_case_clause753 = frozenset([34, 80, 84, 92, 105, 108, 109, 111, 115, 116, 117, 118, 119, 122, 123, 125, 128, 129, 130, 131])
    FOLLOW_break_stmt_in_case_clause756 = frozenset([1])
    FOLLOW_106_in_case_test782 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_expr_in_case_test784 = frozenset([91])
    FOLLOW_91_in_case_test786 = frozenset([1])
    FOLLOW_110_in_default_clause806 = frozenset([91])
    FOLLOW_91_in_default_clause808 = frozenset([1, 34, 80, 84, 92, 105, 108, 109, 111, 115, 116, 117, 118, 119, 122, 123, 125, 128, 129, 130, 131])
    FOLLOW_stmt_in_default_clause810 = frozenset([1, 34, 80, 84, 92, 105, 108, 109, 111, 115, 116, 117, 118, 119, 122, 123, 125, 128, 129, 130, 131])
    FOLLOW_115_in_for_stmt833 = frozenset([75])
    FOLLOW_75_in_for_stmt835 = frozenset([34, 80, 84, 92])
    FOLLOW_exec_list_in_for_stmt839 = frozenset([92])
    FOLLOW_92_in_for_stmt842 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_expr_in_for_stmt844 = frozenset([92])
    FOLLOW_92_in_for_stmt846 = frozenset([34, 76, 80, 84])
    FOLLOW_exec_list_in_for_stmt850 = frozenset([76])
    FOLLOW_76_in_for_stmt853 = frozenset([132])
    FOLLOW_block_in_for_stmt855 = frozenset([1])
    FOLLOW_116_in_foreach_stmt886 = frozenset([75])
    FOLLOW_75_in_foreach_stmt888 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_expr_in_foreach_stmt890 = frozenset([104])
    FOLLOW_104_in_foreach_stmt892 = frozenset([34])
    FOLLOW_each_in_foreach_stmt894 = frozenset([76])
    FOLLOW_76_in_foreach_stmt896 = frozenset([132])
    FOLLOW_block_in_foreach_stmt898 = frozenset([1])
    FOLLOW_each_val_in_each922 = frozenset([1])
    FOLLOW_ID_in_each937 = frozenset([97])
    FOLLOW_97_in_each939 = frozenset([34])
    FOLLOW_each_val_in_each941 = frozenset([1])
    FOLLOW_ID_in_each_val963 = frozenset([1, 82])
    FOLLOW_82_in_each_val966 = frozenset([34])
    FOLLOW_ID_in_each_val968 = frozenset([1, 82])
    FOLLOW_129_in_throw_stmt993 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_expr_in_throw_stmt995 = frozenset([92])
    FOLLOW_92_in_throw_stmt997 = frozenset([1])
    FOLLOW_130_in_try_stmt1017 = frozenset([132])
    FOLLOW_block_in_try_stmt1019 = frozenset([107])
    FOLLOW_catch_clause_in_try_stmt1021 = frozenset([1, 107, 114])
    FOLLOW_finally_clause_in_try_stmt1024 = frozenset([1])
    FOLLOW_107_in_catch_clause1051 = frozenset([75])
    FOLLOW_75_in_catch_clause1053 = frozenset([34])
    FOLLOW_module_in_catch_clause1055 = frozenset([34, 76])
    FOLLOW_ID_in_catch_clause1057 = frozenset([76])
    FOLLOW_76_in_catch_clause1060 = frozenset([132])
    FOLLOW_block_in_catch_clause1062 = frozenset([1])
    FOLLOW_114_in_finally_clause1087 = frozenset([132])
    FOLLOW_block_in_finally_clause1089 = frozenset([1])
    FOLLOW_117_in_func_decl1111 = frozenset([34])
    FOLLOW_ID_in_func_decl1113 = frozenset([75])
    FOLLOW_params_in_func_decl1115 = frozenset([132])
    FOLLOW_block_in_func_decl1117 = frozenset([1])
    FOLLOW_75_in_params1141 = frozenset([34, 76, 82])
    FOLLOW_param_decl_in_params1143 = frozenset([76, 82])
    FOLLOW_82_in_params1147 = frozenset([34])
    FOLLOW_param_decl_in_params1149 = frozenset([76, 82])
    FOLLOW_76_in_params1153 = frozenset([1])
    FOLLOW_ID_in_param_decl1174 = frozenset([1, 95])
    FOLLOW_95_in_param_decl1177 = frozenset([8, 30, 34, 39, 47, 61, 75, 100, 121, 126, 132])
    FOLLOW_atom_in_param_decl1179 = frozenset([1])
    FOLLOW_108_in_class_decl1192 = frozenset([34])
    FOLLOW_ID_in_class_decl1194 = frozenset([113, 132])
    FOLLOW_113_in_class_decl1197 = frozenset([34])
    FOLLOW_ID_in_class_decl1199 = frozenset([132])
    FOLLOW_132_in_class_decl1205 = frozenset([117, 124, 136])
    FOLLOW_class_element_in_class_decl1207 = frozenset([117, 124, 136])
    FOLLOW_136_in_class_decl1210 = frozenset([1])
    FOLLOW_var_def_in_class_element1236 = frozenset([1])
    FOLLOW_constructor_in_class_element1240 = frozenset([1])
    FOLLOW_func_decl_in_class_element1244 = frozenset([1])
    FOLLOW_124_in_var_def1254 = frozenset([34])
    FOLLOW_ID_in_var_def1256 = frozenset([92, 95])
    FOLLOW_95_in_var_def1259 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_expr_in_var_def1261 = frozenset([92])
    FOLLOW_92_in_var_def1265 = frozenset([1])
    FOLLOW_124_in_var_def1283 = frozenset([127])
    FOLLOW_127_in_var_def1285 = frozenset([34])
    FOLLOW_ID_in_var_def1287 = frozenset([92, 95])
    FOLLOW_95_in_var_def1290 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_expr_in_var_def1292 = frozenset([92])
    FOLLOW_92_in_var_def1296 = frozenset([1])
    FOLLOW_117_in_constructor1321 = frozenset([120])
    FOLLOW_120_in_constructor1323 = frozenset([75])
    FOLLOW_params_in_constructor1325 = frozenset([132])
    FOLLOW_block_in_constructor1327 = frozenset([1])
    FOLLOW_primary_in_member_expr1354 = frozenset([1, 86])
    FOLLOW_86_in_member_expr1357 = frozenset([34])
    FOLLOW_primary_in_member_expr1359 = frozenset([1, 86])
    FOLLOW_ID_in_primary1382 = frozenset([1, 75, 100])
    FOLLOW_index_expr_in_primary1384 = frozenset([1, 75, 100])
    FOLLOW_call_expr_in_primary1387 = frozenset([1])
    FOLLOW_75_in_call_expr1398 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 76, 83, 100, 121, 126, 132])
    FOLLOW_expr_list_in_call_expr1400 = frozenset([76])
    FOLLOW_76_in_call_expr1403 = frozenset([1])
    FOLLOW_100_in_index_expr1439 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_expr_in_index_expr1441 = frozenset([101])
    FOLLOW_101_in_index_expr1443 = frozenset([1])
    FOLLOW_100_in_index_expr1458 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_expr_in_index_expr1460 = frozenset([88])
    FOLLOW_88_in_index_expr1462 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 101, 121, 126, 132])
    FOLLOW_expr_in_index_expr1464 = frozenset([101])
    FOLLOW_101_in_index_expr1467 = frozenset([1])
    FOLLOW_exec_expr_in_exec_list1492 = frozenset([1, 82])
    FOLLOW_82_in_exec_list1495 = frozenset([34, 80, 84])
    FOLLOW_exec_expr_in_exec_list1497 = frozenset([1, 82])
    FOLLOW_member_expr_in_member_list1520 = frozenset([1, 82])
    FOLLOW_82_in_member_list1523 = frozenset([34])
    FOLLOW_member_expr_in_member_list1525 = frozenset([1, 82])
    FOLLOW_member_expr_in_exec_expr1537 = frozenset([1, 71, 74, 78, 80, 81, 84, 85, 90, 95, 103, 134])
    FOLLOW_assign_op_in_exec_expr1542 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_expr_in_exec_expr1544 = frozenset([1])
    FOLLOW_80_in_exec_expr1565 = frozenset([1])
    FOLLOW_84_in_exec_expr1582 = frozenset([1])
    FOLLOW_80_in_exec_expr1613 = frozenset([34])
    FOLLOW_member_expr_in_exec_expr1615 = frozenset([1])
    FOLLOW_84_in_exec_expr1630 = frozenset([34])
    FOLLOW_member_expr_in_exec_expr1632 = frozenset([1])
    FOLLOW_exec_list_in_exec_stmt1678 = frozenset([92])
    FOLLOW_92_in_exec_stmt1680 = frozenset([1])
    FOLLOW_expr_in_expr_list1703 = frozenset([1, 82])
    FOLLOW_82_in_expr_list1706 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_expr_in_expr_list1708 = frozenset([1, 82])
    FOLLOW_82_in_expr_list1712 = frozenset([1])
    FOLLOW_logic_or_expr_in_expr1734 = frozenset([1])
    FOLLOW_logic_and_expr_in_logic_or_expr1744 = frozenset([1, 135])
    FOLLOW_135_in_logic_or_expr1747 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_logic_and_expr_in_logic_or_expr1750 = frozenset([1, 135])
    FOLLOW_bitwise_or_expr_in_logic_and_expr1762 = frozenset([1, 72])
    FOLLOW_72_in_logic_and_expr1765 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_bitwise_or_expr_in_logic_and_expr1768 = frozenset([1, 72])
    FOLLOW_bitwise_xor_expr_in_bitwise_or_expr1780 = frozenset([1, 133])
    FOLLOW_133_in_bitwise_or_expr1783 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_bitwise_xor_expr_in_bitwise_or_expr1786 = frozenset([1, 133])
    FOLLOW_bitwise_and_expr_in_bitwise_xor_expr1798 = frozenset([1, 102])
    FOLLOW_102_in_bitwise_xor_expr1801 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_bitwise_and_expr_in_bitwise_xor_expr1804 = frozenset([1, 102])
    FOLLOW_relation_expr_in_bitwise_and_expr1816 = frozenset([1, 73])
    FOLLOW_73_in_bitwise_and_expr1819 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_relation_expr_in_bitwise_and_expr1822 = frozenset([1, 73])
    FOLLOW_add_expr_in_relation_expr1834 = frozenset([1, 69, 93, 94, 96, 98, 99])
    FOLLOW_set_in_relation_expr1837 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_add_expr_in_relation_expr1852 = frozenset([1])
    FOLLOW_mul_expr_in_add_expr1864 = frozenset([1, 79, 83])
    FOLLOW_set_in_add_expr1867 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_mul_expr_in_add_expr1874 = frozenset([1, 79, 83])
    FOLLOW_not_expr_in_mul_expr1886 = frozenset([1, 70, 77, 89])
    FOLLOW_set_in_mul_expr1889 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_not_expr_in_mul_expr1898 = frozenset([1, 70, 77, 89])
    FOLLOW_68_in_not_expr1912 = frozenset([8, 30, 34, 39, 47, 61, 75, 83, 100, 121, 126, 132])
    FOLLOW_negative_expr_in_not_expr1915 = frozenset([1])
    FOLLOW_83_in_negative_expr1950 = frozenset([8, 30, 34, 39, 47, 61, 75, 100, 121, 126, 132])
    FOLLOW_atom_in_negative_expr1954 = frozenset([1])
    FOLLOW_literal_in_atom1987 = frozenset([1])
    FOLLOW_member_expr_in_atom1992 = frozenset([1])
    FOLLOW_array_decl_in_atom1997 = frozenset([1])
    FOLLOW_object_decl_in_atom2002 = frozenset([1])
    FOLLOW_new_clause_in_atom2007 = frozenset([1])
    FOLLOW_sprintf_in_atom2012 = frozenset([1])
    FOLLOW_75_in_atom2017 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_expr_in_atom2019 = frozenset([76])
    FOLLOW_76_in_atom2021 = frozenset([1])
    FOLLOW_121_in_new_clause2062 = frozenset([34])
    FOLLOW_module_in_new_clause2064 = frozenset([75])
    FOLLOW_call_expr_in_new_clause2066 = frozenset([1])
    FOLLOW_ID_in_module2088 = frozenset([1, 86])
    FOLLOW_86_in_module2091 = frozenset([34])
    FOLLOW_ID_in_module2093 = frozenset([1, 86])
    FOLLOW_100_in_array_decl2118 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 101, 121, 126, 132])
    FOLLOW_expr_list_in_array_decl2120 = frozenset([101])
    FOLLOW_101_in_array_decl2123 = frozenset([1])
    FOLLOW_132_in_object_decl2145 = frozenset([34, 39, 61, 82, 136])
    FOLLOW_property_in_object_decl2147 = frozenset([82, 136])
    FOLLOW_82_in_object_decl2151 = frozenset([34, 39, 61])
    FOLLOW_property_in_object_decl2153 = frozenset([82, 136])
    FOLLOW_82_in_object_decl2157 = frozenset([136])
    FOLLOW_136_in_object_decl2160 = frozenset([1])
    FOLLOW_set_in_property2181 = frozenset([91])
    FOLLOW_91_in_property2193 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_expr_in_property2195 = frozenset([1])
    FOLLOW_126_in_sprintf2207 = frozenset([75])
    FOLLOW_75_in_sprintf2209 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_expr_in_sprintf2211 = frozenset([76, 82])
    FOLLOW_82_in_sprintf2214 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_expr_list_in_sprintf2216 = frozenset([76])
    FOLLOW_76_in_sprintf2220 = frozenset([1])
    FOLLOW_100_in_synpred1_Expr1439 = frozenset([8, 30, 34, 39, 47, 61, 68, 75, 83, 100, 121, 126, 132])
    FOLLOW_expr_in_synpred1_Expr1441 = frozenset([101])
    FOLLOW_101_in_synpred1_Expr1443 = frozenset([1])



def main(argv, stdin=sys.stdin, stdout=sys.stdout, stderr=sys.stderr):
    from antlr3.main import ParserMain
    main = ParserMain("ExprLexer", ExprParser)

    main.stdin = stdin
    main.stdout = stdout
    main.stderr = stderr
    main.execute(argv)



if __name__ == '__main__':
    main(sys.argv)
