#include "soa_thread.h"
#include <signal.h>
#include <sys/time.h>
#include <ucontext.h>

/*struct itimerval {
    struct timeval it_interval; // next value 
    struct timeval it_value;    // current value 
};
struct timeval {
    time_t      tv_sec;         // seconds 
    suseconds_t tv_usec;        // microseconds 
};*/

mctx_t *mctx_main, *mctx_func;
struct itimerval *interval_new ;
struct itimerval interval_new_var;

//void quantum_handler (int sig_no, siginfo_t* info, void* vcontext){
void quantum_handler (int sig_no){   
   //signal (sig, catch_alarm);
   //ucntext_t *context = (ucontext_t*)vcontext;
   if(mctx_save(mctx_func) == 0){
         //write(1, "termino el quantum\n", 36);
         printf("termino el quantum\n");
         setitimer (ITIMER_REAL,interval_new,NULL );
         mctx_restore(mctx_main);
   }
   //mctx_restore(mctx_main);
   

}
void set_quantum(int quantum,mctx_t *mctx_qmain_arg,mctx_t *mctx_qfunc_arg){
   mctx_main = mctx_qmain_arg; 
   mctx_func = mctx_qfunc_arg;
   struct timeval new_time,zero_time;
   zero_time.tv_sec = 0;
   zero_time.tv_usec = 0;
   new_time.tv_usec = quantum;
   interval_new = &interval_new_var;  

   interval_new->it_interval = zero_time;
   interval_new->it_value = new_time;

//Adding new handler for SIGALRM
   struct sigaction sa;
   memset((void *)&sa, 0,sizeof(struct sigaction));
   //sa.sa_flags = SA_RESTART | SA_SIGINFO;
   //sa.sa_sigaction = quantum_handler;
   sa.sa_handler = quantum_handler;   
	sa.sa_flags = SA_ONSTACK;
	sigemptyset(&sa.sa_mask);

   if(sigaction(SIGALRM,&sa,NULL) == -1){
      write(2,"Error no se pudo usar quantum_handler para SIGALRM\n",100);
   }
   setitimer (ITIMER_REAL,interval_new,NULL );

}
