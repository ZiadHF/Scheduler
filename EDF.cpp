#include "EDF.h"

//Constructor

EDF::EDF(Scheduler* main) :busy(0),idle(0),totalTime(0) { s = main; }


//Adding, Moving and Removing

void EDF::AddtoRDY(Process* p) {
	numOfProcesses++;
	totalTime +=  p->getWorkingTime();
	list.Insert(p);
}
bool EDF::MoveToRun(int& RunningNum,int time){
	if (list.IsEmpty()){
		if (!currentProcess)
			IncrementIdle();
	}
	else {
		if (!currentProcess) {
			currentProcess = list.getMin();
			if (currentProcess->getfirstTime()) {
				currentProcess->setRT(s->GetSystemTime() - currentProcess->getAT());
				currentProcess->setfirstTime(false);
			}
			RunningNum++;
			return true;
		}
		if (currentProcess->getDL() > list.PeekMin()->getDL()) {
			list.Insert(currentProcess);
			currentProcess = nullptr;
			currentProcess = list.getMin();
			return true;
		}
	}
	return false;
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

// work stealing 
void EDF::setLQF(bool state) {
	LQF = state;
}
void EDF::setSQF(bool state) {
	SQF = state;
}
bool EDF::getLQF() {
	return LQF;
}
bool EDF::getSQF() {
	return SQF;
}
int EDF::getTT() {
	return totalTime;
}
Process* EDF::gettopProcess() {
	numOfProcesses--;
	Process* temp =list.PeekMin();
	totalTime -= temp->getWorkingTime();
	return list.getMin();
}
//Useless Functions

int EDF::getTotalTime() { return totalTime; }
bool EDF::FindProcessByID(int id, Process* x) { return true; }
bool EDF::RemoveProcess(int id, Process** x) { return true; }
EDF::~EDF() {}
