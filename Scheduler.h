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
#include"SJF.h"
#include"UI.h"
using namespace std;
class Scheduler {
public:
private:
	int RR_TS,RTF, MaxW, STL, ForkProb, FCFS_NUM, SJF_NUM, RR_NUM, PROCESS_NUM,SystemTime,PROCESSOR_NUM,RunningProcessesSum, SUM_TRT;
	Queue<Process*>NEW, BLK, TRM;
	LinkedList<SIGKILL> Kill_Process;
	Processor** ProcessorList;
	//Scheduling Functions
	void ScheduleByLeastCount(Process*);
	bool ScheduleNewlyArrivedPhase1();
	bool ScheduleNewlyArrived();
	void ScheduleToShortest(Process*);
	void ScheduleToShortestFCFS(Process*);
	void ScheduleToShortestSJF(Process*);
	void ScheduleToShortestRR(Process*);
	//Process Addition And Removal;
	bool KillProcess(int);
	void KillOrphans(Process*);
	//Process Movement int lists
	bool CheckBLK(Process*);
	//SystemTime and Process Number Manipulation
	void IncrementSystemTime();
	void DecrementSystemTime();
	void IncrementProcessNum();
	void DecrementProcessNum();
	void BLKProcessing();
	void BLKProcessingPhase1();
	void RemoveRandomProcessPhase1();
	bool KillSignalProcessing();
public:
	void Processing();
	//Functions called by Processors
	void AddForkedProcess(Process*);
	void SendToTRM(Process*);
	void SendToBLK(Process*);
	//Others
	Scheduler();
	void LoadFromFile(string);
	bool Terminate();
	void Phase1Processing();
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
	int GetTotalIdleBusy();
	int GetForkProb();
	void PrintTRM();
	void incrementRunningProcessCount();
	void PrintSystemInfo();
	void OutputFile();
	~Scheduler();
};