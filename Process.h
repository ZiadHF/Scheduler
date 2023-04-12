#pragma once
#include "Structs.h"
class Process
{
	const int pID,AT;
	int RT, CT, TT, TRT, WT, N , WorkingTime,RemIOTime;
	IO* IOArr;
	Process* Child = nullptr;
	int currentIO = 0;


public:
	Process(int a, int p, int ct, int io, IO* array);
	Process(int a, int p, int ct, int io);
	void setTT(int tt);
	void setRT(int rt);
	void setRemIOTime(int);
	void setChild(Process*);

	const int getID();
	const int getAT();
	int getRT();
	int getCT();
	int getTT();
	int getTRT();
	int getWT();
	int getN();
	int getRemIOTime();
	bool DecrementRemIOTime();
	int getWorkingTime();
	IO getIO();
	void incrementIO();
	Process* getChild();
	int ReturnTotalIO_D();
	bool DecrementWorkingTime();
};

