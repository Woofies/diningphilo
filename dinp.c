///////////////////////////////////////////////////////////////////////////////////////////////////////////////
// dinp.c
// Author: Justin Dy
//
// Procedures:
//
// eating check if philosophers are hungry and if forks beside them are available before actually eating
// eat is to have philosopher pick up a fork and eat when hungry
// think is when philosopher finishes eating and is thinking replace forks
// philosopher is current philosopher and goes to functions take and return fork
// main initializes semaphores and create pthreads.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include <pthread.h> 
#include <semaphore.h> 
#include <stdio.h>
#include <stdlib.h>
#include <time.h> 
#include <ctype.h>
#include <math.h>

#define NUM_PHIL 5 // number of chairs and also the number of philosophers, plates, forks
#define EATING 0 // Define value when philosopher is eating
#define HUNGRY 1 // define value when philosopher is hungry
#define THINKING 2  // define value when philosopher is thinking
#define LEFT (philnum + 4) % NUM_PHIL // value for left of a philosopher
#define RIGHT (philnum + 1) % NUM_PHIL // value for right of a philosopher

int state[NUM_PHIL]; // array for the state of philosophers eating, hungry, thinking
int philnum[NUM_PHIL] = { 0, 1, 2, 3, 4 }; // number represent philosophers

sem_t n; // makes sure philosopher threads are not in the critical sections at the same time
sem_t s[NUM_PHIL]; 

///////////////////////////////////////////////////////////////////////////////
// void eating(int philnum)
// Author: Justin Dy
// Date:  10 March 2019
// Description: Check if current philosopher is hungry, and other philosophers
// on either side are not currently eating.
//
// Parameters:
// philnum	I/P	int	argument passed based on specific philosopher thread
///////////////////////////////////////////////////////////////////////////////
void eating(int philnum) 
{ 
	if (state[philnum] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING) // check if philosopher is hungry and philosophers left and right are not eating
	{ 
		state[philnum] = EATING; // change state of philosopher to eating
		sleep(2); 
		printf("Philosopher %d picks up forks %d and %d\n", philnum + 1, LEFT + 1, philnum + 1); // Print philosopher number is eating and using forks number # and number #
		printf("Philosopher %d is eating spaghetti\n", philnum + 1); // print out philosopher # is eating
		sem_post(&s[philnum]); // Used during think for hungry philosophers
	} 
} 

//////////////////////////////////////////////////////////////////////////////////////
// void eat(int philnum)
// Author: Justin Dy
// Date:  10 March 2019
// Description: Philosopher picks up fork set state to hungry and eat when conditions
// in previous function is met, or wait if not
//
// Parameters:
// philnum	I/P	int	argument passed based on specific philosopher thread
//////////////////////////////////////////////////////////////////////////////////////
void eat(int philnum) 
{ 
	/*sem_trywait sem_timedwait*/sem_wait(&n); 
	state[philnum] = HUNGRY; // Set state of philosopher to hungry
	printf("Philosopher %d is hungry\n", philnum + 1); // print philosopher # is hungry indicating when they should/need to eat
	eating(philnum); // go to eating function and if philosophers next to current are not eating then eat and print out
	sem_post(&n); 
	/*sem_trywait sem_timedwait*/sem_wait(&s[philnum]); // if cannot eat because of philosophers next to current are eating then wait until done
	sleep(rand()%5); 
} 

//////////////////////////////////////////////////////////////////////////////////////
// void think(int philnum)
// Author: Justin Dy
// Date:  10 March 2019
// Description: Philosopher puts down fork set state to thinking
//
// Parameters:
// philnum	I/P	int	argument passed based on specific philosopher thread
////////////////////////////////////////////////////////////////////////////////////// 
void think(int philnum) 
{ 

	/*sem_trywait sem_timedwait*/sem_wait(&n); 
	state[philnum] = THINKING; // Set state of philosopher to thinking
	printf("Philosopher %d putting forks %d and %d down\n", philnum + 1, LEFT + 1, philnum + 1); // print philosopher # is putting fork # and # down
	printf("Philosopher %d is thinking\n", philnum + 1); // print philosopher # is thinking
	eating(LEFT); // forks that were put down are now free for use
	eating(RIGHT); 
	sem_post(&n);
	sleep(rand()%5); 	
} 

//////////////////////////////////////////////////////////////////////////////////////
// void* philosopher(void* num)
// Author: Justin Dy
// Date:  10 March 2019
// Description: Philosophers alternately eat and then think assuming no limit to
// spaghetti supply and philosophers will pick up forks when available and eat when both
// left and right forks are available, checked in eat, replaced in think.
//
// Parameters:
// num	I/P	void*	argument passed specifying specific philosopher pthread
////////////////////////////////////////////////////////////////////////////////////// 
void* philospher(void* num) 
{ 
	while (1) 
	{ 
		int* i = num; 
		sleep(1); 
		eat(*i); 
		sleep(0); 
		think(*i); 
	} 
} 

///////////////////////////////////////////////////////////////////////////////
// void main()
// Author: Justin Dy
// Date:  10 March 2019
// Description: Initialize all semaphores and philosopher pthreads created and
// kill all 5 threads after 5 minutes
///////////////////////////////////////////////////////////////////////////////
int main() 
{ 
	int i; 
	pthread_t thread_id[NUM_PHIL];  
	sem_init(&n, 0, 1); // initialize semaphores
	for (i = 0; i < NUM_PHIL; i++) 
	{
		sem_init(&s[i], 0, 0); 
	}
	for (i = 0; i < NUM_PHIL; i++) // create 5 philosopher threads
	{ 
		pthread_create(&thread_id[i], NULL, philospher, &philnum[i]); 
		printf("Philosopher %d is thinking\n", i + 1); 
	} 
	sleep(300);
	for (i = 0; i < NUM_PHIL; i++) 
	{ 
		pthread_cancel(thread_id[i]); 
	}
		pthread_exit(NULL); 
} 
