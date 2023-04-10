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
private:
	int RR_TS,RTF, MaxW, STL, ForkProb, FCFS_NUM, SJF_NUM, RR_NUM, PROCESS_NUM;
	Queue* NEW, BLK, TRM;
	LinkedList<SIGKILL> Kill_Process;
	Processor* ProccesorList;

public:
	void LoadFromFile(string);
	Scheduler();
	void AddForkedProcess(Process);

};