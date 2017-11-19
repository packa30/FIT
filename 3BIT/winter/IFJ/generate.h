/*  @Project: IFJ17
    @author: xgrofc00@stud.fit.vutbr.cz
    @author: xkrajc17@stud.fit.vutbr.cz
    @author: xfujac00@stud.fit.vutbr.cz
    @author: xpobis00@stud.fit.vutbr.cz
*/

#include<stdio.h>
#include<stdlib.h>
//#include"str.h"
//#include"error.h"
#include"instrlist.h"
#include"symtable.h"

#define TYPE_INT (1012)
#define TYPE_DOUBLE (1005)
#define TYPE_STRING (1018)
#define GLOBAL (25001)
#define LOCAL (25002)

typedef struct IF_present{
  char *else_now;
  char *end_now;
}*PIF_present;

typedef struct IF_stack{
  PIF_present *data;
  int top;
}*PIF_stack;

PIF_stack IF_init();
void push_IF(PIF_stack stack,char *else_now, char *end_now);
void pop_IF(PIF_stack stack);
void IF_add(PIF_stack,char *else_now,char *end_now,char *present_func,tInstr instr);

int generate(tListOfInstr list,Pmain_table table);
void arithmetic2(int type,char *param);
void def_ret_func(Pmain_table table,char *present_func);
void add_params(Pmain_table table,char *scope,char *param,int type,char *present_func);
void add_arithmetic(Pmain_table table,char *scope, tInstr instr, char*present_func);
