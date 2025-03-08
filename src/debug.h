//
// Created by Sheng Tai on 08/03/2025.
//

#ifndef GARAGE_DOOR_SYSTEM_DEBUG_H
#define GARAGE_DOOR_SYSTEM_DEBUG_H

#ifdef DEBUG_ENABLE
#include "pico/stdlib.h"
#include <iostream>

/** Use the following marco to print debug message, it will print the message with a timestamp
 *  DPRINT("message");
 *  or
 *  DPRINT("name", value);
 *
 *  To disable debug message, comment out the DEBUG_ENABLE in root CMakeLists.txt
 *
 */

// use marco overloading to support different number of arguments
#define GET_MACRO(_1, _2, NAME, ...) NAME

// two versions of DEBUG_PRINT, one for single argument, one for two arguments
#define DEBUG_PRINT1(msg) Debug::print(msg)
#define DEBUG_PRINT2(name, value) Debug::print(name, value)

// use GET_MACRO to select the correct version of DEBUG_PRINT
#define DPRINT(...) GET_MACRO(__VA_ARGS__, DEBUG_PRINT2, DEBUG_PRINT1)(__VA_ARGS__)

#else
// when DEBUG_ENABLE is not defined, DEBUG_PRINT will be replaced with empty statement
#define DEBUG_PRINT(...) ((void)0)
#endif

class Debug {
  public:
    static void print(const std::string &message) {
        std::cout << "[" << time_us_32() << "] " << message << std::endl;
    }

    template <typename T> static void print(const std::string &name, const T &value) {
        std::cout << "[" << time_us_32() << "] " << name << " = " << value << std::endl;
    }
};

#endif // GARAGE_DOOR_SYSTEM_DEBUG_H
