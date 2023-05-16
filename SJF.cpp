#include"SJF.h"

bool SJF::RemoveProcess(int id, Process** x) {
	return false;
}
void SJF::AddtoRDY(Process* x) {
	numOfProcesses++;
	totalTime += x->getWorkingTime();
	list.Insert(x);
}
bool SJF::FindProcessByID(int id, Process* x) {
	return false;
}
SJF::SJF(Scheduler* main) : busy(0),idle(0){
	s = main;
}
bool SJF::MoveToRun(int& RunningNum,int time) {
	if (list.IsEmpty()) {
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
	}
	return false;
}

float SJF::GetIdle() { return idle; }

float SJF::GetBusy() { return busy; }

void SJF::IncrementBusy() { busy++; }

void SJF::IncrementIdle() { idle++; }

Process* SJF::GetRun() { return currentProcess; }

void SJF::tick() {
	int tmp2 = s->GetSystemTime();
	MoveToRun(s->RunningProcessesSum, tmp2);
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
int SJF::getTotalTime() {
	return totalTime;
}

void SJF::SetScheduler(Scheduler* sc) { s = sc; }

int SJF::getNumOfProcesses() { return numOfProcesses; }

void SJF::RemoveRun() {
	numOfProcesses--;
	s->RunningProcessesSum--;
	currentProcess = nullptr;
}

MinHeap& SJF::getlist() {
	return list;
}

//Work Stealing

int SJF::getTT() {
	return totalTime;
}
Process* SJF::gettopProcess() {
	if (list.IsEmpty()) {
		return nullptr;
	}
	Process* temp = list.PeekMin();
	numOfProcesses--;
	totalTime -= temp->getWorkingTime();
	return list.getMin();
}