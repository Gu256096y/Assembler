#include "examination.h"

/*syntax error in the first or in the second examination*/
void syntaxError(char err[], int *linep)
{
	fprintf(stderr, "Line %d: ", *linep);
	fprintf(stderr, "%s\n", err);
}

/*igp goes down if needed*/
void err_on(int *igp)
{
	if(*igp)
	{
		(*igp)--;
	}
}

