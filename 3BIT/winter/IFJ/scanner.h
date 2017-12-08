#ifndef __SCANNER_H__
#define __SCANNER_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "str.h"

//klucove slova IFJ17

#define true        1
#define false       0

#define KW_AS       1000
#define KW_ASC      1001
#define KW_DECLARE  1002
#define KW_DIM      1003
#define KW_DO       1004
#define KW_DOUBLE   1005
#define KW_ELSE     1006
#define KW_END      1007
#define KW_CHR      1008
#define KW_FUNCTION 1009
#define KW_IF       1010
#define KW_INPUT    1011
#define KW_INTEGER  1012
#define KW_LENGTH   1013
#define KW_LOOP     1014
#define KW_PRINT    1015
#define KW_RETURN   1016
#define KW_SCOPE    1017
#define KW_STRING   1018
#define KW_SUBSTR   1019
#define KW_THEN     1020
#define KW_WHILE    1021
//Rezervovane klucove slova
#define KW_AND      1022
#define KW_BOOLEAN  1023
#define KW_CONTINUE 1024
#define KW_ELSEIF   1025
#define KW_EXIT     1026
#define KW_FALSE    1027
#define KW_FOR      1028
#define KW_NEXT     1029
#define KW_NOT      1030
#define KW_OR       1031
#define KW_SHARED   1032
#define KW_STATIC   1033
#define KW_TRUE     1034
//Operatory
#define OP_PLUS     1500
#define OP_MINUS    1501
#define OP_MUL      1502
#define OP_DIV      1503
#define OP_IDIV     1504
#define OP_EQ       1505
#define OP_LOWER    1506
#define OP_BIGGER   1507
#define OP_LOWEREQ  1508
#define OP_BIGGEREQ 1509
#define OP_NOTEQ    1510

#define SEMICOLON   1600
#define PAR_LEFT    1601
#define PAR_RIGHT   1602
#define NEW_LINE    1603
#define COMMA       1604
//Stavy automatu
#define TDOUBLE     1998
#define TSTRING     1999
#define START       2000
#define TINTEGER    2001
#define IDENTIFY    2002
#define DOT         2003
#define EXPONENT    2004
#define EXP_OP      2005
#define STR_START   2006
#define STR_SPEC    2007
#define OKEY        2008
#define OKEY1       2009
#define OKEY2       2010
#define OKEY3       2011
#define ZERO        2012
#define ZERO1       2013
#define LN_KOMENT   2014
#define BL_KOMENT   2015
#define EXP_OP_DIGIT 2016
#define EXPONENT2    20017
#define NETERMINAL  3000

int isKeyword(string *str);
int getToken(FILE *fp, string *str);

#endif
