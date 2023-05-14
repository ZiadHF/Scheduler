#include "EDF.h"

//Constructor

EDF::EDF(Scheduler* main) { s = main; }


//Adding, Moving and Removing

void EDF::AddtoRDY(Process* p) {
	numOfProcesses++;
	totalTime +=  p->getCT();
	list.Insert(p);
}
bool EDF::MoveToRun(int& RunningNum,int time){
	if (list.IsEmpty()){
		IncrementIdle();
		return false;
	}
	else {
		IncrementBusy();
		RunningNum++;
		if (!currentProcess) {
			currentProcess = list.getMin();
			return true;
		}
		if (currentProcess->getDL() > list.PeekMin()->getDL()) {
			AddtoRDY(currentProcess);
			currentProcess = list.getMin();
			return true;
		}
		return false;
	}
}
void EDF::RemoveRun() {
	currentProcess = nullptr;
	numOfProcesses--;
}


//Ticking

void EDF::tick(){
	int tmp = s->GetSystemTime();
	int tmp2 = s->GetSTL_Time();
	MoveToRun(tmp,tmp2);
	if (currentProcess) {
		totalTime--;
		if (!currentProcess->DecrementWorkingTime()) {
			Process* rem = currentProcess;
			RemoveRun();
			s->SendToTRM(rem);
			return;
		}
		if (currentProcess->CheckIO())
			return;
		if (currentProcess->getCT() - currentProcess->getWorkingTime() == currentProcess->getIO().R) {
			Process* blk = currentProcess;
			RemoveRun();
			s->SendToBLK(blk);
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
