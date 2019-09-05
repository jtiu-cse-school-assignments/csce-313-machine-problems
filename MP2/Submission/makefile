# makefile

all: dataserver simpleclient client mutex mutexGuard semaphore

reqchannel.o: reqchannel.H reqchannel.C
	g++ -c -g reqchannel.C

dataserver: dataserver.C reqchannel.o 
	g++ -o dataserver dataserver.C reqchannel.o -lpthread

simpleclient: simpleclient.C reqchannel.o
	g++ -o simpleclient simpleclient.C reqchannel.o
	
client: client.C reqchannel.o
	g++ -o client client.C reqchannel.o
	
mutex: mutex.cpp mutex.H
	g++ -c -g mutex.cpp
	
mutexGuard: mutex.H mutex_guard.H mutex_guard.cpp
	g++ -c -g mutex_guard.cpp
	
semaphore: semaphore.H semaphore.cpp
	g++ -c -g semaphore.cpp
	
clean:
	rm *.o
