#include "output.h"

/*A function that gets number and returns a string that contains the value
of the number in our special 32 base*/	
char* base32_maker(short num)
{
	/*current will help us calculating the 32 base value*/
	int current = 0;		
	/*counters of how many "32" in the number, and after that how many "1"*/
	int counter_32 = 0, counter_1 = 0;
	/*a string that help us find the values of signes in our 32 base*/
	char *signs = SIGNES;
	/*that string will contain the value of the number in our 32 base*/
	char *num_32 = (char*)malloc(sizeof(char)*TWO_SIGNES);
	if(num_32 == NULL)
	{
		memoryAllocateError();
	}
	/*if the number is negative we have to to complement*/
	if(num < 0)
	{
		num = MAX_IN_FIVE_BITS*(MAX_IN_FIVE_BITS + 1) + (MAX_IN_FIVE_BITS + num) + 1;
	}
	/*counting the "32's"*/
	while((current + BASE) <= num)
	{
		current += BASE;
		counter_32++;
	}
	/*counting the "1's"*/
	while((current + 1) <= num)
	{
		current++;
		counter_1++;
	}	
	/*making the string to return it*/
	num_32[0] = signs[counter_32];
	num_32[1] = signs[counter_1];
	return num_32;
}
