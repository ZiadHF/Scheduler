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
SJF::SJF() {
}
bool SJF::MoveToRun() {
	if (currentProcess == nullptr) {
		currentProcess = list.getMin();
		return true;
	}
	return false;
}

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