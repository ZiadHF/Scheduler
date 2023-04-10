#pragma once
#include<string>
#include<fstream>
#include<iostream>
#include"Process.h"
#include"Structs.h"
#include"LinkedList.h"
#include"Queue.h"
using namespace std;
class Scheduler {
public:
private:
	int RR_TS,RTF, MaxW, STL, ForkProb, FCFS_NUM, SJF_NUM, RR_NUM, PROCESS_NUM,SystemTime;
	Queue<Process*>NEW, BLK, TRM;
	LinkedList<SIGKILL> Kill_Process;
	Processor* ProccesorList;
public:
	void LoadFromFile(string);
	Scheduler();
	void AddForkedProcess(Process*);
	void ScheduleToShortest(Process*);
	void ScheduleToShortestFCFS(Process*);
	void ScheduleToShortestSJF(Process*);
	void ScheduleToShortestRR(Process*);
	bool KillProcess(int);
	void KillOrphans(Process*);
	void AddToBLK(Process*);
	void AddToTRM(Process*);
};