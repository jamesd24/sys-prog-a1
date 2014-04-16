#include <stdlib.h>
#include <stdio.h>
int main (void)
{
int c;
	if ( stdin != NULL )
	{
		while ( (c = fgetc(stdin)) != EOF )
		{		
			printf("%c", c);
		}
	}
	else
	{
	perror("An error has occured!");	
	}
	return EXIT_SUCCESS;
}
