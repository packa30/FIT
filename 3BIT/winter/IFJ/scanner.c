#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "scanner.h"

int isEnd(char c){
    if(isspace(c))
      return true;
    switch (c) {
      case '(': case ')': case ';':
      case '<': case '>': case '=':
      case '/': case '+': case '-':
      case '*': case ',': case '!':
      case '\\':
      return true;
    }
    return false;
}

int isKeyword(string *str) {
  /*
    if(!(strcmp(str->str,"_" )))
      errors(LEX);
      */
    if(!(strcmp(str->str,"as")))
        return KW_AS;
    if(!(strcmp(str->str,"dim")))
        return KW_DIM;
    if(!(strcmp(str->str,"asc")))
        return KW_ASC;
    if(!(strcmp(str->str,"declare")))
        return KW_DECLARE;
    if(!(strcmp(str->str,"do")))
        return KW_DO;
    if(!(strcmp(str->str,"double")))
        return KW_DOUBLE;
    if(!(strcmp(str->str,"else")))
        return KW_ELSE;
    if(!(strcmp(str->str,"end")))
        return KW_END;
    if(!(strcmp(str->str,"chr")))
        return KW_CHR;
    if(!(strcmp(str->str,"function")))
        return KW_FUNCTION;
    if(!(strcmp(str->str,"if")))
        return KW_IF;
    if(!(strcmp(str->str,"input")))
        return KW_INPUT;
    if(!(strcmp(str->str,"integer")))
        return KW_INTEGER;
    if(!(strcmp(str->str,"length")))
        return KW_LENGTH;
    if(!(strcmp(str->str,"loop")))
        return KW_LOOP;
    if(!(strcmp(str->str,"print")))
        return KW_PRINT;
    if(!(strcmp(str->str,"return")))
        return KW_RETURN;
    if(!(strcmp(str->str,"scope")))
        return KW_SCOPE;
    if(!(strcmp(str->str,"string")))
        return KW_STRING;
    if(!(strcmp(str->str,"substr")))
        return KW_SUBSTR;
    if(!(strcmp(str->str,"then")))
        return KW_THEN;
    if(!(strcmp(str->str,"while")))
        return KW_WHILE;
    if(!(strcmp(str->str,"and")))
        return KW_AND;
    if(!(strcmp(str->str,"boolean")))
        return KW_BOOLEAN;
    if(!(strcmp(str->str,"continue")))
        return KW_CONTINUE;
    if(!(strcmp(str->str,"elseif")))
        return KW_ELSEIF;
    if(!(strcmp(str->str,"exit")))
        return KW_EXIT;
    if(!(strcmp(str->str,"false")))
        return KW_FALSE;
    if(!(strcmp(str->str,"for")))
        return KW_FOR;
    if(!(strcmp(str->str,"next")))
        return KW_NEXT;
    if(!(strcmp(str->str,"not")))
        return KW_NOT;
    if(!(strcmp(str->str,"or")))
        return KW_OR;
    if(!(strcmp(str->str,"shared")))
        return KW_SHARED;
    if(!(strcmp(str->str,"static")))
        return KW_STATIC;
    if(!(strcmp(str->str,"true")))
        return KW_TRUE;

    return false;
}

int getToken(FILE *fp , string *str){
  int returnVal = 0;
  int c ;
  int read = true;
  int state_before;
  int state = START;
  strClear(str);
  while(read){
    c = fgetc(fp);
    if(c == '#')
      errors(LEX);
    switch (state) {
        case START:
          if(isspace(c) || c == '\n'){
            if(c =='\n'){
              strAddChar(str,tolower(c));
              returnVal = NEW_LINE;
              read = false;
            }
          }else if(isalpha(c) || c == '_'){
            strAddChar(str,tolower(c));
            returnVal = state = IDENTIFY;
          }else if(c == '!'){
            returnVal = state = TSTRING;
          }else if(isdigit(c)){
            strAddChar(str,tolower(c));
            returnVal = state = TINTEGER;
          }else if(c == '\''){
            strAddChar(str,tolower(c));
            state_before = state;
            state = LN_KOMENT;
          }else if(c == '/'){
            strAddChar(str,tolower(c));
            if((c = fgetc(fp)) == '\''){
              state_before = state;
              state = BL_KOMENT;
            }else{
              ungetc(c,fp);
              returnVal = OP_DIV;
              read = false;
            }
          }else{
            strAddChar(str,tolower(c));
            switch (c) {
              case EOF:
                return EOF;
              case '(':
                returnVal = PAR_LEFT;   read = false;
                break;
              case ')':
                returnVal = PAR_RIGHT;  read = false;
                break;
              case ';':
                returnVal = SEMICOLON;  read = false;
                break;
              case '+':
                returnVal = OP_PLUS;    read = false;
                break;
              case '-':
                returnVal = OP_MINUS;   read = false;
                break;
              case '*':
                returnVal = OP_MUL;     read = false;
                break;
              case '/':
                returnVal = OP_DIV;     read = false;
                break;
              case '\\':
                returnVal = OP_IDIV;     read = false;
                break;
              case '=':
                returnVal = OP_EQ;      read = false;
                break;
              case '<':
                returnVal = state = OP_LOWER;
                break;
              case '>':
                returnVal = state = OP_BIGGER;
                break;
              case ',':
                returnVal = COMMA ; read = false;
                break;
              default:
                errors(LEX);
            }
          }
          break; // end START

        case LN_KOMENT:
          if(c == '\n' || c == EOF){
            strRemoveChar(str);
            state = state_before;
            ungetc(c,fp);
          }
          break; //LN_KOMENT

        case BL_KOMENT:
          if(c == EOF){
            errors(LEX);
          }else if(c == '\''){
            if((c = fgetc(fp)) == '/'){
              state = state_before;
              strRemoveChar(str);

              if(state == TINTEGER || state == DOT || state == EXP_OP_DIGIT || state == IDENTIFY){
                read = false;
              }
            }
          }
          break; // end BL_KOMENT

        case OP_LOWER:
          strAddChar(str,tolower(c));
          if(c == '>'){
            returnVal = OP_NOTEQ;
          }else if(c == '='){
            returnVal = OP_LOWEREQ;
          }else if(c == '\''){
            state_before = state;
            state = LN_KOMENT;
          }else if(c == '/'){
            if((c = fgetc(fp)) == '\''){
              state_before = state;
              state = BL_KOMENT;
            }else{
              ungetc(c,fp);
              strRemoveChar(str);
            }
          }else{
            ungetc(c,fp);
            strRemoveChar(str);
          }
          read = false;
          break; // end OP_LOWER

        case OP_BIGGER:
          strAddChar(str,tolower(c));
          if(c == '='){
            returnVal = OP_BIGGEREQ;
          }else if(c == '\''){
            state_before = state;
            state = LN_KOMENT;
          }else if(c == '/'){
            if((c = fgetc(fp)) == '\''){
              state_before = state;
              state = BL_KOMENT;
            }else{
              ungetc(c,fp);
              strRemoveChar(str);
            }
          }else{
            ungetc(c,fp);
            strRemoveChar(str);
          }
          read = false;
          break; // end OP_BIGGER

        case TINTEGER:
          strAddChar(str,tolower(c));
          if(isdigit(c)){
            ;
          }else if(c == '\''){
            state_before = state;
            state = LN_KOMENT;
          }else if(c == '/'){
            if((c = fgetc(fp)) == '\''){
              state_before = state;
              state = BL_KOMENT;
            }else{
              ungetc(c,fp);
              ungetc(47,fp);
              strRemoveChar(str);
              read = false;
            }
          }else if(c == '.'){
            returnVal = TDOUBLE;
            state = DOT;
          }else if(c == 'e' || c == 'E'){
            returnVal = TDOUBLE;
            state = EXPONENT;
          }else if(isEnd(c)){
            ungetc(c,fp);
            strRemoveChar(str);
            read = false;
          }else{
            errors(LEX);
          }
          // GOTO
          break; // end integer

        case DOT:
          strAddChar(str,tolower(c));
          if(isdigit(c)){
            ;
          }else if(c == 'e' || c == 'E'){
            state = EXPONENT;
          }else if(c == '\''){
            state_before = state;
            state = LN_KOMENT;
          }else if(c == '/'){
            if((c = fgetc(fp)) == '\''){
              state_before = state;
              state = BL_KOMENT;
            }else{
              ungetc(c,fp);
              ungetc(47,fp);
              strRemoveChar(str);
              read = false;
            }
          }else if(isEnd(c)){
            ungetc(c,fp);
            strRemoveChar(str);
            read = false;
          }else{
            errors(LEX);
          }
          break; // end DOT3

        case EXPONENT:
          strAddChar(str,tolower(c));
          if(isdigit(c)){
            state = EXPONENT2;
          }else if(c == '+' || c == '-'){
            state = EXP_OP;
          }else{
            errors(LEX);
          }
          break; // end EXPONENT

        case EXPONENT2:
          strAddChar(str,tolower(c));
          if(isdigit(c)){
            ;
          }else if(c == '+' || c =='-'){
            read = false;
            ungetc(c,fp);
            strRemoveChar(str);
          }
          else if(isEnd(c)){
            strRemoveChar(str);
            ungetc(c,fp);
            read = false;
          }else{
            errors(LEX);
          }
          break; // end EXPONENT

        case EXP_OP:
          strAddChar(str,tolower(c));

          if(isdigit(c)){
            state = EXP_OP_DIGIT;
          }else if(c == '+' || c == '-'){
            read = false;
            ungetc(c,fp);
          }else{
            errors(LEX);
          }
          break; // end EXP_OP

        case EXP_OP_DIGIT:
          strAddChar(str,tolower(c));
          if(isdigit(c)){
            ;
          }else if(c == '\''){
            state_before = state;
            state = LN_KOMENT;
          }else if(c == '/'){
            if((c = fgetc(fp)) == '\''){
              state_before = state;
              state = BL_KOMENT;
            }else{
              ungetc(c,fp);
              ungetc(47,fp);
              strRemoveChar(str);
              read = false;
            }
          }else if(isEnd(c)){
            strRemoveChar(str);
            ungetc(c,fp);
            read = false;
          }else{
            errors(LEX);
          }
          break; // EXP_OP_DIGIT

        case TSTRING:
          if(c == '"'){
            state = STR_START;
          }else{
            errors(LEX);
          }
          break; // end KW_STRING

        case STR_START:
          strAddChar(str,c);//tolower(c));
          if(c == EOF || c == '\n'){
            errors(LEX);
          }else if(c == '"'){
            strRemoveChar(str);
            read = false;
          }else if(c == '\\'){
            state = STR_SPEC;
            //strRemoveChar(str);
          }else {
            ;
          }
          break; // end STR_START

        case STR_SPEC:
          strAddChar(str,c);//tolower(c));
          if(c == '0'){
            state = ZERO;
          }else if(c == '1'){
            state = OKEY;
          }else if(c == '2'){
            state = OKEY2;
          }else if(c == '"' || c == 'n' || c == 't' || c == '\\'){
            state = STR_START;
            strRemoveChar(str);
            if(c == '\"'){
              strRemoveChar(str);
              strAddChar(str,'\"');
            }
            if(c == 't'){
              strRemoveChar(str);
              strAddChar(str,'\t');
            }
            if(c == 'n'){
              strRemoveChar(str);
              strAddChar(str,'\n');
            }
            if(c == '\\'){
              strRemoveChar(str);
              strAddChar(str,'\\');
            }
          }else{
            errors(LEX);
          }
          break; // end STR_SPEC

        case ZERO:
          strAddChar(str,tolower(c));
          if(c == '3'){
            state = ZERO1;
          }else if(c > '3' && c <= '9'){
            state = OKEY1;
          }else{
            errors(LEX);
          }
          break; // end ZERO

        case ZERO1:
          strAddChar(str,tolower(c));
          if(c > '1' && c <= '9'){
            state = STR_START;
          }else{
            errors(LEX);
          }
          break; // end ZERO1

        case OKEY:
          strAddChar(str,tolower(c));
          if(c >= '0' && c <= '9'){
            state = OKEY1;
          }else{
            errors(LEX);
          }
          break; // end OKEY

        case OKEY1:
          strAddChar(str,tolower(c));
          if(c >= '0' && c < '9'){
            state = STR_START;
          }else{
            errors(LEX);
          }
          break; // end OKEY1

        case OKEY2:
          strAddChar(str,tolower(c));
          if(c == '5'){
            state = OKEY3;
          }else if(c >= '0' && c <= '4'){
            state = OKEY1;
          }else{
            errors(LEX);
          }
          break; // end OKEY2

        case OKEY3:
          strAddChar(str,tolower(c));
          if(c >= '0' && c <= '5'){
            state = STR_START;
          }else{
            errors(LEX);
          }
          break;

        case IDENTIFY:
          strAddChar(str,tolower(c));
          if(isalnum(c) || c == '_'){
            ;
          }else if(c == '\''){
            state_before = state;
            state = LN_KOMENT;
          }else if(c == '/'){
            if((c = fgetc(fp)) == '\''){
              state_before = state;
              state = BL_KOMENT;
            }else{
              ungetc(c,fp);
              ungetc(47,fp);
              strRemoveChar(str);
              read = false;
              if(isKeyword(str)){
                returnVal = isKeyword(str);
              }
            }
          }else if(isEnd(c)){
            strRemoveChar(str);
            read = false;
            ungetc(c,fp);

            if(isKeyword(str)){
              returnVal = isKeyword(str);
            }
          }else{
            errors(LEX);
          }

          break; // end IDENTIFY
    }
  }
  //printf("%s ", str->str);
  return returnVal;
}
