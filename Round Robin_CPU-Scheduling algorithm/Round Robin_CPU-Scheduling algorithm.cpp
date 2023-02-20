// CPU-Scheduling Simulation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

/************************************************************
Individual Processes with several private attributes
values returned with getter functions
altered with setter functions
calculateTime function determines TAT, WT, RT that result from the rest
************************************************************/

class Process {
private:
	int arrivalTime,
		burstTime,
		completionTime,     //Also referred to as exitTime
		turnAroundTime,
		waitingTime,
		responseTime,
		/*Internal*/
		remainder,
		startTime,
		runTime;

public:
	void setArrivalTime(int arrivalTime) {
		this->arrivalTime = arrivalTime;
	}
	void setBurstTime(int burstTime) {
		this->burstTime = burstTime;
	}
	void setCompletionTime(int completionTime) {
		this->completionTime = completionTime;
	}
	void setTurnAroundTime(int turnAroundTime) {
		this->turnAroundTime = turnAroundTime;
	}
	void setWaitingTime(int waitingTime) {
		this->waitingTime = waitingTime;
	}
	void setResponseTime(int responseTime) {
		this->responseTime = responseTime;
	}
	void setRemainder(int remainder) {
		this->remainder = remainder;
	}
	void setStartTime(int startTime) {
		this->startTime;
	}
	void setRunTime(int runTime) {
		this->runTime = runTime;
	}

	int getArrivalTime() const {
		return arrivalTime;
	}
	int getBurstTime() const {
		return burstTime;
	}
	int getCompletionTime() const {
		return completionTime;
	}
	int getTurnAroundTime() const {
		return turnAroundTime;
	}
	int getWaitingTime() const {
		return waitingTime;
	}
	int getResponseTime() const {
		return responseTime;
	}
	int getRemainder() const {
		return remainder;
	}
	int getStartTime() const {
		return startTime;
	}
	int getRunTime() const {
		return runTime;
	}

	void calculateTime() {
		turnAroundTime = completionTime - arrivalTime;
		waitingTime = turnAroundTime - burstTime;
		responseTime = startTime - arrivalTime;
	}

	Process() {
		arrivalTime = 0;
		burstTime = 0;
		completionTime = 0;
		turnAroundTime = 0;
		waitingTime = 0;
		responseTime = 0;
		remainder = 0;
		startTime = 0;
		runTime = 0;
	}
};

/******************************************************
This is where the main logic happens, typically only one object instantiated:
generate a sample set with a fixed length (create process objects, function will initialize AT and BT and assign 0 to the rest)
create a queue, determine processing order (basically a simulation)
print out all of the processes and their members in a table
*******************************************************/
class Scheduler {

private:
	/* This could be implimented later on
	   int sampleLength;
	   int quantumTime;
	   int burstTimeMax;
   */
	int returnTotalTime(vector<Process*> sample) {  //Returns the absolute totalTime (doesn't include pauses inbetween processing yet)
		int totalExecutionTime = 0;
		int smallest = sample[0]->getArrivalTime(); //Initialization with the arrivalTime of the first element of sample
		for (auto const& processes : sample) {
			totalExecutionTime += processes->getBurstTime();
			if (processes->getArrivalTime() < smallest) {
				smallest = processes->getArrivalTime();
			}
		}
		return totalExecutionTime + smallest;
	}
	int returnRandNum(unsigned int upperLimit) { //Returns a random unsigned Integer greater than 0 and smaller than upperLimit
		unsigned int randNum;
		randNum = 1 + rand() % upperLimit;
		return randNum;
	}
public:
	/*This function takes 3 parameters (int, int, Process*) to initialize the sample container (set sampleLength, for each process: set arrivalTime & burstTime) */
	void generateProcessSampleset(int sampleLength, int burstTimeMax, vector<Process> &sample) {
		for (int i = 0; i < sampleLength; i++) {
			Process* process = new Process();
			process->setArrivalTime(i);		//This should be randomized later on, too
			process->setBurstTime(returnRandNum(burstTimeMax));
			sample.push_back(*process);
		}
	}
	void deleteProcessSampleset(vector<Process*> sample) {
		for (auto const& processes : sample) {
			delete processes;
		}
	}
	void printProcessSampleset(vector<Process*> sample) {
		cout << "            " << "  AT" << "  BT" << "  CT" << "  TT" << "  WT" << "  RT" << endl;
		for (int row = 0; row < sample.size(); row++) {
			sample[row]->calculateTime();
			cout << "Process #" << left << setw(3) << row;
			cout << right << setw(4) << sample[row]->getArrivalTime();
			cout << right << setw(4) << sample[row]->getBurstTime();
			cout << right << setw(4) << sample[row]->getCompletionTime();
			cout << right << setw(4) << sample[row]->getTurnAroundTime();
			cout << right << setw(4) << sample[row]->getWaitingTime();
			cout << right << setw(4) << sample[row]->getResponseTime();
			/*For debugging purposes only, to be removed later*/
			cout << right << setw(4) << sample[row]->getRemainder();
			cout << right << setw(4) << sample[row]->getStartTime();
			cout << right << setw(4) << sample[row]->getRunTime();

			cout << endl;
		}
	}
	//Bubble sort, could be switched with another algorithm later
	void sort(vector<Process*> sample) {
		int memory = sample[0]->getArrivalTime();
		for (int i = 1; i < sample.size(); i++) {
			for (int j = 0; j < sample.size()-i; j++) {
				if (sample[j]->getArrivalTime() > sample[j+1]->getArrivalTime()) {
					memory = sample[j]->getArrivalTime();
					sample[j + 1]->setArrivalTime(sample[j]->getArrivalTime());
					sample[j]->setArrivalTime(sample[j + 1]->getArrivalTime());
				}
			}
		}
	}
	/*
	Process returnNextNewProcess(int currQueueI, int currSampleI, int time, vector<Process*>& sample, vector<Process*>& queue) {
		bool found;
		int smallest = sample.size() - 1;
		Process nextProcess;
		nextProcess = *sample[currSampleI];
		//Search algorithm could be improved in the future
		//Should return index of element of sample with the smallest arrivalTime, conditions:upperLimit=time, element not in queue inbetween currEL and end of queue + RM != 0
		/*
		for (int i = 0; i < sample.size(); i++) {
			found = false;		//Reset found to false
			if (sample[i]->getArrivalTime() <= time && sample[i]->getRemainder() != 0) {	//Make sure the process has arrived yet and is not finished
				for (int j = currQueueI + 1; j < queue.size(); j++) {
					if (queue[j] == sample[i]) {							//Make sure the process isn't already upcoming
						found = true;
					}
					else found = false;
				}
				if (!found && sample[i]->getArrivalTime() < sample[smallest]->getArrivalTime()) {
					smallest = i;
				}
				//if (find(queue.begin()+currQueueI+1, queue.end(), sample[i][0]) != queue.end())
			}
		}
		*/

		/*
		if (smallest != sample.size()) {
			nextProcess = *sample[smallest];
			nextProcess = find(sample[0].begin(), sample[0].end(), time) - sample[0].begin(); //Wrong: Should return the smallest element of sample[0], conditions:upperLimit=time, element not in queue inbetween currEL and end of queue + RM != 0
			if ((find(queue.begin(), queue.end(), nextProcess) != queue.end()) && currProcess == nextProcess ) { //Queue next process
				nextProcess = find(sample[0].begin(), sample[0].end(), time) - sample[0].begin();
				queue.push_back(nextProcess);
			}
		*/
		//return nextProcess;
		//}
		

	void createQueue(int quantumTime, vector<Process*>& sample, vector<Process*>& queue) {
		/*Initialization******************************************************************************************************/
		int time = 0;			//Current time, iterator
		int currQueueI = 0;		//Current queue index
		int currSampleI = 0;	//Index of the last new element queued (for returnNextNewProcess())
		int processingQuant = quantumTime;	//This variable is needed if the remaining burst time is smaller than the quantumTime -> shorter period, preempted earlier
		const int totalTime = returnTotalTime(sample);
		int processesNeverQueued = sample.size();	//Counter, to keep track of new Processes that haven't entered the ready queue (for returnNextNewProcess())
		sort(sample);
		queue.push_back(sample[0]);
		/*********************************************************************************************************************/

		//This program can't handle an empty ready queue in the middle of processing yet (Goal: increasing timer without anything else until process arrives)
		while (time <= totalTime) {		//Main loop, iterates over time units
			Process* currProcess = queue[currQueueI]; 
			if (currProcess->getArrivalTime() >= time) {
				if (currProcess->getBurstTime() < quantumTime) {
					processingQuant = currProcess->getBurstTime() % quantumTime;
				}
				for (int i = 0; i < processingQuant; i++) {		//Sub loop, iterates over processingQuants
					if (currProcess->getRemainder() == currProcess->getBurstTime()) {	//If process 
						currProcess->setStartTime(time);								//ST
						processesNeverQueued--;
					}

					time++;																//Increment timer

					if (currProcess->getRemainder() == 0) {
						currProcess->setCompletionTime(time);							//CT
					}
					currProcess->setRemainder(currProcess->getRemainder() - 1);   //Decrement remainder
					
					//Queue new process
					if (processesNeverQueued != 0 && sample[currSampleI]->getArrivalTime() <= time) {
						queue.push_back(sample[currSampleI]);
					}
				}
				//Requeue Process if not finished
				if (currProcess->getRemainder() != 0) {
					queue.push_back(currProcess);
				}
				currQueueI++;			//Increment Queue-Index
			}
			else time++;
		}
	}
	/*Scheduler(int sampleLength, int quantumTime, int burstTime)
	{
		this->sampleLength = sampleLength;
		this->quantumTime = quantumTime;
		this->burstTimeMax = burstTimeMax;
	}
	*/
};

int main() {
	/* Settings */
	const int sampleLength = 5;
	const int quantumTime = 3;
	const int burstTimeMax = 15;

	Scheduler s1;
	//Scheduler s1(sampleLength, quantumTime, burstTimeMax);    //Pass settings' paramaters to to the constructor to use them locally anywhere within the Scheduler class
	vector<Process*> sample;    //Container for all the current Process objects
	vector<Process*> queue;     //Actual queue
	bool found;

	srand(time(0)); //Set new seed for rand()

	s1.generateProcessSampleset(sampleLength, quantumTime, sample);
	s1.printProcessSampleset(sample);
	s1.createQueue(quantumTime, sample, queue);
	s1.printProcessSampleset(sample);
	s1.deleteProcessSampleset(sample);
}

