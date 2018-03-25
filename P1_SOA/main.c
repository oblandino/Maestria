#include "Threads_Progress.h"
#include <stdio.h>

void hello(void* grap_t_arg){
   grap_t * grap_t_v =  (grap_t*)grap_t_arg ;
   int modo = grap_t_v ->modo;
   mctx_t *mctx_return = grap_t_v -> mctx_ret;
   printf("Hello Wolrd \n");
   if(modo){
      while(1){
         //printf("while \n");   
      }
   }
   mctx_restore(mctx_return);
}

int main(int argc,char *argv[]){
   gtk_init(&argc, &argv);   
   int Porcentaje_temp[5] = {26,56,89,99,'\0'};
   double Resultado_temp[5] = {3.1,3.14,3.141,3.1416,'\0'};
   int tids_tmp[5] = {6,1,3,5,'\0'};
   int quantum = 100*1000;
   int modo = 0; //no expropiativo
   mctx_t mctx_create_thread[2],mctx_main;
   mctx_t *mctx_p;
   
   grap_t grap_arg,hello_arg; 

   grap_t *grap_args = &grap_arg;
   grap_args -> Porcentaje = Porcentaje_temp ;
   grap_args -> Resultado = Resultado_temp;
   grap_args -> tids = tids_tmp;
   grap_args -> mctx_ret = &mctx_main;
   grap_args -> mctx_func = & mctx_create_thread[0];
   grap_args -> quantum = quantum;
   grap_args -> modo = modo;


   grap_t *hello_args = &hello_arg;
   hello_args -> mctx_ret = & mctx_main;
   hello_args -> modo = modo;

   //hello_args -> mctx_func = & mctx_create_thread[1];

   size_t sk_size = SIGSTKSZ;
   char sk_addr[2][SIGSTKSZ];
   mctx_create(&mctx_create_thread[1],hello, (void*)hello_args,(void*)sk_addr[1], sk_size);
   mctx_create(&mctx_create_thread[0],graphics, (void*)grap_args,(void*)sk_addr[0], sk_size);
   
   mctx_t *mctx_pmain = &mctx_main;
   mctx_t *mctx_quantum = &mctx_create_thread[0];
   if(modo){
	   //Block SIGALRM signal while inside main
	   sigset_t sigs;
		sigemptyset(&sigs);
		sigaddset(&sigs,SIGALRM);   
	   sigprocmask (SIG_BLOCK, &sigs, NULL);
	   set_quantum(quantum,mctx_pmain,mctx_quantum);
   }
   //ejecucion de Threads
   int turno = 0;
   while(1){
      if(turno==0){
         printf("turno 0\n");                  
         mctx_quantum = &mctx_create_thread[0]; 
         mctx_switch(mctx_pmain,&mctx_create_thread[0]);
         turno=1;
      } else {
         printf("turno 1\n");         
         mctx_quantum = &mctx_create_thread[1];          
         mctx_switch(mctx_pmain,&mctx_create_thread[0]);
         turno=0;
      }
   }
   return 0;
}
