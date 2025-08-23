#include "examination.h"

/*this function makes the second run of the assembler on a given code.*/
void second_run(FILE *fp, short code[], int *igp, int *icp, int *lcp, labels *l)
{
	/*r for registers*/
	reg r[] = { {"r0", 0}, {"r1", 1}, {"r2", 2}, {"r3", 3}, {"r4", 4}, {"r5", 5}, {"r6", 6}, {"r7", 7} };
	/*token for strtok, c_line to get the next lines, c_line_keeper to keep c_line*/
	char *token;
	char c_line[MAX_LINE], c_line_keeper[MAX_LINE];
	/*line to count lines*/
	int line = 0;
	int *linep = &line;
	/*checking the code*/
	while(fgets(c_line, MAX_LINE, fp) != NULL)
	{
		/*i for loops,is_reg if first operand is register, operand for what operand, token to for access to struct*/
		int i, is_reg = 0, operand;
		char *token2;
		line++;
		/*we are ignoring notes*/
		if(c_line[0] == NOTE)
		{
			continue;
		}
		/*we are ignoring white lines*/
		if(strlen(c_line) == 1 || (strlen(c_line) == 2 && c_line[0] == SPACE))
		{
			continue;
		}
		/*no need new line*/
		c_line[strlen(c_line) - 1] = END_S;	
		strcpy(c_line_keeper, c_line);
		token = strtok(c_line, SPACE_TOK);
		/*checking if first parameter is a label definition, if it is, we will skip it*/
		for(i = 0; i < (*lcp); i++)
		{
			/*t to keep token*/
			char *t = (char*)malloc(sizeof(char)*strlen(token));
			if(t == NULL)
			{
				memoryAllocateError();
			}
			strcpy(t, token);
			t[strlen(t) - 1] = END_S;
			/*if equal skipping*/
			if(!strcmp(t, l[i].lname))
			{
				token = strtok(NULL, SPACE_TOK);
				break;
			}	
		}
		/*if data, string, struct or extern we continue to next line*/
		if(!strcmp(token, EXT) || !strcmp(token, DATA) || !strcmp(token, STRING) || !strcmp(token, STRUCT))
		{
			while(token != NULL)
			{
				token = strtok(NULL, SPACE_TOK);
			}
			/*read next line*/
			continue;
		}
		/*checking if entry. if it is, we will check if the labels exist, and if not taken by extern*/
		if(!strcmp(token, ENT))
		{
			token = strtok(NULL, COMMA_TOK);
			first_check_extern_or_entry(c_line_keeper, token, igp, linep);
			while(token != NULL)
			{
				char *te = (char*)malloc(sizeof(char)*strlen(token));
				if(te == NULL)
				{
					memoryAllocateError();
				}
				strcpy(te, token);
				if(te[0] == SPACE)
				{
					te = te + 1;
				}
				if(te[strlen(te) - 1] == SPACE)
				{
					te[strlen(te) - 1] = END_S;
				}
				/*checking if such label exist*/
				i = 0;
				while(i < (*lcp))
				{
					if(!strcmp(te, l[i].lname))
					{
						/*if its already defined as entry*/
						if(l[i].extern_or_entry == EXT_TYPE)
						{
							syntaxError("Error: A label defined as extern can not be defined as entry.", linep);;
							err_on(igp);
							break;
						}
						else
						{
							l[i].extern_or_entry = ENT_TYPE;
							break;
						}
					}
					else
					{
						i++;
					}
				}
				/*if not found label*/
				if(i == (*lcp))
				{
					syntaxError("Error: Label defined as entry has never been used in the code.", linep);;
					err_on(igp);
				}
				token = strtok(NULL, COMMA_TOK);
			}
			while(token != NULL)
			{
				token = strtok(NULL, SPACE_TOK);
			}
			/*we can read next line*/		
			continue;
		}
		/*setting up the code table*/
		token = strtok(NULL, COMMA_TOK);
		operand = 1;
		while(token != NULL)
		{
			/*temp if token have space in end or beggining*/
			char *temp = (char*)malloc(sizeof(char)*strlen(token));
			if(temp == NULL)
			{
				memoryAllocateError();
			}
			strcpy(temp, token);
			if(temp[0] == SPACE)
			{
				temp = temp + 1;
			}
			if(temp[strlen(temp) - 1] == SPACE)
			{
				temp[strlen(temp) - 1] = END_S;
			}
			token = strtok(NULL, COMMA_TOK);
			/*if register*/
			i = 0;
			while(i < REG_NUM)
			{
				if(!strcmp(temp, r[i].regname))
				{
					if((*igp))
					{
						if(!is_reg)
						{
							(*icp)++;
							is_reg = 1;
						}
						if(operand == 1)
						{
							code[*icp] = r[i].num << REG_SO;
						}
						if(operand == 2)
						{
							code[*icp] = code[*icp] | r[i].num << REG_DE;
						}
					}
					break;
				}
				i++;
			}
			/*if register lets continue*/
			if(i < REG_NUM)
			{	
				operand++;
				continue;
			}	
			/*if immediate*/
			if(temp[0] == NUM_START)
			{
				if(temp[1] == MINUS)
				{
					if((*igp))
					{
						(*icp)++;
						code[(*icp)] = (-1)*(atoi(temp + 2)) << DE_PLACE;
					}
					operand++;
					continue;
				}
				else if(temp[1] == PLUS)
				{
					if((*igp))
					{
						(*icp)++;
						code[(*icp)] = (atoi(temp + 2)) << DE_PLACE;
					}
					operand++;
					continue;
				}
				else
				{
					if((*igp))
					{
						(*icp)++;
						code[(*icp)] = (atoi(temp + 1)) << DE_PLACE;
					}
					operand++;
					continue;
				}
			}
			/*if direct*/
			i = 0;
			while(i < (*lcp))
			{
				if(!strcmp(temp, l[i].lname))
				{
					if((*igp))
					{
						(*icp)++;
						/*if extern*/
						if(l[i].extern_or_entry == EXT_TYPE)
						{
							if(l[i].appear_num == 0)
							{
								l[i].appear = (short*)malloc(0);
								if(l[i].appear == NULL)
								{
									memoryAllocateError();
								}
							}
							l[i].appear_num++;
							l[i].appear = (short*)realloc(l[i].appear, sizeof(short)*l[i].appear_num);
							if(l[i].appear == NULL)
							{
								memoryAllocateError();
							}
							l[i].appear[l[i].appear_num - 1] = (*icp);
							code[(*icp)] = E;
						}
						/*if entry or regular*/
						else
						{
							code[(*icp)] = l[i].address << DE_PLACE;
							code[(*icp)] = code[(*icp)] | R;
						}
					}
					break;
				}
				i++;
			}
			/*if direct lets continue*/
			if(i < (*lcp))
			{
				operand++;
				continue;
			}
			/*if access to struct*/
			token2 = strtok(temp, DOT);
			i = 0;
			while(i < (*lcp))
			{
				if(!strcmp(token2, l[i].lname))
				{
					if((*igp))
					{
						(*icp)++;
						/*if extern*/
						if(l[i].extern_or_entry == EXT_TYPE)
						{
							code[(*icp)] = E;
						}
						/*if entry or regular*/
						else
						{
							code[(*icp)] = l[i].address << DE_PLACE;
							code[(*icp)] = code[(*icp)] | R;
						}
					}
					break;
				}
				i++;
			}
			/*if not access to struct, no such operand!*/
			if(i == (*lcp))
			{
				syntaxError("Error: no such kind of operand.", linep);
				err_on(igp);
				while(token2 != NULL)
				{
					token2 = strtok(NULL, DOT);
				}
				token = strtok(NULL, COMMA_TOK);
				continue;
			}
			/*checking if num part or string part of struct*/
			token2 = strtok(NULL, DOT);
			if(!strcmp(token2, NUM_PART))
			{
				if((*igp))
				{
					(*icp)++;
					code[(*icp)] = 1 << DE_PLACE;
				}
			}
			if(!strcmp(token2, STR_PART))
			{
				if((*igp))
				{
					(*icp)++;
					code[(*icp)] = 2 << DE_PLACE;
				}
			}
			while(token2 != NULL)
			{
				token2 = strtok(NULL, DOT);
			}
			operand++;
		}
		while(token != NULL)
		{
			token = strtok(NULL, COMMA_TOK);
		}	
		(*icp)++;
	}
}
