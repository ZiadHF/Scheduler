#include "EDF.h"

//Constructor

EDF::EDF(Scheduler* main) { s = main; }


//Adding, Moving and Removing

void EDF::AddtoRDY(Process* p) {
	numOfProcesses++;
	totalTime +=  p->getWorkingTime();
	list.Insert(p);
}
bool EDF::MoveToRun(int& RunningNum,int time){
	if (list.IsEmpty()){
		IncrementIdle();
		return false;
	}
	else {
		if (!currentProcess) {
			currentProcess = list.getMin();
			RunningNum++;
			return true;
		}
		if (currentProcess->getDL() > list.PeekMin()->getDL()) {
			list.Insert(currentProcess);
			currentProcess = nullptr;
			currentProcess = list.getMin();
			return true;
		}
		return false;
	}
}
void EDF::RemoveRun() {
	currentProcess = nullptr;
	s->RunningProcessesSum--;
	numOfProcesses--;
}


//Ticking

void EDF::tick(){
	int tmp2 = s->GetSystemTime();
	MoveToRun(s->RunningProcessesSum,tmp2);
	if (currentProcess) {
		IncrementBusy();
		if (!currentProcess->CheckIO()) {
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
	}
}


//Increments

void EDF::IncrementBusy() { busy++; }
void EDF::IncrementIdle() { idle++; }


//Setters and Getters

void EDF::SetScheduler(Scheduler* sc) { s = sc; }
Process* EDF::GetRun() { return currentProcess; }
int EDF::getNumOfProcesses() { return numOfProcesses; }
float EDF::GetBusy() { return busy; }
float EDF::GetIdle() { return idle; }
MinHeap& EDF::getlist() { return list; }


//Useless Functions

int EDF::getTotalTime() { return totalTime; }
bool EDF::FindProcessByID(int id, Process* x) { return true; }
bool EDF::RemoveProcess(int id, Process** x) { return true; }
EDF::~EDF() {}
