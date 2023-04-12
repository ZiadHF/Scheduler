#pragma once
#include"RR.h"
RR::RR(int t) {
	TimeSlice = t;
	remainingticks = t;
}

void RR::AddtoRDY(Process* x) {
	numOfProcesses++;
	totalTime = totalTime + x->getCT();
	list.Enqueue(x);
}

bool RR::RemoveProcess(int id, Process* x) {
	return false;

}
bool RR::FindProcessByID(int id, Process* x) {
	return false;
}
 
void RR::tick(Process* rem, Process* child, Process* blk) {
	//Case 1: no running process.
	if (currentProcess == nullptr) {
		remainingticks = TimeSlice;
		bool processGet = list.Dequeue(currentProcess);
		if (processGet) {
			currentProcess->DecrementWorkingTime();
			remainingticks--;
			totalTime--;
			if (remainingticks == 0) {
				Process* x;
				list.Dequeue(x);
				list.Enqueue(x);
				currentProcess = nullptr;
				return;
			}
			// Removing the process if the CT ended.
			if (currentProcess->getWorkingTime() == 0) {
				rem = currentProcess;
				return;
			}
		 
			if (currentProcess->getCT() - currentProcess->getWorkingTime() == currentProcess->getIO().R) {
				blk = currentProcess;
				currentProcess = nullptr;
			}
		}

	}
	//Case 2 Already one process in run 
	else {
		currentProcess->DecrementWorkingTime();
		remainingticks--;
		totalTime--;
		if (remainingticks == 0) {
			Process* x;
			list.Dequeue(x);
			list.Enqueue(x);
			currentProcess = nullptr;
			return;
		}
		// Removing the process if the CT ended.
		if (currentProcess->getWorkingTime() == 0) {
			rem = currentProcess;
			currentProcess = nullptr;
			return;
		}
		 
		if (currentProcess->getCT() - currentProcess->getWorkingTime() == currentProcess->getIO().R) {
			blk = currentProcess;
			currentProcess = nullptr;
		}
	}
}

int RR::getTotalTime() {
	return totalTime;
}
int RR::getNumOfProcesses() {
	return numOfProcesses;
}