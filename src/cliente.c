#include "include/monitor.pb-c.h" 
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
#include <pthread.h>
#include <termios.h>      
/* netbd.h es necesitada por la estructura hostent ;-) */

#define PORT 8000         
/* El Puerto por defecto abierto del nodo remoto */

#define MAXDATASIZE 2000   
/* El número máximo de datos en bytes */

static int fd;

void *thread(void *vargp); //funcion hilo que estara pendiente cuando se presione 'q'
int mygetch(); //funcion que permitira salir del programa

void main(int argc, char *argv[])
{
   int puerto=PORT;
   pthread_t tid;
 

   
   size_t SysBytes,PerfBytes;       
   /* ficheros descriptores */

   uint8_t bufSys[MAXDATASIZE],bufPerf[MAXDATASIZE];  
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

   
	SystemInfo *message;
	uint8_t buf[MAXDATASIZE];
	size_t n,len,curl;
	curl=recv(fd,buf,2,0);
	n=atoi(buf);
	len=recv(fd,buf,n,0);
	message =system_info__unpack(NULL,len,buf);
	if(message==NULL){
		fprintf(stderr,"ERROR\n");
		}

while(1){
//system("clear");
printf("\e[1;1H\e[2J");
   	curl=recv(fd,buf,2,0);
	n=atoi(buf);
	len=recv(fd,buf,n,0);
	
	PerformanceInfo__LoadAvg *promload;

	PerformanceInfo *msg;
	msg=performance_info__unpack(NULL,len,buf);
	if(msg==NULL){
		fprintf(stderr,"ERROR\n");
	}
	
	promload=msg->loadavg;
	
	int h,m,s;
	h=(msg->uptime/3600);
	m=(msg->uptime-h*3600)/60;
	s=msg->uptime-(h*3600+m*60);
	

printf("\t---MONITOR REMOTO DE RENDIMIENTO---\n");

	printf("System Name: %s\n",message->system_name);
	printf("System Version: %s\n",message->version);
	printf("Uptime: %dh %dm %ds\n",h,m,s);
	printf("Load Average (1-min, 5-min, 15-min): %0.2f, %0.2f, %0.2f\n",promload->min5,promload->min10,promload->min15);
	printf("Cores: %d\n",message->num_processors);
	
	for(int i=1;i<=message->num_processors;i++){
		printf("Core %d: %d%%\n",i,msg->processor_usage[i-1]);
	}

	printf("Total Memory: %d kB\n",message->mem_total);
	printf("Free Memory: %d kB\n",msg->mem_free);
	printf("Number of processes: %d\n",msg->num_process);
	printf("Number of processes running: %d\n\n",msg->num_process_running);

   /* muestra el mensaje de que envio el servidor =) */

printf("Presione q para salir\n");

	pthread_create(&tid, NULL, thread, NULL);

sleep(1);

}

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
	if(mygetch()=='q'){
		close(fd);/* cerramos fd =) */
		exit(0);
	}
	return NULL;
}
