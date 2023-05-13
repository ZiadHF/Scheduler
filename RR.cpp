#pragma once
#include"RR.h"
RR::RR(int t) : busy(0), idle(0) {
	TimeSlice = t;
	remainingticks = t;
}

void RR::AddtoRDY(Process* x) {
	numOfProcesses++;
	totalTime = totalTime + x->getCT();
	list.Enqueue(x);
}

bool RR::RemoveProcess(int id, Process** x) {
	return false;

}

float RR::GetIdle() { return idle; }

float RR::GetBusy() { return busy; }

void RR::IncrementBusy() { busy++; }

void RR::IncrementIdle() { idle++; }

bool RR::FindProcessByID(int id, Process* x) {
	return false;
}
bool RR::MoveToRun(int& RunningNum,int time) {
	if (!(list.IsEmpty())) {
		if (currentProcess == nullptr && !(list.Peek()->JustArrived(time))) {
			list.Dequeue(&currentProcess);
			RunningNum++;
			return true;
		}
	}
	return false;
}
Process* RR::GetRun() {
	Process* x = currentProcess;
	return x;
 }
void RR::tick(Process* rem, Process* child, Process* blk) {
	//Case 1: no running process.
	if (currentProcess == nullptr) {
		IncrementIdle();
		remainingticks = TimeSlice;
		bool processGet = list.Dequeue(&currentProcess);
		if (processGet) {
			IncrementBusy();
			currentProcess->DecrementWorkingTime();
			remainingticks--;
			totalTime--;
			if (remainingticks == 0) {
				Process* x;
				list.Dequeue(&x);
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
		IncrementBusy();
		remainingticks--;
		totalTime--;
		if (remainingticks == 0) {
			Process* x;
			list.Dequeue(&x);
			list.Enqueue(x);
			currentProcess = nullptr;
			return;
		}
		// Removing the process if the CT ended.
		if (currentProcess->getWorkingTime() == 0) {
			rem = currentProcess;
			s->SendToTRM(rem);
			currentProcess = nullptr;
			return;
		}
		 
		if (currentProcess->getCT() - currentProcess->getWorkingTime() == currentProcess->getIO().R) {
			blk = currentProcess;
			s->SendToBLK(blk);
			currentProcess = nullptr;
		}
	}
}

void RR::SetScheduler(Scheduler* sc) { s = sc; }

int RR::getTotalTime() {
	return totalTime;
}

int RR::getNumOfProcesses() {
	return numOfProcesses;
}
void RR::RemoveRun() { 
	numOfProcesses--;
	currentProcess = nullptr; }
Queue<Process*>& RR::getlist() {
	return list;
}
