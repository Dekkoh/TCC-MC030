#ifndef __TCCFRM_H__
#define __TCCFRM_H__

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#include <wx/frame.h>
#else
#include <wx/wxprec.h>
#endif

//Do not add custom headers between 
//Header Include Start and Header Include End.
//wxDev-C++ designer will remove them. Add custom headers after the block.
////Header Include Start
#include <wx/slider.h>
#include <wx/listctrl.h>
#include <wx/textctrl.h>
#include <wx/stattext.h>
#include <wx/checkbox.h>
#include <wx/button.h>
////Header Include End
#include <vector>
#include <wx/filedlg.h>
#include <wx/filefn.h> 
#include <wx/wfstream.h>
#include <wx/textfile.h>
#include <sstream>
//#include <opencv2/opencv.hpp>
#define HAVE_STRUCT_TIMESPEC
#include <pthread.h>
#include <string>
#include <numeric>
#include <iterator>
#include <cstdint>

////Dialog Style Start
#undef tccFrm_STYLE
#define tccFrm_STYLE wxCAPTION | wxSYSTEM_MENU | wxMINIMIZE_BOX | wxCLOSE_BOX
////Dialog Style End

class tccFrm : public wxFrame
{
private:
	DECLARE_EVENT_TABLE();

public:
	tccFrm(wxWindow *parent, wxWindowID id = 1, const wxString &title = wxT("tcc"), const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize, long style = tccFrm_STYLE);
	virtual ~tccFrm();
	void addButtonClick(wxCommandEvent& event);
	void loadButtonClick(wxCommandEvent& event);
	void saveButtonClick(wxCommandEvent& event);
	void connectButtonClick(wxCommandEvent& event);
	void playButtonClick(wxCommandEvent& event);
	void upButtonClick(wxCommandEvent& event);
	void downButtonClick(wxCommandEvent& event);
	void deleteButtonClick(wxCommandEvent& event);
	static void *readValue(void *motorStruct);
	static void *camera(void *aux);
	static void *moveThread(void *aux);
	static void *sensorThread(void *aux);

private:
	pthread_t th[25];
	pthread_t tCamera;
	pthread_t tSensor;
	std::vector<std::string> ports;
	std::vector<std::string> nameMotor;
	std::vector<wxSlider*> motorSliders;
	std::vector<wxCheckBox*> motorCheckBoxs;
	std::vector<wxTextCtrl*> maxPositions;
	std::vector<wxTextCtrl*> minPositions;
	wxTextCtrl *maxPosition2;
	wxTextCtrl *minPosition2;
	wxTextCtrl *maxPosition3;
	wxTextCtrl *minPosition3;
	wxTextCtrl *maxPosition4;
	wxTextCtrl *minPosition4;
	wxTextCtrl *maxPosition5;
	wxTextCtrl *minPosition5;
	wxTextCtrl *maxPosition6;
	wxTextCtrl *minPosition6;
	wxTextCtrl *maxPosition7;
	wxTextCtrl *minPosition7;
	wxTextCtrl *maxPosition8;
	wxTextCtrl *minPosition8;
	wxTextCtrl *maxPosition9;
	wxTextCtrl *minPosition9;
	wxTextCtrl *maxPosition10;
	wxTextCtrl *minPosition10;
	wxTextCtrl *maxPosition11;
	wxTextCtrl *minPosition11;
	wxTextCtrl *maxPosition12;
	wxTextCtrl *minPosition12;
	wxTextCtrl *maxPosition13;
	wxTextCtrl *minPosition13;
	wxTextCtrl *maxPosition14;
	wxTextCtrl *minPosition14;
	wxTextCtrl *maxPosition15;
	wxTextCtrl *minPosition15;
	wxTextCtrl *maxPosition16;
	wxTextCtrl *minPosition16;
	wxTextCtrl *maxPosition17;
	wxTextCtrl *minPosition17;
	wxTextCtrl *maxPosition18;
	wxTextCtrl *minPosition18;
	wxTextCtrl *maxPosition19;
	wxTextCtrl *minPosition19;
	wxTextCtrl *maxPosition20;
	wxTextCtrl *minPosition20;
	wxTextCtrl *maxPosition21;
	wxTextCtrl *minPosition21;
	wxTextCtrl *maxPosition22;
	wxTextCtrl *minPosition22;
	wxTextCtrl *maxPosition23;
	wxTextCtrl *minPosition23;
	wxTextCtrl *maxPosition24;
	wxTextCtrl *minPosition24;
	wxTextCtrl *maxPosition25;
	wxTextCtrl *minPosition25;
	//Do not add custom control declarations between
	//GUI Control Declaration Start and GUI Control Declaration End.
	//wxDev-C++ will remove them. Add custom code after the block.
	////GUI Control Declaration Start
	wxTextCtrl *maxPosition1;
	wxTextCtrl *minPosition1;
	wxStaticText *MaxPosition;
	wxStaticText *MinPosition;
	wxButton *deleteButton;
	wxButton *downButton;
	wxButton *upButton;
	wxSlider *motorSlider25;
	wxSlider *motorSlider24;
	wxSlider *motorSlider23;
	wxSlider *motorSlider22;
	wxSlider *motorSlider21;
	wxSlider *motorSlider20;
	wxSlider *motorSlider19;
	wxSlider *motorSlider18;
	wxSlider *motorSlider17;
	wxSlider *motorSlider16;
	wxSlider *motorSlider15;
	wxSlider *motorSlider14;
	wxSlider *motorSlider13;
	wxSlider *motorSlider12;
	wxSlider *motorSlider11;
	wxSlider *motorSlider10;
	wxSlider *motorSlider9;
	wxSlider *motorSlider8;
	wxSlider *motorSlider7;
	wxSlider *motorSlider6;
	wxSlider *motorSlider5;
	wxSlider *motorSlider4;
	wxSlider *motorSlider3;
	wxSlider *motorSlider2;
	wxSlider *motorSlider1;
	wxListCtrl *movements;
	wxCheckBox *motorCheckBox25;
	wxCheckBox *motorCheckBox24;
	wxCheckBox *motorCheckBox23;
	wxCheckBox *motorCheckBox22;
	wxCheckBox *motorCheckBox21;
	wxCheckBox *motorCheckBox20;
	wxCheckBox *motorCheckBox19;
	wxCheckBox *motorCheckBox18;
	wxCheckBox *motorCheckBox17;
	wxCheckBox *motorCheckBox16;
	wxCheckBox *motorCheckBox15;
	wxCheckBox *motorCheckBox14;
	wxCheckBox *motorCheckBox13;
	wxCheckBox *motorCheckBox12;
	wxCheckBox *motorCheckBox11;
	wxCheckBox *motorCheckBox10;
	wxCheckBox *motorCheckBox9;
	wxCheckBox *motorCheckBox8;
	wxCheckBox *motorCheckBox7;
	wxCheckBox *motorCheckBox6;
	wxCheckBox *motorCheckBox5;
	wxCheckBox *motorCheckBox4;
	wxCheckBox *motorCheckBox3;
	wxCheckBox *motorCheckBox2;
	wxCheckBox *motorCheckBox1;
	wxTextCtrl *timeText;
	wxStaticText *staticTimeText;
	wxCheckBox *torqueCheckBox;
	wxButton *playButton;
	wxButton *saveButton;
	wxButton *loadButton;
	wxButton *addButton;
	////GUI Control Declaration End
private:
	//Note: if you receive any error with these enum IDs, then you need to
	//change your old form code that are based on the #define control IDs.
	//#defines may replace a numeric value for the enum names.
	//Try copy and pasting the below block in your old form header files.
	enum
	{
		////GUI Enum Control ID Start
		ID_SENSORCHECKBOX = 1237,
		ID_VALUECHECKBOX = 1236,
		ID_SENSORTABLE = 1235,
		ID_MAXPOSITION1 = 1234,
		ID_MINPOSITION1 = 1233,
		ID_MAXPOSITION = 1232,
		ID_MINPOSITION = 1231,
		ID_DELETEBUTTON = 1230,
		ID_DOWNBUTTON = 1229,
		ID_UPBUTTON = 1228,
		ID_VALUETABLE = 1227,
		ID_MOTORSLIDER25 = 1226,
		ID_MOTORSLIDER24 = 1225,
		ID_MOTORSLIDER23 = 1224,
		ID_MOTORSLIDER22 = 1185,
		ID_MOTORSLIDER21 = 1184,
		ID_MOTORSLIDER20 = 1183,
		ID_MOTORSLIDER19 = 1182,
		ID_MOTORSLIDER18 = 1181,
		ID_MOTORSLIDER17 = 1180,
		ID_MOTORSLIDER16 = 1151,
		ID_MOTORSLIDER15 = 1150,
		ID_MOTORSLIDER14 = 1149,
		ID_MOTORSLIDER13 = 1148,
		ID_MOTORSLIDER12 = 1147,
		ID_MOTORSLIDER11 = 1146,
		ID_MOTORSLIDER10 = 1145,
		ID_MOTORSLIDER9 = 1144,
		ID_MOTORSLIDER8 = 1115,
		ID_MOTORSLIDER7 = 1114,
		ID_MOTORSLIDER6 = 1113,
		ID_MOTORSLIDER5 = 1112,
		ID_MOTORSLIDER4 = 1105,
		ID_MOTORSLIDER3 = 1104,
		ID_MOTORSLIDER2 = 1102,
		ID_MOTORSLIDER1 = 1101,
		ID_MOVEMENTS = 1098,
		ID_MOTORCHECKBOX25 = 1097,
		ID_MOTORCHECKBOX24 = 1096,
		ID_MOTORCHECKBOX23 = 1095,
		ID_MOTORCHECKBOX22 = 1094,
		ID_MOTORCHECKBOX21 = 1093,
		ID_MOTORCHECKBOX20 = 1092,
		ID_MOTORCHECKBOX19 = 1091,
		ID_MOTORCHECKBOX18 = 1090,
		ID_MOTORCHECKBOX17 = 1089,
		ID_MOTORCHECKBOX16 = 1060,
		ID_MOTORCHECKBOX15 = 1059,
		ID_MOTORCHECKBOX14 = 1058,
		ID_MOTORCHECKBOX13 = 1057,
		ID_MOTORCHECKBOX12 = 1056,
		ID_MOTORCHECKBOX11 = 1055,
		ID_MOTORCHECKBOX10 = 1054,
		ID_MOTORCHECKBOX9 = 1053,
		ID_MOTORCHECKBOX8 = 1024,
		ID_MOTORCHECKBOX7 = 1023,
		ID_MOTORCHECKBOX6 = 1022,
		ID_MOTORCHECKBOX5 = 1021,
		ID_MOTORCHECKBOX4 = 1014,
		ID_MOTORCHECKBOX3 = 1013,
		ID_MOTORCHECKBOX2 = 1011,
		ID_MOTORCHECKBOX1 = 1010,
		ID_TIMETEXT = 1008,
		ID_STATICTIMETEXT = 1007,
		ID_TORQUECHECKBOX = 1006,
		ID_PLAYBUTTON = 1005,
		ID_CONNECTBUTTON = 1004,
		ID_SAVEBUTTON = 1003,
		ID_LOADBUTTON = 1002,
		ID_ADDBUTTON = 1001,
		////GUI Enum Control ID End
		ID_DUMMY_VALUE_ //don't remove this value unless you have other enum values
	};

private:
	void OnClose(wxCloseEvent& event);
	void CreateGUIControls();
};

#endif