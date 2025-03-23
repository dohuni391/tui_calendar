#include "../include/Date.hpp"

Date::Date() : _day(1), _month(1), _year(1900) {
    
}
Date::Date(const int& day, const int& month, const int& year) : _day(day), _month(month), _year(year) {
    if (day < 1 || day > getDaysOfMonth(month, year) || month < 1 || month > 12 || year < 1900) {
        throw std::invalid_argument("Invalid date: " + std::to_string(day) + "/" + std::to_string(month) + "/" + std::to_string(year));
    }
}
Date::Date(const std::string& date) {
    *this = fromString(date);
}

const int Date::getDay() const { return _day; }
const int Date::getMonth() const { return _month; }
const int Date::getYear() const { return _year; }

const std::string Date::toString() const {
    return std::to_string(_day) + "/" + std::to_string(_month) + "/" + std::to_string(_year);
}

void Date::setDay(int day) {
    if (day < 1 || day > getDaysOfMonth(_month, _year)) {
        throw std::invalid_argument("Invalid day: " + std::to_string(day));
    }
    _day = day;
}
void Date::setMonth(int month) {
    if (month < 1 || month > 12) {
        throw std::invalid_argument("Invalid month: " + std::to_string(month));
    }
    if (_day > getDaysOfMonth(month, _year)) {
        _day = getDaysOfMonth(month, _year);
    }
    _month = month;
}
void Date::setYear(int year) {
    if (year < 1900) {
        throw std::invalid_argument("Invalid year: " + std::to_string(year));
    }
    _year = year;
}
void Date::setDate(const int& day, const int& month, const int& year) {
    setDay(day);
    setMonth(month);
    setYear(year);
}
void Date::setDateFromString(const std::string& date) {
    *this = fromString(date, *this);
}

void Date::addDays(const int& days){
    if (days < 0) {
        for (int i = 0; i < -days; ++i) {
            if (_day == 1) {
                addMonths(-1);
                _day = getDaysOfMonth(_month, _year);
            } else {
                _day--;
            }
        }
    } else {
        for (int i = 0; i < days; ++i) {
            if (_day == getDaysOfMonth(_month, _year)) {
                addMonths(1);
                _day = 1;
            } else {
                _day++;
            }
        }
    }
}
void Date::addWeeks(const int& weeks) {
    addDays(weeks * 7);
}
void Date::addMonths(const int& months) {
    if (months < 0) {
        for (int i = 0; i < -months; ++i) {
            if (_month == 1) {
                setMonth(12);
                addYears(-1);
            } else {
                setMonth(_month - 1);
            }
        }
    } else {
        for (int i = 0; i < months; ++i) {
            if (_month == 12) {
                setMonth(1);
                addYears(1);
            } else {
                setMonth(_month + 1);
            }
        }
    }
}
void Date::addYears(const int& years) {
    _year += years;
    if (_year < 1900) {
        throw std::invalid_argument("Invalid year: " + std::to_string(_year));
    }
}
void Date::add(const int& interval, const int& frequency) {
    switch (frequency) {
        case 0:
            addDays(interval);
            break;
        case 1:
            addDays(interval);
            break;
        case 2:
            addWeeks(interval);
            break;
        case 3:
            addMonths(interval);
            break;
        case 4:
            addYears(interval);
            break;
        default:
            throw std::invalid_argument("Invalid type: " + std::to_string(frequency));
    }
}

const bool Date::isLeapYear(const int& year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}
const int Date::countLeapYears(const int& year) {
    if (year <= 1) {
        return 0;
    }
    return year / 4 - year / 100 + year / 400;
}
const int Date::getDaysOfMonth(const int& month, const int& year) {
    if (month == 2) {
        return isLeapYear(year) ? 29 : 28;
    } else if (month == 4 || month == 6 || month == 9 || month == 11) {
        return 30;
    } else {
        return 31;
    }
}
const int Date::getDaysOfMonth(const Date& date) {
    return getDaysOfMonth(date.getMonth(), date.getYear());
}
const int Date::getTotalDays(const int& day, const int& month, const int& year) {
    int totalDays = (year - 1) * 365 + countLeapYears(year);
    for (int i = 1; i < month; i++) {
        totalDays += getDaysOfMonth(i, year);
    }
    return totalDays + day;
}
const int Date::getTotalDays(const Date& date) {
    return getTotalDays(date.getDay(), date.getMonth(), date.getYear());
}
const int Date::getDayOfWeek(const int& day, const int& month, const int& year) {
    return getTotalDays(day, month, year) % 7;
}
const int Date::getDayOfWeek(const Date& date) {
    return getTotalDays(date) % 7;
}

const std::string Date::getDayOfWeekName(const int& index) {
    static const std::string dayNames[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
    return dayNames[index];
}
const std::string Date::getMonthName(const int& index) {
    static const std::string monthNames[] = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
    return monthNames[index];
}

Date& Date::fromString(const std::string& str, Date& date) {
    size_t wordCount = 0;
    std::istringstream iss(str);
    std::string word;
    while (iss >> word) {
        ++wordCount;
    }

    std::vector<std::string> formats = {
        "%d %B %Y", "%B %d %Y", "%Y %B %d",
        "%d %m %Y", "%d %m %y", "%Y %m %d",
        "%d/%m/%Y", "%B %d", "%d %B", "%d"
    };
    int day = 0, month = 0, year = 0;

    std::tm tm = {};
    bool valid = false;

    for (const std::string& format : formats) {
        size_t formatWordCount = std::count(format.begin(), format.end(), ' ') + 1;
        if (formatWordCount != wordCount) {
            continue;
        }

        std::istringstream ss(str);
        ss >> std::get_time(&tm, format.c_str());
        if (!ss.fail()) {
            valid = true;
            day = tm.tm_mday;
            month = tm.tm_mon + 1;
            year = tm.tm_year + 1900;
            if (format == "%B %d" || format == "%d %B") {
                year = date.getYear();
            } else if (format == "%d") {
                month = date.getMonth();
                year = date.getYear();
            }
            break;
        }
    }

    if (!valid) {
        throw std::invalid_argument("Invalid date format: " + str);
    }
    
    date.setDate(day, month, year);
    return date;
}
Date Date::fromString(const std::string& str) {
    Date date = getToday();
    return fromString(str, date);
}

Date Date::getToday() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    return Date(ltm->tm_mday, ltm->tm_mon + 1, ltm->tm_year + 1900);
}

bool Date::operator==(const Date& date) const {
    return _day == date.getDay() && _month == date.getMonth() && _year == date.getYear();
}
bool Date::operator!=(const Date& date) const {
    return !(*this == date);
}
bool Date::operator<(const Date& date) const {
    if (_year < date.getYear()) {
        return true;
    } else if (_year > date.getYear()) {
        return false;
    } else if (_month < date.getMonth()) {
        return true;
    } else if (_month > date.getMonth()) {
        return false;
    } else {
        return _day < date.getDay();
    }
}
bool Date::operator>(const Date& date) const {
    return !(*this < date || *this == date);
}
bool Date::operator<=(const Date& date) const {
    return *this < date || *this == date;
}
bool Date::operator>=(const Date& date) const {
    return *this > date || *this == date;
}
