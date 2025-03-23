#ifndef DATE_HPP
#define DATE_HPP

#include <string>
#include <vector>
#include <ctime>
#include <sstream>
#include <iomanip>
#include "Time.hpp"

class Date {
private:
    int _day;
    int _month;
    int _year;
    Time _time; //Not Implemented Yet
    int _timeZone; //Not Implemented Yet

public:
    Date();
    Date(const int& day, const int& month, const int& year);
    Date(const std::string& date);

    const int getDay() const;
    const int getMonth() const;
    const int getYear() const;

    const std::string toString() const;

    void setDay(int day);
    void setMonth(int month);
    void setYear(int year);

    void setDate(const int& day, const int& month, const int& year);
    void setDateFromString(const std::string& date);

    void addDays(const int& days);
    void addWeeks(const int& weeks);
    void addMonths(const int& months);
    void addYears(const int& years); 

    //adds the interval to the date in the specified frequency(Recurrence Rule)
    //frequency: 0,1 - daily, 2 - weekly, 3 - monthly, 4 - yearly
    void add(const int& interval, const int& frequency);

    static const bool isLeapYear(const int& year);
    static const int countLeapYears(const int& year);

    static const int getDaysOfMonth(const int& month, const int& year);
    static const int getDaysOfMonth(const Date& date);

    static const int getTotalDays(const int& day, const int& month, const int& year);
    static const int getTotalDays(const Date& date);

    static const int getDayOfWeek(const int& day, const int& month, const int& year);
    static const int getDayOfWeek(const Date& date);

    static const std::string getDayOfWeekName(const int& index);
    static const std::string getMonthName(const int& index);
    
    static Date getToday();
    
    //gets the date from the string in the format of
    //"%d %B %Y", "%B %d %Y", "%Y %B %d"
    //"%d %m %Y", "%d %m %y", "%Y %m %d"
    //"%d/%m/%Y", "%B %d", "%d %B"
    static Date& fromString(const std::string& str, Date& date);
    static Date fromString(const std::string& str);

    bool operator==(const Date& date) const;
    bool operator!=(const Date& date) const;
    bool operator<(const Date& date) const;
    bool operator>(const Date& date) const;
    bool operator<=(const Date& date) const;
    bool operator>=(const Date& date) const;
};

#endif // DATE_HPP
