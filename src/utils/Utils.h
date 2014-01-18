#ifndef SRC_UTILS_UTILS_H
#define SRC_UTILS_UTILS_H

#include <string>
#include <sstream>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

namespace Utils
{
    float random(float min, float max);
    float randomVariation(float center, float variation);

    template<typename T> T clamp(T val, T min, T max)
    {
        return (val < min) ? min : ((val > max) ? max : val);
    }

    template<typename T> const std::string toString(T val)
    {
        std::ostringstream ss;
        ss << val;
        return ss.str();
    }

    template<typename T> T parse(const std::string& str)
    {
        std::istringstream ss;
        T ret;

        ss.str(str);
        ss >> ret;
        return ret;
    }
}

#endif /* SRC_UTILS_UTILS_H */
