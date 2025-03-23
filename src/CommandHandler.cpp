#include "../include/CommandHandler.hpp"

const bool isNumber(const std::string& str) {
    return !str.empty() && std::all_of(str.begin(), str.end(), ::isdigit);
}
const std::vector<std::string> getArgs(const std::string& str, const char delimiter, std::vector<std::string>& args) {
    std::istringstream iss(str);
    std::string arg;
    while (std::getline(iss, arg, delimiter)) {
        args.push_back(arg);
    }
    return args;
}
const std::vector<std::string> getArgs(const std::string& str, std::vector<std::string>& args) {
    return getArgs(str, ' ', args);
}

CommandHandler::CommandHandler(Calendar& calendar) : _calendar(calendar) {}
void CommandHandler::executeCommand(const std::string &input){
    std::vector<std::string> args;
    std::string arg;
    getArgs(input, args);
    std::string command = args[0];
    args.erase(args.begin());

    if (command == "view") {
        viewCommand(args);
    } else if (command == "add") {
        addCommand();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else if (command== "remove") {
        removeCommand(args);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else if (command == "edit") {
        editCommand(args);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else if (command == "settings" || command == "setting") {
        settingsCommand();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else if (command == "help") {
        helpCommand();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    } else {
        throw std::invalid_argument("Invalid command");
    }
}

void CommandHandler::handleNextPrevArgument(int multiplier, const std::vector<std::string>& args, Date& curDate){
    if (args.size() > 3){
        throw std::invalid_argument("Invalid input");
    }

    if (args.size() == 1){
        curDate.addDays(multiplier);
        return;
    }
    
    std::string unit = args[1];
    if (args.size() == 3 && isNumber(args[1])) {
        int number = std::stoi(args[1]);
        multiplier *= number;
        unit = args[2];
    }

    if (unit == "day"){
        curDate.addDays(multiplier);
    } else if (unit == "week") {
        curDate.addWeeks(multiplier);
    } else if (unit == "month") {
        curDate.addMonths(multiplier);
    } else if (unit == "year") {
        curDate.addYears(multiplier);
    } else if (isNumber(unit)) {
        int day = std::stoi(unit);
        curDate.addDays(day * multiplier);
    } else {
        throw std::invalid_argument("Invalid input");
    }
}
void CommandHandler::viewCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        throw std::invalid_argument("No arguments provided for view command");
    }

    int day = 0, month = 0, year = 0;
    Date& curDate = _calendar.getCurDate();
    if (args[0] == "today") { //view today
        curDate = Date::getToday();
    } else if (args[0] == "next"){ //view next
        handleNextPrevArgument(1, args, curDate);
    } else if (args[0] == "prev"){ //view prev
        handleNextPrevArgument(-1, args, curDate);
    } else { //view [date]
        std::ostringstream oss;
        for (const auto& arg : args){
            oss << arg << " ";
        }
        _calendar.setCurDate(Date::fromString(oss.str(), curDate));
    }
}

void CommandHandler::addCommand() {
    std::string input;

    std::string title;
    std::string description;
    Date startDate = _calendar.getCurDate();
    Date endDate = _calendar.getCurDate();
    int priority;
    RecurrenceRule recurrenceRule;

    std::cout << "Title: ";
    std::getline(std::cin, title);

    std::cout << "Description: ";
    std::getline(std::cin, description);

    std::cout << "Start date: ";
    std::getline(std::cin, input);
    startDate.setDateFromString(input);

    std::cout << "Priority: ";
    std::getline(std::cin, input);
    if (input.empty()) {
        priority = 0;
    } else if (isNumber(input)) {
        priority = std::stoi(input);
    } else {
        throw std::invalid_argument("Invalid priority: " + input);
    }

    std::cout << "Is recurring? [y/n]: ";
    std::getline(std::cin, input);
    if (input == "Y" || input == "y") {
        std::cout << "Every: ";

        std::string recurrenceRuleStr;
        std::getline(std::cin, recurrenceRuleStr);

        std::vector<std::string> args;
        getArgs(recurrenceRuleStr, args);

        std::string frequencyStr = args[0];
        if (args.size() != 1){
            if (isNumber(args[0])){
                recurrenceRule.setInterval(std::stoi(args[0]));
            } else {
                throw std::invalid_argument("Invalid recurrence interval: " + args[0]);
            }
            frequencyStr = args[1];
        }
        
        if (frequencyStr == "day" || frequencyStr == "days") {
            recurrenceRule.setFrequency(RecurrenceRule::Frequency::DAILY);
        } else if (frequencyStr == "week" || frequencyStr == "weeks") {
            recurrenceRule.setFrequency(RecurrenceRule::Frequency::WEEKLY);
            std::cout << "On (1-7, seperated by commas): ";
            std::string daysOfWeekStr;
            std::getline(std::cin, daysOfWeekStr);
            std::vector<std::string> daysOfWeek;
            getArgs(daysOfWeekStr, ',', daysOfWeek);
            for (const auto& day : daysOfWeek){
                if (isNumber(day)){
                    int dayInt = std::stoi(day) - 1;
                    recurrenceRule.addDaysOfWeek(dayInt);
                } else {
                    throw std::invalid_argument("Invalid weekday");
                }
            }
        } else if (frequencyStr == "month" || frequencyStr == "months") {
            recurrenceRule.setFrequency(RecurrenceRule::Frequency::MONTHLY);
        } else if (frequencyStr == "year" || frequencyStr == "years") {
            recurrenceRule.setFrequency(RecurrenceRule::Frequency::YEARLY);
        } else {
            throw std::invalid_argument("Invalid recurrence interval: " + frequencyStr);
        }

        std::cout << "Ends on: ";
    } else {
        std::cout << "End date (press enter if one-day event): ";
    }

    std::getline(std::cin, input);
    if (input.empty()) {
        endDate = startDate;
    } else {
        endDate.setDateFromString(input);
    }
    _calendar.addEvent(Event(title, description, startDate, endDate, priority, recurrenceRule));
    _calendar.saveToFile();
    std::cout << "Event added successfully." << std::endl;
}

void CommandHandler::removeCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        throw std::invalid_argument("No arguments provided");
    }

    std::vector<int> eventIndice = _calendar.getCurDateEventIndice();
    if (args[0] == "all") {
        std::cout << "Are you sure? This will remove all the events in this day. [y/n]";
        std::string confirm;
        std::getline(std::cin, confirm);
        if (confirm == "Y" || confirm == "y") {
            _calendar.removeEvents(eventIndice);
        } else {
            std::cout << "Removal cancelled." << std::endl;
        }
    } else if (isNumber(args[0])) {
        int index = std::stoi(args[0]);
        if (index < 0 || index >= eventIndice.size()) {
            throw std::invalid_argument("Invalid index");
        }
        std::cout << "Are you sure? This cannot be undone. [y/n]\n";
        std::string confirm;
        std::getline(std::cin, confirm);
        if (confirm == "Y" || confirm == "y") {
            _calendar.removeEvent(eventIndice[index]);
        } else {
            std::cout << "Removal cancelled." << std::endl;
        }
    }
    _calendar.saveToFile();
    std::cout << "Event removed successfully." << std::endl;
}

void CommandHandler::editCommand(const std::vector<std::string>& args) {
    if (args.empty()) {
        throw std::invalid_argument("No arguments provided for edit command");
    }

    std::vector<int> eventIndice = _calendar.getCurDateEventIndice();
    if (!isNumber(args[0])) {
        throw std::invalid_argument("Invalid index");
    }

    int index = std::stoi(args[0]);
    if (index < 0 || index >= eventIndice.size()) {
        throw std::invalid_argument("Invalid index");
    }

    Event& event = _calendar.getAllEvents()[eventIndice[index]];
    std::cout << "Editing event: " << event.getTitle() << "\n";
    std::cout << "1. Title: " << event.getTitle() << "\n";
    std::cout << "2. Description: " << event.getDescription() << "\n";
    std::cout << "3. Start Date: " << event.getStartDate().toString() << "\n";
    if (event.isOneDayEvent()) {
        std::cout << "4. End Date: " << "One-day event\n";
    } else {
        std::cout << "4. End Date: " << event.getEndDate().toString() << "\n";
    }
    std::cout << "5. Priority: " << event.getPriority() << "\n";
    std::cout << "6. Recurrence Rule: " << event.getRecurrenceRule().toString() << "\n";

    std::cout << "Choose a parameter to edit (1-6): ";
    std::string choice;
    std::getline(std::cin, choice);

    if (!isNumber(choice)) {
        throw std::invalid_argument("Invalid choice: " + choice);
    }

    std::string input;
    int param = std::stoi(choice);
    if (param < 1 || param > 6) {
        throw std::invalid_argument("Invalid choice: " + choice);
    }

    if (param == 1) {
        std::cout << "New Title: ";
        std::getline(std::cin, input);
        event.setTitle(input);
    } else if (param == 2) {
        std::cout << "New Description: ";
        std::getline(std::cin, input);
        event.setDescription(input);
    } else if (param == 3) {
        std::cout << "New Date: ";
        std::getline(std::cin, input);
        event.setStartDate(Date::fromString(input));
    } else if (param == 4) {
        std::cout << "New Date: ";
        std::getline(std::cin, input);
        if (input.empty()) {
            event.setEndDate(event.getStartDate());
        } else {
            event.setEndDate(Date::fromString(input));
        }
    } else if (param == 5) {
        std::cout << "New Priority: ";
        std::getline(std::cin, input);
        if (isNumber(input)) {
            event.setPriority(std::stoi(input));
        } else {
            throw std::invalid_argument("Invalid priority: " + input);
        }
    } else if (param == 6) {
        std::cout << "New Recurrence Rule: \n";
        std::cout << "Every: ";
        std::getline(std::cin, input);
        RecurrenceRule newRule;
        std::vector<std::string> args;
        getArgs(input, args);
        std::string frequencyStr = args[0];
        if (args.size() != 1) {
            if (isNumber(args[0])) {
                newRule.setInterval(std::stoi(args[0]));
            } else {
                throw std::invalid_argument("Invalid recurrence interval: " + args[0]);
            }
            frequencyStr = args[1];
        }
        if (frequencyStr == "day" || frequencyStr == "days") {
            newRule.setFrequency(RecurrenceRule::Frequency::DAILY);
        } else if (frequencyStr == "week" || frequencyStr == "weeks") {
            newRule.setFrequency(RecurrenceRule::Frequency::WEEKLY);
            std::cout << "On (1-7, separated by commas): ";
            std::string daysOfWeekStr;
            std::getline(std::cin, daysOfWeekStr);
            std::vector<std::string> daysOfWeek;
            getArgs(daysOfWeekStr, ',', daysOfWeek);
            for (const auto& day : daysOfWeek) {
                if (isNumber(day)) {
                    int dayInt = std::stoi(day) - 1;
                    newRule.addDaysOfWeek(dayInt);
                } else {
                    throw std::invalid_argument("Invalid weekday");
                }
            }
        } else if (frequencyStr == "month" || frequencyStr == "months") {
            newRule.setFrequency(RecurrenceRule::Frequency::MONTHLY);
        } else if (frequencyStr == "year" || frequencyStr == "years") {
            newRule.setFrequency(RecurrenceRule::Frequency::YEARLY);
        } else {
            throw std::invalid_argument("Invalid recurrence interval: " + frequencyStr);
        }
        event.setRecurrenceRule(newRule);
    } else {
        throw std::invalid_argument("Invalid parameter choice");
    }

    _calendar.saveToFile();
    std::cout << "Event edited successfully." << std::endl;
}

void CommandHandler::settingsCommand() {
    std::cout << "Settings:" << std::endl;
    std::cout << "1. Change user" << "\n";
    std::cout << "2. Delete user" << "\n";
    std::cout << "3. Delete all events" << "\n";
    std::cout << "4. Reset" << "\n";
    std::cout << "Choose a setting to change (1-5): ";
    std::string input;
    std::getline(std::cin, input);
    if (!isNumber(input)) {
        throw std::invalid_argument("Invalid choice");
    }

    int choice = std::stoi(input);
    if (choice == 1) {
        std::cout << "Current user: " << _calendar.getUser() << "\n";
        std::cout << "Changing the user will create a new calendar.\n";
        std::cout << "You can always switch back to the previous user.\n";
        std::cout << "Enter new user: ";
        std::string username;
        std::getline(std::cin, username);
        if (username.empty()) {
            throw std::invalid_argument("Invalid username");
        }
        _calendar.setUser(username);
    } else if (choice == 2) {
        std::ifstream usersFile("users.txt");
        if (!usersFile.is_open()) {
            throw std::runtime_error("Unable to open users.txt");
        }

        std::vector<std::string> users;
        std::string line;
        while (std::getline(usersFile, line)) {
            users.push_back(line);
        }
        usersFile.close();

        if (users.size() <= 1) {
            std::cout << "Cannot delete the only user." << std::endl;
            return;
        }

        std::cout << "Current users:" << std::endl;
        for (size_t i = 0; i < users.size(); ++i) {
            std::cout << i + 1 << ". " << users[i] << std::endl;
        }

        std::cout << "Enter username or index to delete: ";
        std::getline(std::cin, input);

        int index = -1;
        if (isNumber(input)) {
            index = std::stoi(input) - 1;
            if (index < 0 || index >= users.size()) {
                throw std::invalid_argument("Invalid index");
            }
        } else {
            auto it = std::find(users.begin(), users.end(), input);
            if (it == users.end()) {
                throw std::invalid_argument("Username not found");
            }
            index = std::distance(users.begin(), it);
        }

        if (users[index] == _calendar.getUser()) {
            std::cout << "Cannot delete the current user." << std::endl;
            return;
        }

        users.erase(users.begin() + index);

        std::ofstream outFile("users.txt");
        if (!outFile.is_open()) {
            throw std::runtime_error("Unable to open users.txt");
        }

        for (const auto& user : users) {
            outFile << user << std::endl;
        }
        outFile.close();
        std::cout << "User deleted successfully." << std::endl;
    } else if (choice == 3) {
        std::cout << "Are you sure? This will remove all events in this calendar. [y/n]" << std::endl;
        std::string confirm;
        std::getline(std::cin, confirm);
        if (confirm == "Y" || confirm == "y") {
            _calendar.removeAllEvents();
        } else {
            std::cout << "Reset cancelled." << std::endl;
        }
    } else if (choice == 4) {
        std::cout << "Are you sure? This will remove all events and users in all calendars." << std::endl;
        std::cout << "This action cannot be undone. [y/n]" << std::endl;
        std::string confirm;
        std::getline(std::cin, confirm);
        if (confirm == "Y" || confirm == "y") {
            _calendar.reset();
        } else {
            std::cout << "Reset cancelled." << std::endl;
        }
    } else {
        throw std::invalid_argument("Invalid choice");
    }
}

void CommandHandler::helpCommand() {
    std::cout << "Commands:" << std::endl;
    std::cout << "view [date] - View events on a specific date. Refer to README.txt for detailed usage" << std::endl;
    std::cout << "add - Add an event" << std::endl;
    std::cout << "remove [all/index] - Remove an event" << std::endl;
    std::cout << "edit [index] - Edit an event" << std::endl;
    std::cout << "settings - Change settings" << std::endl;
    std::cout << "reset - Remove all events" << std::endl;
    std::cout << "help - Display this message" << std::endl;
}