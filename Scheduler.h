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
#include"EDF.h"
using namespace std;
class Scheduler {
public:
private:

	int RR_TS,RTF, MaxW, STL, ForkProb, FCFS_NUM, SJF_NUM, RR_NUM, EDF_NUM, PROCESS_NUM,SystemTime,PROCESSOR_NUM, SUM_TRT,OverheatNum,RROverHeated,SJFOverHeated;
  float DLPass, RRMigration, SJFMigration, WRKSteal, ForkedProcess, KilledProcess;
	Queue<Process*>NEW, BLK, TRM,PRK;

	
	LinkedList<SIGKILL> Kill_Process;
	Processor** ProcessorList;
	//Scheduling Functions
	void ScheduleByLeastCount(Process*);
	bool ScheduleNewlyArrivedPhase1();
	void ScheduleToShortestOverHeat(Process* added);
	bool ScheduleNewlyArrived();
	bool ScheduleToShortest(Process*);
	bool ScheduleToShortestRR(Process*);
	bool ScheduleToShortestSJF(Process*);
  void WorkStealing();
	//Process Addition And Removal;
	bool KillProcess(int);
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
	bool SchedulePRK();
public:
	int RunningProcessesSum;
	void ScheduleToShortestFCFS(Process*);
	void SendToShortest(Process * x);
	void Processing(bool mode);
	//Functions called by Processors
	bool ProcessMigration(Process*,bool x);
	void AddForkedProcess(Process*);
	void SendToTRM(Process*);
	void SendToBLK(Process*);
	void KillOrphans(Process*);
	//Others
	Scheduler();
	void LoadFromFile(string);
	bool Terminate();
	void Phase1Processing();
	//Getters
	int getRunningProcess();
	int Get_RR_TimeSlice();
	int GetRTF();
	int GetMaxW();
	int GetSTL_Time();
	int GetPROCESSNUM();
	int GetSystemTime();
	int GetFCFS_NUM();
	int GetSJF_NUM();
	int GetRR_NUM();
	void IncrementRROverheat();
	void IncrementSJFOverheat();
	void DecrementRROverheat();
	void DecrementSJFOverheat();
	int GetTotalIdleBusy();
	int GetForkProb();
	void PrintTRM();
	void incrementRunningProcessCount();
	void PrintSystemInfo();
	void OutputFile();
	~Scheduler();
};