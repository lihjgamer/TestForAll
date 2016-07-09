#pragma once

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#endif
#include <fstream>

namespace lihj
{

	// 功能开关 取消定义则不输出
	#define USE_COLOR_TEXT

	// 输出消息 最大字节数
	#define MAX_STRING_BUF 1024

	// Linux 
	#ifndef WIN32
	#define	FOREGROUND_RED		1		
	#define FOREGROUND_GREEN	2		
	#define FOREGROUND_BLUE		4
	#endif // !WIN32

	typedef enum
	{
		Console_Color_Default = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,
		Console_Color_Green = FOREGROUND_GREEN,
		Console_Color_Yellow = FOREGROUND_RED | FOREGROUND_GREEN,

		Console_Color_Red = FOREGROUND_RED,

		Console_Color_Blue = FOREGROUND_BLUE,

		Console_Color_Cyan = FOREGROUND_BLUE | FOREGROUND_GREEN,
		Console_Color_Magenta = FOREGROUND_RED | FOREGROUND_BLUE,
	}enumConsoleColor;

	typedef enum
	{
		Log_Level_Info,		// 默认颜色分别对应 enumConsoleColor 颜色的顺序
		Log_Level_Debug,
		Log_Level_Debug1,
		Log_Level_Error,
		Log_Level_Fatal,
		Log_Level_Max,		
	}enumLogLevel;



	class CLogger
	{
	public:
		virtual void Info(const char* format, ...);
		virtual void Debug(const char* format, ...);
		virtual void Debug1(const char* format, ...);
		virtual void Error(const char* format, ...);
		virtual void Fatal(const char* format, ...);

		void SetLogLevelColor(enumLogLevel log_level, enumConsoleColor console_color);
		enumConsoleColor GetLogLevelColor(enumLogLevel log_level);

		void SetIntensifyColor(bool val);
	    bool GetIntensifyColor();
	private:
	#ifdef WIN32
		HANDLE hConsole;
	#endif
		enumLogLevel  m_log_level;
		std::fstream m_out_put_file;
		char *       m_pStringBuf;
		bool		 m_bIntensifyColor;	// 控制台文字颜色加深

		enumConsoleColor m_LogLevelColor[Log_Level_Max];

		void GenerateToString(const char* format, va_list& args);
		void PrintToConsole(enumLogLevel log_level);
		void PrintToFile(enumLogLevel log_level);
	public:
		//CLogger();
		// 构造函数 默认 以 可读/可写/追加 方式打开文件 如果想使用情况方式 可以设置模式为 std::ios::in|std::ios::out|std::ios::trunc 覆盖原有文件
		CLogger(enumLogLevel log_level = Log_Level_Debug, const char* filename = "log.log", std::ios_base::openmode = std::ios::in | std::ios::out | std::ios::app);

		virtual ~CLogger(void);
	};

	#ifdef USE_COLOR_TEXT
	extern CLogger g_logger;
	#define LogInfo(format,...) g_logger.Info(format,##__VA_ARGS__)
	#define LogDebug(format,...) g_logger.Debug(format,##__VA_ARGS__)
	#define LogDebug1(format,...) g_logger.Debug1(format,##__VA_ARGS__)
	#define LogError(format,...) g_logger.Error(format,##__VA_ARGS__)
	#define LogFatal(format,...) g_logger.Fatal(format,##__VA_ARGS__)

	#else
 
	#define LogInfo(format,...) //g_clr_output.PrintMsg(RED_COLOR_MSG,format,##__VA_ARGS__)
	#define LogDebug(format,...) //g_clr_output.PrintMsg(BLUE_COLOR_MSG,format,##__VA_ARGS__)
	#define LogError(format,...) //g_clr_output.PrintMsg(GREEN_COLOR_MSG,format,##__VA_ARGS__)
	#define LogFatal(format,...) //g_clr_output.PrintMsg(DEFAULT_COLOR_MSG,format,##__VA_ARGS__)
	#endif

}
