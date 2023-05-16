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
	float busy,idle;
	int totalTime = 0; // The variable that has the count
	int totalTimeexc = 0;
	bool LQF = false;
	bool SQF = false;
	Scheduler* s = nullptr;

public:
	 FCFS(int forkP,Scheduler*);
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
	 int getTT() ;
	 void setLQF(bool state) ;
	 void setSQF(bool state) ;
	 bool getLQF() ;
	 bool getSQF() ;
	 Process* gettopProcess();
	 LinkedList<Process*>& getlist();
};
 