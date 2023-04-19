#include "UI.h"
#include <cstdio>
UI::UI(int timestep) {
	cout<<"----------------------------------------\n";
	cout << "Current Timestep:" << timestep << endl;
	cout << "-----------------     RDY processes     -----------------" << endl;}
UI::UI() {}

string UI::GetFileName() {
	string temp;
	cout << "Enter the filename you want to run followed by .txt\n";
	cin >> temp;
	return temp;
}
/*
ostream& operator << (ostream& output, Process*& obj) {
	output << obj.getID();
	return output;
}
*/
ostream& operator << (ostream& output, FCFS*& obj) {
	output << obj->getlist().getCount() << " " << "RDY" << ": ";
	obj->getlist().Print();
	return output;
}
ostream& operator << (ostream& output, RR*& obj) {
	output << obj->getlist().getCount() << " " << "RDY" << ": ";
	obj->getlist().Print();
	return output;
}
ostream& operator << (ostream& output, SJF*& obj) {
	output << obj->getlist().getSize() << " " << "RDY" << ": ";
	obj->getlist().Print();
	return output;
}
void UI::printFCFSProcessorInfo(FCFS* obj, int ProccessorID) {
	cout << "processor " << ProccessorID << "[FCFS]: " << obj << endl;
}
void UI::printRRProcessorInfo(RR* obj, int ProccessorID) {
	cout << "processor " << ProccessorID << "[RR]: " << obj << endl;
}
void UI::printSJFProcessorInfo(SJF* obj, int ProccessorID) {
	cout << "processor " << ProccessorID <<	"[SJF]: " << obj << endl;
}
void UI::nextTS() {
	cout << "Press Enter to continue." << endl;
cin.get();
return ;
}
void UI::printBLK(Queue<Process*>& blk,int numOfBlkProcesses) {
	cout << "-----------------     BLK processes     -----------------" << endl;
	cout << numOfBlkProcesses << " BLK: ";
	blk.Print();
}
void UI::printTRM(Queue<Process*>& trm, int numOfTRMProcesses) {
	cout << "\n";
	cout << "-----------------     TRM processes     -----------------" << endl;
	cout << numOfTRMProcesses << " TRM: ";
	trm.Print();
	cout << "\n";
}
void UI::PrintRunBase(int Numofcurrentproccesses) {
	cout <<"\n" << "-----------------     Run processes     -----------------" << endl;
	cout << Numofcurrentproccesses << " RUN: ";
}
void UI::printRunloop(int processorID, Process* running) {
	cout << running << "(P" << processorID << ") ";
	
}