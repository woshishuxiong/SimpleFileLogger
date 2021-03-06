#include "SystemWrap.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <time.h>
#include "CriticalSection.h"
#ifdef __ANDROID__
#include <android/log.h>
#endif


#define FILE_LOGGING 1
#define CONSOLE_LOGGING 1

namespace SimpleFileLogger {

#if defined(FILE_LOGGING) || defined(CONSOLE_LOGGING)
    static CriticalSection LogLock;
    const int LogBufferSize = 10240;
    static char LogBuffer[LogBufferSize];

    static FILE* logFile = NULL;
#endif


    void LogInit(const char* path) 
    {
#ifdef FILE_LOGGING
        logFile = fopen(path, "a+");
#endif
    }

    void LogRelease()
    {
#ifdef FILE_LOGGING
        if (logFile)
            fclose(logFile);
#endif

    }

    void Trace(const char *fmt, ...)
    {
#if defined(FILE_LOGGING) || defined(CONSOLE_LOGGING)
        CriticalScope scope_lock(&LogLock);
        {
            va_list argptr;
            va_start(argptr, fmt);
            vsnprintf(LogBuffer, LogBufferSize - 1, fmt, argptr);
            LogBuffer[LogBufferSize - 1] = 0;
            va_end(argptr);

            static char tmp[64];
    #if defined(_WIN32) || defined(WIN32)
            SYSTEMTIME lpSystemTime;
            GetLocalTime(&lpSystemTime);
            sprintf(tmp, "[%04d%02d%02d %02d:%02d:%02d:%03d] ", lpSystemTime.wYear, lpSystemTime.wMonth, lpSystemTime.wDay, lpSystemTime.wHour, lpSystemTime.wMinute, lpSystemTime.wSecond, lpSystemTime.wMilliseconds);

    #else
            time_t t = time(NULL);
            struct tm* tmx;
            tmx = localtime(&t);
            int year = tmx->tm_year + 1900;
            int month = tmx->tm_mon + 1;
            int mday = tmx->tm_mday;
            sprintf(tmp, "[%04d%02d%02d-%02d:%02d:%02d] ", year, month, mday);
    #endif 


    #ifdef FILE_LOGGING
            if (logFile) {
                fwrite(tmp, strlen(tmp), 1, logFile);
                fwrite(LogBuffer, strlen(LogBuffer), 1, logFile);
                fwrite("\n", 1, 1, logFile);
                fflush(logFile);
            }
    #endif

    #if defined(__ANDROID__)
            __android_log_print(ANDROID_LOG_INFO, "SimpleFileLogger", "%s \n", LogBuffer);
    #else
            printf(tmp);
            printf("%s \n", LogBuffer);
    #endif
        }
#endif
    }

}

