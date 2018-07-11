
#include "AllMovements.h"

#include <wx/listctrl.h>

#ifndef __MOVEMENTS_TABLE_VIEWER_H__

#define __MOVEMENTS_TABLE_VIEWER_H__

class MovementsTableViewer : public wxListCtrl {

protected:

	virtual wxString OnGetItemText(long item, long column) const {

		return AllMovements::getInstance()->get(item)->getAllPropertiesAsOneRow()[column];

	}

public:MovementsTableViewer(wxFrame* frm, long id) :

	wxListCtrl(frm, id, wxPoint(348, 38), wxSize(224, 518), wxLC_REPORT | wxLC_VIRTUAL, wxDefaultValidator, wxT("movements")) {
	InsertColumn(0, wxT("Motors Movements"), wxLIST_FORMAT_LEFT, 150);
	InsertColumn(1, wxT("Time"), wxLIST_FORMAT_LEFT, 50);
	InsertColumn(2, wxT("Torque"), wxLIST_FORMAT_LEFT, 50);
}

};

#endif
