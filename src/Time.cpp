#include "../include/Time.hpp"

Time::Time(int h, int m, int tz) : _hours(h), _minutes(m), _timezoneOffset(tz) {
    if (h < 0 || h >= 24 || m < 0 || m >= 60) {
        throw std::invalid_argument("Invalid time provided");
    }
}

void Time::setHours(int h) {
    if (h < 0 || h >= 24) {
        throw std::invalid_argument("Invalid hours provided");
    }
    _hours = h;
}
void Time::setMinutes(int m) {
    if (m < 0 || m >= 60) {
        throw std::invalid_argument("Invalid minutes provided");
    }
    _minutes = m;
}
void Time::setTimezoneOffset(int tz) {
    _timezoneOffset = tz;
    _hours += _timezoneOffset;
    if (_hours >= 24) {
        _hours -= 24;
    } else if (_hours < 0) {
        _hours += 24;
    }
}

void Time::addHours(int h) {
    _hours += h;
    if (_hours >= 24) {
        _hours = 24;
        _minutes = 0;
    } else if (_hours < 0) {
        _hours = 0;
    }
}
void Time::addMinutes(int m) {
    int totalMinutes = _minutes + m;
    int additionalHours = totalMinutes / 60;
    _minutes = totalMinutes % 60;
    addHours(additionalHours);
}

int Time::getHours() const {
    return _hours;
}
int Time::getMinutes() const {
    return _minutes;
}
int Time::getTimezoneOffset() const {
    return _timezoneOffset;
}

Time Time::fromString(const std::string& str) {
    int h, m;
    char sep;
    std::istringstream iss(str);
    if (!(iss >> h >> sep >> m) || sep != ':' || h < 0 || h >= 24 || m < 0 || m >= 60) {
        throw std::invalid_argument("Invalid time string format");
    }
    return Time(h, m);
}
std::string Time::toString() const {
    std::ostringstream oss;
    oss << std::setw(2) << std::setfill('0') << _hours << ":"
        << std::setw(2) << std::setfill('0') << _minutes;
    return oss.str();
}