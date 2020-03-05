#ifndef TRAIN_H
#define TRAIN_H

#include "bitmap.h"

#define B_SEAT 20
#define C_SEAT 40

class Train{
private:
	int id;
	int route[33];
	int fare[2][32]; // not needed
	Bitmap *business[33];
	Bitmap *coach[33];
	int bu, co; //business; coach
	int servedP; // served passanger
	int servedR; // served request
	int maxPass; // max number of passanger for the whole running time
	int busyTime; // busiest time that have most passanger on train
public:
	Train(int i);
	int getId();
	void setRoute(int *r);
	int* getRoute(){
		return route;
	}
	Bitmap* getSeats(bool bus, int i){
		if(bus) return business[i];
		else return coach[i];
	}
	Bitmap* getBus(int i){ return business[i]; }
	Bitmap* getCoa(int i){ return coach[i]; }
	void printRoute();	
	void TakeSeat(int start, int end, int passn, int *sTaken, bool bus);
	int ConvertTime(int t);
	int ServedP(){return servedP;}
	int ServedR(){return servedR;}
	int MaxPass(){return maxPass;}
	int BusyTime(){return busyTime;}
	void AddServe(int P, int R);
	void UpdateBusy(int mp, int bt);
};


#endif