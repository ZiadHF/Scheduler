#include"Scheduler.h"

int main() {
	Scheduler Test;
	Test.LoadFromFile("test.txt");
	cout << Test.FCFS_NUM << endl;
	cout << Test.SJF_NUM << endl;
	cout << Test.RR_NUM << endl;
	cout << Test.RR_TS << endl;
	cout << Test.RTF << endl;
	cout << Test.MaxW << endl;
	cout << Test.STL << endl;
	cout << Test.ForkProb << endl;
	cout << Test.PROCESS_NUM << endl;
}