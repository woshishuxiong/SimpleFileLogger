
#pragma once

#if defined(_WIN32) || defined(WIN32)
#include <windows.h>
#elif defined(__unix) || defined(unix) || defined(__MACH__)
#include <unistd.h>
#include <pthread.h>
#endif

#if defined(_WIN32) || defined(WIN32)
#define IMUTEX_TYPE         CRITICAL_SECTION
#define IMUTEX_INIT(m)      InitializeCriticalSection((CRITICAL_SECTION*)(m))
#define IMUTEX_DESTROY(m)   DeleteCriticalSection((CRITICAL_SECTION*)(m))
#define IMUTEX_LOCK(m)      EnterCriticalSection((CRITICAL_SECTION*)(m))
#define IMUTEX_UNLOCK(m)    LeaveCriticalSection((CRITICAL_SECTION*)(m))

#elif defined(__unix) || defined(unix) || defined(__MACH__)
#include <unistd.h>
#include <pthread.h>
#define IMUTEX_TYPE         pthread_mutex_t
#define IMUTEX_INIT(m)      pthread_mutex_init((pthread_mutex_t*)(m), 0)
#define IMUTEX_DESTROY(m)   pthread_mutex_destroy((pthread_mutex_t*)(m))
#define IMUTEX_LOCK(m)      pthread_mutex_lock((pthread_mutex_t*)(m))
#define IMUTEX_UNLOCK(m)    pthread_mutex_unlock((pthread_mutex_t*)(m))
#endif


namespace SimpleFileLogger 
{
    class CriticalSection
    {
    public:
        CriticalSection() { IMUTEX_INIT(&_mutex); }
        virtual ~CriticalSection() { IMUTEX_DESTROY(&_mutex); }
        void enter() { IMUTEX_LOCK(&_mutex); }
        void leave() { IMUTEX_UNLOCK(&_mutex); }

        IMUTEX_TYPE& mutex() { return _mutex; }
        const IMUTEX_TYPE& mutex() const { return _mutex; }

    protected:
        IMUTEX_TYPE _mutex;
    };



    class CriticalScope
    {
    public:
        CriticalScope(CriticalSection *c): _critical(c) 
        { 
            if (_critical)
            {
                _critical->enter();
            }
        }

        virtual ~CriticalScope() 
        { 
            if (_critical)
            {
                _critical->leave();
                _critical = NULL;
            }
        }

    protected:
        CriticalSection *_critical;
    };

};
