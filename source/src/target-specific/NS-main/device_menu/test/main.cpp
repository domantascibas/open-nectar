#include <curses.h>
#include <iostream>

#include "../src/menu_service.h"

int main() {
    char in;
    std::cout << "q -quit, w - up, s - down, d - next";

    localization::setup();

    while (in != 'q') {
        std::cin >> in;

        switch (in) {
            case 'w': // up
                menu_service::upButtonPressed();
                break;
            case 's': // down
                menu_service::downButtonPressed();
                break;
            case 'd': // forward
                menu_service::nextButtonPressed();
                break;
        }
    }
    return 0;
}