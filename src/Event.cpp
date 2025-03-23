#include "../include/Event.hpp"

Event::Event(const std::string& title, const std::string& description, const Date& startDate, const Date& endDate, const int& priority, const RecurrenceRule& recurrenceRule)
: _title(title), _description(description), _startDate(startDate), _endDate(endDate), _recurrenceRule(recurrenceRule), _priority(priority) {
    if (_startDate > _endDate) {
        throw std::invalid_argument("Start date cannot be after end date: " + _startDate.toString() + " > " + _endDate.toString());
    }
}

const std::string& Event::getTitle() const { return _title; }
const Date& Event::getStartDate() const { return _startDate; }
const Date& Event::getEndDate() const { return _endDate; }
const RecurrenceRule& Event::getRecurrenceRule() const { return _recurrenceRule; }
const std::string& Event::getDescription() const { return _description; }
const int& Event::getPriority() const { return _priority; }

const bool Event::isOneDayEvent() const {
    return _startDate == _endDate;
}
const bool Event::hasDescription() const {
    return !_description.empty();
}
const bool Event::isRecurringEvent() const {
    return _recurrenceRule.getFrequency() != RecurrenceRule::Frequency::NONE;
}
const std::vector<Date> Event::getDates() const {
    if (isOneDayEvent()) {
        return {_startDate};
    } else if (isRecurringEvent()) {
        return getRecurringDates();
    } else {
        return getRangeDates();
    }
}
const std::vector<Date> Event::getRangeDates() const {
    std::vector<Date> dates;
    if (isOneDayEvent()) {
        return dates;
    }

    Date curDate = _startDate;
    while (curDate <= _endDate) {
        dates.push_back(curDate);
        curDate.addDays(1);
    }
    return dates;
}
const std::vector<Date> Event::getRecurringDates() const {
    std::vector<Date> dates;
    if (!isRecurringEvent()) {
        return dates;
    }

    if (_recurrenceRule.getFrequency() == RecurrenceRule::Frequency::WEEKLY) {
        std::array<bool, 7> daysOfWeek = _recurrenceRule.getDaysOfWeek();

        for (size_t i = 0; i < daysOfWeek.size(); i++) {
            if (!daysOfWeek[i]) {
                continue;
            }

            Date curDate = _startDate;
            int startDayOfWeek = Date::getDayOfWeek(curDate);
            int daysToAdd = (i - startDayOfWeek + 7) % 7;
            curDate.addDays(daysToAdd);

            while (curDate <= _endDate) {
                dates.push_back(curDate);
                curDate.add(_recurrenceRule.getInterval(), static_cast<int>(_recurrenceRule.getFrequency()));
            }
        }
    } else {
        Date curDate = _startDate;
        while (curDate <= _endDate) {
            dates.push_back(curDate);
            curDate.add(_recurrenceRule.getInterval(), static_cast<int>(_recurrenceRule.getFrequency()));
        }
    }
    
    return dates;
}

void Event::setTitle(const std::string& title) { _title = title; }
void Event::setDescription(const std::string& description) { _description = description; }
void Event::setStartDate(const Date& startDate) { _startDate = startDate; }
void Event::setEndDate(const Date& endDate) { _endDate = endDate; }
void Event::setRecurrenceRule(const RecurrenceRule& recurrenceRule) { _recurrenceRule = recurrenceRule; }
void Event::setPriority(const int& priority) { _priority = priority; }

const std::string Event::serialize() const {
    std::ostringstream oss;
    oss << _title << "|"
        << _description << "|"
        << _startDate.toString() << "|"
        << _endDate.toString() << "|"
        << _priority << "|"
        << _recurrenceRule.serialize() << ";";
    return oss.str();
}
Event Event::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string title, description, startDateStr, endDateStr, priorityStr, recurrenceRuleStr;
    std::getline(iss, title, '|');
    std::getline(iss, description, '|');
    std::getline(iss, startDateStr, '|');
    std::getline(iss, endDateStr, '|');
    std::getline(iss, priorityStr, '|');
    std::getline(iss, recurrenceRuleStr, ';');

    Date startDate = Date::fromString(startDateStr);
    Date endDate = Date::fromString(endDateStr);
    int priority = std::stoi(priorityStr);
    RecurrenceRule recurrenceRule = RecurrenceRule::deserialize(recurrenceRuleStr);

    return Event(title, description, startDate, endDate, priority, recurrenceRule);
}

bool Event::operator==(const Event& event) const {
    return _title == event.getTitle() && _description == event.getDescription() && _startDate == event.getStartDate() && _endDate == event.getEndDate();
}
bool Event::operator!=(const Event& event) const {
    return !(*this == event);
}
