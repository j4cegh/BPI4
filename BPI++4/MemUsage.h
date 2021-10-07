#pragma once
#include <wx/wx.h>
#include <tchar.h>
#include "const.h"
#define DIV 1048576


wxDEFINE_EVENT(myEVT_THREAD_UPDATE_MEM, wxThreadEvent);

class MemUsage : public wxFrame, public wxThreadHelper
{
    MEMORYSTATUSEX status;
    wxStaticText* MemUSG;
public:

    MemUsage() : wxFrame(nullptr, wxID_ANY, wxT("BPI++4 v") + ver),
        MemUSG(new wxStaticText(this, wxID_ANY, "donut"))
    {
        wxPanel* panel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(1920, 1080));

        Common::setBackgroundColor(panel, wxColor(*wxWHITE));
        Common::BGColortoTextBG(MemUSG, panel);
        Common::enlargeAndSetFont(MemUSG, 30, wxFONTFAMILY_DEFAULT);
        MemUsage::SetSize(wxSize(resX, resY));
        if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR) {
            wxLogError("Could not create the worker thread!");
            return;
        }
        if (GetThread()->Run() != wxTHREAD_NO_ERROR) {
            wxLogError("Could not run the worker thread!");
            return;
        }

        Bind(myEVT_THREAD_UPDATE_MEM, &MemUsage::OnThreadUpdate, this);
    }


private:
    wxThread::ExitCode Entry() override {

        while (!GetThread()->TestDestroy()) {
            wxThreadEvent* event = new wxThreadEvent(myEVT_THREAD_UPDATE_MEM);




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
        MemUSG->SetLabel(event.GetString());
    }

    wxSemaphore m_shutdownSemaphore;
    wxDECLARE_EVENT_TABLE();
};
wxBEGIN_EVENT_TABLE(MemUsage, wxFrame)
EVT_CLOSE(MemUsage::OnClose)
wxEND_EVENT_TABLE()





