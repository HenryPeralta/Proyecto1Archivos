/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    pmkdisk = 258,
    psize = 259,
    pfit = 260,
    pbestfit = 261,
    pfirtsfit = 262,
    pworstfit = 263,
    punidad = 264,
    pkilobyte = 265,
    pmegabyte = 266,
    ppath = 267,
    pextension = 268,
    prmdisk = 269,
    pfdisk = 270,
    pbyte = 271,
    ptype = 272,
    ppartiprimaria = 273,
    ppartiextendida = 274,
    ppartilogica = 275,
    pdelete = 276,
    pfast = 277,
    pfull = 278,
    pname = 279,
    padd = 280,
    pmount = 281,
    pmkdir = 282,
    punto = 283,
    bracketabre = 284,
    bracketcierra = 285,
    corcheteabre = 286,
    corchetecierra = 287,
    puntocoma = 288,
    potencia = 289,
    coma = 290,
    parentesisabre = 291,
    parentesiscierra = 292,
    llaveabre = 293,
    llavecierra = 294,
    mas = 295,
    menos = 296,
    multiplicacion = 297,
    igual = 298,
    dolar = 299,
    dospuntos = 300,
    diagonal = 301,
    entero = 302,
    numnegativo = 303,
    cadena = 304,
    identificador = 305,
    caracter = 306,
    ruta = 307,
    rutacualquiera = 308,
    suma = 309,
    multi = 310,
    division = 311
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 30 "parser.y"

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
class comando_mount *mount;

#line 128 "parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
