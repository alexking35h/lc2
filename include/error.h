// Error Handling class declaration.

#ifndef ERROR_H_
#define ERROR_H_

#include <string>

// Error handler class.
// 'report_error' is a virtual methods, so subclasses can implement
// different error-handling.
class ErrorReporter
{
    public:
        virtual ~ErrorReporter();
        virtual void report_error(int, const std::string&);
};

#endif