#ifndef ADVENTURE2016_WINDOW_HPP
#define ADVENTURE2016_WINDOW_HPP

#include <ncurses.h>

namespace gui {

struct Size {
    int width;
    int height;
};

class Window {
public:
    virtual void update(int input) = 0;
    virtual void redraw() = 0;
    virtual void resize(const Size& maxSize) = 0;
    virtual WINDOW* getCursesWindow() = 0;
    virtual ~Window(){};
};

}


#endif //ADVENTURE2016_WINDOW_HPP
