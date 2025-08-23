#include "preProccessor.h"

/*if there is an error no need making the am file raising problem*/
void error_on(int *igp, int *pp, FILE *fam)
{
	if(*igp)
	{
		(*igp)--;
		fclose(fam);
		remove(TEMP_NAME);
	}
	(*pp)++;
}

/*no need making am file*/
void error_on_v2(int *igp, FILE *fam)
{
	if(*igp)
	{
		(*igp)--;
		fclose(fam);
		remove(TEMP_NAME);
	}
}

/*too many arguments when calling a macro*/
void macroCallError(int *lcp)
{
	fprintf(stderr, "Unnecessary arguments after calling a macro in line %d.\n", *lcp);
}

/*unnessecary arguments after endmacro*/
void endMacroError(int *lcp)
{
	fprintf(stderr, "Unnecessary arguments after endmacro in line %d.\n", *lcp);
}

/*too many arguments when defining a macro*/
void unneccessaryArgError(int *lcp)
{
	fprintf(stderr, "Unnecessary arguments after defining a macro in line %d.\n", *lcp);
}

/*macro name taken by another macro*/
void takenName(int *lcp, char *name)
{
	fprintf(stderr, "Name of the macro defined in line %d is taken by another macro: %s.\n", *lcp, name);
}

/*macro name taken by operation data register etc*/
void takenNameV2(int *lcp, char *not_valid)
{
	fprintf(stderr, "Name of the macro defined in line %d is taken by an operation,\nkind of data, or register: %s.\n", *lcp, not_valid);
}

/*missing name definiotion*/
void missingName(int *lcp)
{
	fprintf(stderr, "Missing name definition of the macro defined in line %d.\n", *lcp);
}
