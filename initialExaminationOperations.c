#include "examination.h"

/*if rts or hlt command. which means no source operand, no dest*/
void rts_hlt(char *token, int *linep, int *igp, int *icp)
{
	/*if unneccessary operand*/
	token = strtok(NULL, SPACE_TOK);
	if(token != NULL)
	{
		syntaxError("Error: Operation contains too many operands.", linep);
		err_on(igp);
	}
	if(*igp)
	{
		(*icp)++;
	}
}

/*checking if the immediate operand is good*/
void immediate(char *token, int *linep, int *igp)
{
	/*is the short integer is good*/
	short minus_on = 1;
	long value;
	short *minp = &minus_on;
	long *valuep = &value;
	/*if error*/
	if(!short_check(token + 1, linep, igp, minp, valuep))
	{
		syntaxError("Error: Operand is not a short integer.", linep);
		err_on(igp);
		return;
	}
}

/*first operand of two operanded*/
void two_operaneded_first(char *c_line_keeper, char *token, int *linep, int *igp, int *icp, short code[], short islea, short iscmp)
{
	/*r for registers*/
	reg r[] = { {"r0", 0}, {"r1", 1}, {"r2", 2}, {"r3", 3}, {"r4", 4}, {"r5", 5}, {"r6", 6}, {"r7", 7} };
	/*char token keeper will help keep the original token, and token2 for check if access for struct,
	token_keeper2 for the is direct temp2 keeping token2*/
	char *token_keeper, *token_keeper2, *token2, *temp2;
	/*first we will check if too many or not enough operands, i for the loop, nc for num of commas,k kind of operand*/
	int i, k, nc = 0;
	for(i = 0; i < strlen(c_line_keeper); i++)
	{
		if(c_line_keeper[i] == COMMA)
		{
			nc++;
		}
	}
	i = 0;
	/*too many or not enough commas*/
	if(nc != 1)
	{
		syntaxError("Error: Operation missing at least one operand or contains too many operands.", linep);
		err_on(igp);
		return;
	}
	/*missing operand?*/
	token = strtok(NULL, COMMA_TOK);
	if(token == NULL)
	{
		syntaxError("Error: Operation missing at least one operand.", linep);
		err_on(igp);
		return;	
	}
	token_keeper = (char*)malloc(sizeof(char)*strlen(token));
	if(token_keeper == NULL)
	{
		memoryAllocateError();
	}
	strcpy(token_keeper, token);
	if(token_keeper[strlen(token_keeper) - 1] == SPACE)
	{
		token_keeper[strlen(token_keeper) - 1] = END_S;	
	}
	/*missing operand?*/
	token = strtok(NULL, SPACE_TOK);
	if(token == NULL)
	{
		syntaxError("Error: Operation missing at least one operand.", linep);
		err_on(igp);
		return;	
	}
	/*checking if immediate only if not lea*/
	if(token_keeper[0] == NUM_START && !islea)
	{
		k = IMMEDIATE;
		immediate(token_keeper, linep, igp);
		/*if all good*/
		if(*igp)
		{
			code[*icp] = code[*icp] | (k << SO_PLACE);
		}
		single_operanded(token, linep, igp, icp, code, iscmp, 1, 1, 0);
		return;
	}
	/*checking if register*/
	while(i < REG_NUM && !islea)
	{
		/*if it is a reg name we can stop*/
		if(!strcmp(token_keeper, r[i].regname))
		{
			k = REG;
			if(*igp)
			{
				code[*icp] = code[*icp] | (k << SO_PLACE);
			}
			single_operanded(token, linep, igp, icp, code, iscmp, 1, 1, 1);
			return;
		}
		i++;
	}
	/*checking if direct*/
	token_keeper2 = (char*)malloc(sizeof(char)*(strlen(token_keeper) + 1));
	if(token_keeper2 == NULL)
	{
		memoryAllocateError();
	}
	strcpy(token_keeper2, token_keeper);
	if(is_legal_label_name(strcat(token_keeper2, END_L_S), linep, igp, 1))
	{
		/*if it is a legal label name it might be direct. in the second run we will 
		truely get into it*/
		k = DIRECT;
		free(token_keeper2);
		if(*igp)
		{
			code[*icp] = code[*icp] | (k << SO_PLACE);
		}
		single_operanded(token, linep, igp, icp, code, iscmp, 1, 1, 0);
		return;
	}
	free(token_keeper2);
	/*checking access to struct*/
	token2 = strtok(token_keeper, DOT);
	temp2 =(char*)malloc(sizeof(char)*(strlen(token2) + 1));
	if(temp2 == NULL)
	{
		memoryAllocateError();
	}
	strcpy(temp2, token2);
	if(is_legal_label_name(strcat(temp2, END_L_S), linep, igp, 1))
	{
		free(temp2);
		token2 = strtok(NULL, SPACE_TOK);
		/*checking if the access value is good*/
		if(token2 == NULL)
		{
			free(token_keeper);
			syntaxError("Error: No such kind of operand.", linep);
			err_on(igp);
			return;	
		}
		/*if it is good, lets add it*/
		else if(!strcmp(token2, NUM_PART) || !strcmp(token2, STR_PART))
		{
			k = ACCESS;
			if(*igp)
			{
				code[*icp] = code[*icp] | (k << SO_PLACE);
			}
			single_operanded(token, linep, igp, icp, code, iscmp, 2, 1, 0);
			free(token_keeper);
			return;
		}
		/*no such kind of operand*/
		else
		{
			free(token_keeper);
			syntaxError("Error: No such kind of operand.", linep);
			err_on(igp);
			return;	
		}
	}
	free(token_keeper);
	free(temp2);
	/*not good operand at all*/
	{
		syntaxError("Error: No such kind of operand.", linep);
		err_on(igp);
		single_operanded(token, linep, igp, icp, code, iscmp, 0, 1, 0);
	}
}

/*single operanded commands*/
void single_operanded(char *token, int *linep, int *igp, int *icp, short code[], short isprn_cmp, int how_many_to_add, int istwo, int is_reg)
{
	/*r for registers*/
	reg r[] = { {"r0", 0}, {"r1", 1}, {"r2", 2}, {"r3", 3}, {"r4", 4}, {"r5", 5}, {"r6", 6}, {"r7", 7} };
	/*char token keeper will help keep the original token, and token2 for check if access for struct, temp2 keeping token2*/
	char *token_keeper, *token_keeper2, *token2, *temp2;
	/*k kind of operand i for loop*/
	int k, i = 0;
	/*missing operand?*/
	if(!istwo)
	{
		token = strtok(NULL, SPACE_TOK);
		if(token == NULL)
		{
			syntaxError("Error: Operation missing at least one operand.", linep);
			err_on(igp);
			return;	
		}
	}
	token_keeper = (char*)malloc(sizeof(char)*strlen(token));
	if(token_keeper == NULL)
	{
		memoryAllocateError();
	}
	strcpy(token_keeper, token);
	/*checking if immediate only if prn*/
	if(token[0] == NUM_START && isprn_cmp)
	{
		k = IMMEDIATE;
		immediate(token, linep, igp);
		/*if unnessecary opperand*/
		token = strtok(NULL, SPACE_TOK);
		if(token != NULL)
		{
			syntaxError("Error: Operation contains too many operands.", linep);
			err_on(igp);
			return;
		}
		/*if all good*/
		if(*igp)
		{
			code[*icp] = code[*icp] | (k << DE_PLACE);
			(*icp) = (*icp) +  2 + how_many_to_add;
		}
		return;
	}
	/*checking if register*/
	while(i < REG_NUM)
	{
		/*if it is a reg name we can stop*/
		if(!strcmp(token, r[i].regname))
		{
			k = REG;
			/*if unnessecary opperand*/
			token = strtok(NULL, SPACE_TOK);
			if(token != NULL)
			{
				syntaxError("Error: Operation contains too many operands.", linep);
				err_on(igp);
			}
			if(*igp)
			{
				code[*icp] = code[*icp] | (k << DE_PLACE);
				if(!is_reg)
				{
					(*icp) = (*icp) + 2 + how_many_to_add;
				}
				else
				{
					(*icp) = (*icp) + 1 + how_many_to_add;
				}
			}
			return;
		}
		i++;
	}
	/*checking if direct*/
	token_keeper2 = (char*)malloc(sizeof(char)*(strlen(token) + 1));
	if(token_keeper2 == NULL)
	{
		memoryAllocateError();
	}
	strcpy(token_keeper2, token);
	if(is_legal_label_name(strcat(token_keeper2, END_L_S), linep, igp, 1))
	{
		/*if it is a legal label name it might be direct. in the second run we will 
		truely get into it*/
		k = DIRECT;
		/*if unnessecary opperand*/
		token = strtok(NULL, SPACE_TOK);
		free(token_keeper2);
		if(token != NULL)
		{
			syntaxError("Error: Operation contains too many operands.", linep);
			err_on(igp);
		}
		if(*igp)
		{
			code[*icp] = code[*icp] | (k << DE_PLACE);
			(*icp) = (*icp) + 2 + how_many_to_add;
		}
		return;
	}
	free(token_keeper2);
	/*checking access to struct*/
	token2 = strtok(token_keeper, DOT);
	temp2 =(char*)malloc(sizeof(char)*(strlen(token2) + 1));
	if(temp2 == NULL)
	{
		memoryAllocateError();
	}
	strcpy(temp2, token2);
	if(is_legal_label_name(strcat(temp2, END_L_S), linep, igp, 1))
	{
		free(temp2);
		/*checking if the access value is good*/
		token2 = strtok(NULL, SPACE_TOK);
		if(token2 == NULL)
		{
			free(token_keeper);
			syntaxError("Error: No such kind of operand.", linep);
			err_on(igp);
			return;	
		}
		/*if it is good, lets add it*/
		else if(!strcmp(token2, NUM_PART) || !strcmp(token2, STR_PART))
		{
			k = ACCESS;
			/*if unnessecary opperand*/
			token = strtok(NULL, SPACE_TOK);
			if(token != NULL)
			{
				syntaxError("Error: Operation contains too many operands.", linep);
				err_on(igp);
			}
			if(*igp)
			{
				code[*icp] = code[*icp] | (k << DE_PLACE);
				(*icp) = (*icp) + 3 + how_many_to_add;
			}
			free(token_keeper);
			return;
		}
		/*no such kind of operand*/
		else
		{
			free(token_keeper);
			syntaxError("Error: No such kind of operand.", linep);
			err_on(igp);
			return;	
		}
	}
	free(token_keeper);
	free(temp2);
	/*not good operand at all*/
	{
		syntaxError("Error: No such kind of operand.", linep);
		err_on(igp);
	}
}
