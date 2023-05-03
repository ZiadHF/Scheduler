#pragma once
#include"Process.h"
class Processor {
public:
	virtual bool FindProcessByID(int id, Process* x) = 0;
	virtual bool RemoveProcess(int id,Process** x) = 0;
	virtual void tick(Process* x, Process *y = nullptr,Process * z = nullptr) = 0;
	virtual bool MoveToRun(int&,int) = 0;
	virtual Process* GetRun() = 0;
	virtual void AddtoRDY(Process* x) = 0;
	virtual int getTotalTime() = 0;
	virtual int getNumOfProcesses() = 0;
	virtual void RemoveRun() = 0;
	virtual float GetBusy() = 0;
	virtual float GetIdle() = 0;
};