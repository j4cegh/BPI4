#pragma once
#include <wx/wx.h>
static class Common
{
public:
	Common();
	~Common();
	static void setBackgroundColor(wxPanel* panel, wxColour wxCl)
	{
		panel->SetBackgroundColour(wxCl);
	}
	static void BGColortoTextBG(wxStaticText* text, wxPanel* panel)
	{
		text->SetBackgroundColour(panel->GetBackgroundColour());
	}
	static void enlargeAndSetFont(wxStaticText* text, int yy, wxFontFamily wff)
	{
		text->SetFont(wxFontInfo(wxSize(0, yy))
			.Family(wff)
		);
	}
	

private:
	
};

Common::Common()
{
}

Common::~Common()
{
}
