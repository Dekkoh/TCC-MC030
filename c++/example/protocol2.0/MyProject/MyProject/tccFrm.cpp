#include "tccFrm.h"

//Do not add custom headers between
//Header Include Start and Header Include End
//wxDev-C++ designer will remove them
////Header Include Start
////Header Include End
#include "MovementsTableViewer.h"
#include "ValuesTableViewer.h"
#include "SensorTableViewer.h"
#include "motorController.h"
#include "imu.h"
#include <ctime>
#include <cmath>
//----------------------------------------------------------------------------
// tccFrm
//----------------------------------------------------------------------------
//Add Custom Events only in the appropriate block.
//Code added in other places will be removed by wxDev-C++
////Event Table Start
BEGIN_EVENT_TABLE(tccFrm, wxFrame)
////Manual Code Start
////Manual Code End

EVT_CLOSE(tccFrm::OnClose)
EVT_BUTTON(ID_DELETEBUTTON, tccFrm::deleteButtonClick)
EVT_BUTTON(ID_DOWNBUTTON, tccFrm::downButtonClick)
EVT_BUTTON(ID_UPBUTTON, tccFrm::upButtonClick)
EVT_BUTTON(ID_PLAYBUTTON, tccFrm::playButtonClick)
EVT_BUTTON(ID_CONNECTBUTTON, tccFrm::connectButtonClick)
EVT_BUTTON(ID_SAVEBUTTON, tccFrm::saveButtonClick)
EVT_BUTTON(ID_LOADBUTTON, tccFrm::loadButtonClick)
EVT_BUTTON(ID_ADDBUTTON, tccFrm::addButtonClick)
END_EVENT_TABLE()
////Event Table End
wxListCtrl *valueTable;
wxListCtrl *sensorTable;
wxCheckBox *valueCheckBox;
wxCheckBox *sensorCheckBox;
std::vector<std::string> motorPorts;
std::vector<int> motorIdx;
std::vector<bool> activeMotor;


tccFrm::tccFrm(wxWindow *parent, wxWindowID id, const wxString &title, const wxPoint &position, const wxSize& size, long style)
	: wxFrame(parent, id, title, position, size, style)
{
	CreateGUIControls();
}

tccFrm::~tccFrm()
{
}

struct motStruct {
	unsigned idx;
	int index;
	std::string portName;
};

void saveFile(string filePath, std::string write) {
	wxTextFile output_stream(filePath);
	output_stream.Open();
	wxString movem(write);
	output_stream.AddLine(movem);
	output_stream.Write();
	output_stream.Close();
}

void *tccFrm::moveThread(void *motorStruct)
{
	for (int i = 0; i < AllMovements::getInstance()->size(); i++) {
		Movement * my_data = AllMovements::getInstance()->get(i);
		wxString moves = my_data->move;
		int time = my_data->time;
		bool tq = my_data->torque;
		std::string cstring = std::string(moves.mb_str());

		std::stringstream move(cstring);
		std::string mv;
		std::vector<std::string> allMoves;

		while (std::getline(move, mv, ';')) {
			allMoves.push_back(mv);
		}
		int time2 = 0;
		for (unsigned j = 0; j < allMoves.size(); j++) {
			std::stringstream move1(allMoves[j]);
			std::string mv1;
			std::vector<std::string> allMoves1;

			while (std::getline(move1, mv1, '=')) {
				allMoves1.push_back(mv1);
			}
			int idx = atoi(allMoves1[0].c_str());
			if (!activeMotor[idx - 1]) {
				continue;
			}
			int finalPos = atoi(allMoves1[1].c_str());
			Values * v = AllValues::getInstance()->get(idx - 1);
			std::string ini = std::string(v->position.mb_str());
			int initialPos = atoi(ini.c_str());
			float velocity = (((fabs((int)(finalPos - initialPos))) / time) * 64);
			// Code to send to motors
			// idx,pos,torque,time


			if (velocity > 509) {
				velocity = 509;
				float time3 = (((fabs((int)(finalPos - initialPos))) / velocity) *64);
				if (time3 > time2) {
					time2 = time3;
				}
			}

			std::string s = std::to_string(time2);
			std::wstring stemp = std::wstring(s.begin(), s.end());
			LPCWSTR sw = stemp.c_str();
			OutputDebugString(sw);
			std::string s2 = "\n";
			std::wstring stemp2 = std::wstring(s2.begin(), s2.end());
			LPCWSTR sw2 = stemp2.c_str();
			OutputDebugString(sw2);


			set_torque(motorIdx[idx - 1], motorPorts[idx - 1], 1);
			set_profile_velocity(motorIdx[idx - 1], motorPorts[idx - 1], (int)velocity);
			move_to(motorIdx[idx - 1], motorPorts[idx - 1], finalPos);
			//set_torque(motorIdx[idx - 1], motorPorts[idx - 1], tq ? 1 : 0);
		}
		if (time2 > time) {
			time = time2;
		}

		if (time < 500) {
			time = time * 2;
		} else {
			time += 500;
		}

		Sleep(time);

	}
	return NULL;
}

void *tccFrm::readValue(void *motorStruct)
{
	int my_data;
	int time = 0;
	my_data = (int ) motorStruct;
	while (true) {
		// TODO : fazer chamada para motor

		//std::string s = std::to_string();
		int pos = get_present_position(motorIdx[my_data], motorPorts[my_data]);
		int cur = get_present_current(motorIdx[my_data], motorPorts[my_data]);
		int vel = get_present_velocity(motorIdx[my_data], motorPorts[my_data]);
		int tor = get_present_torque(motorIdx[my_data], motorPorts[my_data]);
		int ten = get_input_voltage(motorIdx[my_data], motorPorts[my_data]);
		Values * v = AllValues::getInstance()->get(my_data);
		if (pos >= 0 && pos < 4096) {
			v->position = wxString(std::to_string(pos));
			v->velocity = wxString(std::to_string(vel));
			v->torque = tor == 0 ? false : true;
			v->current = wxString(std::to_string(cur));
			v->tensao = wxString(std::to_string(ten));
			if (valueCheckBox->GetValue()) {
				std::string sv = std::string("time") + std::to_string(time) + std::string("Motor ") + std::to_string(my_data)
					+ std::string(": position=") + std::string(v->position.mb_str())
					+ std::string(": velocity=") + std::string(v->velocity.mb_str())
					+ std::string(": torque=") + std::string(v->torque ? "true" : "false")
					+ std::string(": current=") + std::string(v->current.mb_str());
				saveFile("valueOutput.txt", sv);
			}
		}
		valueTable->Refresh();
		Sleep(100);
		time += 100;
	}
	return NULL;
}

void *tccFrm::sensorThread(void *motorStruct)
{
	int time = 0;
	double data[17] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	while (true) {
		getData(data);
		Sensor * s = AllSensor::getInstance()->get(0);
		s->accelX = data[5];
		s->accelY = data[6];
		s->accelZ = data[7];
		s->gravX = data[2];
		s->gravY = data[3];
		s->gravZ = data[4];
		s->magX = data[8];
		s->magY = data[9];
		s->magZ = data[10];
		s->quatX = data[11];
		s->quatY = data[12];
		s->quatZ = data[13];
		s->eulerX = data[14];
		s->eulerY = data[15];
		s->eulerZ = data[16];

		if (sensorCheckBox->GetValue()) {
			std::string sv = std::string("time ") + std::to_string(time)
				+ std::string("Sensor AccelX: ") + std::to_string(s->accelX)
				+ std::string(" AccelY: ") + std::to_string(s->accelY)
				+ std::string(" AccelZ: ") + std::to_string(s->accelZ)
				+ std::string(" GyroX: ") + std::to_string(s->gravX)
				+ std::string(" GyroY: ") + std::to_string(s->gravY)
				+ std::string(" GyroZ: ") + std::to_string(s->gravZ)
				+ std::string(" MagX: ") + std::to_string(s->magX)
				+ std::string(" MagY: ") + std::to_string(s->magY)
				+ std::string(" MagZ: ") + std::to_string(s->magZ)
				+ std::string(" QuatX: ") + std::to_string(s->quatX)
				+ std::string(" QuatY: ") + std::to_string(s->quatY)
				+ std::string(" QuatZ: ") + std::to_string(s->quatZ)
				+ std::string(" EulerX: ") + std::to_string(s->eulerX)
				+ std::string(" EulerY: ") + std::to_string(s->eulerY)
				+ std::string(" EulerZ: ") + std::to_string(s->eulerZ);
			saveFile("sensorOutput.txt", sv);
		}
		sensorTable->Refresh();
		sensorTable->SetItemCount(AllSensor::getInstance()->size());
		time += 100;
	}
	return NULL;
}

void tccFrm::CreateGUIControls()
{

	wxTextFile input_stream("motor.cfg");
	input_stream.Open();
	wxString str;
	for (int i = 0; i < 25; i++) {
		activeMotor.push_back(false);
		std::string aux("Motor");
		nameMotor.push_back(aux);
	}

	wxTextFile output_stream("sensorOutput.txt");
	output_stream.Open();
	output_stream.Clear();
	output_stream.Close();

	wxTextFile output_stream2("valueOutput.txt");
	output_stream2.Open();
	output_stream2.Clear();
	output_stream2.Close();

	if (input_stream.Exists()) {
		str = input_stream.GetFirstLine();
		while (str.compare("") == 0) {
			str = input_stream.GetNextLine();
		}

		std::string sensorLine = std::string(str.mb_str());

		std::stringstream sen(sensorLine);
		std::string segmentSens;
		std::vector<std::string> seglistSen;
		while (std::getline(sen, segmentSens, '=')) {
			seglistSen.push_back(segmentSens);

		}
		initUM7(seglistSen[1], 115200);

		str = input_stream.GetNextLine();
		while (str.compare("") == 0) {
			str = input_stream.GetNextLine();
		}

		std::string number = std::string(str.mb_str());

		std::stringstream test(number);
		std::string segment;
		std::vector<std::string> seglist;
		while (std::getline(test, segment, '=')) {
			seglist.push_back(segment);
		}
		int nPorts = atoi(seglist[1].c_str());
		for (int i = 0; i < nPorts; i++) {
			str = input_stream.GetNextLine();
			while (str.compare("") == 0) {
				str = input_stream.GetNextLine();
			}
			std::string port = std::string(str.mb_str());
			std::stringstream port_stream(port);
			std::string seg;
			std::vector<std::string> segmentList;
			while (std::getline(port_stream, seg, '=')) {
				segmentList.push_back(seg);
			}
			ports.push_back(segmentList[1]);
			initialize(segmentList[1]);

		}

		int i = 0;
		for (str = input_stream.GetNextLine(); !input_stream.Eof(); str = input_stream.GetNextLine()) {
			if (str.compare("") != 0) {
				std::string idMotor = std::string(str.mb_str());
				std::stringstream idLabel(idMotor);
				std::string id;
				std::vector<std::string> idList;
				while (std::getline(idLabel, id, '=')) {
					idList.push_back(id);
				}
				motorIdx.push_back(atoi(idList[1].c_str()));

				str = input_stream.GetNextLine();
				while (str.compare("") == 0) {
					str = input_stream.GetNextLine();
				}

				std::string labelMotor = std::string(str.mb_str());
				std::stringstream labelName(labelMotor);
				std::string lb;
				std::vector<std::string> labelList;
				while (std::getline(labelName, lb, '=')) {
					labelList.push_back(lb);
				}
				nameMotor[i] = labelList[1];
				i++;
			}
		}
	}

	//pthread_create(&tCamera, NULL, &tccFrm::camera, NULL);

	//Do not add custom code between
	//GUI Items Creation Start and GUI Items Creation End
	//wxDev-C++ designer will remove them.
	//Add the custom code before or after the blocks
	////GUI Items Creation Start
	sensorCheckBox = new wxCheckBox(this, ID_SENSORCHECKBOX, _("Save Sensor Values"), wxPoint(930, 17), wxSize(137, 17), 0, wxDefaultValidator, _("sensorCheckBox"));

	valueCheckBox = new wxCheckBox(this, ID_VALUECHECKBOX, _("Save Motor Values"), wxPoint(699, 14), wxSize(121, 17), 0, wxDefaultValidator, _("valueCheckBox"));

	maxPosition1 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 55), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));

	minPosition1 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 55), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));

	MaxPosition = new wxStaticText(this, ID_MAXPOSITION, _("Max"), wxPoint(293, 33), wxDefaultSize, 0, _("MaxPosition"));

	MinPosition = new wxStaticText(this, ID_MINPOSITION, _("Min"), wxPoint(236, 33), wxDefaultSize, 0, _("MinPosition"));

	deleteButton = new wxButton(this, ID_DELETEBUTTON, _("DELETE"), wxPoint(575, 210), wxSize(48, 25), 0, wxDefaultValidator, _("deleteButton"));

	downButton = new wxButton(this, ID_DOWNBUTTON, _("DOWN"), wxPoint(575, 180), wxSize(48, 25), 0, wxDefaultValidator, _("downButton"));

	upButton = new wxButton(this, ID_UPBUTTON, _("UP"), wxPoint(575, 150), wxSize(48, 25), 0, wxDefaultValidator, _("upButton"));

	valueTable = new ValuesTableViewer(this, ID_VALUETABLE);

	motorSlider25 = new wxSlider(this, ID_MOTORSLIDER25, 0, 0, 4095, wxPoint(80, 535), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider25"));
	motorSlider25->SetRange(0, 4095);
	motorSlider25->SetValue(0);

	motorSlider24 = new wxSlider(this, ID_MOTORSLIDER24, 0, 0, 4095, wxPoint(80, 515), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider24"));
	motorSlider24->SetRange(0, 4095);
	motorSlider24->SetValue(0);

	motorSlider23 = new wxSlider(this, ID_MOTORSLIDER23, 0, 0, 4095, wxPoint(80, 495), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider23"));
	motorSlider23->SetRange(0, 4095);
	motorSlider23->SetValue(0);

	motorSlider22 = new wxSlider(this, ID_MOTORSLIDER22, 0, 0, 4095, wxPoint(80, 475), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider22"));
	motorSlider22->SetRange(0, 4095);
	motorSlider22->SetValue(0);

	motorSlider21 = new wxSlider(this, ID_MOTORSLIDER21, 0, 0, 4095, wxPoint(80, 455), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider21"));
	motorSlider21->SetRange(0, 4095);
	motorSlider21->SetValue(0);

	motorSlider20 = new wxSlider(this, ID_MOTORSLIDER20, 0, 0, 4095, wxPoint(80, 435), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider20"));
	motorSlider20->SetRange(0, 4095);
	motorSlider20->SetValue(0);

	motorSlider19 = new wxSlider(this, ID_MOTORSLIDER19, 0, 0, 180, wxPoint(80, 415), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider19"));
	motorSlider19->SetRange(0, 180);
	motorSlider19->SetValue(0);

	motorSlider18 = new wxSlider(this, ID_MOTORSLIDER18, 0, 0, 4095, wxPoint(80, 395), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider18"));
	motorSlider18->SetRange(0, 4095);
	motorSlider18->SetValue(0);

	motorSlider17 = new wxSlider(this, ID_MOTORSLIDER17, 0, 0, 4095, wxPoint(80, 375), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider17"));
	motorSlider17->SetRange(0, 4095);
	motorSlider17->SetValue(0);

	motorSlider16 = new wxSlider(this, ID_MOTORSLIDER16, 0, 0, 4095, wxPoint(80, 355), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider16"));
	motorSlider16->SetRange(0, 4095);
	motorSlider16->SetValue(0);

	motorSlider15 = new wxSlider(this, ID_MOTORSLIDER15, 0, 0, 4095, wxPoint(80, 335), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider15"));
	motorSlider15->SetRange(0, 4095);
	motorSlider15->SetValue(0);

	motorSlider14 = new wxSlider(this, ID_MOTORSLIDER14, 0, 0, 4095, wxPoint(80, 315), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider14"));
	motorSlider14->SetRange(0, 4095);
	motorSlider14->SetValue(0);

	motorSlider13 = new wxSlider(this, ID_MOTORSLIDER13, 0, 0, 4095, wxPoint(80, 295), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider13"));
	motorSlider13->SetRange(0, 4095);
	motorSlider13->SetValue(0);

	motorSlider12 = new wxSlider(this, ID_MOTORSLIDER12, 0, 0, 4095, wxPoint(80, 275), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider12"));
	motorSlider12->SetRange(0, 4095);
	motorSlider12->SetValue(0);

	motorSlider11 = new wxSlider(this, ID_MOTORSLIDER11, 0, 0, 4095, wxPoint(80, 255), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider11"));
	motorSlider11->SetRange(0, 4095);
	motorSlider11->SetValue(0);

	motorSlider10 = new wxSlider(this, ID_MOTORSLIDER10, 0, 0, 4095, wxPoint(80, 235), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider10"));
	motorSlider10->SetRange(0, 4095);
	motorSlider10->SetValue(0);

	motorSlider9 = new wxSlider(this, ID_MOTORSLIDER9, 0, 0, 180, wxPoint(80, 215), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider9"));
	motorSlider9->SetRange(0, 180);
	motorSlider9->SetValue(0);

	motorSlider8 = new wxSlider(this, ID_MOTORSLIDER8, 0, 0, 4095, wxPoint(80, 195), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider8"));
	motorSlider8->SetRange(0, 4095);
	motorSlider8->SetValue(0);

	motorSlider7 = new wxSlider(this, ID_MOTORSLIDER7, 0, 0, 4095, wxPoint(80, 175), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider7"));
	motorSlider7->SetRange(0, 4095);
	motorSlider7->SetValue(0);

	motorSlider6 = new wxSlider(this, ID_MOTORSLIDER6, 0, 0, 4095, wxPoint(80, 155), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider6"));
	motorSlider6->SetRange(0, 4095);
	motorSlider6->SetValue(0);

	motorSlider5 = new wxSlider(this, ID_MOTORSLIDER5, 0, 0, 4095, wxPoint(80, 135), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider5"));
	motorSlider5->SetRange(0, 4095);
	motorSlider5->SetValue(0);

	motorSlider4 = new wxSlider(this, ID_MOTORSLIDER4, 0, 0, 4095, wxPoint(80, 115), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider4"));
	motorSlider4->SetRange(0, 4095);
	motorSlider4->SetValue(0);

	motorSlider3 = new wxSlider(this, ID_MOTORSLIDER3, 0, 0, 4095, wxPoint(80, 95), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider3"));
	motorSlider3->SetRange(0, 4095);
	motorSlider3->SetValue(0);

	motorSlider2 = new wxSlider(this, ID_MOTORSLIDER2, 0, 0, 4095, wxPoint(80, 75), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider2"));
	motorSlider2->SetRange(0, 4095);
	motorSlider2->SetValue(0);

	motorSlider1 = new wxSlider(this, ID_MOTORSLIDER1, 0, 0, 4095, wxPoint(80, 55), wxSize(150, 22), wxSL_HORIZONTAL | wxSL_SELRANGE, wxDefaultValidator, _("motorSlider1"));
	motorSlider1->SetRange(0, 4095);
	motorSlider1->SetValue(0);

	movements = new MovementsTableViewer(this, ID_MOVEMENTS);

	motorCheckBox25 = new wxCheckBox(this, ID_MOTORCHECKBOX25, _(nameMotor[24]), wxPoint(5, 535), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox25"));

	motorCheckBox24 = new wxCheckBox(this, ID_MOTORCHECKBOX24, _(nameMotor[23]), wxPoint(5, 515), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox24"));

	motorCheckBox23 = new wxCheckBox(this, ID_MOTORCHECKBOX23, _(nameMotor[22]), wxPoint(5, 495), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox23"));

	motorCheckBox22 = new wxCheckBox(this, ID_MOTORCHECKBOX22, _(nameMotor[21]), wxPoint(5, 475), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox22"));

	motorCheckBox21 = new wxCheckBox(this, ID_MOTORCHECKBOX21, _(nameMotor[20]), wxPoint(5, 455), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox21"));

	motorCheckBox20 = new wxCheckBox(this, ID_MOTORCHECKBOX20, _(nameMotor[19]), wxPoint(5, 435), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox20"));

	motorCheckBox19 = new wxCheckBox(this, ID_MOTORCHECKBOX19, _(nameMotor[18]), wxPoint(5, 415), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox19"));

	motorCheckBox18 = new wxCheckBox(this, ID_MOTORCHECKBOX18, _(nameMotor[17]), wxPoint(5, 395), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox18"));

	motorCheckBox17 = new wxCheckBox(this, ID_MOTORCHECKBOX17, _(nameMotor[16]), wxPoint(5, 375), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox17"));

	motorCheckBox16 = new wxCheckBox(this, ID_MOTORCHECKBOX16, _(nameMotor[15]), wxPoint(5, 355), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox16"));

	motorCheckBox15 = new wxCheckBox(this, ID_MOTORCHECKBOX15, _(nameMotor[14]), wxPoint(5, 335), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox15"));

	motorCheckBox14 = new wxCheckBox(this, ID_MOTORCHECKBOX14, _(nameMotor[13]), wxPoint(5, 315), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox14"));

	motorCheckBox13 = new wxCheckBox(this, ID_MOTORCHECKBOX13, _(nameMotor[12]), wxPoint(5, 295), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox13"));

	motorCheckBox12 = new wxCheckBox(this, ID_MOTORCHECKBOX12, _(nameMotor[11]), wxPoint(5, 275), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox12"));

	motorCheckBox11 = new wxCheckBox(this, ID_MOTORCHECKBOX11, _(nameMotor[10]), wxPoint(5, 255), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox11"));

	motorCheckBox10 = new wxCheckBox(this, ID_MOTORCHECKBOX10, _(nameMotor[9]), wxPoint(5, 235), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox10"));

	motorCheckBox9 = new wxCheckBox(this, ID_MOTORCHECKBOX9, _(nameMotor[8]), wxPoint(5, 215), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox9"));

	motorCheckBox8 = new wxCheckBox(this, ID_MOTORCHECKBOX8, _(nameMotor[7]), wxPoint(5, 195), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox8"));

	motorCheckBox7 = new wxCheckBox(this, ID_MOTORCHECKBOX7, _(nameMotor[6]), wxPoint(5, 175), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox7"));

	motorCheckBox6 = new wxCheckBox(this, ID_MOTORCHECKBOX6, _(nameMotor[5]), wxPoint(5, 155), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox6"));

	motorCheckBox5 = new wxCheckBox(this, ID_MOTORCHECKBOX5, _(nameMotor[4]), wxPoint(5, 135), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox5"));

	motorCheckBox4 = new wxCheckBox(this, ID_MOTORCHECKBOX4, _(nameMotor[3]), wxPoint(5, 115), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox4"));

	motorCheckBox3 = new wxCheckBox(this, ID_MOTORCHECKBOX3, _(nameMotor[2]), wxPoint(5, 95), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox3"));

	motorCheckBox2 = new wxCheckBox(this, ID_MOTORCHECKBOX2, _(nameMotor[1]), wxPoint(5, 75), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox2"));

	motorCheckBox1 = new wxCheckBox(this, ID_MOTORCHECKBOX1, _(nameMotor[0]), wxPoint(5, 55), wxSize(75, 17), 0, wxDefaultValidator, _("motorCheckBox1"));

	timeText = new wxTextCtrl(this, ID_TIMETEXT, _(""), wxPoint(405, 7), wxSize(70, 19), 0, wxDefaultValidator, _("timeText"));

	staticTimeText = new wxStaticText(this, ID_STATICTIMETEXT, _("us"), wxPoint(480, 5), wxDefaultSize, 0, _("staticTimeText"));

	torqueCheckBox = new wxCheckBox(this, ID_TORQUECHECKBOX, _("Torque ON/OFF"), wxPoint(500, 5), wxSize(109, 17), 0, wxDefaultValidator, _("torqueCheckBox"));

	playButton = new wxButton(this, ID_PLAYBUTTON, _("Play"), wxPoint(325, 5), wxSize(75, 25), 0, wxDefaultValidator, _("playButton"));

	saveButton = new wxButton(this, ID_SAVEBUTTON, _("Save"), wxPoint(165, 5), wxSize(75, 25), 0, wxDefaultValidator, _("saveButton"));

	loadButton = new wxButton(this, ID_LOADBUTTON, _("Load"), wxPoint(85, 5), wxSize(75, 25), 0, wxDefaultValidator, _("loadButton"));

	addButton = new wxButton(this, ID_ADDBUTTON, _("Add"), wxPoint(5, 5), wxSize(75, 25), 0, wxDefaultValidator, _("addButton"));

	SetTitle(_("tcc"));
	SetIcon(wxNullIcon);
	SetSize(0, 0, 1300, 600);
	Center();

	////GUI Items Creation End
	maxPosition2 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 75), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition2 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 75), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));
	maxPosition3 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 95), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition3 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 95), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));
	maxPosition4 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 115), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition4 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 115), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));
	maxPosition5 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 135), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition5 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 135), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));
	maxPosition6 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 155), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition6 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 155), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));
	maxPosition7 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 175), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition7 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 175), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));
	maxPosition8 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 195), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition8 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 195), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));
	maxPosition9 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 215), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition9 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 215), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));
	maxPosition10 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 235), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition10 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 235), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));
	maxPosition11 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 255), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition11 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 255), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));
	maxPosition12 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 275), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition12 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 275), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));
	maxPosition13 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 295), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition13 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 295), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));
	maxPosition14 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 315), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition14 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 315), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));
	maxPosition15 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 335), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition15 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 335), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));
	maxPosition16 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 355), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition16 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 355), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));
	maxPosition17 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 375), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition17 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 375), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));
	maxPosition18 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 395), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition18 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 395), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));
	maxPosition19 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 415), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition19 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 415), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));
	maxPosition20 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 435), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition20 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 435), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));
	maxPosition21 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 455), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition21 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 455), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));
	maxPosition22 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 475), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition22 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 475), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));
	maxPosition23 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 495), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition23 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 495), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));
	maxPosition24 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 515), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition24 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 515), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));
	maxPosition25 = new wxTextCtrl(this, ID_MAXPOSITION1, _(""), wxPoint(280, 535), wxSize(48, 19), 0, wxDefaultValidator, _("maxPosition1"));
	minPosition25 = new wxTextCtrl(this, ID_MINPOSITION1, _(""), wxPoint(226, 535), wxSize(48, 19), 0, wxDefaultValidator, _("minPosition1"));

	minPositions.push_back(minPosition1);
	minPositions.push_back(minPosition2);
	minPositions.push_back(minPosition3);
	minPositions.push_back(minPosition4);
	minPositions.push_back(minPosition5);
	minPositions.push_back(minPosition6);
	minPositions.push_back(minPosition7);
	minPositions.push_back(minPosition8);
	minPositions.push_back(minPosition9);
	minPositions.push_back(minPosition10);
	minPositions.push_back(minPosition11);
	minPositions.push_back(minPosition12);
	minPositions.push_back(minPosition13);
	minPositions.push_back(minPosition14);
	minPositions.push_back(minPosition15);
	minPositions.push_back(minPosition16);
	minPositions.push_back(minPosition17);
	minPositions.push_back(minPosition18);
	minPositions.push_back(minPosition19);
	minPositions.push_back(minPosition20);
	minPositions.push_back(minPosition21);
	minPositions.push_back(minPosition22);
	minPositions.push_back(minPosition23);
	minPositions.push_back(minPosition24);
	minPositions.push_back(minPosition25);

	maxPositions.push_back(maxPosition1);
	maxPositions.push_back(maxPosition2);
	maxPositions.push_back(maxPosition3);
	maxPositions.push_back(maxPosition4);
	maxPositions.push_back(maxPosition5);
	maxPositions.push_back(maxPosition6);
	maxPositions.push_back(maxPosition7);
	maxPositions.push_back(maxPosition8);
	maxPositions.push_back(maxPosition9);
	maxPositions.push_back(maxPosition10);
	maxPositions.push_back(maxPosition11);
	maxPositions.push_back(maxPosition12);
	maxPositions.push_back(maxPosition13);
	maxPositions.push_back(maxPosition14);
	maxPositions.push_back(maxPosition15);
	maxPositions.push_back(maxPosition16);
	maxPositions.push_back(maxPosition17);
	maxPositions.push_back(maxPosition18);
	maxPositions.push_back(maxPosition19);
	maxPositions.push_back(maxPosition20);
	maxPositions.push_back(maxPosition21);
	maxPositions.push_back(maxPosition22);
	maxPositions.push_back(maxPosition23);
	maxPositions.push_back(maxPosition24);
	maxPositions.push_back(maxPosition25);

	motorSliders.push_back(motorSlider1);
	motorSliders.push_back(motorSlider2);
	motorSliders.push_back(motorSlider3);
	motorSliders.push_back(motorSlider4);
	motorSliders.push_back(motorSlider5);
	motorSliders.push_back(motorSlider6);
	motorSliders.push_back(motorSlider7);
	motorSliders.push_back(motorSlider8);
	motorSliders.push_back(motorSlider9);
	motorSliders.push_back(motorSlider10);
	motorSliders.push_back(motorSlider11);
	motorSliders.push_back(motorSlider12);
	motorSliders.push_back(motorSlider13);
	motorSliders.push_back(motorSlider14);
	motorSliders.push_back(motorSlider15);
	motorSliders.push_back(motorSlider16);
	motorSliders.push_back(motorSlider17);
	motorSliders.push_back(motorSlider18);
	motorSliders.push_back(motorSlider19);
	motorSliders.push_back(motorSlider20);
	motorSliders.push_back(motorSlider21);
	motorSliders.push_back(motorSlider22);
	motorSliders.push_back(motorSlider23);
	motorSliders.push_back(motorSlider24);
	motorSliders.push_back(motorSlider25);
	motorCheckBoxs.push_back(motorCheckBox1);
	motorCheckBoxs.push_back(motorCheckBox2);
	motorCheckBoxs.push_back(motorCheckBox3);
	motorCheckBoxs.push_back(motorCheckBox4);
	motorCheckBoxs.push_back(motorCheckBox5);
	motorCheckBoxs.push_back(motorCheckBox6);
	motorCheckBoxs.push_back(motorCheckBox7);
	motorCheckBoxs.push_back(motorCheckBox8);
	motorCheckBoxs.push_back(motorCheckBox9);
	motorCheckBoxs.push_back(motorCheckBox10);
	motorCheckBoxs.push_back(motorCheckBox11);
	motorCheckBoxs.push_back(motorCheckBox12);
	motorCheckBoxs.push_back(motorCheckBox13);
	motorCheckBoxs.push_back(motorCheckBox14);
	motorCheckBoxs.push_back(motorCheckBox15);
	motorCheckBoxs.push_back(motorCheckBox16);
	motorCheckBoxs.push_back(motorCheckBox17);
	motorCheckBoxs.push_back(motorCheckBox18);
	motorCheckBoxs.push_back(motorCheckBox19);
	motorCheckBoxs.push_back(motorCheckBox20);
	motorCheckBoxs.push_back(motorCheckBox21);
	motorCheckBoxs.push_back(motorCheckBox22);
	motorCheckBoxs.push_back(motorCheckBox23);
	motorCheckBoxs.push_back(motorCheckBox24);
	motorCheckBoxs.push_back(motorCheckBox25);

	sensorTable = new SensorTableViewer(this, ID_SENSORTABLE);
	Sensor * s = new Sensor(10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

	AllSensor::getInstance()->add(s);
	sensorTable->SetItemCount(AllSensor::getInstance()->size());

	for (int i = 0; i < 25; i++) {
		Values * m = new Values((i + 1), wxString("0"), wxString("0"), false, wxString("0"), wxString("0"));

		AllValues::getInstance()->add(m);

		if (activeMotor[i]) {
			motorCheckBoxs[i]->SetForegroundColour(wxColour(0, 255, 0));
		}
		else {
			motorCheckBoxs[i]->SetForegroundColour(wxColour(255, 0, 0));
		}
	
		
	}
	valueTable->SetItemCount(AllValues::getInstance()->size());

	for (int i = 0; i < 25; i++) {
		if (i < (int)motorIdx.size()) {
			for (std::string p : ports) {
				uint16_t aux = ping(motorIdx[i], p);
				if (aux != -1) {
					motorPorts.push_back(p);
					struct motStruct td;
					td.idx = i;
					td.index = motorIdx[i];
					td.portName = p;
					activeMotor[i] = true;
					motorCheckBoxs[i]->SetForegroundColour(wxColour(0, 255, 0));
					pthread_create(&th[i], NULL, &tccFrm::readValue, (void *)i);
					break;

				}
			}
		}
	}

	pthread_create(&tSensor, NULL, &tccFrm::sensorThread, NULL);
}

//void *tccFrm::camera(void *aux) {
//	cv::VideoCapture cap;
//	do {
//		cap.open(0);
//	} while (!cap.isOpened());
//	cv::namedWindow("raw", cv::WINDOW_AUTOSIZE);
//
//
//	cv::UMat bgr_frame;
//
//	for (;;)
//	{
//		cap >> bgr_frame;
//		if (bgr_frame.empty()) break;
//
//		cv::imshow("raw", bgr_frame);
//
//		Sleep(100);
//	}
//
//	cap.release();
//
//	return NULL;
//}


void tccFrm::OnClose(wxCloseEvent& event)
{
	// TODO: Chamar terminate
	pthread_exit(NULL);
	for (std::string p : ports) {
		terminate(p);
	}
	Destroy();
}

/*
* addButtonClick
*/
void tccFrm::addButtonClick(wxCommandEvent& event)
{
	// insert your code here
	int d;
	if (timeText->GetValue().compare("") != 0) {
		d = atoi(timeText->GetValue());
		std::string move("");
		for (unsigned i = 0; i < 25; i++) {
			if (motorCheckBoxs[i]->GetValue() && activeMotor[i]) {
				if (move.compare("") != 0) {
					move = move + ";";
				}
				unsigned id = i + 1;
				char numstr[21];
				char numstr2[21];
				float x = motorSliders[i]->GetValue();
				float y0 = 0;
				if (minPositions[i]->GetValue().compare("") != 0) {
					y0 = atoi(minPositions[i]->GetValue());
				}
				float y1 = 4095;
				if (maxPositions[i]->GetValue().compare("") != 0) {
					y1 = atoi(maxPositions[i]->GetValue());
				}
				float y = y0 * (1 - ((x) / (4095))) + y1 * (1 - ((4095 - x) / (4095)));

				move = move + _itoa(id, numstr, 10) + "=" + _itoa(y, numstr2, 10);
			}
		}

		Movement * m = new Movement(wxString(move), d, torqueCheckBox->GetValue());

		AllMovements::getInstance()->add(m);

		movements->SetItemCount(AllMovements::getInstance()->size());
	}
	else {
		MessageBox(NULL, wxString("Time not specified!"), wxString("Error!"), MB_OK);
	}
}

/*
* loadButtonClick
*/
void tccFrm::loadButtonClick(wxCommandEvent& event)
{
	AllMovements::getInstance()->clearList();
	wxFileDialog
		openFileDialog(this, _("Open .txt file"), "", "",
			"TXT files (*.txt)|*.txt", wxFD_OPEN | wxFD_FILE_MUST_EXIST);
	if (openFileDialog.ShowModal() == wxID_CANCEL) {
		return;     // the user changed idea...
	}
	// proceed loading the file chosen by the user;
	// this can be done with e.g. wxWidgets input streams:
	wxTextFile input_stream;
	input_stream.Open(openFileDialog.GetPath());
	wxString str;
	str = input_stream.GetFirstLine();
	std::string allMoves = std::string(str.mb_str());

	std::stringstream test(allMoves);
	std::string segment;
	std::vector<std::string> seglist;
	while (std::getline(test, segment, '|')) {
		seglist.push_back(segment);
	}

	for (unsigned i = 0; i < seglist.size(); i++) {
		std::stringstream move(seglist[i]);
		std::string seg;
		std::vector<std::string> parts;

		while (std::getline(move, seg, ':')) {
			parts.push_back(seg);
		}

		std::stringstream time(parts[1]);
		std::string t;
		std::vector<std::string> times;

		while (std::getline(time, t, '=')) {
			times.push_back(t);
		}

		std::stringstream torque(parts[2]);
		std::string tq;
		std::vector<std::string> torques;

		while (std::getline(torque, tq, '=')) {
			torques.push_back(tq);
		}
		bool tor;
		if (torques[1].compare("true") == 0) {
			tor = true;
		}
		else {
			tor = false;
		}
		wxString movem(parts[0]);
		Movement * m = new Movement(movem, atoi(times[1].c_str()), tor);

		AllMovements::getInstance()->add(m);

		movements->SetItemCount(AllMovements::getInstance()->size());
	}

}

/*
* saveButtonClick
*/
void tccFrm::saveButtonClick(wxCommandEvent& event)
{

	std::string moveSave("");
	// insert your code her
	for (int i = 0; i < AllMovements::getInstance()->size(); i++) {
		Movement * m = AllMovements::getInstance()->get(i);
		wxString moves = m->move;
		std::string cstring = std::string(moves.mb_str());
		int time = m->time;
		std::string tq = m->torque ? "torque=true" : "torque=false";
		if (moveSave.compare("") != 0) {
			moveSave = moveSave + "|";
		}
		char numstr2[21];
		moveSave = moveSave + cstring + ":time=" + _itoa(time, numstr2, 10) + ":" + tq;
	}

	wxFileDialog
		saveFileDialog(this, _("Save .txt file"), "", "",
			"TXT files (*.txt)|*.txt", wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
	if (saveFileDialog.ShowModal() == wxID_CANCEL) {
		return;     // the user changed idea...
	}

	// save the current contents in the file;
	// this can be done with e.g. wxWidgets output streams:
	wxTextFile output_stream(saveFileDialog.GetPath());
	if (!output_stream.Exists()) {
		output_stream.Create();
	}
	output_stream.Open();
	output_stream.Clear();
	wxString movem(moveSave);
	output_stream.AddLine(movem);
	output_stream.Write();
	output_stream.Close();

}

/*
* connectButtonClick
*/
void tccFrm::connectButtonClick(wxCommandEvent& event)
{
	// insert your code here
}

/*
* playButtonClick
*/
void tccFrm::playButtonClick(wxCommandEvent& event)
{
	// insert your code here
	pthread_t moves;
	pthread_create(&moves, NULL, &tccFrm::moveThread, NULL);
	
}

/*
* upButtonClick
*/
void tccFrm::upButtonClick(wxCommandEvent& event)
{
	long itemIndex = -1;
	std::vector<int> upMoves;
	while ((itemIndex = movements->GetNextItem(itemIndex,
		wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND) {
		// Got the selected item index
		upMoves.push_back(itemIndex);
	}
	if (upMoves[0] - 1 >= 0) {
		Movement *aux1 = AllMovements::getInstance()->get(upMoves[0]);
		Movement *aux2 = AllMovements::getInstance()->get(upMoves[0] - 1);
		wxString auxString = aux1->move;
		int auxTime = aux1->time;
		bool auxTorque = aux1->torque;

		aux1->move = aux2->move;
		aux1->time = aux2->time;
		aux1->torque = aux2->torque;
		aux2->move = auxString;
		aux2->time = auxTime;
		aux2->torque = auxTorque;

	}
	movements->SetItemCount(AllMovements::getInstance()->size());
	movements->SetItemState(upMoves[0], 0, wxLIST_STATE_SELECTED);
	movements->SetItemState(upMoves[0] - 1, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	upMoves.clear();
}

/*
* downButtonClick
*/
void tccFrm::downButtonClick(wxCommandEvent& event)
{
	long itemIndex = -1;
	std::vector<int> downMoves;
	while ((itemIndex = movements->GetNextItem(itemIndex,
		wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND) {
		downMoves.push_back(itemIndex);
	}

	if (downMoves[0] + 1 < AllMovements::getInstance()->size()) {
		Movement *aux1 = AllMovements::getInstance()->get(downMoves[0]);
		Movement *aux2 = AllMovements::getInstance()->get(downMoves[0] + 1);
		wxString auxString = aux1->move;
		int auxTime = aux1->time;
		bool auxTorque = aux1->torque;

		aux1->move = aux2->move;
		aux1->time = aux2->time;
		aux1->torque = aux2->torque;
		aux2->move = auxString;
		aux2->time = auxTime;
		aux2->torque = auxTorque;

	}
	movements->SetItemCount(AllMovements::getInstance()->size());
	movements->SetItemState(downMoves[0], 0, wxLIST_STATE_SELECTED);
	movements->SetItemState(downMoves[0] + 1, wxLIST_STATE_SELECTED, wxLIST_STATE_SELECTED);
	downMoves.clear();
}

/*
* deleteButtonClick
*/
void tccFrm::deleteButtonClick(wxCommandEvent& event)
{
	long itemIndex = -1;
	std::vector<int> deleteMoves;
	while ((itemIndex = movements->GetNextItem(itemIndex,
		wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED)) != wxNOT_FOUND) {
		// Got the selected item index
		deleteMoves.push_back(itemIndex);
	}

	// Delete in reverse order.
	for (long n = (deleteMoves.size() - 1); 0 <= n; n--)
	{
		AllMovements::getInstance()->deleteIndex(deleteMoves[n]);
		movements->SetItemCount(AllMovements::getInstance()->size());
	}
	deleteMoves.clear();
}

/*std::string s = std::to_string(motorIdx[i]);
std::wstring stemp = std::wstring(s.begin(), s.end());
LPCWSTR sw = stemp.c_str();
OutputDebugString(sw);
std::string s2 = "\n";
std::wstring stemp2 = std::wstring(s2.begin(), s2.end());
LPCWSTR sw2 = stemp2.c_str();
OutputDebugString(sw2);*/