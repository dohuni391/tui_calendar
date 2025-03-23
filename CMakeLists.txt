cmake_minimum_required(VERSION 3.10.0)
project(tui_calendar VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add all source files
set(SOURCES
    src/Calendar.cpp
    src/CommandHandler.cpp
    src/Date.cpp
    src/Event.cpp
    src/main.cpp
    src/RecurrenceRule.cpp
    src/Time.cpp
)

add_executable(tui_calendar ${SOURCES})
