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
	// OverHeating functions:
	int getTOH();
	// Setters and getters:
	float GetBusy();
	float GetIdle();
	int getTT();
	int getTotalTime();
	int getNumOfProcesses();
	Process* GetRun();
	void SetScheduler(Scheduler*);
	Process* gettopProcess();
	MinHeap& getlist();
	// Moving Processes
	bool RemoveProcess(int id, Process** x);
	bool MoveToRun(int&,int);
	void AddtoRDY(Process* x);
	void tick();
	void RemoveRun();
	// Helper Functions:
	void IncrementBusy();
	void IncrementIdle();
	bool FindProcessByID(int id, Process* x);

};
