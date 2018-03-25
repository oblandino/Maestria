#include "Threads_Progress.h"
#include <stdio.h>

void hello(void* mctx_return_arg){
   mctx_t * mctx_return =  (mctx_t*)mctx_return_arg;
   printf("Hello Wolrd \n");
   mctx_restore(mctx_return);
}

int main(int argc,char *argv[]){
   gtk_init(&argc, &argv);   
   int Porcentaje_temp[5] = {26,56,89,99,'\0'};
   double Resultado_temp[5] = {3.1,3.14,3.141,3.1416,'\0'};
   int tids_tmp[5] = {6,1,3,5,'\0'};
   mctx_t mctx_create_thread,mctx_main;
   mctx_t mctx_create_th[5];

   mctx_t *sf_arg = &mctx_main;   
   mctx_t *mctx_p = &mctx_create_thread;

   grap_t grap_arg;   
   grap_t *grap_args = &grap_arg;

   grap_args -> Porcentaje = Porcentaje_temp ;
   grap_args -> Resultado = Resultado_temp;
   grap_args -> tids = tids_tmp;
   grap_args -> mctx_ret = & mctx_main;
   grap_args -> mctx_func = & mctx_create_thread;


   size_t sk_size = SIGSTKSZ;
   char sk_addr[SIGSTKSZ];
   mctx_create(mctx_p,graphics, (void*)grap_args,(void*)sk_addr, sk_size);   
   while(1){
	   //Se ejecuta funcion
      mctx_switch(&mctx_main,&mctx_create_thread);
      printf("Estoy en el main\n");
   }
   return 0;
}
