#pragma once
#include"RR.h"
RR::RR(int t,Scheduler* main,int overH,int prob) : busy(0), idle(0) {
	TimeSlice = t;
	remainingticks = t;
	s = main;
	OverheatProb = prob;
	Overheat = overH;
}
// Adding to the RDY queue.
void RR::AddtoRDY(Process* x) {
	numOfProcesses++;
	totalTime += x->getWorkingTime();
	list.Enqueue(x);
}
int RR::getTOH() {
	return TOH;
}

bool RR::RemoveProcess(int id, Process** x) { return false; }

float RR::GetIdle() { return idle; }

float RR::GetBusy() { return busy; }

void RR::IncrementBusy() { busy++; }

void RR::IncrementIdle() { idle++; }

bool RR::FindProcessByID(int id, Process* x) {
	return false;
}
bool RR::MoveToRun(int& RunningNum, int time) {
	// Incrementing IDLE if nothing is the RDY and the RUN.
	if (list.IsEmpty()) {
		if (!currentProcess)
			IncrementIdle();
	}
	else {
		if (currentProcess) {
			// Conditions of Migrating the process in the RUN.
			if (currentProcess->getWorkingTime() < s->GetRTF() && s->GetSJF_NUM() > 0) {
				Process* move = currentProcess;
				
				if (s->ProcessMigration(move, false)) {
					totalTime -= currentProcess->getWorkingTime();
					RemoveRun();
				}
				
			}
		}
		//	Adding to RUN and doing the things needed if this is the first time the process is getting into the RUN.
		if (!currentProcess) {
			list.Dequeue(&currentProcess);
			if (currentProcess->getfirstTime()) {
				currentProcess->setRT(s->GetSystemTime() - currentProcess->getAT());
				currentProcess->setfirstTime(false);
			}
			remainingticks = TimeSlice;
			s->IncrementRunningProcessesSum();
			return true;
		}
	}
	return false;
}
Process* RR::GetRun() { return currentProcess; }

void RR::tick() {
	// Overheating Logic 
	int OverHeatRand = std::rand() % 100;
	// TOH stands for Time OverHeated. Counts the timesteps left for the overheated processor.
	// Used for checking if the processor is overheated too.
	if (TOH > 0) {
		TOH--;
		while (!list.IsEmpty()) {
			Process** temp = new Process*;
			Process** temp2 = temp;
			list.Dequeue(temp);
			s->SendToShortest(*temp);
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
			Process** temp = new Process*;
			Process** temp2 = temp;
			list.Dequeue(temp);
			numOfProcesses--;
			totalTime -= (*temp)->getWorkingTime();
			s->SendToShortest(*temp);
			delete temp2;
		}

		return;
	}
	// RR processing logic.
	int tmp3 = s->getRunningProcess();
	MoveToRun(tmp3, s->GetSystemTime());
	 
	if (currentProcess) {
		if (currentProcess) {
			// Conditions for migration.
			if (currentProcess->getWorkingTime() < s->GetRTF() && s->GetSJF_NUM() > 0) {
				Process* move = currentProcess;
				if (s->ProcessMigration(move, false)) {
					totalTime -= currentProcess->getWorkingTime();
					RemoveRun();
				}
				return;
			}
		}
		IncrementBusy();
		// BLK checks.
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
		remainingticks--;
		// Switching if Ticks are finished.
		if (remainingticks == 0) {
			list.Enqueue(currentProcess);
			currentProcess = nullptr;
			list.Dequeue(&currentProcess);
			remainingticks = TimeSlice;
			return;
		}
	}
}

void RR::SetScheduler(Scheduler* sc) { s = sc; }

int RR::getTotalTime() {
	return totalTime;
}

int RR::getNumOfProcesses() {
	return numOfProcesses;
}
void RR::RemoveRun() { 
	numOfProcesses--;
	s->DecrementRunningProcessesSum();
	currentProcess = nullptr; }

Queue<Process*>& RR::getlist() {
	return list;
}

//Work Stealing

int RR::getTT() {
	return totalTime;
}
Process* RR::gettopProcess() {
	if (list.IsEmpty()) {
		return nullptr;
	}
	Process* temp;
	list.Dequeue(&temp);
	totalTime -= temp->getWorkingTime();
	numOfProcesses--;
	return temp;
}
