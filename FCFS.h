#pragma once
#include"Processor.h"
#include "Scheduler.h"
#include"LinkedList.h"
class Scheduler;
class FCFS :public Processor {
private:
	LinkedList<Process*> list;
	Process* currentProcess = nullptr;
	int numOfProcesses = 0 ;
	float forkProb;
	float busy,idle;
	int totalTime = 0; // The variable that has the count
	Scheduler* s = nullptr;
public:
	 FCFS(float forkP,Scheduler*);
	 void SetScheduler(Scheduler*);
	 bool FindProcessByID(int id, Process* x);
	 bool RemoveProcess(int id,Process** x);
	 void AddtoRDY(Process* x);
	 void tick(Process* rem, Process* child,Process* blk);
	 int getTotalTime();
	 Process* GetRun();
	 int getNumOfProcesses();
	 bool MoveToRun(int& ,int);
	 void RemoveRun();
	 void IncrementBusy();
	 void IncrementIdle();
	 float GetBusy();
	 float GetIdle();
	 LinkedList<Process*>& getlist();
};
 