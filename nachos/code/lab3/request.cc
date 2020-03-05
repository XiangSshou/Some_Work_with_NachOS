#include "request.h"
#include <stdlib.h>

// create a request randomly
Request::Request(int i, int time){
	id[0] = i;
	id[1] = time;
	dep = rand() % 20;
	des = rand() % 20;
	while(des == dep) des = rand() % 20;
	dept = 96 - time > 0 ? rand() % (96 - time) + time : time;
	arrt = -1;
	bus = rand() % 2 == 1 ? true : false;
	pas = rand() % 8 + 1;
	onTrain = -1;
	for(int i=0; i<8; i++)seat[i]=-1;
}

void
Request::PrintReq(){
	const char* sta[20]  = {"Syracuse", "New York", "Buffalo", "Rochester",
		"Albany", "Niagara Falls", "Ithaca", "Utica",
		"Yonkers", "Binghamton", "Poughkeepsie", "White Plains",
		"Schenectady", "Saratoga Springs", "Newburgh", "Lake George",
		"Plattsburgh", "New Rochelle", "Lake Placid", "Cooperstown"};
	printf("Request %d-%02d:\n  Departure: %s  Destination: %s  Passanger: %d\n  Departure Time: %02d:%02d  Class: %s \n", id[0], id[1], sta[dep], sta[des], pas, 6+dept/6, 10*(dept%6), bus?"Business":"Coach");
}