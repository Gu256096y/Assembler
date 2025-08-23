#include "output.h"

/*gets all the information needed to make the object file and making it!*/
void obj_maker(short code[], short data[], int code_size, int data_size, char *file_name)
{
	/*fob for the obj file, i is for the for loop, token for strtok, and so as well dot. data_str and code_str for base32*/
	int i;
	FILE *fob;
	char *token, *code_str, *data_str;
	token = strtok(file_name, DOT);
	/*creating the file*/
	fob = fopen(strcat(token,OB), "w");
	if(fob == NULL)
	{
		memoryAllocateError();
	}
	/*prints all the information needed in the file*/
	rewind(fob);
	code_str = base32_maker(code_size);
	data_str = base32_maker(data_size);
	if(strlen(code_str) > TWO_SIGNES)
	{
		code_str[2] = END_S;
	}
	if(strlen(data_str) > TWO_SIGNES)
	{
		data_str[2] = END_S;
	}
	fprintf(fob, "%s %s\n", code_str, data_str);
	for(i = FIRST_CODE_CELL; i < code_size + FIRST_CODE_CELL; i++)
	{
		char *add = base32_maker(i);
		if(strlen(add) > TWO_SIGNES)
		{
			add[2] = END_S;
		}
		code_str = base32_maker(code[i]);
		if(strlen(code_str) > TWO_SIGNES)
		{
			code_str[2] = END_S;
		}
		fprintf(fob, "%s %s\n", add, code_str);
	}
	for(i = FIRST_CODE_CELL + code_size; i < data_size + code_size + FIRST_CODE_CELL; i++)
	{
		char *add = base32_maker(i);
		data_str = base32_maker(data[i]);
		if(strlen(add) > TWO_SIGNES)
		{
			add[2] = END_S;
		}
		if(strlen(data_str) > TWO_SIGNES)
		{
			data_str[2] = END_S;
		}
		fprintf(fob, "%s %s\n", add, data_str);
	}
	/*kind of free for token for just in case*/
	while(token != NULL)
	{
		token = strtok(NULL, DOT);
	}
	fclose(fob);
}



