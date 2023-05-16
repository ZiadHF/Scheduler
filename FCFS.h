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
	int forkProb;
	int Overheat;
	int OverheatProb ;
	int TOH = 0; // Time of overheating
	float busy,idle;
	int totalTime = 0; // The variable that has the count
	int totalTimeexc = 0;
	Scheduler* s = nullptr;
public:
	 FCFS(int forkP,Scheduler*,int Overheat,int prob);
	 int getTOH();
	 void SetScheduler(Scheduler*);
	 bool FindProcessByID(int id, Process* x);
	 bool RemoveProcess(int id,Process** x);
	 void AddtoRDY(Process* x);
	 void tick();
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
 