#pragma once
#include<iostream>
#include<string>
#include "Process.h"
#include"Processor.h"
#include"FCFS.h"
#include"RR.h"
#include "SJF.h"
using namespace std;
#include "string"
#include "conio.h"
#include "Queue"
class SJF;
class FCFS;
class RR;
class UI
{
	friend ostream& operator << (ostream& output, Process& obj);
	friend ostream& operator << (ostream& output, FCFS& obj);
	friend ostream& operator << (ostream& output, RR& obj);
	friend ostream& operator << (ostream& output, SJF& obj);	

public:
	UI();
	UI(int timestep);
	string GetFileName();
	void printFCFSProcessorInfo(FCFS* obj,int ProccessorID);
	void printRRProcessorInfo(RR* obj, int ProccessorID);
	void printSJFProcessorInfo(SJF* obj, int ProccessorID);
	void nextTS();
	void printBLK(Queue<Process*>& blk, int numOfBlkProcesses);
	void printTRM(Queue<Process*>& trm, int numOfTRMProcesses);
	void PrintRunBase(int Numofcurrentproccesses);
	void printRunloop(int processorID, Process* running);
};

