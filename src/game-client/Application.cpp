#include "Application.hpp"
#include <glog/logging.h>

namespace {
const short NUM_COLOURS = 8;
const short ALL_COLORS[NUM_COLOURS] = {
    COLOR_BLACK,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_YELLOW,
    COLOR_BLUE,
    COLOR_MAGENTA,
    COLOR_CYAN,
    COLOR_WHITE,
};
}

namespace gui {

Application::Application() {
    initscr();
    noecho();
    raw();
    halfdelay(1);

    for(short i = 0; i < NUM_COLOURS; i++) {
        for(short j = 0; j < NUM_COLOURS; j++) {
            init_pair(i + j, ALL_COLORS[i], ALL_COLORS[j]);
        }
    }
    start_color();

    clear();
    refresh();
    getmaxyx(stdscr, h, w);
}

void Application::switchToWindow(const std::string& name) {
    CHECK(windows.count(name) == 1) << "No window with name: " << name;
    clear();
    refresh();
    currentWindow = windows.at(name);
    currentWindow->resize(Size{w, h});
    currentWindow->redraw();
}

void Application::update() {
    CHECK(currentWindow != nullptr) << "No window selected";

    if (is_term_resized(h, w)) {
        getmaxyx(stdscr, h, w);
        clear();
        resizeterm(h, w);
        refresh();
        currentWindow->resize(Size{w, h});
    }

    int input = wgetch(currentWindow->getCursesWindow());
    switch (input) {
        case ERR:
            break;
        default:
            currentWindow->update(input);
            break;
    }

    currentWindow->redraw();
}

Application::~Application() {
    endwin();
}

void Application::addWindow(const std::string& name, Window *window) {
    windows.insert({name, window});
    if (currentWindow == nullptr) {
        switchToWindow(name);
    }
}

}