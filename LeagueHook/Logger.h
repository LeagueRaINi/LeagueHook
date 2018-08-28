#pragma once

#include <windows.h>
#include <fstream>

enum LogLevel
{
	Info,
	Error,
	Success,
	Warning,
	Debug
};

class Logger
{
public:
	Logger( bool to_file = false );
	~Logger();

	auto LogInfo( const char* format, ... ) -> void;
	auto LogError( const char* format, ... ) -> void;
	auto LogSuccess( const char* format, ... ) -> void;
	auto LogWarning( const char* format, ... ) -> void;
	auto LogDebug( const char* format, ... ) -> void;

private:
	std::ofstream _log_file{};

	HANDLE _console_o_out, _console_o_in, _console_o_err;
	HANDLE _console_out, _console_in, _console_err;

	auto AttachConsole() -> void;
	auto DetachConsole() -> void;

	auto Log( LogLevel loglevel, const char* format, va_list args ) -> void;
};