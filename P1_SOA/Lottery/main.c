#include "lottery.h"
#include "arctan_thread.h"
#include <stdio.h>


/*void hello(void* mctx_return_arg){
   mctx_t * mctx_return =  (mctx_t*)mctx_return_arg;
   printf("Hello Wolrd \n");
   mctx_restore(mctx_return);
}

int scheduler(char *mode, int thread_num, int tickets[], int quantum, int thread_id[]){
	int winner_thread=0;
	winner_thread = lottery(thread_num, tickets, thread_id);
	printf("Winner Thread: %d\n", winner_thread);
	
	printf("MODO: %s\n", mode);

	if(strcmp(mode,"Expropiativo")==0){
		printf("Quantum: %dms\n", quantum);
	}else if(strcmp(mode,"No Expropiativo")==0){	
		printf("Porcentaje: %d%% \n",quantum);
	} 
	return winner_thread;

}*/


int main(int argc,char *argv[]){

	int thread_num = 7;
	int thread_id[] = {7,14,28,56,112,224,448}; 
	int tickets[] = {13,11,15,9,17,7,19};
	char mode[] = "No Expropiativo";
	int quantum = 15;
	int winner_thread=0;
	int i=0;
	int j=0;
	int indice=0;


	size_t sk_size = SIGSTKSZ;
	char sk_addr[thread_num][SIGSTKSZ];
	mctx_t mctx_create_thread[thread_num], mctx_main;
	mctx_t *sf_arg = &mctx_main;


	int workload[]={4,8,16,32,64,128,256};
	double percent_halt[]={10,15,20,25,30,35,40};
	double percent_done[]={0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	double pi[]={0.0,0.0,0.0,0.0,0.0,0.0,0.0};
	int item[]={0.0,0.0,0.0,0.0,0.0,0.0,0.0};

	initrand();

	arctan_t arctan_arg[thread_num];
	//mctx_t mctx_create_thread[thread_num];
	
	for(j=0;j<thread_num;j++){
		while(percent_done[j] <100){
			printf("-------------------------------------------------------------\n");
			//winner_thread = scheduler(mode, thread_num, tickets, quantum, thread_id);
			winner_thread = lottery(thread_num, tickets, thread_id);
			printf("Winner Thread: %d\n",winner_thread);

			for(i=0;i<thread_num;i++){
				if(thread_id[i]==winner_thread){
					indice = i;
					printf("Indice: %d\n", indice);
					arctan_t *arctan_args = &arctan_arg[i]; 
			
					arctan_args -> workload = &workload[i];
					arctan_args -> percent_halt = &percent_halt[i];
					arctan_args -> percent_done = &percent_done[i];
					arctan_args -> pi = &pi[i];
					arctan_args -> item = &item[i];
					arctan_args -> mctx_ret = & mctx_main;
					arctan_args -> mctx_func = & mctx_create_thread[i];
		
					mctx_t *mctx_p = &mctx_create_thread[i];
		
					//mctx_t *mctx_p = &mctx_create_thread;
					//if(percent_done[i]==0){
					mctx_create(mctx_p, arctan, (void*)arctan_args, (void*)sk_addr[i], sk_size);
					//}
					//printf("ADDRESS: %d\n",sk_addr[i]);
					mctx_switch(&mctx_main, &mctx_create_thread[i]);
					break;         
				}
			}
	
			printf ("Porcentaje Parcial: %f Thread %d\n", percent_done[indice], indice);
			//mctx_switch(&mctx_main, &mctx_create_thread[indice]);

			if(percent_done[indice]==100){
				tickets[indice]=0;
			}

		}
	}

	printf("Fin Programa \n");

}
