#include "Logger.h"
#include "Utilities.h"
#include "WinApi.h"

static std::string _log_level_strings[] =
{
	"Info",
	"Error",
	"Success",
	"Warning",
	"Debug"
};

Logger::Logger( bool to_file )
{
	AttachConsole();

	if ( !to_file )
		return;

	char file_path[ MAX_PATH ];
	if ( !Utils::GetCurrentDir( file_path ) )
		return;
	if ( !Utils::CreateFolder( file_path, "\\logs\\" ) )
		return;

	strcat_s( file_path, MAX_PATH, "Log" );
	strcat_s( file_path, MAX_PATH, Utils::GetTimeStamp( false, true ).c_str() );
	strcat_s( file_path, MAX_PATH, ".txt" );

	_log_file.open( file_path, std::ios::out | std::ios::app );
}

Logger::~Logger()
{
	DetachConsole();

	if ( !_log_file.is_open() )
		return;

	_log_file.flush();
	_log_file.close();
}

auto Logger::AttachConsole() -> void
{
	_console_o_out = LI( GetStdHandle )( STD_OUTPUT_HANDLE );
	_console_o_err = LI( GetStdHandle )( STD_ERROR_HANDLE );
	_console_o_in = LI( GetStdHandle )( STD_INPUT_HANDLE );

	LI( AllocConsole )() && ::AttachConsole ( LI( GetCurrentProcessId )() );

	_console_out = LI( GetStdHandle )( STD_OUTPUT_HANDLE );
	_console_err = LI( GetStdHandle )( STD_ERROR_HANDLE );
	_console_in = LI( GetStdHandle )( STD_INPUT_HANDLE );

	LI( SetConsoleMode )( _console_out, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT );
	LI( SetConsoleMode )( _console_in, ENABLE_INSERT_MODE | ENABLE_EXTENDED_FLAGS | ENABLE_PROCESSED_INPUT | ENABLE_QUICK_EDIT_MODE );
}

auto Logger::DetachConsole() -> void
{
	if ( !_console_out || !_console_err || !_console_in )
		return;

	LI( FreeConsole );

	if ( _console_o_out )
		LI( SetStdHandle )( STD_OUTPUT_HANDLE, _console_o_out );
	if ( _console_o_err )
		LI( SetStdHandle )( STD_ERROR_HANDLE, _console_o_err );
	if ( _console_o_in )
		LI( SetStdHandle )( STD_INPUT_HANDLE, _console_o_in );
}

auto Logger::LogInfo( const char* format, ... ) -> void
{
	va_list args;

	va_start( args, format );

	Log( LogLevel::Info, format, args );

	va_end( args );
}

auto Logger::LogError( const char* format, ... ) -> void
{
	va_list args;

	va_start( args, format );

	Log( LogLevel::Error, format, args );

	va_end( args );
}

auto Logger::LogSuccess( const char* format, ... ) -> void
{
	va_list args;

	va_start( args, format );

	Log( LogLevel::Success, format, args );

	va_end( args );
}

auto Logger::LogWarning( const char* format, ... ) -> void
{
	va_list args;

	va_start( args, format );

	Log( LogLevel::Warning, format, args );

	va_end( args );
}

auto Logger::LogDebug( const char* format, ... ) -> void
{
#if _DEBUG
	va_list args;

	va_start( args, format );

	Log( LogLevel::Debug, format, args );

	va_end( args );
#endif
}

auto Logger::Log( LogLevel loglevel, const char* format, va_list args ) -> void
{
	auto buf_size = _vscprintf( format, args ) + 1;
	auto buf = std::make_unique< char[] >( buf_size );
	vsprintf_s( buf.get(), buf_size, format, args );

	auto timestamp = Utils::GetTimeStamp( true );
	auto loglevel_s = _log_level_strings[ loglevel ];

	auto padding = 7 - loglevel_s.length();
	if ( padding > 0 )
		loglevel_s.insert( loglevel_s.begin(), padding, ' ' );

	auto message_size = timestamp.length() + loglevel_s.length() + buf_size + 8;
	auto message = std::make_unique< char[] >( message_size );
	sprintf_s( message.get(), message_size, "[%s] %s| %s\r\n", timestamp.c_str(), loglevel_s.c_str(), buf.get() );

	if ( _log_file.is_open() )
		_log_file << message;
	if ( _console_out == INVALID_HANDLE_VALUE )
		return;

	auto cs_buf = CONSOLE_SCREEN_BUFFER_INFO{};
	LI( GetConsoleScreenBufferInfo )( _console_out, &cs_buf );

	switch (loglevel)
	{
	case LogLevel::Error:
		LI( SetConsoleTextAttribute )( _console_out, FOREGROUND_RED | FOREGROUND_INTENSITY );
		break;
	case LogLevel::Success:
		LI( SetConsoleTextAttribute )( _console_out, FOREGROUND_GREEN | FOREGROUND_INTENSITY );
		break;
	case LogLevel::Warning:
		LI( SetConsoleTextAttribute )( _console_out, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY );
		break;
	case LogLevel::Debug:
		LI( SetConsoleTextAttribute )( _console_out, FOREGROUND_RED | FOREGROUND_GREEN );
		break;
	default:
		break;
	}

	LI( WriteConsoleA )( _console_out, message.get(), ( DWORD )strlen( message.get() ), nullptr, nullptr );
	LI( SetConsoleTextAttribute )( _console_out, cs_buf.wAttributes );
}