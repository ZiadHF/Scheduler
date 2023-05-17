#include "Process.h"

Process::Process() : pID(),AT(){ }

Process::Process(int a, int p, int ct,int dl, int io, IO* array,bool isFRK) : pID(p), AT(a), CT(ct), N(io), DL(dl) {
	WorkingTime = CT;
	TT = 0;
	RT = 0;
	WT = 0;
	IOArr = array;
	LChild = nullptr;
	RChild = nullptr;
	RemIOTime = IOArr[currentIO].D;
	ListChangeTime = AT;
	TRT = 0;
	isForked = false;
}

Process::Process(int a, int p, int ct, int dl,int io ,bool isFRK) : pID(p), AT(a), CT(ct), N(io), DL(dl) {
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
	isForked = isFRK;
}

void Process::setTT(int tt) {
	TT = tt;
	TRT = TT - AT;
	WT = TRT - CT + 1;
	//if (WT < 0)
	//	WT = 0;
}

void Process::setWT(int x) { WT = x; }

bool Process:: getisKilled() { return isKilled; }

void Process::setisKilled(bool ch) { isKilled = ch; }

bool Process::getisForked() { return isForked; }

bool Process::getfirstTime() { return firstTime; }

void Process::setfirstTime(bool s) { firstTime = s; }

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

int Process::getDL() { return DL; }

int Process::getWorkingTime() { return WorkingTime; }

int Process::getIO_D() {
	int sum = 0;
	if (N > 0) {
		for (int i = 0; i < N; i++)
			sum += IOArr[i].D;
	}
	return sum;
}

IO Process::getIO() {
		return IOArr[currentIO];
}

bool Process::CheckIO() { 
	if (N == 0 || currentIO > N)
		return true;
	return false;
}
IO* Process::GetIOArr() {
	return IOArr;
}

int Process::getIOIncrement() { return currentIO; }

void Process::incrementIO() { currentIO++; }

int Process::getRemIOTime() { return RemIOTime; }

void Process::setRemIOTime(int in) { RemIOTime = in; }

int Process::getTotalIO_D() { return totalIO_D; }

void Process::incrementtotalIO_D() { totalIO_D++; }

bool Process::DecrementWorkingTime() {	
	WorkingTime--;
	if (WorkingTime == 0)
		return false;
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