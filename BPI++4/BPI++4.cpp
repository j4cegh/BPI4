// Main application for BPI++ att4
#include <wx/wx.h>
#include <tchar.h>
#include "const.h"
#include "Common.h"
#include "PCInfo.h"
// define on thread update; we bind it to a void later
wxDEFINE_EVENT(myEVT_THREAD_UPDATE, wxThreadEvent);
// define DIV; Byte to MB for memory conversion later
#define DIV 1048576

MEMORYSTATUSEX status;
class MainFrame : public wxFrame, public wxThreadHelper {
    wxStaticText* textMemoryUsage;
public:
    
    
    void PCInfoClicked(wxCommandEvent& event)
    {
        PCInfo* pcInfo = new PCInfo;
        pcInfo->Show();
    };
    MainFrame() : wxFrame(nullptr, wxID_ANY, wxT("BPI++4 v") + ver),
        // add the memory usage text
        textMemoryUsage(new wxStaticText(this, wxID_ANY, wxNow()))
    {
        

        
        // positionz
        MainFrame::SetSize(wxSize(resX, resY));
        // panel
        wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1920, 1080));
        // COLORZ
        Common::setBackgroundColor(panel, wxColour(*wxWHITE));
        
        
        // set text color to BG color
        Common::BGColortoTextBG(textMemoryUsage, panel);

        
        
        // Just for fun lmao
        Update();
        // set font
        Common::enlargeAndSetFont(textMemoryUsage, 30, wxFONTFAMILY_DEFAULT);
        //self explanatory no comments
        
        
        wxButton* button2 = new wxButton(panel, wxID_ANY, L"PC Info", wxPoint(0, 250), wxSize(200, 40));
        button2->Bind(wxEVT_BUTTON, &MainFrame::PCInfoClicked, this);
        button2->SetBackgroundColour(wxColor(*wxLIGHT_GREY));

        // I guess threading issues?
        if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR) {
            wxLogError("Could not create the worker thread!");
            return;
        }
        if (GetThread()->Run() != wxTHREAD_NO_ERROR) {
            wxLogError("Could not run the worker thread!");
            return;
        }

        Bind(myEVT_THREAD_UPDATE, &MainFrame::OnThreadUpdate, this);
    }
private:
    wxThread::ExitCode Entry() override {

        while (!GetThread()->TestDestroy()) {
            wxThreadEvent* event = new wxThreadEvent(myEVT_THREAD_UPDATE);
            
            GlobalMemoryStatusEx(&status);
            status.dwLength = sizeof(status);
            int totMemMB = status.ullTotalPhys / DIV;
            int usedMemMB = totMemMB - status.ullAvailPhys / DIV;
            int memFreeMB = status.ullAvailPhys / DIV;
            
            
            // Set text of the PC infos
            event->SetString(
                "\nMemory usage: \n" +
                std::to_string(usedMemMB) + "MB / " + 
                std::to_string(totMemMB) + "MB"
                + "\n\n" +
                "Memory free: " + std::to_string(memFreeMB) + "MB"
            );
            wxQueueEvent(this, event);

            if (m_shutdownSemaphore.WaitTimeout(1000) != wxSEMA_TIMEOUT)
            {
                break;
            }
        }
        return (wxThread::ExitCode)0;
    }

    void OnClose(wxCloseEvent& event) {
        auto t = GetThread();
        if (t) {
            m_shutdownSemaphore.Post();
            t->Wait();
        }
        event.Skip();
    }

    void OnThreadUpdate(wxThreadEvent& event)
    {

        textMemoryUsage->SetLabel(event.GetString());
    }

    wxSemaphore m_shutdownSemaphore;
    wxDECLARE_EVENT_TABLE();
};

wxBEGIN_EVENT_TABLE(MainFrame, wxFrame)
EVT_CLOSE(MainFrame::OnClose)
wxEND_EVENT_TABLE()

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