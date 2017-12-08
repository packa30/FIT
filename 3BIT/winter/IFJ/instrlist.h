/*  @Project: IFJ17
    @author: xgrofc00@stud.fit.vutbr.cz
    @author: xkrajc17@stud.fit.vutbr.cz
    @author: xfujac00@stud.fit.vutbr.cz
    @author: xpobis00@stud.fit.vutbr.cz
*/
#ifndef __INSTRLIST_H__
#define __INSTRLIST_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"

#define true 1
#define false 0

#define I_MUL 15
#define I_DEFVAR 16
#define I_CALL 3
#define I_CALLEND 20
#define I_RETURN 4
#define I_MOVE 5
#define I_ADD 6
#define I_SUB 7
#define I_DIV 8
#define I_IF 9
#define I_CYCLE 10
#define I_READ 11
#define I_WRITE 12
#define I_CONCAT 13
#define I_LABEL 14
#define I_IFEND 17
#define I_ELSE 26
#define I_CYCLEEND 18
#define I_MAIN 19
#define I_ENDFUNC 21
#define I_MOVE_RETVAL 22
#define I_LT 23
#define I_GT 24
#define I_EQ 25
#define I_INPUT 27
#define I_LT_EQ 28
#define I_GT_EQ 29
#define I_DEFTMP 30
#define I_IDIV 31
#define I_NOT_EQ 32
#define I_INT_TO_FLOAT 33
#define I_FLOAT_TO_INT 34
#define I_STRLEN 35
#define I_SUBSTR 36
#define I_ASC 37
#define I_CHR 38
#define I_SUBSTREND 39

#define TYPE_INT (2001)
#define TYPE_DOUBLE (1998)
#define TYPE_STRING (1999)
/* trojadresna instrukcia ma tvar: */
typedef struct Instr
{
    int instTyp;    //typ instrukcie
    void* addr1;    //adresa 1
    void* addr2;    //adresa 2
    void* addr3;    //adresa 3
    int type2;
    int type3;
} *tInstr;


typedef struct listItem2
{
  tInstr Instruction;
  struct listItem2 *nextItem;
} *tListItem;

typedef struct ListOfInstr
{
  tListItem first;  // ukazatel na prvni prvek
  tListItem last;   // ukazatel na posledni prvek
  tListItem active; // ukazatel na aktivni prvek
} *tListOfInstr;

tInstr instrFill(int type, void *addr1, void *addr2, void *addr3,int type2,int type3);
tListOfInstr listInit();
void listFree(tListOfInstr L);
void listInsertLast(tListOfInstr L, tInstr I);
void listFirst(tListOfInstr L);
void listNext(tListOfInstr L);
void listGoto(tListOfInstr L, void *gotoInstr);
void *listGetPointerLast(tListOfInstr L);
tInstr *listGetData(tListOfInstr L);
int instr_add(tListOfInstr List,int type,char *add1,char *add2, char *add3,int type2,int type3);

#endif
