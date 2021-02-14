#ifndef ERROR_H
#define ERROR_H

#include <string>

class ErrorReporter
{
    public:
        virtual ~ErrorReporter();
        virtual void report_error(int, const std::string&);
};

#endif