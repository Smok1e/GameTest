#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX

//------------------------

#include "Logging.h"
#include <Windows.h>
#include "Utils.hpp"

//------------------------

bool  _TraceAllowed      = true;
FILE* _TraceOutputStream = stdout;

//------------------------

Location::Location ():
	m_file (nullptr),
	m_func (nullptr),
	m_line (-1)
{}

Location::Location (const char* file, const char* function, int line):
	m_file (file),
	m_func (function),
	m_line (line)
{}

Location::Location (const Location& copy):
	m_file (copy.m_file),
	m_func (copy.m_func),
	m_line (copy.m_line)
{}

//------------------------

void Location::setFile (const char* file)
{
	m_file = file;
}

const char* Location::getFile () const
{
	return m_file? m_file: "Unknown file";
}

void Location::setFunction (const char* function)
{
	m_func = function;
}

const char* Location::getFunction () const
{
	return m_func? m_func: "Unknown function";
}

void Location::setLine (int line)
{
	m_line = line;
}

int Location::getLine () const
{
	return m_line;
}

//------------------------

void _Trace (const Location& location, const char* format, ...)
{
	if (!_TraceAllowed) return;

	size_t len = strlen (location.getFunction ());
	fprintf (_TraceOutputStream, "[%-.*s%s, line #%-3d]: ", len > _MaxFunctionNameLen? _MaxFunctionNameLen - 3: _MaxFunctionNameLen, location.getFunction (), len > _MaxFunctionNameLen? "...": "", location.getLine ());

	va_list args = {};
	va_start (args, format);
	vfprintf_s (_TraceOutputStream, format, args);
	va_end (args);

	fprintf (_TraceOutputStream, "\n");
}

//------------------------

void _DebugMessage (const Location& location, const char* format, ...)
{
	va_list args = {};
	va_start (args, format);
	size_t buffsize = vsnprintf (nullptr, 0, format, args) + 1;
	va_end (args);

	char* buffer = new char[buffsize];

	va_start (args, format);
	vsprintf_s (buffer, buffsize, format, args);
	va_end (args);

	fprintf (_TraceOutputStream, "[%s, line #%d]: ", location.getFunction (), location.getLine ());
	
	va_start (args, format);
	vfprintf_s (_TraceOutputStream, format, args);
	va_end (args);

	printf ("\n");

	int res = MessageBoxA (GetConsoleWindow (), buffer, "Debug message", MB_ABORTRETRYIGNORE | MB_ICONWARNING);
	delete[] (buffer);

	if (res == IDABORT) abort ();
}

//------------------------

void _GameAssert (const Location& location, bool expr_res, const char* expr, const char* format, ...)
{
	if (expr_res) return;

	va_list args = {};
	va_start (args, format);
	std::string message = Format (format, args);
	va_end (args);

	std::string text = Format ("Assertion '%s' failed in function '%s', at line #%d:\n%s\n\nAbort process?", expr, location.getFunction (), location.getLine (), text.c_str ());
	if (MessageBoxA (GetConsoleWindow (), text.c_str (), "Debug assertion failed", MB_ICONERROR | MB_ABORTRETRYIGNORE) == IDABORT)
		abort ();
}

void _GameAssert (const Location& location, bool expr_res, const char* expr)
{
	if (expr_res) return;

	std::string text = Format ("Assertion '%s' failed in function '%s', at line #%d\n\nAbort process?", expr, location.getFunction (), location.getLine ());
	if (MessageBoxA (GetConsoleWindow (), text.c_str (), "Debug assertion failed", MB_ICONERROR | MB_ABORTRETRYIGNORE) == IDABORT)
		abort ();
}

//------------------------

void SetTraceAllowed (bool allow)
{
	_TraceAllowed = allow;
}

bool GetTraceAllowed ()
{
	return _TraceAllowed;
}

//------------------------

void SetTraceOutputStream (FILE* stream)
{
	_TraceOutputStream = stream;
}

FILE* GetTraceOutputStream ()
{
	return _TraceOutputStream;
}

//------------------------