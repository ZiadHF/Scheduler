#pragma once
#include<string>
#include<fstream>
#include<iostream>
#include"Process.h"
#include"Structs.h"
using namespace std;
class Scheduler {
private:
	int RR_TS,RTF, MaxW, STL, ForkProb, FCFS_NUM, SJF_NUM, RR_NUM, PROCESS_NUM;
	Queue* NEW, BLK, TRM;
	List<SIGKILL> KILL_Process;
	
public:
	void LoadFromFile(string);
	void NewArrivals(Processor*);
	void IO_Requests(Processor*);
	void IO_Complete(Processor*);
};