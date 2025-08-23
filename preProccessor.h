#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "systemErrors.h"

#define MAX_LINE 82
#define TEMP_NAME "temp.txt"
#define TEMP_NAME_TS "ts.txt"
#define MAX_NAME 30
#define NEW_LINE "\n"
#define TAB '\t'
#define SPACE ' '
#define SPACE_TOK " "
#define EMPTY ""
#define END_S '\0'
#define TWO_SIGNES 2
#define AM ".am"
#define AS ".as"
#define DOT "."
#define TAKEN 30
#define MAX_CODE 356
#define MACRO "macro"
#define END_MACRO "endmacro"

struct macros
{
	char *mname;
	char mdetails[MAX_CODE][MAX_LINE];
};

typedef struct macros macro;

int am_maker(FILE *fp, char *file_name);
void delete_line(FILE *fp, int line_number, char *file_name);
void add_line(FILE *fp, int line_number, char *new_line, char *file_name);
void delete_ts(FILE *fp, char *file_name);
int is_exist_macro(FILE *fam, macro *m, int *igp, char *token, int *mcp, int *lcp);
macro new_macro(FILE *fp, int *mcp, int *igp, char *token, FILE *fam, int *lcp, char *c_line, macro *m);
void error_on(int *igp, int *pp, FILE *fam);
void error_on_v2(int *igp, FILE *fam);
void macroCallError(int *lcp);
void endMacroError(int *lcp);
void unneccessaryArgError(int *lcp);
void takenName(int *lcp, char *name);
void takenNameV2(int *lcp, char *not_valid);
void missingName(int *lcp);
