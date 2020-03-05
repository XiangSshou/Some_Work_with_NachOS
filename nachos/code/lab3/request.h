#ifndef REQ_H
#define REQ_H


#include <stdio.h>

class Request{
private:
	int id[2];
	int dep; //departure station
	int des; //destination station
	int dept; //departure time
	int arrt; //arrival time
	bool bus; //class
	int pas; //passenger
	int onTrain; //which train
	int seat[8];
public:
	Request(int i, int time);
	void PrintReq();
	int* getId(){return id;}
	int getDep(){return dep;} //departure station
	int getDes(){return des;} //destination station
	int getDept(){return dept;} //departure time
	int getArrt(){return arrt;}
	bool getBus(){return bus;} //class
	int getPas(){return pas;} //passenger
	int getOnTrain(){return onTrain;}
	void setOnTrain(int t){onTrain = t;} //which train
	void setSeat(int* seats){
		for(int i = 0; i < pas; i++)seat[i] = seats[i];
	}
	void setTime(int d, int a){
		dept = d;
		arrt = a;
	}
};

#endif