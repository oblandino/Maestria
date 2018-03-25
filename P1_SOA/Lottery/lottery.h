#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
		//printf("Thread Tickets: %d\n", thread_tickets[i]);
		//printf("Total Tickets: %d\n", total_tickets);
	}

	winner_ticket = rand()%(total_tickets+1);
	printf("Winner ticket: %d\n", winner_ticket);
	
	for(j=0;j<thread_num;j++){
		acum += thread_tickets[j];
		//printf("Acum: %d\n", acum);
		if(acum >= winner_ticket){
			winner_thread = thread_id[j];
			//printf("Winner thread: %d\n", winner_thread);
			return winner_thread;
		}
	}

}
