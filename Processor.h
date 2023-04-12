#pragma once
#include"Process.h"
class Processor {
	 
	virtual bool RemoveProcess(int id) = 0;
	virtual void tick(Process* x, Process *y = nullptr,Process * z = nullptr) = 0;
	virtual void AddtoRDY(Process* x) = 0;
	virtual int getTotalTime() = 0;
	virtual int getNumOfProcesses() = 0;
};