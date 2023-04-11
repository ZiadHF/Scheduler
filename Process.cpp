#include "Process.h"

Process::Process(int a, int p, int ct, int io, IO* array) : pID(p), AT(a), CT(ct), N(io) {
	WorkingTime = CT;
	TT = 0;
	RT = 0;
	WT = 0;
	IOArr = array;
	Child = nullptr;
	RemIOTime = 0;
}
Process::Process(int a, int p, int ct, int io) : pID(p), AT(a), CT(ct), N(io) {
	WorkingTime = CT;
	TT = 0;
	RT = 0;
	WT = 0;
	IOArr = nullptr;
	Child = nullptr;
	RemIOTime = 0;
}
void Process::setTT(int tt) {
	TT = tt;
	TRT = TT - AT;
	WT = TRT - CT;
}
void Process::setChild(Process* child) { Child = child; }

void Process::setRT(int rt) { RT = rt; }

void Process::setRT(int rt) { RT = rt; }

const int Process::getID() { return pID; }

const int Process::getAT() { return AT; }

Process* Process::getChild() { return Child; }
int Process::getRT() { return RT; }

int Process::getCT() { return CT; }

int Process::getTT() { return TT; }

int Process::getTRT() { return TRT; }

int Process::getWT() { return WT; }

int Process::getN() { return N; }

int Process::getWorkingTime() { return WorkingTime; }

int Process::getRemIOTime() { return RemIOTime; }

int Process::ReturnTotalIO_D(){
	int s = 0;
	for (int i = 0; i < N; i++)
		s += IOArr[i].D;
}

bool Process::DecrementWorkingTime() {	
	if (WorkingTime == 0)
		return false;
	WorkingTime--;
	return true;
}

bool Process::DecrementRemIOTime() {
	if (RemIOTime == 0)
		return false;
	RemIOTime--;
	return true;
}