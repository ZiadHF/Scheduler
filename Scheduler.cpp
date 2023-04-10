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
		NEW.enqueue(tempPro);
		
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
		Kill_Process.add(temp3);
	}
	ProccesorList = new Processor[FCFS_NUM + SJF_NUM + RR_NUM];
}

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
}
void Scheduler::AddForkedProcess(Process parent) {
	Process* added = new Process(time, ++PROCESS_NUM, parent.ct, 0);


}