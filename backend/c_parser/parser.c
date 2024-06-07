#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


typedef enum {
  TOK_UNKNOWN,
  TOK_LITTERAL,
  TOK_NUMBER,
  TOK_DIRECTIVE,
  TOK_VARTYPE,
  TOK_FUNCTION,
  TOK_STRING,
  TOK_OPERATOR,
  TOK_KEYWORD,
  TOK_EOF
} TokenType;


const char* keywords[] = {
    "auto",
    "break",
    "case",
    "char",
    "const",
    "continue",
    "default",
    "do",
    "double",
    "else",
    "enum",
    "extern",
    "float",
    "for",
    "goto",
    "if",
    "inline",
    "int",
    "long",
    "register",
    "restrict",
    "return",
    "short",
    "signed",
    "sizeof",
    "static",
    "struct",
    "switch",
    "typedef",
    "union",
    "unsigned",
    "void",
    "volatile",
    "while"
};
const int numKeywords = sizeof(keywords) / sizeof(keywords[0]);

int isKeyword(char* tok) {
  for (int i = 0; i < numKeywords; ++i) {
    if (strcmp(keywords[i], tok) == 0) {
      return 1;
    }
  }
  return 0;
}

typedef struct {
  TokenType type;
  char* content;
} Token;

Token* createToken(TokenType type, char* content) {
  Token* tok = (Token*)malloc(sizeof(Token));
  tok->type = type;
  tok->content = strdup(content);
  return tok;
}

Token* getNextToken(char* code, int* index) {
  int start;
  char buffer[256];
  while (code[*index] != '\0') {
    if (isspace(code[*index]) && 0) {
      (*index)++;
      continue;
    }
  

    if (isdigit(code[*index])) {
      start = *index;
      while (isdigit(code[*index])) {
        (*index)++;
      }
      strncpy(buffer, code +start, *index-start);
      buffer[*index-start] = '\0';
      return createToken(TOK_NUMBER, buffer);

    }

    if (code[*index] == '"') {
      start = *index; 
      (*index)++;
      while (code[(*index)] != '"' && code[*index] != '\0') {
        (*index)++;
      }
      (*index)++;
      strncpy(buffer, code +start, *index-start);
      buffer[*index-start] = '\0';
      return createToken(TOK_STRING, buffer);
    }

    if (strchr(",()><+-*/{}[]=;", code[*index]) != NULL) {
      start = *index;
      if (strchr("><+-=*/", code[*index]) != NULL) {
        while (strchr("<>+-*/{}=", code[*index]) != NULL) {
          (*index)++;
        }
      } else {
        (*index)++;
      }
      
      strncpy(buffer, code+start, *index-start);
      buffer[*index-start] = '\0';
      return createToken(TOK_OPERATOR, buffer);
    }

    if (isalpha(code[*index]) || code[*index] == '_'){
      start = *index;
      while (isalnum(code[*index]) || code[*index] == '_') {
        (*index)++;
      }
      strncpy(buffer, code + start, *index - start);
      buffer[*index - start] = '\0';
      if (code[*index] == '(') {
        return createToken(TOK_FUNCTION, buffer);
      }
      if (isKeyword(buffer)) {
        return createToken(TOK_KEYWORD, buffer);
      } else {
        return createToken(TOK_LITTERAL, buffer);
      }
    }

    (*index)++;
  }
  return createToken(TOK_EOF, ""); 
}

void freeToken(Token* tok) {
  free(tok->content);
  free(tok);
}

void displayToken(Token* tok) {
  switch (tok->type) {
    case TOK_LITTERAL:
      printf("TOK_LITTERAL : ");
      break;
    case TOK_KEYWORD:
      printf("TOK_KEYWORD : "); 
      break;
    case TOK_OPERATOR:
      printf("TOK_OPERATOR : ");
      break;
    case TOK_FUNCTION:
      printf("TOK_FUNCTION : ");
      break;
    case TOK_STRING:
      printf("TOK_STRING : ");
      break;
    case TOK_NUMBER:
      printf("TOK_NUMBER : ");
      break;
    default:
      break;
  }
  
  printf("%s\n", tok->content);
}

char* readFile(char* name) {
  FILE* file = fopen(name, "r");
  fseek(file, 0, SEEK_END);
  long size = ftell(file);
  rewind(file);

  char* ret = (char*)malloc(size+1);
  fread(ret, 1, size, file);

  ret[size] = '\0';
  fclose(file);
  return ret;
}

char* htmlLexer(char* code) {
    int index = 0;
    Token* tok;
    char* ret = (char*)malloc(sizeof(char));
    *ret = '\0'; // Initialisation de ret avec une chaîne vide
    char buff[100]; // Déclaration d'un buffer de taille fixe pour sprintf
    char* type;
    char* space = "";

    while ((tok = getNextToken(code, &index))->type != TOK_EOF) {
        switch (tok->type) {
            case TOK_LITTERAL:
                type = "litteral";
                space = " ";
                break;
            case TOK_KEYWORD:
                type = "keyword";
                space = " ";
                break;
            case TOK_OPERATOR:
                type = "operator";
                space = "<br>"; 
                break;
            case TOK_FUNCTION:
                type = "function";
                space = " ";
                break;
            case TOK_STRING:
                type = "string";
                space = " ";
                break;
            case TOK_NUMBER:
                type = "number";
                space = " ";
                break;
            default:
                break;
        }
        sprintf(buff, "<span class='%s'>%s%s</span>", type, tok->content, space);
        ret = (char*)realloc(ret, strlen(ret) + strlen(buff) + 1);
        strcat(ret, buff);
        free(tok->content);
        free(tok);
    }

    return ret;
}

int main(int argc, char* argv[]) {

  printf("%s\n" ,htmlLexer(argv[1]));
  return 0;

  int index = 0;
  Token* tok;
  char* code = readFile("example.c");
  while((tok=getNextToken(code, &index))->type != TOK_EOF) {
    displayToken(tok);
    freeToken(tok);
  }
  return 0;
}
