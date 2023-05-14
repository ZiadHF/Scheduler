#pragma once
#include "Processor.h"
#include "Scheduler.h"

class EDF :
    public Processor
{
	int busy, idle,totalTime, numOfProcesses = 0;
	MinHeap list = MinHeap(100,false);
	Process* currentProcess = nullptr;
	Scheduler* s = nullptr;
public:
	//Constructor
	EDF(Scheduler*);
	//Increments
	void IncrementBusy();
	void IncrementIdle();
	//Setters and Getters
	void SetScheduler(Scheduler*);
	float GetBusy();
	float GetIdle();
	int getTotalTime();
	MinHeap& getlist();
	int getNumOfProcesses();
	Process* GetRun();
	//Adding, Moving and Removing
	void AddtoRDY(Process* x);
	bool MoveToRun(int&,int);
	void RemoveRun();
	//Ticking
	void tick();
	//Useless Functions
	bool FindProcessByID(int id, Process* x);
	bool RemoveProcess(int id, Process** x);
	~EDF();
};

