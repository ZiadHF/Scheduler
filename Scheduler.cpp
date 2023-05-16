#pragma once
#include<cstdlib>
#include <fstream>
#include "Scheduler.h"
//Reads everything from an input file and does all necessary initializations
void Scheduler::LoadFromFile(string file) {
	ifstream read;
	read.open(file);	
	string temp;
	//Looping on 1st 4 lines getting initial system values
	for (int i = 1; i < 5; i++) {
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
			//Number of processes
			PROCESS_NUM = stoi(temp);
			break;
		}
	}
	//Looping on N Process lines
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
		Process* tempPro=new Process(Process_Data[0], Process_Data[1], Process_Data[2], Process_Data[4],Process_Data[3],IOArr);

		NEW.Enqueue(tempPro);
		
		//Testing function for printing all processes.
		/*
			cout <<endl<< Process_Data[0] << " " << Process_Data[1] << " " << Process_Data[2] << " " << Process_Data[3] << " ";
			for (int u = 0; u < Process_Data[3]; u++) {
				cout << "(" << IOArr[u].R << "," << IOArr[u].D << ")";
			}
			cout << endl;
		*/
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
	//Creating the list of available processors
	PROCESSOR_NUM = FCFS_NUM + SJF_NUM + RR_NUM + EDF_NUM;
	ProcessorList = new Processor*[PROCESSOR_NUM];
	for (int i = 0; i < PROCESSOR_NUM; i++) {
		if (i >= 0 && i < FCFS_NUM) {
			ProcessorList[i] = new FCFS(ForkProb,this);
		}
		if (i >= FCFS_NUM && i < (FCFS_NUM+SJF_NUM)) {
			ProcessorList[i] = new SJF(this);
		}
		if (i >= FCFS_NUM + SJF_NUM && i < PROCESSOR_NUM-EDF_NUM) {
			ProcessorList[i] = new RR(RR_TS,this);
		}
		if (i >= PROCESSOR_NUM - EDF_NUM && PROCESSOR_NUM) {
			ProcessorList[i] = new EDF(this);
		}
	}
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
	RRMigration = 0;
	SJFMigration = 0;
	WRKSteal = 0;
	ForkedProcess = 0;
	KilledProcess = 0;
}

//

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

//Schedule newly arrived process Phase1
bool Scheduler::ScheduleNewlyArrivedPhase1() {
	Process* temp;
	temp = NEW.Peek();
	if (temp!=nullptr && temp->getAT() == SystemTime) {
		NEW.Dequeue(&temp);
		ScheduleByLeastCount(temp);
		return true;
	}
	return false;

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

//Schedules processes in the shortest FCFS RDY Queue
void Scheduler::ScheduleToShortestFCFS(Process* added) {
	int index = 0;
	int shortest = ProcessorList[0]->getTotalTime();
	for (int i = 1; i < FCFS_NUM; i++) {
		if (ProcessorList[i]->getTotalTime() < shortest) {
			index = i;
			shortest = ProcessorList[i]->getTotalTime();
		}
	}
	ProcessorList[index]->AddtoRDY(added);
}

//Schedules processes in the shortest SJF RDY Queue
void Scheduler::ScheduleToShortestSJF(Process* added) {
	int index = FCFS_NUM;
	int end = FCFS_NUM + SJF_NUM;
	int shortest = ProcessorList[index]->getTotalTime();
	for (int i = FCFS_NUM+1 ; i < end; i++) {
		if (ProcessorList[i]->getTotalTime() < shortest) {
			index = i;
			shortest = ProcessorList[i]->getTotalTime();
		}
	}
	ProcessorList[index]->AddtoRDY(added);
}

//Schedules processes in the Shortest RR RDY Queue
void Scheduler::ScheduleToShortestRR(Process* added) {
	int index = FCFS_NUM + SJF_NUM;
	int end = PROCESSOR_NUM - EDF_NUM;
	int shortest = ProcessorList[index]->getTotalTime();
	for (int i = FCFS_NUM + SJF_NUM + 1 ; i < end; i++) {
		if (ProcessorList[i]->getTotalTime() < shortest) {
			index = i;
			shortest = ProcessorList[i]->getTotalTime();
		}
	}
	ProcessorList[index]->AddtoRDY(added);
}

//Schedules processes in the shortest RDY Queue
void Scheduler::ScheduleToShortest(Process* added) {
	int index=0;
	int shortest = ProcessorList[0]->getTotalTime();
	for (int i = 1; i < PROCESSOR_NUM; i++) {
		if (ProcessorList[i]->getTotalTime() < shortest) {
			index = i;
			shortest = ProcessorList[i]->getTotalTime();
		}
	}
	ProcessorList[index]->AddtoRDY(added);
}

//Schedule according to process count in RDY Queue(Phase 1 Function)
void Scheduler::ScheduleByLeastCount(Process* added) {
	int index=0;
	int least = ProcessorList[0]->getNumOfProcesses();
	for (int i = 1; i < PROCESSOR_NUM; i++) {
		if (ProcessorList[i]->getNumOfProcesses() < least) {
			index = i;
			least = ProcessorList[i]->getNumOfProcesses();
		}
	}
	ProcessorList[index]->AddtoRDY(added);
}

//Searches for a process by id and terminates it(Only applicable in FCFS Processor)
bool Scheduler::KillProcess(int IDKill) {
	Process* temp=nullptr;
	for (int i = 0; i < FCFS_NUM; i++) {
		if (ProcessorList[i]->GetRun() != nullptr) {
			temp = ProcessorList[i]->GetRun();
			if (temp->getID() == IDKill) {
				ProcessorList[i]->RemoveProcess(IDKill,&temp);
				KilledProcess++;
				SendToTRM(temp);
				return true;
			}
		}
		if (ProcessorList[i]->FindProcessByID(IDKill,temp)) {
			ProcessorList[i]->RemoveProcess(IDKill,&temp);
			KilledProcess++;
			SendToTRM(temp);
			return true;
		}
	}
	return false;
}

//Kill Children with no parent
void Scheduler::KillOrphans(Process* TRMParent) {
	Process* tempR;
	Process* tempL;
	if (TRMParent != nullptr) {
		tempL = TRMParent->getLChild();
		if (tempL != nullptr) {
			KillProcess(tempL->getID());
		}
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
	KillOrphans(temp);
	temp->setTT(SystemTime);
	if (temp->getisKilled())
		temp->setWT(temp->getTRT() - (temp->getCT() - temp->getWorkingTime()));
	SUM_TRT += temp->getTRT();
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

//Manipulating ProcessNumbers
void Scheduler::IncrementProcessNum() { PROCESS_NUM++; }
void Scheduler::DecrementProcessNum() { PROCESS_NUM--; }

//Manipulating SystemTime
void Scheduler::IncrementSystemTime() { SystemTime++; }
void Scheduler::DecrementSystemTime() { SystemTime--; }

//Processing functions
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
void Scheduler::Processing(bool mode) {
	//Check for kill signals before processing
	while (KillSignalProcessing());
	//Schedule Processes arriving at current timestep
	while (ScheduleNewlyArrived());
	//Check running processes
	for (int i = 0; i < PROCESSOR_NUM; i++)
		ProcessorList[i]->tick();
	//Processing of IO
	BLKProcessing();
	if (mode)
		PrintSystemInfo();
	SystemTime++;
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

void Scheduler::ProcessMigration(Process* p,bool x) {
	if (x) {
		RRMigration++;
		ScheduleToShortestRR(p);
	}
	else {
		SJFMigration++;
		ScheduleToShortestSJF(p);
	}
}

void Scheduler::Phase1Processing() {
	//Simple simulator function for Phase1
	while (ScheduleNewlyArrivedPhase1());
	for (int i = 0; i < PROCESSOR_NUM; i++) {
		ProcessorList[i]->MoveToRun(RunningProcessesSum, SystemTime);
		if (ProcessorList[i]->GetRun() != nullptr)
		{
			Process* temp;
			int random = 1 + rand() % 100;
			if (random >= 1 && random <= 15) {
				temp = ProcessorList[i]->GetRun();
				ProcessorList[i]->RemoveRun();
				RunningProcessesSum--;
				SendToBLK(temp);
			}
			if (random >= 20 && random <= 30) {
				temp = ProcessorList[i]->GetRun();
				ProcessorList[i]->RemoveRun();
				RunningProcessesSum--;
				ScheduleByLeastCount(temp);
			}
			if (random >= 50 && random <= 60) {
				temp = ProcessorList[i]->GetRun();
				ProcessorList[i]->RemoveRun();
				RunningProcessesSum--;
				SendToTRM(temp);
			}
		}
	}
	BLKProcessing();
	RemoveRandomProcessPhase1();
	PrintSystemInfo();
	SystemTime++;
}
void Scheduler::BLKProcessingPhase1() {
	//BLK Processing using random numbers for Phase1
	Process* temp;
	temp = BLK.Peek();
	int random;
	random = 1 + rand() % 100;
	if (temp) {
		if (random < 10) {
			BLK.Dequeue(&temp);
			ScheduleByLeastCount(temp);
		}
	}
}



//Termination condition
bool Scheduler::Terminate() {
	if (TRM.getCount() == PROCESS_NUM)
		return true;
	return false;
}

//Removing random functions from ready for Phase1
void Scheduler::RemoveRandomProcessPhase1() {
	int random;
	Process* temp=nullptr;
	for (int i = 0; i < FCFS_NUM; i++) {
		if (ProcessorList[i]->getNumOfProcesses() != 0&& ProcessorList[i]->getNumOfProcesses()!=1) {
			while (1) {
				random = 1 + rand() % PROCESS_NUM;
				if (ProcessorList[i]->RemoveProcess(random, &temp)){
					SendToTRM(temp);
					break;
				}
			}
		}
	}
}

//Print Termination list
void Scheduler::PrintTRM() {
	TRM.Print();
}

//Increase the Num of processes in run state of processors
void Scheduler::incrementRunningProcessCount() {
	RunningProcessesSum++;
}

//Print all System information
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
	wind.PrintRunBase(RunningProcessesSum);
	for (int i = 0; i < PROCESSOR_NUM; i++) {
		Process* temp=ProcessorList[i]->GetRun();
		if (temp!=nullptr) {
			wind.printRunloop(i + 1, temp);
		}
	}
	wind.printTRM(TRM, TRM.getCount());
}

void PrintProcesses(Process* p,FILE* outputFile){
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

void Scheduler::OutputFile() {
	FILE* outputFile;
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
		float RRMig = (RRMigration / PROCESS_NUM) * 100;
		float SJFMig = (SJFMigration / PROCESS_NUM) * 100;
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
		fprintf(outputFile, "Processes: %d [%d FCFS, %d SJF, %d RR, %d EDF]\n",PROCESSOR_NUM, FCFS_NUM, SJF_NUM, RR_NUM, EDF_NUM);
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

int Scheduler::GetTotalIdleBusy(){
	int sum = 0;
	return sum;
}

int Scheduler::getRunningProcess() { return RunningProcessesSum; }

Scheduler::~Scheduler() {
	Process** temp = new Process * [PROCESS_NUM];
	for (int i = 0; i < PROCESS_NUM; i++) {
		TRM.Dequeue(&temp[i]);
		delete temp[i];
	}
	delete[]temp;
	for (int i = 0; i < PROCESSOR_NUM; i++) {
		delete ProcessorList[i];
	}
	
}