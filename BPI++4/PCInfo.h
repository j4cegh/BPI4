#pragma once
#include <wx/wx.h>
#include <wx/wxprec.h>
#include <tchar.h>
#include "const.h"
#include <dxdiag.h>


class PCInfo : public wxFrame
{
    
    wxStaticText* PCInfost;
public:
    
    PCInfo() : wxFrame(nullptr, wxID_ANY, wxT("BPI++4 v" + ver)),
        PCInfost(new wxStaticText(this, wxID_ANY, "hey"))
    {
        
        wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1920, 1080));
        PCInfo::SetSize(resX, resY);
        // set BG color
        Common::setBackgroundColor(panel, wxColor(*wxWHITE));
        // set font
        Common::enlargeAndSetFont(PCInfost, 30, wxFONTFAMILY_DEFAULT);
        // set text BG to BG color
        Common::BGColortoTextBG(PCInfost, panel);

        // start device procedure
        
        

        DISPLAY_DEVICE dd = { sizeof(dd), 0 };
        BOOL f = EnumDisplayDevices(NULL, 1, &dd, EDD_GET_DEVICE_INTERFACE_NAME);
        wchar_t* d = dd.DeviceString;
        std::wstring ws(d);
        std::string gpuName(ws.begin(), ws.end());
        if (!f)
            std::cout << "problem..";
        else {
            
            PCInfost->SetLabel("GPU: " + gpuName);
        }



            
            
        
        



        
    }


private:


    

    
};





