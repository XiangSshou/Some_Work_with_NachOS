#include "kernel.h"
#include "main.h"
#include "admin.h"
#include "thread.h"
#include "threadtest.h"
#include "request.h"


// Old functions, no longer used

// void
// SimpleThread(int which)
// {
//     int num;
    
//     for (num = 0; num < 20; num++) {
//         printf("*** thread %d looped %d times\n", which, num);
//         kernel->currentThread->Yield();
//     }
// }

// void
// ThreadTest()
// {
//     Thread *t = new Thread("forked thread");
//     t->Fork((VoidFunctionPtr) SimpleThread, (void *) 1);
    
//     SimpleThread(0);
// }

// globle variable admin, kernel of the simulation
Admin *admin;

// train thread function
void RunTrain(int i){
	// this part is just for testing.
	//printf("Create Thread Train%d\n", i);
	kernel->currentThread->Yield();
	while(admin -> getTime() <=96){
		admin -> TrainRun(i);
		kernel->currentThread->Yield();
	}
}

// request thread function
void RunRequest(int i){
	// this part is just for testing.
	//printf("Create Thread Request%d\n", i);
	kernel->currentThread->Yield();
	while(admin -> getTime() < 96){
		//printf("Request%d\n", i);
		Request* req = new Request(i, admin -> getTime());
		req -> PrintReq(); // print out the request
		admin->Deal(req); // deal with that request
		kernel->currentThread->Yield(); // switch to another thread
	}
}

// admin thread function
void RunAdmin(){
	// creates five request threads
	for(int i = 0; i < 5; i++){
		char tname[15];
		sprintf(tname, "Request%d", i);
		Thread *t = new Thread(tname);
		t->Fork((VoidFunctionPtr) RunRequest, (void *) i);
	}
	// printf("Running\n");
	// creates five train threads
	int routes[5][33] = {0};
	for(int i = 0; i < 5; i++){
		char tname[10];
		sprintf(tname, "Train%d", i);
		Thread *t = new Thread(tname);
		t->Fork((VoidFunctionPtr) RunTrain, (void *) i);
	}
	printf("\nFor each train, it will spend 20 minutes from one station to another, and stay at that station for 10 minutes.\n\n");
	// read route from file
	admin -> ReadRoute();
	// this yield is actually not needed but I decided to keep it, because there may be some test in other threads.
	kernel->currentThread->Yield();
	printf("\n");
	int time;
	// start the loop, each loop is one simulation time
	while(admin -> getTime() <=96){
		time = admin -> getTime();
		printf("\nCurrent Time: %02d:%02d\n", 6 + time/6, (time%6)*10);
		kernel->currentThread->Yield();
		// chage the current time.
		admin -> Tick();
	}
	// print out the summary
	admin -> summary();
	printf("Finish\n");
}


void
ThreadTest()
{
	srand(unsigned(time(0))); //random seed
	admin = new Admin();
	//create admin thread
    Thread *t = new Thread("Admin");
    t->Fork((VoidFunctionPtr) RunAdmin, (void *) NULL);
    
    kernel->currentThread->Yield();
}