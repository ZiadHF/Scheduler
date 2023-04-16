#include"Scheduler.h"
#include"UI.h"
int main() {
	Scheduler Test;
	UI cmd;
	//cmd.GetFileName()
	Test.LoadFromFile("test.txt");
	while (!Test.Terminate()) {
		Test.Phase1Processing();
	}
}