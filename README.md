# Digital Timer & Alarm System

An embedded C project that implements a digital clock, timer, and alarm management system using a microcontroller.

The project provides a menu-driven interface through a character LCD and matrix keypad. It uses an RTC module to maintain date and time, external EEPROM for persistent alarm storage, and interrupt-based functionality for real-time operations.

## Features

* Digital clock with date and time
* Real-time clock using the DS1307 RTC
* Character LCD interface
* Matrix keypad interface
* Set and modify date and time
* Create alarms
* View stored alarms
* Delete alarms
* Support for multiple alarm types
* Daily alarms
* One-time alarms
* Weekly alarms
* Find the nearest upcoming alarm
* Alarm ringing screen
* External EEPROM for storing alarm information
* Timer functionality
* Interrupt-based processing
* Menu-driven user interface

## Hardware / Peripherals Used

The project interfaces with several hardware peripherals:

### Microcontroller

The application is designed as an embedded C project targeting a PIC microcontroller.

### DS1307 RTC

The DS1307 Real-Time Clock is used to maintain:

* Seconds
* Minutes
* Hours
* Day
* Date
* Month
* Year

The RTC communicates with the microcontroller through the I2C protocol.

### Character LCD

A character LCD is used to display:

* Current date and time
* Menus
* Alarm information
* Timer information
* User prompts
* Alarm notifications

### Matrix Keypad

A matrix keypad provides user input for:

* Menu navigation
* Setting date and time
* Creating alarms
* Selecting alarm types
* Deleting alarms
* Controlling the timer

### External EEPROM

External EEPROM is used for persistent storage of alarm information so that alarms can be retained independently of normal program execution.

## System Architecture

```text
                 +----------------------+
                 |    PIC MCU           |
                 |                      |
                 |  Main Application    |
                 +----------+-----------+
                            |
          +-----------------+------------------+
          |                 |                  |
          v                 v                  v
     +---------+       +----------+       +----------+
     |  LCD    |       | Keypad   |       |  Timer   |
     +---------+       +----------+       +----------+
                            |
                            |
                 +----------+----------+
                 |                     |
                 v                     v
             +-------+           +-----------+
             | DS1307|           | EEPROM    |
             |  RTC  |           |           |
             +-------+           +-----------+
```

## Project Structure

```text
DIGITAL_TIMER.X/
│
├── alarm.h
├── alarm_ring_screen.c
│
├── clcd.c
├── clcd.h
│
├── delay_function.c
│
├── delete_alarm.c
├── display_alarm.c
│
├── ds1307.c
├── ds1307.h
│
├── external_eeprom.c
├── external_eeprom.h
│
├── find_nearest_alarm.c
├── home.c
│
├── i2c.c
├── i2c.h
│
├── insert_new_alarm.c
│
├── isr.c
├── isr.h
│
├── main.c
├── main.h
│
├── matrix_keypad.c
├── matrix_keypad.h
│
├── menu.c
│
├── set_alarm.c
├── set_alarm_type.c
├── set_daily_alarm.c
├── set_once_alarm.c
├── set_time_date.c
├── set_weekly_alarm.c
│
├── timer.c
├── timer.h
│
├── view_alarm.c
│
└── Makefile
```

## Source File Description

| File                   | Purpose                                           |
| ---------------------- | ------------------------------------------------- |
| `main.c`               | Main entry point of the application               |
| `main.h`               | Common declarations and definitions               |
| `clcd.c`               | Character LCD control functions                   |
| `clcd.h`               | LCD function declarations                         |
| `matrix_keypad.c`      | Matrix keypad handling                            |
| `matrix_keypad.h`      | Keypad declarations                               |
| `ds1307.c`             | DS1307 RTC communication and time/date operations |
| `ds1307.h`             | RTC declarations                                  |
| `i2c.c`                | I2C communication functions                       |
| `i2c.h`                | I2C declarations                                  |
| `external_eeprom.c`    | External EEPROM read/write operations             |
| `external_eeprom.h`    | EEPROM declarations                               |
| `alarm_ring_screen.c`  | Alarm ringing interface                           |
| `insert_new_alarm.c`   | Creation of new alarms                            |
| `set_alarm.c`          | Alarm configuration                               |
| `set_alarm_type.c`     | Selection/configuration of alarm types            |
| `set_daily_alarm.c`    | Daily alarm configuration                         |
| `set_once_alarm.c`     | One-time alarm configuration                      |
| `set_weekly_alarm.c`   | Weekly alarm configuration                        |
| `view_alarm.c`         | Displays stored alarms                            |
| `display_alarm.c`      | Alarm display functionality                       |
| `delete_alarm.c`       | Deletes existing alarms                           |
| `find_nearest_alarm.c` | Finds the nearest upcoming alarm                  |
| `home.c`               | Home screen functionality                         |
| `menu.c`               | Main menu handling                                |
| `set_time_date.c`      | Date and time configuration                       |
| `timer.c`              | Timer functionality                               |
| `timer.h`              | Timer declarations                                |
| `isr.c`                | Interrupt service routines                        |
| `isr.h`                | Interrupt declarations                            |
| `delay_function.c`     | Delay-related functionality                       |

## Alarm Types

The system supports different types of alarms:

### Once Alarm

A one-time alarm that is triggered at a specific date and time.

### Daily Alarm

An alarm that repeats every day at a configured time.

### Weekly Alarm

An alarm that is triggered on selected days of the week.

## User Interface

The system uses a combination of:

```text
Matrix Keypad
      |
      v
User Input
      |
      v
Menu System
      |
      +----------------+
      |                |
      v                v
Alarm Management    Time/Date
      |                |
      v                v
External EEPROM     DS1307 RTC
      |
      v
Character LCD
```

## Communication Protocols

### I2C

The I2C protocol is used for communication between the microcontroller and:

* DS1307 RTC
* External EEPROM

This allows the microcontroller to read/write time information and store alarm data.

## Interrupts

Interrupt service routines are used to handle time-sensitive operations.

The interrupt system helps the application perform real-time tasks while allowing the main application to handle user interaction and menu operations.

## Development Environment

The project is structured as a Microchip embedded C project and can be developed using:

* MPLAB X IDE
* XC8 compiler
* PIC microcontroller development hardware

The exact device configuration depends on the microcontroller used in the project.

## Building the Project

This project is intended to be built using the Microchip development environment.

Open the project in **MPLAB X IDE** and configure the appropriate:

1. PIC microcontroller
2. XC8 compiler
3. Programmer/debugger
4. Hardware configuration

Then build the project using:

```text
Build Project
```

The generated firmware can then be programmed onto the target microcontroller.

## Embedded Concepts Demonstrated

This project demonstrates practical embedded C concepts including:

* Microcontroller programming
* GPIO handling
* LCD interfacing
* Matrix keypad interfacing
* I2C communication
* RTC interfacing
* EEPROM interfacing
* Interrupt handling
* Timers
* Embedded state management
* Modular programming
* Header files
* Hardware abstraction
* Menu-driven embedded applications
* Persistent data storage

## Possible Improvements

Future improvements could include:

* More flexible alarm configuration
* Snooze functionality
* Improved alarm notification interface
* Buzzer volume control
* Multiple simultaneous alarms
* Better menu navigation
* Low-power/sleep modes
* Improved EEPROM data validation
* Alarm enable/disable functionality
* More advanced timer modes
* Battery monitoring
* Improved error handling

## Learning Outcomes

This project provides practical experience with:

* Embedded C programming
* PIC microcontrollers
* Peripheral interfacing
* I2C communication
* RTC modules
* EEPROM memory
* Interrupts
* Timers
* LCDs
* Matrix keypads
* Real-time event handling
* Modular embedded software design

## Author

**Pratham**
