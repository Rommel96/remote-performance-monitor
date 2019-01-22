/* Estos son los ficheros de cabecera usuales */
#include <stdio.h>          
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>
#include <syslog.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include "include/csapp.h"
#include "include/read.h"
#include "include/daemon_mode.h"

#define PORT 8000 /* El puerto por defecto que será abierto */
#define BACKLOG 1024 /* El número de conexiones permitidas */

void *thread(void *vargp);
void echo(int connfd);

char *ctime_s(){	//Hora del sistema
	time_t local_time;
	time(&local_time);
	char *time=ctime(&local_time);
	time[strlen(time)-1]='\0';
	return time;
}

//Variables globales que determinan si una opción esta siendo usada
int cflag = 0; //Opción -c, ejecucion en modo consola
int ldflag = 0; //Opción -l, archivo default donde se guardara los eventos
int lpflag = 0; //Opción -l, archivo personalizado donde se guardara los eventos
int dflag = 0; //se ejecuta como demonio
int sflag = 0; //guarda eventos en el sistema
FILE *filelog; //file stream dond se guardar los eventos


int main(int argc, char *argv[])
{
   char *puerto= "8000";

   int fd, *fd2; /* los ficheros descriptores */

   struct sockaddr_in server; 
   /* para la información de la dirección del servidor */

   struct sockaddr_in client; 
   /* para la información de la dirección del cliente */

   socklen_t sin_size=sizeof(struct sockaddr_in);
   pthread_t tid;

   openlog("monitord", LOG_CONS, LOG_DAEMON);

  if (argc >4) { 
      /* esto es para validar el numero de argumentos del programa */
      fprintf(stderr,"Number incorrects of options\n  Use <-h> for Help!\n");
      exit(-1);
   }else if(argc==1){
	sflag=1;
	dflag=1;
	}
//Validacion de los argumentos
  if((argc==2)&&strcmp(argv[1], "-h") == 0){
	printf("monitord broadcasts the host's usage and performance data to all connected clients.\n\nUsage:\n  monitord [-c] [<port>]\n  monitord [-l <log_file>] [<port>]\n  monitord -h\n\nOptions:\n  -h             Help, show this screen.\n  -c             Console mode.\n  -l <log_file>  Log file to use [default: log.txt].\n");
	exit(0);
   }else if((argc==2&&strlen(argv[1])>3)){/* Asignacion del puerto personalizado */
		puerto=argv[1];
		sflag=1;
		dflag=1;
         }else if((argc==3)&&strcmp(argv[1], "-c") == 0){
		printf("Modo consola\n");
		puerto=argv[2];
		cflag=1;
		closelog();
         }else if((argc==3)&&strcmp(argv[1], "-l") == 0){
		printf("Los eventos se guardaran en log.txt\n");
		puerto=argv[2];
		lpflag=1;
		dflag=1;
		filelog=fopen("log.txt","a");
		closelog();
         }else if((argc==4)&&strcmp(argv[1], "-l") == 0){
		printf("Los eventos se guardaran en %s\n",argv[2]);
		puerto=argv[3];
		ldflag=1;
		filelog=fopen(argv[2],"a");
		dflag=1;
		closelog();
	 }else if(argc !=1){
		fprintf(stderr, "error in the arguments\n  Use <-h> for Help!\n");
		exit(-1);
	 }
		if(dflag)
			daemonize();
	  
	 fd = Open_listenfd(puerto);
   
	   while(1) {
	      //read_sysinfo();
	      fd2=Malloc(sizeof(int));
	      /* A continuación la llamada a accept() */
	      *fd2 = Accept(fd,(struct sockaddr *)&client,&sin_size);	      

	      if(cflag)
	      	printf("[%s] Conexion establecida con %s\n",ctime_s(),inet_ntoa(client.sin_addr) );
	      if(sflag)
		syslog(LOG_NOTICE,"Conexion establecida con %s\n",inet_ntoa(client.sin_addr));
	      if(lpflag || ldflag)
		fprintf(filelog,"[%s\b] Conexion establecida con %s\n",ctime_s(),inet_ntoa(client.sin_addr) );
	      /* que mostrará la IP del cliente */
		
		pthread_create(&tid, NULL, thread, fd2);

	   }
}


/*void *thread(void *vargp)
{
	int fd2 = *((int *)vargp);
	pthread_detach(pthread_self());
	Free(vargp);
	//hash(connfd);
	char tmp[MAXBUF];
	int flag=0;
//recv(fd2,tmp,MAXBUF,0)==0 ||
while( flag==0){
	read_sysinfo();
   	struct stat mi_stat;
   	if (stat("info.txt", &mi_stat) < 0) {
	if(cflag) 
		fprintf(stderr, "Archivo no encontrado!\n");
	if(sflag)
		syslog(LOG_NOTICE,"Archivo no encontrado!\n");
	if(lpflag || ldflag)
		fprintf(filelog,"Archivo no encontrado!\n");
	exit(-1);
	}
   	size_t size = mi_stat.st_size;
   	char contenido[size];
   	int file= Open("info.txt",O_RDONLY,0);
   	rio_readn(file,contenido,size);
   	close(file);
	pid_t pid;
	int status;
	if((pid=fork())==0){
		write(fd2,contenido,size);
		//que enviará el mensaje al cliente
		exit(0);
		}
	waitpid(pid,&status,0);
      	if(status!=0){
		flag=1;
		if(cflag)
	      		printf("Se desconecto un cliente\n");
		if(sflag)
			syslog(LOG_NOTICE,"Se desconecto un cliente\n");
		if(lpflag || ldflag)
			fprintf(filelog,"Se desconecto un cliente\n");
	}
	sleep(1);
}
	Close(fd2);
	return NULL;
}*/


void *thread(void *vargp)
{
	int connfd = *((int *)vargp);
	pthread_detach(pthread_self());
	Free(vargp);
	echo(connfd);
	Close(connfd);
	return NULL;
}

void echo(int fd2)
{

	char tmp[MAXBUF];
	int flag=0;

while( flag==0){
//Rio_writen(fd2,"hola",6);
	read_sysinfo();
   	struct stat mi_stat;
   	if (stat("info.txt", &mi_stat) < 0) {
	if(cflag) 
		fprintf(stderr, "Archivo no encontrado!\n");
	if(sflag)
		syslog(LOG_NOTICE,"Archivo no encontrado!\n");
	if(lpflag || ldflag)
		fprintf(filelog,"Archivo no encontrado!\n");
	exit(-1);
	}
   	size_t size = mi_stat.st_size;
   	char contenido[size];
   	int file= Open("info.txt",O_RDONLY,0);
   	rio_readn(file,contenido,size);
   	close(file);
	pid_t pid;
	int status;
	if((pid=fork())==0){
		write(fd2,contenido,size);
		//que enviará el mensaje al cliente
		exit(0);
		}
	waitpid(pid,&status,0);
      	if(status!=0){
		flag=1;
		if(cflag)
	      		printf("[%s] Se desconecto un cliente\n",ctime_s());
		if(sflag)
			syslog(LOG_NOTICE,"Se desconecto un cliente\n");
		if(lpflag || ldflag)
			fprintf(filelog,"[%s] Se desconecto un cliente\n",ctime_s());
	}
	sleep(1);
}

}
