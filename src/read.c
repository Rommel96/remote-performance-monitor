/* Codigo que lee la informacion del sistema*/
#include "include/csapp.h"
#include "include/read.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/sysinfo.h>
#define TAM 100

int read_sysinfo(void){
	int fd_version,fd_loadavg;
	struct stat mi_stat;
	FILE * info=fopen("info.txt","w+");
	fd_version=Open("/proc/version_signature",O_RDONLY,0);
	FILE * fd_uptime=fopen("/proc/uptime","r");
	fd_loadavg=Open("/proc/loadavg",O_RDONLY,0);
	FILE * fd_stat=fopen("/proc/stat","r");
	FILE * fd_meminfo=fopen("/proc/meminfo","r");
//Lectura de VERSION	
	if(fstat(fd_version,&mi_stat) < 0);
			//fprintf(stderr, "Archivo no existe!\n");
	size_t size=mi_stat.st_size;
	char buff[TAM]="\0";
	size=rio_readn(fd_version,buff,TAM);
//Lectura de UPTIME	
	float tiempo;
	int h,m,s;
	fscanf(fd_uptime,"%f ",&tiempo);
	h=(tiempo/3600);
	m=(tiempo-h*3600)/60;
	s=tiempo-(h*3600+m*60);
//Lectura de LOADAVG
	char buff_load[TAM]="\0";
	size=rio_readn(fd_loadavg,buff_load,14);
//Lectura de los CORES
	struct sysinfo sys_info;
	char cpu[TAM],temp[10]="";
	double tam=0,porc=0;
	double u;
	double uso[get_nprocs()];
	for(int i=0;i<=get_nprocs();i++){
		fgets(cpu,TAM,fd_stat);
		if(i==0)
			sscanf(cpu,"%s %lf ",temp,&tam);
		else
			sscanf(cpu,"%s %lf ",temp,&porc);
		if(i>0){
			uso[i-1]=(porc/tam)*100;
		}
	}
//Lectura de la MEMORIA
	char mem[TAM],totalram[20],freeram[20];
	for(int i=0;i<2;i++){
		fgets(mem,TAM,fd_meminfo);
		if(i==0){
			sscanf(mem,"%s %s",temp,totalram);
			}
		else{
			sscanf(mem,"%s %s",temp,freeram);
			}
	}
	char numprocs[TAM],numero[10]="",procs_run[5]="";
	int flag=0;
	while(fgets(numprocs,10,fd_stat)!=NULL && flag==0){
		if(strcmp(numprocs,"processes")==0){
			fgets(numero,10,fd_stat);
		}
		if(strcmp(fgets(numprocs,14,fd_stat),"procs_running")==0){
			fgets(procs_run,5,fd_stat);
			flag=1;
		}
	}
//Impresion del informacion con formato en archivo
	fprintf(info,"Sistema: %sUptime: %dh-%dm-%ds\nPromedio carga (1-min, 5-min, 15-min): %s\nCores: %d\n",buff,h,m,s,buff_load,get_nprocs());

	for(int i=0;i<get_nprocs();i++){
		fprintf(info,"Core %d: %0.2lf\n",i+1,*(uso+i));
	}
	fprintf(info,"Memoria Total: %s kB\nMemoria Libre: %s kB\nNúmero de procesos:%sNúmero de procesos en ejecución:%s",totalram,freeram,numero,procs_run);

/*//Impresion del informacion con formato en consola
	printf("Sistema: %sUptime: %dh-%dm-%ds\nPromedio carga (1-min, 5-min, 15-min): %s\nCores: %d\nMemoria Total: %s kB\nMemoria Libre: %s kB\nNúmero de procesos:%sNúmero de procesos en ejecución:%s\n",buff,h,m,s,buff_load,get_nprocs(),totalram,freeram,numero,procs_run);*/

//Cerrar a los ARCHIVOS
fclose(info);
fclose(fd_uptime);
fclose(fd_stat);
fclose(fd_meminfo);
close(fd_version);
close(fd_loadavg);

	return 1;
}
