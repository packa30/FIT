#ifndef __PARSER_H__
#define __PARSER_H__

#include "scanner.h"
#include "generate.h"

#define before_scope 10001
#define after_scope  10000
//Stavy pre automat, ktorý rieši výrazy
#define STATE_LPAR    100
#define STATE_RPAR    101
#define STATE_ID      102
#define STATE_OP      103
#define STATE_MINUS   104
#define STATE_MINUS2  105
#define STATE_START   106
#define STATE_NOT     107
#define STATE_OPER    109

FILE *fp;

tListOfInstr insList;
Pmain_table table;

char *fname;
char *name_id;

int patotmp;

typedef struct{
    string *s;
    int type;
}expData;

typedef struct expListItem{
  expData *data;
  struct expListItem *next;
}tExpListItem;

typedef struct{
  struct expListItem *first;
  struct expListItem *last;
  struct expListItem *active;
}expList;

int checkExp(int isIF,expList *list,int add);
int declareID();
int print();
int input();
int fundeclare();
int fundef();
int checkFile();
int endScope();
int endFunction();
int endIF();
int checkWhile();
int scope(int ,int,int);
int parse();

void initExpData(expData *data);
void expListNext(expList *list);
void expFirst(expList *list);
void expNext(expList *list);
int insertExp(expList *list,expData *data);
void expListInit(expList *list);
int expType(expList *list);
char *expVal(expList *list);
int expLast(expList *list);
void expListFree(expList *L);
int expListLength(expList *list);
void sematika(int type,expList *list,int isIF);

void checkID(string *s);
int getTypeFromKW(int result);
void patov_destroy(tList L);

#endif
