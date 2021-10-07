// Main application for BPI++ att4
#include <wx/wx.h>
#include <tchar.h>
#include "const.h"
#include "Common.h"
#include "PCInfo.h"
#include "MemUsage.h"

// define DIV; Byte to MB for memory conversion later
#define DIV 1048576


class MainFrame : public wxFrame {
public:
    MEMORYSTATUSEX status;
    
    void PCInfoClicked(wxCommandEvent& event)
    {
        PCInfo* pcInfo = new PCInfo;
        pcInfo->Show();
    };
    void RamStatusClicked(wxCommandEvent& event)
    {
        MemUsage* memusage = new MemUsage;
        memusage->Show();
    };
    MainFrame() : wxFrame(nullptr, wxID_ANY, wxT("BPI++4 v") + ver + " | Main window")
        
    {
        

        
        // positionz
        MainFrame::SetSize(wxSize(resX, resY));
        // panel
        wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1920, 1080));
        // COLORZ
        Common::setBackgroundColor(panel, wxColour(*wxWHITE));
        
        
        

        
        
        // Just for fun lmao
        Update();

        //self explanatory no comments
        
        // ramstatus frame
        

        wxButton* button2 = new wxButton(panel, wxID_ANY, L"PC Specs", wxPoint(0, 150), wxSize(200, 40));
        button2->Bind(wxEVT_BUTTON, &MainFrame::PCInfoClicked, this);
        button2->SetBackgroundColour(wxColor(*wxLIGHT_GREY));

        wxButton* ramstat = new wxButton(panel, wxID_ANY, L"Memory usage", wxPoint(0, 200), wxSize(200, 40));
        ramstat->Bind(wxEVT_BUTTON, &MainFrame::RamStatusClicked, this);
        ramstat->SetBackgroundColour(wxColor(*wxLIGHT_GREY));

        

    }
private:
    

    

    
};



class MainApp : public wxApp
{
    wxFrame* m_frame = nullptr;
public:
    bool OnInit() override {
        m_frame = new MainFrame;
        m_frame->Show();
        return true;
    }
};

IMPLEMENT_APP(MainApp);