
#include "AllValues.h"

#include <wx/listctrl.h>

#ifndef __VALUES_TABLE_VIEWER_H__

#define __VALUES_TABLE_VIEWER_H__

class ValuesTableViewer : public wxListCtrl {

protected:

	virtual wxString OnGetItemText(long item, long column) const {

		return AllValues::getInstance()->get(item)->getAllPropertiesAsOneRow()[column];

	}

public:ValuesTableViewer(wxFrame* frm, long id) :

	wxListCtrl(frm, id, wxPoint(631, 38), wxSize(301, 518), wxLC_REPORT | wxLC_VIRTUAL, wxDefaultValidator, wxT("valueTable")) {
	InsertColumn(0, wxT("Index"), wxLIST_FORMAT_LEFT, 50);
	InsertColumn(1, wxT("Position"), wxLIST_FORMAT_LEFT, 57);
	InsertColumn(2, wxT("Velocity"), wxLIST_FORMAT_LEFT, 55);
	InsertColumn(3, wxT("Torque"), wxLIST_FORMAT_LEFT, 50);
	InsertColumn(4, wxT("Current"), wxLIST_FORMAT_LEFT, 52);
	InsertColumn(5, wxT("Tensao"), wxLIST_FORMAT_LEFT, 52);
}

};

#endif
