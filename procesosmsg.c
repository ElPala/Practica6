#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/msg.h>
#include <wait.h>

typedef struct {
	long msg_type;		// Tipo de mensaje, debe ser long
	char mensaje[100];	// Contenido
	} MSGTYPE;


#define CICLOS 10
char *pais[3]={"Peru","Bolvia","Colombia"};
#define KEY 0x1111



void proceso(int i, int idsem)
{
  int k;
  for(k=0;k<CICLOS;k++)
  {
    waitsem(idsem);

    printf("Entra %s",pais[i]);
    fflush(stdout);
    sleep(rand()%3);
    printf("- %s Sale\n",pais[i]);

    signalsem(idsem);
    // Espera aleatoria fuera de la sección crítica
    sleep(rand()%3);
  }
  exit(0); // Termina el proceso
}

int main()
{
  int pid;
  int status;
  int shmid, shmidsem;
  int i;
  int idsem;
  int entradasMsg, terminadosMsg;
  int paisesMsg[3];
  // Crear un buzón o cola de mensajes
	entradasMsg=msgget(0x1234,0666|IPC_CREAT);
  terminadosMsg=msgget(0x1235,0666|IPC_CREAT);
  paisesMsg[0]=msgget(0x1236,0666|IPC_CREAT);
  paisesMsg[1]=msgget(0x1237,0666|IPC_CREAT);
  paisesMsg[2]=msgget(0x1238,0666|IPC_CREAT);
  srand(getpid());
  for(i=0;i<3;i++)
  {
// Crea un nuevo proceso hijo que ejecuta la función proceso()
	  pid=fork();
	  if(pid==0)
	    proceso(i,idsem);
  }
  for(i=0;i<3;i++)
    pid = wait(&status);
	return 0;
}
