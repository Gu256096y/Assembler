#include "preProccessor.h"

/*this function gets the original asm code and replace it with the
after macro file. it returns if there were errors or not.
also cleaning notes.
for my comfurt, i also used this function to delete unnecessary tabs and spaces*/

int am_maker(FILE *fp, char *file_name)
{
	/*m table of macros*/
	macro *m;
	/*fam for am file*/
	FILE *fam = fopen(TEMP_NAME, "w");
	/*token for str tok, nfn new file name*/
	char *token;
	/*c_line to get a line, ofn old file name, c_line_keeper keeps the original c_line
	which is being changed because of token*/
	char c_line[MAX_LINE], c_line_keeper[MAX_LINE];
	/*mcount counting macros, linec counting the line in the original file, isgood if there is an 
	error or not*/
	int mcount = 0, linec = 0, isgood = 1;
	int *mcp = &mcount, *lcp = &linec, *igp = &isgood;
	m =(macro*)malloc(mcount);
	if(m == NULL)
	{
		memoryAllocateError();
	}
	if(fam == NULL)
	{
		fileOpeningError();
	}
	/*making the file comfortable to work with*/
	delete_ts(fp, file_name);
	/*checking every line and line until EOF*/
	while(fgets(c_line, MAX_LINE, fp) != NULL)
	{
		/*removing \n*/
		if(strlen(c_line) > 1)
		{
			c_line[strlen(c_line) - 1] = END_S;
		}
		/*keeping original c_line*/
		strcpy(c_line_keeper, c_line);	
		/*add 1 to count of lines*/
		linec++;
		/*checking if name of existing macro*/
		token = strtok(c_line, SPACE_TOK);
		/*if exists macro is on we can read a new line*/
		if(is_exist_macro(fam, m, igp, token, mcp, lcp))
		{
			/*kind of free*/
			while(token != NULL)
			{
				token = strtok(NULL, SPACE_TOK);
			}
			continue;
		}
		/*if there is a macro definition*/
		if(!strcmp(token, MACRO))
		{
			m = (macro*)realloc(m,sizeof(macro)*(mcount+1));
			if(m == NULL)
			{
				memoryAllocateError();
			}
			m[mcount] = new_macro(fp, mcp, igp, token, fam, lcp, c_line, m);
			continue;
		}
		/*copying the line*/
		if(isgood)
		{
			fputs(c_line_keeper, fam);
			fputs(NEW_LINE, fam);
		}
		while(token != NULL)
		{
			token = strtok(NULL, SPACE_TOK);
		}
	}
	/*saving the after macro file*/
	if(isgood)
	{
		fclose(fp);
		fclose(fam);
		remove(file_name);
		fp = fopen(TEMP_NAME, "r");
		if(fp == NULL)
		{
			fileOpeningError();
		}
		token = strtok(file_name, DOT);
		rename(TEMP_NAME, strcat(token, AM));
		while(token != NULL)
		{
			token = strtok(NULL, DOT);
		}
	}
	free(m);
	return isgood;
}




