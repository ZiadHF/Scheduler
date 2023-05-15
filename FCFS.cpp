#include"FCFS.h"
#include <cstdlib>
#include <ctime>
#include<iostream>
FCFS::FCFS(int forkP,Scheduler* main) : busy(0),idle(0) {
	forkProb = forkP;
	s = main;
}

void FCFS::AddtoRDY(Process* x) {
	numOfProcesses++;
	totalTime += x->getWorkingTime();
	list.Insert(x);
}
bool FCFS::MoveToRun(int& RunningNum,int time) {
	if (list.IsEmpty()) {
		if (!currentProcess)
			IncrementIdle();
	}
	else{
		if (!currentProcess) {
			Process** temp = &currentProcess;
			list.RemoveHead(temp);
			if (currentProcess->getfirstTime()) {
				currentProcess->setRT(s->GetSystemTime() - currentProcess->getAT());
				currentProcess->setfirstTime(false);
			}
			RunningNum++;
			return true;
		}
	}
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
	if (currentProcess != nullptr) {
		if (currentProcess->getID() == id) {
			totalTime -= currentProcess->getWorkingTime();
			RemoveRun();
			return true;
		}
	}
	if (list.RemoveByID(id, x)) {
		Process* temp = *x;
		totalTime -= temp->getWorkingTime();
		numOfProcesses--;
		return true;
	}
	return false;
}
void FCFS::tick() {
	//Case 1: no running process.
	int tmp2 = s->GetSystemTime();
	MoveToRun(s->RunningProcessesSum,tmp2);
	if (currentProcess) {
		IncrementBusy();
		// Removing the process if the CT ended.
		if (!currentProcess->CheckIO())
		{
			if (currentProcess->getCT() - currentProcess->getWorkingTime() == currentProcess->getIO().R) {
				Process* blk = currentProcess;
				totalTime -= currentProcess->getWorkingTime();
				RemoveRun();
				s->SendToBLK(blk);
				return;
			}
		}
		
			totalTime--;
		if (!currentProcess->DecrementWorkingTime()) {
			Process* rem = currentProcess;
			RemoveRun();
			s->SendToTRM(rem);
			return;
		}
		// Checking the forking probability.
		// Generate a random number between 0 and 100
		int randomNumber = 1 + rand() % 100;
		if (randomNumber <= forkProb)
			if (!currentProcess->getLChild() || !currentProcess->getRChild())
				s->AddForkedProcess(currentProcess);
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
	s->RunningProcessesSum--;
	currentProcess = nullptr; }

LinkedList<Process*>& FCFS::getlist() {
	return list;
}