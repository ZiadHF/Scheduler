#include"FCFS.h"
#include <cstdlib>
#include <ctime>
#include<iostream>
FCFS::FCFS(int forkP,Scheduler* main,int OverH) : busy(0),idle(0) {
	forkProb = forkP;
	s = main;
	Overheat = OverH;
	 
}

void FCFS::AddtoRDY(Process* x) {
	numOfProcesses++;
	totalTime += x->getWorkingTime();
	if (!x->getisForked())
		totalTimeexc += x->getWorkingTime();
	list.Insert(x);
}
bool FCFS::MoveToRun(int& RunningNum,int time) {
	if (list.IsEmpty()) {
		if (!currentProcess)
			IncrementIdle();
	}
	else{
		if (currentProcess) {
			if (currentProcess->getWorkingTime() > s->GetMaxW() && !currentProcess->getisForked() && s->GetRR_NUM() > 0) {
				Process* move = currentProcess;
				totalTime -= currentProcess->getWorkingTime();
				RemoveRun();
				if (s->GetRR_NUM() > 0)
					s->ProcessMigration(move, true);
			}
		}
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
int FCFS::getTOH() {
	return TOH;
}
bool FCFS::RemoveProcess(int id,Process** x) {
	if (currentProcess != nullptr) {
		if (currentProcess->getID() == id) {
			currentProcess->setisKilled(true);
			totalTime -= currentProcess->getWorkingTime();
			RemoveRun();
			return true;
		}
	}
	if (list.RemoveByID(id, x)) {
		Process* temp = *x;
		temp->setisKilled(true);
		totalTime -= temp->getWorkingTime();
		numOfProcesses--;
		return true;
	}
	return false;
}
void FCFS::tick() {
	int OverHeatRand = std::rand() % 100;
	if (TOH > 0) {
		TOH--;
		while (!list.IsEmpty()) {
			Process** temp = new Process*;
			Process** temp2 = temp;
			list.RemoveHead(temp);
			s->SendToShortest(*temp);
			numOfProcesses--;
			delete temp2;
		}
		return;
	}
	
	if (OverHeatRand <= OverheatProb) {
		TOH = Overheat;
		if (currentProcess != nullptr) {
			s->SendToShortest(currentProcess);
			numOfProcesses--;
			totalTime -= currentProcess->getWorkingTime();
			currentProcess = nullptr;
		}
		
		while (!list.IsEmpty()) {
			Process** temp = new Process *;
			Process** temp2 = temp;
			list.RemoveHead(temp);
			numOfProcesses--;
			totalTime -= (*temp)->getWorkingTime();
			s->SendToShortest(*temp);
			delete temp2;
		}
	 
			return;
	}
	 
	
	//Case 1: no running process.
	MoveToRun(s->RunningProcessesSum,s->GetSystemTime());
	if (currentProcess) {
		if (currentProcess) {
			if (currentProcess->getWorkingTime() > s->GetMaxW() && !currentProcess->getisForked() && s->GetRR_NUM() > 0) {
				Process* move = currentProcess;
				totalTime -= currentProcess->getWorkingTime();
				RemoveRun();
				s->ProcessMigration(move, true);
				return;
			}
		}
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