#ifndef _NS_TIME_HM_H
#define _NS_TIME_HM_H

#include "stdint.h"
#include "ns_string.h"

using namespace std;

struct time_hm {
    time_hm() : hours(0), minutes(0) {};
    time_hm(uint8_t h, uint8_t m) : hours(h), minutes(m) {};

    std::string string() { return itos(hours) + " : " + itos(minutes); };

    // time++ operator for minutes
    time_hm operator++() {
        ++minutes;
        if (minutes >= 60)
            minutes -= 60;
        return *this;
    }
    // time-- operator
    time_hm operator--() {
        if (minutes == 0)
            minutes = 60;
        --minutes;
        return *this;
    };
    // ++time operator for hours
    time_hm operator++(int) {
        ++hours;
        if (hours >= 24)
            hours -= 24;
        return *this;
    };
    // --time operator
    time_hm operator--(int) {
        if (hours == 0)
            hours = 24;
        --hours;
        return *this;
    }

    uint8_t hours;
    uint8_t minutes;
};

#endif
