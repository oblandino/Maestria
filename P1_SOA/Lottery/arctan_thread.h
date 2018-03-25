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

typedef struct arctan_st{
        int *workload, *item;
        double *percent_halt,*pi,*percent_done;
        mctx_t * mctx_ret;
        mctx_t * mctx_func;   
}arctan_t;

int *workload, *item;
double *percent_halt,*pi,*percent_done;
mctx_t * mctx_return;
mctx_t * mctx_function;   

void arctan (void* arctan_arg) {
 arctan_t *arctan_dat=(arctan_t*)arctan_arg;
 workload = arctan_dat -> workload;
 percent_halt = arctan_dat -> percent_halt;
 pi = arctan_dat -> pi;
 percent_done = arctan_dat -> percent_done;
 item = arctan_dat -> item;
 mctx_return = arctan_dat -> mctx_ret;
 mctx_function = arctan_dat -> mctx_func;

/*
 int workload=100;
 double percent_halt=50.0;
 int iterations=workload*50;
 double arctan=0.0;
 double pi=0.0;
 double percent_done=0.0;
 int halt_flag=0;

 mctx_t *mctx_return = (mctx_t*)mctx_return_arg;
*/
 
 int n=0;
 int iterations=*workload*50;
 double arctan_result=0.0;
 int halt_flag=0;

 printf("Input workload=%i\n", *workload);
 printf("Starting pi value calculation\n");

printf("Porcentaje entrada=%f\n",*percent_done);
printf("Item entrada=%d\n",*item);

double percent_done_local=0.0;

 for (n=0; n<iterations; n++) {
	if ((percent_done_local+*percent_done) <100){
	if (halt_flag==1) {
		*pi+=4.0 * arctan_result;
		*percent_done += percent_done_local;
		*item += n;
        	printf("Workload percent done=%f\n",*percent_done);
        	printf("Partial Pi result=%.10lf\n",*pi);
        	mctx_switch(mctx_function,mctx_return);
		printf("DEBUG\n");
		printf("Porcentaje entrada=%f\n",*percent_done);

		halt_flag=0;
	}
	arctan_result=arctan_result + pow((double)-1.0, (double) (n+*item)) / (2.0*(double)(n+*item)+1.0);
	//printf("Iteration %i value %f\n",n+*item,4*arctan_result+*pi);
	percent_done_local = ((double)(n+1)/(double)iterations)*100.0;
	//printf("Workload percent done=%f\n",*percent_done);
	if (percent_done_local == *percent_halt) {
		halt_flag=1;
		printf("Halt flag set\n");
		}
	}
	}
        *pi+=4.0 * arctan_result;
	*percent_done += percent_done_local;
	printf("Workload percent done=%f\n",*percent_done);
        printf("Final Pi result=%.10lf\n",*pi);
//	return(pi,percent_done);
	mctx_switch(mctx_function,mctx_return);
}

/*
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
*/
