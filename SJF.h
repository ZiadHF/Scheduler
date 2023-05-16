#pragma once
#include"Processor.h"
#include"MinHeap.h"
#include "Scheduler.h"
class Scheduler;
class SJF :public Processor {
private:
	float busy,idle;
	MinHeap list = MinHeap(100,true);
	Process* currentProcess = nullptr;
	int numOfProcesses = 0;
	int totalTime = 0; // The variable that has the count
	int Overheat;
	int OverheatProb ;
	Scheduler* s = nullptr;
	int TOH = 0;
public:
	SJF(Scheduler*,int OverH,int prob);
	int getTOH();
	void SetScheduler(Scheduler*);
	bool RemoveProcess(int id, Process** x);
	bool MoveToRun(int&,int);
	Process* GetRun();
	void AddtoRDY(Process* x);
	void tick();
	int getTotalTime();
	int getNumOfProcesses();
	bool FindProcessByID(int id, Process* x);
	void RemoveRun();
	void IncrementBusy();
	void IncrementIdle();
	float GetBusy();
	float GetIdle();
	int getTT();
	Process* gettopProcess();
	MinHeap& getlist();
};
