#ifndef ADMIN_H
#define ADMIN_H

#include "kernel.h"
#include "list.h"
#include "main.h"
#include "thread.h"
#include "train.h"
#include "request.h"

class Admin{
private:
	const char* station[20]; // station names
	int time; // current time
	Train *train[5]; // five trains
	List<Request*> granted; // list of granteds requests
	List<Request*> refused; // list of refused requests
	List<Request*> onTrain; // list of requests currently on train
public:
	Admin();
	void ReadRoute(); // read route from file
	void PrintRoute(int i); // print the route of train i
	int getTime(){ return time; }
	void Tick(){ time++; } // add 10 minutes to current time
	void Deal(Request* req); // deal with request req
	void TrainRun(int i); // this function is called when the system switches to the train thread
	void summary(); // print out the summary
};

#endif