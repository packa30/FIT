#include "precedence.h"

//#include "parser.h"
/**
 * Funkcia overuje, či sa v zozname nachádza operátor.
 * @param   list  zoznam obsahujúci výraz
 * @return        true ak sa vo výraze nachádza operátor, inak false
 */
int expListOP(expList *list){
  expFirst(list);
  int result;
  while(expLast(list)){
    result = expType(list);
    switch (result) {
      case OP_EQ:       case OP_NOTEQ:
      case OP_BIGGEREQ: case OP_BIGGER:
      case OP_LOWEREQ:  case OP_LOWER:
        return true;
        break;
    }
    if(list->active->next == NULL)
      break;
    expNext(list);
  }

  expFirst(list);
  return false;
}
/**
 * Funkcia zišťuje dĺžku zoznamu (počet prvkov vo výraze)
 * @param   list  zoznam obsahujúci výraz
 * @return        vráti dlžku zoznamu
 */
int expListLength(expList *list){
  expFirst(list);
  int i = 0;
  while(expLast(list)){
    i++;
    if(list->active->next == NULL)
      break;
    expNext(list);
  }
  expFirst(list);
  return i;
}
/**
 * Funkcia uvoľňuje položky v zozname obsahujúcom výraz
 * @param   L   zoznam obsahujúci výraz
 */
void expListFree(expList *L){
  if(L!=NULL){
    tExpListItem *ptr;
    while (L->first != NULL)
    {
      ptr = L->first;
      L->first = L->first->next;
      // uvolnime celou polozku
      strFree(ptr->data->s);
      free(ptr->data->s);
      free(ptr->data);
      free(ptr);
    }
  }
}
/**
 * Funkcia inicializuje položku zoznamu
 * @param   data  ukazovatel na položku reprezentujúcu jeden prvok výrazu
 */
void initExpData(expData *data){
  data->s = malloc(sizeof(string));
  if(data->s == NULL)
    errors(INTERNAL);
  strInit(data->s);
}
/**
 * Funkcia inicializuje samotný zoznam. Nastavuje ukazovatele na NULL
 * @param   list  ukazovatel na zoznam, ktorý inicializujeme
 */
void expListInit(expList *list){
  list->first = NULL;
  list->last = NULL;
  list->active = NULL;
}

/**
 * Funkcia nastaví prvý prvok zoznamu na aktívny. Volajúci musí overiť, že zoznam je inicializovaný.
 * @param   list  ukazovatel na zoznam pre výraz
 */
void expFirst(expList *list){
    list->active = list->first;
}
/**
 * Funkcia nastaví aktivitu na nasledujúci prvok v zozname. Aktivita zostáva na poslednom prvku a nestráca sa.
 * @param   list   ukazovatel na zoznam pre výraz
 */
void expNext(expList *list){
    if(list->active->next != NULL)
        list->active = list->active->next;
}
/**
 * Funkcia pre vkladanie položky do zoznamu.
 * @param   list   ukazovatel na zoznam pre výraz
 * @param   data   ukazovatel na data, ktoré vkladáme do zoznamu
 * @return         1
 */
int insertExp(expList *list,expData *data){
  if(list == NULL)
    return 0;
  tExpListItem * new = malloc(sizeof(tExpListItem));
  expData *new_data = malloc(sizeof(expData));
  initExpData(new_data);
  // nove data
  new_data->type = data->type;
  strCopyString(new_data->s,data->s);
  new->data = new_data;
  new->next = NULL;

  if(list->first == NULL){
    list->first = new;
    list->last = list->first;
  }else{
    list->last->next = new;
    list->last = list->last->next;
  }

  return 1;
}

int expType(expList *list){
  return list->active->data->type;
}

char *expVal(expList *list){
  return list->active->data->s->str;
}

int expLast(expList *list){
  if(list->active == NULL)
    return false;
  return true;
}


int checkExp(int isIF, expList *list,int add){

  string *s = malloc(sizeof(string));
  strInit(s);

  expData *data = malloc(sizeof(expData));
  initExpData(data);

  char *temp;

  int result,state,read,pars;
  pars = add;
  state = STATE_START;
  int isMinus= 0;
  read = true;
  while(read){
    result = getToken(fp,s);
    if (result == NEW_LINE && isIF == 2){
      return result;}
    switch (state) {
      case STATE_START:
        if(result == PAR_LEFT){
          state = STATE_LPAR;
        }else if(result == OP_PLUS){
          state = STATE_OP;
        }else if(result == TSTRING || result == TDOUBLE || result == TINTEGER || result == IDENTIFY){
          state = STATE_ID;
        }else if(result == OP_MINUS){
          state = STATE_MINUS;
          isMinus = 1;
          continue;
        }else if(result == KW_NOT){
          state = STATE_NOT;
        }else{
          errors(SYNTAX);
        }
        data->type = result;
        strCopyString(data->s,s);
        insertExp(list,data);
        break;
      // STATE START
        case STATE_NOT:
          if(result == OP_MINUS){
            state = STATE_MINUS2;
          }else if(result == PAR_LEFT){
            state = STATE_LPAR;
          }else if(result == TSTRING || result == TDOUBLE || result == TINTEGER || result == IDENTIFY){
            state = STATE_ID;
          }else{
            errors(SYNTAX);
          }
          break;
        // STATE not
        case STATE_LPAR:
          pars++;
          if(result == PAR_LEFT){
            ;
          }else if(result == OP_MINUS){
            state = STATE_MINUS2;
            continue;
          }else if(result == TINTEGER || result == TDOUBLE || result == TSTRING || result == IDENTIFY){
            state = STATE_ID;
          }else if(result == KW_NOT){
            state = STATE_NOT;
          }else{
            errors(SYNTAX);
          }
          data->type = result;
          strCopyString(data->s,s);
          insertExp(list,data);
          break;
        // sTATE LPAR
        case STATE_MINUS:
          if(result == PAR_LEFT){
            state = STATE_LPAR;
          }else if(result == OP_MINUS){
            state = STATE_MINUS2;
            continue;
          }else if(result == TSTRING || result == TINTEGER || result == TDOUBLE || result == IDENTIFY){
            if(isMinus){
              s->length++;
              temp = malloc(s->length+1);
              temp[0] = '-';
              temp[1] = '\0';
              strcat(temp,s->str);
              strcpy(s->str,temp);
              data->type = result;
              strCopyString(data->s,s);
              insertExp(list,data);
              free(temp);
              isMinus = 0;
              state = STATE_ID;
              continue;
            }
            state = STATE_ID;
          }else{
            errors(SYNTAX);
          }
          data->type = result;
          strCopyString(data->s,s);
          insertExp(list,data);
          break;
        //state MINUS

        case STATE_MINUS2:
          //errors(SYNTAX);
          if(result == TINTEGER || result == TDOUBLE || result == TSTRING || result == IDENTIFY){
            state = STATE_ID;
          }else{
            errors(SYNTAX);
          }
          s->length++;
          temp = malloc(s->length+1);
          temp[0] = '-';
          temp[1] = '\0';
          strcat(temp,s->str);
          strcpy(s->str,temp);
          data->type = result;
          strCopyString(data->s,s);
          insertExp(list,data);
          free(temp);
          break;
        // state MINUS2

        case STATE_OP:
          if(result == PAR_LEFT){
            state = STATE_LPAR;
          }else if(result == OP_MINUS){
            state = STATE_MINUS2;
            continue;
          }else if(result == KW_NOT){
            state = STATE_NOT;
          }else if(result == TINTEGER || result == TDOUBLE || result == TSTRING || result == IDENTIFY){
            state = STATE_ID;
          }else{
            errors(SYNTAX);
          }
          data->type = result;
          strCopyString(data->s,s);
          insertExp(list,data);
          break;
        // state OP
        case STATE_ID:
          if(result == OP_MINUS){
            state = STATE_MINUS;
          }else if(result == OP_MUL || result == OP_DIV || result == OP_IDIV || result == OP_PLUS || result == OP_IDIV){
            state = STATE_OP;
          }else if(result == PAR_RIGHT){
            state = STATE_RPAR;
        }else if(result == KW_THEN || result == NEW_LINE || result == SEMICOLON){
            if(isIF == 1 && result != KW_THEN)
              errors(SYNTAX);
            else if(isIF == 0 && result != NEW_LINE)
              errors(SYNTAX);
            else if(isIF == 2 && result != SEMICOLON){
                errors(SYNTAX);
            }
            read = false;
            continue;
          }else if(result == KW_AND || result == KW_OR || result == OP_LOWER || result == OP_BIGGER || result == OP_EQ || result == OP_NOTEQ || result == OP_BIGGEREQ || result == OP_LOWEREQ){
              state = STATE_OPER;
          }else{
            errors(SYNTAX);
          }
          if(isMinus){
            s->length++;
            temp = malloc(s->length+1);
            temp[0] = '-';
            temp[1] = '\0';
            strcat(temp,s->str);
            strcpy(s->str,temp);
            data->type = result;
            strCopyString(data->s,s);
            insertExp(list,data);
            free(temp);
            isMinus = 0;
          }
          data->type = result;
          strCopyString(data->s,s);
          insertExp(list,data);

          break;

        case STATE_RPAR:
          pars--;
          if(result == PAR_RIGHT){
            ;
          }else if(result == OP_MUL || result == OP_DIV || result == OP_IDIV || result == OP_PLUS || result == OP_IDIV){
            state = STATE_OP;
          }else if(result == KW_THEN || result == NEW_LINE || result== SEMICOLON){
            if(pars != 0){
              errors(SYNTAX);
            }
            if(isIF==1 && result != KW_THEN){
              errors(SYNTAX);
            }
            else if(!isIF && result != NEW_LINE){
              errors(SYNTAX);
            }
            else if(isIF == 2 && result != SEMICOLON){
              errors(SYNTAX);
              }
            read = false;
            continue;
          }else if(result == KW_AND || result == KW_OR || result == OP_LOWER || result == OP_BIGGER || result == OP_EQ || result == OP_NOTEQ || result == OP_BIGGEREQ || result == OP_LOWEREQ){
              state = STATE_OPER;
          }else if(result == OP_MINUS){
            state = STATE_MINUS;
          }else{
            errors(SYNTAX);
          }

          data->type = result;
          strCopyString(data->s,s);
          insertExp(list,data);
          break;

        case STATE_OPER:
          if(result == PAR_LEFT){
            state = STATE_LPAR;
          }else if(result == OP_MINUS){
            state = STATE_MINUS2;
            continue;
          }else if(result == TINTEGER || result == TDOUBLE || result == TSTRING || result == IDENTIFY){
            state = STATE_ID;
          }else if(result == KW_NOT){
            state = STATE_NOT;
          }else{
            errors(SYNTAX);
          }
          data->type = result;
          strCopyString(data->s,s);
          insertExp(list,data);
          break;
          default:
            errors(SYNTAX);
        // state Oper

    }
  }
  strFree(data->s);
  free(data->s);
  free(data);
  strFree(s);
  free(s);
  return 1;
}
/**
 * Funkcia pre overenie správnosti deklarovania premennej. Kontroluje ako syntax, tak aj existenciu premennej v danej funkcii.
 * @return  1
 */
int declareID(){
    string *s = malloc(sizeof(string));
    strInit(s);
    expList * elist = malloc(sizeof(expList));
    expListInit(elist);
    char *name_id;
    int type;

    int result;
    if(getToken(fp,s) == IDENTIFY){
      if(search_PATO(table,s->str,fname)){
        errors(SDEF);
      }
      if(search_PATO(table,NULL,s->str)){
        errors(SDEF);
      }
      name_id = malloc(s->length+1);
      strcpy(name_id,s->str);
      //INSERT TO TABLE

      if(strcmp(fname,"scope") == 0){
      }else{
        if(search_PEPO(table,s->str,fname))
          errors(SDEF);
      }
      if(getToken(fp,s) == KW_AS){

        result = getToken(fp,s);
        type = getTypeFromKW(result);
        switch (result) {
          case KW_STRING: case KW_INTEGER: case KW_DOUBLE:
            instr_add(insList,I_DEFVAR,name_id,NULL,NULL,type,0);

            result = getToken(fp,s);
            if(result == NEW_LINE){
              insert_token(table,type,name_id,NULL,fname,NULL);
            }else if(result == OP_EQ){
              insert_token(table,type,name_id,NULL,fname,NULL);
              checkExp(0,elist,0);
              Pmain_nod func = search_func(table->Root, fname);
              sematika(search_prem_type(table,name_id,fname),elist,0);
              expFirst(elist);
              psa(elist, type, name_id, func);
              //expFirst(elist);
              //instr_add(insList,I_MOVE,name_id,expVal(elist),NULL,expType(elist),0);
            }else{
              errors(SYNTAX);
            }
            break;
          default:
            errors(SYNTAX);
        }

      }else{
        errors(SYNTAX);
      }
    }else{
      errors(SYNTAX);
    }
    free(name_id);
    expListFree(elist);
    free(elist);
    strFree(s);
    free(s);
    return 1;
}
/**
 * Funkcia pre príkaz print. Volá sa kontrola výrazu a precedenčná analýza
 * @return  1
 */
int print(){
  int result;
  Pmain_nod func;
  expList *elist = malloc(sizeof(expList));
  func = search_func(table->Root, fname);

  expListInit(elist);
  checkExp(2,elist,0);
  psa(elist,KW_PRINT,NULL,func);

  expListFree(elist);
  expListInit(elist);

  while(1){

    expListInit(elist);
    result = checkExp(2,elist,0);
    if(result == NEW_LINE)
      break;
    psa(elist,KW_PRINT,NULL,func);
    expListFree(elist);
  }
  expListFree(elist);
  //free(elist);
return 1;
}
/**
 * Funkcia pre príkaz input. Kontroluje syntax príkazu, či premenná existuje a akého je typu.
 * @return  1, alebo ukončuje program s chybovým kódom.
 */
int input(){
  string *s = malloc(sizeof(string));
  strInit(s);
  int type;
  if(getToken(fp,s) == IDENTIFY){
    if(search_PATO(table,s->str,fname)){
      type = search_prem_type(table,s->str,fname);
      instr_add(insList,I_INPUT,s->str,NULL,NULL,type,0);
    }else{
      errors(SDEF);
    }
    if(getToken(fp,s) != NEW_LINE)
      errors(SYNTAX);
  }else{
    errors(SYNTAX);
  }
  strFree(s);
  free(s);
  return 1;

}
/**
 * Funkcia pre kontrolu deklarácie užívateľskej funkcie.
 * @return  1, alebo ukončuje program s kódom chyby
 */
int fundeclare(){
  string *s = malloc(sizeof(string));
  strInit(s);
  int nothing = 0;
  int result;
  int type;

  tList list = Listinit();

  if(getToken(fp,s) != KW_FUNCTION)
    errors(SYNTAX);

  if(getToken(fp,s) != IDENTIFY)
    errors(SYNTAX);
  fname = malloc(strlen(s->str));
  strcpy(fname,s->str);

  if(getToken(fp,s) != PAR_LEFT){
    errors(SYNTAX);}

    while(1){
      result = getToken(fp,s);
      if(result == IDENTIFY){
        nothing = 1;
        if(search_PATO(table,NULL,s->str)){
          errors(SDEF);
        }
        name_id = malloc(s->length+1);
        strcpy(name_id,s->str);
      }else if(result == PAR_RIGHT && nothing == 0){
        break;
      }else{
        errors(SYNTAX);
      }

      if(getToken(fp,s) != KW_AS)
        errors(SYNTAX);

      result = getToken(fp,s);
      type = getTypeFromKW(result);
      if(result != KW_INTEGER && result != KW_DOUBLE && result != KW_STRING)
        errors(SYNTAX);
      Insert(list,type,name_id);
      result = getToken(fp,s);

      if(result == PAR_RIGHT){
        break;
      } //declare function id ( x as integer)
      else if(result != COMMA){
        errors(SYNTAX);
      }  //declare function id ( x as integer,

        free(name_id);

    }

    if(getToken(fp,s) != KW_AS)
      errors(SYNTAX);

    result = getToken(fp,s);
    type = getTypeFromKW(result);
    if(result != KW_INTEGER && result != KW_DOUBLE && result != KW_STRING)
      errors(SYNTAX);

    if(search_PATO(table,NULL,fname)){
      errors(SDEF);
    }else{
      insert_token(table,type,NULL,NULL,fname,list);
    }

    result = getToken(fp,s);
    if(result != NEW_LINE)
      errors(SYNTAX);
  //destroy_list(list->first);
  //free(list);
  strFree(s);
  free(s);
  return 1;

}
/**
 * Funkcia vráti dátový typ podľa kľúčového slova.
 * @param   result   kľúčové slovo
 * @return           typ podľa kľúčového slova, false ak slovo nie je jedno z: int, double, string
 */
int getTypeFromKW(int result){
  if(result == KW_INTEGER)
    return TINTEGER;
  if(result == KW_DOUBLE)
    return TDOUBLE;
  if(result == KW_STRING)
    return TSTRING;
  return false;
}
/**
 * Funkcia pre kontrolu definície užívateľskej funkcie.
 * @return  1, alebo ukončuje program s kódom chyby
 */
int fundef(){
  string *s = malloc(sizeof(string));
  strInit(s);
  int result , type;
  int nothing = 0;
  tList list = Listinit();

  if(getToken(fp,s) != IDENTIFY)
    errors(SYNTAX);
  fname = malloc(s->length+1);
  strcpy(fname,s->str);

  if(getToken(fp,s) != PAR_LEFT)
    errors(SYNTAX);

  while(1){
    result = getToken(fp,s);

    if(result == IDENTIFY){
      nothing = 1;
      if(search_PATO(table,NULL,s->str)){
        errors(SDEF);
      }
      name_id = malloc(s->length+1);
      strcpy(name_id,s->str);
    }else if(result == PAR_RIGHT && nothing == 0){
      break;
    }else{
      errors(SYNTAX);
    }

    if(getToken(fp,s) != KW_AS)
      errors(SYNTAX);

    result = getToken(fp,s);
    type = getTypeFromKW(result);

    if(result != KW_INTEGER && result != KW_DOUBLE && result != KW_STRING)
      errors(SYNTAX);

    Insert(list,type,name_id);

    result = getToken(fp,s);

    if(result == PAR_RIGHT) //declare function id ( x as integer)
      break;
    else if(result != COMMA)
      errors(SYNTAX);
    free(name_id);
  }
  if(getToken(fp,s) != KW_AS)
    errors(SYNTAX);

  result = getToken(fp,s);
  type = getTypeFromKW(result);

  if(result != KW_INTEGER && result != KW_DOUBLE && result != KW_STRING)
    errors(SYNTAX);
  result = getToken(fp,s);

  if(result != NEW_LINE)
    errors(SYNTAX);

  if(search_PATO(table,NULL,fname)){
    if(is_defined(table,fname)){
      errors(SDEF);
    }
    if(!compare_whole_list(table,fname,list)){
      errors(SDEF);
    }
    func_define(table,fname);
    //destroy_list(list->first);
    free(list);
  }else{
    insert_token(table,type,NULL,NULL,fname,list);
    func_define(table,fname);
    instr_add(insList,I_LABEL,fname,NULL,NULL,0,0);
  }

  strFree(s);
  free(s);
  return 1;

}
/**
 * Funkcia overuje správnosť ukončenia zdrojového súboru.
 * @return  1 ak je súbor správne zakončený, inak ukončuje program s kódom 2
 */
int checkFile(){
  string *s = malloc(sizeof(string));
  strInit(s);
  int result;
  while((result = getToken(fp,s)) != EOF){
    if(result == NEW_LINE)
      continue;
    else{
      errors(SYNTAX);
    }
  }
  strFree(s);
  free(s);
  return 1;
}
/**
 * Funkcia pre koniec hlavného tela programu zdrojového súboru. Volá generátor cieľového kódu.
 * @return  1 pri správnosti, inak ukončuje program s kódom 2
 */
int endScope(){
  string *s = malloc(sizeof(string));
  strInit(s);
  if(getToken(fp,s) == KW_SCOPE){
    checkFile();
    //vypisinstrlist(insList);
    generate(insList,table);
    listFree(insList);
    //free(insList);
    //destroy(table);
    free(fname);
    fclose(fp);
  }else{
    errors(SYNTAX);
  }
  strFree(s);
  free(s);
  return 1;

}
/**
 * Funkcia overuje syntaktickú správnosť ukončenia užívateľskej funkcie funkcie.
 * @return  1 pri správnosti, inak ukončuje program s kódom 2
 */
int endFunction(){
  string *s = malloc(sizeof(string));
  strInit(s);

  if(getToken(fp,s) != KW_FUNCTION)
    errors(SYNTAX);
  if(getToken(fp,s) != NEW_LINE)
    errors(SYNTAX);

  strFree(s);
  free(s);
  return 1;

}
/**
 * Funkcia overuje správnosť ukončenia podmieneného príkazu.
 * @return  1 pri správnosti, inak ukončuje program s kódom 2
 */
int endIf(){
  string *s = malloc(sizeof(string));
  strInit(s);
  if(getToken(fp,s) != KW_IF)
    errors(SYNTAX);
  if(getToken(fp,s) != NEW_LINE)
    errors(SYNTAX);

  strFree(s);
  free(s);
  return 1;

}

/**
  returnVal == 1 tak je v tele funkcii , pretoďż˝e scope nemďż˝ďż˝e maďż˝ nďż˝vratovďż˝ hodnotu
*/

/**
 * Funkcia pre overenie správnosti cyklu.
 * @param   isScope   príznak, či sa nachádza v hlavnom tele programu
 * @return             1 pri správnosti, inak ukončuje program s kódom 2
 */

int checkWhile(int isScope){
  string *s = malloc(sizeof(string));
  strInit(s);
  expList *elist = malloc(sizeof(expList));
  expListInit(elist);
  int type;

  if(getToken(fp,s) != KW_WHILE){
    errors(SYNTAX);}
    instr_add(insList,I_CYCLE,NULL,NULL,NULL,0,0);
    checkExp(0,elist,0);
    expFirst(elist);

    while(expLast(elist)){
      if (elist->active->next==NULL){
        break;
      }
      if (expType(elist)!= PAR_LEFT){
        type = expType(elist);
        break;
      }
      expNext(elist);
    }

    sematika(type,elist,1);
    if(expListLength(elist) < 3){
      errors(SPAR);
    }
    if(!expListOP(elist)){
      errors(SPAR);
    }
    expFirst(elist);
    Pmain_nod func = search_func(table->Root, fname);
    psa(elist,KW_DO,NULL,func);

    if(scope(0,isScope,1) != KW_LOOP){
      errors(SYNTAX);
    }
    instr_add(insList,I_CYCLEEND,NULL,NULL,NULL,0,0);

    if(getToken(fp,s) != NEW_LINE)
      errors(SYNTAX);

  expListFree(elist);
  free(elist);
  strFree(s);
  free(s);
  return 1;

}
/**
 * Funkcia pre overenie identifikátoru. Kontroluje v akom príkaze sa nachádza a vykonáva patričné kontroly.
 * Kontroluje či bola deklarovaná a prehľadáva tabuľku symbolov, akého je dátového typu.
 * @param   id    názov identifikátoru
 */
void checkID(string *id){
  expList *elist = malloc(sizeof(expList));
  expListInit(elist);
  int isCall = 0;
  tList list = Listinit();
  int result,isExist,type;
  char *call;
  char *str;
  char *str1;
  string *s = malloc(sizeof(string));
  strInit(s);

  expData *data = malloc(sizeof(expData));
  initExpData(data);
  isExist = 0; // kontrola ďż˝i premena existuje vofunkcii
  if(search_PATO(table,id->str,fname) || search_PEPO(table,id->str,fname))
    isExist = 1;
  if(isExist){
    if(getToken(fp,s) != OP_EQ)
      errors(SYNTAX);
    // premenna =
    result = getToken(fp,s);
    int stat;
    if(search_PATO(table,id->str,fname))
      stat = search_prem_type(table,id->str,fname);
    else{
      stat = typeFromList(table,id->str,fname);
    }
    if(result == OP_MINUS){
      ungetc(45,fp);
      checkExp(0,elist,0);
      sematika(stat,elist,0);
      expFirst(elist);
      Pmain_nod func = search_func(table->Root, fname);
      psa(elist, stat, id->str, func);

    }
    // vstavanĂ© funkcie
    else if(result == KW_CHR){
      if(search_PATO(table,id->str,fname))
        stat = search_prem_type(table,id->str,fname);
      else{
        stat = typeFromList(table,id->str,fname);
      }

      if(stat != TSTRING){
        errors(SPAR);
      }

      if(getToken(fp,s) != PAR_LEFT)
        errors(SYNTAX);

      result = getToken(fp,s);

      if(result == TDOUBLE || result == TINTEGER){
        type = result;
      }else if(result == IDENTIFY){
        if(search_PATO(table,s->str,fname))
          type = search_prem_type(table,s->str,fname);
        else if(search_PEPO(table,s->str,fname))
          type = typeFromList(table,s->str,fname);
        else
          errors(SDEF);

        if(type == TSTRING){
          errors(SPAR);
        }
      }else{
        errors(SPAR);
      }
      if(type == TDOUBLE){
        char lol[50];
        sprintf(lol,"tetop%d",patotmp);
        instr_add(insList,I_DEFTMP,lol,NULL,NULL,TINTEGER,0);
        instr_add(insList,I_FLOAT_TO_INT,lol,s->str,NULL,TDOUBLE,0);
        instr_add(insList,I_CHR,id->str,lol,NULL,TINTEGER,0);
        patotmp++;
      }
      else{
        instr_add(insList,I_CHR,id->str,s->str,NULL,type,0);
      }
      if(getToken(fp,s) != PAR_RIGHT)
        errors(SYNTAX);
      if(getToken(fp,s) != NEW_LINE)
        errors(SYNTAX);


    }
    else if(result == KW_ASC){
      if(search_PATO(table,id->str,fname))
        stat = search_prem_type(table,id->str,fname);
      else{
        stat = typeFromList(table,id->str,fname);
      }

      if(stat == TSTRING){
        errors(SPAR);
      }
    // overenie navratovĂ©ho typu

      if(getToken(fp,s) != PAR_LEFT)
        errors(SYNTAX);

      result = getToken(fp,s);

      if(result == TSTRING){
        ;
      }else if(result == IDENTIFY){
        if(search_PATO(table,s->str,fname))
          type = search_prem_type(table,s->str,fname);
        else if(search_PEPO(table,s->str,fname))
          type = typeFromList(table,s->str,fname);
        else
          errors(SDEF);

        if(type != TSTRING){
          errors(SPAR);
        }
      }else{
        errors(SPAR);
      }
      str = malloc(s->length+1);
      strcpy(str,s->str);

      if(getToken(fp,s) != COMMA)
        errors(SYNTAX);

      result = getToken(fp,s);

      if(result == TDOUBLE || result == TINTEGER){
        ;
      }
      else if(result == IDENTIFY){
        if(search_PATO(table,s->str,fname))
          type = search_prem_type(table,s->str,fname);
        else if(search_PEPO(table,s->str,fname))
          type = typeFromList(table,s->str,fname);
        else
          errors(SDEF);

        if(type == TSTRING){
          errors(SPAR);
        }
      }else{
        errors(SPAR);
      }
      if(type != TINTEGER){
        char lol[50];
        sprintf(lol,"tetop%d",patotmp);
        instr_add(insList,I_DEFTMP,lol,NULL,NULL,TINTEGER,0);
        instr_add(insList,I_FLOAT_TO_INT,lol,s->str,NULL,TDOUBLE,0);
        instr_add(insList,I_ASC,id->str,str,lol,TSTRING,TINTEGER);
        patotmp++;
      }
      else{
        instr_add(insList,I_ASC,id->str,str,s->str,TSTRING,TINTEGER);
      }

      free(str);
      if(getToken(fp,s) != PAR_RIGHT)
        errors(SYNTAX);

      if(getToken(fp,s) != NEW_LINE)
        errors(SYNTAX);
      }
    else if(result == KW_LENGTH){
      if(search_PATO(table,id->str,fname))
        stat = search_prem_type(table,id->str,fname);
      else{
        stat = typeFromList(table,id->str,fname);
      }
      if(stat == TSTRING)
        errors(SPAR);

      if(getToken(fp,s) != PAR_LEFT)
        errors(SYNTAX);

      result = getToken(fp,s);

      if(result == TSTRING){
        type = result;
      }else if(result == IDENTIFY){
        if(search_PATO(table,s->str,fname))
          type = search_prem_type(table,s->str,fname);
        else if(search_PEPO(table,s->str,fname))
          type = typeFromList(table,s->str,fname);
        else
          errors(SDEF);
        if(type != TSTRING)
          errors(SPAR);
      }else{
        errors(SPAR);
      }

      instr_add(insList,I_STRLEN,id->str,s->str,NULL,type,0);
      if(getToken(fp,s) != PAR_RIGHT)
        errors(SYNTAX);
      if(getToken(fp,s) != NEW_LINE)
        errors(SYNTAX);
    }
    else if(result == KW_SUBSTR){
      if(search_PATO(table,id->str,fname))
        stat = search_prem_type(table,id->str,fname);
      else{
        stat = typeFromList(table,id->str,fname);
      }
      if(stat != TSTRING)
        errors(SPAR);
      // kontrola navratovĂ©ho typu

      if(getToken(fp,s) != PAR_LEFT)
        errors(SYNTAX);

      result = getToken(fp,s);

      if(result == TSTRING){
        ;
      }else if(result == IDENTIFY){
        if(search_PATO(table,s->str,fname))
          type = search_prem_type(table,s->str,fname);
        else if(search_PEPO(table,s->str,fname))
          type = typeFromList(table,s->str,fname);
        else
          errors(SDEF);
        if(type != TSTRING)
          errors(SPAR);
      }else{
        errors(SPAR);
      }

      str = malloc(s->length+1);
      strcpy(str,s->str);

      if(getToken(fp,s) != COMMA)
        errors(SYNTAX);

      result = getToken(fp,s);


      if(result == TDOUBLE || result == TINTEGER){
        ;
      }else if(result == IDENTIFY){
        if(search_PATO(table,s->str,fname))
          type = search_prem_type(table,s->str,fname);
        else if(search_PEPO(table,s->str,fname))
          type = typeFromList(table,s->str,fname);
        else
          errors(SDEF);

        if(type == TSTRING){
          errors(SPAR);
        }
      }else{
        errors(SPAR);
      }
      str1 = malloc(s->length+1);
      strcpy(str1,s->str);

      if(getToken(fp,s) != COMMA)
        errors(SYNTAX);

      result = getToken(fp,s);


      if(result == TDOUBLE || result == TINTEGER){
        ;
      }else if(result == IDENTIFY){
        if(search_PATO(table,s->str,fname))
          type = search_prem_type(table,s->str,fname);
        else if(search_PEPO(table,s->str,fname))
          type = typeFromList(table,s->str,fname);
        else
          errors(SDEF);

        if(type == TSTRING){
          errors(SPAR);
        }
      }else{
        errors(SPAR);
      }

      instr_add(insList,I_SUBSTR,NULL,str1,s->str,TINTEGER,TINTEGER);
      instr_add(insList,I_SUBSTREND,id->str,str,NULL,TSTRING,TSTRING);
      free(str);
      free(str1);
      if(getToken(fp,s) != PAR_RIGHT)
        errors(SYNTAX);
      if(getToken(fp,s) != NEW_LINE)
        errors(SYNTAX);
    }
    // zacina nejakďż˝m datovym typom
    else if(result == TINTEGER || result == TDOUBLE ||  result == TSTRING){

      data->type = result;
      strCopyString(data->s,s);
      insertExp(elist,data);
      result = getToken(fp,s);
      if(result >= 1500 && result <= 1510){
        data->type = result;
        strCopyString(data->s,s);
        insertExp(elist,data);
        checkExp(0,elist,0);
        sematika(search_prem_type(table,id->str,fname),elist,0);
        if(stat){

          expFirst(elist);
          Pmain_nod func = search_func(table->Root, fname);
          psa(elist, stat, id->str, func);
        }
      }else if(result == NEW_LINE){
        sematika(search_prem_type(table,id->str,fname),elist,0);
        if(stat){
          expFirst(elist);
          Pmain_nod func = search_func(table->Root, fname);
          psa(elist, stat, id->str, func);
        }
      }else{
        errors(SYNTAX);
      }
    }
    //zaďż˝ina zatvorkou
    else if(result == PAR_LEFT){
      ungetc(40,fp); // vratenie zďż˝tvorky, pokraďż˝ovanie v checkExp
      checkExp(0,elist,0);
      expFirst(elist);
      sematika(search_prem_type(table,id->str,fname),elist,0);
      if (stat){
        Pmain_nod func = search_func(table->Root, fname);
        psa(elist, stat, id->str, func);
      }

    }
    else if(result == IDENTIFY){

      if(search_PATO(table,s->str,fname) || search_PEPO(table,s->str,fname)){
        // je to premena
        if(search_PATO(table,s->str,fname))
          data->type = search_prem_type(table,s->str,fname);
        else
          data->type = typeFromList(table,s->str,fname);
        type = data->type = result;
        strCopyString(data->s,s);
        insertExp(elist,data);
        result = getToken(fp,s);
        if(result >= 1500 && result <= 1510){
          data->type = result;
          strCopyString(data->s,s);
          insertExp(elist,data);
          checkExp(0,elist,0);
          sematika(type,elist,0);

          if (stat){
            Pmain_nod func = search_func(table->Root, fname);
            expFirst(elist);
            psa(elist, stat, id->str, func);
          }
          if(search_PATO(table,s->str,fname))
            type = search_prem_type(table,s->str,fname);
          else
            type = typeFromList(table,s->str,fname);
        }else if(result == NEW_LINE){
          sematika(search_prem_type(table,id->str,fname),elist,0);
          if(stat){
            expFirst(elist);
            Pmain_nod func = search_func(table->Root, fname);
            psa(elist, stat, id->str, func);
          }
        }else{
          errors(SYNTAX);
        }
      }
      else if(search_PATO(table,NULL,s->str)){
        //je to funkcia ktorďż˝ existuje
        isCall = 1;
        call = malloc(s->length+1);
        strcpy(call,s->str);
        //cpy jej nďż˝zov do call -> volana funkcia
        if(getToken(fp,s) != PAR_LEFT)
          errors(SYNTAX);
        //x = funkcia(
        while(1){
          result = getToken(fp,s);
          if(result == TINTEGER || result == TDOUBLE || result == TSTRING){
            Insert(list,result,s->str);
          }else if(result == IDENTIFY){
            if(search_PATO(table,s->str,fname)){
              result = search_prem_type(table,s->str,fname);
              Insert(list,result,s->str);
            }else{
              errors(SDEF);
            }
          }else if (result == PAR_RIGHT) {
            break;
            // x = funkcia()
          }else{
            errors(SYNTAX);
          }

          result = getToken(fp,s);
          if(result == COMMA){
            ;
          }else if(result == PAR_RIGHT){
            break;
          }else{

            errors(SYNTAX);
          }
        }
        //print_list(list);
        if(!compare_whole_list_call(table,call,list)){
          errors(SPAR);
        }
        instr_add(insList,I_CALL,call,NULL,NULL,0,0);
        Pmain_nod func = search_func(table->Root, call);


        tList_item item = func->list->first;
        tList_item item2 = list->first;
        while(item != NULL){
          instr_add(insList,I_DEFVAR,item->item->name,NULL,NULL,0,0);
          instr_add(insList,I_MOVE,item->item->name,item2->item->name,NULL,item->item->type,item2->item->type);
          item = item->next;
          item2 = item2->next;
        }
        instr_add(insList,I_CALLEND,NULL,NULL,NULL,0,0);
        instr_add(insList,I_MOVE_RETVAL,id->str,NULL,NULL,0,0);
      }
      else{
        errors(SDEF);
       }
    }

  }
  else{
    errors(SDEF);
  }

  int funType = 0;
  int idType ;
  if(search_PATO(table,id->str,fname))
    idType = search_prem_type(table,id->str,fname);
  else
    idType = typeFromList(table,id->str,fname);

  if(isCall == 1)
    funType = search_func_type(table,call);
  if(idType == TINTEGER || idType == TDOUBLE){
    if(funType == TSTRING)
      errors(SPAR);
  }else if(idType == TSTRING){
    if(idType == TINTEGER || idType == TDOUBLE)
      errors(SPAR);
  }else{
    errors(SPAR);
  }

  if(isCall == 1)
    free(call);


  expListFree(elist);
  free(elist);
  //destroy_list(list->first);
  free(list);
  strFree(data->s);
  free(data->s);
  free(data);
  strFree(s);
  free(s);

}
/**
 * Funkcia pre hlavné telo programu. Volá sa rekurzívne.
 * @param   returnVal   typ návratovej hodnoty funkcie
 * @param   isScope     príznak, či sa nachádza v hlavnom tele programu
 * @param   isIF        príznak či sa nachádza v podmienenom príkaze
 * @return              //TODO
 */
int scope(int returnVal , int isScope , int isIF){
  string *s = malloc(sizeof(string));
  strInit(s);

  expList *elist;
  elist = malloc(sizeof(expList));
  expListInit(elist);
  int returniMiTotoProsim;
  int result,read = true;
  int expresion;
  int type;
  while(read){
    elist = malloc(sizeof(expList));
    expListInit(elist);
    result = getToken(fp,s);
    if(result == NEW_LINE){
        expListFree(elist);
        free(elist);
        continue;
    }
    if(result == KW_DIM && isIF == 0){
      declareID();
    }
    else if(result == IDENTIFY){
      checkID(s);
    }
    else if(result == KW_PRINT){
      print();
    }else if(result == KW_INPUT){
      input();
    }else if(result == KW_DO){
      checkWhile(isScope);
    }else if(result == KW_IF){
      checkExp(1,elist,0);

      if(expListLength(elist) < 3){
        errors(SPAR);
      }
      if(!expListOP(elist)){
        errors(SPAR);
      }
      expFirst(elist);
      if(expType(elist) == IDENTIFY){
        if(search_PATO(table,expVal(elist),fname)){
          type = search_prem_type(table,expVal(elist),fname);}
        else if(search_PEPO(table,expVal(elist),fname)){
          type = typeFromList(table,expVal(elist),fname);}
      }else{
        type = expType(elist);
      }
      Pmain_nod func = search_func(table->Root, fname);
      sematika(type,elist,1);
      psa(elist, result, NULL, func);
      expresion = scope(1,isScope,1);
      if(expresion != KW_ELSE)
        errors(SYNTAX);
      if(expresion == KW_ELSE){
        expresion = getToken(fp,s);
        if(expresion != NEW_LINE)
          errors(SYNTAX);
        if(scope(1,isScope,1) != KW_END)
          errors(SYNTAX);
        endIf();
        instr_add(insList,17,NULL,NULL,NULL,0,0);
      }else if(expresion == KW_END){
        endIf();
      }else{
        errors(SYNTAX);
      }
    }else if(result == KW_RETURN && returnVal && !isScope){
      checkExp(0,elist,0);
      Pmain_nod func = search_func(table->Root, fname);

//      sematika(search_func_type(table,fname),elist,0);

      expFirst(elist);
      psa(elist, KW_RETURN, NULL, func);

      //instr_add(insList,I_RETURN,expVal(elist),NULL,NULL,expType(elist),0);
    }else if(result == KW_ELSE){
      returniMiTotoProsim = KW_ELSE;
      instr_add(insList,I_ELSE,NULL,NULL,NULL,0,0);
      expListFree(elist);
      free(elist);
      break;
    }else if(result == KW_END){
      returniMiTotoProsim = KW_END;
      expListFree(elist);
      free(elist);
      break;
    }else if(result == KW_LOOP){
      returniMiTotoProsim = KW_LOOP;
      expListFree(elist);
      free(elist);
      break;
    }else{
      errors(SYNTAX);
    }
    expListFree(elist);
    free(elist);
  }// WHILE

  strFree(s);
  free(s);
  return returniMiTotoProsim;
}
/**
 * Vstupný bod syntaktického analyzátora. Hľadá hlavné telo programu. Volá jednotlivé funkcie formou rekurzívneho zostupu.
 * @return        1 pri správnosti, inak ukončuje program s kódom 2
 */
int parse(){
  patotmp=0;
  string *s = malloc(sizeof(string));
  strInit(s);
  table = Table_create();
  insList = listInit();

  int read = true;
  int result;
  int isScope = 0;
  while(read){
    result = getToken(fp,s);
    if(result == KW_SCOPE && isScope == 0){  // Nďż˝jdenie ďż˝asti scope
      isScope = 1;
      continue;
    }
    if(result == NEW_LINE && isScope == 0)
      continue;

    if(isScope){
      fname = malloc(strlen("scope"));
      strcpy(fname,"scope");
      instr_add(insList,I_MAIN,fname,NULL,NULL,0,0);// inďż˝trukcia scope
      insert_token(table,0,NULL,NULL,fname,NULL);
      scope(0,isScope,0);
      endScope();
      break;
    }else{
      if(result == KW_DECLARE){
        fundeclare();
        instr_add(insList,I_LABEL,fname,NULL,NULL,0,0);
        free(fname);
      }else if(result == KW_FUNCTION){
        fundef();
        while(1){
          result = scope(1,isScope,0);
          if(result == KW_END)
            break;
          else
            scope(1,isScope,0);
        }
        endFunction();
        instr_add(insList,I_ENDFUNC,NULL,NULL,NULL,0,0);
        free(fname);
      }else{

        errors(SYNTAX);
      }
    }
  }
  strFree(s);
  free(s);
  return 1;
}

/**
 * Funkcia pre overovanie sématickej správnosti.
 * @param    type   dátový typ premennej
 * @param    list   ukazovatel na zoznam pre výraz
 * @param    isIF   príznak pre podmienený príkaz
 */
void sematika(int type,expList *list ,int isIF){
  int read=true;
  expFirst(list);
  int thisType;
  if(type == KW_INTEGER || type == TINTEGER){
    while(expLast(list)){

      if(expType(list) == IDENTIFY){ // je to premenna
        if(search_PATO(table,expVal(list),fname)){ // je to premenan vo funkcii
          if(search_prem_type(table,expVal(list),fname) == TSTRING){
            errors(SPAR);
          }
        }else if(search_PEPO(table,expVal(list),fname)){ // je to premenna v parametroch funkcii
          if(typeFromList(table,expVal(list),fname) == TSTRING){
            errors(SPAR);
          }
        }else{
          errors(SDEF);
        }
        // GOTO kontrola premmenej
      }else if(expType(list) == OP_IDIV){
        if(thisType != TINTEGER || list->active->next->data->type != TINTEGER)
          errors(SPAR);
      }else if(expType(list) == TSTRING){
        errors(SPAR);
      }

      if(list->active->next == NULL){
        break;
      }
      thisType = expType(list);
      expNext(list);
    }
  }else if(type == KW_DOUBLE || type == TDOUBLE){
    while(expLast(list)){
      if(expType(list) == IDENTIFY){
        if(search_PATO(table,expVal(list),fname)){
          if(search_prem_type(table,expVal(list),fname) == TSTRING){
            errors(SPAR);
          }
        }else if(search_PEPO(table,expVal(list),fname)){
          if(typeFromList(table,expVal(list),fname) == TSTRING){
            errors(SPAR);
          }
        }else{
          errors(SDEF);
        }
      }else if(expType(list) == OP_IDIV){
        if(thisType != TINTEGER || list->active->next->data->type != TINTEGER)
          errors(SPAR);
      }else if(expType(list) == TSTRING){
        errors(SPAR);
      }
      if(list->active->next == NULL){
        break;
      }
      thisType = expType(list);
      expNext(list);
    }
  }

    else if(type == KW_STRING || type == TSTRING){
    int result;
    while(read){
    if(expType(list) == IDENTIFY){
        if(search_PATO(table,expVal(list),fname)){
            type = search_prem_type(table, expVal(list), fname);
            if (type != TSTRING)
              errors(SPAR);
        }else if (search_PEPO(table,expVal(list),fname)){
            type = typeFromList(table, expVal(list), fname);
            if (type != TSTRING)
              errors(SPAR);
        }else{
            errors(SDEF);
        }
        if(list->active->next == NULL)
            break;
        expNext(list);
        continue;
    }

      if(isIF){
        result = expType(list);
        switch (result) {
          case OP_EQ:       case OP_NOTEQ:
          case OP_BIGGEREQ: case OP_BIGGER:
          case OP_LOWEREQ:  case OP_LOWER:
          case OP_PLUS: case TSTRING: case KW_STRING:
          case PAR_LEFT: case PAR_RIGHT:
            break;
          default:
            errors(SPAR);
        }
      }else{
        result = expType(list);
        switch (result) {
          case OP_PLUS: case TSTRING: case KW_STRING:
          case PAR_LEFT: case PAR_RIGHT:
            break;
          default:
            errors(SPAR);
        }
      }
      if(list->active->next == NULL)
        break;
      else
        expNext(list);
    }
  }
}
