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
SJF::SJF(Scheduler* main,int OverH,int prob) : busy(0),idle(0){
	s = main;
	Overheat = OverH;
	OverheatProb = prob;
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
			s->IncrementRunningProcessesSum();
			return true;
		}
	}
	return false;
}

float SJF::GetIdle() { return idle; }

float SJF::GetBusy() { return busy; }

void SJF::IncrementBusy() { busy++; }

void SJF::IncrementIdle() { idle++; }
int SJF::getTOH() { return TOH; }
Process* SJF::GetRun() { return currentProcess; }

void SJF::tick() {
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
int SJF::getTotalTime() {
	return totalTime;
}

void SJF::SetScheduler(Scheduler* sc) { s = sc; }

int SJF::getNumOfProcesses() { return numOfProcesses; }

void SJF::RemoveRun() {
	numOfProcesses--;
	s->DecrementRunningProcessesSum();
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