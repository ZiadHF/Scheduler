#include"FCFS.h"
#include <cstdlib>
#include <ctime>
#include<iostream>

//Constructor

FCFS::FCFS(int forkP,Scheduler* main,int OverH,int prob) : busy(0),idle(0) {
	forkProb = forkP;
	s = main;
	Overheat = OverH;
	OverheatProb = prob;
}

//Adding, Moving and Removing

void FCFS::AddtoRDY(Process* x) {
	numOfProcesses++;
	totalTime += x->getWorkingTime();
	if (!x->getisForked())
		totalTimeexc += x->getWorkingTime();
	list.Insert(x);
}
bool FCFS::MoveToRun(int& RunningNum,int time) {
	if (list.IsEmpty()) {
		if (!currentProcess)
			IncrementIdle();
	}
	else{
		if (currentProcess) {
			if (currentProcess->getWorkingTime() > s->GetMaxW() && !currentProcess->getisForked() && s->GetRR_NUM() > 0) {
				Process* move = currentProcess;
					if (s->ProcessMigration(move, true)) {
            if (!currentProcess->getisForked())
					totalTimeexc -= currentProcess->getWorkingTime();
						totalTime -= currentProcess->getWorkingTime();
						RemoveRun();
					}
			}
		}
		if (!currentProcess) {
			Process** temp = &currentProcess;
			list.RemoveHead(temp);
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
void FCFS::RemoveRun() {
	numOfProcesses--;
	s->DecrementRunningProcessesSum();
	currentProcess = nullptr;
}


//Ticking

void FCFS::tick() {
	//Setting Overheat Probability
	int OverHeatRand = std::rand() % 100;
	//Checks if TOH increment > 0
	if (TOH > 0) {
		TOH--;
		while (!list.IsEmpty()) {
			Process** temp = new Process*;
			Process** temp2 = temp;
			list.RemoveHead(temp);
			s->SendToShortest(*temp);
			numOfProcesses--;
			delete temp2;
		}
		return;
	}
	//Check if rand < probability
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
			list.RemoveHead(temp);
			numOfProcesses--;
			totalTime -= (*temp)->getWorkingTime();
			if ((*temp)->getisForked()) {
				s->ScheduleToShortestFCFS(*temp);
			}
			else {
				s->SendToShortest(*temp);
			}

			delete temp2;
		}

		return;
	}
	int tmp3 = s->getRunningProcess();
	MoveToRun(tmp3, s->GetSystemTime());
	if (currentProcess) {
		//Process Migration
		if (currentProcess) {
			if (currentProcess->getWorkingTime() > s->GetMaxW() && !currentProcess->getisForked() && s->GetRR_NUM() > 0) {
				Process* move = currentProcess;
				if (s->ProcessMigration(move, true)) {
					totalTime -= currentProcess->getWorkingTime();
					if (!currentProcess->getisForked())
						totalTimeexc -= currentProcess->getWorkingTime();
					RemoveRun();
				}

				return;
			}
		}
		//Increments Busy
		IncrementBusy();
		//Checks if it goes in BLK
		if (!currentProcess->CheckIO())
		{
			if (currentProcess->getCT() - currentProcess->getWorkingTime() == currentProcess->getIO().R) {
				Process* blk = currentProcess;
				totalTime -= currentProcess->getWorkingTime();
				if (!currentProcess->getisForked())
					totalTimeexc -= currentProcess->getWorkingTime();
				RemoveRun();
				s->SendToBLK(blk);
				return;
			}
		}
		totalTime--;
		//Checks if process if forked and decrements totaltime excluding forked processes
		if (!currentProcess->getisForked())
			totalTimeexc--;
		//Decerements Working Time
		if (!currentProcess->DecrementWorkingTime()) {
			Process* rem = currentProcess;
			RemoveRun();
			s->SendToTRM(rem);
			return;
		}
		// Checking the forking probability.
		// Generate a random number between 0 and 100
		int randomNumber = 1 + rand() % 100;
		if (randomNumber <= forkProb)
			if (!currentProcess->getLChild() || !currentProcess->getRChild())
				s->AddForkedProcess(currentProcess);
	}
}

//Increments

void FCFS::IncrementBusy() { busy++; }
void FCFS::IncrementIdle() { idle++; }

// Setters and Getters

void FCFS::SetScheduler(Scheduler* sc) { s = sc; }
int FCFS::getTOH() { return TOH; }
float FCFS::GetBusy() { return busy; }
float FCFS::GetIdle() { return idle; }
int FCFS::getTotalTime() { return totalTime; }
int FCFS::getTT() { return totalTimeexc; }
int FCFS::getNumOfProcesses() { return numOfProcesses; }
LinkedList<Process*>& FCFS::getlist() { return list; }
Process* FCFS::GetRun() { return currentProcess; }
Process* FCFS::gettopProcess() {
	if (list.IsEmpty()) {
		return nullptr;
	}
	Process* x = nullptr;
	list.getNextNONforked(&x);
	if (!x)
		return nullptr;
	totalTime -= x->getWorkingTime();
	return x;
}


//Finding and Removing

bool FCFS::FindProcessByID(int id, Process* x) {
	//Finds Process by ID
	return list.FindByID(id, x);
}
bool FCFS::RemoveProcess(int id, Process** x) {
	//Checks if Process exists and deletes it
	if (currentProcess != nullptr) {
		if (currentProcess->getID() == id) {
			currentProcess->setisKilled(true);
			totalTime -= currentProcess->getWorkingTime();
			if (!currentProcess->getisForked())
				totalTimeexc -= currentProcess->getWorkingTime();
			RemoveRun();
			return true;
		}
	}
	if (list.RemoveByID(id, x)) {
		Process* temp = *x;
		temp->setisKilled(true);

		totalTime -= temp->getWorkingTime();
		if (!temp->getisForked())
			totalTimeexc -= temp->getWorkingTime();
		numOfProcesses--;
		return true;
	}
	return false;
}
