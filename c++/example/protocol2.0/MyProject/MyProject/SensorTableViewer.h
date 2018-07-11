
#include "AllSensor.h"

#include <wx/listctrl.h>

#ifndef __SENSOR_TABLE_VIEWER_H__

#define __SENSOR_TABLE_VIEWER_H__

class SensorTableViewer : public wxListCtrl {

protected:

	virtual wxString OnGetItemText(long item, long column) const {

		return AllSensor::getInstance()->get(item)->getAllPropertiesAsOneRow()[column];

	}

public:SensorTableViewer(wxFrame* frm, long id) :

	wxListCtrl(frm, id, wxPoint(953, 39), wxSize(305, 213), wxLC_REPORT | wxLC_VIRTUAL, wxDefaultValidator, wxT("sensorTable")) {
	InsertColumn(0, wxT("Accel X"), wxLIST_FORMAT_LEFT, 50);
	InsertColumn(1, wxT("Accel Y"), wxLIST_FORMAT_LEFT, 50);
	InsertColumn(2, wxT("Accel Z"), wxLIST_FORMAT_LEFT, 50);
	InsertColumn(3, wxT("Gyro X"), wxLIST_FORMAT_LEFT, 50);
	InsertColumn(4, wxT("Gyro Y"), wxLIST_FORMAT_LEFT, 50);
	InsertColumn(5, wxT("Gyro Z"), wxLIST_FORMAT_LEFT, 50);
	InsertColumn(6, wxT("Mag X"), wxLIST_FORMAT_LEFT, 50);
	InsertColumn(7, wxT("Mag Y"), wxLIST_FORMAT_LEFT, 50);
	InsertColumn(8, wxT("Mag Z"), wxLIST_FORMAT_LEFT, 50);
	InsertColumn(9, wxT("Quat X"), wxLIST_FORMAT_LEFT, 50);
	InsertColumn(10, wxT("Quat Y"), wxLIST_FORMAT_LEFT, 50);
	InsertColumn(11, wxT("Quat Z"), wxLIST_FORMAT_LEFT, 50);
	InsertColumn(12, wxT("Euler X"), wxLIST_FORMAT_LEFT, 50);
	InsertColumn(13, wxT("Euler Y"), wxLIST_FORMAT_LEFT, 50);
	InsertColumn(14, wxT("Euler Z"), wxLIST_FORMAT_LEFT, 50);
}

};

#endif
