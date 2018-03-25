#include "lottery.h"


arg_t arg_t_var;
int main(int argc,char *argv[]){

	gtk_init(&argc, &argv);   
	initrand();

	input_parser(&arg_t_var);   

/////////////////////////////////////Preparar Variables
	int thread_num = arg_t_var.thread_num;
	int thread_id[thread_num+2]; 
   int percent_done[thread_num+2];
	double pi[thread_num+2];
	int item[thread_num+2];
   int workload[thread_num+2];
   int *workload_p = arg_t_var.workload;
   int tickets[thread_num+2] ;
   int *tickets_p = arg_t_var.tickets;
	int percent_halt[thread_num+2];
   int *percent_halt_p = arg_t_var.percent_halt;
   for (int i=0;i<=thread_num;i++){ //asignando TID a cada Thread, para simplificar se le asigna es mismo nuermo del indice
      thread_id[i] =i;
      percent_done[i] = 0;
      pi[i] = 0;
      item[i] = 0;
      workload[i]=workload_p[i];
      tickets[i] = tickets_p[i];
	   percent_halt[i] = percent_halt_p[i];

   }
   thread_id[thread_num+1] = '\0';
   percent_done[thread_num+1] = '\0';
   pi[thread_num+1] = '\0';
   item[thread_num+1] ='\0';
	int modo = arg_t_var.modo; //Expropiativo =1, no-expropiativo =0
	int quantum = arg_t_var.quantum; //milisegundos

	int winner_thread=0;
	int i=0;
	int indice=0;
   quantum = quantum*1000;//microsegundos a milisegundo a 
	size_t sk_size = SIGSTKSZ;
	char sk_addr[thread_num][SIGSTKSZ];
	mctx_t mctx_create_thread[thread_num], mctx_main;

   int inicio = 0;
	int final = 0;
/////////////////////////////////////Preparar Variables End

	if(modo == 0){
		noExpropiativo(thread_num,thread_id,tickets,modo,quantum,winner_thread,i,indice,workload,percent_halt,percent_done,pi,item,inicio,final);
	} else {
		expropiativo(thread_num,thread_id,tickets,modo,quantum,winner_thread,i,indice,workload,percent_halt,percent_done,pi,item,inicio,final);
	}
	printf("Fin Programa \n");

}
