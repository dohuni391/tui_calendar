#ifndef COMMANDHANDLER_H
#define COMMANDHANDLER_H

#include <vector>
#include <string>
#include "Calendar.hpp"

class CommandHandler {
    private:
        Calendar& _calendar;

    public:
        CommandHandler(Calendar& calendar);
        void executeCommand(const std::string& input);

    private:
        void handleNextPrevArgument(const int multiplier, const std::vector<std::string>& args, Date& curDate);
        void viewCommand(const std::vector<std::string>& args);

        void addCommand();
        void removeCommand(const std::vector<std::string>& args);
        void editCommand(const std::vector<std::string>& args);
        void resetCommand();
        void settingsCommand();
        void helpCommand();

};

#endif // COMMANDHANDLER_H