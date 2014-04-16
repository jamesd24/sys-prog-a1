/* ----------------------------------------------------------------- */
/* Command Line Interface Program  jShell.c                          */
/* ----------------------------------------------------------------- */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <libgen.h>

/* Global Variables */
int LINE_BUFFER = 512;
char* argv[128];
int back = 0;
char currentdir[512];

/* Functions */
int print_error(int errorNum, char *str);
int call_system(char *line);
int parse_line(char *line);
char* white_skip(char* str);
char* find_last_arg();
int find_last_arg_index();


/* Main Function Starts Here */
int main(void)
{
	char line[LINE_BUFFER];
   
     printf("----------------------------------------\n");
     printf("jShell: Simple CLI writen by James Dahms\n");
     printf("----------------------------------------\n");
     
     while (*line != EOF) 
     {  
		//Get Current Dir
		if (getcwd(currentdir, sizeof(currentdir)) != NULL) ;
		else print_error(106,'\0'); 
                  
		printf("jShell@%s--> ",basename(currentdir));
		fflush(stdout);   
		if( fgets (line, LINE_BUFFER, stdin) != NULL)
		{ 	
			if (strlen(line) == LINE_BUFFER-1)
			{
				print_error(101,'\0');
				//Flush stdin buffer
				int ch;
				while((ch = getchar())!='\n'  && ch != EOF  );	
			}
			
			else if(*line != '\n')
			{
				if(call_system(line) == EXIT_FAILURE)
				{
					print_error(103,line);
				}						
			}	  								
		}		          				
     }
     return EXIT_SUCCESS;
}

int call_system(char *line){
	
	pid_t pid; 
	
	parse_line(line);
	
	if(strcmp(argv[0],"&") == 0)
	{
		return EXIT_SUCCESS;
	}
	
	if(strcmp(*argv,"exit") == 0)
	{
		exit(EXIT_SUCCESS);
	}
	if(strcmp(argv[0],"cd") == 0)
	{
		chdir(argv[1]);
		return EXIT_SUCCESS;
	}
	if ((pid = fork()) < 0) 
	{          
		print_error(104,'\0');
		_exit(EXIT_FAILURE); //Fork failed exit process
    }
     
    if (pid == 0) 
    {         
		if (execvp(argv[0], argv) == -1) 
		{     
		print_error(105,*argv);
		_exit(EXIT_FAILURE); //Exec failed exit process
         }
    }
	
	if(back == 0)
	{                                 
		waitpid(-1,NULL,0);      
    }   
     	
	return EXIT_SUCCESS;
}

/* FUNCTION  parse_line                                                  					*/
/* This function takes an input line and seperates it into comands and arguments.    		*/
/* All char strings seperated by white space are stored into the argv array.                  	*/
int parse_line(char *line)
{	
	int i = 0;
	char * pch;
	if(*line == '"')
	{
		pch = strtok (line,"\"");
	}
	else
	{
		pch= strtok (line,"\n\t ");
	}
	while (pch != NULL)
	{	
		if(pch[0] == '"')
		{
			argv[i] = pch+1;
			i++;
			pch = strtok (NULL,"\"");
		}
		else
		{
			argv[i] = pch;
			i++;
			pch = strtok (NULL,"\n\t ");
		}		
	}
	//Handle for single &amp char
	if(strcmp(argv[0],"&") == 0)
	{
		print_error(107,'\0');
		return EXIT_SUCCESS;
	}
	
	if(strcmp(find_last_arg(),"&") == 0)
	{
		argv[find_last_arg_index()] = NULL;
		back = 1;
	}
	else
	{
		back = 0;
	}
	
	return EXIT_SUCCESS;
}

/* FUNCTION white_skip 									*/
/* This function skips a pointer past any white space 	*/
char* white_skip(char* str)
{
	while (isspace(*str)) ++str;	
	return str;
}

/* FUNCTION find_last_arg					*/
/* Get the last argument in argv			*/
char* find_last_arg()
{
	char* str = argv[0];
	int len = sizeof(argv)/sizeof(int);
	
	for (int i = len-1; i > 0; --i)
	{
		if(argv[i] != NULL) return argv[i];
	}
	return str;
}
/* FUNCTION find_last_arg_index					*/
/* Get the last argument index in argv			*/
int find_last_arg_index()
{
	int len = sizeof(argv)/sizeof(int);
	
	for (int i = len-1; i > 0; --i)
	{
		if(argv[i] != NULL) return i;
	}
	return 0;
}
/* FUNCTION print_error                                                				*/
/* 	The function will print the specified error.									*/
/*	This is used for error handling. All errors will be defined here.				*/
int print_error(int errorNum, char *str)
{
	switch(errorNum)
	{
		case 101: fprintf(stderr,"ERROR %i: Line input length to long\n", errorNum); break;
		case 102: fprintf(stderr,"ERROR %i: A printing error has occured\n", errorNum); break;
		case 103: fprintf(stderr,"ERROR %i: Error executing command %s \n", errorNum,str); break;
		case 104: fprintf(stderr,"ERROR %i: Forking child process failed\n", errorNum); break;
		case 105: fprintf(stderr,"ERROR %i: Execute failed, %s is an unknown command\n", errorNum,str); break;
		case 106: fprintf(stderr,"ERROR %i: Unable to find current directory\n", errorNum); break;
		case 107: fprintf(stderr,"ERROR %i: & cannot be first argument\n", errorNum); break;
		case 108: fprintf(stderr,"ERROR %i: Define new error\n", errorNum); break;
		default: fprintf(stderr,"ERROR: Unknown error\n"); return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}


	
