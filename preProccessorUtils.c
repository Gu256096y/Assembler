#include "preProccessor.h"

/*checks if this is a call to already exist macro*/
int is_exist_macro(FILE *fam, macro *m, int *igp, char *token, int *mcp, int *lcp)
{
	/*mflag for if macro exist, i for loop*/
	int mflag = 0, i = 0;
	while(i < *mcp)
	{
		/*if exists such name of macro*/
		if(!strcmp(token, m[i].mname))
		{
			/*we have macro on, lets check if there are unnecessary arguments*/
			mflag++;
			token = strtok(NULL, SPACE_TOK);
			/*if we do, we are making the relevant arrangments*/
			if(token != NULL)
			{
				macroCallError(lcp);
				error_on_v2(igp, fam);
			}
			/*if we dont, and we didnt have old errors, lets put the macro in the am file*/
			else
			{
				if(*igp)
				{
					int j = 0;
					while(strcmp(m[i].mdetails[j], EMPTY))
					{			
						fputs((m[i].mdetails[j]),fam);	
						if(strlen(m[i].mdetails[j]) > 1)
						{
							fputs(NEW_LINE, fam);
						}
						j++;						
					}
				}
				break;
			}
		}
		i++;
	}
	return mflag;
}

/*creates a new macro for the macro list*/
macro new_macro(FILE *fp, int *mcp, int *igp, char *token, FILE *fam, int *lcp, char *c_line, macro *m)
{
	/*mt to return the macro*/
	macro mt;
	/*table of invalid names of macros*/
	char not_valid[TAKEN][MAX_NAME] = { "mov", "cmp", "add", "sub", "not", "clr", "lea", "inc", "dec", "jmp", "bne", "get",
	 "prn", "jsr", "rts", "hlt", ".data", ".string", ".struct", ".entry", ".extern", "r0", "r1", "r2", "r3", "r4", "r5", "r6",
	 "r7", "PSW" };
	/*mn to keep the name, problem if there is a problem in name, k for loops*/
	char *mn;
	int problem = 0, k = 0;
	int *pp = &problem;
	/*we have new macro*/
	(*mcp)++;
	token = strtok(NULL, SPACE_TOK);
	/*missing name*/
	if(token == NULL)
	{
		missingName(lcp);
		error_on(igp, pp, fam);
	}
	/*no missing name*/
	else
	{
		/*is name taken by operation or another macro*/
		for(k = 0; k < TAKEN; k++)
		{
			if(!strcmp(token, not_valid[k]))
			{
				takenNameV2(lcp, not_valid[k]);
				error_on(igp, pp, fam);
			}
		}	
		for(k = 0; k < *mcp - 1; k++)
		{
			if(!strcmp(token, m[k].mname))
			{
				takenName(lcp, m[k].mname);
				error_on(igp, pp, fam);
			}
		}	
		mn = (char*)malloc(strlen(token)*sizeof(char));
		if(mn == NULL)
		{
			memoryAllocateError();
		}
		strcpy(mn, token);
		token = strtok(NULL, SPACE_TOK);
		/*too many argumens*/
		if(token != NULL)
		{
			unneccessaryArgError(lcp);
			error_on(igp, pp, fam);
			while(token != NULL)
			{
				token = strtok(NULL, SPACE_TOK);
			}
		}
	}
	/*new macro*/
	/*if we had name problem will name it randomaly*/
	if(problem)
	{
		mt.mname = (char*)malloc(sizeof(char)*(*mcp));
		if(mt.mname == NULL)
		{
			memoryAllocateError();
		}
		for(k = 0; k < *mcp; k++)
		{
			mt.mname[k] = SPACE;
		}
		mt.mname[k + 1] = END_S;
	}
	/*lets make this new macro*/
	else
	{
		mt.mname = (char*)malloc(sizeof(char)*strlen(mn));
		if(mt.mname == NULL)
		{
			memoryAllocateError();
		}
		strcpy(mt.mname, mn);	
	}
	free(mn);	
	for(k = 0; k < MAX_CODE; k++)
	{
		strcpy(mt.mdetails[k], EMPTY);
	}
	/*making the macro lines*/
	for(k = 0; k < MAX_CODE; k++)
	{
		char c_line_k[MAX_LINE];
		(*lcp)++;
		fgets(c_line, MAX_LINE, fp);
		if(strlen(c_line) > 1)
		{
			c_line[strlen(c_line) - 1] = END_S;
		}
		strcpy(c_line_k, c_line);
		token = strtok(c_line, SPACE_TOK);
		if(!strcmp(c_line, END_MACRO))
		{
			token = strtok(NULL, SPACE_TOK);
			/*unnecessary argumens*/
			if(token != NULL)
			{
				endMacroError(lcp);
				error_on_v2(igp, fam);
				while(token != NULL)
				{
					token = strtok(NULL, SPACE_TOK);
				}
			}
			break;
		}
		strcpy(mt.mdetails[k], c_line_k);
	}
	return mt;
}

/*this function deletes uneccessary tabs and spaces and keeps the information in
array of strings*/
void delete_ts(FILE *fp, char *file_name)
{
	/*tab or space flag, i for the for loop, j to make order in the new line, c to count lines*/
	short flag = 0, i = 0, j = 0;
	int c = 0;
	/*cline gets the current line*/
	char cline[MAX_LINE];
	/*deleting tabs and spaces and keeping the information*/	
	while(fgets(cline, MAX_LINE, fp) != NULL)
	{	
		c++;	
		for(i = 0; cline[i] != END_S; i++)
		{
			if(!i && (cline[i] == SPACE || cline[i] == TAB))
			{
				for(j = 0; cline[j] != END_S; j++)
				{
					cline[j] = cline[j + 1];
				}
				i--;
			}
			else if(!flag && (cline[i] == SPACE || cline[i] == TAB))		
			{
				cline[i] = SPACE;
				flag++;
			}
			else if(flag && (cline[i] == SPACE || cline[i] == TAB))		
			{
				for(j = i; cline[j] != END_S; j++)
				{
					cline[j] = cline[j + 1];
				}
				i--;
			}
			else if(flag && (cline[i] != SPACE && cline[i] != TAB)) 
			{
				flag--;
			}
		}
		/*we dont want the new line*/
		cline[strlen(cline) - 1]  = END_S;
		rewind(fp);
		/*setting up the edited file*/	
		add_line(fp, c, cline, file_name);
		delete_line(fp, c + 1, file_name);
		for(i = 0; i < c; i++)
		{
			fgets(cline, MAX_LINE, fp);
		}
	}
	/*order*/
	rewind(fp);							
}

/*this function adds a specified line in a file*/
void add_line(FILE *fp, int line_number, char *new_line, char *file_name)
{
	/*ft temporary file*/
	FILE *ft;
	/*i for the while loop*/
	int i = 0;
	/*curr_line gets the current line*/
	char curr_line[MAX_LINE];
	/*opening a new file*/
	ft = fopen(TEMP_NAME_TS, "w");
	/*is succeeded?*/
	if(ft == NULL)
	{
		fileOpeningError();
	}	
	/*making the new file*/
	while(fgets(curr_line, MAX_LINE, fp) != NULL)
	{
		i++;
		if(i != line_number)
		{
			fputs(curr_line, ft);
		}
		/*add the new line*/
		else
		{
			fputs(new_line, ft);
			fputs("\n", ft);
			fputs(curr_line, ft);
		}
	}
	/*deleting the old file and rename the new file*/
	fclose(ft);
	fclose(fp);
	remove(file_name);
	rename(TEMP_NAME_TS, file_name);
	/*now fp points to the new file*/
	fp = fopen(file_name, "r");
	if(fp == NULL)
	{
		fileOpeningError();
	}
}

/*this function deletes a specified line in a file*/
void delete_line(FILE *fp, int line_number, char *file_name)
{
	/*ft temporary file*/
	FILE *ft;
	/*i for the while loop*/
	int i = 0;
	/*curr_line gets the current line*/
	char curr_line[MAX_LINE];
	/*opening a new file*/
	ft = fopen(TEMP_NAME_TS, "w");
	/*is succeeded?*/
	if(ft == NULL)
	{
		fileOpeningError();
	}
	/*making the new file*/
	while(fgets(curr_line, MAX_LINE, fp) != NULL)
	{
		
		i++;
		if(i != line_number)
		{
			fputs(curr_line, ft);
		}
	}
	/*deleting the old file and rename the new file*/
	fclose(ft);
	fclose(fp);
	remove(file_name);
	rename(TEMP_NAME_TS, file_name);
	/*now fp points to the new file*/
	fp = fopen(file_name, "r");
	if(fp == NULL)
	{
		fileOpeningError();
	}
}
		
