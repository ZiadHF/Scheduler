#pragma once
#include "Processor.h"
#include "Scheduler.h"

class EDF :
    public Processor
{
	float busy, idle;
	MinHeap list = MinHeap(100,false);
	Process* currentProcess = nullptr;
	int numOfProcesses = 0;
	Scheduler* s;
public:
	//Constructor
	EDF();
	//Increments
	void IncrementBusy();
	void IncrementIdle();
	//Setters and Getters
	void SetScheduler(Scheduler*);
	float GetBusy();
	float GetIdle();
	MinHeap& getlist();
	int getNumOfProcesses();
	Process* GetRun();
	//Adding, Moving and Removing
	void AddtoRDY(Process* x);
	bool MoveToRun(int&,int);
	void RemoveRun();
	//Ticking
	void tick(Process* rem, Process* ch, Process* blk);
	//Useless Functions
	int getTotalTime();
	bool FindProcessByID(int id, Process* x);
	bool RemoveProcess(int id, Process** x);
	~EDF();
};

