#include"FCFS.h"
#include <cstdlib>
#include <ctime>
#include<iostream>
FCFS::FCFS(float forkP,Scheduler* main) : busy(0),idle(0) {
	//forkProb = forkP/100;
	forkProb = forkP;
	s = main;
}

void FCFS::AddtoRDY(Process* x) {
	numOfProcesses++;
	totalTime += x->getCT();
	list.Insert(x);
}
bool FCFS::MoveToRun(int& RunningNum,int time) {
	if (!(list.IsEmpty())) {
		IncrementBusy();
		if (!currentProcess) {
			Process** temp = &currentProcess;
			list.RemoveHead(temp);
			RunningNum++;
			return true;
		}
		return false;
	}
	IncrementIdle();
	return false;
}
Process* FCFS::GetRun() {
	return currentProcess;
}

float FCFS::GetBusy() { return busy; }

float FCFS::GetIdle() { return idle; }

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
void FCFS::tick() {
	//Case 1: no running process.
	int tmp = s->GetSystemTime();
	int tmp2 = s->GetSTL_Time();
	MoveToRun(tmp,tmp2);
	if (currentProcess) {
		totalTime--;
		// Removing the process if the CT ended.
		if (!currentProcess->DecrementWorkingTime()) {
			Process* rem = currentProcess;
			currentProcess = nullptr;
			s->SendToTRM(rem);
			return;
		}

		// Checking the forking probability.
			// Generate a random number between 0 and 100
		int randomNumber = rand() % 101;
		if (randomNumber <= forkProb)
			if (!currentProcess->getLChild() || !currentProcess->getRChild())
				s->AddForkedProcess(currentProcess);
		if (currentProcess->getN() == 0)
			return;
		if (currentProcess->getCT() - currentProcess->getWorkingTime() == currentProcess->getIO().R) {
			Process* blk = currentProcess;
			currentProcess = nullptr;
			s->SendToBLK(blk);
		}
	}
}

void FCFS::SetScheduler(Scheduler* sc) { s = sc; }

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