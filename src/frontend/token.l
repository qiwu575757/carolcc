%option noyywrap yylineno
%x COMMENT
%x SINGLELINECOMMENT

%{

#include <string>
#include <iostream>
#include "syntax_tree.h"
#include "parser.h"
extern int yyline;

#define SAVE_TOKEN yylval.string = new std::string(yytext,yyleng)
#define TOKEN(t) (yylval.token=t)
%}


%%

"/*"		{BEGIN(COMMENT);}
<COMMENT>"*/"  {BEGIN(INITIAL);}
<COMMENT>([^*]|\n)+|.
<COMMENT><<EOF>>   {printf("Unterminated comment\n"); return 0;}
"//" {BEGIN(SINGLELINECOMMENT);}
<SINGLELINECOMMENT>.*
<SINGLELINECOMMENT>\n {BEGIN(INITIAL);}
<SINGLELINECOMMENT><<EOF>> {BEGIN(INITIAL);}

[ \t] {}
\n {yyline++;}
[1-9][0-9]*|0[0-7]*|(0x|0X)[0-9a-fA-F]*  {SAVE_TOKEN; return TINTEGER;}

[0-9]+[Ee][0-9]+  {SAVE_TOKEN; return TFLOATNUM;}
[0-9]+[Ee]"+"[0-9]+  {SAVE_TOKEN; return TFLOATNUM;}

[0-9]+[Ee]"-"[0-9]+ {SAVE_TOKEN; return TFLOATNUM;}//here may have problems
[0-9]*"."[0-9]*[Ee][-+][0-9]+ {SAVE_TOKEN; return TFLOATNUM;}
[0-9]*"."[0-9]*  {SAVE_TOKEN; return TFLOATNUM;}
0[Xx][0-9a-fA-F]*"."[0-9a-fA-F]*[Pp][+-][0-9]+ {SAVE_TOKEN; return TFLOATNUM;}

return {return TOKEN(TRETURN);}
const {return TOKEN(TCONST);}
int {return TOKEN(TINT);}
float {return TOKEN(TFLOAT);}
void {return TOKEN(TVOID);}
break {return TOKEN(TBREAK);}
continue {return TOKEN(TCONTINUE);}
if    {return TOKEN(TIF);}
else  {return TOKEN(TELSE);}
while {return TOKEN(TWHILE);}

[a-zA-Z_][a-zA-Z0-9_]* {SAVE_TOKEN; return TIDENTIFIER;}

";"  {return TOKEN(TSEMICOLOM);}
","  {return TOKEN(TCOMMA);}
"("  {return TOKEN(TLPAREN);}
")"  {return TOKEN(TRPAREN);}
"["  {return TOKEN(TLMPAREN);}
"]"  {return TOKEN(TRMPAREN);}
"{"  {return TOKEN(TLBPAREN);}
"}"  {return TOKEN(TRBPAREN);}

"-"  {return TOKEN(TMINUS);}
"!"  {return TOKEN(TNOT);}

"+"  {return TOKEN(TPLUS);}
"/"  {return TOKEN(TDIV);}
"*"  {return TOKEN(TMULTI);}
"%"  {return TOKEN(TMOD);}

"<"  {return TOKEN(TL);}
"<=" {return TOKEN(TLE);}
">"  {return TOKEN(TG);}
">=" {return TOKEN(TGE);}
"!=" {return TOKEN(TNE);}
"==" {return TOKEN(TE);}
"&&" {return TOKEN(TLOGAND);}
"||" {return TOKEN(TLOGOR);}

"=" {return TOKEN(TASSIGN);}

<<EOF>> {yyterminate();}
%%
