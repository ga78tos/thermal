#include "Dispatcher.h"
#include "TimeUtil.h"
#include "Priorities.h"
#include "Enumerations.h"
#include "Semaphores.h"
#include "Scratch.h"
#include "Statistics.h"


#include <semaphore.h>
#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <errno.h>
#include <unistd.h>


#define _INFO 0
#define _DEBUG 0
using namespace std;

#if MONOTONIC == 1
	#define CLCK CLOCK_MONOTONIC
#else
	#define CLCK CLOCK_REALTIME
#endif

///Constructor needs the relative release times of the jobs
Dispatcher::Dispatcher(vector<unsigned long> _rl_arrive_times, 
	unsigned _id): TimedRunnable(_id) {
	thread_type = dispatcher;
	rl_arrive_times = _rl_arrive_times;
}	


Dispatcher::~Dispatcher(){
	// std::cout << "dispatcher with id " << id << " is being destructed\n";
}

///This is the pthread's wrapper function
void Dispatcher::wrapper(){
	#if _INFO == 1
  	Semaphores::print_sem.wait_sem();
	cout << "Dispatcher: " << id << " waiting for initialization\n";
	Semaphores::print_sem.post_sem();
  	#endif

  //Wait until the simulation is initialized
	while( !Pipeline::isInitialized() );

  	#if _INFO == 1
	Semaphores::print_sem.wait_sem();
	cout << "Dispatcher: " << id << " begining execution \n";
	Semaphores::print_sem.post_sem();
  	#endif

	
	///wait for the simulation start
	while(!Pipeline::isSimulating()){};

	///Dispatcher releases jobs to a Pipeline object
	if (pipeline == NULL){
		cout << "Dispatcher error: Pipeline is null!\n";
		return;
	}

	///Invokes parent class memember function to release jobs
	///in a timed pattern
	timedRun();

	#if _INFO == 1
	Semaphores::print_sem.wait_sem();
	cout << "Dispatcher " << id << " exiting wrapper...\n";
	Semaphores::print_sem.post_sem();
  #endif
}

///This function needs the index of 'itself'
/// index indicates this is the 'index'th time this function is called
void Dispatcher::timedJob(unsigned index){
	///get the absolute release time of the new job
	//cerr << "dispatching" << endl;
	//struct timespec start, end;
	//clock_gettime(CLCK, &start);
	unsigned long releasetime = Statistics::getRelativeTime();
	///release the 'index'th job, inform the job the current time
	jobs[index].release(releasetime);
	///pass the pointer of the new job to pipeline
	pipeline->newJob(&jobs[index], rl_arrive_times[index]);
	#if _DEBUG == 1
	Semaphores::print_sem.wait_sem();
	cout << "Dispatcher " << id << " dispatched a new job: " << index << endl;
	Semaphores::print_sem.post_sem();
	#endif
	//clock_gettime(CLCK, &end);
	//cout << "disp: " << (TimeUtil::convert_us(end) - TimeUtil::convert_us(start)) << endl;
}

void Dispatcher::setPipeline(Pipeline * p){
	pipeline = p;
	
}

void Dispatcher::createJobs(vector<unsigned long> wcets, float factor,
	unsigned long _rltDeadline){
	jobs.clear();
	///get the number of stage from Scratch class
	unsigned nstage = Scratch::getNstage();
	///create the jobs according to their releative release times
	for (unsigned i = 0; i < rl_arrive_times.size(); ++i){
		Job j = Job(nstage, i+1, _rltDeadline);
		j.setRCET(wcets, factor);
		jobs.push_back(j);
	}
}

void Dispatcher::activate(){
	setPriority(Priorities::get_disp_pr());
}

///This function needs the absolute start time of the simulation
void Dispatcher::setAbsReleaseTimes(unsigned long start_time){
	convertRelativeTimesToAbs(rl_arrive_times, start_time);
}

///No unblocking mechanism is needed
void Dispatcher::join(){
	join2();
}
