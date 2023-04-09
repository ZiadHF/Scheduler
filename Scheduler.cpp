#include "Scheduler.h"
void Scheduler::LoadFromFile(string file) {
	ifstream read(file);
	string temp;
	getline(read, temp);
	int k=0;
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
}