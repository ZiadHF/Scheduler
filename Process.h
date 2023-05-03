#pragma once
#include <iostream>
using namespace std;
#include "Structs.h"
class Process
{
	const int pID,AT;
	int RT, CT, TT, TRT, WT, N , WorkingTime,RemIOTime,ListChangeTime;
	IO* IOArr;
	Process* LChild = nullptr;
	Process* RChild = nullptr;
	int currentIO = 0;


public:
	Process();
	Process(int a, int p, int ct, int io, IO* array);
	Process(int a, int p, int ct, int io);
	void setTT(int tt);
	void setRT(int rt);
	void setRemIOTime(int);
	void setLChild(Process*);
	void setRChild(Process*);
	void setListChangeTime(int);
	const int getID();
	const int getAT();
	int getIO_D();
	int getRT();
	int getCT();
	int getTT();
	int getTRT();
	int getWT();
	int getN();
	int getRemIOTime();
	int getListChangeTime();
	bool DecrementRemIOTime();
	int getWorkingTime();
	IO getIO();
	void incrementIO();
	Process* getLChild();
	Process* getRChild();
	friend ostream& operator<<(ostream& os, Process*& p){
		int temp = p->getID();
		os << temp;
			return os;
	}
	int ReturnTotalIO_D();
	bool DecrementWorkingTime();
	bool JustArrived(int);
};

