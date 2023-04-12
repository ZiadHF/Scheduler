#include"FCFS.h"
#include <cstdlib>
#include <ctime>
FCFS::FCFS(float forkP) {
	forkProb = forkP/100;
}

void FCFS::AddtoRDY(Process* x) {
	numOfProcesses++;
	totalTime = totalTime + x->getCT();
	list.Insert(x);
}

bool FCFS::FindProcessByID(int id, Process* x) {
	return list.FindByID(id, x);
}
bool FCFS::RemoveProcess(int id,Process* x) {
	if (list.RemoveByID(id, *x)) {
		totalTime = totalTime - x->getWorkingTime();
		numOfProcesses--;
	}
}
void FCFS::tick(Process* rem, Process* child, Process* blk) {
	//Case 1: no running process.
	if (currentProcess == nullptr) {
		bool processGet = list.RemoveHead(*currentProcess);
		if (processGet) {
			currentProcess->Decrement();
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
				if (currentProcess->getChild() == nullptr) {
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
		currentProcess->Decrement();
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
			if (currentProcess->getChild() == nullptr) {
				child = currentProcess;
			}
		}
		if (currentProcess->getCT() - currentProcess->getWorkingTime() == currentProcess->getIO().R) {
			blk = currentProcess;
			currentProcess = nullptr;
		}
	}
}

int FCFS::getTotalTime() {
	return totalTime;
}
