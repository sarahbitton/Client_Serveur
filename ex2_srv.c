#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include<sys/wait.h>
#include <sys/types.h>


void func(int sig)
{

	signal(SIGUSR1, func);
	pid_t pid;
		if ((pid = fork()) < 0) {
			perror("Error creating child");
			exit(-1);
		}
		else if (pid == 0) // child
		{
			
	//get clients info
	int pid1, num1, operation, num2;
	FILE *fp1;
	FILE *fp2;
	if (( fp1 = fopen("to_srv.txt", "r")) == NULL)
	{
		perror("File opening error\n");
		exit(-1);
	}
	fscanf(fp1, "%d\n%d\n%d\n%d\n", &pid1,&num1,&operation,&num2);
	fclose(fp1);
	
	//removing to_srv file after using
	remove("./to_srv.txt");
	
	int result;
	switch(operation){ 
		case 1:
		result=num1+num2;
		break;
		case 2:
		result=num1-num2;
		break;
		case 3:
		result=num1*num2;
		break;
		case 4:
		if(num2 == 0)
		{
			printf("Can't divide by 0\n");
			result = -1;
		}
		else 
			result=num1/num2;
		break;
		default:
		break;
	}
		
	
	
	//creating to_clientxxxx  file
	char name[50];
	if(sprintf(name, "to_client%d.txt", pid1)<0)
	{
		perror("sPrintf error\n");
		exit(-1);
	}
	
	if (( fp2 = fopen(name, "w+")) == NULL)
	{
		perror("File opening error\n");
		exit(-1);
	}
	fprintf(fp2, "%d", result);	
	
	fclose(fp2);
	
	kill(pid1, SIGUSR2);
	
	exit(0);
	
		}
		else {
			signal(SIGCHLD,SIG_IGN);
		}
	
}


int main(int argc, char *argv[])
{
	printf("pid server %d\n",getpid());
	signal(SIGUSR1, func);
	int status;

	while(1)
	{
		pause();
		wait(&status);
	}
}
