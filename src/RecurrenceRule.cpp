#include "../include/RecurrenceRule.hpp"

RecurrenceRule::RecurrenceRule(Frequency frequency, int interval, std::array<bool,7> daysOfWeek)
    : _frequency(frequency), _interval(interval), _daysOfWeek(daysOfWeek) {
    if (interval < 1) {
        throw std::invalid_argument("Interval must be at least 1");
    }
}

const RecurrenceRule::Frequency RecurrenceRule::getFrequency() const {
    return _frequency;
}
const int RecurrenceRule::getInterval() const {
    return _interval;
}
const std::array<bool, 7>& RecurrenceRule::getDaysOfWeek() const {
    return _daysOfWeek;
}

void RecurrenceRule::setFrequency(Frequency frequency) {
    this->_frequency = frequency;
}
void RecurrenceRule::setInterval(int interval) {
    if (interval < 1) {
        throw std::invalid_argument("Interval must be at least 1");
    }
    this->_interval = interval;
}
void RecurrenceRule::setDaysOfWeek(const std::array<bool,7>& daysOfWeek) {
    if (_frequency != Frequency::WEEKLY) {
        throw std::invalid_argument("Weekdays can only be specified for weekly recurrence");
    }
    this->_daysOfWeek = daysOfWeek;
}
void RecurrenceRule::addDaysOfWeek(int day) {
    if (_frequency != Frequency::WEEKLY) {
        throw std::invalid_argument("Weekdays can only be specified for weekly recurrence");
    }
    if (day < 0 || day > 6) {
        throw std::invalid_argument("Invalid weekday");
    }

    for (int i = 0; i < _daysOfWeek.size(); i++) {
        if (i == day) {
            _daysOfWeek[i] = true;
        }
    }
    
}
void RecurrenceRule::removeDaysOfWeek(int day) {
    if (_frequency != Frequency::WEEKLY) {
        throw std::invalid_argument("Weekdays can only be specified for weekly recurrence");
    }
    if (day < 0 || day > 6) {
        throw std::invalid_argument("Invalid weekday");
    }

    for (int i = 0; i < _daysOfWeek.size(); i++) {
        if (i == day) {
            _daysOfWeek[i] = false;
        }
    }
}

const std::string RecurrenceRule::toString() const {
    if (_frequency == Frequency::NONE) { return "NONE"; }

    std::string str;
    str += "Every ";
    if (_interval > 1) {
        str += std::to_string(_interval) + " ";
    }

    static const std::unordered_map<Frequency, std::string> frequencyMap = {
        {Frequency::DAILY, "day"},
        {Frequency::WEEKLY, "week"},
        {Frequency::MONTHLY, "month"},
        {Frequency::YEARLY, "year"}
    };

    auto it = frequencyMap.find(_frequency);
    str += it->second;
    if (_interval > 1) {
        str += "s";
    }

    if (_frequency == Frequency::WEEKLY && !_daysOfWeek.empty()) {
        str += " on ";
        for (int i = 0; i < _daysOfWeek.size(); ++i) {
            if (_daysOfWeek[i]) {
                str += Date::getDayOfWeekName(i);
                if (i < _daysOfWeek.size() - 1) {
                    str += ", ";
                }
            }
        }
    }

    return str;
}

std::string RecurrenceRule::serialize() const {
    std::ostringstream oss;
    oss << static_cast<int>(_frequency) << "|"
        << _interval << "|";

    if (_frequency == Frequency::WEEKLY) {
        for (bool day : _daysOfWeek) {
            oss << (day ? '1' : '0');
        }
    }

    return oss.str();
}
RecurrenceRule RecurrenceRule::deserialize(const std::string& data) {
    std::istringstream iss(data);
    std::string token;
    std::vector<std::string> tokens;

    while (std::getline(iss, token, '|')) {
        tokens.push_back(token);
    }

    Frequency frequency = static_cast<Frequency>(std::stoi(tokens[0]));
    int interval = std::stoi(tokens[1]);
    std::array<bool, 7> weekdays = {false, false, false, false, false, false, false};

    if (frequency == Frequency::WEEKLY) {
        for (size_t i = 0; i < 7; i++) {
            weekdays[i] = tokens[2][i] == '1';
        }
    }

    return RecurrenceRule(frequency, interval, weekdays);
}