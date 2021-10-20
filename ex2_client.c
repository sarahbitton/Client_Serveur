#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include<sys/wait.h>
#include <signal.h>
#include <sys/types.h>

#define MAX 15


void func(int sig)
{
	FILE *fp;

	char name[50];
	printf("Client  : %s\n",name);
	if ((fp = fopen(name, "r")) == NULL)
	{
		perror("File opening error\n");
		exit(-1);
	}
	char result[MAX];
	fgets(result,MAX,fp);// read from file 
		
	printf("Client %d Result  : %s\n",getpid(), result);
	signal(SIGUSR2, func);
	fclose(fp);
	exit(0);
}


int checkerToServe(){ 
	int counter = 0;
	int fd_to_serveur;
	do{
	 
	fd_to_serveur=open("to_srv.txt", O_RDONLY);
	int random = rand()%4;
	random++;
	counter++;
	printf("need to wait %d, counter= %d\n",random,counter); 
	sleep(random);
	}while((fd_to_serveur != -1) && (counter <= 10)); // fait le while tant que cest vrai 
	if(fd_to_serveur == -1) // fichier etait ferme et pas utiliser par un client 
		fd_to_serveur=open("to_srv.txt",(O_WRONLY| O_TRUNC| O_CREAT),0666);
	return fd_to_serveur;	
}


int main(int argc, char *argv[])
{
	signal(SIGUSR2, func);
	FILE *fp;
	if(argc != 5)
	{
		printf("error"); //error in the numbers of args
		exit(-1);
	}
	

//Check if connexion with the serveur already exists

int fd_to_serveur = open("to_srv.txt", O_RDONLY);
	if(fd_to_serveur == -1) //-1 il a pas reussit a ouvire 
		fd_to_serveur=open("to_srv.txt",(O_WRONLY| O_TRUNC| O_CREAT),0666); //pour le premier client  
	else{ 
	fd_to_serveur=checkerToServe();
	
	if(fd_to_serveur== -1){ 
		printf("Error the client could not connect to server\n");	
		exit(1);
	}
	}

	
	
	dup2(fd_to_serveur,1);
	printf("%d\n%s\n%s\n%s\n", getpid(), argv[2],argv[3],argv[4]);
	
	
	
	
	fclose(fp);
	
	int pid_server = atoi(argv[1]);
	kill(pid_server, SIGUSR1); //send to the serveur
	pause();
	
	exit(0);
	 
	
}
