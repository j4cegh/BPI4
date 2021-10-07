#pragma once
#include "targetver.h"
#include <windows.h>
#include <wx/wx.h>
#include <wx/wxprec.h>
#include <tchar.h>
#include "const.h"
#include <array>
#include <intrin.h>
#include "PCInfoHelper.h"



class PCInfo : public wxFrame
{
    
    wxStaticText* PCInfost;
    
public:
    
    PCInfo() : wxFrame(nullptr, wxID_ANY, wxT("BPI++4 v" + ver)),
        PCInfost(new wxStaticText(this, wxID_ANY, "hey"))
    {
        
        
        
        wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1920, 1080));
        // enlarge because pc components' names are huge lmao
        PCInfo::SetSize(resX + 400, resY);
        // set BG color
        Common::setBackgroundColor(panel, wxColor(*wxWHITE));
        // set font & change the size A BIT so it fits on most windows
        Common::enlargeAndSetFont(PCInfost, 25, wxFONTFAMILY_DEFAULT);
        // set text BG to BG color
        Common::BGColortoTextBG(PCInfost, panel);

        // start device procedure
        // PCInfoHelper contains *most* components' info.

        

        DISPLAY_DEVICE dd = { sizeof(dd), 0 };
        BOOL f = EnumDisplayDevices(NULL, 1, &dd, EDD_GET_DEVICE_INTERFACE_NAME);
        wchar_t* d = dd.DeviceString;
        std::wstring ws(d);
        std::string gpuName(ws.begin(), ws.end());
        
        
        if (!f)
            std::cout << "problem..";
        else {
            
            PCInfost->SetLabel(
                "GPU: " + 
                gpuName + " " + 
                PcInfoHelper::getGPUVRAM() + 
                "GB" + 
                "\n" + 
                "CPU: " + PcInfoHelper::GetCpuInfo() + "\n" +
                "MB: " + "" + "\n");
        }



            
            
        
        



        
    }


private:


    

    
};





