#pragma once
#include<cstdlib>
#include <fstream>
#include "Scheduler.h"
//Reads everything from an input file and does all necessary initializations

bool Scheduler::LoadFromFile(string file) {
	ifstream read;
	read.open(file);	
	string temp;
	//Looping on 1st 5 lines getting initial system values
	for (int i = 1; i < 6; i++) {
		getline(read, temp);
		int k;
		int c;
		switch (i)
		{
		case(1):
			//Handling the number of processors line
			k = 0;
			c = 0;
			int processornumbers[4];
			while (temp[k] != '\0') {
				string temp2;
				while (temp[k] != ' '&& temp[k] != '\0') {
					temp2 += temp[k];					
				k++;
				}
				processornumbers[c] = stoi(temp2);
				c++;
				if(temp[k] != '\0')
				k++;
			}
			FCFS_NUM = processornumbers[0];
			SJF_NUM = processornumbers[1];
			RR_NUM = processornumbers[2];
			EDF_NUM = processornumbers[3];
			break;
		case(2):
			//The roundrobin time slice number
			RR_TS = stoi(temp);
			break;
		case(3):
			//Handling the RTF, MAxW, STL and fork probability in order
			k = 0;
			c = 0;
			int misc[4];
			while (temp[k] != '\0') {
				string temp2;
				while (temp[k] != ' '&& temp[k] != '\0') {
					temp2+= temp[k];
				k++;
				}
				misc[c] = stoi(temp2);
				c++;
				if(temp[k] != '\0')
				k++;
			}
			RTF = misc[0];
			MaxW = misc[1];
			STL = misc[2];
			ForkProb = misc[3];
			break;
		case(4):
			//Overheat duration input
			OverheatNum = stoi(temp);
		case(5):
			//Number of processes
			PROCESS_NUM = stoi(temp);
			break;
		}
	}
	//Looping on N Process lines
	PROCESSOR_NUM = FCFS_NUM + SJF_NUM + RR_NUM + EDF_NUM;
	if (PROCESS_NUM == 0 || PROCESSOR_NUM == 0)
		return false;
	for (int i = 0; i < PROCESS_NUM; i++) {
		getline(read, temp);
		int k = 0;
		int c = 0;
		int Process_Data[5];
		while (temp[k] != '\0'&&temp[k]!='(') {
			string temp2;
			while (temp[k] != ' '&& temp[k] != '\0' && temp[k] != '(') {
				temp2 += temp[k];
			k++;
			}
			Process_Data[c] = stoi(temp2);
			c++;
			if (temp[k] != '\0')
				k++;
		} 
		IO* IOArr = new IO[Process_Data[3]];
		//Handle the io requests of the process if there are any.
		if (Process_Data[3] != 0) {
			for (int j = 0; j < Process_Data[3]; j++) {
				k++;
				string temp2;
				while (temp[k] != ',') {
					temp2 += temp[k];
					k++;
				}
				IOArr[j].R = stoi(temp2);
				k++;
				temp2 = "";
				while (temp[k] != ')') {
					temp2 += temp[k];
					k++;
				}
				IOArr[j].D = stoi(temp2);
				if (j != Process_Data[3] - 1)
					k += 2;
			}
		}
		//Adding the created Processes to the new queue
		Process* tempPro=new Process(Process_Data[0], Process_Data[1], Process_Data[2], Process_Data[4],Process_Data[3],IOArr);
		NEW.Enqueue(tempPro);
	}
	//Looping on Kill Signals
	getline(read, temp);
	while (getline(read, temp)) {
		int k = 0;
		int c = 0;
		int misc[2];
		while (temp[k] != '\0') {
			string temp2;
			while (temp[k] != ' '&& temp[k] != '\0') {
				temp2 += temp[k];
				k++;
			}
			misc[c] = stoi(temp2);
			c++;
			if(temp[k] != '\0')
			k++;
		}
		SIGKILL temp3;
		temp3.Kill_PID = misc[1];
		temp3.Kill_Time = misc[0];
		Kill_Process.Insert(temp3);
	}
	//Initializing the overheat probability with a low value, can be changed only from here
	int OverheatProbability = 0;

	//Creating the list of available processors
	PROCESSOR_NUM = FCFS_NUM + SJF_NUM + RR_NUM + EDF_NUM;
	ProcessorList = new Processor*[PROCESSOR_NUM];
	for (int i = 0; i < PROCESSOR_NUM; i++) {
		if (i >= 0 && i < FCFS_NUM) {
			ProcessorList[i] = new FCFS(ForkProb,this,OverheatNum,OverheatProbability);
		}
		if (i >= FCFS_NUM && i < (FCFS_NUM+SJF_NUM)) {
			ProcessorList[i] = new SJF(this, OverheatNum, OverheatProbability);
		}
		if (i >= FCFS_NUM + SJF_NUM && i < PROCESSOR_NUM-EDF_NUM) {
			ProcessorList[i] = new RR(RR_TS,this, OverheatNum, OverheatProbability);
		}
		if (i >= PROCESSOR_NUM - EDF_NUM && PROCESSOR_NUM) {
			ProcessorList[i] = new EDF(this, OverheatNum, OverheatProbability);
		}
	}
	return true;
}
//Scheduler default constructor, initializes everything

Scheduler::Scheduler() {
	RR_TS = 0;
	RTF = 0; 
	MaxW = 0;
	STL = 0; 
	ForkProb = 0;
	FCFS_NUM = 0;
	SJF_NUM = 0;
	RR_NUM = 0;
	EDF_NUM = 0;
	PROCESS_NUM = 0;
	ProcessorList = nullptr;
	SystemTime = 0;
	RunningProcessesSum = 0;
	PROCESSOR_NUM = 0;
	SUM_TRT = 0;
	DLPass = 0;
	OverheatNum = 0;
	RRMigration = 0;
	SJFMigration = 0;
	WRKSteal = 0;
	ForkedProcess = 0;
	KilledProcess = 0;
}

//Creating the forked processes
void Scheduler::AddForkedProcess(Process* parent) {
	Process* added = new Process(SystemTime, ++PROCESS_NUM, parent->getWorkingTime(),parent->getDL(), 0, true);
	if (!parent->getLChild())
		parent->setLChild(added);
	else
		parent->setRChild(added);
	ForkedProcess++;
	ScheduleToShortestFCFS(added);
}

//Phase 2 Scheduling of new items into shortest RDY queue
bool Scheduler::ScheduleNewlyArrived() {
	Process* temp;
	temp = NEW.Peek();
	if (temp!=nullptr && temp->getAT() == SystemTime) {
		NEW.Dequeue(&temp);
		ScheduleToShortest(temp);
		return true;
	}
	return false;
}

//Scheduling to the PRK queue in case of total system overheat
bool Scheduler::SchedulePRK() {
	Process* temp = nullptr;
	PRK.Dequeue(&temp);
	if (temp != nullptr) {
		if (ScheduleToShortest(temp)) {
			return true;
		}
	}
	return false;
}

//Schedules processes in the shortest FCFS RDY Queue
void Scheduler::ScheduleToShortestFCFS(Process* added) {
	int index = -1;
	int shortest = INT_MAX;
	for (int i = 1; i < FCFS_NUM; i++) {
		if (ProcessorList[i]->getTOH() > 0) {
			continue;
		}
		if (ProcessorList[i]->getTotalTime() < shortest) {
			index = i;
			shortest = ProcessorList[i]->getTotalTime();
		}
	}
	if (index == -1) {
		added->setisKilled(true);
		SendToTRM(added);
		return;
	}
	ProcessorList[index]->AddtoRDY(added);
}

//Schedules processes in the shortest SJF RDY Queue
bool Scheduler::ScheduleToShortestSJF(Process* added) {
	int index = -1;
	int end = FCFS_NUM + SJF_NUM;
	int shortest = INT_MAX;
	for (int i = FCFS_NUM ; i < end; i++) {
		if (ProcessorList[i]->getTOH() > 0) {
			continue;
		}
		if (ProcessorList[i]->getTotalTime() < shortest) {
			index = i;
			shortest = ProcessorList[i]->getTotalTime();
		}

	}
	if (index == -1) {
		return false;
	}
	ProcessorList[index]->AddtoRDY(added);
	return true;
}

//Schedules processes in the Shortest RR RDY Queue
bool Scheduler::ScheduleToShortestRR(Process* added) {
	int index = -1;
	int end = PROCESSOR_NUM - EDF_NUM;
	int shortest = INT_MAX;
	for (int i = FCFS_NUM + SJF_NUM  ; i < end; i++) {
		if (ProcessorList[i]->getTOH() > 0) {
			continue;
		}
		if (ProcessorList[i]->getTotalTime() < shortest) {
			index = i;
			shortest = ProcessorList[i]->getTotalTime();
		}
	}
	if (index == -1) {
		return false;
	}
	ProcessorList[index]->AddtoRDY(added);
	return true;
}

//Schedules processes in the shortest RDY Queue
bool Scheduler::ScheduleToShortest(Process* added) {
	int index = -1;
	
	int shortest = INT_MAX;
	for (int i = 0; i < PROCESSOR_NUM; i++) {
		if (ProcessorList[i]->getTOH() > 0) {
			continue;
		}
		if (ProcessorList[i]->getTotalTime() < shortest) {
			index = i;
			shortest = ProcessorList[i]->getTotalTime();
		}
	}
	 
	if (index == -1) {
		PRK.Enqueue(added);
		return false;
	}
	ProcessorList[index]->AddtoRDY(added);
	return true;
}

//Extra Function used in cases of external classes
void Scheduler::SendToShortest(Process* x) {
	this->ScheduleToShortest(x);
}

//Searches for a process by id and terminates it(Only applicable in FCFS Processor)
bool Scheduler::KillProcess(int IDKill) {
	Process* temp=nullptr;
	for (int i = 0; i < FCFS_NUM; i++) {
		//Checks if the process to be removed is in the run state in the processor and removes it
		if (ProcessorList[i]->GetRun() != nullptr) {
			temp = ProcessorList[i]->GetRun();
			if (temp->getID() == IDKill) {
				ProcessorList[i]->RemoveProcess(IDKill,&temp);
				KilledProcess++;
				SendToTRM(temp);
				return true;
			}
		}
		//Checks the RDY queue for the process and removes it if found
		if (ProcessorList[i]->FindProcessByID(IDKill,temp)) {
			ProcessorList[i]->RemoveProcess(IDKill,&temp);
			KilledProcess++;
			SendToTRM(temp);
			return true;
		}
	}
	return false;
}

//Checks for forked children of the process and terminates them.
void Scheduler::KillOrphans(Process* TRMParent) {
	Process* tempR;
	Process* tempL;
	if (TRMParent != nullptr) {
		//Terminating left child
		tempL = TRMParent->getLChild();
		if (tempL != nullptr) {
			KillProcess(tempL->getID());
		}
		//Terminating right child
		tempR = TRMParent->getRChild();
		if (tempR != nullptr) {
			KillProcess(tempR->getID());
		}
	}
	return;
}

//Add to IO request list (BLK)
void Scheduler::SendToBLK(Process* temp) { BLK.Enqueue(temp); }

//Add to the termination list(TRM)
void Scheduler::SendToTRM(Process* temp) {
	TRM.Enqueue(temp);
	//Calls the kill orphan function to remove all forked processes related to it
	KillOrphans(temp);
	//Setting process termination time and other data members related to termination
	temp->setTT(SystemTime);
	//Checks if the process being terminated was removed abnormally and correctly sets is waiting time.
	if (temp->getisKilled()) {
		int num = (temp->getTT()-temp->getAT()) - (temp->getCT() - temp->getWorkingTime());
		if (num >= 0)
			temp->setWT(num);
		else
			temp->setWT(0);
	}
	//Adds to total turn around time of the system
	SUM_TRT += temp->getTRT();
	//Checks if process has terminated before deadline
	if (temp->getDL() > SystemTime)
		DLPass++;
}

//Getters of data members
int Scheduler::GetMaxW() { return MaxW; }
int Scheduler::GetPROCESSNUM() { return PROCESS_NUM; }
int Scheduler::GetRTF() { return RTF; }
int Scheduler::GetSTL_Time() { return STL; }
int Scheduler::GetSystemTime() { return SystemTime; }
int Scheduler::Get_RR_TimeSlice() { return RR_TS; }
int Scheduler::GetFCFS_NUM() { return FCFS_NUM; }
int Scheduler::GetSJF_NUM() { return SJF_NUM; }
int Scheduler::GetRR_NUM() { return RR_NUM; }
int Scheduler::GetForkProb() { return ForkProb; }
int Scheduler::getRunningProcess() { return RunningProcessesSum; }

//Manipulating DataMembers
void Scheduler::IncrementProcessNum() { PROCESS_NUM++; }
void Scheduler::DecrementProcessNum() { PROCESS_NUM--; }

void Scheduler::IncrementSystemTime() { SystemTime++; }
void Scheduler::DecrementSystemTime() { SystemTime--; }

void Scheduler::IncrementRunningProcessesSum() { RunningProcessesSum++; }
void Scheduler::DecrementRunningProcessesSum() { RunningProcessesSum--; }

//Processing functions
void Scheduler::Processing(bool mode) {
	//Check for kill signals before processing
	while (KillSignalProcessing());
	//Schedule Processes arriving at current timestep
	while (ScheduleNewlyArrived());
	// Tests if process is able to move from the PRK queue to the unoverheated processors
	while (SchedulePRK());
	// Activate Work Stealing
	if (SystemTime % STL == 0 && SystemTime != 0)
		WorkStealing();
	//Check running processes
	for (int i = 0; i < PROCESSOR_NUM; i++)
		ProcessorList[i]->tick();
	//Processing of IO
	BLKProcessing();
	if (mode)
		PrintSystemInfo();
	SystemTime++;
}

void Scheduler::BLKProcessing() {
	Process* temp = BLK.Peek();
	if (temp) {
		temp->incrementtotalIO_D();
		if (CheckBLK(temp)) {
			Process** pt = &temp;
			temp->incrementIO();
			BLK.Dequeue(pt);
			ScheduleToShortest(temp);
		}
	}
}

bool Scheduler::CheckBLK(Process* ptr) {
	if (!ptr->DecrementRemIOTime())
		return true;
	return false;
}

bool Scheduler::KillSignalProcessing() {
	int temp = 0;
	Kill_Process.CheckKillSignal(&temp, SystemTime);
	if (temp != 0) {
		KillProcess(temp);
		return true;
	}
	return false;
}

void Scheduler::WorkStealing() {
	int min = INT_MAX;
	int max = INT_MIN;
	int tempindmin = -1;
	int tempindmax = -1;
	Processor* LQF = nullptr;
	Processor* SQF = nullptr;
	for (int i = 0; i < PROCESSOR_NUM; i++) {
		if (ProcessorList[i]->getTT() < min) {
			min = ProcessorList[i]->getTT();
			tempindmin = i;
		}
		if (ProcessorList[i]->getTT() > max) {
			max = ProcessorList[i]->getTT();
			tempindmax = i;
		}
	}
	LQF = ProcessorList[tempindmax];
	SQF = ProcessorList[tempindmin];
	float TTmax = LQF->getTT();
	float TTmin = SQF->getTT();
	float STLRatio;
	while (true) {
		STLRatio = ((TTmax - TTmin) / TTmax) * 100;
		if (STLRatio < 40)
			break;
		Process* tmpo = LQF->gettopProcess();
		if (!tmpo)
			break;
		SQF->AddtoRDY(tmpo);
		WRKSteal++;
		TTmax = LQF->getTT();
		TTmin = SQF->getTT();
	}

}

bool Scheduler::ProcessMigration(Process* p,bool x) {
	if (x) {

  	RRMigration++;
		return ScheduleToShortestRR(p);
	}
	else {
    	SJFMigration++;
		 return ScheduleToShortestSJF(p);
	}
}

//Termination condition
bool Scheduler::Terminate() {
	if (TRM.getCount() == PROCESS_NUM||PROCESS_NUM==0||PROCESSOR_NUM==0)
		return true;
	return false;
}

//Print all System information and needed functions
void Scheduler::PrintSystemInfo() {
	UI wind(SystemTime);
	for (int i = 0; i < PROCESSOR_NUM; i++) {
		if (i >= 0 && i < FCFS_NUM) {
			wind.printFCFSProcessorInfo((FCFS*)ProcessorList[i], i + 1);
		}
		if (i >= FCFS_NUM && i < (FCFS_NUM + SJF_NUM)) {
			wind.printSJFProcessorInfo((SJF*)ProcessorList[i], i + 1);
		}
		if (i >= FCFS_NUM + SJF_NUM && i < PROCESSOR_NUM - EDF_NUM) {
			wind.printRRProcessorInfo((RR*)ProcessorList[i], i + 1);
		}
		if (i >= PROCESSOR_NUM - EDF_NUM && PROCESSOR_NUM) {
			wind.printEDFProcessorInfo((EDF*)ProcessorList[i], i + 1);
		}
	}
	wind.printBLK(BLK, BLK.getCount());
	wind.printPRK(PRK, PRK.getCount());
	wind.PrintRunBase(RunningProcessesSum);
	for (int i = 0; i < PROCESSOR_NUM; i++) {
		Process* temp=ProcessorList[i]->GetRun();
		if (temp!=nullptr) {
			wind.printRunloop(i + 1, temp);
		}
	}
	wind.printTRM(TRM, TRM.getCount());
}

void Scheduler::PrintTRM() {
	TRM.Print();
}

//Output File creation. Calculates all required percentages and handles formatting.
void Scheduler::OutputFile() {
	FILE* outputFile;
	if (PROCESS_NUM == 0 || PROCESSOR_NUM == 0)
		return;
	try {
		if (fopen_s(&outputFile,"output.txt", "w"))
			throw runtime_error("Failed to open the file.");
		fprintf(outputFile,"%-8s %-8s %-8s %-8s %-8s %-8s %-8s %-8s\n", "TT", "pID", "AT", "CT", "IO_D", "WT", "RT", "TRT");
		int i = TRM.getCount();
		int totalwt = 0;
		int totalrt = 0;
		int totaltrt = SUM_TRT;
		Process** temp = new Process * [PROCESS_NUM];
		for (int i = 0; i < PROCESS_NUM; i++) {
			TRM.Dequeue(&temp[i]);
			totalwt += (temp[i])->getWT();
			totalrt += (temp[i])->getRT();
			PrintProcesses(temp[i], outputFile);
			TRM.Enqueue(temp[i]);
		}
		float avgwt = float(totalwt) / PROCESS_NUM;
		float avgrt = float(totalrt) / PROCESS_NUM;
		float avgtrt = float(totaltrt) / PROCESS_NUM;
		float TotalMigration = RRMigration + SJFMigration;
		float RRMig = 0;
		float SJFMig = 0;
		if (TotalMigration != 0) {
			RRMig = (RRMigration / TotalMigration) * 100;
			SJFMig = (SJFMigration / TotalMigration) * 100;
		}
		float WRK = (WRKSteal / PROCESS_NUM) * 100;
		float FRK = (ForkedProcess / PROCESS_NUM) * 100;
		float KILL = (KilledProcess / PROCESS_NUM) * 100;
		float DEADLINE = (DLPass / PROCESS_NUM) * 100;
		delete[] temp;
		fprintf(outputFile, "\nProcesses: %d\n", PROCESS_NUM);
		fprintf(outputFile, "AVG WT: %-10.3f AVG RT: %-10.3f AVG TRT: %-10.3f\n",avgwt,avgrt,avgtrt);
		fprintf(outputFile, "TRM before deadline %%: %-0.3f%%\n", DEADLINE);
		fprintf(outputFile, "Migration %%: \t RTF = %-0.3f%%\t\tMaxW = %-0.3f%%\n",SJFMig,RRMig);
		fprintf(outputFile, "Work Steal %%: %-0.3f%%\n", WRK);
		fprintf(outputFile, "Forked Processes %%: %-0.3f%%\n", FRK);
		fprintf(outputFile, "Killed Processes %%: %-0.3f%%\n\n", KILL);
		fprintf(outputFile, "Processors: %d [%d FCFS, %d SJF, %d RR, %d EDF]\n",PROCESSOR_NUM, FCFS_NUM, SJF_NUM, RR_NUM, EDF_NUM);
		fprintf(outputFile, "Processor Load:\n");
		float sumload = 0;
		for (int i = 0; i < PROCESSOR_NUM; i++) {
			float load = (ProcessorList[i]->GetBusy() / SUM_TRT) * 100;
			sumload += load;
			fprintf(outputFile, "p%d: %-0.3f%%\n", i + 1, load);
		}
		fprintf(outputFile, "Average Load: %-0.3f%%\n", sumload / PROCESSOR_NUM);
		fprintf(outputFile, "\n");
		fprintf(outputFile, "Processor Utilization:\n");
		float sumuti = 0;
		for (int i = 0; i < PROCESSOR_NUM; i++) {
			float utilization = (ProcessorList[i]->GetBusy() / (ProcessorList[i]->GetBusy() + ProcessorList[i]->GetIdle())) * 100;
			sumuti += utilization;
			fprintf(outputFile, "p%d: %-0.3f%%\n", i + 1, utilization);
		}
		fprintf(outputFile, "Average Utilization : %-0.3f%%\n", sumuti / PROCESSOR_NUM);
		fclose(outputFile);
	}
	catch (const exception e) {
		cerr << "Error: " << e.what() << std::endl;
	}
}

//Handles formatting of the printed processes in output file
void Scheduler::PrintProcesses(Process* p, FILE* outputFile) {
	int AT = p->getAT();
	int TT = p->getTT();
	int RT = p->getRT();
	int CT = p->getCT();
	int pID = p->getID();
	int WT = p->getWT();
	int TRT = p->getTRT();
	int IO_D = p->getTotalIO_D();
	fprintf(outputFile, "%-8d %-8d %-8d %-8d %-8d %-8d %-8d %-8d\n", TT, pID, AT, CT, IO_D, WT, RT, TRT);
}

//Scheduler class destructor, handling all necessary deallocations

Scheduler::~Scheduler() {
	if (PROCESSOR_NUM != 0 && PROCESS_NUM != 0) {
		Process** temp = new Process * [PROCESS_NUM];
		for (int i = 0; i < PROCESS_NUM; i++) {
			TRM.Dequeue(&temp[i]);
			IO* arrtemp = temp[i]->GetIOArr();
			if (arrtemp)
				delete[] arrtemp;
			delete temp[i];
		}
		delete[]temp;
		for (int i = 0; i < PROCESSOR_NUM; i++) {
			delete ProcessorList[i];
		}
		delete[] ProcessorList;
	}
}
