#ifndef __TCCFRMApp_h__
#define __TCCFRMApp_h__

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include <wx/wx.h>
#else
#include <wx/wxprec.h>
#endif

class MyProjectApp : public wxApp
{
public:
	bool OnInit();
	int OnExit();
};

#endif
