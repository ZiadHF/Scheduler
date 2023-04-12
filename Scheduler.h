#pragma once
#include<string>
#include<fstream>
#include<iostream>
#include"Process.h"
#include"Structs.h"
#include"LinkedList.h"
#include"Queue.h"
#include"Processor.h"
#include"FCFS.h"
#include"RR.h"
using namespace std;
class Scheduler {
public:
private:
	int RR_TS,RTF, MaxW, STL, ForkProb, FCFS_NUM, SJF_NUM, RR_NUM, PROCESS_NUM,SystemTime;
	Queue<Process*>NEW, BLK, TRM;
	LinkedList<SIGKILL> Kill_Process;
	Processor** ProcessorList;
public:
	void LoadFromFile(string);
	Scheduler();
	//Scheduling Functions
	bool ScheduleNewlyArrived();
	void ScheduleToShortest(Process*);
	void ScheduleToShortestFCFS(Process*);
	void ScheduleToShortestSJF(Process*);
	void ScheduleToShortestRR(Process*);
	void ScheduleByLeastCount(Process*);
	//Process Addition And Removal;
	void AddForkedProcess(Process*);
	bool KillProcess(int);
	void KillOrphans(Process*);
	//Process Movement int lists
	void SendToBLK(Process*);
	void SendToTRM(Process*);
	//Getters
	int Get_RR_TimeSlice();
	int GetRTF();
	int GetMaxW();
	int GetSTL_Time();
	int GetPROCESSNUM();
	int GetSystemTime();
	int GetFCFS_NUM();
	int GetSJF_NUM();
	int GetRR_NUM();
	int GetForkProb();
	//SystemTime and Process Number Manipulation
	void IncrementSystemTime();
	void DecrementSystemTime();
	void IncrementProcessNum();
	void DecrementProcessNum();
	void BLKProcessing();
	
};