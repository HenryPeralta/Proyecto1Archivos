%{
#include "scanner.h"//se importa el header del analisis sintactico
#include <QString>
#include <string>
#include "qdebug.h"
#include <iostream>
#include "comando_mkdisk.h"
#include "comando_rmdisk.h"
#include "comando_fdisk.h"
#include "string"
#include <string.h>
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
class comando_rmdisk *rmdisk;
class comando_fdisk *fdisk;
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
%token<TEXT> prmdisk;
%token<TEXT> pfdisk;
%token<TEXT> pbyte;
%token<TEXT> ptype;
%token<TEXT> ppartiprimaria;
%token<TEXT> ppartiextendida;
%token<TEXT> ppartilogica;
%token<TEXT> pdelete;
%token<TEXT> pfast;
%token<TEXT> pfull;
%token<TEXT> pname;
%token<TEXT> padd;

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
%type<rmdisk> COMANDORMDISK;
%type<TEXT> TAMANOBYTE;
%type<TEXT> TIPOAJUSTE;
%type<TEXT> PATH;
%type<fdisk> COMANDOFDISK;
%type<TEXT> TIPONAME;
%type<TEXT> TIPOPARTICION;
%type<TEXT> TAMBYTEFDISK;
%type<TEXT> TIPODELETE
//%type<TEXT> TIPOADD

%left suma menos
%left multi division
%left potencia
%start INICIO
%%

INICIO : LEXPA { }
;

LEXPA: pmkdisk COMANDOMKDISK
{
    $2->crearDisco($2);
    printf("estoy en lexpa primera produccion");
}
| prmdisk COMANDORMDISK
{
    $2->borrarDisco($2);
    printf("estoy en lexpa primera produccion prmdisk");
}
| pfdisk COMANDOFDISK
{
    $2->adminParticiones($2);
    printf("estoy en lexpa primera produccion pfdisk");
}
;

COMANDOMKDISK:
COMANDOMKDISK menos psize igual entero {int tam = atoi($5); $1->psize = $3; $1->size = tam; $$=$1;}
| menos psize igual entero {int tam = atoi($4); comando_mkdisk *disco = new comando_mkdisk(); disco->size= tam; disco->psize = $2; $$=disco; }
| COMANDOMKDISK menos punidad igual TAMANOBYTE {$1->punit = $3; $1->unit= $5; $$=$1;}
| menos punidad igual TAMANOBYTE {comando_mkdisk *disco = new comando_mkdisk(); disco->unit= $4; disco->punit = $2; $$=disco;}
| COMANDOMKDISK menos pfit igual TIPOAJUSTE {$1->pfit = $3; $1->fit= $5; $$=$1;}
| menos pfit igual TIPOAJUSTE {comando_mkdisk *disco = new comando_mkdisk(); disco->fit= $4; disco->pfit = $2; $$=disco;}
| COMANDOMKDISK menos ppath igual PATH {$1->ppath = $3; $1->path= $5; $$=$1;}
| menos ppath igual PATH {comando_mkdisk *disco = new comando_mkdisk(); disco->path= $4; disco->ppath = $2; $$=disco;}
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

COMANDORMDISK:
menos ppath igual PATH {comando_rmdisk *disco = new comando_rmdisk(); disco->path= $4; $$=disco;}
;

COMANDOFDISK:
COMANDOFDISK menos psize igual entero {int tam = atoi($5); $1->psize = $3; $1->size = tam; $$=$1; }
| menos psize igual entero {int tam = atoi($4); comando_fdisk *disco = new comando_fdisk(); disco->size= tam; disco->psize = $2; $$=disco; }
| COMANDOFDISK menos ppath igual PATH {$1->ppath = $3; $1->path= $5; $$=$1;}
| menos ppath igual PATH {comando_fdisk *disco = new comando_fdisk(); disco->path= $4; disco->ppath = $2; $$=disco;}
| COMANDOFDISK menos pname igual TIPONAME {$1->pname = $3; $1->name= $5; $$=$1;}
| menos pname igual TIPONAME {comando_fdisk *disco = new comando_fdisk(); disco->name= $4; disco->pname = $2; $$=disco;}
| COMANDOFDISK menos ptype igual TIPOPARTICION {$1->ptype = $3; $1->type= $5; $$=$1;}
| menos ptype igual TIPOPARTICION {comando_fdisk *disco = new comando_fdisk(); disco->type= $4; disco->ptype = $2; $$=disco;}
| COMANDOFDISK menos punidad igual TAMBYTEFDISK {$1->punit = $3; $1->unit= $5; $$=$1;}
| menos punidad igual TAMBYTEFDISK {comando_fdisk *disco = new comando_fdisk(); disco->unit= $4; disco->punit = $2; $$=disco;}
| COMANDOFDISK menos pfit igual TIPOAJUSTE {$1->pfit = $3; $1->fit= $5; $$=$1;}
| menos pfit igual TIPOAJUSTE {comando_fdisk *disco = new comando_fdisk(); disco->fit= $4; disco->pfit = $2; $$=disco;}
| COMANDOFDISK menos pdelete igual TIPODELETE {$1->pcoman_delete = $3; $1->coman_delete= $5; $$=$1;}
| menos pdelete igual TIPODELETE {comando_fdisk *disco = new comando_fdisk(); disco->coman_delete= $4; disco->pcoman_delete = $2; $$=disco;}
| COMANDOFDISK menos padd igual entero {$1->padd = $3; $1->add= atoi($5); $$=$1;}
| menos padd igual entero {comando_fdisk *disco = new comando_fdisk(); disco->add= atoi($4); disco->padd = $2; $$=disco;}
;

TIPONAME:
identificador {$$;}
| cadena {$$;}
;

TIPOPARTICION:
ppartiprimaria {$$;}
| ppartiextendida {$$;}
| ppartilogica {$$;}
;

TAMBYTEFDISK:
pbyte {$$;}
| pkilobyte {$$;}
| pmegabyte {$$;}
;

TIPODELETE:
pfast {$$;}
| pfull {$$;}
;

/*TIPOADD:
menos entero{
    string menox($1);
    string enterox($2);
    menox.append(enterox);

    strcpy($$, menox.c_str());
}
| entero{$$;}
;*/
