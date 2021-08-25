%{
#include "scanner.h"//se importa el header del analisis sintactico
#include <QString>
#include <string>
#include "qdebug.h"
#include <iostream>
#include "comando_mkdisk.h"
#include "string"
#include <string.h>
//#include "obmkdisk.h"
using namespace std;
extern int yylineno; //linea actual donde se encuentra el parser (analisis lexico) lo maneja BISON
extern int columna; //columna actual donde se encuentra el parser (analisis lexico) lo maneja BISON
extern char *yytext; //lexema actual donde esta el parser (analisis lexico) lo maneja BISON

int yyerror(const char* mens)
{
std::cout << mens <<" "<<yytext<< std::endl;
return 0;
}
%}
//error-verbose si se especifica la opcion los errores sintacticos son especificados por BISON
%defines "parser.h"
%output "parser.cpp"
//%error-verbose
%define parse.error verbose
%locations
%union{
//se especifican los tipo de valores para los no terminales y lo terminales
//char TEXT [256];
//QString TEXT;
char TEXT[256];
//char texto[256];
int texto;
//QString texto;
class comando_mkdisk *mkdisk;
}
//TERMINALES DE TIPO TEXT, SON STRINGS

%token<TEXT> pmkdisk;
%token<TEXT> psize;
%token<TEXT> pfit;
%token<TEXT> pbestfit;
%token<TEXT> pfirtsfit;
%token<TEXT> pworstfit;
%token<TEXT> punidad;
%token<TEXT> pkilobyte;
%token<TEXT> pmegabyte;
%token<TEXT> ppath;
%token<TEXT> pextension;

%token<TEXT> pmkdir;


%token<TEXT> punto;
%token<TEXT> bracketabre;
%token<TEXT> bracketcierra;
%token<TEXT> corcheteabre;
%token<TEXT> corchetecierra;
%token<TEXT> puntocoma;
%token<TEXT> potencia;
%token<TEXT> coma;
%token<TEXT> parentesisabre;
%token<TEXT> parentesiscierra;

%token<TEXT> llaveabre;
%token<TEXT> llavecierra;
%token<TEXT> mas;
%token<TEXT> menos;
%token<TEXT> multiplicacion;
%token<TEXT> igual;
%token<TEXT> dolar;
%token<TEXT> dospuntos;
%token<TEXT> diagonal;
//%token<TEXT> barra;

%token<TEXT> entero;
%token<TEXT> numnegativo;
%token<TEXT> cadena;
%token<TEXT> identificador;
%token<TEXT> caracter;
%token<TEXT> ruta;
%token<TEXT> rutacualquiera;
//%token<TEXT> pdisk;




%type<mkdisk> COMANDOMKDISK;
%type<TEXT> TAMANOBYTE;
%type<TEXT> TIPOAJUSTE;
%type<TEXT> PATH;

%left suma menos
%left multi division
%left potencia
%start INICIO
%%

INICIO : LEXPA { }
;

LEXPA:  pmkdisk COMANDOMKDISK
{

$2->crearDisco($2);
printf("estoy en lexpa primera produccion");
}


;

COMANDOMKDISK:
COMANDOMKDISK menos psize igual entero {int tam = atoi($5); $1->size = tam; $$=$1;}
| menos psize igual entero {int tam = atoi($4); comando_mkdisk *disco = new comando_mkdisk(); disco->size= tam; $$=disco; }
//| COMANDOMKDISK menos psize igual menos entero
//| menos psize igual menos entero
| COMANDOMKDISK menos punidad igual TAMANOBYTE {$1->unit= $5; $$=$1;}
| menos punidad igual TAMANOBYTE {comando_mkdisk *disco = new comando_mkdisk(); disco->unit= $4; $$=disco;}
| COMANDOMKDISK menos pfit igual TIPOAJUSTE {$1->fit= $5; $$=$1;}
| menos pfit igual TIPOAJUSTE {comando_mkdisk *disco = new comando_mkdisk(); disco->fit= $4; $$=disco;}
| COMANDOMKDISK menos ppath igual PATH {$1->path= $5; $$=$1;}
| menos ppath igual PATH {comando_mkdisk *disco = new comando_mkdisk(); disco->path= $4; $$=disco;}
;

TAMANOBYTE:
pkilobyte {$$;}
| pmegabyte {$$;}
;

TIPOAJUSTE:
pbestfit {$$;}
| pfirtsfit {$$;}
| pworstfit {$$;}
;

PATH:
PATH diagonal identificador {
    string raiz($2);
    string dire($3);
    raiz.append(dire);
    string path($1);
    path.append(raiz);

    strcpy($$, path.c_str());
}
| diagonal identificador {
    string raiz($1);
    string dire($2);
    raiz.append(dire);

    strcpy($$, raiz.c_str());
}
|PATH diagonal identificador punto pextension{
    string ide($5);
    string point($4);
    point.append(ide);
    string raiz($3);
    raiz.append(point);
    string dire($2);
    dire.append(raiz);
    string path($1);
    path.append(dire);

    strcpy($$, path.c_str());
}
|diagonal identificador punto pextension{
    string ide($4);
    string point($3);
    point.append(ide);
    string raiz($2);
    raiz.append(point);
    string dire($1);
    dire.append(raiz);

    strcpy($$, dire.c_str());
}
|cadena{$$;}
;
