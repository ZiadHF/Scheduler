#pragma once
#include"RR.h"
RR::RR(int t,Scheduler* main) : busy(0), idle(0) {
	TimeSlice = t;
	remainingticks = t;
	s = main;
}

void RR::AddtoRDY(Process* x) {
	numOfProcesses++;
	totalTime += x->getWorkingTime();
	list.Enqueue(x);
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
	if (list.IsEmpty()) {
		if (!currentProcess)
			IncrementIdle();
	}
	else {
		if (currentProcess) {
			if (currentProcess->getWorkingTime() < s->GetRTF() && s->GetSJF_NUM() > 0) {
				Process* move = currentProcess;
				totalTime -= currentProcess->getWorkingTime();
				RemoveRun();
				s->ProcessMigration(move, false);
			}
		}
		if (!currentProcess) {
			list.Dequeue(&currentProcess);
			if (currentProcess->getfirstTime()) {
				currentProcess->setRT(s->GetSystemTime() - currentProcess->getAT());
				currentProcess->setfirstTime(false);
			}
			remainingticks = TimeSlice;
			RunningNum++;
			return true;
		}
	}
	return false;
}
Process* RR::GetRun() { return currentProcess; }

void RR::tick() {
	int tmp2 = s->GetSystemTime();
	MoveToRun(s->RunningProcessesSum, tmp2);
	//Case 2 Already one process in run 
	if (currentProcess) {
		if (currentProcess) {
			if (currentProcess->getWorkingTime() < s->GetRTF() && s->GetSJF_NUM() > 0) {
				Process* move = currentProcess;
				totalTime -= currentProcess->getWorkingTime();
				RemoveRun();
				s->ProcessMigration(move, false);
				return;
			}
		}
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
		remainingticks--;
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
	s->RunningProcessesSum--;
	currentProcess = nullptr; }
void RR::setLQF(bool state) {
	LQF = state;
}
void RR::setSQF(bool state) {
	SQF = state;
}
bool RR::getLQF() {
	return LQF;
}
bool RR::getSQF() {
	return SQF;
}
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
Queue<Process*>& RR::getlist() {
	return list;
}
