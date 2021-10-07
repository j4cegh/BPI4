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
#include <atlcore.h>


#define DIV 1048576

class PCInfo : public wxFrame
{
    
    
    
    std::wstring GetStrRegistry(std::string regvalname, LPCTSTR subkey)
    {
        std::wstring stemp = std::wstring(regvalname.begin(), regvalname.end());
        LPCWSTR sw = stemp.c_str();
        // Try open registry key
        HKEY hKey = NULL;
        LPCTSTR pszSubkey = subkey;
        if (RegOpenKey(HKEY_LOCAL_MACHINE, pszSubkey, &hKey) != ERROR_SUCCESS)
        {
            // Error:
            // throw an exception or something...
            //
            // (In production code a custom C++ exception 
            // derived from std::runtime_error could be used)
            
        }

        // Buffer to store string read from registry
        TCHAR szValue[1024];
        DWORD cbValueLength = sizeof(szValue);

        // Query string value
        if (RegQueryValueEx(
            hKey,
            sw,
            NULL,
            NULL,
            reinterpret_cast<LPBYTE>(&szValue),
            &cbValueLength)
            != ERROR_SUCCESS)
        {
            // Error
            // throw an exception or something...
            
        }


        std::wstring g(szValue);
        return g;
    }
    
    
    wxStaticText* PCInfost;
    
public:
    
    PCInfo() : wxFrame(nullptr, wxID_ANY, wxT("BPI++4 v" + ver + " | Spec-fetcher")),
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
            std::wstring MBBrand(GetStrRegistry("BaseBoardManufacturer", _T("HARDWARE\\DESCRIPTION\\System\\BIOS")));
            std::wstring MBModel(GetStrRegistry("BaseBoardProduct", _T("HARDWARE\\DESCRIPTION\\System\\BIOS")));
            MEMORYSTATUSEX mstat;
            mstat.dwLength = sizeof(mstat);
            GlobalMemoryStatusEx(&mstat);
            
            PCInfost->SetLabel(
                "GPU: " +
                gpuName + " " +
                PcInfoHelper::getGPUVRAM() +
                "GB" +
                "\n" +
                "CPU: " +
                PcInfoHelper::GetCpuInfo()
                + "\n" +
                "RAM: " +
                std::to_string((int)round(mstat.ullTotalPhys / DIV / 1024 + 0.5)) + "GB" +
                "\n" +
                "MoBo: " +
                MBBrand + 
                " " + 
                MBModel + 
                "\n");
        }



            
            
        
        



        
    }


private:


    

    
};





