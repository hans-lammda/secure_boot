#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

/*

  http://www.parkes.atnf.csiro.au/observing/documentation/computing_notes/linux_threads.html
  pthread_attr_init (&id_attr);
  pthread_attr_setdetachstate(&id_attr, PTHREAD_CREATE_DETACHED);
  pthread_create( , &id_attr, , ); 
 
 ulimit -s 2048 

gcc -pthread -o thread-limit thread-limit.c
./thread-limit
*/ 


#define MAX_THREADS 10000
int i;

void run(void) {
char c;
if (i < 10)
printf("Address of c = %u KB\n", (unsigned int) &c / 1024);
sleep(60 * 60);
}

int main(int argc, char *argv[]) {
int rc = 0;
pthread_t thread[MAX_THREADS];
printf("Creating threads ...%d\n",getpid());
for (i = 0; i < MAX_THREADS && rc == 0; i++) {
rc = pthread_create(&(thread[i]), NULL, (void *) &run, NULL);
if (rc == 0) {
pthread_detach(thread[i]);
if ((i + 1) % 100 == 0)
printf("%i threads so far ...\n", i + 1);
}
else
printf("Failed with return code %i creating thread %i.\n",
rc, i + 1);
}
sleep(20);
exit(0);
}
