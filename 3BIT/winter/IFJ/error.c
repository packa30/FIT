#include "error.h"
#include "parser.h"

void errors(int err) {
//printf("    Chyba:%s\n", s->str);
  //destroy_all(table->Root);

  switch (err) {
    case 1:
    //  fprintf(stderr, "Lexival error\n");
      exit(err);
    case 2:
    //  fprintf(stderr, "Syntax error\n");
      exit(err);
    case 3:
      //fprintf(stderr, "Definition error\n");
      exit(err);
    case 4:
      //fprintf(stderr, "Type error\n");
      exit(err);
    case 6:
    //  fprintf(stderr, "Others error\n");
      exit(err);
    case 99:
    //  fprintf(stderr, "Internal error\n");
      exit(err);
  }
}
