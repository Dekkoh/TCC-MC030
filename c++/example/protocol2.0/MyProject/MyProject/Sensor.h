#include <wx/wx.h>

class Sensor {

private:

public:

	double accelX;

	double accelY;

	double accelZ;

	double gravX;

	double gravY;

	double gravZ;

	double magX;

	double magY;

	double magZ;

	double quatX;

	double quatY;

	double quatZ;

	double eulerX;

	double eulerY;

	double eulerZ;

	wxString* getAllPropertiesAsOneRow() {

		wxString* r = new wxString[15];

		r[0] = wxString::Format(wxT("%f"), accelX);

		r[1] = wxString::Format(wxT("%f"), accelY);

		r[2] = wxString::Format(wxT("%f"), accelZ);

		r[3] = wxString::Format(wxT("%f"), gravX);

		r[4] = wxString::Format(wxT("%f"), gravY);

		r[5] = wxString::Format(wxT("%f"), gravZ);

		r[6] = wxString::Format(wxT("%f"), magX);

		r[7] = wxString::Format(wxT("%f"), magY);

		r[8] = wxString::Format(wxT("%f"), magZ);

		r[9] = wxString::Format(wxT("%f"), quatX);

		r[10] = wxString::Format(wxT("%f"), quatY);

		r[11] = wxString::Format(wxT("%f"), quatZ);

		r[12] = wxString::Format(wxT("%f"), eulerX);

		r[13] = wxString::Format(wxT("%f"), eulerY);

		r[14] = wxString::Format(wxT("%f"), eulerZ);

		return r;

	}

	Sensor(double aX, double aY, double aZ, double gX, double gY, double gZ, double mX, double mY, double mZ, double q1, double q2, double q3, double e1, double e2, double e3) {

		accelX = aX;

		accelY = aY;

		accelZ = aZ;

		gravX = gX;

		gravY = gY;

		gravZ = gZ;

		magX = mX;

		magY = mY;

		magZ = mZ;

		quatX = q1;

		quatY = q2;

		quatZ = q3;

		eulerX = e1;

		eulerY = e2;

		eulerZ = e3;

	}

};
