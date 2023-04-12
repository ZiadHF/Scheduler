#pragma once
#include"Processor.h"
#include"LinkedList.h"
class FCFS :public Processor {
private:
	LinkedList<Process*> list;
	Process* currentProcess = nullptr;
	int numOfProcesses;
	int totalTime; // The variable that has the count
public:
	 FCFS();
	 bool FindProcessByID(int id, Process* x);
	 bool RemoveProcess(int id);
	 Process AddtoRDY(Process* x);
	 bool tick(Process* x);
	 void AddProcess(Process* x);
};
 