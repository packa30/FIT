/*  @Project: IFJ17
    @author: xgrofc00@stud.fit.vutbr.cz
    @author: xkrajc17@stud.fit.vutbr.cz
    @author: xfujac00@stud.fit.vutbr.cz
    @author: xpobis00@stud.fit.vutbr.cz
*/

//#include "instrlist.h"
#include "generate.h"
void jednoduche_volaniefunkcie_zakl_operatory_rovnaky_typ();
void factorial_instrukcie();
void jednoduchy_while();
int main(){
  //factorial_instrukcie();
  //jednoduche_volaniefunkcie_zakl_operatory_rovnaky_typ();
  jednoduchy_while();
  return 0;
}
void jednoduche_volaniefunkcie_zakl_operatory_rovnaky_typ(){
  tListOfInstr list = listInit();
  if(instr_add(list,I_LABEL,"jebat",NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_DEFVAR,"l",NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_MUL,"l","k","10",TYPE_INT,TYPE_INT) == false){printf("what the fuck\n");}
  if(instr_add(list,I_RETURN,"l",NULL,NULL,TYPE_INT,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_ENDFUNC,NULL,NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_MAIN,"scope",NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_DEFVAR,"a",NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_DEFVAR,"b",NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_MOVE,"a","1",NULL,TYPE_INT,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_MOVE,"b","10",NULL,TYPE_INT,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_DEFVAR,"c",NULL,NULL,TYPE_INT,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_ADD,"c","a","b",TYPE_INT,TYPE_INT) == false){printf("what the fuck\n");}
  if(instr_add(list,I_CALL,"jebat",NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_DEFVAR,"k",NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_MOVE,"k","c",NULL,TYPE_INT,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_CALLEND,NULL,NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_MOVE_RETVAL,"c",NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_WRITE,"c",NULL,NULL,0,0) == false){printf("what the fuck\n");}



  tList list2 = Listinit();
  Insert(list2, TYPE_INT, "k");
  Pmain_table table = Table_create();


  if(insert_token(table, TYPE_INT, NULL, NULL, "jebat", list2) != true){errors(INTERNAL);}
  if(insert_token(table, TYPE_INT, "l", NULL, "jebat", NULL) != true){errors(INTERNAL);}
  if(insert_token(table, TYPE_INT, "a", NULL, "scope", NULL) != true){errors(INTERNAL);}
  if(insert_token(table, TYPE_INT, "b", NULL, "scope", NULL) != true){errors(INTERNAL);}
  if(insert_token(table, TYPE_INT, "c", NULL, "scope", NULL) != true){errors(INTERNAL);}

  generate(list,table);
  listFree(list);
  free(list);
  destroy(table);
}
void factorial_instrukcie(){
  tList list2 = Listinit();
  Insert(list2, TYPE_INT, "n");
  Pmain_table table = Table_create();
  if(insert_token(table, TYPE_INT, NULL, NULL, "factorial", list2) != true){errors(INTERNAL);}
  if(insert_token(table, TYPE_INT, "temp_result", NULL, "factorial", NULL) != true){errors(INTERNAL);}
  if(insert_token(table, TYPE_INT, "decremented_n", NULL, "factorial", NULL) != true){errors(INTERNAL);}
  if(insert_token(table, TYPE_INT, "result", NULL, "factorial", NULL) != true){errors(INTERNAL);}
  if(insert_token(table, TYPE_INT, NULL, NULL, "scope", NULL) != true){errors(INTERNAL);}
  if(insert_token(table, TYPE_INT, "a", NULL, "scope", NULL) != true){errors(INTERNAL);}
  if(insert_token(table, TYPE_INT, "vysl", NULL, "scope", NULL) != true){errors(INTERNAL);}

  tListOfInstr list = listInit();
  if(instr_add(list,I_LABEL,"factorial",NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_DEFVAR,"temp_result",NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_DEFVAR,"decremented_n",NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_DEFVAR,"result",NULL,NULL,0,0) == false){printf("what the fuck\n");}

  if(instr_add(list,I_IF,NULL,NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_LT,NULL,"n","2",TYPE_INT,TYPE_INT) == false){printf("what the fuck\n");}
  if(instr_add(list,I_MOVE,"result","1",NULL,TYPE_INT,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_ELSE,NULL,NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_SUB,"decremented_n","n","1",0,TYPE_INT) == false){printf("what the fuck\n");}
  if(instr_add(list,I_CALL,"factorial",NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_DEFVAR,"n",NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_MOVE,"n","decremented_n",NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_CALLEND,NULL,NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_MOVE_RETVAL,"temp_result",NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_MUL,"result","n","temp_result",0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_IFEND,NULL,NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_RETURN,"result",NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_ENDFUNC,NULL,NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_MAIN,"scope",NULL,NULL,0,0) == false){printf("what the fuck\n");}

  if(instr_add(list,I_DEFVAR,"a",NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_DEFVAR,"vysl",NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_WRITE,"Zadajte cislo pro vypocet faktorialu\n",NULL,NULL,TYPE_STRING,0) == false){printf("what the fuck\n");}

  if(instr_add(list,I_INPUT,"a",NULL,NULL,TYPE_INT,0) == false){printf("what the fuck\n");}
  //if(instr_add(list,I_MOVE,"a","4",NULL,TYPE_INT,0) == false){printf("what the fuck\n");}

  if(instr_add(list,I_IF,NULL,NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_GT,NULL,"0","a",TYPE_INT,TYPE_INT) == false){printf("what the fuck\n");}
  if(instr_add(list,I_WRITE,"\nFacto\\rial ne#lze spocitat\n",NULL,NULL,TYPE_STRING,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_ELSE,NULL,NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_IF,NULL,NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_EQ,NULL,"a","10",TYPE_INT,TYPE_INT) == false){printf("what the fuck\n");}
  if(instr_add(list,I_WRITE,"Pepo ty si najsamlepsi na svete\n",NULL,NULL,TYPE_STRING,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_ELSE,NULL,NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_WRITE,"Skus znova\n",NULL,NULL,TYPE_STRING,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_IFEND,NULL,NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_CALL,"factorial",NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_DEFVAR,"n",NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_MOVE,"n","a",NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_CALLEND,NULL,NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_MOVE_RETVAL,"vysl",NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_WRITE,"Vysledek je:",NULL,NULL,TYPE_STRING,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_WRITE,"vysl",NULL,NULL,TYPE_INT,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_WRITE,"\n",NULL,NULL,TYPE_STRING,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_IFEND,NULL,NULL,NULL,0,0) == false){printf("what the fuck\n");}

  generate(list,table);
  listFree(list);
  free(list);
  destroy(table);
}
void jednoduchy_while(){
  tList list2 = Listinit();
  Insert(list2, TYPE_INT, "n");
  Pmain_table table = Table_create();
  if(insert_token(table, TYPE_INT, NULL, NULL, "scope", NULL) != true){errors(INTERNAL);}
  if(insert_token(table, TYPE_INT, "a", NULL, "scope", NULL) != true){errors(INTERNAL);}
  if(insert_token(table, TYPE_INT, "vysl", NULL, "scope", NULL) != true){errors(INTERNAL);}

  tListOfInstr list = listInit();
  if(instr_add(list,I_MAIN,"scope",NULL,NULL,0,0) == false){printf("what the fuck\n");}

  if(instr_add(list,I_DEFVAR,"a",NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_DEFVAR,"vysl",NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_MOVE,"vysl","0",NULL,TYPE_INT,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_WRITE,"zadajte velkost kokota\n",NULL,NULL,TYPE_STRING,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_INPUT,"a",NULL,NULL,TYPE_INT,0) == false){printf("what the fuck\n");}
  //if(instr_add(list,I_MOVE,"a","4",NULL,TYPE_INT,0) == false){printf("what the fuck\n");}

  if(instr_add(list,I_WRITE,"\tO|\t\t\t|O\n",NULL,NULL,TYPE_STRING,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_CYCLE,NULL,NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_LT,NULL,"vysl","a",TYPE_INT,TYPE_INT) == false){printf("what the fuck\n");}
  if(instr_add(list,I_WRITE,"\t |\t\t\t|\n",NULL,NULL,TYPE_STRING,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_ADD,"vysl","vysl","1",TYPE_INT,TYPE_INT) == false){printf("what the fuck\n");}
  if(instr_add(list,I_CYCLEEND,NULL,NULL,NULL,0,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_WRITE,"\t -------------------------\n",NULL,NULL,TYPE_STRING,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_WRITE,"\t !           |           !\n",NULL,NULL,TYPE_STRING,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_WRITE,"\t !           |           !\n",NULL,NULL,TYPE_STRING,0) == false){printf("what the fuck\n");}
  if(instr_add(list,I_WRITE,"\t ------------|------------\n",NULL,NULL,TYPE_STRING,0) == false){printf("what the fuck\n");}

  generate(list,table);
  listFree(list);
  free(list);
  destroy(table);
}
