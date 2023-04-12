#pragma once
#pragma once
#include"Processor.h"
#include"LinkedList.h"
class RR :public Processor {
private:
	LinkedList<Process*> list;
	Process* currentProcess = nullptr;
	int numOfProcesses;
	int totalTime; // The variable that has the count
public:
	RR(int);
	bool FindProcessByID(int id, Process* x);
	bool RemoveProcess(int id);
	Process AddtoRDY(Process* x);
	bool tick(Process* x);
	int GetQueueTime();
	void AddProcess(Process* x);
	int GetProcessCount();
};
