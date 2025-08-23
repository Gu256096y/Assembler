#include "examination.h"
#include "output.h"
#include "preProccessor.h"
#include "systemErrors.h"

/*the main function of the assembler*/
int main(int argc, char *argv[])
{
	/*assembler runs over files*/
	while(--argc)
	{
		/*code contains operation words. data contatins instruction words*/
		short code[MAX_CODE] = { 0 }, data[MAX_CODE] = { 0 };
		/*file_name to keep the original file name*/
		char *file_name = (char*)malloc(sizeof(char)*(strlen(argv[argc]) + 3));
		/*isgood will get 0 value if error appears, ic to count code words
		dc to count data words, label counter to count labels.
		is extern and is entry will get 1 value if there were extern or entry instructions*/
		int isgood = 1, ic = FIRST_CODE_CELL, dc = 0, label_counter = 0, is_extern = 0, is_entry = 0, i;
		int *igp = &isgood, *icp = &ic, *dcp = &dc, *lcp = &label_counter;
		/*l to keep the list of labels*/
		labels *l;
		FILE *fp;
		strcpy(file_name, argv[argc]);
		strcat(file_name, AS);
		fp = fopen(file_name, "r");
		if(fp == NULL)
		{
			fileOpeningError();
		}
		if(file_name == NULL)
		{
			memoryAllocateError();
		}
		/*pre proccessor's time to work.*/
		isgood = am_maker(fp, file_name);
		if(!isgood)
		{
			fprintf(stderr, "File %s: Pre proccesser failed. Details above.\n", argv[argc]);
			continue;
		}
		rewind(fp);
		/*first run of the assembler if everything is fine*/
		l = first_run(fp, code, data, igp, dcp, icp, lcp);
		/*if we have filed, why have we failed?*/
		if(!isgood)
		{
			fprintf(stderr, "File %s: First Examination of code failed. Details above.\n", argv[argc]);
			continue;
		}
		rewind(fp);
		ic = FIRST_CODE_CELL;
		/*second run of the assembler if everything is fine*/
		second_run(fp, code, igp, icp, lcp, l);
		/*if we have failed, why?*/
		if(!isgood)
		{
			fprintf(stderr, "File %s: Second examination of code failed. Details above.\nThe assembler will check the next file.\n", argv[argc]);
			continue;
		}
		ic = ic - FIRST_CODE_CELL;
		/*if we are good lets make the obj file*/
		obj_maker(code, data, ic, dc, file_name);
		/*checking if ext and ent files are needed*/
		for(i = 0; i < label_counter; i++)
		{
			if(l[i].extern_or_entry == EXT_TYPE)
			{
				is_extern = 1;
			}
			if(l[i].extern_or_entry == ENT_TYPE)
			{
				is_entry = 1;
			}
		}
		/*if we need ext file lets make it*/
		if(is_extern)
		{
			FILE *fob;
			char *token;
			token = strtok(file_name, DOT);
			fob = fopen(strcat(token,EXT_F), "w");
			if(fob == NULL)
			{
				memoryAllocateError();
			}
			for(i = 0; i < (*lcp); i++)
			{
				if(l[i].extern_or_entry == EXT_TYPE)
				{
					int j;
					for(j = 0; j < l[i].appear_num; j++)
					{
						char *app = base32_maker(l[i].appear[j]);
						if(strlen(app) > TWO_SIGNES)
						{
							app[2] = END_S;
						}
						fprintf(fob, "%s %s\n", l[i].lname, app);
					}
				}
			}
			while(token != NULL)
			{
				token = strtok(NULL, DOT);
			}
			fclose(fob);
		}
		/*if we need ent file lets make it*/
		if(is_entry)
		{
				FILE *fob;
				char *token;
				token = strtok(file_name, DOT);
				fob = fopen(strcat(token,ENT_F), "w");
				if(fob == NULL)
				{
					memoryAllocateError();
				}
				for(i = 0; i < (*lcp); i++)
				{
					if(l[i].extern_or_entry == ENT_TYPE)
					{
						char *add = base32_maker(l[i].address);
						if(strlen(add) > TWO_SIGNES)
						{
							add[2] = END_S;
						}
						fprintf(fob, "%s %s\n", l[i].lname, add);
					}
				}
				while(token != NULL)
				{
					token = strtok(NULL, DOT);
				}
				fclose(fob);
		}
		/*continue to next file*/
	}
	return 0;
}


