#include <wx/wx.h>

class Values {

private:

public:
	int index;
	wxString position;
	wxString velocity;
	wxString current;
	bool torque;
	wxString tensao;

	wxString* getAllPropertiesAsOneRow() {

		wxString* r = new wxString[6];

		r[0] << index;

		r[1] = position;

		r[2] = velocity;

		r[3] = torque ? "True" : "False";

		r[4] = current;

		r[5] = tensao;

		return r;

	}

	Values(int i, wxString p, wxString v, bool tq, wxString c, wxString t) {
		index = i;

		position = p;

		velocity = v;

		torque = tq;

		current = c;

		tensao = t;

	}
};
