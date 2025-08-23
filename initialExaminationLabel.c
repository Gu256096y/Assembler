#include "examination.h"

/*checks if there is label definition and returns if yes or not. also changing igp if problem*/
int is_new_label(char *token, labels *l, int *igp, int *lcp, int *linep)
{
	/*check if it is a label or not, i for loop*/
	int check = 0, i;
	/*table of invalid names of labels*/
	char not_valid[TAKEN][MAX_NAME] = { "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "get",
	 "prn", "jsr", "rts", "hlt", ".data", ".string", ".struct", ".entry", ".extern", "r0", "r1", "r2", "r3", "r4", "r5", "r6",
	 "r7", "PSW" };
	/*checks the first param to see if it is a label or not, : end*/
	if(token[strlen(token) - 1] == END_L)
	{
		check++;
	}
	else
	{
		return check;
	}
	/*checking if it is a good label or not. maybe name is in wrong syntax, maybe it is taken... who knows?*/
	if(!is_legal_label_name(token, linep, igp, 0))
	{
		check = 0;
	}
	for(i = 0; i < TAKEN; i++)
	{
		if(!strcmp(token, strcat(not_valid[i], END_L_S)))
		{
			syntaxError("Error: Label's name is taken by register, operation or instruction.", linep);
			err_on(igp);
			check = 0;
			return check;
		}
		/*because of strcat*/
		not_valid[i][strlen(not_valid[i]) - 1] = END_S;
	}
	for(i = 0; i < *lcp; i++)
	{
		if(!strcmp(token, strcat(l[i].lname, END_L_S)))
		{
			syntaxError("Error: Label's name is taken by another label.", linep);
			err_on(igp);
			check = 0;
		}
		/*because pf strcat*/
		l[i].lname[strlen(l[i].lname) - 1] = END_S;
	}
	return check;
}

/*makes a new temporary label to add to the list*/
labels make_new_label(char *token)
{
	/*temporary label*/
	labels lt;
	/*name to keep the name of the label*/
	char *name = (char*)malloc(sizeof(char)*strlen(token));
	if(name == NULL)
	{
		memoryAllocateError();
	}
	strcpy(name, token);
	/*no need :*/
	name[strlen(name) - 1] = END_S;
	lt.lname = (char*)malloc(sizeof(char)*strlen(name));
	if(lt.lname == NULL)
	{
		memoryAllocateError();
	}
	strcpy(lt.lname, name);
	if(lt.lname[0] == SPACE)
	{
		lt.lname = lt.lname + 1;
	}
	free(name);
	return lt;
}

/*checks if legal label name*/
int is_legal_label_name(char *token, int *linep, int *igp, int as_operand_command)
{
	/*check if good, i for loop, temp to keep token*/
	int check = 1, i;
	char *temp;
	if(token[0] == SPACE)
	{
		temp = token + 1;
	}
	else
	{
		temp = token;
	}
	if(strlen(temp) >= MAX_LABEL)
	{
		if(!as_operand_command)
		{
			syntaxError("Error: Label's name is too long.", linep);
			err_on(igp);
		}	
		check = 0;
	}
	if(!isalpha(temp[0]))
	{
		if(!as_operand_command)
		{
			syntaxError("Error: Label's name first character is not a letter.", linep);
			err_on(igp);
		}
		check = 0;
	}
	for(i = 1; i < strlen(temp) - 1; i++)
	{
		if(!isalnum(temp[i]))
		{
			if(!as_operand_command)
			{
				syntaxError("Error: Label's name contains non alpha numeric character.", linep);
				err_on(igp);
			}
			check = 0;
		}
	}
	return check;
}
