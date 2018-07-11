#include <wx/wx.h>

class Movement {

private:

public:

	wxString move;

	int time;

	bool torque;

	wxString* getAllPropertiesAsOneRow() {

		wxString* r = new wxString[4];

		r[0] = move;

		r[1] << time;

		r[2] = torque ? "True" : "False";

		return r;

	}

	Movement(wxString m, int t, bool tq) {

		move = m;

		time = t;

		torque = tq;

	}

};
