#pragma once
#include"Processor.h"
#include"Queue.h"
class RR :public Processor {
private:
	Queue<Process*> list;
	Process* currentProcess = nullptr;
	int numOfProcesses = 0;
	int totalTime = 0; // The variable that has the count
	int remainingticks;
	int TimeSlice;
	float busy,idle;
public:
	RR(int t);
	bool RemoveProcess(int id, Process** x);
	bool MoveToRun(int&,int); 
	Process* GetRun();
	void AddtoRDY(Process* x);
	void tick(Process* rem, Process* child, Process* blk);
	int getTotalTime();
	int getNumOfProcesses();
	bool FindProcessByID(int id, Process* x);
	void RemoveRun();
	void IncrementBusy();
	void IncrementIdle();
	Queue<Process*>& getlist();
};
