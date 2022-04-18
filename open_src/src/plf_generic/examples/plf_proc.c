#include <plf/platform.h>
#include <plf/plf_proc.h>
#define NUM_THREADS 5
static int exited[NUM_THREADS];
void start_thread(void *threadid)
{
   int tid;
   tid = (int)threadid;
   printf("Thread %d is started and sleeps for %d seconds\n", tid, tid);
   sleep(tid);
   exited[tid] = 1;
   printf("Thread %d is exiting.\n", tid);
   NThreadExit(NULL);
}
int main() 
{
  pid_t pid;
  pthread_t threads[NUM_THREADS];
  pthread_attr_t attr;
  int t, cnt;
  /* Check if process zero is running. If it is there is a serious problem. */
  if (CheckProcess((pid_t)0) != 0)
    return 1;
  /* Check if this process is running. It should be. */
  pid = getpid();
  if (CheckProcess(pid) != 1)
    return 1;
  /* Initialize and set thread detached attribute */
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
  /* Start NUM_THREADS detached threads. */
  for(t=0;t<NUM_THREADS;t++){
    exited[t] = 0;
    if (NThreadCreate(&threads[t], &attr, (void *)start_thread, (void *)t) != 0)
      return 1;
    if (NThreadDetach(&threads[t]) != 0)
      return 1;
  }
  pthread_attr_destroy(&attr);
  /* Wait for all the threads to exit */
  while(1) {
    cnt = 0;
    for(t=0;t<NUM_THREADS;t++)
      if (exited[t] == 1)
	cnt++;
    if (cnt == NUM_THREADS)
      break;
  }
  printf("Process handling OK\n");
  return 0;
}
