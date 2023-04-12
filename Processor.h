#pragma once
#include"Process.h"
class Processor {
	virtual bool FindProcessByID(int id, Process* x) = 0;
	virtual bool RemoveProcess(int id) = 0;
	virtual Process AddtoRDY(Process* x) = 0;
	virtual bool tick(Process* x) = 0;
	virtual void AddProcess(Process* x) = 0;
};