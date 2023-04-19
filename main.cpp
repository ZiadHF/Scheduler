#include"Scheduler.h"
#include"UI.h"
#include<cstdlib>
#include<ctime>
int main() {
	srand(time(0));
	//cout << time(0);
	Scheduler Test;
	UI cmd;
	Test.LoadFromFile(cmd.GetFileName());
	while (!Test.Terminate()) {
 		Test.Phase1Processing();
	cmd.nextTS();
	system("cls");
	}
	cout << Test.GetSystemTime();
}