#include "Logger.h"

#include <cstdio>
#include <iostream>
#include <fstream>
#include <cstdarg>

/* kolorowanie w konsoli */
#ifdef PLATFORM_WIN32
#   include <windows.h>
#endif

/* domyslna instancja */
Logger Logger::mGlobalInstance(NULL, Logger::AUTO_FLUSH | Logger::WITH_PREFIXES | Logger::WITH_COLORS);


Logger::Logger(const char* filename, unsigned options)
{
    mOptions = options;

    if (filename == NULL)
    {
        mLogFile = stderr;
        mLogStream = &std::cerr;
    }
    else
    {
        mLogFile = fopen(filename, "w");
        mLogStream = new std::ofstream(filename);

        if (!mLogFile)
            fprintf(stderr, "Fatal error: Logger couldn't open file %s (handle)\n", filename);
        if (!mLogStream->good())
            fprintf(stderr, "Fatal error: Logger couldn'y open file %s (stream)\n", filename);
    }

    mCurrentStreamSeverity = MESSAGE;
}

Logger::~Logger()
{
    if (mLogFile && mLogFile != stderr)
        fclose(mLogFile);
    if (mLogStream && mLogStream != &std::cerr)
        delete mLogStream;

    /* przywrocenie kolorow */
    setupColors(MESSAGE);
}


void Logger::log(Severity severity, const char* format, ...)
{
    assert(mLogFile && "Invalid log file");

    if (mOptions & WITH_COLORS)
        setupColors(severity);
    if (mOptions & WITH_PREFIXES)
        fputs(getPrefix(severity), mLogFile);

    va_list list;
    va_start(list, format);
    vfprintf(mLogFile, format, list);
    va_end(list);

    if (mOptions & AUTO_FLUSH)
        fflush(mLogFile);
}


void Logger::setupColors(Severity severity)
{
#ifdef PLATFORM_LINUX
    static const char* colors[] = {
        "\033\[0;37m",  /* DEBUG - bialy */
        "\033\[0;37m",  /* MESSAGE - bialy */
        "\033\[0;33m",  /* WARNING - zolty */
        "\033\[0;31m"   /* ERROR - czerwony */
    };

    fputs(colors[(int)severity], mLogFile);
#elif defined(PLATFORM_WIN32)
    HANDLE hConsole = 0;
    if (mLogFile == stderr)
        hConsole = ::GetStdHandle(STD_ERROR_HANDLE);
    else if (mLogFile == stdout)
        hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE);

    static WORD colors[] = {
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,    /* DEBUG - bialy */
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE,    /* MESSAGE - bialy */
        FOREGROUND_RED | FOREGROUND_GREEN,                      /* WARNING - zolty */
        FOREGROUND_RED                                          /* ERROR - czerwony */
    };

    if (hConsole)
        ::SetConsoleTextAttribute(hConsole, colors[(int)severity]);
#endif
}

const char* Logger::getPrefix(Severity severity)
{
    static const char* prefixes[] = {
        "",             /* DEBUG */
        "",             /* MESSAGE */
        "warning: ",    /* WARNING */
        "ERROR: "       /* ERROR */
    };

    return prefixes[(int)severity];
}

