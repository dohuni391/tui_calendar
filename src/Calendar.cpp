#include "../include/Calendar.hpp"

std::vector<Event> Calendar::s_events;

Calendar::Calendar() : _user("default"), _curDate(Date::getToday()) {
    welcome();
}
Calendar::Calendar(const std::string& user) : _user(user), _curDate(Date::getToday()) {
    loadFromFile();
}

std::string Calendar::getUser() const {
    return _user;
}
Date& Calendar::getCurDate() {
    return _curDate;
}
std::vector<Event>& Calendar::getAllEvents() const {
    return s_events;
}

// Helper function to sort indices based on priority (descending order)
std::vector<int>& sortIndexWithPriority(std::vector<int>& indices, const std::vector<Event>& events) {
    for (size_t i = 0; i < indices.size(); ++i) {
        for (size_t j = i + 1; j < indices.size(); ++j) {
            if (events[indices[i]].getPriority() < events[indices[j]].getPriority()) {
                std::swap(indices[i], indices[j]);
            }
        }
    }
    return indices;
}

std::vector<int> Calendar::getEventIndice(const Date& date) const {
    std::vector<int> indices;
    for (size_t i = 0; i < s_events.size(); ++i) {
        Event& event = s_events[i];
        std::vector<Date> dates = event.getDates();
        if (std::find(dates.begin(), dates.end(), date) != dates.end()) {
            indices.push_back(i);
        }
    }

    sortIndexWithPriority(indices, s_events);

    return indices;
}
std::vector<int> Calendar::getEventIndice(const Date& date, const std::string& title) const {
    std::vector<int> indices;
    for (size_t i = 0; i < s_events.size(); ++i) {
        Event& event = s_events[i];
        std::vector<Date> dates = event.getDates();
        if (std::find(dates.begin(), dates.end(), date) != dates.end() && event.getTitle() == title) {
            indices.push_back(i);
        }
    }
    sortIndexWithPriority(indices, s_events);
    return indices;
}
std::vector<int> Calendar::getCurDateEventIndice() const {
    return getEventIndice(_curDate);
}

void Calendar::setUser(const std::string& user) {
    _user = user;

    std::ifstream file(user + ".txt");
    if (!file.is_open()) {
        std::ofstream newFile(user + ".txt");
        newFile.close();

        std::ofstream write("users.txt", std::ofstream::app);
        write << user << std::endl;
        write.close();
    }
    loadFromFile();
}
void Calendar::setCurDate(const Date& date) {
    _curDate = date;
}
void Calendar::addEvent(const Event& event) {
    s_events.push_back(event);
}
void Calendar::removeEvent(const Event& event) {
    s_events.erase(std::remove(s_events.begin(), s_events.end(), event), s_events.end());
}
void Calendar::removeEvent(const int& index) {
    s_events.erase(s_events.begin() + index);
}
void Calendar::removeEvents(const std::vector<int>& indices) {
    for (int i = indices.size() - 1; i >= 0; --i) {
        s_events.erase(s_events.begin() + indices[i]);
    }
}
void Calendar::removeAllEvents() {
    s_events.clear();
    std::ofstream write(_user + ".txt");
    write.close();
}

void Calendar::saveToFile() const {
    std::ofstream write(_user + ".txt");
    for (const Event& event : s_events) {
        write << event.serialize() << std::endl;
    }
    write.close();
}
void Calendar::loadFromFile() {
    std::ifstream read(_user + ".txt");
    if (!read.is_open()) {
        std::cerr << "Error: File not found: " << std::endl;
        return;
    }
    s_events.clear();
    std::string line;
    while (std::getline(read, line)) {
        s_events.push_back(Event::deserialize(line));
    }
    read.close();
}

void Calendar::printEvents(const Date& date) const {
    std::vector<int> eventIndex = getEventIndice(date);
    if (eventIndex.empty()) {
        std::cout << "———————————————————————————" << "\n";
        std::cout << "No events on " << date.toString() << "\n";
        return;
    }

    std::cout << "———————————————————————————";
    for (int i = 0; i < eventIndex.size(); ++i) {
        Event& event = s_events[eventIndex[i]];
        std::cout << "\nTitle: " << event.getTitle() << "\n";
        
        if (event.hasDescription()) {
            std::cout << "Description: " << event.getDescription() << "\n";
        }

        if (event.isRecurringEvent()) {
            std::cout << "Date: " << event.getRecurrenceRule().toString()
                << " from " << event.getStartDate().toString() 
                << " to " << event.getEndDate().toString() + "\n";
        } else {
            std::cout << "Date: " << event.getStartDate().toString();
            if (!event.isOneDayEvent()) {
                std::cout << " ~ " << event.getEndDate().toString();
            }
            std::cout << "\n";
        }

        std::cout << "Priority: " << event.getPriority() << "\n";
        std::cout << "———————————————————————————";
    }

    std::cout << std::endl;
}
void Calendar::printCurDateEvents() const {
    printEvents(_curDate);
}
void Calendar::printCalendar() const {
    std::cout << "\n———————————————————————————" << "\n";
    std::cout << "        " << Date::getMonthName(_curDate.getMonth()-1) << " " << _curDate.getYear() << "\n";
    std::cout << "———————————————————————————" << "\n";
    std::cout << "Sun Mon Tue Wed Thu Fri Sat" << "\n";
    std::cout << "———————————————————————————" << "\n";

    for (int i = 0; i < Date::getDayOfWeek(1, _curDate.getMonth(), _curDate.getYear()); ++i) {
        std::cout << "    ";
    }

    for (int i = 1; i <= Date::getDaysOfMonth(_curDate); ++i) {
        if (i == _curDate.getDay()) {
            std::cout << "[" << std::setw(2) << std::setfill(' ') << i << "]";
        } else {
            std::cout << std::setw(3) << std::setfill(' ') << i << " ";
        }
        if (Date::getDayOfWeek(i, _curDate.getMonth(), _curDate.getYear()) == 6 && i != Date::getDaysOfMonth(_curDate)) {
            std::cout << "\n";
        }
    }
    std::cout << std::endl;
}

void Calendar::welcome() {
    std::ifstream checkFile("users.txt");
    if (checkFile.is_open()) {
        std::string user;
        getline(checkFile, user);
        setUser(user);
        checkFile.close();
        std::cout << "Welcome " << _user;

        Date tomorrow = _curDate;
        tomorrow.addDays(1);

        std::vector<int> todayEvents = getEventIndice(_curDate);
        std::vector<int> tomorrowEvents = getEventIndice(tomorrow);

        int totalUpcomingEvents = todayEvents.size() + tomorrowEvents.size();

        std::cout << "\nThere are " << totalUpcomingEvents << " upcoming events.\n";

        if (!todayEvents.empty()) {
            std::cout << "\nEvents today:\n";
            printEvents(_curDate);
        }

        if (!tomorrowEvents.empty()) {
            std::cout << "\nEvents tomorrow:\n";
            printEvents(tomorrow);
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }
    
    std::cout << "Welcome to Calendar!" << std::endl;
    std::cout << "Please enter your name: ";
    std::string user;
    std::getline(std::cin, user);
    if (user.empty()) {
        throw std::invalid_argument("Invalid username");
    }
    setUser(user);
}
void Calendar::reset() {
    s_events.clear();
    std::ifstream file("users.txt");
    if (file.is_open()) {
        std::string user;
        while (std::getline(file, user)) {
            std::remove((user + ".txt").c_str());
        }
        file.close();
    }
    std::remove("users.txt");
    welcome();
}