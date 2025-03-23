#ifndef TIME_H
#define TIME_H

#include <string>
#include <sstream>
#include <iomanip>

class Time {
private:
    int _hours;
    int _minutes;
    int _timezoneOffset;

public:
    Time(int h = 0, int m = 0, int tz = 0);

    void setHours(int h);
    void setMinutes(int m);
    void setTimezoneOffset(int tz);

    void addHours(int h);
    void addMinutes(int m);

    int getHours() const;
    int getMinutes() const;
    int getTimezoneOffset() const;

    Time fromString(const std::string& str);
    std::string toString() const;
};

#endif // TIME_H
