#pragma once
#include <wx/wx.h>
#include <tchar.h>

wxDEFINE_EVENT(myEVT_THREAD_UPDATE_CPU, wxThreadEvent);

class ThreadFrame : public wxFrame, public wxThreadHelper
{
    wxStaticText* CPUInfo;
public:
    ThreadFrame() : wxFrame(nullptr, wxID_ANY, wxT("BPI++4 v69"))
    {



        Bind(myEVT_THREAD_UPDATE_CPU, &ThreadFrame::OnThreadUpdate, this);
    }


private:
    wxThread::ExitCode Entry() override {

        while (!GetThread()->TestDestroy()) {
            wxThreadEvent* event = new wxThreadEvent(myEVT_THREAD_UPDATE_CPU);




            // Set text
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
wxBEGIN_EVENT_TABLE(ThreadFrame, wxFrame)
EVT_CLOSE(ThreadFrame::OnClose)
wxEND_EVENT_TABLE()





