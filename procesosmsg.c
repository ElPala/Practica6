#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/types.h>

#define CICLOS 10

char * pais[3] = {
  "Peru",
  "Bolvia",
  "Colombia"
};

typedef struct {
	long msg_type;
	char mensaje[100];
	} MSGTYPE;

int id;
MSGTYPE me;
MSGTYPE mr;

void proceso(int i) {
  int k;
  for (k = 0; k < CICLOS; k++) {

    msgrcv(id, &mr, sizeof(MSGTYPE)-sizeof(long), 1, 0);

    printf("Entra %s", pais[i]);
    fflush(stdout);
    sleep(rand() % 3);
    printf("- %s Sale\n", pais[i]);
    // Salida de la sección crítica
    // Espera aleatoria fuera de la sección crítica

    strcpy(me.mensaje,"Fin");
    msgsnd(id,&me,sizeof(MSGTYPE)-sizeof(long),0);
    sleep(rand() % 3);
  }
  exit(0);
  // Termina el proceso
}

int main() {
  int pid;
  int status;
  int i;

  id = msgget(0x1235, IPC_CREAT | 0666);
  me.msg_type = 1;
  strcpy(me.mensaje,"Start");
  msgsnd(id,&me,sizeof(MSGTYPE)-sizeof(long),0);

  srand(getpid());
  for (i = 0; i < 3; i++) {
    // Crea un nuevo proceso hijo que ejecuta la función proceso()
    pid = fork();
    if (pid == 0)
      proceso(i);
  }
  for (i = 0; i < 3; i++)
    pid = wait( & status);

  msgctl(id, IPC_RMID, NULL);

}
