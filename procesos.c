#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/sem.h>
#define CICLOS 10
char *pais[3]={"Peru","Bolvia","Colombia"};
#define KEY 0x1111

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short  *array;
};

struct sembuf p = { 0, -1, SEM_UNDO};
struct sembuf v = { 0, +1, SEM_UNDO};

union semun u;

void initsem(int idsem, int val){


  union semun u;
  u.val = val;
  if (semctl(idsem, 0, SETVAL, u) < 0) {
    exit(1);
  }


}

void signalsem(int idsem){

  if (semop( idsem, &v, 1) < 0) {
      /* error handling code */
  }

}

void waitsem(int idsem){
  if (semop(idsem, &p, 1) < 0) {
      /* error handling code */
  }
}


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
  if ((idsem = semget(KEY, 1, 0666 | IPC_CREAT) < 0)) {
    return(1);
  }

	initsem(idsem, 1);
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
