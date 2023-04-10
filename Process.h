#include "Structs.h"
#pragma once
class Process
{
	const int pID,AT;
	int RT, CT, TT, TRT, WT, N , WorkingTime;
	IO* IOArr;
public:
	Process(int a, int p, int ct, int io, IO* array);
	void setTT(int tt);
	void setRT(int rt);
	const int getID();
	const int getAT();
	int getRT();
	int getCT();
	int getTT();
	int getTRT();
	int getWT();
	int getN();
	int ReturnTotalIO_D();
	bool Decrement();
};

