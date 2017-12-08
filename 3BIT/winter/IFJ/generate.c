/*  @Project: IFJ17
    @author: xgrofc00@stud.fit.vutbr.cz
    @author: xkrajc17@stud.fit.vutbr.cz
    @author: xfujac00@stud.fit.vutbr.cz
    @author: xpobis00@stud.fit.vutbr.cz
*/
#include<ctype.h>
#include "generate.h"

/**
 * Funkcia ktorá generuje medzikód IFJcode17 na štandartný výstup. Volá sa až po úspešnej
 * syntaktickej a sémantickej analýze.
 * @param   list    inštrukčná páska
 * @param   table   tabulka symbolov
 * @return          1
 */
int generate(tListOfInstr list,Pmain_table table){
  printf(".IFJcode17\r\n");
  printf("JUMP $$main\r\n");
  tListItem item_help;
  tListItem item = list->first;
  char *input;
  char *scope="LF";
  char *present_func;
  int temp=0;
  int while_do=false;
  /*char *else_now=malloc(150);
  char *end_now=malloc(150);
  char *start_now=malloc(150);*/
  int ifs_and_loop_in_f;
  PIF_stack stack = IF_init();
  PCYCLE_stack stack2 = CYCLE_init();
  tInstr instr;

  while(item != NULL){
    instr = item->Instruction;
    switch (instr->instTyp) {
      case I_DEFVAR:
        printf("DEFVAR %s@%s\r\n",scope,(char *)instr->addr1);
        switch (instr->type2) {
          case TYPE_INT:
            printf("MOVE %s@%s int@0\r\n",scope,(char *)instr->addr1);
            break;
          case TYPE_DOUBLE:
            printf("MOVE %s@%s float@0.0\r\n",scope,(char *)instr->addr1);
            break;
          case TYPE_STRING:
            printf("MOVE %s@%s string@\r\n",scope,(char *)instr->addr1);
            break;
          default:
            break;
        }
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
        if(item->nextItem != NULL){
          ifs_and_loop_in_f=ifs_whiles(item->nextItem);
        for (int i = 0; i < ifs_and_loop_in_f; i++) {
          printf("DEFVAR %s@&tmp%d\r\n",scope,i);
        }
        defvar_tmp(item->nextItem,scope,present_func,table);}
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
      case I_CONCAT:
        printf("CONCAT %s@%s ",scope,(char *)instr->addr1);
        add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
        printf(" ");
        add_params(table,scope,(char *)instr->addr3,instr->type3,present_func);
        printf("\r\n");
        break;
      case I_INT_TO_FLOAT:
        printf("INT2FLOAT %s@%s ",scope, (char *)instr->addr1);
        add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
        printf("\r\n");
        break;
      case I_FLOAT_TO_INT:
        printf("FLOAT2R2EINT %s@%s ",scope, (char *)instr->addr1);
        add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
        printf("\r\n");
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
        if(item->nextItem != NULL){
        ifs_and_loop_in_f=ifs_whiles(item->nextItem);
        for (int i = 0; i < ifs_and_loop_in_f; i++) {
          printf("DEFVAR %s@&tmp%d\r\n",scope,i);
        }
        defvar_tmp(item->nextItem,scope,present_func,table);}
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
      case I_STRLEN:
        printf("STRLEN %s@%s ",scope,(char *)instr->addr1);
        add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
        printf("\r\n");
        break;
      case I_ASC:
        printf("STRI2INT %s@%s ",scope,(char *)instr->addr1);
        add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
        printf(" ");
        add_params(table,scope,(char *)instr->addr3,instr->type3,present_func);
        printf("\r\n");
        break;
      case I_CHR:
        printf("INT2CHAR %s@%s ",scope,(char *)instr->addr1);
        add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
        printf("\r\n");
        break;
      case I_SUBSTR:
        printf("MOVE %s@&tmp%d ",scope,temp+1);
        add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
        printf("\r\n");
        printf("MOVE %s@&tmp%d ",scope,temp+2);
        add_params(table,scope,(char *)instr->addr3,instr->type3,present_func);
        printf("\r\n");

        if(item->nextItem->Instruction->instTyp != I_SUBSTREND){
          errors(INTERNAL);
        }
        item=item->nextItem;
        instr=item->Instruction;

        printf("LABEL $subchar$%s%d\r\n",(char *)instr->addr1,temp);
        printf("GETCHAR %s@&tmp%d ",scope,temp);
        add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
        printf(" ");
        printf("%s@&tmp%d\r\n",scope,temp+1);
        printf("CONCAT %s@%s %s@%s %s@&tmp%d\r\n",scope,(char *)instr->addr1,scope,(char *)instr->addr1,scope,temp);
        printf("ADD %s@&tmp%d %s@&tmp%d int@1\r\n",scope,temp+1,scope,temp+1);
        printf("LT %s@&tmp%d %s@&tmp%d %s@&tmp%d\r\n",scope,temp+3,scope,temp+1,scope,temp+2);
        printf("JUMPIFNEQ $subchar$%s%d %s@&tmp%d bool@false\r\n",(char *)instr->addr1,temp,scope,temp+3);
        temp+=4;
        break;
      case I_IF:
        //printf("DEFVAR %s@&tmpif%d\r\n",scope,temp);
        item=item->nextItem;
        instr=item->Instruction;
        switch (instr->instTyp) {
          case I_LT:
            printf("LT %s@&tmp%d ",scope,temp);
            add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
            printf(" ");
            add_params(table,scope,(char *)instr->addr3,instr->type3,present_func);
            IF_add(stack,present_func,"LT",temp);
            printf("\r\n");
            printf("JUMPIFNEQ %s %s@&tmp%d bool@true\r\n",stack->data[stack->top]->else_now,scope,temp);
            break;
          case I_GT:
            printf("GT %s@&tmp%d ",scope,temp);
            add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
            printf(" ");
            add_params(table,scope,(char *)instr->addr3,instr->type3,present_func);
            IF_add(stack,present_func,"GT",temp);
            printf("\r\n");
            printf("JUMPIFNEQ %s %s@&tmp%d bool@true\r\n",stack->data[stack->top]->else_now,scope,temp);
            break;
          case I_EQ:
            printf("EQ %s@&tmp%d ",scope,temp);
            add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
            printf(" ");
            add_params(table,scope,(char *)instr->addr3,instr->type3,present_func);
            IF_add(stack,present_func,"EQ",temp);
            printf("\r\n");
            printf("JUMPIFNEQ %s %s@&tmp%d bool@true\r\n",stack->data[stack->top]->else_now,scope,temp);
            break;
          case I_LT_EQ:
            printf("GT %s@&tmp%d ",scope,temp);
            add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
            printf(" ");
            add_params(table,scope,(char *)instr->addr3,instr->type3,present_func);
            IF_add(stack,present_func,"LTEQ",temp);
            printf("\r\n");
            printf("JUMPIFEQ %s %s@&tmp%d bool@true\r\n",stack->data[stack->top]->else_now,scope,temp);
            break;
          case I_GT_EQ:
            printf("LT %s@&tmp%d ",scope,temp);
            add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
            printf(" ");
            add_params(table,scope,(char *)instr->addr3,instr->type3,present_func);
            IF_add(stack,present_func,"GTEQ",temp);
            printf("\r\n");
            printf("JUMPIFEQ %s %s@&tmp%d bool@true\r\n",stack->data[stack->top]->else_now,scope,temp);
            break;
          case I_NOT_EQ:
            printf("EQ %s@&tmp%d ",scope,temp);
            add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
            printf(" ");
            add_params(table,scope,(char *)instr->addr3,instr->type3,present_func);
            IF_add(stack,present_func,"NOTEQ",temp);
            printf("\r\n");
            printf("JUMPIFEQ %s %s@&tmp%d bool@true\r\n",stack->data[stack->top]->else_now,scope,temp);
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
        //printf("DEFVAR %s@&tmpcycle%d\r\n",scope,temp);
        if(while_do != false){
          while_do = false;
          item=item->nextItem;
          instr=item->Instruction;
          switch (instr->instTyp) {
            case I_LT:
              printf("LT %s@&tmp%d ",scope,temp);
              add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
              printf(" ");
              add_params(table,scope,(char *)instr->addr3,instr->type3,present_func);
              CYCLE_add(stack2,present_func,instr,temp,"LT");
              printf("\r\n");
              printf("JUMPIFNEQ %s %s@&tmp%d bool@true\r\n",stack2->data[stack2->top]->end_now,scope,stack2->data[stack2->top]->temp);
              printf("LABEL %s\r\n",stack2->data[stack2->top]->start_now);
              break;
            case I_GT:
              printf("GT %s@&tmp%d ",scope,temp);
              add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
              printf(" ");
              add_params(table,scope,(char *)instr->addr3,instr->type3,present_func);
              CYCLE_add(stack2,present_func,instr,temp,"GT");
              printf("\r\n");
              printf("JUMPIFNEQ %s %s@&tmp%d bool@true\r\n",stack2->data[stack2->top]->end_now,scope,stack2->data[stack2->top]->temp);
              printf("LABEL %s\r\n",stack2->data[stack2->top]->start_now);
              break;
            case I_EQ:
              printf("EQ %s@&tmp%d ",scope,temp);
              add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
              printf(" ");
              add_params(table,scope,(char *)instr->addr3,instr->type3,present_func);
              CYCLE_add(stack2,present_func,instr,temp,"EQ");
              printf("\r\n");
              printf("JUMPIFNEQ %s %s@&tmp%d bool@true\r\n",stack2->data[stack2->top]->end_now,scope,stack2->data[stack2->top]->temp);
              printf("LABEL %s\r\n",stack2->data[stack2->top]->start_now);
              break;
            case I_LT_EQ:
              printf("GT %s@&tmp%d ",scope,temp);
              add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
              printf(" ");
              add_params(table,scope,(char *)instr->addr3,instr->type3,present_func);
              CYCLE_add(stack2,present_func,instr,temp,"LTEQ");
              printf("\r\n");
              printf("JUMPIFEQ %s %s@&tmp%d bool@true\r\n",stack2->data[stack2->top]->end_now,scope,stack2->data[stack2->top]->temp);
              printf("LABEL %s\r\n",stack2->data[stack2->top]->start_now);
              break;
            case I_GT_EQ:
              printf("LT %s@&tmp%d ",scope,temp);
              add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
              printf(" ");
              add_params(table,scope,(char *)instr->addr3,instr->type3,present_func);
              CYCLE_add(stack2,present_func,instr,temp,"GTEQ");
              printf("\r\n");
              printf("JUMPIFEQ %s %s@&tmp%d bool@true\r\n",stack2->data[stack2->top]->end_now,scope,stack2->data[stack2->top]->temp);
              printf("LABEL %s\r\n",stack2->data[stack2->top]->start_now);
              break;
            case I_NOT_EQ:
              printf("EQ %s@&tmp%d ",scope,temp);
              add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
              printf(" ");
              add_params(table,scope,(char *)instr->addr3,instr->type3,present_func);
              CYCLE_add(stack2,present_func,instr,temp,"NOTEQ");
              printf("\r\n");
              printf("JUMPIFEQ %s %s@&tmp%d bool@true\r\n",stack2->data[stack2->top]->end_now,scope,stack2->data[stack2->top]->temp);
              printf("LABEL %s\r\n",stack2->data[stack2->top]->start_now);
              break;
            default:
              break;
          }
          temp++;
          stack2->data[stack2->top]->item=item_help;
        }
        else{
          item_help = item->nextItem;
          while_do=true;
        }
        break;
      case I_CYCLEEND:
        item_help = stack2->data[stack2->top]->item;
        while(item_help->Instruction->instTyp != I_CYCLE){
          switch (item_help->Instruction->instTyp) {
            case I_MOVE:
              printf("MOVE %s@%s ",scope,(char *)item_help->Instruction->addr1);
              add_params(table,scope,(char *)item_help->Instruction->addr2,item_help->Instruction->type2,present_func);
              printf("\r\n");
              break;
            case I_ADD:
              printf("ADD %s@%s ",scope,(char *)item_help->Instruction->addr1);
              add_arithmetic(table,scope,item_help->Instruction,present_func);
              break;
            case I_SUB:
              printf("SUB %s@%s ",scope,(char *)item_help->Instruction->addr1);
              add_arithmetic(table,scope,item_help->Instruction,present_func);
              break;
            case I_DIV:
              printf("DIV %s@%s ",scope,(char *)item_help->Instruction->addr1);
              add_arithmetic(table,scope,item_help->Instruction,present_func);
              break;
            case I_MUL:
              printf("MUL %s@%s ",scope,(char *)item_help->Instruction->addr1);
              add_arithmetic(table,scope,item_help->Instruction,present_func);
              break;
            case I_CONCAT:
              printf("CONCAT %s@%s ",scope,(char *)item_help->Instruction->addr1);
              add_params(table,scope,(char *)item_help->Instruction->addr2,item_help->Instruction->type2,present_func);
              printf(" ");
              add_params(table,scope,(char *)item_help->Instruction->addr3,item_help->Instruction->type3,present_func);
              printf("\r\n");
              break;
            case I_INT_TO_FLOAT:
              printf("INT2FLOAT %s@%s ",scope, (char *)instr->addr1);
              add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
              printf("\r\n");
              break;
            case I_FLOAT_TO_INT:
              printf("FLOAT2INT %s@%s ",scope, (char *)instr->addr1);
              add_params(table,scope,(char *)instr->addr2,instr->type2,present_func);
              printf("\r\n");
              break;
            default:
              break;
          }
          item_help=item_help->nextItem;
        }
        item_help=NULL;
        switch (stack2->data[stack2->top]->instr->instTyp) {
          case I_LT:
            printf("LT %s@&tmp%d ",scope,stack2->data[stack2->top]->temp);
            add_params(table,scope,(char *)stack2->data[stack2->top]->instr->addr2,stack2->data[stack2->top]->instr->type2,present_func);
            printf(" ");
            add_params(table,scope,(char *)stack2->data[stack2->top]->instr->addr3,stack2->data[stack2->top]->instr->type3,present_func);
            printf("\n");
            printf("JUMPIFNEQ %s %s@&tmp%d bool@false\r\n",stack2->data[stack2->top]->start_now,scope,stack2->data[stack2->top]->temp);
            printf("LABEL %s\r\n",stack2->data[stack2->top]->end_now);
            pop_CYCLE(stack2);
            break;
          case I_GT:
            printf("GT %s@&tmp%d ",scope,stack2->data[stack2->top]->temp);
            add_params(table,scope,(char *)stack2->data[stack2->top]->instr->addr2,stack2->data[stack2->top]->instr->type2,present_func);
            printf(" ");
            add_params(table,scope,(char *)stack2->data[stack2->top]->instr->addr3,stack2->data[stack2->top]->instr->type3,present_func);
            printf("\n");
            printf("JUMPIFNEQ %s %s@&tmp%d bool@false\r\n",stack2->data[stack2->top]->start_now,scope,stack2->data[stack2->top]->temp);
            printf("LABEL %s\r\n",stack2->data[stack2->top]->end_now);
            pop_CYCLE(stack2);
            break;
          case I_EQ:
            printf("EQ %s@&tmp%d ",scope,stack2->data[stack2->top]->temp);
            add_params(table,scope,(char *)stack2->data[stack2->top]->instr->addr2,stack2->data[stack2->top]->instr->type2,present_func);
            printf(" ");
            add_params(table,scope,(char *)stack2->data[stack2->top]->instr->addr3,stack2->data[stack2->top]->instr->type3,present_func);
            printf("\n");
            printf("JUMPIFNEQ %s %s@&tmp%d bool@false\r\n",stack2->data[stack2->top]->start_now,scope,stack2->data[stack2->top]->temp);
            printf("LABEL %s\r\n",stack2->data[stack2->top]->end_now);
            pop_CYCLE(stack2);
            break;
          case I_LT_EQ:
            printf("GT %s@&tmp%d ",scope,stack2->data[stack2->top]->temp);
            add_params(table,scope,(char *)stack2->data[stack2->top]->instr->addr2,stack2->data[stack2->top]->instr->type2,present_func);
            printf(" ");
            add_params(table,scope,(char *)stack2->data[stack2->top]->instr->addr3,stack2->data[stack2->top]->instr->type3,present_func);
            printf("\n");
            printf("JUMPIFEQ %s %s@&tmp%d bool@false\r\n",stack2->data[stack2->top]->start_now,scope,stack2->data[stack2->top]->temp);
            printf("LABEL %s\r\n",stack2->data[stack2->top]->end_now);
            pop_CYCLE(stack2);
            break;
          case I_GT_EQ:
            printf("LT %s@&tmp%d ",scope,stack2->data[stack2->top]->temp);
            add_params(table,scope,(char *)stack2->data[stack2->top]->instr->addr2,stack2->data[stack2->top]->instr->type2,present_func);
            printf(" ");
            add_params(table,scope,(char *)stack2->data[stack2->top]->instr->addr3,stack2->data[stack2->top]->instr->type3,present_func);
            printf("\n");
            printf("JUMPIFEQ %s %s@&tmp%d bool@false\r\n",stack2->data[stack2->top]->start_now,scope,stack2->data[stack2->top]->temp);
            printf("LABEL %s\r\n",stack2->data[stack2->top]->end_now);
            pop_CYCLE(stack2);
            break;
          case I_NOT_EQ:
            printf("EQ %s@&tmp%d ",scope,stack2->data[stack2->top]->temp);
            add_params(table,scope,(char *)stack2->data[stack2->top]->instr->addr2,stack2->data[stack2->top]->instr->type2,present_func);
            printf(" ");
            add_params(table,scope,(char *)stack2->data[stack2->top]->instr->addr3,stack2->data[stack2->top]->instr->type3,present_func);
            printf("\n");
            printf("JUMPIFEQ %s %s@&tmp%d bool@false\r\n",stack2->data[stack2->top]->start_now,scope,stack2->data[stack2->top]->temp);
            printf("LABEL %s\r\n",stack2->data[stack2->top]->end_now);
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
  free(stack2->data);
  free(stack2);
  free(stack->data);
  free(stack);
  /*free(else_now);
  free(end_now);
  free(start_now);*/
  return true;
}

/**
 * //TODO
 * @param   type    datový typ premennej
 * @param   param   hodnota premennej
 */
void arithmetic2(int type,char *param){
  char changed_string[2048];
  char *ho;
  ho = malloc(4);

  int p=0;
    switch (type) {
      case TYPE_INT:
        printf("int@%s",param);
        break;
      case TYPE_DOUBLE:
        printf("float@%s",param);
        break;
      case TYPE_STRING:
      //changed_string = malloc(1);
        for(unsigned int i=0;i < strlen(param);i++){
          int found = false;
          for (int help=0;help<=32;help++ ) {
            if(param[i] == help){
              if(help<10)
                sprintf(ho,"0%d",help);
              else
                sprintf(ho,"%d",help);
              //changed_string=(char *)realloc(changed_string, strlen(changed_string)+4);
              changed_string[p]='\\';
              changed_string[p+1]='0';
              changed_string[p+2]=ho[0];
              changed_string[p+3]=ho[1];
              p+=4;
              found = true;
              break;
            }
          }
          if(found != true){
            if(param[i] == 35 || (param[i] == 92 && !isdigit(param[i+1])) )
            {
              //changed_string=(char *)realloc(changed_string, strlen(changed_string)+4);
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
              p+=4;
              found = true;
            }
            if(found != true){
              //changed_string=(char *)realloc(changed_string,strlen(changed_string)+1);
              //changed_string[p]='k';
              changed_string[p]=param[i];
              //changed_string[p]=param[i];
              //strncpy(&changed_string[p],&param[i],1);
              p+=1;
            }
          }
        }
        //changed_string=realloc(changed_string,strlen(changed_string)+1);
        changed_string[p]='\0';

        printf("string@%s",changed_string);
        //free(changed_string);
        break;
      default:
        break;
    }
    free(ho);
    //free(param);
}

/**
 * Funkcia nastaví implicitnú návratovú hodnotu funckie podľa typu
 * @param   table   tabuľka symbolov
 * @param   present_func  funkcia ktorej hodnotu definujeme
 */
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

/**
 * //TODO
 * @param   table         tabuľka symbolov
 * @param   scope         aktuálny rámec, v ktorom existuje premenná
 * @param   param         názov premennej
 * @param   type          dátový typ premennej
 * @param   present_func  funkcia, v ktorej sa premenná nachádza
 */
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

/**
 * Funkcia inicializuje zásobník pre podmienky. Alokuje pamäť a nastaví index vrcholu na -1
 * @return  ukazovatel na zásobník
 */
PIF_stack IF_init(){
  PIF_stack stack = malloc(sizeof(struct IF_stack));
  stack->data=malloc(sizeof(PIF_present));
  if(stack == NULL || stack->data==NULL){errors(INTERNAL);}
  stack->top=-1;
  return stack;
}

/**
 * Funkcia na vkladanie na vrchol zásobníka. Posunie vrchol a vloží položku na tento index.
 * @param   stack   ukazateľ na zásobník kam chceme vkladať
 * @param   else_now
 * @param   end_now
 */
void push_IF(PIF_stack stack,char *else_now, char *end_now){
  stack->top++;
  stack->data[stack->top] = malloc(sizeof(PIF_present));
  //stack->data[stack->top]->else_now=malloc(strlen(else_now)+1);
  //stack->data[stack->top]->end_now=malloc(strlen(end_now)+1);
  if(stack->data[stack->top] == NULL ){errors(INTERNAL);}
  strcpy(stack->data[stack->top]->else_now,else_now);
  strcpy(stack->data[stack->top]->end_now,end_now);
}

/**
 * Funkcia odoberie položku z vrcholu zásobníka. Uvoľní ju z pamäte a zmení index vrcholu
 * @param   stack   zásobník z ktorého odoberáme položku
 */
void pop_IF(PIF_stack stack){
  if(stack->top != -1){
    if(stack->data[stack->top] != NULL){
      /*if(stack->data[stack->top]->else_now != NULL)
        free(stack->data[stack->top]->else_now);
      if(stack->data[stack->top]->end_now != NULL)
        free(stack->data[stack->top]->end_now);*/
      free(stack->data[stack->top]);
    }
  }
  stack->top--;
}

/**
 * //TODO
 * @param   stack         zásobník kam budeme vkladať
 * @param   present_func  funkcia v ktorej sa nachádzame
 * @param   temp          inštrukcia porovnania
 * @param   tmp           počítadlo premenných
 */
void IF_add(PIF_stack stack,char *present_func,char *temp,int tmp){
  char else_n[150];
  char end[150];
  char ho[4];
  sprintf(ho,"%d",tmp);
  strcpy(else_n,"$");
  strcat(else_n,present_func);
  //strcat(else_n,"$");
  //strcat(else_n,(char *)instr->addr2);
  strcat(else_n,"$");
  strcat(else_n,temp);
  strcat(else_n,"$else");
  strcat(else_n,"$");
  strcat(else_n,ho);

//  strcat(else_n,(char *)instr->addr3);

  strcpy(end,"$");
  strcat(end,present_func);
  //strcat(end,"$");
  //strcat(end,(char *)instr->addr2);
  strcat(end,"$");
  strcat(end,temp);
  strcat(end,"$end");
  strcat(end,"$");
  strcat(end,ho);
  //strcat(end,(char *)instr->addr3);

  push_IF(stack,else_n,end);
}

/**
 * Funkcia inicializuje zásobník pre cykly. Alokuje pamäť a nastaví index vrcholu na -1
 * @return  ukazovatel na zásobník
 */
PCYCLE_stack CYCLE_init(){
  PCYCLE_stack stack = malloc(sizeof(struct CYCLE_stack));
  stack->data=malloc(sizeof(PCYCLE_present));
  if(stack == NULL || stack->data==NULL){errors(INTERNAL);}
  stack->top=-1;
  return stack;
}

/**
 * Funkcia na vkladanie na vrchol zásobníka. Posunie vrchol a vloží položku na tento index.
 * @param   stack     ukazateľ na zásobník kam chceme vkladať
 * @param   start_now
 * @param   end_now
 * @param   tmp
 * @param   instr     typ inštrukcie
 */
void push_CYCLE(PCYCLE_stack stack,char *start_now,char *end_now,int tmp,tInstr instr){
  stack->top++;
  stack->data[stack->top] = malloc(sizeof(PCYCLE_present));
  if(stack->data[stack->top] == NULL){errors(INTERNAL);}
  //printf("\n%s\t%lu\n",start_now,strlen(start_now));
  strcpy(stack->data[stack->top]->start_now,start_now);
  strcpy(stack->data[stack->top]->end_now,end_now);
  stack->data[stack->top]->temp=tmp;
  stack->data[stack->top]->instr=instr;
}

/**
 * Funkcia odoberie položku z vrcholu zásobníka. Uvoľní ju z pamäte a zmení index vrcholu
 * @param   stack   zásobník z ktorého odoberáme položku
 */
void pop_CYCLE(PCYCLE_stack stack){
  if(stack->top != -1){
    if(stack->data[stack->top] != NULL){
      free(stack->data[stack->top]);
    }
  }
  stack->top--;
}

/**
 * //TODO
 * @param   stack         zásobník kam budeme vkladať
 * @param   present_func  funkcia v ktorej sa nachádzame
 * @param   temp          inštrukcia porovnania
 * @param   tmp           počítadlo premenných
 * @param   instr         typ inštrukcie
 */
void CYCLE_add(PCYCLE_stack stack,char *present_func,tInstr instr,int tmp,char *temp){
  char start[150];
  char end[150];
  char ho[4];
  sprintf(ho,"%d",tmp);

  strcpy(start,"$");
  strcat(start,present_func);
  //strcat(start,"$");
  //strcat(start,(char *)instr->addr2);
  strcat(start,"$");
  strcat(start,temp);
  strcat(start,"$start");
  strcat(start,"$");
  strcat(start,ho);
  //strcat(start,(char *)instr->addr3);

  strcpy(end,"$");
  strcat(end,present_func);
  //strcat(end,"$");
  //strcat(end,(char *)instr->addr2);
  strcat(end,"$");
  strcat(end,temp);
  strcat(end,"$end");
  strcat(end,"$");
  strcat(end,ho);

  //strcat(end,(char *)instr->addr3);
  push_CYCLE(stack,start,end,tmp,instr);
}

/** //TODO
 * @param   item   položka z inštrukčnej pásky
 * @return         počet zanorení
 */
int ifs_whiles(tListItem item)
{
  tListItem item2 = item;
  tInstr instr = item2->Instruction;
  int k=0;
  while (item2 != NULL){
    if(instr->instTyp == I_ENDFUNC)
      break;
    if(instr->instTyp == I_IF || instr->instTyp == I_CYCLE){
      k++;
    }
    if(instr->instTyp == I_SUBSTR){
      k+=4;
    }
    item2=item2->nextItem;
    if(item2 != NULL)
      instr=item2->Instruction;

  }
  return k;
}

/**
 * Funkcia pre generovanie definovania premennej na rámci
 * @param   item          položka z inštrukčnej pásky
 * @param   scope         požadovaný rámec, v ktorom definujeme premennú
 * @param   present_func  funkcia v ktorej sa nachádzame
 * @param   table         tabuľka symbolov
 */
void defvar_tmp(tListItem item,char *scope,char *present_func,Pmain_table table){
  tListItem item2 = item;
  tInstr instr = item2->Instruction;
  while(item2 != NULL){
    if(instr->instTyp == I_ENDFUNC)
      break;
    if(instr->instTyp == I_DEFTMP){
      printf("DEFVAR %s@%s\r\n",scope,(char *)instr->addr1);
      insert_generate_token(table,(char *)instr->addr1,present_func);
    }
    item2=item2->nextItem;
    if(item2 != NULL)
      instr=item2->Instruction;
  }

}

/**
 * Ladiaca funkcia pre výpis inštrukčnej pásky. Vypisuje jej obsah.
 * @param   L   inštrukčná páska
 */
void vypisinstrlist(tListOfInstr L){
  tListItem item = L->first;
  while(item != NULL){
    printf("%i\t%s\t%s\t%s\n",item->Instruction->instTyp,(char *)item->Instruction->addr1,(char *)item->Instruction->addr2,(char *)item->Instruction->addr3);
    item=item->nextItem;
  }
}
