#include "examination.h"

/*this function makes the first run of the assembler on a given code.*/
labels* first_run(FILE *fp, short code[], short data[], int *igp, int *dcp, int *icp, int *lcp)
{
	/*d to make more affective work with data functions*/
	data_types d[] = { {DATA, check_data}, {STRING, check_string}, {STRUCT, check_struct} };
	/*c to make more comfortable to work with code types
	in here defines would make things less understood to me*/
	codes c[] = { {"mov", 0, 7}, {"cmp", 1, 8}, {"add", 2, 7}, {"sub", 3, 7}, {"not", 4, 3}, {"clr", 5, 3},
			{"lea", 6, 5}, {"inc", 7, 3}, {"dec", 8, 3}, {"jmp", 9, 3}, {"bne", 10, 3}, {"get", 11, 3},
			{"prn", 12, 4}, {"jsr", 13, 3}, {"rts", 14, 0}, {"hlt", 15, 0} };
	/*l for making the labels*/
	labels *l;
	/*c_line to get the next line input and its keeper*/
	char c_line[MAX_LINE], c_line_keeper[MAX_LINE];
	/*token for strtoks*/
	char *token;
	/*isgood if we had errors or not line to count lines, j for loop*/
	int line = 0, j;
	int *linep = &line;
	l = (labels*)malloc(sizeof(labels)*(*lcp));
	if(l == NULL)
	{
		memoryAllocateError();
	}
	/*checking the code*/
	while(fgets(c_line, MAX_LINE, fp) != NULL)
	{
		/*label_flag to see if we have label, i for loop*/
		int label_flag = 0, i = 0;
		/*keeps label name*/
		char *label_name;
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
		/*if we have new label time to raise the flag*/
		if(is_new_label(token, l, igp, lcp, linep))
		{
			label_flag++;
			label_name = (char*)malloc(sizeof(char)*strlen(token));
			if(label_name == NULL)
			{
				memoryAllocateError();
			}
			strcpy(label_name, token);
		}
		/*lets continue to check this line*/
		if(label_flag)
		{
			token = strtok(NULL, SPACE_TOK);
		}
		/*check if data, and if it does, we are updating the label type and add the information to data list*/
		if(is_data(token))
		{
			/*i for loop, dc keep to keep original dc*/
			int i, dc_keep = *dcp;
			for(i = 0; i < DATA_TYPES; i++)
			{
				if(!strcmp(token, d[i].dname))
				{
					(*(d[i].func))(c_line_keeper, token, linep, igp, dcp, data);
				}
			}
			/*making the label if needed*/
			if(label_flag)
			{
				(*lcp)++;
				l = (labels*)realloc(l, sizeof(labels)*(*lcp));
				if(l == NULL)
				{
					memoryAllocateError();
				}
				l[(*lcp) - 1] = make_new_label(label_name);	
				l[(*lcp) - 1].code_or_data = DATA_TYPE;
				l[(*lcp) - 1].address = dc_keep;
				free(label_name);
			}
			/*kind of free*/
			while(token != NULL)
			{
				token = strtok(NULL, SPACE_TOK);
			}
			/*we can check the next line*/
			continue;
		}
		/*entry we check in the second examintaion of code*/
		if(!strcmp(token, ENT))
		{
			if(label_flag)
			{
				syntaxError("Warning only: Defining a label before .entry instruction means nothing.", linep);
				(*lcp)++;
				l = (labels*)realloc(l, sizeof(labels)*(*lcp));
				if(l == NULL)
				{
					memoryAllocateError();
				}
				l[(*lcp) - 1] = make_new_label(label_name);	
				l[(*lcp) - 1].address = -1;
				free(label_name);	
			}
			while(token != NULL)
			{
				token = strtok(NULL, SPACE_TOK);
			}
			continue;
		}
		/*checking extern*/
		if(!strcmp(token, EXT))
		{
			/*checking problems*/
			token = strtok(NULL, COMMA_TOK);
			first_check_extern_or_entry(c_line_keeper, token, igp, linep);
			while(token != NULL)
			{
				/*temp instead of token*/
				char *temp;
				temp = (char*)malloc(sizeof(char)*(strlen(token) + 1));
				if(temp == NULL)
				{
					memoryAllocateError();
				}
				strcpy(temp, token);
				/*if current ext is a good label we are adding it to the list with ext sign*/
				if(is_new_label(strcat(temp, END_L_S), l, igp, lcp, linep))
				{
					(*lcp)++;;
					l = (labels*)realloc(l, sizeof(labels)*(*lcp));
					if(l == NULL)
					{
						memoryAllocateError();
					}
					/*making the extern label*/
					l[(*lcp) - 1] = make_new_label(temp);	
					free(temp);
					l[(*lcp) - 1].code_or_data = DATA_TYPE;
					l[(*lcp) - 1].address = 0;
					l[(*lcp) - 1].extern_or_entry = EXT_TYPE;
					l[(*lcp) - 1].appear_num = 0;
					token = strtok(NULL, COMMA_TOK);
				}
				/*if not a good label we can stop*/
				else
				{
					break;
				}
			}
			/*unnesseccary label defining?*/
			if(label_flag)
			{
				syntaxError("Warning only: Defining a label before .extern instruction means nothing.", linep);
				(*lcp)++;
				l = (labels*)realloc(l, sizeof(labels)*(*lcp));
				if(l == NULL)
				{
					memoryAllocateError();
				}
				l[(*lcp) - 1] = make_new_label(label_name);	
				l[(*lcp) - 1].address = -1;
				free(label_name);
			}
			while(token != NULL)
			{
				token = strtok(NULL, SPACE_TOK);
			}
			/*lets get the next line*/
			continue;
		}
		/*if label adding it as code type*/
		if(label_flag)
		{
			(*lcp)++;
			l = (labels*)realloc(l, sizeof(labels)*(*lcp));
			if(l == NULL)
			{
				memoryAllocateError();
			}
			l[(*lcp) - 1] = make_new_label(label_name);	
			l[(*lcp) - 1].code_or_data = CODE_TYPE;
			l[(*lcp) - 1].address = *icp;
			free(label_name);
		}
		/*check if code*/
		while(i < CODE_NUM)
		{
			if(!strcmp(c[i].cname, token))
			{
				break;
			}
			i++;
		}
		/*no such command situation*/
		if(i == CODE_NUM)
		{
			syntaxError("Error: No such command or instruction.", linep);	
			err_on(igp);
			continue;
		}
		/*put operation number in tabel*/
		code[*icp] = c[i].cnum << OP_PLACE;
		/*calling the function to take care of the rest of the line*/
		/*no source no dest*/
		if(c[i].cotype == 0)
		{
			rts_hlt(token, linep, igp, icp);
		}
		/*no source, 0,1,2,3 dest*/
		else if(c[i].cotype == 4)
		{
			/*prn*/
			single_operanded(token, linep, igp, icp, code, 1, 0, 0, 0);
		}
		/*no source, 1,2,3 dest*/
		else if(c[i].cotype == 3)
		{
			/*single operanded no prn*/
			single_operanded(token, linep, igp, icp, code, 0, 0, 0, 0);
		}
		/*source 1,2, dest 1,2,3*/
		else if(c[i].cotype == 5)
		{
			two_operaneded_first(c_line_keeper, token, linep, igp, icp, code, 1, 0);
		}
		/*source 0,1,2,3, dest 1,2,3*/
		else if(c[i].cotype == 7)
		{
			two_operaneded_first(c_line_keeper, token, linep, igp, icp, code, 0, 0);
		}
		/*source 0,1,2,3, dest 0,1,2,3*/
		else if(c[i].cotype == 8)
		{
			two_operaneded_first(c_line_keeper, token, linep, igp, icp, code, 0, 1);
		}
		while(token != NULL)
		{
			token = strtok(NULL, SPACE_TOK);
		}
	}
	/*updating the data table!*/
	for(j = 0; j < (*dcp); j++)
	{
		int temp = data[j];
		data[j] = 0;
		data[j + (*icp)] = temp;
	}
	/*updating data labels*/
	for(j = 0; j < (*lcp); j++)
	{
		if(l[j].code_or_data == DATA_TYPE && !(l[j].extern_or_entry == EXT_TYPE))
		{
			l[j].address = l[j].address + (*icp);
		}
	}	
	return l;
}
		
