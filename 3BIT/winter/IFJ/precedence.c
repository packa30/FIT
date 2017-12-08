/*
  Projekt: Prekladac jazyka IFJ17
  Peter Grofčík   xgrofc00
  Martin Fujaček  xfujac00
  Patrik Krajč    xkrajc17
  Rastislav Pôbiš xpobis00
*/

#include "precedence.h"

/* Tabulka pre precedenčnú syntaktickú analýzu pre výrazy*/
const char prectab[8][8]={
  /* OP1 OP2 OP3  (   )  REL  i   $*/
    {'>','<','<','<','>','>','<','>'}, //OP1 +,-
    {'>','>','>','<','>','>','<','>'}, //OP2 *,/
    {'>','<','>','<','>','>','<','>'}, // OP3 idiv
    {'<','<','<','<','=','<','<',' '}, // (
    {'>','>','>',' ','>','>',' ','>'}, // )
    {'<','<','<','<','>','>','<','>'}, // REL
    {'>','>','>',' ','>','>',' ','>'}, // i
    {'<','<','<','<',' ','<','<','1'} // $
};
// globálna inštrukčná páska
extern tListOfInstr *instr_list;

/**
 * @brief Funkcia mapuje podobné tokeny do požadovanej skupiny. Napr. operátory + a - majú
 * v tabulke rovnaké hodnoty -> zlúčia sa do OP1 atd. a tabulka neobsahuje zbytočne veľa prvkov.
 * @param token Token ktorý chceme spracovať
 * @return      Vracia index do precedenčnej tabulky.
 */
int group_tokens(int token){
  switch(token){
    case OP_PLUS:
    case OP_MINUS:
      return 0; break;  // OP1
    case OP_MUL:
    case OP_DIV:
      return 1; break;  // OP2
    case OP_IDIV:
      return 2; break;
    case PAR_LEFT:
      return 3; break;  // (
    case PAR_RIGHT:
      return 4; break;  // )
    case OP_EQ:
    case OP_LOWER:
    case OP_BIGGER:
    case OP_LOWEREQ:
    case OP_BIGGEREQ:
    case OP_NOTEQ:
      return 5; break;  // REL
    case IDENTIFY:
    case TINTEGER:
    case TDOUBLE:
    case TSTRING:
      return 6; break;  // i
    case KW_THEN:
    case -1:  //EOL
      return 7; break;  // $
    default:
      return 10;
  }
}

/**
 * Inicializácia zásobníka. Nastaví vrchol na NULL.
 * @param stack Zásobník, ktorý chceme inicializovať
 */
void stackInit(Stack *stack){
    stack->top = NULL;
}

/**
 * Uvolnenie zásobníka. Odstráni z pamäte.
 * @param stack Zásobník, ktorý chceme uvoľniť
 */
void stackFree(Stack *stack){
  while(stack->top != NULL){
    pop(stack);
  }
}

/**
 * Funkcia vkladá symbol na zásobník.
 * @param   stack   zásobník kam chceme vkladať
 * @param   symbol  Vkladaný symbol
 * @param   name    vkladaný string, pracovný ho nepotrebuje tak ho volám s NULL
 */
void push(Stack *stack, int symbol, string *name){
  if(name==NULL){
    stack_item_t *tmp = malloc(sizeof(stack_item_t));
    if (tmp == NULL)
      errors(INTERNAL);
    tmp->symbol=symbol;
    tmp->next=stack->top;
    stack->top=tmp;
  }else {
    stack_item_t *tmp = malloc(sizeof(stack_item_t));
    if (tmp == NULL)
      errors(INTERNAL);
    tmp->name=malloc(sizeof(string));
    if(tmp->name==NULL)
      errors(INTERNAL);
    strInit(tmp->name);
    strCopyString(tmp->name, name);
    tmp->symbol=symbol;
    tmp->next=stack->top;
    stack->top=tmp;
  }
}

/**
 * Funkcia odoberie položku z vrcholu a uvoľní ju z pamäte.
 * @param   stack   zásobník z ktorého popujeme
 */
void pop(Stack *stack){
  if(stack->top!=NULL){
    stack_item_t *tmp = stack->top;
    stack->top=tmp->next;

    if(tmp !=NULL)
    free(tmp);
  }
}

/**
 * Funkcia ktorá generuje inštrukcie do pásky
 * @param   stackT      zásobník s pomocnými premennými
 * @param   counter     počítadlo pomocných premenných
 * @param   retType     typ výslednej premennej
 * @param   instruction vkladaná inštrukcia
 */
void gentape(Stack *stackT, int counter, int retType, int instruction){
  stack_item_t *opnd1 = malloc(sizeof(stack_item_t));
  stack_item_t *opnd2 = malloc(sizeof(stack_item_t));
  if (opnd1==NULL || opnd2==NULL)
    errors(INTERNAL);
  opnd1->name=malloc(sizeof(string));
  if (opnd1->name==NULL)
    errors(INTERNAL);
  strInit(opnd1->name);
  opnd2->name=malloc(sizeof(string));
  if (opnd2->name==NULL)
    errors(INTERNAL);
  strInit(opnd2->name);

  strCopyString(opnd2->name, stackT->top->name);
  opnd2->symbol=stackT->top->symbol;
  pop(stackT);

  strCopyString(opnd1->name, stackT->top->name);
  opnd1->symbol=stackT->top->symbol;
  pop(stackT);

  switch (instruction){
    case I_LT:
    case I_LT_EQ:
    case I_GT:
    case I_GT_EQ:
    case I_EQ:
    case I_NOT_EQ:
      instr_add(insList, instruction, NULL, opnd1->name->str, opnd2->name->str, opnd1->symbol, opnd2->symbol);
      push(stackT, retType, opnd1->name);
      strFree(opnd1->name);
      strFree(opnd2->name);
      free(opnd1);
      free(opnd2);
      return;
    break;
    default:
    break;
  }

  string *s = malloc(sizeof(string));
  if (s==NULL)
    errors(INTERNAL);
  strInit(s);
  char var[64];
  int i=0;
  sprintf(var,"&tmpv%d", counter);
  while(var[i]!='\0'){
    strAddChar(s, var[i]);
    i++;
  }

  instr_add(insList, I_DEFTMP, var, NULL, NULL, 0, 0);
  instr_add(insList, instruction, var, opnd1->name->str, opnd2->name->str, opnd1->symbol, opnd2->symbol);
  push(stackT, retType, s);

  strFree(opnd1->name);
  strFree(opnd2->name);
  strFree(s);
  free(opnd1);
  free(opnd2);

}

/**
 * Funkcia pretypuje integer na float.
 * @param   stackT  zásobník s pomocnými premennými
 * @param   opnd    operand, ktorý chceme pretypovať
 * @param   counter počítadlo pomocných premenných
 */
void retypeIntToFloat(Stack *stackT, int opnd, int counter){
  stack_item_t *opnd1 = malloc(sizeof(stack_item_t));
  stack_item_t *opnd2 = malloc(sizeof(stack_item_t));
  if (opnd1==NULL || opnd2==NULL)
    errors(INTERNAL);
  opnd1->name=malloc(sizeof(string));
  if (opnd1->name==NULL)
    errors(INTERNAL);
  strInit(opnd1->name);
  opnd2->name=malloc(sizeof(string));
  if (opnd2->name==NULL)
    errors(INTERNAL);
  strInit(opnd2->name);

  strCopyString(opnd2->name, stackT->top->name);
  opnd2->symbol=stackT->top->symbol;
  pop(stackT);
  strCopyString(opnd1->name, stackT->top->name);
  opnd1->symbol=stackT->top->symbol;
  pop(stackT);

  string *s = malloc(sizeof(string));
  if (s==NULL)
    errors(INTERNAL);
  strInit(s);
  char var[64];
  int i=0;
  sprintf(var,"&tmpv%d", counter);
  while(var[i]!='\0'){
    strAddChar(s, var[i]);
    i++;
  }
  if (opnd==1) {
    instr_add(insList, I_DEFTMP, var, NULL, NULL, 0, 0);
    instr_add(insList, I_INT_TO_FLOAT, var, opnd2->name->str, NULL, opnd2->symbol, 0);
    push(stackT, opnd1->symbol, opnd1->name);
    push(stackT, TDOUBLE, s);
  } else {
      instr_add(insList, I_DEFTMP, var, NULL, NULL, 0, 0);
      instr_add(insList, I_INT_TO_FLOAT, var, opnd1->name->str, NULL, opnd1->symbol, 0);
      push(stackT, TDOUBLE, s);
      push(stackT, opnd2->symbol, opnd2->name);
  }
  strFree(s);
  strFree(opnd1->name);
  strFree(opnd2->name);
  free(opnd1);
  free(opnd2);
}

/**
 * Funkcia na pretypovanie float na integer. To isté čo vyššie ale naopak
 * @param   stackT  zásobník s pomocnými premennými
 * @param   opnd    operand, ktorý chceme pretypovať
 * @param   counter počítadlo pomocných premenných
 */
 void retypeFloatToInt(Stack *stackT, int opnd, int counter){
   stack_item_t *opnd1 = malloc(sizeof(stack_item_t));
   stack_item_t *opnd2 = malloc(sizeof(stack_item_t));
   if (opnd1==NULL || opnd2==NULL)
     errors(INTERNAL);
   opnd1->name=malloc(sizeof(string));
   if (opnd1->name==NULL)
     errors(INTERNAL);
   strInit(opnd1->name);

   opnd2->name=malloc(sizeof(string));
   if (opnd2->name==NULL)
     errors(INTERNAL);
   strInit(opnd2->name);

   strCopyString(opnd2->name, stackT->top->name);
   opnd2->symbol=stackT->top->symbol;
   pop(stackT);
   if (stackT->top!=NULL) {
     /* code */
     strCopyString(opnd1->name, stackT->top->name);
     opnd1->symbol=stackT->top->symbol;
     pop(stackT);
   }

   string *s = malloc(sizeof(string));
   if (s==NULL)
     errors(INTERNAL);
   strInit(s);
   char var[64];
   int i=0;
   sprintf(var,"&tmpv%d", counter);
   while(var[i]!='\0'){
     strAddChar(s, var[i]);
     i++;
   }
   if (opnd==1) {
     instr_add(insList, I_DEFTMP, var, NULL, NULL, 0, 0);
     instr_add(insList, I_FLOAT_TO_INT, var, opnd2->name->str, NULL, opnd2->symbol, 0);
     push(stackT, opnd1->symbol, opnd1->name);
     push(stackT, TINTEGER, s);
   } else {
       instr_add(insList, I_DEFTMP, var, NULL, NULL, 0, 0);
       instr_add(insList, I_FLOAT_TO_INT, var, opnd1->name->str, NULL, opnd1->symbol, 0);
       push(stackT, TINTEGER, s);
       push(stackT, opnd2->symbol, opnd2->name);
   }
   strFree(s);
   strFree(opnd1->name);
   strFree(opnd2->name);
   free(opnd1);
   free(opnd2);
 }

/**
 * Funkcia odoberie a odstráni n položiek zo zásobníka a volá funkciu pre generovanie inštrukcii.
 * Overuje typovú kompatibilitu operandov, ošetruje delenie nulou.
 * @param   stack   pracovný zásobník precedenčky, z neho popujeme n-krat
 * @param   stackT  zásobník s pomocnými premennými
 * @param   n       počet odoberaných položiek
 * @param   stat    príkaz, s ktorým sa volá precedenčka
 * @param   func    názov funkcie v ktorej sa nachádzame
 */
void multipop(Stack *stack, Stack *stackT, int n, int stat, Pmain_nod func){
  static int counter=1;
  // popnem jeden a zistím aký je tam operátor, ak tam je E tak redukujem (E)!!!
  pop(stack);
  n--;
  int vrch = top(stack);
  PSymbol variab1 =NULL;
  PSymbol  variab2=NULL;

  if (stackT->top->symbol==IDENTIFY || search_PEPO(table,stackT->top->name->str,fname)){
    if(search_PATO(table,stackT->top->name->str,fname))
      variab1= STsearch(func->func_tree->Root, stackT->top->name->str);
    if(variab1 == NULL){
      variab1 = varfromlist(func, stackT->top->name->str);
    }
  }
  if(stackT->top->next!=NULL){
    if(stackT->top->next->symbol==IDENTIFY || search_PEPO(table,stackT->top->next->name->str,fname)){
      if(search_PATO(table,stackT->top->next->name->str,fname))
        variab2= STsearch(func->func_tree->Root, stackT->top->next->name->str);
      if(variab2 == NULL){
        variab2 = varfromlist(func, stackT->top->next->name->str);
      }
    }
  }
  // podľa operátora generujeme inštrukciu, pričom kontroluje typovú kompatibilitu

  switch (vrch) {
    // operátor +, súčet a konkatenácia
    case OP_PLUS:
      if (variab1==NULL&&variab2==NULL) {
        if(stackT->top->symbol==TSTRING && stackT->top->next->symbol==TSTRING)
          gentape(stackT, counter++, TSTRING, I_CONCAT);
        else {
          if ((stackT->top->symbol==TSTRING && stackT->top->next->symbol!=TSTRING) || (stackT->top->symbol!=TSTRING && stackT->top->next->symbol==TSTRING))
            errors(SPAR); // chyba typovej kompatibility, nemôže byž len jeden String

          if (stackT->top->symbol < stackT->top->next->symbol)
            retypeIntToFloat(stackT, 2, counter++);
          if (stackT->top->symbol > stackT->top->next->symbol)
            retypeIntToFloat(stackT, 1, counter++);

          if (stackT->top->symbol==TDOUBLE)
            gentape(stackT, counter++, TDOUBLE, opToInstr(vrch));
          if (stackT->top->symbol==TINTEGER)
            gentape(stackT, counter++, TINTEGER, opToInstr(vrch));
        }
      }

      if (variab1==NULL&&variab2!=NULL) {
        if(stackT->top->symbol==TSTRING && variab2->type==TSTRING)
          gentape(stackT, counter++, TSTRING, I_CONCAT);
        else {
          if ((stackT->top->symbol==TSTRING && variab2->type!=TSTRING) || (stackT->top->symbol!=TSTRING && variab2->type==TSTRING))
            errors(SPAR);

          if (stackT->top->symbol < variab2->type)
            retypeIntToFloat(stackT, 2, counter++);
          if (stackT->top->symbol > variab2->type)
            retypeIntToFloat(stackT, 1, counter++);


          if (stackT->top->symbol==TDOUBLE)
            gentape(stackT, counter++, TDOUBLE, opToInstr(vrch));
          if (stackT->top->symbol==TINTEGER)
            gentape(stackT, counter++, TINTEGER, opToInstr(vrch));
        }
      }

      if (variab1!=NULL&&variab2==NULL) {
        if(variab1->type==TSTRING && stackT->top->next->symbol==TSTRING)
          gentape(stackT, counter++, TSTRING, I_CONCAT);

        else {
          if ((variab1->type==TSTRING && stackT->top->next->symbol!=TSTRING) || (variab1->type!=TSTRING && stackT->top->next->symbol==TSTRING))
            errors(SPAR);

          if (variab1->type > stackT->top->next->symbol)
            retypeIntToFloat(stackT, 1, counter++);
          if (variab1->type < stackT->top->next->symbol)
            retypeIntToFloat(stackT, 2, counter++);

          // ak je na vrchole identifikátor, nedošlo k pretypovaniu a použijeme typ premennej, ak sa pretypovalo, na vrchole už bude nový typ
          if(stackT->top->symbol == IDENTIFY){
            if (variab1->type==TDOUBLE)
             gentape(stackT, counter++, TDOUBLE, opToInstr(vrch));
            if(variab1->type==TINTEGER)
             gentape(stackT, counter++, TINTEGER, opToInstr(vrch));
          }
          else {
           if (stackT->top->symbol==TDOUBLE)
            gentape(stackT, counter++, TDOUBLE, opToInstr(vrch));
           if(stackT->top->symbol==TINTEGER)
            gentape(stackT, counter++, TINTEGER, opToInstr(vrch));
          }
        }
      }

      if (variab1!=NULL&&variab2!=NULL) {
        if(variab1->type==TSTRING && variab2->type==TSTRING)
          gentape(stackT, counter++, TSTRING, I_CONCAT);
        else {
          if ((variab1->type==TSTRING && variab2->type!=TSTRING) || (variab1->type!=TSTRING && variab2->type==TSTRING))
            errors(SPAR);

          if (variab1->type < variab2->type)
            retypeIntToFloat(stackT, 2, counter++);
          if (variab1->type > variab2->type)
            retypeIntToFloat(stackT, 1, counter++);

          if (stackT->top->symbol==IDENTIFY){
            if (variab1->type==TDOUBLE)
              gentape(stackT, counter++, TDOUBLE, opToInstr(vrch));
            if (variab1->type==TINTEGER)
              gentape(stackT, counter++, TINTEGER, opToInstr(vrch));
          }
          else {
            if (stackT->top->symbol==TDOUBLE)
              gentape(stackT, counter++, TDOUBLE, opToInstr(vrch));
            if (stackT->top->symbol==TINTEGER)
              gentape(stackT, counter++, TINTEGER, opToInstr(vrch));
          }
        }
      }
    break; // koniec +
    // Operátory - a *
    case OP_MINUS:
    case OP_MUL:
      if ((stackT->top->symbol==TSTRING && stackT->top->next->symbol!=TSTRING) || (stackT->top->symbol!=TSTRING && stackT->top->next->symbol==TSTRING))
        errors(SPAR);

      if (variab1==NULL&&variab2==NULL) {
        if (stackT->top->symbol < stackT->top->next->symbol)
          retypeIntToFloat(stackT, 2, counter++);
        if (stackT->top->symbol > stackT->top->next->symbol)
          retypeIntToFloat(stackT, 1, counter++);

        if (stackT->top->symbol==TDOUBLE)
          gentape(stackT, counter++, TDOUBLE, opToInstr(vrch));
        if (stackT->top->symbol==TINTEGER)
          gentape(stackT, counter++, TINTEGER, opToInstr(vrch));
      }

      if (variab1==NULL&&variab2!=NULL) {
        if ((stackT->top->symbol==TSTRING && variab2->type!=TSTRING) || (stackT->top->symbol!=TSTRING && variab2->type==TSTRING))
          errors(SPAR);

        if (stackT->top->symbol < variab2->type)
          retypeIntToFloat(stackT, 2, counter++);
        if (stackT->top->symbol > variab2->type)
          retypeIntToFloat(stackT, 1, counter++);

        if (stackT->top->symbol==TDOUBLE)
          gentape(stackT, counter++, TDOUBLE, opToInstr(vrch));
        if (stackT->top->symbol==TINTEGER)
          gentape(stackT, counter++, TINTEGER, opToInstr(vrch));
      }

      if (variab1!=NULL&&variab2==NULL) {
        if ((variab1->type==TSTRING && stackT->top->next->symbol!=TSTRING) || (variab1->type!=TSTRING && stackT->top->next->symbol==TSTRING))
          errors(SPAR);

        if (variab1->type > stackT->top->next->symbol)
          retypeIntToFloat(stackT, 1, counter++);
        if (variab1->type < stackT->top->next->symbol)
          retypeIntToFloat(stackT, 2, counter++);

         if (stackT->top->symbol==IDENTIFY){
           if (variab1->type==TDOUBLE)
            gentape(stackT, counter++, TDOUBLE, opToInstr(vrch));
           if(variab1->type==TINTEGER)
            gentape(stackT, counter++, TINTEGER, opToInstr(vrch));
        }
        else {
          if (stackT->top->symbol==TDOUBLE)
           gentape(stackT, counter++, TDOUBLE, opToInstr(vrch));
          if(stackT->top->symbol==TINTEGER)
           gentape(stackT, counter++, TINTEGER, opToInstr(vrch));
        }
      }

      if (variab1!=NULL&&variab2!=NULL) {
        if ((variab1->type==TSTRING && variab2->type!=TSTRING) || (variab1->type!=TSTRING && variab2->type==TSTRING))
          errors(SPAR);

        if (variab1->type > variab2->type)
          retypeIntToFloat(stackT, 1, counter++);
        if (variab1->type < variab2->type)
          retypeIntToFloat(stackT, 2, counter++);

        if(stackT->top->symbol==IDENTIFY){
          if (variab1->type==TDOUBLE)
            gentape(stackT, counter++, TDOUBLE, opToInstr(vrch));
          if(variab1->type==TINTEGER)
            gentape(stackT, counter++, TINTEGER, opToInstr(vrch));
        }
        else {
          if (stackT->top->symbol==TDOUBLE)
            gentape(stackT, counter++, TDOUBLE, opToInstr(vrch));
          if(stackT->top->symbol==TINTEGER)
            gentape(stackT, counter++, TINTEGER, opToInstr(vrch));
        }
      }
    break;  // koniec - a *
    // operátory / a backslash
    case OP_DIV:
    case OP_IDIV:
      if (variab1==NULL&&variab2==NULL){
          int reflag=0;
		reflag=reflag;
        if (stackT->top->symbol==TSTRING || stackT->top->next->symbol==TSTRING)
        errors(SPAR);


        // kontrola delenia nulou
        if((strcmp(stackT->top->name->str, "0") == 0) || strcmp(stackT->top->name->str, "0.0") == 0)
            errors(OTHER);
        if (vrch==OP_IDIV) {
                  /* code */
          if (stackT->top->symbol== TDOUBLE)
              {retypeFloatToInt(stackT, 1, counter++);
              reflag=1;}
          if (stackT->top->next->symbol== TDOUBLE)
            {retypeFloatToInt(stackT, 2, counter++);
              reflag=1;}
        }
        if (stackT->top->symbol==TDOUBLE && stackT->top->next->symbol==TDOUBLE){
          gentape(stackT, counter++, TDOUBLE, opToInstr(vrch));
        }
        else {
          if (stackT->top->symbol== TINTEGER)
            retypeIntToFloat(stackT, 1, counter++);
          if (stackT->top->next->symbol== TINTEGER)
            retypeIntToFloat(stackT, 2, counter++);

          gentape(stackT, counter++, TDOUBLE, opToInstr(vrch));
        }
      }
      if (variab1==NULL&&variab2!=NULL) {
        int reflag=0;
        if (stackT->top->symbol==TSTRING || variab2->type==TSTRING)
          errors(SPAR);

        // kontrola delenia nulou
        if((strcmp(stackT->top->name->str, "0") == 0) || strcmp(stackT->top->name->str, "0.0") == 0)
            errors(OTHER);
        if (vrch==OP_IDIV) {
              /* code */
          if (stackT->top->symbol== TDOUBLE)
              {retypeFloatToInt(stackT, 1, counter++);
              reflag=1;}
          if (variab2->type== TDOUBLE)
              {retypeFloatToInt(stackT, 2, counter++);
              reflag=1;}
        }

        if (stackT->top->symbol==TDOUBLE && variab2->type==TDOUBLE&&reflag==0)
          {gentape(stackT, counter++, TDOUBLE, opToInstr(vrch));
            }
        else {

          if (stackT->top->symbol== TINTEGER)
            retypeIntToFloat(stackT, 1, counter++);
          if (variab2->type== TINTEGER&&reflag==0)
            retypeIntToFloat(stackT, 2, counter++);
          if (stackT->top->next->symbol==TINTEGER&&reflag==1){

            retypeIntToFloat(stackT, 2, counter++);}
          gentape(stackT, counter++, TDOUBLE, opToInstr(vrch));
        }
      }
      if (variab1!=NULL&&variab2==NULL) {
        int reflag=0;
        if (variab1->type==TSTRING || stackT->top->next->symbol==TSTRING)
          errors(SPAR);
        if (vrch==OP_IDIV) {
            /* code */
            if (variab1->type== TDOUBLE)
                {retypeFloatToInt(stackT, 1, counter++);
                reflag=1;}
            if (stackT->top->next->symbol== TDOUBLE)
                {retypeFloatToInt(stackT, 2, counter++);
                reflag=1;}
        }
        if (variab1->type==TDOUBLE && stackT->top->next->symbol==TDOUBLE && reflag==0)
          gentape(stackT, counter++, TDOUBLE, opToInstr(vrch));
        else {
          if (variab1->type== TINTEGER)
            retypeIntToFloat(stackT, 1, counter++);
          if (stackT->top->symbol== TINTEGER&&reflag==1)
            retypeIntToFloat(stackT, 1, counter++);
          if (stackT->top->next->symbol== TINTEGER)
            retypeIntToFloat(stackT, 2, counter++);

          gentape(stackT, counter++, TDOUBLE, opToInstr(vrch));
        }
      }
      if (variab1!=NULL&&variab2!=NULL) {
          int reflag=0;
        if (variab1->type==TSTRING || variab2->type==TSTRING)
        errors(SPAR);
        if (vrch==OP_IDIV) {
            /* code */
            if (variab1->type== TDOUBLE)
                {retypeFloatToInt(stackT, 1, counter++);
                reflag=1;}
            if (variab2->type== TDOUBLE)
                {retypeFloatToInt(stackT, 2, counter++);
                reflag=1;}
        }
        if (variab1->type==TDOUBLE && variab2->type==TDOUBLE&&reflag==0)
          gentape(stackT, counter++, TDOUBLE, opToInstr(vrch));
        else {
          if (variab1->type== TINTEGER)
            retypeIntToFloat(stackT, 1, counter++);
          if (stackT->top->symbol== TINTEGER&&reflag==1)
            retypeIntToFloat(stackT, 1, counter++);
          if (variab2->type== TINTEGER)
            retypeIntToFloat(stackT, 2, counter++);
          if (stackT->top->next->symbol== TINTEGER&&reflag==1)
            retypeIntToFloat(stackT, 2, counter++);

          gentape(stackT, counter++, TDOUBLE, opToInstr(vrch));
        }
      }
      if(vrch == OP_IDIV)
        retypeFloatToInt(stackT,1, counter++);
    break;  // opeátory delenia
    // relačné operátory
    case OP_LOWER:
    case OP_BIGGER:
    case OP_EQ:
    case OP_LOWEREQ:
    case OP_BIGGEREQ:
    case OP_NOTEQ: ;

      if (variab1==NULL&&variab2==NULL){
        if(stackT->top->symbol < stackT->top->next->symbol)
          retypeIntToFloat(stackT, 2, counter++);
        if (stackT->top->symbol > stackT->top->next->symbol)
          retypeIntToFloat(stackT, 1, counter++);
      }
      if (variab1!=NULL&&variab2==NULL){
        if (variab1->type!=TSTRING && stackT->top->next->symbol!=TSTRING){
          if (variab1->type < stackT->top->next->symbol)
            retypeIntToFloat(stackT, 2, counter++);
          if (variab1->type > stackT->top->next->symbol)
            retypeIntToFloat(stackT, 1, counter++);
        }
      }
      if (variab1==NULL&&variab2!=NULL){
        if (stackT->top->symbol!=TSTRING && variab2->type!=TSTRING){
          if(stackT->top->symbol < variab2->type)
            retypeIntToFloat(stackT, 2, counter++);
          if (stackT->top->symbol > variab2->type)
            retypeIntToFloat(stackT, 1, counter++);
        }
      }
      if (variab1!=NULL&&variab2!=NULL){
        if (variab1->type!=TSTRING && variab2->type!=TSTRING){
          if(variab1->type < variab2->type)
            retypeIntToFloat(stackT, 2, counter++);
          if (variab1->type > variab2->type)
            retypeIntToFloat(stackT, 1, counter++);
        }
      }
      if(stat==KW_IF)
        instr_add(insList, I_IF, NULL, NULL, NULL, 0, 0);
      if(stat==KW_DO)
        instr_add(insList, I_CYCLE, NULL, NULL, NULL, 0, 0);

      gentape(stackT, counter++, TINTEGER, opToInstr(vrch));

    break;  // koniec relačných
    case NETERMINAL:
    break;
  }

  // POVODNY CYKLUS!!!!
  while(n>0){
    pop(stack);
    n--;
  }
}

/**
 * Funkcia vracia symbol na vrchole zásobníka
 * @param   stack   želaný zásobník
 * @return  vráti symbol na vrchole, ak je prázdny 0
 */
int top(Stack *stack){
  if (stack->top!=NULL)
    return stack->top->symbol;
  return 0;
}

/**
 * Funkcia overí, či token je terminál
 * @param   token   token ktorý chceme overiť
 * @return  true ak je terminál
 */
bool isTerm(int token){
  return (token < NETERMINAL);
}

/**
 * Funkcia vráti terminál najbližšie vrcholu zásobníka
 * @param   stack   želaný zásobník
 * @return  vráti terminál najbližšie vrcholu, alebo -1 ak je prázdny
 */
int topTerm(Stack stack){
  stack_item_t *tmp = stack.top;
  while(tmp!=NULL){

    if(isTerm(tmp->symbol))
      return tmp->symbol;
    else tmp= tmp->next;
  }
  return -1;
}

/**
 * Funckia vloží '<' pri shiftovaní.
 * Vloží sa nad najvyšší terminál
 * @param   stack   želaný zásboník
 */
void pushLessThan(Stack *stack){
  if(isTerm(top(stack)))
  {
    push(stack, '<', NULL);}
  else{
    int neterm = top(stack);
    pop(stack);
    push(stack, '<', NULL);
    push(stack, neterm, NULL);
  }
}

/*
* Funkcia ktorá prevedie operátor na inštrukciu.
* @param  op  operátor ktorý chceme previesť.
* @return     vracia prevedenú inštrukciu, -1 ak parameter nieje operátor
*/
int opToInstr(int op){
  switch (op){
    case OP_PLUS:
      return I_ADD; break;
    case OP_MINUS:
      return I_SUB; break;
    case OP_MUL:
      return I_MUL; break;
    case OP_DIV:
    case OP_IDIV:
      return I_DIV; break;
    case OP_LOWER:
      return I_LT; break;
    case OP_LOWEREQ:
      return I_LT_EQ; break;
    case OP_BIGGEREQ:
      return I_GT_EQ; break;
    case OP_NOTEQ:
      return I_NOT_EQ; break;
    case OP_EQ:
      return I_EQ; break;
    case OP_BIGGER:
      return I_GT; break;
    default:
      return -1;
  }
}

/**
 * Precedenčná syntaktická analýza
 * @param   list  zoznam tokenov ktoré sa majú vyhodnotiť
 * @param   stat  typ príkazu, v kotorom sa volá precedenčka /priradenie/if/while
 * @param   name  nazov premennej do ktorej sa ma uložiť priradenie
                  NULL znamená že sa nejedná o priradenie
 * @param   func  názov funkcie v ktorej sa výraz nachádza
 * @return        1 pri syntakticky správnom výraze.
 */
int psa(expList *list, int stat, char *name, Pmain_nod func){
  // vložím si $ na koniec listu

  expData *endmarker = malloc(sizeof(expData));
  initExpData(endmarker);
  endmarker->type=-1;
  expLast(list);
  insertExp(list, endmarker);
  expFirst(list);
  // inicializácia stackov
  Stack stackp, stackT;
  stackInit(&stackp);
  stackInit(&stackT);
  int lastType;
  string *value = malloc(sizeof(string));
  if(value==NULL)
    errors(INTERNAL);
  strInit(value);
  push(&stackp, -1, NULL);  //vložím $ na zásobník

  // začiatok PSA
  expFirst(list);
  int token = expType(list);
    do{
      switch(prectab[group_tokens(topTerm(stackp))][group_tokens(token)]){
        case '<':
          //shiftuj
          pushLessThan(&stackp);
          push(&stackp, token, NULL);
          strCopyString(value, list->active->data->s);
          lastType=expType(list);
          expNext(list);

          if (list->active!=NULL){
            token=expType(list);
          }
        break;
        case '=':
          // vlož ) na zásobník a načítaj ďalší token
          push(&stackp, token, NULL);
          expNext(list);
          token=list->active->data->type;
          break;
        case '>':
          // redukovanie
          // ak je na zásobníku operator
          if(topTerm(stackp) < TDOUBLE){

            multipop(&stackp, &stackT, 3, stat, func);        //chceme použiť pravidlo E->E?E, prípadne E->(E)
            //teraz by mal byt na vrchole '<'
            if (top(&stackp)=='<'){
              pop(&stackp);
              push(&stackp, NETERMINAL, NULL);
            } else { //nesedia neterminály
                errors(SYNTAX);
              }

          } else {  //na zásobníku je i
              push(&stackT, lastType, value);
              pop(&stackp);  //pop už len raz, lebo používame E->i
              if (top(&stackp)=='<'){
                pop(&stackp);
                push(&stackp, NETERMINAL, NULL);
            } else {  //očakával som < ale nieje tam
                stackFree(&stackp);
                stackFree(&stackT);
                errors(SYNTAX);
                }
          }
          break;
        case '1':
          // sem sa asi nikdy nedostane, lebo tomu bráni podmienka cyklu :D
          return 1;
        case ' ':
          // prázdne políčko v tabulke znamená syntaktickú chybu
          stackFree(&stackp);
          stackFree(&stackT);
          errors(SYNTAX);
        break
      ;
      }
      if(topTerm(stackp) ==-1 && group_tokens(token) == 7 )
        break;
    }while(topTerm(stackp)!=-1 || group_tokens(token) != 7);

    //vyhľadanie premennej v tabulke symbolov
    PSymbol variab = NULL;
    if(stackT.top!=NULL){
      if(stackT.top->symbol==IDENTIFY || search_PEPO(table,stackT.top->name->str,fname)){
        if(search_PATO(table,stackT.top->name->str,fname))
          variab = STsearch(func->func_tree->Root, stackT.top->name->str);
        if(variab == NULL){
          variab = varfromlist(func, stackT.top->name->str);
        }
      }
    }
    // generovanie inštrukcie podľa typu príkazu, pretypovanie pred priradením do premennej

    switch(stat){
      case TINTEGER:
        if(variab!=NULL) {
          if (variab->type != stat)
            instr_add(insList, I_FLOAT_TO_INT, name, variab->name, NULL, variab->type, 0);
          else
            instr_add(insList, I_MOVE, name, variab->name, NULL, variab->type, 0);
        }
        else {
          if (stackT.top->symbol != stat)
            instr_add(insList, I_FLOAT_TO_INT, name, stackT.top->name->str, NULL, stackT.top->symbol, 0);
          else
            instr_add(insList, I_MOVE, name, stackT.top->name->str, NULL, stackT.top->symbol, 0);
        }
      break;
      case TDOUBLE:
        if(variab!=NULL) {
          if (variab->type != stat)
            instr_add(insList, I_INT_TO_FLOAT, name, variab->name, NULL, variab->type, 0);
          else
            instr_add(insList, I_MOVE, name, variab->name, NULL, variab->type, 0);
        }
        else {
          if (stackT.top->symbol != stat)
            instr_add(insList, I_INT_TO_FLOAT, name, stackT.top->name->str, NULL, stackT.top->symbol, 0);
          else
            instr_add(insList, I_MOVE, name, stackT.top->name->str, NULL, stackT.top->symbol, 0);
        }
      break;
      case TSTRING:
        if(variab!=NULL)
          instr_add(insList, I_MOVE, name, variab->name, NULL, variab->type, 0);
        else
          instr_add(insList, I_MOVE, name, stackT.top->name->str, NULL, stackT.top->symbol, 0);
      break;
      case KW_RETURN:
        instr_add(insList, I_RETURN, stackT.top->name->str, NULL, NULL, stackT.top->symbol, 0);
      break;
      case KW_PRINT:
        if (variab!=NULL) {

          instr_add(insList, I_WRITE, variab->name, NULL, NULL,variab->type, 0);
        }
        else{

        instr_add(insList, I_WRITE, stackT.top->name->str, NULL, NULL,stackT.top->symbol, 0);
      }
      break;
      default:
      break;
    }

    stackFree(&stackp);
    stackFree(&stackT);
    strFree(value);
    strFree(endmarker->s);
    free(endmarker);

    return 1; //PSA sa vykonala ok
}
