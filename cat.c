#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int print (FILE *f);
int printline (FILE *f);
int numline (FILE *f);
int dollarline (FILE *f);

int main (int argc, char *argv[])
{
	char *opt[argc];
	char *files[argc];
	int optc = 0;
	int filec = 0;
	int i;
	for(i = 1; i < argc; i++)
		{
			if (argv[i][0] == '-')
			{
				opt[optc] = argv[i];
				optc++;
			}
			else
			{
				files[filec] = argv[i];
				filec++;
			}
				
		}
		//printf("opt=%d files=%d\n", optc,filec);
			
	FILE *f;
	f = stdin;
		if (filec == 0 && optc == 0)
		{
			print(f);
		}
		if (filec == 0 && optc >= 1)
		{
			int i;                                                                                    
			for( i = 0; i < optc; i++ )            
			{
				switch( opt[i][1] )
				{
					case 'e': dollarline(f); break;              /* Options*/
					case 'E': dollarline(f); break;
					case 'n': numline(f); break;
					case 'N': numline(f); break;
				}
			}
		}
		if (filec >= 1 && optc == 0)
		{
			int i;
			for(i = 0; i < filec; i++)
			{
			f = fopen(files[i], "r");
				if ( f == 0 )
				{
					printf( "Could not open file: %s\n", files[i]);
				}
				else 
				{
				print(f);
				}					
			}
		}
		else
		{
			int i;
			for(i = 0; i < filec; i++)
			{
			f = fopen(files[i], "r");
				if ( f == 0 )
				{
					printf( "Could not open file: %s\n", files[i]);
				}
				else 
				{
					int j;
					for( j = 0; j < optc; j++ )            
					{
						switch( opt[j][1] )
						{
							case 'e': dollarline(f); break;              /* Options*/
							case 'E': dollarline(f); break;
							case 'n': numline(f); break;
							case 'N': numline(f); break;
						}
					}
				}					
			}
		}
	return EXIT_SUCCESS;
}
int printline (FILE *f)
{
	int c;
	if ( f != NULL )
	{
		while ((c = fgetc(f)) != '\n' )
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
int print (FILE *f)
{
	int c;
	while ((c = fgetc(f)) != EOF)
	{
		ungetc(c,f);
		printline(f); printf("\n");
	}
	return EXIT_SUCCESS;
}
int numline (FILE *f)
{
	int c;
	int num = 1;
	while ((c = fgetc(f)) != EOF)
	{
		ungetc(c,f);
		printf("\t%d  ", num);
		printline(f); printf("\n");
		num++;
	}
	return EXIT_SUCCESS;
}
int dollarline (FILE *f)
{
	int c;
	while ((c = fgetc(f)) != EOF)
	{
		ungetc(c,f);
		printline(f); printf("$\n");
	}
	return EXIT_SUCCESS;
}

