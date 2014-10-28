/*------------------------------ plp2.l -------------------------------*/

D	[0-9]
L	[a-zA-Z]
LD	[0-9a-zA-Z]

%{

#include <string.h>
#include <vector>
#include <iostream>

typedef struct{
std::string lexema;
std::string cod;
std::string trad;

int dir;
int dbase;

int nlin;
int ncol;

int tipo;	//Nombra un tipo en base a la numeracion de la tabla de tipos
int tipoOp;	//Se usa para controlar que instruccion usar en caso de diferencia entre entero y real (xxxi/xxxr). Nombra el tipo entero o real, no el resto

}Atributos;

#define YYSTYPE Atributos
#define ERRLEXICO	1
#define ERRSINT		2
#define ERREOF		3

#include "plp2.tab.h"

int ncol = 1,
nlin = 1;

int findefichero = 0;

int ret(int token);						// funcion que actualiza nlin y ncol y devuelve el token
void msgError(int nerror, int nlin,int ncol,const char *s);	// funcion para producir mensajes de error
%}

%%

%{
	/* codigo local */
%}

" "		{ncol++;}
[\t]		{ncol++;}
[\n]		{nlin++;ncol=1;}
"//"(.)*	{; /* comentarios, no hacer nada */ }
"algoritmo"	{return ret(algoritmo);}
"falgoritmo"	{return ret(falgoritmo);}
"var"		{return ret(var);}
"fvar"		{return ret(fvar);}
"entero"	{return ret(entero);}
"real"		{return ret(real);}
"logico"	{return ret(logico);}
"tabla"		{return ret(tabla);}
"de"		{return ret(de);}
"escribe"	{return ret(escribe);}
"lee"		{return ret(lee);}
"si"		{return ret(si);}
"entonces"	{return ret(entonces);}
"sino"		{return ret(sino);}
"mientras"	{return ret(mientras);}
"hacer"		{return ret(hacer);}
"blq"		{return ret(blq);}
"fblq"		{return ret(fblq);}
"cierto"	{return ret(cierto);}
"falso"		{return ret(falso);}				 /* Las palabras reservadas deben aparecer antes de la regla que reconoce los identificadores, para evitar que sean reconocidas como identificadores en lugar de como palabras reservadas */
{L}({LD})*	{return ret(id);}
{D}+		{return ret(numentero);}
{D}+(\.){D}+	{return ret(numreal);}
"+"		{return ret(opas);}
"-"		{return ret(opas);}
"*"		{return ret(opmd);}
"/"		{return ret(opmd);}
"="		{return ret(oprel);}
"<>"		{return ret(oprel);}
"<"		{return ret(oprel);}
">"		{return ret(oprel);}
">="		{return ret(oprel);}
"<="		{return ret(oprel);}
"||"	{return ret(obool);}
"&&"	{return ret(ybool);}
"!"		{return ret(nobool);}
"("		{return ret(pari);}
")"		{return ret(pard);}
";"		{return ret(pyc);}
","		{return ret(coma);}
":="	{return ret(opasig);}
":"		{return ret(dospto);}
"["		{return ret(lcor);}
"]"		{return ret(rcor);}

.		{msgError(ERRLEXICO,nlin,ncol,yytext);}

%%

int yywrap(void){findefichero=1; return 1;}			 /* para no tener que linkar con la libreria del lex */

int ret(int token){
	yylval.lexema=strdup(yytext);

	//Anyadimos las traducciones de las operaciones artimético-lógicas para gestionar más rápidamente sus traducciones en el YACC
	if(yylval.lexema == "+")
		yylval.trad = "add";
	else if(yylval.lexema == "-")
		yylval.trad = "sub";
	else if(yylval.lexema == "*")
		yylval.trad = "mul";
	else if(yylval.lexema == "/")
		yylval.trad = "div";
	else if(yylval.lexema == "&&")
		yylval.trad = "and";
	else if(yylval.lexema == "||")
		yylval.trad = "or";
	else if(yylval.lexema == "=")
		yylval.trad = "eql";
	else if(yylval.lexema == "<>")
		yylval.trad = "neq";
	else if(yylval.lexema == ">")
		yylval.trad = "gtr";
	else if(yylval.lexema == ">=")
		yylval.trad = "geq";
	else if(yylval.lexema == "<")
		yylval.trad = "lss";
	else if(yylval.lexema == "<=")
		yylval.trad = "leq";
	else if(yylval.lexema == "!")
		yylval.trad = "not";

	yylval.nlin=nlin;
	yylval.ncol=ncol;

	ncol+=(strlen(yytext));

	return(token);
}

