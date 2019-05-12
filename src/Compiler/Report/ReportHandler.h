/*
 * ReportHandler.h
 * 
 * This file is part of the XShaderCompiler project (Copyright (c) 2014-2019 by Lukas Hermanns)
 * See "LICENSE.txt" for license information.
 */

#ifndef XSC_REPORT_HANDLER_H
#define XSC_REPORT_HANDLER_H


#include "SourceArea.h"
#include <Xsc/Report.h>
#include <Xsc/Log.h>
#include <string>
#include <stack>
#include <set>
#include <functional>
#include <vector>


namespace Xsc
{


class SourceCode;

// Interface for a common report callback procedure (either for error or warning messages).
using OnReportProc = std::function<void(const std::string& msg, const AST* ast)>;

// Report handler class for simpler error and warning handling.
class ReportHandler
{

    public:

        ReportHandler(Log* log);

        void Warning(
            bool                breakWithExpection,
            const std::string&  msg,
            SourceCode*         sourceCode          = nullptr,
            const SourceArea&   area                = SourceArea::ignore
        );

        void SubmitReport(
            bool                            breakWithExpection,
            const ReportTypes               type,
            const std::string&              typeName,
            const std::string&              msg,
            SourceCode*                     sourceCode      = nullptr,
            const SourceArea&               area            = SourceArea::ignore,
            const std::vector<SourceArea>&  secondaryAreas  = {}
        );

        // Returns true if any errors have been submitted.
        inline bool HasErrors() const
        {
            return hasErrors_;
        }

        // Pushes the specified context description string onto the stack. The top most description will be added to the next report message.
        void PushContextDesc(const std::string& contextDesc);
        void PopContextDesc();

        /*
        Appends a hint for the next upcomming report.
        Implemented as static function to avoid passing lots of report data around the code.
        */
        static void HintForNextReport(const std::string& hint);

    private:

        Report MakeReport(
            const ReportTypes               type,
            const std::string&              typeName,
            const std::string&              source,
            const std::string&              msg,
            SourceCode*                     sourceCode,
            const SourceArea&               area,
            const std::vector<SourceArea>&  secondaryAreas
        );

        Log*                        log_                = nullptr;
        bool                        hasErrors_          = false;

        std::stack<std::string>     contextDescStack_;

        // TODO: sometimes the same error/warning message appears multiple times, these sets are a workaround for this issue
        std::set<SourcePosition>    errorPositions_;
        std::set<SourcePosition>    warningPositions_;

};


} // /namespace Xsc


#endif



// ================================================================================