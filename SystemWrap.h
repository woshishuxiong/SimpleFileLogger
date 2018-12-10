#ifndef __SYSTEMWRAP_H__
#define __SYSTEMWRAP_H__


#include <stdio.h>
#include <assert.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>


namespace SimpleFileLogger {

    void LogInit(const char* path);

    void LogRelease();

    void Trace(const char *fmt, ...);
}

#endif


