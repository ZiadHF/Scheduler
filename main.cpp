#include"Scheduler.h"
#include"UI.h"
#include<cstdlib>
#include<ctime>
int main() {
	srand(1681857245);
	//cout << time(0);
	Scheduler Test;
	UI cmd;
	//cmd.GetFileName()
	Test.LoadFromFile("test.txt");
	while (!Test.Terminate()) {
 		Test.Phase1Processing();
	}
	Test.PrintTRM();
	cout << Test.GetSystemTime();
	
}