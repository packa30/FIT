/*  @Project: IFJ17
    @author: xgrofc00@stud.fit.vutbr.cz
    @author: xkrajc17@stud.fit.vutbr.cz
    @author: xfujac00@stud.fit.vutbr.cz
    @author: xpobis00@stud.fit.vutbr.cz
*/

#ifndef __GENERATE_H__
#define __GENERATE_H__

#include<stdio.h>
#include<stdlib.h>
//#include"str.h"
//#include"error.h"
#include"instrlist.h"
#include"symtable.h"

#define TYPE_INT (2001)
#define TYPE_DOUBLE (1998)
#define TYPE_STRING (1999)
#define GLOBAL (25001)
#define LOCAL (25002)

typedef struct IF_present{
  char else_now[512];
  char end_now[512];
}*PIF_present;

typedef struct IF_stack{
  PIF_present *data;
  int top;
}*PIF_stack;

typedef struct CYCLE_present{
  char start_now[512];
  char end_now[512];
  tInstr instr;
  tListItem item;
  int temp;
}*PCYCLE_present;

typedef struct CYCLE_stack{
  PCYCLE_present *data;
  int top;
}*PCYCLE_stack;


PIF_stack IF_init();
void push_IF(PIF_stack stack,char *else_now, char *end_now);
void pop_IF(PIF_stack stack);
void IF_add(PIF_stack,char *present_func,char *temp,int tmp);

PCYCLE_stack CYCLE_init();
void push_CYCLE(PCYCLE_stack stack,char *start_now,char *end_now,int tmp,tInstr instr);
void pop_CYCLE(PCYCLE_stack stack);
void CYCLE_add(PCYCLE_stack stack,char *present_func,tInstr instr,int tmp,char *temp);

int generate(tListOfInstr list,Pmain_table table);
void arithmetic2(int type,char *param2);
void def_ret_func(Pmain_table table,char *present_func);
void add_params(Pmain_table table,char *scope,char *param,int type,char *present_func);
void add_arithmetic(Pmain_table table,char *scope, tInstr instr, char*present_func);

int ifs_whiles(tListItem item);
void defvar_tmp(tListItem item,char *scope,char *present_func,Pmain_table table);

void vypisinstrlist(tListOfInstr L);
#endif
