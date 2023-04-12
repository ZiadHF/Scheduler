#pragma once
#include "Scheduler.h"
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
			k = 0;
			c = 0;
			int processornumbers[3];
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
			break;
		case(2):
			RR_TS = stoi(temp);
			break;
		case(3):
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
			PROCESS_NUM = stoi(temp);
			break;
		}
	}
	//Looping on N Process lines
	for (int i = 0; i < PROCESS_NUM; i++) {
		getline(read, temp);
		int k = 0;
		int c = 0;
		int Process_Data[4];
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
		Process* tempPro=new Process(Process_Data[0], Process_Data[1], Process_Data[2], Process_Data[3],IOArr);
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
	//Creating the list of available processes
	ProcessorList = new Processor*[FCFS_NUM + SJF_NUM + RR_NUM];
	for (int i = 0; i < FCFS_NUM + SJF_NUM + RR_NUM; i++) {
		if (i > 0 && i < FCFS_NUM) {
			ProcessorList[i] = new FCFS(ForkProb);
		}
		if (i >= FCFS_NUM && i < PROCESS_NUM - RR_NUM) {
			ProcessorList[i] = new RR(RR_TS);
		}
		if (i >= FCFS_NUM + SJF_NUM && i < RR_NUM) {
			ProcessorList[i] = new RR(RR_TS);
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
	PROCESS_NUM = 0;
	ProcessorList = nullptr;
	SystemTime = 0;
}

//Creating the forked processes
void Scheduler::AddForkedProcess(Process* parent) {
	Process* added = new Process(SystemTime, ++PROCESS_NUM, parent->getWorkingTime(),0);
}

//Schedule newly arrived process
bool Scheduler::ScheduleNewlyArrived() {
	Process* temp;
	temp = NEW.Peek();
	if (temp->getAT() == SystemTime) {
		ScheduleByLeastCount(temp);
		return true;
	}
	return false;

}

//Schedules processes in the shortest FCFS RDY Queue
void Scheduler::ScheduleToShortestFCFS(Process* added) {
	int index;
	int shortest = -1;
	for (int i = 0; i < FCFS_NUM; i++) {
		if (ProcessorList[i]->getTotalTime() > shortest) {
			index = i;
		}
	}
	ProcessorList[index]->AddtoRDY(added);
}

//Schedules processes in the shortest SJF RDY Queue
void Scheduler::ScheduleToShortestSJF(Process* added) {
	int index;
	int shortest = -1;
	for (int i = FCFS_NUM; i < PROCESS_NUM-RR_NUM; i++) {
		if (ProcessorList[i]->getTotalTime() > shortest) {
			index = i;
		}
	}
	ProcessorList[index]->AddtoRDY(added);
}

//Schedules processes in the Shortest RR RDY Queue
void Scheduler::ScheduleToShortestRR(Process* added) {
	int index=-1;
	int shortest = -1;
	for (int i = SJF_NUM+FCFS_NUM; i < PROCESS_NUM; i++) {
		if (ProcessorList[i]->getTotalTime() > shortest) {
			index = i;
		}
	}
	ProcessorList[index]->AddtoRDY(added);
}

//Schedules processes in the shortest RDY Queue
void Scheduler::ScheduleToShortest(Process* added) {
	int index=-1;
	int shortest = -1;
	for (int i = 0; i < PROCESS_NUM; i++) {
		if (ProcessorList[i]->getTotalTime() > shortest) {
			index = i;
		}
	}
	ProcessorList[index]->AddtoRDY(added);
}

//Schedule according to process count in RDY Queue(Phase 1 Function)
void Scheduler::ScheduleByLeastCount(Process* added) {
	int index=-1;
	int least = -1;
	for (int i = 0; i < PROCESS_NUM; i++) {
		if (ProcessorList[i]->getNumOfProcesses() > least) {
			index = i;
		}
	}
	ProcessorList[index]->AddtoRDY(added);
}

//Kills a process with a kill signal
bool Scheduler::KillProcess(int IDKill) {
	Process* temp=nullptr;
	for (int i = 0; i < FCFS_NUM; i++) {
		if (ProcessorList[i]->FindProcessByID(IDKill,temp)) {
			ProcessorList[i]->RemoveProcess(IDKill,temp);
			temp->setTT(SystemTime);
			TRM.Enqueue(temp);
			KillOrphans(temp);
			return true;
		}
	}
	return false;
}

//Kill Children with no parent
void Scheduler::KillOrphans(Process* TRMParent) {
	Process* temp;
	temp = TRMParent->getChild();
	while (temp) {
		KillProcess(temp->getID());
		temp = temp->getChild();
	}
}

//Add to IO request list (BLK)
void Scheduler::SendToBLK(Process* temp) { BLK.Enqueue(temp); }

//Add to the termination list(TRM)
void Scheduler::SendToTRM(Process* temp) { TRM.Enqueue(temp); }

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
void Scheduler::BLKProcessing() {
	Process* temp = BLK.Peek();
	if (!(temp->DecrementRemIOTime()))
		ScheduleByLeastCount(temp);
}