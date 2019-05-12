/*
 * ReportView.h
 * 
 * This file is part of the XShaderCompiler project (Copyright (c) 2014-2019 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef XSC_REPORT_VIEW_H
#define XSC_REPORT_VIEW_H


#include <Xsc/Log.h>
#include <wx/richtext/richtextctrl.h>


namespace Xsc
{


struct ReportedError
{
    int         line;
    wxString    text;
};

class ReportView : public wxRichTextCtrl
{

    public:

        ReportView(wxWindow* parent, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize);

        void ClearAll();

        void AddReport(const Report& r, const std::string& indent = "");

        inline const std::vector<ReportedError>& GetReportedErrors() const
        {
            return reportedErrors_;
        }

    private:

        void Write(const std::string& s, const wxColour& color = *wxWHITE);
        void WriteLn(const std::string& indent, const std::string& s, const wxColour& color = *wxWHITE);

        void AddReportedError(const std::string& sloc, const std::string& msg);

        std::vector<ReportedError> reportedErrors_;

};


} // /namespace Xsc


#endif



// ================================================================================