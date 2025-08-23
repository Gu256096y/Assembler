#include "systemErrors.h"

/*error in allocating memory*/
void memoryAllocateError()
{
	fprintf(stderr, "Could not allocate memory.\nRun the assembler again.\nGoodbye.\n");
	exit(ERROR);
}

/*error in opening file*/
void fileOpeningError()
{
	fprintf(stderr, "could not open file.\nRun the assembler again.\nGoodbye.\n");
	exit(ERROR);
}
