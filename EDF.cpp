#include "EDF.h"

//Constructor

EDF::EDF(Scheduler* main, int OverH, int prob) :busy(0), idle(0), totalTime(0) { s = main; Overheat = OverH; OverheatProb = prob; }


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
			s->IncrementRunningProcessesSum();
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
	s->DecrementRunningProcessesSum();
	numOfProcesses--;
}


//Ticking

void EDF::tick(){
	int OverHeatRand = std::rand() % 100;
	if (TOH > 0) {
		TOH--;
		while (!list.IsEmpty()) {
			Process* temp = new Process;
			Process* temp2 = temp;
			temp = list.getMin();
			s->SendToShortest(temp);
			numOfProcesses--;
			delete temp2;
		}
		return;
	}

	if (OverHeatRand < OverheatProb) {
		TOH = Overheat;
		if (currentProcess != nullptr) {
			s->DecrementRunningProcessesSum();
			s->SendToShortest(currentProcess);
			numOfProcesses--;
			totalTime -= currentProcess->getWorkingTime();
			currentProcess = nullptr;
		}

		while (!list.IsEmpty()) {
			Process* temp = new Process;
			Process* temp2 = temp;
			temp = list.getMin();
			numOfProcesses--;
			totalTime -= (temp)->getWorkingTime();
			s->SendToShortest(temp);
			delete temp2;
		}

		return;
	}
	int tmp2 = s->GetSystemTime();
	int tmp3 = s->getRunningProcess();
	MoveToRun(tmp3, tmp2);
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
int EDF::getTotalTime() { return totalTime; }
int EDF::getTT() { return totalTime; }
int EDF::getTOH() { return TOH; }

//Work Stealing

Process* EDF::gettopProcess() {
	if (list.IsEmpty()) {
		return nullptr;
	}
	numOfProcesses--;
	Process* temp = list.PeekMin();
	totalTime -= temp->getWorkingTime();
	return list.getMin();
}
//Useless Functions

bool EDF::FindProcessByID(int id, Process* x) { return true; }
bool EDF::RemoveProcess(int id, Process** x) { return true; }
EDF::~EDF() {}
