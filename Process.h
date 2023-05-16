#pragma once
#include <iostream>
using namespace std;
#include "Structs.h"
class Process
{
	const int pID,AT;
	int RT, CT, TT, TRT, WT, N , DL , WorkingTime,RemIOTime,ListChangeTime;
	int totalIO_D = 0;
	IO* IOArr;
	Process* LChild = nullptr;
	Process* RChild = nullptr;
	int currentIO = 0;
	bool isForked;
	bool isKilled = false;
	bool firstTime = true;

public:
	Process();
	Process(int a, int p, int ct, int io, int dl, IO* array, bool isFRK = false);
	Process(int a, int p, int ct, int io,int dl,bool isFRK = false);
	void setTT(int tt);
	void setRT(int rt);
	void setRemIOTime(int);
	void setLChild(Process*);
	void setRChild(Process*);
	void setListChangeTime(int);
	void setfirstTime(bool);
	const int getID();
	const int getAT();
	int getDL();
	int getIOIncrement();
	int getIO_D();
	int getRT();
	int getCT();
	int getTT();
	int getTRT();
	int getWT();
	int getN();
	int getRemIOTime();
	int getListChangeTime();
	void setisKilled(bool);
	bool getisKilled();
	void setWT(int);
	bool getfirstTime();
	bool getisForked();
	bool DecrementRemIOTime();
	bool CheckIO();
	int getWorkingTime();
	IO getIO();
	void incrementIO();
	void incrementtotalIO_D();
	Process* getLChild();
	Process* getRChild();
	friend ostream& operator<<(ostream& os, Process*& p){
		int temp = p->getID();
		os << temp;
			return os;
	}
	int getTotalIO_D();
	bool DecrementWorkingTime();
	bool JustArrived(int);
};

