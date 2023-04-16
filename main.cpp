#include"Scheduler.h"
#include"UI.h"
int main() {
	Scheduler Test;
	UI cmd;
	Test.LoadFromFile(cmd.GetFileName());
	while (!Test.Terminate()) {
	
	
	}
}