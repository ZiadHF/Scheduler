#include "Scheduler.h"
void Scheduler::LoadFromFile(string file) {
	ifstream read;
	read.open(file);	
	string temp;
	//Looping on 1st 4 lines getting initial system values
	for (int i = 1; i < 5; i++) {
		getline(read, temp);
		switch (i)
		{
		case(1):
			int k = 0;
			int c = 0;
			int processornumbers[3];
			while (temp[k] != '\0') {
				string temp2;
				while (temp[k] != ' ') {
					temp2 = temp[k];
					processornumbers[c] = stoi(temp2);
					c++;
					k++;
				}
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
			int k = 0;
			int c = 0;
			int misc[4];
			while (temp[k] != '\0') {
				string temp2;
				while (temp[k] != ' ') {
					temp2 = temp[k];
					misc[c] = stoi(temp2);
					c++;
					k++;
				}
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
			while (temp[k] != ' ') {
				temp2 = temp[k];
				Process_Data[c] = stoi(temp2);
				c++;
				k++;
			}
			k++;
		} 
		//Handle the io requests of the process if there are any.
		if (Process_Data[3] != 0) {
			int* IO_R = new int[Process_Data[3]];
			int* IO_D = new int[Process_Data[3]];
			for (int j = 0; j < Process_Data[3]; j++) {
				k++;
				string temp2;
				while (temp[k] != ',') {
					temp2 += temp[k];
					k++;
				}
				temp2 = temp[k];
				IO_R[j] = stoi(temp2);
				k++;
				while (temp[k] != ')') {
					temp2 += temp[k];
					k++;
				}
				temp2 = temp[k];
				IO_D[j] = stoi(temp2);
				if (j != 4)
				k+=2;
			}
			Process temp(Process_Data[0], Process_Data[1], Process_Data[2], Process_Data[3], IO_R, IO_D);
			NEW.enqueue(temp);
		}
	}
	//Looping on Kill Signals
	while (getline(read, temp)) {
		int k = 0;
		int c = 0;
		int misc[2];
		while (temp[k] != '\0') {
			string temp2;
			while (temp[k] != ' ') {
				temp2 = temp[k];
				misc[c] = stoi(temp2);
				c++;
				k++;
			}
			k++;
		}
		SIGKILL temp3;
		temp3.Kill_PID = misc[1];
		temp3.Kill_Time = misc[0];
		KILL_Process.add(temp3);
	}
}