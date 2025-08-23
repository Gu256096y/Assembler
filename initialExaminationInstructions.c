#include "examination.h"

/*extern first check*/
void first_check_extern_or_entry(char *c_line_keeper, char *token, int *igp, int *linep)
{
	/*j for loop cooma on to check if missing operand*/
	int j, comma_on = 0;
	/*first we will check if there is a missing operand*/
	if(token == NULL)
	{
		syntaxError("Error: .extern and .entry intruction must have at least one operand.", linep);
		err_on(igp);
		return;
	}
	/*missing operand between commas?*/
	for(j = 0; j < strlen(c_line_keeper); j++)
	{
		if(c_line_keeper[j] == SPACE)
		{
			continue;
		}
		if(comma_on && c_line_keeper[j] == COMMA)
		{
			syntaxError("Error: .extern or .entry missing at least one operand.", linep);
			err_on(igp);		
		}
		else if(c_line_keeper[j] == COMMA)
		{
			comma_on = 1;
		}
		else
		{
			comma_on = 0;
		}
	}
	/*might be missing operand at the end of the line*/
	if(c_line_keeper[strlen(c_line_keeper) - 1] == COMMA || (c_line_keeper[strlen(c_line_keeper) - 1] == SPACE &&
		c_line_keeper[strlen(c_line_keeper) - 2] == COMMA))
	{
		syntaxError("Error: .entry or .extern instruction missing at least one operand.", linep);
		err_on(igp);
	}
}

/*struct is an integer and a string. we will just use what we have build for data and string types*/
void check_struct(char *c_line_keeper, char *token, int *linep, int *igp, int *dcp, short data[])
{
	/*checking if we have short operand*/
	token = strtok(NULL, COMMA_TOK);
	if(token == NULL)
	{
		syntaxError("Error: .struct instruction missing it's operands.", linep);
		err_on(igp);
		return;
	}
	/*checking the short operand*/
	data_operand_short_integer_check(data, token, igp, linep, dcp);
	/*checking the string*/
	check_string(c_line_keeper, token, linep, igp, dcp, data);
}

/*check if the string is good, and adding it*/
void check_string(char *c_line_keeper, char *token, int *linep, int *igp, int *dcp, short data[])
{
	/*temp to keep token*/
	char *temp;
	/*checking if there is a string to add*/
	token = strtok(NULL, SPACE_TOK);
	if(token == NULL)
	{
		err_on(igp);
		syntaxError("Error: .struct or .string instruction missing it's string operand.", linep);
		return;
	}
	/*keeping token*/
	temp = token;
	/*checking if the syntax is good. must have double quotes in the end and
	in the beggining, both not part of the string. double quotes may be
	part of the string!!. also taking care of the situation of only one character (which means automatticly
	not enough double quotes, and case of only double quotes (no string)*/
	if(token[0] != DOUBLE_QUOTES || token [strlen(token) - 1] != DOUBLE_QUOTES || strlen(token) <= 2)
	{
		err_on(igp);
		syntaxError("Error: .struct or .string instruction missing double quotes\nat the beggining or at the end of it's string operand.", linep);
	}
	/*checking if there is unnessecary operand*/
	token = strtok(NULL, SPACE_TOK);
	if(token != NULL)
	{
		err_on(igp);
		syntaxError("Error: .struct or .string instruction contains too many operands", linep);
	}
	/*lets add the string to data table*/
	if(*igp)
	{
		/*k for loop*/
		int k;
		temp++;
		temp[strlen(temp) - 1] = END_S;
		for(k = 0; k < strlen(temp); k++)
		{
			data[*dcp] = temp[k];
			(*dcp)++;
		}
		/*last one is zero*/
		(*dcp)++;
	}
}

/*checks if the data addition is good, and adding it*/
void check_data(char *c_line_keeper, char *token, int *linep, int *igp, int *dcp, short data[])
{
	/*j for loop cooma on to check if missing operand*/
	int j, comma_on = 0;
	/*first we will check if there is a missing operand*/
	token = strtok(NULL, COMMA_TOK);
	if(token == NULL)
	{
		syntaxError("Error: .data instruction missing at least one operand.", linep);
		err_on(igp);
		return;
	}
	/*missing operand between commas?*/
	for(j = 0; j < strlen(c_line_keeper); j++)
	{
		if(c_line_keeper[j] == SPACE)
		{
			continue;
		}
		if(comma_on && c_line_keeper[j] == COMMA)
		{
			syntaxError("Error: .data instruction missing at least one operand.", linep);
			err_on(igp);		
		}
		else if(c_line_keeper[j] == COMMA)
		{
			comma_on = 1;
		}
		else
		{
			comma_on = 0;
		}
	}
	/*might be missing operand at the end of the line*/
	if(c_line_keeper[strlen(c_line_keeper) - 1] == COMMA || (c_line_keeper[strlen(c_line_keeper) - 1] == SPACE &&
		c_line_keeper[strlen(c_line_keeper) - 2] == COMMA))
	{
		syntaxError("Error: .data instruction missing at least one operand.", linep);
		err_on(igp);
	}
	/*adding the information*/
	while(token != NULL)
	{
		if(!data_operand_short_integer_check(data, token, igp, linep, dcp))
		{
			break;
		}
		token = strtok(NULL, COMMA_TOK);
	}		
}

/*checks if it is new data and return if does*/
int is_data(char *token)
{
	/*check if new data or not*/
	int check = 0;
	if(!strcmp(token, DATA) || !strcmp(token, STRING) || !strcmp(token, STRUCT))
	{
		check++;
	}
	return check;
}

/*checks if an operand of data is a good short integer*/
int data_operand_short_integer_check(short data[], char *token, int *igp, int *linep, int *dcp)
{
	/*minus on if there is minus check if it is a good number, value the integer value*/
	short minus_on = 1;
	long value;
	long *valuep = &value;
	short *minp = &minus_on;
	int check = short_check(token, linep, igp, minp, valuep);
	if(*igp)
	{
		data[*dcp] = value*minus_on;
		(*dcp)++;
	}
	return check;
}

/*checks if operand is short integer*/
int short_check(char *token, int *linep, int *igp, short *minp, long *valuep)
{
	/*check if good operand. we will check if the long is not greater than short. 
	prob to keep the problems, temp to keep token without plus or minus*/
	short check = 1;
	char *prob, *temp;
	temp = token;
	/*we dont care about space before operand... also checking for minus\plus sign*/
	if(token[0] == SPACE)
	{
		temp++;
	}
	if(token[0] == PLUS)
	{
		temp++;
	}
	if(token[0] == MINUS)
	{
		temp++;
		(*minp) = -1;
	}
	(*valuep) = strtol(temp, &prob, BASE_TEN);
	/*maybe not a number?*/
	if(prob[0] != END_S)
	{
		if(strcmp(prob, SPACE_TOK))
		{
			syntaxError("Error: operand is not a short integer.", linep);
			err_on(igp);
			token = strtok(NULL, COMMA_TOK);
			check = 0;
		}
	}
	/*if number is too big*/
	if((*valuep) > SHRT_MAX)
	{
		syntaxError("Error: operand is not a short integer.", linep);
		err_on(igp);
		token = strtok(NULL, COMMA_TOK);
		check = 0;
	}
	return check;
}
