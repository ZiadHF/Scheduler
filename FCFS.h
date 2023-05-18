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
	//Constructor
	 FCFS(int forkP,Scheduler*,int Overheat,int prob);
	 //Setters and Getters
	 void SetScheduler(Scheduler*);
	 int getTOH();
	 float GetBusy();
	 int getTT() ;
	 Process* gettopProcess();
	 LinkedList<Process*>& getlist();
	 //Adding, Moving and Removing
	 float GetIdle();
	 void AddtoRDY(Process* x);
	 void RemoveRun();
	 //Ticking
	 void tick();
	 //Finding and Removing
	 bool FindProcessByID(int id, Process* x);
	 bool RemoveProcess(int id,Process** x);
	 int getTotalTime();
	 Process* GetRun();
	 int getNumOfProcesses();
	 bool MoveToRun(int& ,int);
	 //Increments
	 void IncrementBusy();
	 void IncrementIdle();
};
 