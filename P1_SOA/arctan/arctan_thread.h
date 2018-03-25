#include "soa_thread.h"
#include <ucontext.h>
#include <signal.h>
#include <stdlib.h>
#include <setjmp.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>

/* Procedure to calculate 4arctan(1)=pi */

void arctan (void* mctx_return_arg) {
 int workload=100;
 double percent_halt=50.0;
 int iterations=workload*50;
 double arctan=0.0;
 double pi=0.0;
 int n=0;
 double percent_done=0.0;
 int halt_flag=0;

 mctx_t *mctx_return = (mctx_t*)mctx_return_arg;

 printf("Input workload=%i\n", workload);
 printf("Starting pi value calculation\n");

 for (n=0; n<iterations; n++) {
	if (halt_flag==1) break;
	arctan=arctan + pow((double)-1.0, (double)n) / (2.0*(double)n+1.0);
//	printf("Iteration %i value %i\n",n,2*n+1);
	percent_done = ((double)n/(double)iterations)*100.0;
//	printf("Workload percent done=%f\n",percent_done);
	if (percent_done == percent_halt) {
		halt_flag=1;
		printf("Halt flag set\n");
		}
	}
        pi=4.0*arctan;
	printf("Workload percent done=%f\n",percent_done);
        printf("Pi result=%f\n",pi);
//	return(pi,percent_done);
	mctx_restore(mctx_return);
}


int main(int argc,char *argv[]){
   int i=0;
   size_t sk_size = SIGSTKSZ;
   char sk_addr[SIGSTKSZ];
//   char *sf_arg = "a";

   mctx_t mctx_create_thread,mctx_main;
   mctx_t *sf_arg = &mctx_main;
   mctx_t *mctx_p = &mctx_create_thread;

   mctx_create(mctx_p,arctan, sf_arg,(void*)sk_addr, sk_size);
   for (i=0; i<5; i++) {
     mctx_switch(&mctx_main,&mctx_create_thread);
     printf("Back to main, thread created\n");
}
   return 0;
}
