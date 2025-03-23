#ifndef EVENT_HPP
#define EVENT_HPP

#include <string>
#include <vector>
#include <optional>
#include "Date.hpp"
#include "RecurrenceRule.hpp"

class Event {
private:
    std::string _title;
    std::string _description;
    Date _startDate;
    Date _endDate;
    RecurrenceRule _recurrenceRule;
    int _priority;

public:
    Event(const std::string& title, const std::string& description, const Date& startDate, const Date& endDate, const int& priority, const RecurrenceRule& recurrenceRule);

    const std::string& getTitle() const;
    const Date& getStartDate() const;
    const Date& getEndDate() const;
    const RecurrenceRule& getRecurrenceRule() const;
    const std::string& getDescription() const;
    const int& getPriority() const;

    const bool isOneDayEvent() const;
    const bool hasDescription() const;
    const bool isRecurringEvent() const;
    
    //gets all dates of the event
    const std::vector<Date> getDates() const;
    
    const std::vector<Date> getRangeDates() const;
    const std::vector<Date> getRecurringDates() const;

    void setTitle(const std::string& title);
    void setDescription(const std::string& description);
    void setStartDate(const Date& startDate);
    void setEndDate(const Date& endDate);
    void setRecurrenceRule(const RecurrenceRule& recurrenceRule);
    void setPriority(const int& priority);
    
    //serialize and deserialize
    //serialization format: title|description|startDate|endDate|priority|recurrenceRule;
    const std::string serialize() const;
    static Event deserialize(const std::string& data);

    bool operator==(const Event &event) const;
    bool operator!=(const Event &event) const;
};

#endif // EVENT_HPP