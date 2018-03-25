#include "Threads_Progress.h"
#include "parser.h"


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
   gtk_init(&argc, &argv);   
//	int thread_num = 5;
	int thread_id[] = {0,14,28,56,112,'\0'}; 
//	int tickets[] = {0,11,15,9,17,'\0'};
//	modo = 0; //Expropiativo =1, no-expropiativo =0
//	int quantum = 2; //milisegundos
	int winner_thread=0;
	int i=0;
	int indice=0;

   quantum = quantum*1000;//microsegundos a milisegundo a 
	size_t sk_size = SIGSTKSZ;
	char sk_addr[thread_num][SIGSTKSZ];
	mctx_t mctx_create_thread[thread_num], mctx_main;

//	int workload[]={0,8,16,32,64,'\0'};
//	int percent_halt[]={50,50,50,50,50,'\0'};

	int percent_done[]={0,0,0,0,0,0,0,'\0'};
	double pi[]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,'\0'};
	int item[]={0,0,0,0,0,0,0,'\0'};

	initrand();


	int index=0;
	input_parser(argv[1]);
	printf("Num_threads from file=%d\n", thread_num);
        printf("Quantum from file=%d\n", quantum);
	printf("Modo from file=%d\n", modo);

                for (index=0;index<thread_num;index++)
                {
                printf("Num_tickets thread[%d]=%d\n", index, tickets[index]);
                printf("Workload thread[%d]=%d\n", index, workload[index]);
                printf("Percent_halt thread[%d]=%d\n", index, percent_halt[index]);
        	}


	arctan_t arctan_arg[thread_num];
	grap_t grap_arg;
	int inicio = 0;
	int final = 0;
	//mctx_t mctx_create_thread[thread_num];

	if(modo == 0){
		while(!final){
			printf("-------------------------------------------\n");
			//winner_thread = scheduler(mode, thread_num, tickets, quantum, thread_id);
			int total_tickets=0;
			for(i=0;i<thread_num;i++){
				total_tickets += tickets[i];
				//printf("Total Tickets: %d\n", total_tickets);
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
						printf("Graficos\n");
						grap_t *grap_args = &grap_arg;
						grap_args -> Porcentaje = percent_done;
						grap_args -> Resultado = pi;
						grap_args -> tids = thread_id;
						grap_args -> mctx_ret = &mctx_main;
						grap_args -> mctx_func = &mctx_create_thread[0];
						grap_args -> quantum = 100000;
						grap_args -> modo = modo;

						if(!inicio){
							//mctx_create(mctx_p, graphics, (void*)grap_args, (void*)sk_addr[0], sk_size);
							inicio = 1;
						}
						//mctx_switch(&mctx_main, &mctx_create_thread[0]);
				                break;						

					} else {
						printf("Posicion: %d\n", indice);
						arctan_t *arctan_args = &arctan_arg[i]; 
				
						arctan_args -> workload = &workload[i];
						arctan_args -> percent_halt = &percent_halt[i];
						arctan_args -> percent_done = &percent_done[i];
						arctan_args -> pi = &pi[i];
						arctan_args -> item = &item[i];
						arctan_args -> mctx_ret = & mctx_main;
						arctan_args -> mctx_func = & mctx_create_thread[i];
						arctan_args -> modo = modo;

						printf("Workload=%d\n",*arctan_args -> workload);
						printf("Percent_halt=%d\n",*arctan_args -> percent_halt);
						printf("Modo=%d\n",arctan_args -> modo);

						mctx_t *mctx_p = &mctx_create_thread[i];

						printf("Before arctan_create\n");
						//mctx_t *mctx_p = &mctx_create_thread;
						//if(percent_done[i]==0){
						mctx_create(mctx_p, arctan, (void*)arctan_args, (void*)sk_addr[i], sk_size);
						printf("Before arctan_switch\n");
						//}
						//printf("ADDRESS: %d\n",sk_addr[i]);
						mctx_switch(&mctx_main, &mctx_create_thread[i]);
						break;
					}
				}
			}
	
			printf ("Porcentaje Final: %d%% \n", percent_done[indice]);
			//mctx_switch(&mctx_main, &mctx_create_thread[indice]);

			if(percent_done[indice]>=100){
				tickets[indice]=0;
			}
		}
	} else {
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
			//winner_thread = scheduler(mode, thread_num, tickets, quantum, thread_id);
			int total_tickets=0;
			for(i=0;i<thread_num;i++){
				total_tickets += tickets[i];
				//printf("Total Tickets: %d\n", total_tickets);
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
						//printf("Graficos\n");
						grap_t *grap_args = &grap_arg;
						grap_args -> Porcentaje = percent_done;
						grap_args -> Resultado = pi;
						grap_args -> tids = thread_id;
						grap_args -> mctx_ret = &mctx_main;
						grap_args -> mctx_func = &mctx_create_thread[0];
						grap_args -> quantum = quantum;
						grap_args -> modo = modo;

						if(!inicio){
							//mctx_create(mctx_p, graphics, (void*)grap_args, (void*)sk_addr[0], sk_size);
							inicio = 1;
						}
                  mctx_quantum = &mctx_create_thread[0]; 

						//mctx_switch(&mctx_main, &mctx_create_thread[0]);
				      break;						

					} else {
						//printf("Posicion: %d\n", indice);
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
		
						//mctx_t *mctx_p = &mctx_create_thread;
						//if(percent_done[i]==0){
						mctx_create(mctx_p, arctan, (void*)arctan_args, (void*)sk_addr[i], sk_size);
						//}
						//printf("ADDRESS: %d\n",sk_addr[i]);
                  mctx_quantum = &mctx_create_thread[0]; 
						mctx_switch(&mctx_main, &mctx_create_thread[i]);
						break;
					}
				}
			}
	
			printf ("Porcentaje Final: %d%% \n", percent_done[indice]);
			//mctx_switch(&mctx_main, &mctx_create_thread[indice]);

			if(percent_done[indice]>=100){
				tickets[indice]=0;
			}
		}
 
   }// modo expropiativo
	printf("Fin Programa \n");

}
