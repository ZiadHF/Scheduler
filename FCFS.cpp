#include"FCFS.h"
#include <cstdlib>
#include <ctime>
#include<iostream>
FCFS::FCFS(float forkP) : busy(0),idle(0) {
	forkProb = forkP/100;
}

void FCFS::AddtoRDY(Process* x) {
	numOfProcesses++;
	totalTime = totalTime + x->getCT();
	list.Insert(x);
}
bool FCFS::MoveToRun(int& RunningNum,int time) {
	if (!(list.IsEmpty())) {
		if (currentProcess == nullptr && !(list.getFirst()->JustArrived(time))) {
			Process** temp = &currentProcess;
			list.RemoveHead(temp);
			RunningNum++;
			return true;
		}
	}
	return false;
}
Process* FCFS::GetRun() {
	Process* x = currentProcess;
	return x;
}

bool FCFS::FindProcessByID(int id, Process* x) {
	return list.FindByID(id, x);
}
bool FCFS::RemoveProcess(int id,Process** x) {
	if (list.RemoveByID(id, x)) {
		Process* temp = *x;
		totalTime = totalTime - temp->getWorkingTime();
		numOfProcesses--;
		return true;
	}
	return false;
}
void FCFS::tick(Process* rem, Process* child, Process* blk) {
	//Case 1: no running process.
	if (currentProcess == nullptr) {
		IncrementIdle();
		bool processGet = list.RemoveHead(&currentProcess);
		if (processGet) {
			IncrementBusy();
			currentProcess->DecrementWorkingTime();
			totalTime--;
			// Removing the process if the CT ended.
			if (currentProcess->getWorkingTime() == 0) {
				rem = currentProcess;
				return;
			}
			// Checking the forking probability.
			// Seeding the random number generator.
			std::srand(std::time(0));

			// To make the random num between 1 and 0.
			double random_num = static_cast<double>(std::rand()) / RAND_MAX;
			if (random_num <= forkProb) {
				if (currentProcess->getLChild() == nullptr) {
					child = currentProcess;
				}
				else if (currentProcess->getRChild() == nullptr) {
					child = currentProcess;
				}
			}
			if (currentProcess->getCT() - currentProcess->getWorkingTime() == currentProcess->getIO().R) {
				blk = currentProcess;
				currentProcess = nullptr;
			}
		}

	}
	//Case 2 Already one process in run 
	else {
		IncrementBusy();
		currentProcess->DecrementWorkingTime();
		totalTime--;
		// Removing the process if the CT ended.
		if (currentProcess->getWorkingTime() == 0) {
			rem = currentProcess;
			currentProcess = nullptr;
			return;
		}
		// Checking the forking probability.
		std::srand(std::time(0));

		double random_num = static_cast<double>(std::rand()) / RAND_MAX;
		if (random_num <= forkProb) {
			if (currentProcess->getLChild() == nullptr) {
				child = currentProcess;
			}
		}
		if (currentProcess->getCT() - currentProcess->getWorkingTime() == currentProcess->getIO().R) {
			blk = currentProcess;
			currentProcess = nullptr;
		}
	}
}

void FCFS::IncrementBusy() { busy++; }

void FCFS::IncrementIdle() { idle++; }


int FCFS::getTotalTime() {
	return totalTime;
}
int FCFS::getNumOfProcesses(){
	return numOfProcesses;
}
void FCFS::RemoveRun() { 
	numOfProcesses--;
	currentProcess = nullptr; }
LinkedList<Process*>& FCFS::getlist() {
	return list;
}