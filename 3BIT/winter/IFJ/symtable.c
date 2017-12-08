
/*  @Project: IFJ17
    @author: xgrofc00@stud.fit.vutbr.cz
    @author: xkrajc17@stud.fit.vutbr.cz
    @author: xfujac00@stud.fit.vutbr.cz
    @author: xpobis00@stud.fit.vutbr.cz
*/
#include<string.h>
#include"symtable.h"

void destroy(Pmain_table table){
  if(table->Root != NULL)
    destroy_all(table->Root);
  free(table);
}

void destroy_list(tList_item item){
  if(item == NULL)
    return;
  if(item->next != NULL){
    destroy_list(item->next);
  }
  destroy_symbols(item->item);
  free(item);
}
void destroy_all(Pmain_nod root){
  if(root != NULL){
    if(root->LPtr != NULL){
      destroy_all(root->LPtr);
    }
    if(root->RPtr != NULL){
      destroy_all(root->RPtr);
    }
    if(root->func_tree != NULL){
      STdestroy(root->func_tree);
    }
    if(root->list != NULL){
      destroy_list(root->list->first);
      free(root->list);
    }
    free(root);
  }

}
int compare_list_type(Pmain_table table, char *fname, tList list){
  Pmain_nod func = search_func(table->Root, fname);
  //print_list(func->list);
  //print_list(list);

  if(func == NULL)
    return false;
  if(func->list == NULL && list == NULL)
    return true;
  if(func->list == NULL || list == NULL)
    return false;

  if(func->list->first == NULL || list->first == NULL)
    return false;
  if(func->list->first == NULL && list->first == NULL)
    return true;
  return compare_2_lists(func->list->first,list->first);
}



int compare_2_lists(tList_item L_item, tList_item L2_item)
{
  tList_item item = L_item;
  tList_item item2 = L2_item;
  while(item != NULL && item2 !=NULL){
    if(item->item->type != item2->item->type)
      return false;
    item = item->next;
    item2 = item2->next;
  }
  if (item != NULL || item2 != NULL){return false;}
  return true;

}
int compare_whole_list_call(Pmain_table table, char *fname, tList list){
  Pmain_nod func = search_func(table->Root, fname);
  if(func == NULL)
    return false;
  if(func->list == NULL && list == NULL)
    return true;
  if(func->list == NULL || list == NULL)
    return false;
  if(func->list->first == NULL && list->first == NULL)
    return true;
  if(func->list->first == NULL || list->first == NULL)
    return false;
  return compare_2_lists_whole_call(func->list->first,list->first);
}

int compare_2_lists_whole_call(tList_item L_item, tList_item L2_item)
{
  tList_item item = L_item;
  tList_item item2 = L2_item;
  while(item != NULL && item2 !=NULL){
      if(item->item->type == TYPE_INT || item->item->type == 2001){
        if(item2->item->type == TYPE_DOUBLE || item2->item->type == TYPE_INT){
          ;
        }else if(item2->item->type == 1998 || item2->item->type == 2001){
          ;
        } else{
          return false;
        }
      }
      else if(item->item->type == TYPE_DOUBLE || item->item->type == 1998){
        if(item2->item->type == TYPE_DOUBLE || item2->item->type == TYPE_INT){
          ;
        }else if(item2->item->type == 1998 || item2->item->type == 2001){
          ;
        } else{
          return false;
        }
      }
      else if(item->item->type == TYPE_STRING || item->item->type == 1998){
        if(item2->item->type != TYPE_STRING && item2->item->type != 1998){
          return false;
        }
      }
      else{
        //printf("%i %i \n",item->item->type, item2->item->type );
        return false;
      }
    item = item->next;
    item2 = item2->next;
  }// END WHILE
  if (item != NULL || item2 != NULL){
    return false;
  }
  return true;
}
int compare_whole_list(Pmain_table table, char *fname, tList list){
  Pmain_nod func = search_func(table->Root, fname);
  if(func == NULL)
    return false;
  if(func->list == NULL && list == NULL)
    return true;
  if(func->list == NULL || list == NULL)
    return false;
  if(func->list->first == NULL && list->first == NULL)
    return true;
  if(func->list->first == NULL || list->first == NULL)
    return false;
  return compare_2_lists_whole(func->list->first,list->first);
}
int compare_2_lists_whole(tList_item L_item, tList_item L2_item)
{
  tList_item item = L_item;
  tList_item item2 = L2_item;
  while(item != NULL && item2 !=NULL){
    if(item->item->type != item2->item->type)
      return false;
    if(strcmp(item->item->name, item2->item->name) != 0)
      return false;
    item = item->next;
    item2 = item2->next;
  }
  if (item != NULL || item2 != NULL){return false;}
  return true;

}
tList Listinit(){
  tList L = malloc(sizeof(struct L_list));
  L->first = NULL;
	L->last = NULL;
	L->active = NULL;
  return L;
}

void Insert(tList L, int type, char *name){
	tList_item newItem = malloc(sizeof(struct listItem));
	if(newItem == NULL){errors(INTERNAL);}
  PSymbol new_sym = malloc(sizeof(TSymbol));
  if(new_sym == NULL){errors(INTERNAL);}
  new_sym->type = type;
  strcpy(new_sym->name, name);
	newItem->item = new_sym;
  newItem->next = NULL;

  if(L->first == NULL){
    L->first = newItem;
    L->last = L->first;
  }else{
    L->last->next = newItem;
    L->last = L->last->next;
  }
}

void print_list(tList L){
  tList_item list = L->first;
  while(list != NULL)
  {
    printf("%s  %d\n",list->item->name,list->item->type);
    list=list->next;
  }
}
int printf_tree(Pmain_nod nod){
  printf("%s\n", nod->name );
  if(nod->list != NULL)
    print_list(nod->list);
  if(nod->func_tree != NULL)
    printf_tree_params(nod->func_tree->Root);
  if(nod->LPtr != NULL)
    printf_tree(nod->LPtr);
  if(nod->RPtr != NULL)
    printf_tree(nod->RPtr);
  return 1;
}

int printf_tree_params(PSymbol sym){
  if(sym == NULL)
    return false;
  printf("%s\n", sym->name);
  if(sym->LPtr != NULL)
    printf_tree_params(sym->LPtr);
  if(sym->RPtr != NULL)
    printf_tree_params(sym->RPtr);
  return 1;
}
int search_PATO(Pmain_table table, char *name, char *fname){
  //printf("%s\n%s\n",name,fname);
  Pmain_nod func = search_func(table->Root, fname);
  if( func == NULL)
    return false;
  //printf("%s\n%s\n",func->name,fname );
  if(name == NULL){
    return true;
  }
  else{
    if(func->func_tree != NULL){
      PSymbol prem = STsearch(func->func_tree->Root, name);
      if( prem == NULL ){
        //printf("pojebali ti semena\n");
        return false;
      }
    }
    else{
      return false;
    }
  }
  //printf("Symbol name: %s, type: %d, value: %s, funkcia (vrchny nod ): %s\n", prem->name, prem->type, prem->value, func->name);
  return true;
}

int typeFromList(Pmain_table table,char*name,char*fname){
  Pmain_nod func = search_func(table->Root, fname);
  if (func == NULL){return false;}
  else{
    if(func->list != NULL){
    tList_item item = func->list->first;
      while(item != NULL){
        if(strcmp(name,item->item->name) == 0){return item->item->type;}
        item=item->next;
      }
    }
  }
  return false;
}
PSymbol varfromlist(Pmain_nod func, char *name){
  if(func->list != NULL){
    tList_item item = func->list->first;
    while(item != NULL){
      if(strcmp(name,item->item->name) == 0){return item->item;}
      item=item->next;
    }
  }
  return 0;
}

int search_PEPO(Pmain_table table,char *name,char *fname){
  Pmain_nod func = search_func(table->Root, fname);
  if (func == NULL){return false;}
  else{
    if(func->list != NULL){
    tList_item item = func->list->first;
      while(item != NULL){
        if(strcmp(name,item->item->name) == 0){return true;}
        item=item->next;
      }
    }
  }
  return false;
}
int search_func_type(Pmain_table table, char *fname){
  Pmain_nod func = search_func(table->Root, fname);
  if (func == NULL){return false;}
  return func->type;
}
int search_prem_type(Pmain_table table, char* name,char *fname){
  Pmain_nod func = search_func(table->Root, fname);
  if (func == NULL){return false;}
  else{
    PSymbol prem = STsearch(func->func_tree->Root, name);
    if( prem == NULL ){return false;}
    else{
      return prem->type;
    }
  }
  return false;


}
Pmain_nod search_func(Pmain_nod root, char *name){
    if (root == NULL){
      return NULL;
    }
    else {
      //printf("%s\n%s\n",root->name,name);
      if(strcmp(root->name, name)==0){
        //printf("kokotina na entu\n");
        return root;
 		  }
      else {
 			 if(strcmp(root->name, name) < 0) {
         //printf("okay mal by som byt tu\n");
 				 return search_func(root->LPtr, name);
 			 }
       else{
 				 return search_func(root->RPtr, name);
 		   }
     }
   }
    return NULL;
}


Pmain_nod insert_token_recursive(Pmain_nod *parent, int type, char *name, char *value, char *fname){
  if(*parent == NULL){
    Pmain_nod nod = (Pmain_nod)malloc(sizeof(main_nod));
    if(nod == NULL){errors(INTERNAL);}
    strcpy(nod->name, fname);
    nod->LPtr=NULL;
    nod->RPtr=NULL;
    nod->type=type;
    if(name == NULL ){}
    else{
      PSymtab table = STcreate();
      if(STinsert(table, type, name, value) != 1){errors(INTERNAL);}
      nod->func_tree=table;
    }
    *parent=nod;
    return nod;
  }
  else{
    //printf("%s          %s\n",(*parent)->name, fname );
    if (strcmp((*parent)->name, fname) < 0){
      return insert_token_recursive(&((*parent)->LPtr), type, name, value, fname);
    }
    else { // do praveho
      if(strcmp((*parent)->name, fname) > 0){
          return insert_token_recursive(&((*parent)->RPtr), type, name, value, fname);
      }
      else {
            if((*parent)->func_tree == NULL){
              PSymtab table = STcreate();
              if(STinsert(table, type, name, value) != 1){errors(INTERNAL);}
              (*parent)->func_tree=table;
            }
            else{
              if(STinsert((*parent)->func_tree, type, name, value) != 1){errors(INTERNAL);}
            }
            return (*parent);
      }

    }

  }


}


int insert_token(Pmain_table table, int type, char *name, char *value, char *fname, tList list){
  if(fname == NULL || table == NULL){errors(INTERNAL);}
  if (name==NULL ){
    if(list == NULL){
        Pmain_nod func = search_func(table->Root, fname);
        if(func != NULL){return false;}
        if(insert_token_recursive(&(table->Root), type, name, value, fname) != NULL)
          return true;
        return false;

    }
    else{
      Pmain_nod func = search_func((table->Root), fname);
      if(func != NULL){return false;}
      Pmain_nod nod = insert_token_recursive(&(table->Root), type, name, value, fname);
      if( nod == NULL)
        return false;
      nod->list=list;
      return true;
    }
  }
  else{
    if(insert_token_recursive(&(table->Root), type, name, value, fname) != NULL)
      return true;
    return false;
  }
}
void func_define(Pmain_table table, char *fname){
    if(fname == NULL || table == NULL){errors(INTERNAL);}
    Pmain_nod func = search_func(table->Root, fname);
    func->defined=true;
}
int is_defined(Pmain_table table, char *fname){
  if(fname == NULL || table == NULL){errors(INTERNAL);}
  Pmain_nod func = search_func(table->Root, fname);
  if(func == NULL){
    return false;
  }
  if(func->defined == true)
    return true;
  return false;
}
Pmain_table Table_create(){
  Pmain_table table = (Pmain_table)malloc(sizeof(main_table));
  if(table == NULL ){errors(INTERNAL);}
  table->Root=NULL;
  return table;
}




/*  @brief Vytvori tabulku symbolov, neprijima argumenty
  @return vracia ukazatel na vytvoreny korenovy uzol tabulky
*/
PSymtab STcreate(){
    PSymtab table = (PSymtab)malloc(sizeof(TSymtab));
    if(table==NULL){
        errors(INTERNAL); // ??? error malloc
    }

    table->Root=NULL;
    return table;
}

/* POmocna funkcia pre rekurzivne vkladanie symbolu do stromu
  @param parent ukazatel na rodicovsky uzol
  @param type typ vkladanej hodnoty
  @param name nazov/kluc vkladaneho symbolu
  @return vracia ukazatel na vkladany symbol alebo NULL
*/
PSymbol STinsert_fun(PSymbol *parent, int type, char *name, char *value){
    //printf("%s               %s\n",(*parent)->name, name );
    if (*parent==NULL){ //vkladam na zaciatok
      PSymbol symbol = (PSymbol)malloc(sizeof(TSymbol));
      if(symbol==NULL){
        errors(INTERNAL);
      } else {
        if(value != NULL){
          symbol->value=(char *)malloc(strlen(value)+1);
          strcpy(symbol->value, value);
        }
        symbol->type = type;
        strcpy(symbol->name, name);
        symbol->LPtr=NULL;
        symbol->RPtr=NULL;
        *parent=symbol;
        //printf("Inserting symbol: %s [%s, %s]\n", symbol->name, symbol->value, symbol->fname);
        return(symbol);
      }
    }
    else {  //vkladat do laveho podstromu
      if (strcmp((*parent)->name, name) < 0){
        return STinsert_fun(&((*parent)->LPtr), type, name, value);
      }
      else { // do praveho
        if(strcmp((*parent)->name, name) > 0){
            return STinsert_fun(&((*parent)->RPtr), type, name, value);
        }
        else {  //je tam ten kluc
            if((*parent)->value == NULL){
              (*parent)->value = malloc(strlen(value)+1);
            }
            strcpy((*parent)->value, value);
            return (*parent);
        }

      }
    }
  return NULL;
}

/*Vlozi symbol do tabulky symbolov
  @param table Ukazatel na tabulku
  @param type vkaldany typ
  @param name vkladany nazov/kluc symbolu
  @return vracia ukazatel na vlozeny symbol alebo zhodi program :D
*/
int STinsert(PSymtab table, int type, char *name, char *value){
  if (table==NULL || name==NULL){
    errors(INTERNAL);
  }
  if(STinsert_fun(&(table->Root), type, name, value) != NULL)
    return 1;
  return 0;

}

/* Zrusi symbol a vsetkych synov
  @param root Ukazatel na symbol
*/
void destroy_symbols(PSymbol root){
  if(root!=NULL){
  	destroy_symbols(root->LPtr);
  	destroy_symbols(root->RPtr);
  	//free(root->name);
    free(root->value);
    free(root);
  }
  return;
}

/* Vyhlada symbol v tabulke
  @param root ukazatel na korenovy symbol (nie tabulka)
  @param name ukazatel na hladany retazec
  @return vracia ukazatel na najdeny symbol alebo NULL ak nenajde
*/
PSymbol STsearch(PSymbol root, char *name){
    if (root == NULL){
      return NULL;
    }
    else {
      //printf("%s\n%s\n",root->name, name );
      if(strcmp(root->name, name)==0){
        return root;
 		  }
      else {								/* Kluc by mal lezat v pravom podstrome */
 			 if(strcmp(root->name, name) < 0) {
 				 return STsearch(root->LPtr, name);
 			 }
       else{
 				 return STsearch(root->RPtr, name);
 		   }
     }
   }
}

/* @brief Uvolni tabulku volanim funkcie pre odstranenie vsetkych symbolov
  @param table ukazatel na tabulku symbolov
 */
void STdestroy(PSymtab table){
  if (table==NULL) return;
  destroy_symbols(table->Root);
  free(table);
}

int insert_generate_token(Pmain_table table, char *name, char *fname){
  if(fname == NULL || table == NULL){errors(INTERNAL);}
  if (name!=NULL ){
      Pmain_nod func = search_func(table->Root, fname);
      if(func == NULL){return false;}
      if(func->func_tree == NULL){
        PSymtab table = STcreate();
        if(STinsert(table, 0, name, NULL) != 1){errors(INTERNAL);}
        func->func_tree=table;
      }
      else{
        if(STinsert(func->func_tree, 0, name, NULL) != 1){errors(INTERNAL);}
      }

      /*if(insert_generate_token_recursive(&(table->Root), name, fname) != false)
        return true;
      return false;*/
  }
  return true;
}
int insert_generate_token_recursive(Pmain_nod *parent,char *name, char *fname){
  if(*parent == NULL){
    return false;
  }
  else{
    //printf("%s          %s\n",(*parent)->name, fname );
    if (strcmp((*parent)->name, fname) < 0){
      return insert_generate_token_recursive(&((*parent)->LPtr), name, fname);
    }
    else { // do praveho
      if(strcmp((*parent)->name, fname) > 0){
          return insert_generate_token_recursive(&((*parent)->RPtr), name, fname);
      }
      else {
            if((*parent)->func_tree == NULL){
              PSymtab table = STcreate();
              if(STinsert(table, 0, name, NULL) != 1){errors(INTERNAL);}
              (*parent)->func_tree=table;
            }
            else{
              if(STinsert((*parent)->func_tree, 0, name, NULL) != 1){errors(INTERNAL);}
            }
            return true;
      }

    }

  }


}
