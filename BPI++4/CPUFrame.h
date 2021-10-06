#pragma once
#include <wx/wx.h>
#include <tchar.h>
#include "const.h"
#include "Common.h"

wxDEFINE_EVENT(myEVT_THREAD_UPDATE_CPU, wxThreadEvent);

class CPUFrame : public wxFrame, public wxThreadHelper
{
	wxStaticText* CPUInfo;
public:
    CPUFrame() : wxFrame(nullptr, wxID_ANY, wxT("BPI++4 v" + ver)),
        // define CPUInfo with static text
        CPUInfo(new wxStaticText(this, wxID_ANY, wxNow()))
    {
        // create panel
        wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1920, 1080));
        // set BG color
        Common::setBackgroundColor(panel, wxColor(*wxWHITE));
        // resolution
        CPUFrame::SetSize(wxSize(resX, resY));

        // set text BG to BG color
        Common::BGColortoTextBG(CPUInfo, panel);
        // set font
        Common::enlargeAndSetFont(CPUInfo, 30, wxFONTFAMILY_DEFAULT);

       
        if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR) {
            wxLogError("Could not create the worker thread!");
            return;
        }
        if (GetThread()->Run() != wxTHREAD_NO_ERROR) {
            wxLogError("Could not run the worker thread!");
            return;
        }
        Bind(myEVT_THREAD_UPDATE_CPU, &CPUFrame::OnThreadUpdate, this);
    }


private:
    wxThread::ExitCode Entry() override {

        while (!GetThread()->TestDestroy()) {
            wxThreadEvent* event = new wxThreadEvent(myEVT_THREAD_UPDATE_CPU);

            


            // Set text of the PC infos
            event->SetString(
                "x"
            );
            wxQueueEvent(this, event);
            // every 1000 ms aka every second
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

        CPUInfo->SetLabel(event.GetString());
    }

    wxSemaphore m_shutdownSemaphore;
    wxDECLARE_EVENT_TABLE();
};
wxBEGIN_EVENT_TABLE(CPUFrame, wxFrame)
EVT_CLOSE(CPUFrame::OnClose)
wxEND_EVENT_TABLE()





