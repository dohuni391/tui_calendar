# TUI Calendar

TUI Calendar is a terminal-based calendar application written in C++. It allows users to manage events, view calendars, and handle recurring events through a text-based user interface.

## Features

- Add, edit, and remove events
- View events for specific dates
- Handle recurring events
- Save and load events from a file
- User management
- Time zone (not yet implemented)
- Optimal scheduling (not yet implemented)

## Requirements

- CMake 3.10 or higher
- A C++ compiler (e.g., g++, clang++)

## Installation and Execution
Clone project from Git or download proeject file.

```sh
cd tui_calendar
mkdir build
cd build
cmake ..
make
./tui_calendar
```

## Command Usage Guide

This guide provides an overview of the commands and examples of how to use them.

### Commands

#### 1. `view`
- **Description**: View events on a specific date.
- **Usage**: `view [date]`
- **Examples**:
  - `view today` - View events for today.
  - `view next [interval] week` - View events for the next week.
  - `view prev [interval] month` - View events for the previous month.
  - `view 25/12/2023` - View events for December 25, 2023.

#### 2. `add`
- **Description**: Add a new event to the calendar.
- **Usage**: `add`
- **Steps**:
  1. Enter the event title.
  2. Enter the event description. (optional)
  3. Enter the start date.
  4. Enter the priority (optional, default is 0).
  5. Specify if the event is recurring (y/n).
  6. If recurring, enter the recurrence rule (e.g., "every 2 weeks").
  7. Enter the end date (optional, default is the same as the start date).
- **Example**:
  ```
  add
  Title: Team Meeting
  Description: Weekly team sync
  Start date: 25/12/2023
  Priority: 1
  Is recurring? [y/n]: y
  Every: 1 week
  On (1-7, seperated by commas): 1,7
  Ends on: 25/03/2024
  ```

#### 3. `remove`
- **Description**: Remove an event from the calendar.
- **Usage**: `remove [all/index]`
- **Examples**:
  - `remove all` - Remove all events for the current date.
  - `remove 1` - Remove the event with index 1 for the current date.

#### 4. `edit`
- **Description**: Edit an existing event.
- **Usage**: `edit [index]`
- **Steps**:
  1. Choose the parameter to edit (1-6):
     - 1. Title
     - 2. Description
     - 3. Start Date
     - 4. End Date
     - 5. Priority
     - 6. Recurrence Rule
  2. Enter the new value for the selected parameter.
- **Example**:
  ```
  edit 1
  Choose a parameter to edit (1-6): 1
  New Title: Project Review
  ```

#### 5. `settings`
- **Description**: Change application settings.
- **Usage**: `settings`
- **Options**:
  1. Change user
  2. Delete user
  3. Delete all events
  4. Reset (remove all events and users)
- **Example**:
  ```
  settings
  Choose a setting to change (1-4): 1
  Enter new user: JohnDoe
  ```

#### 6. `help`
- **Description**: Display the list of available commands.
- **Usage**: `help`
- **Example**:
  ```
  help
  ```

### Additional Notes

- **Date Formats**: Dates can be entered in various formats, such as `25 December 2023`, `December 25 2023`, `2023 December 25`, `25 12 2023`, `25/12/2023`, `December 25`, `25 December`, or `25`. The application automatically detects the format based on the input, and if the year or month is omitted, the current year or month is assumed.

- **Recurrence Rules**: When adding or editing the days of week for recurring events. The days of week are represented using numbers where 1 is Sunday, 2 is Monday, etc.