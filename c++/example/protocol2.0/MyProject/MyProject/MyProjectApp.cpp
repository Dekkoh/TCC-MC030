#include "MyProjectApp.h"
#include "tccFrm.h"
#include "dynamixel_sdk.h"
#include "motorController.h"
#include "imu.h"

#include <windows.h>
#include <boost/lexical_cast/lexical_cast_old.hpp>
#include <string>
#include <iostream>
#include <stdlib.h>

#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>


#ifdef _DEBUG
#pragma comment(lib, "wxbase30ud.lib")
#else
#pragma comment(lib, "wxbase30u.lib")
#endif

void *PrintHello(void *threadid) {
	long tid;
	tid = (long)threadid;
	//cout << "Hello World! Thread ID, " << tid << endl;

	pthread_exit(NULL);

	return NULL;
}

bool MyProjectApp::OnInit()
{
	tccFrm* frame = new tccFrm(NULL);
	SetTopWindow(frame);
	frame->Show();

	/*int i = 0;
	double data[17] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

	initUM7("COM6", 115200);

	while (i < 200) {
		getData(data);
		if (i % 50 == 0) {
			int j;
			for (j = 0; j < 17; j++) {
				std::string s = boost::lexical_cast<std::string>(data[j]);
				std::wstring stemp = std::wstring(s.begin(), s.end());
				LPCWSTR sw = stemp.c_str();
				OutputDebugString(sw);
				std::string lb = "\n";
				std::wstring stemp2 = std::wstring(lb.begin(), lb.end());
				LPCWSTR sw2 = stemp2.c_str();
				OutputDebugString(sw2);
			}
		}

		i++;
	}*/

	//terminateUM7("COM6");


	//initialize("COM5");
	//ping(1, "COM5");
	//set_torque(1, "COM5", 1);
	//move_to(1, "COM5", 0);
	//move_to(1, "COM5", 4095);
	//set_torque(1, "COM5", 0);
	//terminate("COM5");

	return true;
}

int MyProjectApp::OnExit()
{
	return 0;
}

wxIMPLEMENT_APP(MyProjectApp);