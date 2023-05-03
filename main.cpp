#include"Scheduler.h"
#include"UI.h"
#include<cstdlib>
#include<ctime>
int main() {
	srand(time(0));
	Scheduler Test;
	UI cmd;
	//cmd.GetFileName()
	Test.LoadFromFile("test.txt");
	while (!Test.Terminate()) {
		system("cls");
		Test.Processing();
		//cmd.nextTS();
	}
	Test.OutputFile();
}