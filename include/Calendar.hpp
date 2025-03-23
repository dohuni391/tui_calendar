#ifndef CALENDAR_H
#define CALENDAR_H

#include <vector>
#include <iostream>
#include <fstream>
#include "Event.hpp"
#include "Date.hpp"

class Calendar {
private:
    static std::vector<Event> s_events; // events are stored statically
    std::string _user;
    Date _curDate;

public:
    Calendar();
    Calendar(const std::string& user);

    std::string getUser() const;
    Date& getCurDate();
    std::vector<Event>& getAllEvents() const;
    std::vector<int> getEventIndice(const Date& date) const;
    std::vector<int> getEventIndice(const Date& date, const std::string& title) const;
    std::vector<int> getCurDateEventIndice() const;

    //sets new user, loads events from file if user exists
    void setUser(const std::string& user);

    void setCurDate(const Date& date);
    void addEvent(const Event& event);
    void removeEvent(const Event& event);
    void removeEvent(const int& index);
    void removeEvents(const std::vector<int>& indices);
    void removeAllEvents();
    
    //save and load events to/from file
    void saveToFile() const;
    void loadFromFile();

    void printAllEvents() const;
    void printEvents(const Date& date) const;
    void printCurDateEvents() const;
    void printCalendar() const;

    void welcome();
    void reset();
};

#endif // CALENDAR_H
