#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//Printing function no options
int print (FILE *f);
//Prints a line with no ending
int printline (FILE *f);
//Print numbered lines Option: -n
int numline (FILE *f);
//Print line with $ at end Option: -E
int dollarline (FILE *f);
//Prints and displays non-printing characters Option: -v
int npchars (FILE *f);
//Prints line ending S and non-printing chars Option: -e
int dlnpchars (FILE *f);
//Prints non-printing chars and tabs as ^I Option: -t
int tabnpchars (FILE *f);
//Prints numbered non-blank lines Option: -b
int nblank (FILE *f);
//Prints multiple adjacent blank lines into one blank line Option: -s
int sblank (FILE *f);

int main (int argc, char *argv[])
{
    char *opt[argc];
    char *files[argc];
    int optc = 0;
    int filec = 0;
    int i;
    //Process command arguments
    for(i = 1; i < argc; i++)
    {
        //Options
        if (argv[i][0] == '-')
        {
            opt[optc] = argv[i];
            optc++;
        }
        //Files
        else
        {
            files[filec] = argv[i];
            filec++;
        }				
    }
    FILE *f;
    f = stdin;
    //If no arguments or options
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
                /* Options*/
                case 'e': dlnpchars(f); break;              
                case 'b': nblank(f); break;
                case 'n': numline(f); break;
                case 't': tabnpchars(f); break;
                case 'v': npchars(f); break;
                case 's': sblank(f); break;
                case 'E': dollarline(f); break;
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
                        case 'e': dlnpchars(f); break;              /* Options*/
                        case 'b': nblank(f); break;
                        case 'n': numline(f); break;
                        case 't': tabnpchars(f); break;
                        case 'v': npchars(f); break;
                        case 's': sblank(f); break;
                        case 'E': dollarline(f); break;
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
        perror("An error has occurred!");	
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
        printf("	%d ", num);
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
int npchars (FILE *f)
{
    int c;
    if ( f != NULL )
    {
        while ((c = fgetc(f)) != EOF )
        {	
            if (c != 10 && c != 9 && iscntrl(c))
            {
                putchar('^');
                putchar(c == '\177' ? '?' :c | 0100);
            }
            else
            {	
                printf("%c", c);
            }
        }
    }
    else
    {
        perror("An error has occurred!");	
    }
    return EXIT_SUCCESS;
}
int dlnpchars (FILE *f)
{
    int c;
    if ( f != NULL )
    {
        while ((c = fgetc(f)) != EOF )
        {	
            if (c != 10 && c != 9 && iscntrl(c))
            {
                putchar('^');
                putchar(c == '\177' ? '?' :c | 0100);
            }
            if (c == '\n')
            {
                printf("$\n");
            }
            else
            {	
                printf("%c", c);
            }
        }
    }
    else
    {
        perror("An error has occurred!");	
    }
    return EXIT_SUCCESS;
}
int tabnpchars (FILE *f)
{
    int c;
    if ( f != NULL )
    {
        while ((c = fgetc(f)) != EOF )
        {	
            if (c == '\t') 
            {
                    printf("^I");
            }
            else if (c != 10 && c != 9 && iscntrl(c))
            {
                    putchar('^');
                    putchar(c == '\177' ? '?' :c | 0100);
            }
            else
            {	
                    printf("%c", c);
            }
        }
    }
    else
    {
        perror("An error has occurred!");	
    }
    return EXIT_SUCCESS;
}
int nblank (FILE *f)
{
    int c;
    int num = 1;
    while ((c = fgetc(f)) != EOF)
    {
        if (c != '\n')
        {
            ungetc(c,f);
            printf("	%d ", num);
            printline(f); printf("\n");
            num++;
        }
    }
	return EXIT_SUCCESS;
}
int sblank (FILE *f)
{
    int c;
    int bc = 0;
    while ((c = fgetc(f)) != EOF)
    {
        if (c == '\n')
        {
            if (bc == 0)
            {
                    ungetc(c,f);
                    printline(f); printf("\n");
            }
            bc = 1;
        }
        else 
        {
            bc = 0;
        }
        if (bc == 0)
        {
            ungetc(c,f);
            printline(f); printf("\n");
        }

    }
    return EXIT_SUCCESS;
}

