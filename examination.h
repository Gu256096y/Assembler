#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>
#include "systemErrors.h"


#define REG_NUM 8
#define NUM_PART "1"
#define STR_PART "2"
#define IMMEDIATE 0
#define DIRECT 1
#define ACCESS 2
#define REG 3
#define R 2
#define E 1
#define REG_SO 6
#define REG_DE 2
#define DE_PLACE 2
#define SO_PLACE 4
#define OP_PLACE 6
#define CODE_NUM 16
#define CODE_TYPE 1
#define DATA_TYPE 2
#define MAX_LINE 82
#define SPACE ' '
#define SPACE_TOK " "
#define COMMA_TOK ","
#define NUM_START '#'
#define TWO_SIGNES 2
#define DOT "."
#define TAKEN 30
#define MAX_NAME 30
#define MAX_CODE 356
#define NOTE ';'
#define END_S '\0'
#define END_L ':'
#define END_L_S ":"
#define DATA ".data"
#define STRING ".string"
#define STRUCT ".struct"
#define EXT ".extern"
#define ENT ".entry"
#define MAX_LABELS 256
#define MAX_LABEL 32
#define DATA_TYPES 3
#define BASE_TEN 10
#define MINUS '-'
#define COMMA ','
#define PLUS '+'
#define DOUBLE_QUOTES '\"'
#define EXT_TYPE 1
#define ENT_TYPE 2

struct regs
{
	char *regname;
	short num;
};

struct label 
{
	char *lname;
	/*code 1 data 2*/
	short code_or_data;
	/*ext 1 ent 2*/
	short extern_or_entry;
	short address;
	short *appear;
	short appear_num;
};

struct dataType
{
	char *dname;
	void (*func)(char*, char*, int*, int*, int*, short*);
};

struct codeType
{
	char *cname;
	short cnum;
	short cotype;
};

typedef struct regs reg;
typedef struct label labels;
typedef struct dataType data_types;
typedef struct codeType codes;

labels* first_run(FILE *fp, short code[], short data[], int *igp, int *dcp, int *icp, int *lcp);
int is_new_label(char *token, labels *l, int *igp, int *lcp, int *linep);
labels make_new_label(char *token);
int is_data(char *token);
void syntaxError(char err[], int *linep);
int short_check(char *token, int *linep, int *igp, short *minp, long *valuep);
void check_data(char *c_line_keeper, char *token, int *linep, int *igp, int *dcp, short data[]);
void check_string(char *c_line_keeper, char *token, int *linep, int *igp, int *dcp, short data[]);
void check_struct(char *c_line_keeper, char *token, int *linep, int *igp, int *dcp, short data[]);
void err_on(int *igp);
int data_operand_short_integer_check(short data[], char *token, int *igp, int *linep, int *dcp);
void first_check_extern_or_entry(char *c_line_keeper, char *token, int *igp, int *linep);
void rts_hlt(char *token, int *linep, int *igp, int *icp);
void single_operanded(char *token, int *linep, int *igp, int *icp, short code[], short isprn_cmp, int how_many_to_add, int istwo, int is_reg);
int is_legal_label_name(char *token, int *linep, int *igp, int as_operand_command);
void immediate(char *token, int *linep, int *igp);
void two_operaneded_first(char *c_line_keeper, char *token, int *linep, int *igp, int *icp, short code[], short islea, short iscmp);
void no_such_label(int *linep);
void label_already_extern(int *linep);
void second_run(FILE *fp, short code[], int *igp, int *icp, int *lcp, labels *l);
