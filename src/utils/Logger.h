#ifndef SRC_HEADERS_LOGGER_H
#define SRC_HEADERS_LOGGER_H

#include <cassert>
#include <ostream>

/* niech zyje windows i jego wszedobylskie makra */
#ifdef ERROR
#   undef ERROR
#endif /* ERROR */

/**
 * \brief Klasa loggera.
 *
 * Umozliwia zapisywanie logow o roznym stopniu "waznosci"
 * Zawiera makro Log, rozwijajace sie do Logger::get(), zwracajacej domyslna instancje
 * loggera. Przykladowe uzycie:
 *
 * - printf-like (obsluguje wszystkie %%* jak normalny printf)
 * \code
 *   Log.log(Logger::MESSAGE, "jakis string %s z numerkiem %d\n", "string", 42);
 * \endcode
 *
 * - cout-like (obsluguje manipulatory typu endl)
 * \code
 *   Log << Logger::ERROR << "och nie, wkradl sie blad" << std::endl;
 * \endcode
 *
 * Mozliwe wartosci "waznosci" komunikatow - patrz: Logger::Severity.
 *
 * Przy tworzeniu obiektu loggera mozna podac opcje wplywajace na jego zachowanie.
 * Szczegoly: Logger::Options.
 */
class Logger
{
public:
    /**
     * \brief Poziom waznosci wiadomosci.
     *
     * Kontroluje kolorowanie i/lub przedrostki w logowanych wiadomosciach.
     */
    enum Severity
    {
        DEBUG = 0,  /**< Wiadomosc do debugu, nie pojawiajaca sie w buildzie release. Kolor: bialy, przedrostek: brak */
        MESSAGE,    /**< Standardowa wiadomosc, kolor: bialy, przedrostek: brak */
        WARNING,    /**< Ostrzezenie, kolor: zolty, przedostek: "warning: " */
        ERROR       /**< Blad, kolor: czerwony, przedrostek: "ERROR: " */
    };

    /**
     * \brief Opcje sterujace zachowaniem loggera.
     */
    enum Options
    {
        AUTO_FLUSH = 1 << 0,    /**< W kazdym wywolaniu log() wywolaj fflush() */
        WITH_COLORS = 1 << 1,   /**< Ustawiaj kolor tekstu przed wypisaniem */
        WITH_PREFIXES = 1 << 2  /**< Dopisuj przedrostki podczas wypisywania */
    };

    /**
     * \brief Konstruktor.
     *
     * \param filename Nazwa pliku, do ktorego zapisac dane. W przypadku podania NULLa
     *                 logi beda szly na stderr.
     * \param options Opcje loggera - kombinacja flag Options.
     */
    Logger(const char* filename, unsigned options);

    /**
     * \brief Destruktor.
     *
     * Zamyka otwarty plik, jesli jest taka potrzeba.
     */
    ~Logger();


    /**
     * \brief Funkcja logujaca.
     *
     * Akceptuje takie same parametry jak printf.
     *
     * \param severity Waznosc wiadomosci.
     * \param format Format string (patrz: printf z cstdlib)
     */
    void log(Severity severity, const char* format, ...);

    /**
     * \brief Operator strumieniowy.
     *
     * Alternatywny sposob na wypisywanie logow. Aktualizuje kolor wg
     * aktualnego ustawionego dla operatora strumieniowego.
     *
     * \param value Tresc wiadomosci.
     * \returns Referencja na modyfikowany obiekt loggera.
     */
    template<typename T> inline Logger& operator <<(const T& value)
    {
#ifndef _DEBUG
        /* w przypadku buildu release, pomijamy wiadomosci typu debug */
        if (mCurrentStreamSeverity == DEBUG)
            return *this;
#endif

        assert(mLogStream && mLogStream->good() && "Invalid log stream");

        if (mOptions & WITH_COLORS)
            setupColors(mCurrentStreamSeverity);

        (*mLogStream) << value;
        if (mOptions & AUTO_FLUSH)
            mLogStream->flush();

        return *this;
    }

    /**
     * \brief Przecizony operator strumieniowy, do zmiany waznosci wiadomosci.
     *
     * Podany tu poziom bedzie obowiazywal dla wszystkich wywolan operatora <<.
     * Wypisuje prefiks i zmienia kolor.
     *
     * \param severity Nowy poziom waznosci wiadomosci.
     * \returns Referencja na modyfikowany obiekt loggera.
     */
    inline Logger& operator <<(const Severity& severity)
    {
        mCurrentStreamSeverity = severity;

        if (mOptions & WITH_COLORS)
            setupColors(mCurrentStreamSeverity);
        if (mOptions & WITH_PREFIXES)
            (*mLogStream) << getPrefix(mCurrentStreamSeverity);

        return *this;
    }

    /**
     * \brief Przecizony operator strumieniowy, do obslugi manipulatorow STLa
     *
     * \param manipulator Manipulator strumieniowy
     * \returns Referencja na modyfikowany obiekt loggera.
     */
    inline Logger& operator <<(std::ostream& (*manipulator)(std::ostream&))
    {
        assert(mLogStream && mLogStream->good() && "Invalid log stream");
        
        (*mLogStream) << manipulator;

        return *this;
    }

    /**
     * \brief Funkcja zwracajaca domyslna instancje loggera.
     *
     * \returns Domyslna instancja loggera.
     */
    static inline Logger& get()
    {
        return mGlobalInstance;
    }

private:
    /**
     * \brief Ustawia kolor tekstu w konsoli na podstawie podanego poziomu waznosci.
     *
     * \param severity Poziom waznosci.
     */
    void setupColors(Severity severity);

    /**
     * \brief Zwraca prefiks powiazany z podanym poziomem waznosci.
     *
     * \param severity Poziom waznosci.
     */
    const char* getPrefix(Severity severity);

    /** Domyslna instancja */
    static Logger mGlobalInstance;

    /** Opcje loggera */
    unsigned mOptions;

    /** Plik loga - dwa sposoby dostepu */
    FILE* mLogFile;
    std::ostream* mLogStream;

    /** Aktualny poziom waznosci wiadomosci przekazywanych operatorem << */
    Severity mCurrentStreamSeverity;
};

#define Log Logger::get()

#endif /* SRC_HEADERS_LOGGER_H */
