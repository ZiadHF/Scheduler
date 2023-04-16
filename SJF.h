#pragma once
#include"Processor.h"
#include"MinHeap.h"
class FCFS :public Processor {
private:
	MinHeap list;
	Process* currentProcess = nullptr;
	int numOfProcesses = 0;
	float forkProb;
	int totalTime = 0; // The variable that has the count
public:
	FCFS(float forkP);
	bool FindProcessByID(int id, Process* x);
	bool RemoveProcess(int id, Process* x);
	void AddtoRDY(Process* x);
	void tick(Process* rem, Process* child, Process* blk);
	int getTotalTime();
	int getNumOfProcesses();
};
