# Some_Work_with_NachOS

## Overall Design  

I created 3 classes, Admin, Train and Request. Everything is stored in the admin class which has a global pointer, so all the threads are able to modify the admin object. The admin object keeps current time, the name of stations, five trains three lists of requests (granted, refused, onTrain).  
    
In Train class, the train id, route, fares, seats information and information about busiest time are stored. Because the seat information is stored in Train, it should help with picking seats.  
    
The Request class is just an object store the request information. When request is created, the constructor will generate the departure/destination station, departure time, class of seat and number of passengers randomly. I use thread ID_time as the ID of that request. I also store the arriving time; the train being chosen and the seats it picked in this class.  
  
For train schedule, in my simulation, each train takes 20 minutes running from one station to another, and stay for 10 minutes, which means the minimum time for train scheduling is 30 minutes.  
     
## Threads  
I create one admin thread (not main thread), 5 train threads and 5 request threads. For every simulation time the running order is admin -> request -> train.  

In admin thread it first does initialization and Fork other thread. After that it called Yield() and let other threads do their initialization. Then, there comes a loop, in that loop the admin thread will just increase the current time and call Yield() again.  

For request threads each thread runs one by one. Even actually there is no initialization in that thread, I still make some part of the code run whenever the function is called. It's just for testing. Every simulation time, this thread will create one request. There are five request threads so five threads will be created each time. After creating the request, the request will be printed, and then it will be checked if it is granted or not. The last thing in the loop is calling Yield().  

For train threads, just like requests, I also created a space that when the thread is called the code in this space will be run just for testing. There are five train threads, which means five trains. Every simulation time, this thread will check if any requests in onTrain List should be removed from the list and also get the requests in granted List into the onTrain List if it is the departure time for that request. Then it will print out the status of the train and call Yield().  

## Detail
To read the route file, there is a ReadRoute() function in Admin class. It reads "../lab3/route.txt" and set the route in each train.  

To deal with requests, I creates Deal() in Main. It will read the information of request and go through five trains to see if there is a possible choice for the request. When it sees the departure station in the schedule, it will record the seat information and continue recording until it sees the destination station. Then check if the available seats are enough. If so, this choice will be record, and after five trains are all checked, one choice will be chosen randomly, and the request will be appended to granted List. If there is no possible choice, the request will be appended to refused list.  

When the train thread is running I call TrainRun() function in Admin class every simulation time. In the function, it will first check if time is an integral multiple of thirty minutes. If not, it will just print out which stations are the train running from and to. If so, it will first create ListIterator of granted and onTrain. If any request on onTrain list has a arrive time same as the current time, that request will be removed from onTrain list. And it will check the requests in granted list, and if the request has a departure time same as the current time, the request will be appended to onTrain list.  

Also, I created many print functions to print out the result.
