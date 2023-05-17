#include"Scheduler.h"
#include"UI.h"
#include<cstdlib>
#include<ctime>
#include <Windows.h>

int main() {

	srand(time(0));
	Scheduler Test;
	UI cmd;
	string s;
	try {
		s = cmd.GetFileName();
		if(!Test.LoadFromFile(s))
			throw int(5);
		
	}
	catch (int z) {
		cout << "No Processors or Processes loaded";
		return 5;
	}
	catch (const exception& e) {
		cout << "An exception occured: " << e.what() << endl;
		return 2;
	}
	int x;
	try {
		cout << "Please Select a mode (1 for Interactive, 2 for StepByStep, 3 for Silent): ";
		cin >> x;

		if (x != 1 && x != 2 && x != 3) {
			throw runtime_error("Invalid input.");
		}
	}
	catch (const exception& e) {
		cout << "An exception occurred: " << e.what() << endl;
		return 1;
	}
	while (!Test.Terminate()) {
		system("cls");
		switch (x) {
			case 1:
				Test.Processing(true);
				cmd.nextTS();
				break;
			case 2:
				Test.Processing(true);
				Sleep(1000);
				break;
			default:
				Test.Processing(false);
				break;
		}
	}
	if (x == 3)
		cout << "The simulation has ended." << endl << "Output file created." << endl << "The simulation took " << Test.GetSystemTime() << " timesteps.";
	Test.OutputFile();
}