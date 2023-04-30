#include "Process.h"

Process::Process(int a, int p, int ct, int io, IO* array) : pID(p), AT(a), CT(ct), N(io) {
	WorkingTime = CT;
	TT = 0;
	RT = 0;
	WT = 0;
	IOArr = array;
	LChild = nullptr;
	RChild = nullptr;
	RemIOTime = 0;
	ListChangeTime = AT;
	TRT = 0;
}

Process::Process(int a, int p, int ct, int io) : pID(p), AT(a), CT(ct), N(io) {
	WorkingTime = CT;
	TT = 0;
	RT = 0;
	WT = 0;
	IOArr = nullptr;
	LChild = nullptr;
	RChild = nullptr;
	RemIOTime = 0;
	ListChangeTime = AT;
	TRT = 0;
}

void Process::setTT(int tt) {
	TT = tt;
	TRT = TT - AT;
	WT = TRT - CT;
}

void Process::setLChild(Process* child) { LChild = child; }

void Process::setRChild(Process* child) { RChild = child; }

void Process::setRT(int rt) { RT = rt; }

const int Process::getID() { return pID; }

const int Process::getAT() { return AT; }

Process* Process::getLChild() { return LChild; }

Process* Process::getRChild() { return RChild; }

int Process::getRT() { return RT; }

int Process::getCT() { return CT; }

int Process::getTT() { return TT; }

int Process::getTRT() { return TRT; }

int Process::getWT() { return WT; }

int Process::getN() { return N; }

int Process::getWorkingTime() { return WorkingTime; }

IO Process::getIO() { return IOArr[currentIO]; }

void Process::incrementIO() { currentIO++; }

int Process::getRemIOTime() { return RemIOTime; }

void Process::setRemIOTime(int in) { RemIOTime = in; }

int Process::ReturnTotalIO_D(){
	int s = 0;
	for (int i = 0; i < N; i++)
		s += IOArr[i].D;
	return s;
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
bool Process::JustArrived(int t) {
	return AT == t;
}
int Process::getListChangeTime() { return ListChangeTime; }

void Process::setListChangeTime(int t) { ListChangeTime = t; }