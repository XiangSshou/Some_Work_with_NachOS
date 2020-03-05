#include "admin.h"
#include "kernel.h"
#include "list.h"
#include "main.h"
#include "thread.h"
#include "train.h"
#include "request.h"
#include "bitmap.h"
#include <fstream>


// void RunTrain(int i){
//  printf("Create Thread Train%d\n", i);
//  kernel->currentThread->Yield();
// }

// void RunRequest(int i){
//  printf("Create Thread Request%d\n", i);
//  kernel->currentThread->Yield();
// }

Admin::Admin(){
    const char* sta[20]  = {"Syracuse", "New York", "Buffalo", "Rochester",
        "Albany", "Niagara Falls", "Ithaca", "Utica",
        "Yonkers", "Binghamton", "Poughkeepsie", "White Plains",
        "Schenectady", "Saratoga Springs", "Newburgh", "Lake George",
        "Plattsburgh", "New Rochelle", "Lake Placid", "Cooperstown"};
    for(int i = 0 ;i < 20; i++){
        station[i] = sta[i];
    }
    time = 0;
    for(int i = 0; i < 5; i++){
        train[i] = new Train(i);
    }
}

// read route from file and set the routes in each train
void
Admin::ReadRoute(){
    // reading the file using file stream
    ifstream rf;
    rf.open("../lab3/route.txt");
    // in the file it just record the station information because the time between stations is always 30 minutes
    if(rf){
        for(int i = 0; i < 5; i++){
            int r[33];
            for(int j = 0; j<33; j++){
                rf >> r[j];
                //printf("%d\n", r[j]);
            }
            train[i] -> setRoute(r);
            // you can test if this function works by using print route, but i call that function someware else.
            //train[i] -> printRoute();
            PrintRoute(i);

        }
    }
    else{
        printf("cannot read file\n");
    }
    rf.close();
}

// print the route in train#t
void Admin::PrintRoute(int t){
    int *r = train[t] -> getRoute();
    printf("\nRoute of Train%d: \n", t);
    for(int i = 0; i<33; i++){
        printf("%02d:%02d - %s\n", 6+i/2, 30*(i%2), station[r[i]]);
    }
}

// this is the function to deal with request
void Admin::Deal(Request* req){
    // get the information of the request
    int *id = req -> getId();
    int dep = req -> getDep();
    int des = req -> getDes(); 
    int dept = req -> getDept(); 
    bool bus = req -> getBus(); 
    int pas = req -> getPas();
    // it stores the possible choices 
    // at most it will have 5 choices (5 trains)
    // 0: if there is a possible choise on that train.
    // 1: departure time
    // 2: arrive time
    // 3-10: seat numbers that this request takes
    int choise[5][11];
    for(int i = 0; i<5; i++){
        choise[i][0] = 0;
    }
    // check each train if there are enough tickets
    for(int i = 0; i<5; i++){
        bool flag = false;//this flag means if the departure station is found before.
        int *route = train[i]->getRoute();
        // bool (*sl)[32] = train[i]->getSl(bus);
        Bitmap* curSeat;
        bool empty[40] = {false};
        int seatNum = bus ? B_SEAT : C_SEAT;
        // change time from 10 minutes to 30 minutes
        int T = dept/3;
        if(dept%3 != 0) T++;
        // check if there is possible choices
        for(; T < 32; T ++){
            curSeat = train[i] -> getSeats(bus, T);
            // find the departure station
            if(route[T] == dep){
                flag = true;
                choise[i][1] = T;
                // record the empty seats
                for(int s = 0; s < seatNum; s++){
                    empty[s] = !curSeat->Test(s);
                }
            }
            if(flag){
                int countE = 0;
                // if some seats are no longer empty, delete them from empty seats list
                for(int s = 0; s < seatNum; s++){
                    if(empty[s] && !curSeat->Test(s)) countE++;
                    if(empty[s] && curSeat->Test(s)) empty[s] = false;
                }
                if(countE < pas){
                    flag = false;
                }
                // if find destination station and the seats are enough
                if(flag && route[T+1] == des){
                    // record this choice and switch to another train
                    choise[i][2] = T+1;
                    choise[i][0] = 1;
                    for(int pasN = 0, seatN = 0; pasN<pas; pasN++){
                        while(!empty[seatN]) {
                            seatN++;
                        }
                        choise[i][pasN+3] = seatN;
                        seatN++;
                    }
                    break;
                }
            }
        }

    }
    // check if there is possible choice
    int choiseN = 0;
    for(int i = 0; i<5; i++){
        choiseN += choise[i][0];
    }
    if(choiseN > 0){
        int c = rand() % choiseN;
        int i;
        int sTaken[8];
        // pick a choice
        for(i = 0; i<5; i++){
            c -= choise[i][0];
            if(c < 0){
                req -> setOnTrain(i);
                req -> setTime(choise[i][1]*3, choise[i][2]*3);
                // pick seats
                for(int p = 0; p < pas; p++){
                    sTaken[p] = choise[i][p+3];
                }
                break;
            }
        }
        // append the choice to granted
        granted.Append(req);
        int uptime = req->getDept();
        int downtime = req->getArrt();
        // take seats in the seat bitmap
        train[i] -> TakeSeat(uptime, downtime, pas, sTaken, bus);
        printf("  Request Granted!  Train#%d: %s %02d:%02d ~ %s %02d:%02d  Seats: ", req->getOnTrain(), station[req->getDep()], 6 + uptime/6, 10*(uptime%6), station[req->getDes()], 6 + (downtime-1)/6, 10*((downtime-1)%6));
        // print out the taken seats
        for(int p = 0; p < pas; p++){
            printf("%s-%d ", bus?"B":"C", sTaken[p]);
        }
        printf("\n");
    }
    // no possible choice, request refused
    else{
        refused.Append(req);
        printf("  Request Refused!\n");
    }
}

void Admin::TrainRun(int i){
    // if it is the time to arrive at the station
    if(time%3 == 0){
        printf("Train#%d:\n  Arrives at %s Station\n", i, station[train[i]->getRoute()[time/3]]);
        ListIterator<Request*>* git = new ListIterator<Request*>(&granted);
        ListIterator<Request*>* oit = new ListIterator<Request*>(&onTrain);
        // find passenger(request) to get off
        while (!oit->IsDone()) {
            Request* req = oit->Item();
            if(req->getArrt() == time && req->getOnTrain() == i){
                oit->Next();
                onTrain.Remove(req);
                printf("  %d passenger(s) form Request %d-%02d get(s) off\n", req->getPas(), req->getId()[0], req->getId()[1]);
                train[i]->AddServe(req->getPas(),1);
                continue;
            }
            oit->Next();
        }
        // fing request to get on
        for (; !git->IsDone(); git->Next()) {
            Request* req = git->Item();
            if(req->getDept() == time && req->getOnTrain() == i){
                onTrain.Append(req);
                printf("  %d passenger(s) form Request %d-%02d get(s) on\n", req->getPas(), req->getId()[0], req->getId()[1]);
            }
        }
        // taken seat in different class
        int bTaken = B_SEAT-train[i]->getBus(time/3)->NumClear();
        int cTaken = C_SEAT-train[i]->getCoa(time/3)->NumClear();
        // find the busiest time
        train[i]->UpdateBusy(bTaken+cTaken,time);
        printf("  %d business seat(s) and %d coach seat(s) are taken.\n", bTaken, cTaken);
        // print out taken seat
        if(bTaken+cTaken>0){
            printf("  Taken seat(s): "); 
            for (int s = 0; s < B_SEAT; s++) {
                if (train[i]->getBus(time/3)->Test(s)) {
                    printf("B-%d ", s);
                }
            }
            for (int s = 0; s < B_SEAT; s++) {
                if (train[i]->getCoa(time/3)->Test(s)) {
                    printf("C-%d ", s);
                }
            }
            printf("\n");
        }

        delete git;
        delete oit;
    }
    // the train is running, not stop
    else{
        printf("Train#%d:\n  Driving from %s Station to %s Station\n", i, station[train[i]->getRoute()[time/3]], station[train[i]->getRoute()[time/3+1]]);
    }
    // show a picture of the train to make the train status more clear
    printf("  Picture of the Train:  ( @: Taken, -: Empty)\n");
    for(int l = 0; l < 2; l++){
        printf("    "); 
        for (int s = 0; s < B_SEAT; s+=2) {
            if (train[i]->getBus(time/3)->Test(s+l)) {
                printf("@");
            }
            else printf("-");
        }
        printf(" ");
        for (int s = 0; s < C_SEAT; s+=2) {
            if (train[i]->getCoa(time/3)->Test(s+l)) {
                printf("@");
            }
            else printf("-");
        }
        printf("\n");
    }
}

// print out summary
void Admin::summary(){
    int tgr = granted.NumInList();
    int trr = refused.NumInList();
    int tr = tgr+trr;
    printf("\n\n-------------SUMMARY-------------\n");
    printf("Total Requests: %d\n", tr);
    printf("Total Granted Requests: %d\n", tgr);
    printf("Train Operation Summary:\n");
    for(int i = 0; i<5; i++){
        printf("  Train#%d:\n", i);
        printf("    Total Served Itinerary: %d\n", train[i]->ServedR());
        printf("    Total Passengers: %d\n", train[i]->ServedP());
        printf("    Busiest Section: %02d:%02d~%02d:%02d with %d passengers\n", 6+train[i]->BusyTime()/6, 10*(train[i]->BusyTime()%6), 6+(train[i]->BusyTime()+2)/6, 10*((train[i]->BusyTime()+2)%6), train[i]->MaxPass());
    }
}