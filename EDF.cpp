#include "EDF.h"

//Constructor

EDF::EDF(): idle(0), busy(0) {}


//Adding, Moving and Removing

void EDF::AddtoRDY(Process* p) {
	numOfProcesses++;
	list.Insert(p);
}
bool EDF::MoveToRun(int& c,int y){
	if (list.IsEmpty()){
		idle++;
		return false;
	}
	else {
		busy++;
		if (!currentProcess) {
			currentProcess = list.getMin();
			return true;
		}
		if (currentProcess->getDL() > list.PeekMin()->getDL()) {
			AddtoRDY(currentProcess);
			currentProcess = list.getMin();
			return true;
		}
	}
}
void EDF::RemoveRun() {
	currentProcess = nullptr;
	numOfProcesses--;
}


//Ticking

void EDF::tick(Process* rem,Process* ch, Process* blk){
	int tmp, tmp2 = 0;
	MoveToRun(tmp,tmp2);
	if (currentProcess) {
		if (!currentProcess->DecrementWorkingTime()) {
			rem = currentProcess;
			RemoveRun();
			s->SendToTRM(rem);
		}
		if (currentProcess->getCT() - currentProcess->getWorkingTime() == currentProcess->getIO().R) {
			blk = currentProcess;
			RemoveRun();
			s->SendToBLK(blk);
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

int EDF::getTotalTime() { return 0; }
bool EDF::FindProcessByID(int id, Process* x) { return true; }
bool EDF::RemoveProcess(int id, Process** x) { return true; }
EDF::~EDF() {}
