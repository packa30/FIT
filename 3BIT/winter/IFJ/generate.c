/*  @Project: IFJ17
    @author: xgrofc00@stud.fit.vutbr.cz
    @author: xkrajc17@stud.fit.vutbr.cz
    @author: xfujac00@stud.fit.vutbr.cz
    @author: xpobis00@stud.fit.vutbr.cz
*/

#include "generate.h"



int generate(tListOfInstr list,Pmain_table table){
  printf(".IFJcode17\r\n");
  printf("JUMP $$main\r\n");
  tListItem item = list->first;
  char *input;
  char *scope="LF";
  char *present_func;
  int temp=0;
  char *else_now=malloc(150);
  char *end_now=malloc(150);
  char *start_now=malloc(150);
  PIF_stack stack = IF_init();
  PCYCLE_stack stack2 = CYCLE_init();

  while(item != NULL){
    tInstr instr = item->Instruction;
    switch (instr->instTyp) {
      case I_DEFVAR:
        printf("DEFVAR %s@%s\r\n",scope,(char *)instr->addr1);
        break;
      case I_MOVE:
        printf("MOVE %s@%s ",scope,(char *)instr->addr1);
        add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
        printf("\r\n");
        break;
      case I_LABEL:
        temp = 0;
        present_func=(char *)instr->addr1;
        printf("LABEL $%s\r\nPUSHFRAME\r\nDEFVAR LF@%%retval\r\nMOVE LF@%%retval ",present_func);
        def_ret_func(table,present_func);
        break;
      case I_ADD:
        printf("ADD %s@%s ",scope,(char *)instr->addr1);
        add_arithmetic(table,scope,instr,present_func);
        break;
      case I_SUB:
        printf("SUB %s@%s ",scope,(char *)instr->addr1);
        add_arithmetic(table,scope,instr,present_func);
        break;
      case I_DIV:
        printf("DIV %s@%s ",scope,(char *)instr->addr1);
        add_arithmetic(table,scope,instr,present_func);
        break;
      case I_MUL:
        printf("MUL %s@%s ",scope,(char *)instr->addr1);
        add_arithmetic(table,scope,instr,present_func);
        break;
      case I_RETURN:
        printf("MOVE %s@%%retval ",scope);
        add_params(table,scope,(char *)instr->addr1,instr->type2,present_func);
        printf("\r\n");
        break;
      case I_ENDFUNC:
        printf("LABEL $%s$epilog\r\nPOPFRAME\r\nRETURN\r\n",present_func);
        break;
      case I_MAIN:
        temp=0;
        printf("LABEL $$main\r\n");
        present_func = (char *)instr->addr1;
        scope = "GF";
        break;
      case I_CALL:
        printf("CREATEFRAME\r\n");
        int calling=1;
        char *to_be_called = (char*)instr->addr1;
        item=item->nextItem;
        while(calling)
        {
          instr = item->Instruction;
          switch (instr->instTyp) {
            case I_DEFVAR:
              printf("DEFVAR TF@%s\r\n",(char *)instr->addr1);
              break;
            case I_MOVE:
              printf("MOVE TF@%s ",(char *)instr->addr1);
              add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
              printf("\r\n");
              break;
            case I_CALLEND:
              printf("CALL $%s\r\n", to_be_called);
              calling=0;
              break;
            default:
              break;
          }
          if(calling){item=item->nextItem;}
        }
        break;
      case I_MOVE_RETVAL:
        printf("MOVE %s@%s TF@%%retval\r\n",scope,(char *)instr->addr1 );
        break;
      case I_WRITE:
        printf("WRITE ");
        add_params(table,scope,(char *)instr->addr1,instr->type2,present_func);
        printf("\r\n");
        break;
      case I_IF:
        printf("DEFVAR %s@&tmpif%d\r\n",scope,temp);
        item=item->nextItem;
        instr=item->Instruction;
        switch (instr->instTyp) {
          case I_LT:
            printf("LT %s@&tmpif%d ",scope,temp);
            add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
            printf(" ");
            add_params(table,scope,(char *)instr->addr3,instr->type3,present_func);
            IF_add(stack,else_now,end_now,present_func,instr,"LT");
            printf("\r\n");
            printf("JUMPIFNEQ %s %s@&tmpif%d bool@true\r\n",else_now,scope,stack->top);
            break;
          case I_GT:
            printf("GT %s@&tmpif%d ",scope,temp);
            add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
            printf(" ");
            add_params(table,scope,(char *)instr->addr3,instr->type3,present_func);
            IF_add(stack,else_now,end_now,present_func,instr,"GT");
            printf("\r\n");
            printf("JUMPIFNEQ %s %s@&tmpif%d bool@true\r\n",else_now,scope,stack->top);
            break;
          case I_EQ:
            printf("EQ %s@&tmpif%d ",scope,temp);
            add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
            printf(" ");
            add_params(table,scope,(char *)instr->addr3,instr->type3,present_func);
            IF_add(stack,else_now,end_now,present_func,instr,"EQ");
            printf("\r\n");
            printf("JUMPIFNEQ %s %s@&tmpif%d bool@true\r\n",else_now,scope,stack->top);
            break;
          default:
            break;

        }
        temp++;
        break;
      case I_ELSE:
        printf("JUMP %s\r\n", stack->data[stack->top]->end_now);
        printf("LABEL %s\r\n",stack->data[stack->top]->else_now);
        break;
      case I_IFEND:
        printf("LABEL %s\r\n",stack->data[stack->top]->end_now);
        pop_IF(stack);
        break;
      case I_INPUT:
        switch (instr->type2) {
          case TYPE_INT:
            input = "int";
            break;
          case TYPE_DOUBLE:
            input = "float";
            break;
          case TYPE_STRING:
            input = "string";
            break;
          default:
            break;
        }
        printf("READ %s@%s %s\r\n",scope,(char *)instr->addr1,input);
        break;
      case I_CYCLE:
        printf("DEFVAR %s@&tmpcycle%d\r\n",scope,temp);
        item=item->nextItem;
        instr=item->Instruction;
        switch (instr->instTyp) {
          case I_LT:
            printf("LT %s@&tmpcycle%d ",scope,temp);
            add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
            printf(" ");
            add_params(table,scope,(char *)instr->addr3,instr->type3,present_func);
            CYCLE_add(stack2,start_now,end_now,present_func,instr,temp,"LT");
            printf("\r\n");
            printf("JUMPIFNEQ %s %s@&tmpcycle%d bool@true\r\n",end_now,scope,stack2->top);
            printf("LABEL %s\r\n",start_now);
            break;
          default:
            break;
        }
        temp++;
        break;
      case I_CYCLEEND:
        switch (stack2->data[stack2->top]->instr->instTyp) {
          case I_LT:
            printf("LT %s@&tmpcycle%d ",scope,stack2->data[stack2->top]->temp);
            add_params(table,scope,(char *)stack2->data[stack2->top]->instr->addr2,stack2->data[stack2->top]->instr->type2,present_func);
            printf(" ");
            add_params(table,scope,(char *)stack2->data[stack2->top]->instr->addr3,stack2->data[stack2->top]->instr->type3,present_func);
            printf("\n");
            printf("JUMPIFNEQ %s %s@&tmpcycle%d bool@false\r\n",stack2->data[stack2->top]->start_now,scope,stack2->data[stack2->top]->temp);
            pop_CYCLE(stack2);
            break;
          default:
            break;
        }
        break;
      default:
        break;
    }
    item=item->nextItem;
  }
  free(stack->data);
  free(stack);
  free(else_now);
  free(end_now);
  free(start_now);
  return true;
}
void arithmetic2(int type,char *param){
  char *changed_string;
  char *ho;
  ho = malloc(4);
    switch (type) {
      case TYPE_INT:
        printf("int@%s",param);
        break;
      case TYPE_DOUBLE:
        printf("float@%s",param);
        break;
      case TYPE_STRING:
        changed_string = malloc(1);
        int p=0;
        for(unsigned int i=0;i <= strlen(param)+1;i++){
          int found = false;
          for (int help=0;help<=32;help++ ) {
            if(param[i] == help){
              if(help<10)
                sprintf(ho,"0%d",help);
              else
                sprintf(ho,"%d",help);
              changed_string=realloc(changed_string, strlen(changed_string)+3);
              changed_string[p]='\\';
              changed_string[p+1]='0';
              changed_string[p+2]=ho[0];
              changed_string[p+3]=ho[1];
              p+=3;
              found = true;
              break;
            }
          }
          if(found != true){
            if(param[i] == 35 || param[i] == 92 )
            {
              changed_string=realloc(changed_string, strlen(changed_string)+3);
              changed_string[p]='\\';
              changed_string[p+1]='0';
              if(param[i] == 35){
                changed_string[p+2]='3';
                changed_string[p+3]='5';
              }
              else{
                changed_string[p+2]='9';
                changed_string[p+3]='2';
              }
              p+=3;
              found = true;
            }
            if(found != true){
              changed_string=realloc(changed_string,strlen(changed_string)+1);
              changed_string[p]=param[i];
            }
          }

          p+=1;
        }
        printf("string@%s",changed_string);
        free(changed_string);
        free(ho);
        break;
    }
}
void def_ret_func(Pmain_table table,char *present_func){
  int func = search_func_type(table,present_func);
  switch (func) {
    case TYPE_INT:
      printf("int@0\r\n");
      break;
    case TYPE_DOUBLE:
      printf("float@0.0\r\n");
      break;
    case TYPE_STRING:
      printf("string@\r\n");
      break;
  }
}
void add_params(Pmain_table table,char *scope,char *param,int type,char *present_func){
  if(!search_PATO(table, param,present_func)){
    if(!search_PEPO(table,param,present_func)){
      arithmetic2(type,param);
    }
    else{
        printf("%s@%s",scope,param);
    }
  }
  else{
    printf("%s@%s",scope,param);
  }
}
void add_arithmetic(Pmain_table table,char *scope, tInstr instr, char*present_func){
  add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
  printf(" ");
  add_params(table,scope,(char *)instr->addr3,instr->type3,present_func);
  printf("\r\n");
}

PIF_stack IF_init(){
  PIF_stack stack = malloc(sizeof(struct IF_stack));
  stack->data=malloc(sizeof(PIF_present));
  if(stack == NULL || stack->data==NULL){errors(INTERNAL);}
  stack->top=-1;
  return stack;
}

void push_IF(PIF_stack stack,char *else_now, char *end_now){
  stack->top++;
  stack->data[stack->top] = malloc(sizeof(PIF_present));
  stack->data[stack->top]->else_now=malloc(strlen(else_now)+1);
  stack->data[stack->top]->end_now=malloc(strlen(end_now)+1);
  if(stack->data[stack->top] == NULL || stack->data[stack->top]->else_now == NULL || stack->data[stack->top]->end_now == NULL){errors(INTERNAL);}
  strcpy(stack->data[stack->top]->else_now,else_now);
  strcpy(stack->data[stack->top]->end_now,end_now);
}
void pop_IF(PIF_stack stack){
  if(stack->top != -1){
    if(stack->data[stack->top] != NULL){
      if(stack->data[stack->top]->else_now != NULL)
        free(stack->data[stack->top]->else_now);
      if(stack->data[stack->top]->end_now != NULL)
        free(stack->data[stack->top]->end_now);
      free(stack->data[stack->top]);
    }
  }
  stack->top--;
}
void IF_add(PIF_stack stack,char *else_now,char *end_now,char *present_func,tInstr instr,char *temp){
  strcpy(else_now,"$");
  strcat(else_now,present_func);
  strcat(else_now,"$");
  strcat(else_now,(char *)instr->addr2);
  strcat(else_now,"$");
  strcat(else_now,temp);
  strcat(else_now,"$");
  strcat(else_now,(char *)instr->addr3);
  strcat(else_now,"$else");

  strcpy(end_now,"$");
  strcat(end_now,present_func);
  strcat(end_now,"$");
  strcat(end_now,(char *)instr->addr2);
  strcat(end_now,"$");
  strcat(end_now,temp);
  strcat(end_now,"$");
  strcat(end_now,(char *)instr->addr3);
  strcat(end_now,"$end");

  push_IF(stack,else_now,end_now);
}


PCYCLE_stack CYCLE_init(){
  PCYCLE_stack stack = malloc(sizeof(struct CYCLE_stack));
  stack->data=malloc(sizeof(PCYCLE_present));
  if(stack == NULL || stack->data==NULL){errors(INTERNAL);}
  stack->top=-1;
  return stack;
}
void push_CYCLE(PCYCLE_stack stack,char *start_now,char *end_now,int tmp,tInstr instr){
  stack->top++;
  stack->data[stack->top] = malloc(sizeof(PCYCLE_present));
  stack->data[stack->top]->start_now=malloc(strlen(start_now)+1);
  stack->data[stack->top]->end_now=malloc(strlen(end_now)+1);
  if(stack->data[stack->top] == NULL || stack->data[stack->top]->start_now == NULL || stack->data[stack->top]->end_now == NULL){errors(INTERNAL);}
  printf("\n%s\t%lu\n",start_now,strlen(start_now));
  printf("%s\t%lu",end_now,strlen(end_now));
  strcpy(stack->data[stack->top]->start_now,start_now);
  strcpy(stack->data[stack->top]->end_now,end_now);

  stack->data[stack->top]->temp=tmp;
  stack->data[stack->top]->instr=instr;
}
void pop_CYCLE(PCYCLE_stack stack){
  if(stack->top != -1){
    if(stack->data[stack->top] != NULL){
      if(stack->data[stack->top]->start_now != NULL)
        free(stack->data[stack->top]->start_now);
      if(stack->data[stack->top]->end_now != NULL)
        free(stack->data[stack->top]->end_now);
      //free(stack->data[stack->top]);
    }
  }
  stack->top--;
}
void CYCLE_add(PCYCLE_stack stack,char *start_now, char *end_now,char *present_func,tInstr instr,int tmp,char *temp){
  strcpy(start_now,"$");
  strcat(start_now,present_func);
  strcat(start_now,"$");
  strcat(start_now,(char *)instr->addr2);
  strcat(start_now,"$");
  strcat(start_now,temp);
  strcat(start_now,"$");
  strcat(start_now,(char *)instr->addr3);
  strcat(start_now,"$start");

  strcpy(end_now,"$");
  strcat(end_now,present_func);
  strcat(end_now,"$");
  strcat(end_now,(char *)instr->addr2);
  strcat(end_now,"$");
  strcat(end_now,temp);
  strcat(end_now,"$");
  strcat(end_now,(char *)instr->addr3);
  strcat(end_now,"$end");
  push_CYCLE(stack,start_now,end_now,tmp,instr);
}
