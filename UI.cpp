#include "UI.h"
UI::UI() {}

string UI::GetFileName() {
	string temp;
	cout << "Enter the filename you want to run followed by .txt\n";
	cin >> temp;
	return temp;
}