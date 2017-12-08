/*
  Projekt: Prekladac jazyka IFJ17
  Peter Grofčík   xgrofc00
  Martin Fujaček  xfujac00
  Patrik Krajč    xkrajc17
  Rastislav Pôbiš xpobis00
*/

#ifndef __PRECEDENCE_H__
#define __PRECEDENCE_H__

#include <stdlib.h>
#include <stdio.h>
#include "error.h"
//#include "instrlist.h"
#include "parser.h"
#include <stdbool.h>

tListOfInstr insList;

/**
 * Jedna položka v zásobíku
 * Symbol - vkladaný token/neterminál
 * type - typ premennej
 * name - názov premennej
 * next - ďalšia položka v zásobníku
 */
typedef struct stack_item{
  int symbol;
  //int type;
  string *name;
  struct stack_item *next;
}stack_item_t;



/**
 * Typ zásobník
 * top - ukazatel na vrchol zásobníku
 */
typedef struct {
    stack_item_t *top;
}Stack;

void debug(Stack stack);
void retypecheck(Stack *stackT, int counter);//, int instruction);
void retypeIntToFloat(Stack *stackT, int opnd, int counter);
void gentape(Stack *stackT, int counter, int retType, int instruction);
int group_tokens(int token);
void stackInit(Stack *stack);
void push(Stack *stack, int symbol, string *name);
void pop(Stack *stack);
void multipop(Stack *stack, Stack *stackT, int n, int stat, Pmain_nod func);
int top(Stack *stack);
bool isTerm(int token);
int topTerm(Stack stack);
void pushLessThan(Stack *stack);
int opToInstr(int op);
int psa(expList *list, int stat, char *name, Pmain_nod func);
#endif
