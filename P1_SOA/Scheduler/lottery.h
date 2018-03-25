#include "parser.h"


void initrand() {
	srand(time(NULL));
}

int lottery(int thread_num, int thread_tickets[], int thread_id[]){

	int acum=0;
	int i,j;
	int total_tickets=0;
	int winner_ticket=0;
	int winner_thread=0;

	for(i=0;i<thread_num;i++){
		total_tickets += thread_tickets[i];
	}

	winner_ticket = 1+rand()%(total_tickets);
	
	for(j=0;j<thread_num;j++){
		acum += thread_tickets[j];
		if(acum >= winner_ticket){
			winner_thread = thread_id[j];
			return winner_thread;
		}
	}
   return 0;

}

void noExpropiativo(int thread_num, int thread_id[], int tickets[], int modo, int quantum, int winner_thread, int i, int indice, int workload[], int percent_halt[], int percent_done[], double pi[], int item[], int inicio, int final){

	size_t sk_size = SIGSTKSZ;
	char sk_addr[thread_num][SIGSTKSZ];
	mctx_t mctx_create_thread[thread_num], mctx_main;

	arctan_t arctan_arg[thread_num];
	grap_t grap_arg;


	while(!final){
		printf("-------------------------------------------\n");
		int total_tickets=0;
		for(i=0;i<thread_num;i++){
			total_tickets += tickets[i];
		}
				
		if(total_tickets>tickets[0]){
			winner_thread = lottery(thread_num, tickets, thread_id);
		} else {
			winner_thread = 0;
			final = 1;
		}

		printf("Thread Ejecutando: %d\n",winner_thread);

		for(i=0;i<thread_num;i++){
			if(thread_id[i]==winner_thread){
				indice = i;
				if(indice == 0){
					grap_t *grap_args = &grap_arg;
					grap_args -> Porcentaje = percent_done;
					grap_args -> Resultado = pi;
					grap_args -> tids = thread_id;
					grap_args -> mctx_ret = &mctx_main;
					grap_args -> mctx_func = &mctx_create_thread[0];
					grap_args -> quantum = 100000;
					grap_args -> modo = modo;

					if(!inicio){
						//mctx_create(&mctx_create_thread[0], graphics, (void*)grap_args, (void*)sk_addr[0], sk_size);
						inicio = 1;
					}
					//mctx_switch(&mctx_main, &mctx_create_thread[0]);
               break;
				} else {
					arctan_t *arctan_args = &arctan_arg[i]; 
				
					arctan_args -> workload = &workload[i];
					arctan_args -> percent_halt = &percent_halt[i];
					arctan_args -> percent_done = &percent_done[i];
					arctan_args -> pi = &pi[i];
					arctan_args -> item = &item[i];
					arctan_args -> mctx_ret = & mctx_main;
					arctan_args -> mctx_func = & mctx_create_thread[i];
					arctan_args -> modo = modo;

		
					mctx_t *mctx_p = &mctx_create_thread[i];
		
					mctx_create(mctx_p, arctan, (void*)arctan_args, (void*)sk_addr[i], sk_size);
					mctx_switch(&mctx_main, &mctx_create_thread[i]);
					break;
				}
			}
		}
	
		printf ("Porcentaje Final: %d%% \n", percent_done[indice]);

		if(percent_done[indice]>=100){
			tickets[indice]=0;
		}
	}
}

void expropiativo(int thread_num, int thread_id[], int tickets[], int modo, int quantum, int winner_thread, int i, int indice, int workload[], int percent_halt[], int percent_done[], double pi[], int item[], int inicio, int final){

	size_t sk_size = SIGSTKSZ;
	char sk_addr[thread_num][SIGSTKSZ];
	mctx_t mctx_create_thread[thread_num], mctx_main;

	arctan_t arctan_arg[thread_num];
	grap_t grap_arg;

	mctx_t *mctx_pmain = &mctx_main;
	mctx_t *mctx_quantum = &mctx_create_thread[0];
	//Block SIGALRM signal while inside main
	sigset_t sigs;
	sigemptyset(&sigs);
	sigaddset(&sigs,SIGALRM);   
	sigprocmask (SIG_BLOCK, &sigs, NULL);
	set_quantum(quantum,mctx_pmain,mctx_quantum);

	while(!final){
		printf("-------------------------------------------\n");
		int total_tickets=0;
		for(i=0;i<thread_num;i++){
			total_tickets += tickets[i];
		}
				
		if(total_tickets>tickets[0]){
			winner_thread = lottery(thread_num, tickets, thread_id);
		} else {
			winner_thread = 0;
			final = 1;
		}

		printf("Thread Ejecutando: %d\n",winner_thread);

		for(i=0;i<thread_num;i++){
			if(thread_id[i]==winner_thread){
				indice = i;
				if(indice == 0){
					grap_t *grap_args = &grap_arg;
					grap_args -> Porcentaje = percent_done;
					grap_args -> Resultado = pi;
					grap_args -> tids = thread_id;
					grap_args -> mctx_ret = &mctx_main;
					grap_args -> mctx_func = &mctx_create_thread[0];
					grap_args -> quantum = quantum;
					grap_args -> modo = modo;

               if(!inicio){
						//mctx_create(&mctx_create_thread[0], graphics, (void*)grap_args, (void*)sk_addr[0], sk_size);
						inicio = 1;
					}
					mctx_quantum = &mctx_create_thread[0];                
					//mctx_switch(&mctx_main, &mctx_create_thread[0]);

					break;						

				} else {
					arctan_t *arctan_args = &arctan_arg[i]; 
				
					arctan_args -> workload = &workload[i];
					arctan_args -> percent_halt = &percent_halt[i];
					arctan_args -> percent_done = &percent_done[i];
					arctan_args -> pi = &pi[i];
					arctan_args -> item = &item[i];
					arctan_args -> mctx_ret = & mctx_main;
					arctan_args -> mctx_func = & mctx_create_thread[i];
					arctan_args -> modo = modo;
			
					mctx_t *mctx_p = &mctx_create_thread[i];
		
					mctx_create(mctx_p, arctan, (void*)arctan_args, (void*)sk_addr[i], sk_size);
					mctx_quantum = &mctx_create_thread[0]; 
					mctx_switch(&mctx_main, &mctx_create_thread[i]);
					break;
				}
			}
		}
	
		printf ("Porcentaje Final: %d%% \n", percent_done[indice]);

		if(percent_done[indice]>=100){
			tickets[indice]=0;
		}
	}
}

