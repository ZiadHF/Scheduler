#pragma once
#include"Processor.h"
#include"Queue.h"
#include "Scheduler.h"
class Scheduler;
class RR :public Processor {
private:
	Queue<Process*> list;
	Process* currentProcess = nullptr;
	int numOfProcesses = 0;
	int totalTime = 0; // The variable that has the count
	int remainingticks;
	int TimeSlice;
	int OverheatProb;
	int TOH = 0;
	int Overheat;
	float busy,idle;
	Scheduler* s = nullptr;
public:
	RR(int t,Scheduler*,int overH,int prob);
	// OverHeating functions:
	int getTOH();
	// Setters and getters:
	int getTT();
	Process* gettopProcess();
	Queue<Process*>& getlist();
	float GetBusy();
	float GetIdle();
	void SetScheduler(Scheduler* );
	Process* GetRun();
	int getTotalTime();
	int getNumOfProcesses();
	// Moving Processes
	bool RemoveProcess(int id, Process** x);
	bool MoveToRun(int&,int); 
	void AddtoRDY(Process* x);
	void tick();
	void RemoveRun();
	// Helper Functions:
	bool FindProcessByID(int id, Process* x);
	void IncrementBusy();
	void IncrementIdle();
	
	
};
