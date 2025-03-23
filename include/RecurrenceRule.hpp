#ifndef RECURRENCERULE_HPP
#define RECURRENCERULE_HPP

#include <array>
#include <vector>
#include <string>
#include <sstream>
#include "Date.hpp"

class RecurrenceRule {
public:
    enum class Frequency {
        NONE,
        DAILY,
        WEEKLY,
        MONTHLY,
        YEARLY
    };

private:
    Frequency _frequency;
    int _interval;
    std::array<bool, 7> _daysOfWeek; //only used if frequency is WEEKLY

public:
    RecurrenceRule(Frequency frequency = Frequency::NONE, int interval = 1, std::array<bool, 7> daysOfWeek = {false, false, false, false, false, false, false});

    const Frequency getFrequency() const;
    const int getInterval() const;
    const std::array<bool, 7>& getDaysOfWeek() const;

    void setFrequency(Frequency frequency);
    void setInterval(int interval);
    void setDaysOfWeek(const std::array<bool, 7>& daysOfWeek);
    void addDaysOfWeek(int day);
    void removeDaysOfWeek(int day);

    const std::string toString() const;

    //serialize and deserialize
    //serialization format: frequency|interval|daysOfWeek;
    std::string serialize() const;
    static RecurrenceRule deserialize(const std::string& data);
};

#endif // RECURRENCERULE_HPP