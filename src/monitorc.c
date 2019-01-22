#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>  
#include <termios.h>
#include <pthread.h>
/* netbd.h es necesitada por la estructura hostent ;-) */

#define PORT 8000         
/* El Puerto por defecto abierto del nodo remoto */

#define MAXDATASIZE 2000   
/* El número máximo de datos en bytes */

int mygetch(); //funcion que permitira salir del programa
void *thread(void *vargp); //funcion hilo para validar la salida del programa


void main(int argc, char *argv[])
{
   int puerto=PORT; 

   pthread_t tid;

   int fd, numbytes=1;       
   /* ficheros descriptores */

   char buf[MAXDATASIZE];  
   /* en donde es almacenará el texto recibido */

   struct hostent *he;         
   /* estructura que recibirá información sobre el nodo remoto */

   struct sockaddr_in server;  
   /* información sobre la dirección del servidor */

   if (argc >3) { 
      /* esto es para validar el numero de argumentos del programa */
      fprintf(stderr,"Number incorrects of options\n  Use <-h> for Help!\n");
      exit(-1);
   }
//printf("%s\n",argv[1]);
   if(strcmp(argv[1], "-h") == 0){
	printf("monitorc connects to a remote monitord service and display the server’s usage and performance data.\n\nUsage:\n  monitorc [<ip>] [<port>]\n  monitorc -h\n\nOptions:\n  -h             Help, show this screen.\n");
	exit(0);
   }

/* Asignacion del puerto personalizado */
   if(argc == 3)
	puerto=atoi(argv[2]);

   if ((he=gethostbyname(argv[1]))==NULL){       
      /* llamada a gethostbyname() */
      fprintf(stderr,"gethostbyname() error\n  Use <-h> for Help!\n");
      exit(-1);
   }


   if ((fd=socket(AF_INET, SOCK_STREAM, 0))==-1){  
      /* llamada a socket() */
      fprintf(stderr,"socket() error\n  Use <-h> for Help!\n");
      exit(-1);
   }

   server.sin_family = AF_INET;
   server.sin_port = htons(puerto); 
   
   server.sin_addr = *((struct in_addr *)he->h_addr);  
   
   bzero(&(server.sin_zero),8);

   if(connect(fd, (struct sockaddr *)&server,
      sizeof(struct sockaddr))==-1){ 
      /* llamada a connect() */
      printf("connect() error\n  Use <-h> for Help!\n");
      exit(-1);
   }

while(send(fd,"ok",3,0)>0){
system("clear");
   if ((numbytes=recv(fd,buf,MAXDATASIZE,0)) == -1){  
      /* llamada a recv() */
      fprintf(stderr,"Error en recv() \n  Use <-h> for Help!\n");
      //exit(-1);
   }
//printf("%d\n",numbytes);
   buf[numbytes]='\0';

printf("\t---MONITOR REMOTO DE RENDIMIENTO---\n");

   printf("%s\n",buf); 
   /* muestra el mensaje de que envio el servidor =) */

printf("Presione q para salir\n");

pthread_create(&tid, NULL, thread, NULL);

sleep(1);

}
     close(fd);/* cerramos fd =) */

}

int mygetch( ) {
	
	struct termios oldt, newt;
	int ch;

	tcgetattr( STDIN_FILENO, &oldt );

	newt = oldt;
	newt.c_lflag &= ~( ICANON | ECHO );

	tcsetattr( STDIN_FILENO, TCSANOW, &newt );

	ch = getchar();

	tcsetattr( STDIN_FILENO, TCSANOW, &oldt );

	return ch;
}

void *thread(void *vargp){
	pthread_detach(pthread_self());
	if(mygetch()=='q')
		exit(0);
	return NULL;
}

