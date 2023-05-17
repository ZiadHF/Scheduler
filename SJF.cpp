#include"SJF.h"
// Since Removing a process isn't supported in SJF its a dummy function.
bool SJF::RemoveProcess(int id, Process** x) {
	return false;
}
// Adding to the RDY queue.
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
	// Incrementing IDLE if nothing is the RDY and the RUN.
	if (list.IsEmpty()) {
		if (!currentProcess)
			IncrementIdle();
	}
	else {
		//	Adding to RUN and doing the things needed if this is the first time the process is getting into the RUN.
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
	// Overheating Logic 
	int OverHeatRand = std::rand() % 100;
	// TOH stands for Time OverHeated. Counts the timesteps left for the overheated processor.
	// Used for checking if the processor is overheated too.
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
	// The condition that uses the overheat probability to check if the processor goes into overheat in this timestep.
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
	// SJF processing logic.
	int tmp2 = s->GetSystemTime();
	int tmp3 = s->getRunningProcess();
	MoveToRun(tmp3, tmp2);
	if (currentProcess) {
		IncrementBusy();
		// Checking if the process need to go to BLK.
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
			// Termination logic.
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

// Work Stealing.

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