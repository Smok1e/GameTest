#include <cstdio>
#include <cstdarg>

//------------------------

#define CurrentLocation           Location      (__FILE__, __FUNCSIG__, __LINE__)
#define Trace(format, ...)        _Trace        (CurrentLocation, format,        __VA_ARGS__);
#define DebugMessage(format, ...) _DebugMessage (CurrentLocation, format,        __VA_ARGS__);
#define GameAssert(expr, ...)     _GameAssert   (CurrentLocation, (expr), #expr, __VA_ARGS__);

//------------------------

extern FILE* _TraceOutputStream;
extern bool  _TraceAllowed;
const  int   _MaxFunctionNameLen = 50;

//------------------------

class Location
{
public:
	Location ();
	Location (const char* file, const char* function, int line);
	Location (const Location& copy);

	void        setFile     (const char* file);
	const char* getFile     () const;
	void        setFunction (const char* function);
	const char* getFunction () const;
	void        setLine     (int line);
	int         getLine     () const;

private:
	const char* m_file;
	const char* m_func;
	int         m_line;

};

//------------------------

void _Trace        (const Location& location, const char* format, ...);
void _DebugMessage (const Location& location, const char* format, ...);
void _GameAssert   (const Location& location, bool expr_res, const char* expr, const char* format, ...);
void _GameAssert   (const Location& location, bool expr_res, const char* expr);

void SetTraceAllowed (bool allow);
bool GetTraceAllowed ();

void  SetTraceOutputStream (FILE* stream);
FILE* GetTraceOutputStream ();

//------------------------