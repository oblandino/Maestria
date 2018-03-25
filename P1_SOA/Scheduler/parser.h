#include "Threads_Progress.h"
typedef struct arg_st{
int modo;
int quantum;
int thread_num;
int *tickets;
int *workload;
int *percent_halt;
} arg_t;

void* input_parser (arg_t* arg_t_var ) {
   int modo_var;
   int quantum_var;
   int thread_num_var;
   char modo_str[32]="";
   int readline, line = 0;
   int index,i =0;
   char var[32]="";
   int value;

   FILE *fp;
   fp = fopen("input.txt", "r");
   if (fp == NULL){
      printf ("Error opening the file\n\n'");
      exit(EXIT_FAILURE);
   } else {
      readline = fscanf(fp, "MODO=%s\n", modo_str);
      if (strcmp(modo_str, "Expropiativo") == 0 || strcmp(modo_str, "No_Expropiativo") == 0) { 
         if (strcmp(modo_str, "Expropiativo") == 0) modo_var=1;
         if (strcmp(modo_str, "No_Expropiativo") == 0) modo_var=0;
      } else {
         printf("Modo \"%s\" Invalido\n", modo_str);
         printf("Los modos validos son \"Expropiativo\" o \"No_Expropiativo\"\n");
         exit(1);
      }
      readline = fscanf(fp, "Quantum=%d\n", &quantum_var);
      readline = fscanf(fp, "Num_threads=%d\n", &thread_num_var);
      
      arg_t_var -> modo = modo_var;
      arg_t_var -> quantum = quantum_var;
      arg_t_var -> thread_num = thread_num_var;
      int tickets_var[thread_num_var+2];
      int workload_var[thread_num_var+2];
      int percent_halt_var[thread_num_var+2];
      arg_t_var -> tickets = tickets_var;
      arg_t_var -> workload =  workload_var;
      arg_t_var -> percent_halt = percent_halt_var;

      readline = fscanf(fp, "Thread %d %s %d\n", &index, var, &value);
      while (readline != EOF){
         line++;
         for (i=1;i<=thread_num_var;i++){
            if(strcmp(var, "Num_tickets") == 0 && index==i)
               {tickets_var[i]=value;}
            if(strcmp(var, "Workload") == 0 && index==i)
	            {workload_var[i]=value;}
            if(strcmp(var, "Percent_halt") == 0 && index==i)
	         {percent_halt_var[i]=value;}
         }
         tickets_var[0]= 50;
         workload_var[0]= 0;
         percent_halt_var[0]= 0;
         tickets_var[thread_num_var+1]= '\0';
         workload_var[thread_num_var+1]= '\0';
         percent_halt_var[thread_num_var+1]= '\0';

         readline = fscanf(fp, "Thread %d %s %d\n", &index, var, &value);
      }
      fclose(fp);
   }
}
