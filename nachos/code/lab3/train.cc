#include "kernel.h"
#include "list.h"
#include "main.h"
#include "thread.h"
#include "train.h"
#include "threadtest.h"

Train::Train(int i): id(i){
	for(int j = 0; j < 33; j++){
		route[j] = -1;
	}
	bu = B_SEAT;
	co = C_SEAT;
	for(int j = 0; j < 32; j++){
		fare[0][j] = 2;
		fare[1][j] = 1;
	}
	for(int j = 0; j < 33; j++){
		business[j] = new Bitmap(B_SEAT);
		coach[j] = new Bitmap(C_SEAT);
	}
	servedP=0;
	servedR=0;
	maxPass=0;
	busyTime=0;
}

int Train::getId(){
	return id;
}

void Train::setRoute(int *r){
	for(int i = 0; i < 33; i++){
		route[i] = r[i];
	}
}

void Train::printRoute(){
	printf("Route of Train%d: ", id);
	for(int i = 0; i<33; i++){
		printf("%d ", route[i]);
	}
	printf("\n");
}

// take (passn) seats in (sTaken) from (start) to (end) in (bus) class
void Train::TakeSeat(int start, int end, int passn, int *sTaken, bool bus){
	int s = ConvertTime(start);
	int e = ConvertTime(end);
	for(int T = s; T < e; T++){
		for(int seatNum = 0; seatNum < passn; seatNum++){
			// printf("%d\n", sTaken[seatNum]);
			if(bus) business[T] -> Mark(sTaken[seatNum]);
			else coach[T] -> Mark(sTaken[seatNum]);
		}
	}
}

// change time form 10 min to 30 min
int Train::ConvertTime(int t){
	int T = t/3;
	if(t%3 > 0)T++;
	return T;
}

// add served passengers and requests
void Train::AddServe(int P, int R){
	servedP += P;
	servedR += R;
}

// update information acroding to busiest time.
void Train::UpdateBusy(int mp, int bt){
	if(mp > maxPass){
		maxPass = mp;
		busyTime = bt;
	}
}