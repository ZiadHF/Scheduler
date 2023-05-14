#pragma once
#include"RR.h"
RR::RR(int t,Scheduler* main) : busy(0), idle(0) {
	TimeSlice = t;
	remainingticks = t;
	s = main;
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
		IncrementBusy();
		if (!currentProcess) {
			list.Dequeue(&currentProcess);
			RunningNum++;
			return true;
		}
		return false;
	}
	IncrementIdle();
	return false;
}
Process* RR::GetRun() { return currentProcess; }

void RR::tick() {
	int tmp = s->GetSystemTime();
	int tmp2 = s->GetSTL_Time();
	MoveToRun(tmp, tmp2);
	//Case 2 Already one process in run 
	if (currentProcess){
		remainingticks = TimeSlice;
		remainingticks--;
		totalTime--;
		if (!currentProcess->DecrementWorkingTime()) {
			Process* rem = currentProcess;
			currentProcess = nullptr;
			s->SendToTRM(rem);
			return;
		}
		if (remainingticks == 0) {
			list.Enqueue(currentProcess);
			currentProcess = nullptr;
			list.Dequeue(&currentProcess);
			return;
		}
		if (currentProcess->CheckIO())
			return;
		if (currentProcess->getCT() - currentProcess->getWorkingTime() == currentProcess->getIO().R) {
			Process* blk = currentProcess;
			currentProcess = nullptr;
			s->SendToBLK(blk);
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
