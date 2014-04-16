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

/* Global Variables */
int LINE_BUFFER = 512;
char* argv[128];
int back = 0;

/* Functions */
int print_error(int errorNum, char *str);
int call_system(char *line);
int parse_line(char *line);
char* white_skip(char* str);


/* Main Function Starts Here */
int main(void)
{
	 char line[LINE_BUFFER];            
     
     printf("----------------------------------------\n");
     printf("jShell: Simple CLI writen by James Dahms\n");
     printf("----------------------------------------\n");
     
     while (*line != EOF) 
     {              
		printf("jShell--> ");
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
	int status; 
	
	parse_line(line);
	
	if(strcmp(*argv,"exit") == 0)
	{
		exit(EXIT_SUCCESS);
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
	
	if(back != 1)
	{                                  
		while (wait(&status) != pid);      
    }   
     	
	return EXIT_SUCCESS;
}

/* FUNCTION  parse_line                                                  					*/
/* This function takes an input line and seperates it into comands and arguments.    		*/
/* All char strings seperated by white space are stored into the argv array.                  	*/

int parse_line(char *line)
{	
	line = white_skip(line);
	char* next_arg = strchr(line, ' ');
	int i = 0;
	
	while(next_arg != NULL)
	{
		next_arg[0] = '\0';
		argv[i] = line;
		++i;
		line = white_skip(next_arg + 1);
		next_arg = strchr(line, ' ');
	}
		
	if (line[0] != '\0') 
	{
		argv[i] = line;
		next_arg = strchr(line, '\n');
		next_arg[0] = '\0';
		++i; 
	}
	//Handle Background process args
	if(strcmp(argv[i-1],"&") == 0 && i!=1)
	{
		back = 1;
		argv[i-1] = NULL;
	}
	else
	{
		back = 0;
		argv[i] = NULL;
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
		case 106: fprintf(stderr,"ERROR %i: Define new error\n", errorNum); break;
		default: fprintf(stderr,"ERROR: Unknown error\n"); return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
	
