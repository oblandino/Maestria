#include <gtk/gtk.h>
#include <stdlib.h>
#include "soa_thread.h"

//gcc Threads_Progress.c `pkg-config --cflags --libs gtk+-3.0`

typedef struct grap_st{
   int *Porcentaje,*tids;
   double *Resultado;
   mctx_t * mctx_ret;
   mctx_t * mctx_func;   
}grap_t;

GtkWidget *window, **progress, *grid, **label;
int *Porcentaje,*tids;
double *Resultado;
mctx_t * mctx_return,*mctx_function;

static gboolean update_widgets(gpointer data){

   int* n_tids = (int*)data;
   for(int i = 0;i<*n_tids;i++){
      gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress[i]), (float)Porcentaje[i]/100);   
      char c[11];
      sprintf(c, "PI = %f", Resultado[i]);
      gtk_label_set_text(GTK_LABEL(label[i]),c);
   }
   mctx_switch(mctx_function,mctx_return);
   return TRUE;
}
void clean_malloc(){
   free(progress);
   free(label);
}
void Widgets_setup(int* n_tids){
   
   window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
   g_signal_connect(window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);
   
   //Crear array de progress bars y labels
   grid = gtk_grid_new();
   progress = malloc((*n_tids) * sizeof(GtkWidget));
   label = malloc((*n_tids) * sizeof(GtkWidget));
   for(int i = 0;i<*n_tids;i++){
      GtkWidget *progress_tmp, *label_tmp;
      progress_tmp = gtk_progress_bar_new();
      progress[i] = progress_tmp;
      gtk_widget_set_size_request (progress[i],1000, -1);
      gtk_progress_bar_set_fraction(GTK_PROGRESS_BAR(progress[i]), 0);
      gtk_progress_bar_set_show_text(GTK_PROGRESS_BAR(progress[i]),TRUE);
      label_tmp = gtk_label_new("Pi = ");
      label[i] = label_tmp;
      gtk_grid_attach(GTK_GRID(grid),progress[i],0,i,1,1);
      gtk_grid_attach(GTK_GRID(grid),label[i],1,i,1,1);      
   }
   gint func_ref = g_timeout_add (1, update_widgets,(gpointer) n_tids );

   gtk_container_add(GTK_CONTAINER(window), grid); 
   gtk_widget_show_all(window);
   gtk_main();
   g_source_remove (func_ref);
   clean_malloc();
}


void graphics(void* grap_arg){
   grap_t * grap_dat= (grap_t*)grap_arg;
   tids = grap_dat -> tids;
   Porcentaje = grap_dat -> Porcentaje;
   Resultado = grap_dat -> Resultado;
   mctx_return = grap_dat -> mctx_ret;
   mctx_function = grap_dat -> mctx_func;

   int n_tids_tmp = 0;
   int *n_tids = &n_tids_tmp;   
   while(tids[*n_tids]){
         (*n_tids)++;
   }
   Widgets_setup(n_tids);
   //mctx_restore(mctx_return);
   exit(0);   
}
/*int main(int argc, char* argv[])
{
   gtk_init(&argc, &argv);   
   int Porcentaje_temp[5] = {26,56,89,99,'\0'};
   double Resultado_temp[5] = {3.1,3.14,3.141,3.1416,'\0'};
   int tids_tmp[5] = {6,1,3,5,'\0'};

   graphics(Porcentaje_temp,tids_tmp,Resultado_temp);
  
   return 0;
}*/
