#include <iostream>
#include "../include/Calendar.hpp"
#include "../include/CommandHandler.hpp"

int main() {
    Calendar calendar = Calendar();
    CommandHandler commandHandler(calendar);

    while (true) {
        calendar.printCalendar();
        calendar.printCurDateEvents();

        std::string input;
        std::getline(std::cin, input);
        if (input.empty()) break;

        try {
            commandHandler.executeCommand(input);
        } catch (const std::exception& e) {
            std::cerr << e.what() << std::endl;
            std::cout << "Enter \"help\" to see available commands" << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
    }

}