#include "logger.h"
#include <stdio.h>
#include <cstdlib>
#include <stdarg.h>
#include <string.h>

#pragma warning(disable:4996)

	namespace lihj
	{
		#ifdef USE_COLOR_TEXT
			CLogger g_logger;
		#endif

		// 构造函数 默认 以 可读/可写/追加 方式打开文件 如果想使用情况方式 可以设置模式为 std::ios::in|std::ios::out|std::ios::trunc 覆盖原有文件
		CLogger::CLogger(enumLogLevel log_level,const char* filename /*= "log.log"*/,std::ios_base::openmode  opentype /*= std::ios::in|std::ios::out|std::ios::app*/)
		{
		#ifdef WIN32
			hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		#endif
	
			m_log_level = log_level;
	
			m_out_put_file.open(filename,opentype);
			m_pStringBuf = new char[MAX_STRING_BUF];
			
			m_bIntensifyColor = true;

			int colorarray[] = 
			{
				Console_Color_Default,
				Console_Color_Green,
				Console_Color_Cyan ,
				Console_Color_Magenta,
				Console_Color_Red ,
				Console_Color_Blue,
	
				Console_Color_Yellow,
			};
	
			for (int log_level_begin = (int)Log_Level_Info;
				log_level_begin < (int)Log_Level_Max;
				log_level_begin ++)
			{
				m_LogLevelColor[log_level_begin] = (enumConsoleColor)colorarray[log_level_begin];
			}
	
		}
	
		CLogger::~CLogger(void)
		{
		#ifdef WIN32
			CloseHandle(hConsole);
		#endif
		
			m_out_put_file.close();
	
			if (m_pStringBuf != NULL)
			{
				delete []m_pStringBuf;
				m_pStringBuf = NULL;
			}
		}
	
		void CLogger::GenerateToString(const char* format, va_list& args)
		{
			//va_start(args, format);
			int len = vsnprintf(NULL, 0, format, args) + 1;
			if (len > MAX_STRING_BUF)
			{
				fprintf(stderr, "Error : String Length Large Then String Buffer Length %d\n", MAX_STRING_BUF);
				sprintf(m_pStringBuf, "Error : String Length Large Then String Buffer Length %d\n", MAX_STRING_BUF);
				return;
			}
			vsprintf(m_pStringBuf, format, args);
		}
	
		void CLogger::PrintToConsole(enumLogLevel log_level)
		{
		#ifdef WIN32
			SetConsoleTextAttribute(hConsole, m_bIntensifyColor ? m_LogLevelColor[log_level] + FOREGROUND_INTENSITY : m_LogLevelColor[log_level]);
			printf("%s",m_pStringBuf);
			SetConsoleTextAttribute(hConsole, Console_Color_Default);
		#else
			printf("\033[%d;%dm%s\033[0m", (int)m_LogLevelColor[log_level],m_bIntensifyColor, m_pStringBuf);
		#endif
		}
	
		void CLogger::PrintToFile(enumLogLevel log_level)
		{
			if (m_log_level <= log_level)
			{
				m_out_put_file.write(m_pStringBuf, strlen(m_pStringBuf));
				m_out_put_file.flush();
			}
		}
	
		void CLogger::Debug(const char* format, ...)
		{
			va_list args;
			va_start(args, format);
	
			GenerateToString(format, args);
			PrintToConsole(Log_Level_Debug);
			PrintToFile(Log_Level_Debug);
		}

		void CLogger::Debug1(const char* format, ...)
		{
			va_list args;
			va_start(args, format);

			GenerateToString(format, args);
			PrintToConsole(Log_Level_Debug1);
			PrintToFile(Log_Level_Debug);
		}
	
		void CLogger::Info(const char* format, ...)
		{
			va_list args;
			va_start(args, format);
	
			GenerateToString(format, args);
			PrintToConsole(Log_Level_Info);
			PrintToFile(Log_Level_Info);
		}
	
		void CLogger::Error(const char* format, ...)
		{
			va_list args;
			va_start(args, format);
	
			GenerateToString(format, args);
			PrintToConsole(Log_Level_Error);
			PrintToFile(Log_Level_Error);
		}
	
		void CLogger::Fatal(const char* format, ...)
		{
			va_list args;
			va_start(args, format);
	
			GenerateToString(format, args);
			PrintToConsole(Log_Level_Fatal);
			PrintToFile(Log_Level_Fatal);
		}
	
		void CLogger::SetLogLevelColor(enumLogLevel log_level, enumConsoleColor console_color)
		{
			m_LogLevelColor[log_level] = console_color;
		}
	
		enumConsoleColor CLogger::GetLogLevelColor(enumLogLevel log_level)
		{
			return m_LogLevelColor[log_level];
		}

		void CLogger::SetIntensifyColor(bool val)
		{
			m_bIntensifyColor = val;
		}

		bool CLogger::GetIntensifyColor()
		{
			return m_bIntensifyColor;
		}

	}

