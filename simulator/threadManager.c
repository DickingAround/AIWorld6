#ifndef threadmanager_c
#define threadmanager_c
#include <pthread.h>

void *initChild(void *args);
void initThread(simulationManager_thread_control *tc, pthread_t *t, void *start_routine, int cn)
{ //This must be called by the parent before the child starts up
 tc->initComplete = 0;
 tc->runAgentDecision = 0;
 tc->runAgentAction = 0;
 tc->done = 0;
 tc->childNumber = cn;
 tc->numberOfChildren = NUMBER_OF_THREADS;
 tc->childHasThisLock = 'a';
 tc->parentHasThisLock = 'b'; 
 tc->subRoutine = start_routine;
 pthread_mutex_init(&(tc->actionLockA),NULL);
 pthread_mutex_init(&(tc->actionLockB),NULL);
 pthread_mutex_init(&(tc->actionLockC),NULL);
 pthread_mutex_lock(&(tc->actionLockC));
 pthread_mutex_lock(&(tc->actionLockB));
 //printf("parent, init thread0: child has %c\n",tc->childHasThisLock);
 pthread_create( t, NULL, initChild, (void*)tc);
 //printf("parent, init thread1: child has %c\n",tc->childHasThisLock);
 while(tc->initComplete == 0)
  {;}//We need to make sure lock A is taken, but taking lock C doesn't matter.
 //printf("parent, init thread2: child has %c\n",tc->childHasThisLock);
}
void *initChild(void *args)
{
 //printf("Child Init is running\n");
 functiontype subRoutine;
 simulationManager_thread_control *tc;
 tc = (simulationManager_thread_control*)args;
 subRoutine = tc->subRoutine;
 //printf("Child init: about to get lock A\n");
 pthread_mutex_lock(&(tc->actionLockA));//This child must take A before the parent can wait on it FIX ME!!!!!!!!!!
 //printf("Child init: about to signal init complete\n"); 
 tc->initComplete = 1; 
 //printf("Child init: about to get lock C\n");
 pthread_mutex_lock(&(tc->actionLockC));//This child will actually wait on C
 //printf("Child init: locks done, starting main subroutine\n");
 subRoutine(tc);
}
void blockOnChildFinishingWork(simulationManager_thread_control *tc)
{
 switch (tc->childHasThisLock)
 {
  case 'a':
    tc->childHasThisLock = 'c'; 
    pthread_mutex_lock(&(tc->actionLockA));
    break;
  case 'b':
    tc->childHasThisLock = 'a';
    pthread_mutex_lock(&(tc->actionLockB));
    break;
  case 'c':
    tc->childHasThisLock = 'b';
    pthread_mutex_lock(&(tc->actionLockC));
    break;
 }
}
void notifyChildOfWorkToDo(simulationManager_thread_control *tc) {
 //pthread_mutex_lock(&(tc->mainLock));
 //I don't need a main mutex lock, because only the parent will ever modify 'thread has this lock'
 //And the locks themselves have been proven to logically prevent any deadlocking or too-early starts
 //printf("parent, notify child of work: child has '%c'\n",tc->childHasThisLock);
 //printf("parent, notify child of work: parent probably has %c\n",tc->parentHasThisLock);
 switch (tc->childHasThisLock) {
  case 'a':
    //tc->childHasThisLock = 'c'; 
    pthread_mutex_unlock(&(tc->actionLockC));
    //pthread_mutex_lock(&(tc->actionLockA));
    break;
  case 'b':
    //tc->childHasThisLock = 'a';
    pthread_mutex_unlock(&(tc->actionLockA));
    //pthread_mutex_lock(&(tc->actionLockB));
    break;
  case 'c':
    //tc->childHasThisLock = 'b';
    pthread_mutex_unlock(&(tc->actionLockB));
    //pthread_mutex_lock(&(tc->actionLockC));
    break;
 } //pthread_mutex_unlock(&(tc->mainLock)); 
}

void notifyParentOfWorkCompleted(simulationManager_thread_control *tc) {
 switch (tc->parentHasThisLock) {
  case 'a':
    tc->parentHasThisLock = 'c';
    pthread_mutex_unlock(&(tc->actionLockC));
    pthread_mutex_lock(&(tc->actionLockA));
    break;
  case 'b':
    tc->parentHasThisLock = 'a';
    pthread_mutex_unlock(&(tc->actionLockA));
    pthread_mutex_lock(&(tc->actionLockB));
    break;
  case 'c':
    tc->parentHasThisLock = 'b';
    pthread_mutex_unlock(&(tc->actionLockB));
    pthread_mutex_lock(&(tc->actionLockC));
    break;
 }
}

void *threadManager_test_run(void* args) {
 //printf("Child is running\n");
 simulationManager_thread_control *tc;
 tc = (simulationManager_thread_control*)args;
 while(tc->done == 0)
 {
  tc->runAgentDecision++; 
  //printf("Child: about to wait on parent. %i\n",tc->runAgentDecision);
  notifyParentOfWorkCompleted(tc); //This will block 
  //printf("Child: done waiting on parent, about to increment\n");
 } 
}

int threadManager_test() {
 int i,j;
 int threads = 20;
 int iterations = 1000; 
 pthread_t worker[threads];
 simulationManager_thread_control tc[threads];
 for(i = 0; i < threads; i++) 
   initThread(&(tc[i]), &(worker[i]), threadManager_test_run,0);
 for(j = 0; j < iterations; j++) {
   for(i = 0; i < threads; i++)  {
    notifyChildOfWorkToDo(&(tc[i])); //This will *not* block
   }
   for(i = 0; i < threads; i++) {
    blockOnChildFinishingWork(&(tc[i]));  //This will block
   }
 }
 for(i = 0; i < threads; i++) 
   if( tc[i].runAgentDecision != iterations)
     return 0;
 return 1;
}
#endif
/*
X has\tX blocked\tY has\tY blocked\taction
c\tb\tb,a\t\tY gets a
c\tb\tb,a\t\tY does work
c\tb\ta\t\tY releases b
c\tb\ta\tc\tY gets c
c,b\t\ta\tc\tX gets b
c,b\t\ta\tc\tX does work
b\t\ta\tc\tX releases c
b\ta\ta\tc\tX gets a
b\ta\ta,c\t\tY gets c
b\ta\ta,c\t\tY does work
b\ta\tc\t\tY releases a
b\ta\tc\tb\tY gets b
b,a\t\tc\tb\tX gets a
b,a\t\tc\tb\tX does work
a\t\tc\tb\tX releases b
a\tc\tc\tb\tX gets c
a\tc\tc,b\t\tY gets b
a\tc\tc,b\t\tY does work
a\tc\tb\t\tY releases c
a\tc\tb\ta\tY gets a
a,c\t\tb\ta\tX gets c
a,c\t\tb\ta\tX does work
c\t\tb\ta\tX releases a
c\tb\tb\ta\tX gets b
c\tb\tb,a\t\tY gets a
*/
