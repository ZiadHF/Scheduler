#include"SJF.h"

bool SJF::RemoveProcess(int id, Process** x) {
	return false;
}
void SJF::AddtoRDY(Process* x) {
	numOfProcesses++;
	totalTime = totalTime + x->getCT();
	list.Insert(x);
}
bool SJF::FindProcessByID(int id, Process* x) {
	return false;
}
SJF::SJF() : busy(0),idle(0){
}
bool SJF::MoveToRun(int& RunningNum,int time) {
	if (!(list.IsEmpty())) {
		if (currentProcess == nullptr && !(list.PeekMin()->JustArrived(time))) {
			RunningNum++;
			currentProcess = list.getMin();
			return true;
		}
	}
	return false;
}

void SJF::IncrementBusy() { busy++; }

void SJF::IncrementIdle() { idle++; }

Process* SJF::GetRun() {
	return currentProcess;
}
void SJF::tick(Process* rem, Process* child, Process* blk) {
	//TODO
	return;
}
int SJF::getTotalTime() {
	return totalTime;
}

int SJF::getNumOfProcesses() {
	return numOfProcesses;
}
void SJF::RemoveRun() {
	numOfProcesses--;
	currentProcess = nullptr;
}
MinHeap& SJF::getlist() {
	return list;
}