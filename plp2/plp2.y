/*------------------------------ plp2.y -------------------------------*/

%token algoritmo falgoritmo var fvar entero real logico tabla de escribe lee si entonces sino mientras hacer blq fblq cierto falso id numentero numreal opas opmd oprel obool ybool nobool pari pard pyc coma opasig dospto lcor rcor

%{

#define ERRLEXICO	1
#define ERRSINT		2
#define ERREOF		3
#define ERRYADECL	4
#define ERRNODECL	5
#define ERRDIM		6
#define ERRFALTAN	7
#define ERRSOBRAN	8
#define ERR_EXP_ENT	9
#define ERR_EXP_LOG	10
#define ERR_EXDER_LOG	11
#define ERR_EXDER_ENT	12
#define ERR_EXDER_RE	13
#define ERR_EXIZQ_LOG	14
#define ERR_EXIZQ_RE	15
#define ERR_NOCABE	16
#define ERR_MAXVAR	17
#define ERR_MAXTIPOS	18
#define ERR_MAXTMP	19

//Definimos estas constantes para los tipos
#define ENTERO		1
#define REAL		2
#define LOGICO		3


#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string.h>
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

//Declaración de clases

//Definición de un tipo y sus elementos: el número del tipo, su tamaño, el tamaño total si forma un array y el tipo base de esa array
class Tipo {
	public:
		int tipo;
		int tam;
		int tamTotal;
		int tipoBase;

	Tipo(int tipo, int tam, int tamTotal, int tipoBase){
		this->tipo = tipo;
		this->tam = tam;
		this->tamTotal = tamTotal;
		this->tipoBase = tipoBase;
	}
};

//Definición de la tabla de tipos formada por un vector del tipo definido arriba y las funciones pedidas en clase de teoría
class TablaTipos{
	private:
		std::vector<Tipo*> tabla;

	public:
		TablaTipos(){
			nuevoTipo(1,1,1,1);
			nuevoTipo(2,1,1,2);
			nuevoTipo(3,1,1,3);
		}

		int nextId(){
			return this->tabla.size() + 1;
		}

		int tamTotal(int tipo){
			for(int i = 0; i < this->tabla.size(); i++){
				if(this->tabla[i]->tipo == tipo)
					return this->tabla[i]->tamTotal;
			}
		}

		void nuevoTipo(int tipo, int tam, int tamTotal, int tipoBase){
			this->tabla.push_back(new Tipo(tipo, tam, tamTotal, tipoBase));
		}
		
		int tam(int tipo){
			for(int i = 0; i < this->tabla.size(); i++){
				if(this->tabla[i]->tipo == tipo)
					return this->tabla[i]->tam;
			}
		}

		int tipoBase(int tipo){
			for(int i = 0; i < this->tabla.size(); i++){
				if(this->tabla[i]->tipo == tipo)
					return this->tabla[i]->tipoBase;
			}
		}
};

//Definición de las variables (símbolo) donde se almacena el tipo, el tipo de la operación (en caso de venir de una), la dirección y el tamaño
class Simbolo{
	public:
	std::string nombre;
	int tipo;
	int tipoOp;
	int dir;
	int tam;

	Simbolo(){}

	Simbolo(std::string nombre, int tipo, int tipoOp, int dir, int tam){
		this->nombre = nombre;
		this->tipo = tipo;
		this->tipoOp = tipoOp;
		this->dir = dir;
		this->tam = tam;
	}
};

//Definición de la tabla de símbolos formada por un vector del tipo definido arriba y las funciones pedidas en clase de teoría
class TablaSimbolos{
	private:
		std::vector<Simbolo*> tabla;
		int lastDir;
	
	public:
		TablaSimbolos(){
			this->lastDir = 100;
		}		
		
		int nextDir(){
			return lastDir;
		}

		void nuevoSimbolo(std::string id, int tipo, int tipoOp, int dir, int tam){
			this->tabla.push_back(new Simbolo(id, tipo, tipoOp, dir, tam));

			lastDir += tam;
		};

		Simbolo* buscarSimbolo(std::string id){
			for(int i = 0; i < tabla.size(); i++){
				if(this->tabla[i]->nombre == id)
					return this->tabla[i];
			}

			return NULL;
		}

		Simbolo* buscarPorDir(int dir){
			for(int i = 0; i < this->tabla.size(); i++){
				if(this->tabla[i]->dir == dir)
					return this->tabla[i];
			}

			return NULL;
		}
};

// Variables y funciones del A. Lexico
extern int ncol,nlin,findefichero;
extern int yylex();
extern char *yytext;
extern FILE *yyin;
int yyerror(std::string s);

//Definiciones de funciones declaradas al final del fichero
int NuevaTemporal();
void getTipoBaseArray(int &tipo);
bool isArray(int tipo);
void errorSemantico(int nerror, Atributos token);
void msgError(int nerror,int nlin,int ncol,const char *s);

//Variable para el conteo de las temporales y las etiquetas
int regTemporal = 10000;
int contadorEtiquetas = 0;
//Variables de reglas
char s[1000];	//Para los sprintf

//Variables auxiliares de las reglas
int i;		//Contador del for
int tipoArray;	//Variable para guardar el tipo base del array utilizando el método recursivo
int dir;	//Variable auxiliar para almacenar una nueva temporal
int diritor;	//Variable auxiliar para almacenar una nueva temporal donde se almacena una conversión itor


Simbolo* simbolo;

//Definición de las tablas
TablaSimbolos tsActual;
TablaTipos ttipos;

%}

%%

//Implementación sintáctica del lenguaje fuente con su implementación semántica
S	: algoritmo dospto id SDec SInstr falgoritmo	{
								$$.cod = $4.cod + $5.cod + "halt\n";
								printf("%s", $$.cod.c_str());
							}
	;

SDec	: Dec	{	$$.cod = $1.cod;	}
	| 	{ 	/*No se hace nada */ 	}
	;

Dec	: var DVar MDVar fvar	{	$$.cod = $2.cod + $3.cod;	}
	;

DVar	: Tipo dospto id	{
					dir = tsActual.nextDir();

					if(!isArray($1.tipo)){
						if($1.tipo == LOGICO){
							sprintf(s, "mov #0 %d;Tipo dospto id\n\n", dir);
							$$.cod = s;
							tsActual.nuevoSimbolo($3.lexema, $1.tipo, ENTERO, dir, ttipos.tamTotal($1.tipo));
						}
						else{
							if($1.tipo == ENTERO)
								sprintf(s, "mov #0 %d;Tipo dospto id\n\n", dir);

							else if($1.tipo == REAL)
								sprintf(s, "mov $0 %d;Tipo dospto id\n\n", dir);

							$$.cod = s;
							tsActual.nuevoSimbolo($3.lexema, $1.tipo, $1.tipo, dir, ttipos.tamTotal($1.tipo));
						}
					}
					else{
						tipoArray = $1.tipo;
						getTipoBaseArray(tipoArray);
						
						if(tipoArray == LOGICO){
							for(i = 0; i < ttipos.tamTotal($1.tipo); i++){
								sprintf(s, "mov #0 %d;Tipo dospto id\n\n", dir+i);
								$$.cod = s;
							}
							tsActual.nuevoSimbolo($3.lexema, $1.tipo, ENTERO, dir, ttipos.tamTotal($1.tipo));
						}
						else{
							if(tipoArray == ENTERO){
								for(i = 0; i < ttipos.tamTotal($1.tipo); i++){
									sprintf(s, "mov #0 %d;Tipo dospto id\n\n", dir+i);
									$$.cod = s;
								}
							}
							else if(tipoArray == REAL){
								for(i = 0; i < ttipos.tamTotal($1.tipo); i++){
									sprintf(s, "mov $0 %d;Tipo dospto id\n\n", dir+i);
									$$.cod = s;
								}
							}
							tsActual.nuevoSimbolo($3.lexema, $1.tipo, $1.tipo, dir, ttipos.tamTotal($1.tipo));
						}				
					}

					dir = tsActual.nextDir();
					if(dir >= 10000)
						errorSemantico(ERR_NOCABE, $3);
				}
	  LId pyc		{
					$$.cod = $4.cod + $5.cod;
					regTemporal = 10000;
				}
	;

MDVar	: DVar MDVar	{	$$.cod = $1.cod + $2.cod;	}
	| 		{ 	/*No se hace nada */ 		}
	;

LId	: coma id	{	
				simbolo = new Simbolo();

				if((simbolo = tsActual.buscarSimbolo($2.lexema)) != NULL)
					errorSemantico(ERRYADECL, $2);
				else{
					simbolo = tsActual.buscarSimbolo($-1.lexema);
					dir = tsActual.nextDir();

					if(simbolo->tipo == LOGICO){
						sprintf(s, "mov #0 %d;coma id\n\n", dir);
						$$.cod = s;
						tsActual.nuevoSimbolo($2.lexema, simbolo->tipo, ENTERO, dir, ttipos.tamTotal(simbolo->tipo));
					}
					else {
						if(simbolo->tipo == ENTERO)
							sprintf(s, "mov #0 %d;coma id\n\n", dir);
						else if(simbolo->tipo == REAL)
							sprintf(s, "mov $0 %d;coma id\n\n", dir);

						$$.cod = s;
						tsActual.nuevoSimbolo($2.lexema, simbolo->tipo, simbolo->tipo, dir, ttipos.tamTotal(simbolo->tipo));
					}
				}

				dir = tsActual.nextDir();
				if(dir >= 10000)
					errorSemantico(ERR_NOCABE, $2);
			}
	  LId		{
				$$.cod = $3.cod + $4.cod;
			}
	| 		{	$$.cod = "";	}
	;

Tipo	: entero			{	$$.tipo = ENTERO;	}
	| real				{	$$.tipo = REAL;		}
	| logico			{	$$.tipo = LOGICO;	}
	| tabla numentero de Tipo	{
						if($2.lexema == "0")
							errorSemantico(ERRDIM, $2);
						$$.tipo = ttipos.nextId();
						ttipos.nuevoTipo($$.tipo, atoi($2.lexema.c_str()), (atoi($2.lexema.c_str())*ttipos.tamTotal($4.tipo)), $4.tipo);
					}
	;

SInstr	: SInstr pyc	{	regTemporal = 10000;		}
	  Instr		{
				$$.cod = $1.cod + $4.cod;
			}
	| Instr 	{
				$$.cod = $1.cod;
			}
	;

Instr	: escribe Expr				{

							if($2.tipo == ENTERO){
								sprintf(s, "wri %d;escribe Ref\nwrl\n\n", $2.dir);
								$$.cod = s;
							}
							else if($2.tipo == REAL){
								sprintf(s, "wrr %d;escribe Ref\nwrl\n\n", $2.dir);
								$$.cod = s;
							}
							else if($2.tipo == LOGICO){								
								contadorEtiquetas++;
								sprintf(s, "mov %d A;escribe Ref\njz L%d\nwrc #99\nwrl\n", $2.dir, contadorEtiquetas);
								$$.cod = $2.cod + s;
								
								contadorEtiquetas++;
								sprintf(s, "jmp L%d\nL%d wrc #102\nwrl\n", contadorEtiquetas, contadorEtiquetas-1);
								$$.cod = $$.cod + s;

								sprintf(s, "L%d ", contadorEtiquetas);
								$$.cod = $$.cod + s;
							}
							
							$$.cod = $2.cod + $$.cod;
						}
	| lee Ref				{

							if($2.tipo == ENTERO)
								sprintf(s, "mov %d A;lee Ref\nmuli #%d\naddi #%d\nrdi @A\n\n", $2.dir, ttipos.tam($2.tipo), $2.dbase);
							else if($2.tipo == REAL)
								sprintf(s, "mov %d A;lee Ref\nmuli #%d\naddi #%d\nrdr @A\n\n", $2.dir, ttipos.tam($2.tipo), $2.dbase);
							else if($2.tipo == LOGICO) {
								dir = NuevaTemporal();
								if(dir == 16383)
									errorSemantico(ERR_MAXTMP, $1);
								sprintf(s, "rdc A;lee Ref\neqli #99\nmov A %d\nmov %d A\nmuli #%d\naddi #%d\nmov %d @A\n\n", dir, $2.dir, ttipos.tam($2.tipo), $2.dbase,dir);
							}
							
							$$.cod = $2.cod + s;
						}
	| si Expr entonces Instr		{
							if($2.tipo != LOGICO)
								errorSemantico(ERR_EXP_LOG,$1);
							else{
								contadorEtiquetas++;
								sprintf(s, "mov %d A;si\njz L%d\n", $2.dir, contadorEtiquetas);
								$$.cod = $2.cod + s + $4.cod;
								
								sprintf(s, "L%d ", contadorEtiquetas);
								$$.cod = $$.cod + s;
							}
						}
	| si Expr entonces Instr sino Instr	{
							if($2.tipo != LOGICO)
								errorSemantico(ERR_EXP_LOG,$1);
							else{
								contadorEtiquetas++;
								sprintf(s, "mov %d A;si sino\njz L%d\n", $2.dir, contadorEtiquetas);
								$$.cod = $2.cod + s + $4.cod;
								
								contadorEtiquetas++;
								sprintf(s, "jmp L%d\nL%d ", contadorEtiquetas, contadorEtiquetas-1);
								$$.cod = $$.cod + s + $6.cod;

								sprintf(s, "L%d ", contadorEtiquetas);
								$$.cod = $$.cod + s;
							}
						}
	| mientras Expr hacer Instr		{
							if($2.tipo != LOGICO)
								errorSemantico(ERR_EXP_LOG,$1);
							else{
								contadorEtiquetas++;
								sprintf(s, "L%d ", contadorEtiquetas);
								$$.cod = s + $2.cod;
								
								contadorEtiquetas++;
								sprintf(s, "mov %d A;mientras\njz L%d\n", $2.dir, contadorEtiquetas);
								$$.cod = $$.cod + s + $4.cod;
								
								sprintf(s, "jmp L%d\nL%d ", contadorEtiquetas-1, contadorEtiquetas);
								$$.cod = $$.cod + s;
							}
						}
	| Ref opasig				{
							if(isArray($1.tipo))
								errorSemantico(ERRFALTAN, $1);
						}
	  Expr		
						{	
							if(isArray($4.tipo))
								errorSemantico(ERRFALTAN, $4);

							if($1.tipo == LOGICO && $1.tipo != $4.tipo)
								errorSemantico(ERR_EXDER_LOG, $2);
							else if($1.tipo == ENTERO && $1.tipo != $4.tipo)
								errorSemantico(ERR_EXDER_ENT, $2);
							else if($1.tipo == REAL && !($4.tipo == REAL || $4.tipo == ENTERO))
								errorSemantico(ERR_EXDER_RE, $2);
							else{
								if($1.tipo == REAL && $4.tipo == ENTERO){
									dir = NuevaTemporal();
									if(dir == 16383)
										errorSemantico(ERR_MAXTMP, $1);
									sprintf(s, "mov %d A;Ref opasig Expr\nitor\nmov A %d\nmov %d A\nmuli #%d\naddi #%d\nmov %d @A\n\n", $4.dir, dir, $1.dir, ttipos.tam($1.tipo), $1.dbase, dir);
								}
								else
									sprintf(s, "mov %d A;Ref opasig Expr\nmuli #%d\naddi #%d\nmov %d @A\n\n", $1.dir, ttipos.tam($1.tipo), $1.dbase, $4.dir);
							}

							$$.cod = $1.cod + $4.cod + s;
						}
	| blq SInstr fblq			{	$$.cod = $2.cod;	}
	;

Expr	: Expr obool Econj	{
					if($3.tipo != LOGICO)
						errorSemantico(ERR_EXDER_LOG, $2);
					else if($1.tipo != LOGICO)
						errorSemantico(ERR_EXIZQ_LOG, $2);
					else{
						dir = NuevaTemporal();
						if(dir == 16383)
							errorSemantico(ERR_MAXTMP, $1);
						if($1.tipoOp == ENTERO && $3.tipoOp == ENTERO){
							sprintf(s, "mov %d A;Expr obool Econj\n%si %d\nmov A %d\n\n", $1.dir, $2.trad.c_str(), $3.dir, dir);
							$$.tipoOp = ENTERO;
							$$.tipo = LOGICO;
						}
						else if($1.tipoOp == REAL && $3.tipoOp == ENTERO){
							diritor = NuevaTemporal();
							if(diritor == 16383)
								errorSemantico(ERR_MAXTMP, $1);
							sprintf(s, "mov %d A;Expr obool Econj\nitor\nmov A %d\nmov %d A\n%sr %d\nmov A %d\n\n", $3.dir, diritor, $1.dir, $2.trad.c_str(), diritor, dir);
							$$.tipoOp = REAL;
							$$.tipo = LOGICO;
						}
						else if($1.tipoOp == ENTERO && $3.tipoOp == REAL){
							sprintf(s, "mov %d A;Expr obool Econj\nitor\n%sr %d\nmov A %d\n\n", $1.dir, $2.trad.c_str(), $3.dir, dir);
							$$.tipoOp = REAL;
							$$.tipo = LOGICO;
						}
						else if($1.tipoOp == REAL && $3.tipoOp == REAL){
							sprintf(s, "mov %d A;Expr obool Econj\n%sr %d\nmov A %d\n\n", $1.dir, $2.trad.c_str(), $3.dir, dir);
							$$.tipoOp = REAL;
							$$.tipo = LOGICO;
						}

						$$.dir = dir;
						$$.cod = $1.cod + $3.cod + s;
					}
				}
	| Econj			{
					$$.cod = $1.cod;
					$$.dir = $1.dir;
					$$.tipoOp = $1.tipoOp;
					$$.tipo = $1.tipo;
					$$.nlin = $1.nlin;
					$$.ncol = $1.ncol;
				}
	;

Econj	: Econj ybool Ecomp	{
					if($3.tipo != LOGICO){
						errorSemantico(ERR_EXDER_LOG, $2);
					}
					else if($1.tipo != LOGICO){
						errorSemantico(ERR_EXIZQ_LOG, $2);
					}
					else{
						dir = NuevaTemporal();
						if(dir == 16383)
							errorSemantico(ERR_MAXTMP, $1);
						if($1.tipoOp == ENTERO && $3.tipoOp == ENTERO){
							sprintf(s, "mov %d A;Econj ybool Ecomp\n%si %d\nmov A %d\n\n", $1.dir, $2.trad.c_str(), $3.dir, dir);
							$$.tipoOp = ENTERO;
							$$.tipo = LOGICO;
						}
						else if($1.tipoOp == REAL && $3.tipoOp == ENTERO){
							diritor = NuevaTemporal();
							if(diritor == 16383)
								errorSemantico(ERR_MAXTMP, $1);
							sprintf(s, "mov %d A;Econj ybool Ecomp\nitor\nmov A %d\nmov %d A\n%sr %d\nmov A %d\n\n", $3.dir, diritor, $1.dir, $2.trad.c_str(), diritor, dir);
							$$.tipoOp = REAL;
							$$.tipo = LOGICO;
						}
						else if($1.tipoOp == ENTERO && $3.tipoOp == REAL){
							sprintf(s, "mov %d A;Econj ybool Ecomp\nitor\n%sr %d\nmov A %d\n\n", $1.dir, $2.trad.c_str(), $3.dir, dir);
							$$.tipoOp = REAL;
							$$.tipo = LOGICO;
						}
						else if($1.tipoOp == REAL && $3.tipoOp == REAL){
							sprintf(s, "mov %d A;Econj ybool Ecomp\n%sr %d\nmov A %d\n\n", $1.dir, $2.trad.c_str(), $3.dir, dir);
							$$.tipoOp = REAL;
							$$.tipo = LOGICO;
						}

						$$.dir = dir;
						$$.cod = $1.cod + $3.cod + s;
					}
				}
	| Ecomp			{
					$$.cod = $1.cod;
					$$.dir = $1.dir;
					$$.tipoOp = $1.tipoOp;
					$$.tipo = $1.tipo;
					$$.nlin = $1.nlin;
					$$.ncol = $1.ncol;
				}
	;

Ecomp	: Esimple oprel Esimple	{
					dir = NuevaTemporal();
					if(dir == 16383)
						errorSemantico(ERR_MAXTMP, $1);
					$$.dir = dir;
					
					if($1.tipo == LOGICO)
						errorSemantico(ERR_EXIZQ_RE, $2);
					if($1.tipo == ENTERO && $3.tipo == ENTERO){
						sprintf(s, "mov %d A;Esimple oprel Esimple\n%si %d\nmov A %d\n\n", $1.dir, $2.trad.c_str(), $3.dir, dir);
						$$.tipoOp = ENTERO;
						$$.tipo = LOGICO;
					}
					else if($1.tipo == REAL && $3.tipo == ENTERO){
						diritor = NuevaTemporal();
						if(diritor == 16383)
							errorSemantico(ERR_MAXTMP, $1);
						sprintf(s, "mov %d A;Esimple oprel Esimple\nitor\nmov A %d\nmov %d A\n%sr %d\nmov A %d\n\n", $3.dir, diritor, $1.dir, $2.trad.c_str(), diritor, dir);
						$$.tipoOp = REAL;
						$$.tipo = LOGICO;
					}
					else if($1.tipo == ENTERO && $3.tipo == REAL){
						sprintf(s, "mov %d A;Esimple oprel Esimple\nitor\n%sr %d\nmov A %d\n\n", $1.dir, $2.trad.c_str(), $3.dir, dir);
						$$.tipoOp = REAL;
						$$.tipo = LOGICO;
					}
					else if($1.tipo == REAL && $3.tipo == REAL){
						sprintf(s, "mov %d A;Esimple oprel Esimple\n%sr %d\nmov A %d\n\n", $1.dir, $2.trad.c_str(), $3.dir, dir);
						$$.tipoOp = REAL;
						$$.tipo = LOGICO;
					}
					else
						errorSemantico(ERR_EXDER_RE, $2);

					$$.cod = $1.cod + $3.cod + s;
				}
	| Esimple		{
					$$.cod = $1.cod;
					$$.dir = $1.dir;
					$$.tipoOp = $1.tipoOp;
					$$.tipo = $1.tipo;
					$$.nlin = $1.nlin;
					$$.ncol = $1.ncol;
				}
	;

Esimple	: Esimple opas Term	{
					dir = NuevaTemporal();
					if(dir == 16383)
						errorSemantico(ERR_MAXTMP, $1);
					$$.dir = dir;
					if($1.tipo == LOGICO)
						errorSemantico(ERR_EXIZQ_RE, $2);
					if($1.tipo == ENTERO && $3.tipo == ENTERO){
						sprintf(s, "mov %d A;Esimple opas Term\n%si %d\nmov A %d\n\n", $1.dir, $2.trad.c_str(), $3.dir, dir);
						$$.tipo = ENTERO;
						$$.tipoOp = ENTERO;
					}
					else if($1.tipo == REAL && $3.tipo == ENTERO){
						diritor = NuevaTemporal();
						if(diritor == 16383)
							errorSemantico(ERR_MAXTMP, $1);
						sprintf(s, "mov %d A;Esimple opas Term\nitor\nmov A %d\nmov %d A\n%sr %d\nmov A %d\n\n", $3.dir, diritor, $1.dir, $2.trad.c_str(), diritor, dir);
						$$.tipo = REAL;
						$$.tipoOp = REAL;
					}
					else if($1.tipo == ENTERO && $3.tipo == REAL){
						sprintf(s, "mov %d A;Esimple opas Term\nitor\n%sr %d\nmov A %d\n\n", $1.dir, $2.trad.c_str(), $3.dir, dir);
						$$.tipo = REAL;
						$$.tipoOp = REAL;
					}
					else if($1.tipo == REAL && $3.tipo == REAL){
						sprintf(s, "mov %d A;Esimple opas Term\n%sr %d\nmov A %d\n\n", $1.dir, $2.trad.c_str(), $3.dir, dir);
						$$.tipo = REAL;
						$$.tipoOp = REAL;
					}
					else
						errorSemantico(ERR_EXDER_RE, $2);

					$$.cod = $1.cod + $3.cod + s;
				}
	| Term			{
					$$.cod = $1.cod;
					$$.dir = $1.dir;
					$$.tipoOp = $1.tipoOp;
					$$.tipo = $1.tipo;
					$$.nlin = $1.nlin;
					$$.ncol = $1.ncol;
				}
	| opas Term		{
					dir = NuevaTemporal();
					if(dir == 16383)
						errorSemantico(ERR_MAXTMP, $1);
					$$.dir = dir;
	
					if($1.trad == "sub"){
						if($2.tipo == ENTERO){
							sprintf(s, "mov #0 A;opas Term\n%si %d\nmov A %d\n\n", $1.trad.c_str(), $2.dir, dir);
							$$.tipo = ENTERO;
						}
						else if($2.tipo == REAL){
							sprintf(s, "mov #0 A;opas Term\n%sr %d\nmov A %d\n\n", $1.trad.c_str(), $2.dir, dir);
							$$.tipo = REAL;
						}
						else
							errorSemantico(ERR_EXDER_RE, $2);
						
					}
					else{
						sprintf(s, "mov %d %d;opas Term\n\n", $2.dir, dir);
						if($2.tipo == ENTERO)
							$$.tipo = ENTERO;
						else if($2.tipo == REAL)
							$$.tipo = REAL;
						else
							errorSemantico(ERR_EXDER_RE, $2);
					}
	
					$$.cod = $2.cod + s;
				}
	;

Term	: Term opmd Factor	{
					dir = NuevaTemporal(); 
					if(dir == 16383)
						errorSemantico(ERR_MAXTMP, $1);
					$$.dir = dir;
	
					if($1.tipo == ENTERO && $3.tipo == ENTERO){
						sprintf(s, "mov %d A;Term opmd Factor\n%si %d\nmov A %d\n\n", $1.dir, $2.trad.c_str(), $3.dir, dir);
						$$.tipo = ENTERO;
						$$.tipoOp = ENTERO;
					}
					else if($1.tipo == REAL && $3.tipo == ENTERO){
						diritor = NuevaTemporal();
						if(diritor == 16383)
							errorSemantico(ERR_MAXTMP,$1);
						sprintf(s, "mov %d A;Term opmd Factor\nitor\nmov A %d\nmov %d A\n%sr %d\nmov A %d\n\n", $3.dir, diritor, $1.dir, $2.trad.c_str(), diritor, dir);
						$$.tipo = REAL;
						$$.tipoOp = REAL;
					}
					else if($1.tipo == ENTERO && $3.tipo == REAL){
						sprintf(s, "mov %d A;Term opmd Factor\nitor\n%sr %d\nmov A %d\n\n", $1.dir, $2.trad.c_str(), $3.dir, dir);
						$$.tipo = REAL;
						$$.tipoOp = REAL;
					}
					else if($1.tipo == REAL && $3.tipo == REAL) {
						sprintf(s, "mov %d A;Term opmd Factor\n%sr %d\nmov A %d\n\n", $1.dir, $2.trad.c_str(), $3.dir, dir);
						$$.tipo = REAL;
						$$.tipoOp = REAL;
					}
					else
						errorSemantico(ERR_EXDER_RE, $2);

					$$.cod = $1.cod + $3.cod + s;
				}
	| Factor		{
					$$.nlin = $1.nlin;
					$$.ncol = $1.ncol;
					$$.dir = $1.dir;
					$$.cod = $1.cod;
					$$.tipo = $1.tipo;
					$$.tipoOp = $1.tipoOp;
				}
	;

Factor	: Ref			{
					if(isArray($1.tipo))
						errorSemantico(ERRFALTAN, $1);
					else{
						$$.nlin = $1.nlin;
						$$.ncol = $1.ncol;
						dir = NuevaTemporal();
						if(dir == 16383)
							errorSemantico(ERR_MAXTMP, $1);
						$$.dir = dir;
						sprintf(s, "mov %d A;Ref\nmuli #%d\naddi #%d\nmov @A %d\n\n", $1.dir, ttipos.tam($1.tipo), $1.dbase, dir);
						$$.cod = $1.cod + s;
						$$.tipo = $1.tipo;
						$$.tipoOp = $1.tipoOp;
					}
				}
	| numentero		{
					$$.nlin = $1.nlin;
					$$.ncol = $1.ncol;
					dir = NuevaTemporal();
					$$.dir = dir;
					if(dir == 16383)
						errorSemantico(ERR_MAXTMP,$1);
					$$.tipo = ENTERO;
					$$.tipoOp = ENTERO;
					sprintf(s, "#%s", $1.lexema.c_str());
					$$.trad = s;
					sprintf(s, "mov %s %d;numentero\n\n", $$.trad.c_str(), dir);
					$$.cod = s;
				}
	| numreal		{
					$$.nlin = $1.nlin;
					$$.ncol = $1.ncol;
					dir = NuevaTemporal();
					$$.dir = dir;
					if(dir == 16383)
						errorSemantico(ERR_MAXTMP,$1);
					$$.tipo = REAL;
					$$.tipoOp = REAL;
					sprintf(s, "$%s", $1.lexema.c_str());
					$$.trad = s;
					sprintf(s, "mov %s %d;numreal\n\n", $$.trad.c_str(), dir);
					$$.cod = s;
				}
	| pari Expr pard	{
					$$.nlin = $2.nlin;
					$$.ncol = $2.ncol;
					$$.dir = $2.dir;
					$$.cod = $2.cod;
					$$.tipo = $2.tipo;
				}
	| nobool Factor		{
					dir = NuevaTemporal();
					if(dir == 16383)
						errorSemantico(ERR_MAXTMP,$1);
					$$.dir = dir;

					if($2.tipoOp == ENTERO){
						sprintf(s, "mov %d A;nobool Factor\n%si\nmov A %d\n\n", $2.dir, $1.trad.c_str(), dir);
						$$.tipoOp = ENTERO;
						$$.tipo = LOGICO;
					}
					else if($2.tipoOp == REAL){
						sprintf(s, "mov %d A;nobool Factor\n%sr\nmov A %d\n\n", $2.dir, $1.trad.c_str(), dir);
						$$.tipoOp = REAL;
						$$.tipo = LOGICO;
					}
	
					$$.cod = $2.cod + s;
				}
	| cierto		{
					$$.nlin = $1.nlin;
					$$.ncol = $1.ncol;
					dir = NuevaTemporal();
					$$.dir = dir;
					if(dir == 16383)
						errorSemantico(ERR_MAXTMP, $1);
					$$.tipo = LOGICO;
					$$.tipoOp = ENTERO;
					$$.trad = "#1";
					sprintf(s, "mov %s %d;cierto\n\n", "#1", dir);
					$$.cod = s;
				}
	| falso			{
					$$.nlin = $1.nlin;
					$$.ncol = $1.ncol;
					dir = NuevaTemporal();
					$$.dir = dir;
					if(dir == 16383)
						errorSemantico(ERR_MAXTMP, $1);
					$$.tipo = LOGICO;
					$$.tipoOp = ENTERO;
					$$.trad = "#0";
					sprintf(s, "mov %s %d;falso\n\n", "#0", dir);
					$$.cod = s;
				}
	;

Ref	: id			{
					simbolo = new Simbolo();
					if((simbolo = tsActual.buscarSimbolo($1.lexema)) == NULL)
						errorSemantico(ERRNODECL, $1);
					else{
						dir = NuevaTemporal();
						if(dir == 16383)
							errorSemantico(ERR_MAXTMP, $1);
						$$.dir = dir;
						$$.dbase = simbolo->dir;
						sprintf(s, "mov #0 %d;id\n\n",dir);
						$$.cod = s;
						$$.tipo = simbolo->tipo;
						$$.tipoOp = simbolo->tipoOp;
					}
				}
		| Ref lcor	{
					if(!isArray($1.tipo))
						errorSemantico(ERRSOBRAN, $2);
				}
		  Esimple rcor	{
					if($4.tipo != ENTERO)
						errorSemantico(ERR_EXP_ENT, $4);
					else{
						$$.tipo = ttipos.tipoBase($1.tipo);
						dir = NuevaTemporal();
						if(dir == 16383)
							errorSemantico(ERR_MAXTMP, $1);
						$$.dir = dir;
						$$.dbase = $1.dbase;
						sprintf(s, "mov %d A;Ref [E]\nmuli #%d\naddi %d\nmov A %d\n\n", $1.dir, ttipos.tam($1.tipo), $4.dir, dir);
						$$.cod = $1.cod + $4.cod + s;
						$$.ncol = $5.ncol;
					}		
				}
	;

%%

int NuevaTemporal(){
	int newDir = regTemporal;
	regTemporal++;
	return newDir;
}

void getTipoBaseArray(int &tipo){
	if(tipo != ENTERO && tipo != REAL && tipo != LOGICO){
		tipo = ttipos.tipoBase(tipo);
		getTipoBaseArray(tipo);
	}
	else 
		return;
}

bool isArray(int tipo){
	return tipo > 3;
}

int yyerror(std::string s)
{
	extern int findefichero;		// de plp2.l
	if (findefichero){
		msgError(ERREOF,0,0,"");
	}
	else{
		msgError(ERRSINT,nlin,ncol-strlen(yytext),yytext);
	}
}

void errorSemantico(int nerror, Atributos token){
	msgError(nerror,token.nlin,token.ncol,token.lexema.c_str());
}

void msgError(int nerror,int nlin,int ncol,const char *s){
	if (nerror != ERREOF){
		fprintf(stderr,"Error %d (%d:%d) ",nerror,nlin,ncol);
		switch (nerror){
			case ERRLEXICO:fprintf(stderr,"caracter '%s' incorrecto\n",s);
				break;
			case ERRSINT:fprintf(stderr,"en '%s'\n",s);
				break;
			case ERRYADECL:fprintf(stderr,"variable '%s' ya declarada\n",s);
				break;
			case ERRNODECL:fprintf(stderr,"variable '%s' no declarada\n",s);
				break;
			case ERRDIM:fprintf(stderr,"la dimension debe ser mayor que cero\n");
				break;
			case ERRFALTAN:fprintf(stderr,"faltan indices\n");
				break;
			case ERRSOBRAN:fprintf(stderr,"sobran indices\n");
				break;
			case ERR_EXP_ENT:fprintf(stderr,"la expresion entre corchetes debe ser de tipo entero\n");
				break;
			case ERR_EXP_LOG:fprintf(stderr,"la expresion debe ser de tipo logico\n");
				break;
			case ERR_EXDER_LOG:fprintf(stderr,"la expresion a la derecha de '%s' debe ser de tipo logico\n",s);
				break;
			case ERR_EXDER_ENT:fprintf(stderr,"la expresion a la derecha de '%s' debe ser de tipo entero\n",s);
				break;
			case ERR_EXDER_RE:fprintf(stderr,"la expresion a la derecha de '%s' debe ser de tipo real o entero\n",s);
				break;
			case ERR_EXIZQ_LOG:fprintf(stderr,"la expresion a la izquierda de '%s' debe ser de tipo logico\n",s);
				break;
			case ERR_EXIZQ_RE:fprintf(stderr,"la expresion a la izquierda de '%s' debe ser de tipo real o entero\n",s);
				break;
			case ERR_NOCABE:fprintf(stderr,"la variable '%s' ya no cabe en memoria\n",s);
				break;
			case ERR_MAXVAR:fprintf(stderr,"en la variable '%s', hay demasiadas variables declaradas\n",s);
				break;
			case ERR_MAXTIPOS:fprintf(stderr,"hay demasiados tipos definidos\n");
				break;
			case ERR_MAXTMP:fprintf(stderr,"no hay espacio para variables temporales\n");
				break;
		}
	}
	else
		fprintf(stderr,"Error al final del fichero\n");
	exit(1);
}

int main(int argc, char *argv[]){
	FILE *fent;
	if (argc == 2){
		fent = fopen(argv[1], "rt");
		if (fent) {
			yyin = fent;
			yyparse();
			fclose(fent);
		}
		else
			fprintf(stderr, "No puedo abrir el fichero\n");
	}
	else
		fprintf(stderr, "Uso: ejemplo <nombre de fichero>\n");
}
