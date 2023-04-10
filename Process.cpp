#include "Process.h"

Process::Process(int a, int p, int ct, int io, IO* array) : pID(p), AT(a), CT(ct), N(io) {
	WorkingTime = CT;
	TT = 0;
	RT = 0;
	WT = 0;
	IOArr = array;
}
void Process::setTT(int tt) {
	TT = tt;
	TRT = TT - AT;
	WT = TRT - CT;
}
void Process::setRT(int rt) { RT = rt; }

void Process::setRT(int rt) { RT = rt; }

const int Process::getID() { return pID; }

const int Process::getAT() { return AT; }

int Process::getRT() { return RT; }

int Process::getCT() { return CT; }

int Process::getTT() { return TT; }

int Process::getTRT() { return TRT; }

int Process::getWT() { return WT; }

int Process::getN() { return N; }

int Process::ReturnTotalIO_D(){
	int s = 0;
	for (int i = 0; i < N; i++)
		s += IOArr[i].D;
}

bool Process::Decrement() {	
	if (WorkingTime == 0)
		return true;
	WorkingTime--;
	return false;
}