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

    #ifdef FILE_LOGGING
            if (logFile) {
                fwrite(LogBuffer, strlen(LogBuffer), 1, logFile);
                fwrite("\n", 1, 1, logFile);
                fflush(logFile);
            }
    #endif

    #if defined(__ANDROID__)
            __android_log_print(ANDROID_LOG_INFO, "SimpleFileLogger", "%s \n", LogBuffer);
    #else
            printf("%s \n", LogBuffer);
    #endif
        }
#endif
    }

}

