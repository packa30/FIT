/*  @Project: IFJ17
    @author: xgrofc00@stud.fit.vutbr.cz
    @author: xkrajc17@stud.fit.vutbr.cz
    @author: xfujac00@stud.fit.vutbr.cz
    @author: xpobis00@stud.fit.vutbr.cz
*/
#include "instrlist.h"

#define true 1

tInstr instrFill(int type, void *addr1, void *addr2, void *addr3,int type2,int type3){
  tInstr instr = malloc(sizeof(struct Instr));
  instr->instTyp=type;
  instr->addr1=addr1;
  instr->addr2=addr2;
  instr->addr3=addr3;
  instr->type2=type2;
  instr->type3=type3;
  return instr;
}

tListOfInstr listInit()
// funkce inicializuje seznam instrukci
{
  tListOfInstr L = malloc(sizeof(struct ListOfInstr));
  L->first  = NULL;
  L->last   = NULL;
  L->active = NULL;
  return L;
}
int instr_add(tListOfInstr List,int type,char *add1,char *add2, char *add3,int type2,int type3){
  char *addr1=NULL;
  char *addr2=NULL;
  char *addr3=NULL;
  if(add1 != NULL){
    addr1 = (char *)malloc(strlen(add1)+1);
    strcpy(addr1,add1);
  }
  if(add2 != NULL){
    addr2 = (char *)malloc(strlen(add2)+1);
    strcpy(addr2,add2);
  }
  if(add3 != NULL){
    addr3 = (char *)malloc(strlen(add3)+1);
    strcpy(addr3,add3);
  }
  tInstr instr = instrFill(type, (void *)addr1, (void *)addr2, (void *)addr3,type2,type3);
  listInsertLast(List,instr);
  return true;
}

void listFree(tListOfInstr L)
// funkce dealokuje seznam instrukci
{
  if(L!=NULL){
    tListItem ptr;
    while (L->first != NULL)
    {
      ptr = L->first;
      L->first = L->first->nextItem;
      // uvolnime celou polozku
      if (ptr->Instruction!=NULL){
        if(ptr->Instruction->addr1 != NULL){
          free(ptr->Instruction->addr1);
        }
        if(ptr->Instruction->addr2 != NULL)
          free(ptr->Instruction->addr2);
        if(ptr->Instruction->addr3 != NULL)
          free(ptr->Instruction->addr3);

        free(ptr->Instruction);
      }
      if (ptr!=NULL)
        free(ptr);
    }
    free(L);
  }
}


void listInsertLast(tListOfInstr L, tInstr I)
// vlozi novou instruci na konec seznamu
{
  if(L!=NULL){
    tListItem newItem;
    newItem = (tListItem)malloc(sizeof (struct listItem2));
    if(newItem!=NULL){
      newItem->Instruction = I;
      newItem->nextItem = NULL;
      if (L->first == NULL)
        L->first = newItem;
      else
        L->last->nextItem=newItem;
      L->last=newItem;
    }
  }
}

void listFirst(tListOfInstr L)
// zaktivuje prvni instrukci
{
  if(L!=NULL)
    L->active = L->first;
}

void listNext(tListOfInstr L)
// aktivni instrukci se stane nasledujici instrukce
{
  if(L!=NULL){
    if (L->active != NULL)
    L->active = L->active->nextItem;
  }
}

void listGoto(tListOfInstr L, void *gotoInstr)
// nastavime aktivni instrukci podle zadaneho ukazatele
// POZOR, z hlediska predmetu IAL tato funkce narusuje strukturu
// abstraktniho datoveho typu
{
  if(L!=NULL)
    L->active = (tListItem) gotoInstr;
}

void *listGetPointerLast(tListOfInstr L)
// vrati ukazatel na posledni instrukci
// POZOR, z hlediska predmetu IAL tato funkce narusuje strukturu
// abstraktniho datoveho typu
{
  if(L!=NULL)
    return (void*) L->last;
  return NULL;
}

tInstr *listGetData(tListOfInstr L)
// vrati aktivni instrukci
{
  if (L==NULL || L->active == NULL)
  {
    printf("Chyba, zadna instrukce neni aktivni");
    return NULL;
  }
  else return &(L->active->Instruction);
}

/*int main(){
  tListOfInstr *L = malloc(sizeof(tListItem));
  tInstr a = instrFill(I_ADD, "2", "5", NULL);
  listInit(L);
  listInsertLast(L, a);
  listFirst(L);
  tInstr *b = listGetData(L);
  printf("[%d,\t%s,\t%s,\t%s]\n",b->instTyp, (char *)b->addr1, (char *)b->addr2, (char *)b->addr3);
  listFree(L);
}*/
