#include "CUtils.h"
#include "CApplicationWindow.h"



int main(int argc, char* argv[])
{
	if (!CApplicationWindow::GetInstance().Initialize()) {
		PRINTLOG("Fail to initialize CApplicationWindow");
		return 1;
	}
	CApplicationWindow::GetInstance().Run();
	CApplicationWindow::GetInstance().UnInitialize();
	return 0;
}