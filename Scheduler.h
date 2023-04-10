#pragma once
#include<string>
#include<fstream>
#include<iostream>
#include"Process.h"
#include"Structs.h"
#include"LLL.h"
using namespace std;
class Scheduler {
public:
	int RR_TS,RTF, MaxW, STL, ForkProb, FCFS_NUM, SJF_NUM, RR_NUM, PROCESS_NUM;
	//Queue* NEW, BLK, TRM;
//	LLL<SIGKILL> KILL_Process;
	void LoadFromFile(string);
	Scheduler();
	//void NewArrivals(Processor*);
	//void IO_Requests(Processor*);
	//void IO_Complete(Processor*);
};