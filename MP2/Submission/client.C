#include <cassert>
#include <string>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>

#include <errno.h>
#include <unistd.h>

#include <sys/time.h> 
#include <time.h>  

#include "reqchannel.H"

using namespace std;

/*--------------------------------------------------------------------------*/
/* DATA STRUCTURES */ 
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* CONSTANTS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* FORWARDS */
/*--------------------------------------------------------------------------*/

    /* -- (none) -- */

/*--------------------------------------------------------------------------*/
/* LOCAL FUNCTIONS -- SUPPORT FUNCTIONS */
/*--------------------------------------------------------------------------*/

string int2string(int number) {
   stringstream ss;//create a stringstream
   ss << number;//add number to the stream
   return ss.str();//return a string with the contents of the stream
}

double time_elapsed(timeval *clock1, timeval *clock2) {
	double elapsedTime;
	elapsedTime = (clock2->tv_sec - clock1->tv_sec) * 1000.0;      // sec to ms
	elapsedTime += (clock2->tv_usec - clock1->tv_usec) / 1000.0;   		// us to ms
	
	return elapsedTime;
}

/*--------------------------------------------------------------------------*/
/* MAIN FUNCTION */
/*--------------------------------------------------------------------------*/

int main(int argc, char * argv[]) {
	
	pid_t pid = fork();

	if(pid == 0) {
		// child process
		execv("dataserver", NULL);
	}

	cout << "CLIENT STARTED:" << endl;

 	cout << "Establishing control channel... " << flush;
 	RequestChannel chan("control", RequestChannel::CLIENT_SIDE);
 	cout << "done." << endl;
 
// 	/* -- Start sending a sequence of requests */
// 
// 	string reply1 = chan.send_request("hello");
// 	cout << "Reply to request 'hello' is '" << reply1 << "'" << endl;
// 
// 	string reply2 = chan.send_request("data Joe Smith");
// 	cout << "Reply to request 'data Joe Smith' is '" << reply2 << "'" << endl;
// 
// 	string reply3 = chan.send_request("data Jane Smith");
// 	cout << "Reply to request 'data Jane Smith' is '" << reply3 << "'" << endl;
// 
// 	for(int i = 0; i < 100; i++) {
// 	string request_string("data TestPerson" + int2string(i));
// 	string reply_string = chan.send_request(request_string);
// 	cout << "reply to request " << i << ":" << reply_string << endl;;
// 	}

	timeval clock1, clock2, clock3, clock4, clock5, clock6;
	double elapsedTime1, elapsedTime2, elapsedTime3;
	double averageElapsedTime;

	gettimeofday(&clock1, NULL);
	string testReply1 = chan.send_request("data");
	gettimeofday(&clock2, NULL);
	elapsedTime1 = time_elapsed(&clock1, &clock2);
	cout << "\nElapsed time for client call 1: " << elapsedTime1 << " ms" << endl << endl;
	
	gettimeofday(&clock3, NULL);
	string testReply2 = chan.send_request("data");
	gettimeofday(&clock4, NULL);
	elapsedTime2 = time_elapsed(&clock3, &clock4);
	cout << "\nElapsed time for client call 2: " << elapsedTime2 << " ms" << endl << endl;
	
	gettimeofday(&clock5, NULL);
	string testReply3 = chan.send_request("data");
	gettimeofday(&clock6, NULL);
	elapsedTime3 = time_elapsed(&clock5, &clock6);
	cout << "\nElapsed time for client call 3: " << elapsedTime3 << " ms" << endl << endl;
	
	averageElapsedTime = (elapsedTime1 + elapsedTime2 + elapsedTime3)/3;
	cout << "Average time for client call: " << averageElapsedTime << " ms" << endl << endl;

	string reply4 = chan.send_request("quit");
	cout << "Reply to request 'quit' is '" << reply4 << endl;
	
	usleep(1000000);

}