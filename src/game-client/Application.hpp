#ifndef ADVENTURE2016_APPLICATION_HPP
#define ADVENTURE2016_APPLICATION_HPP

#include <unordered_map>
#include <string>
#include "Window.hpp"

namespace gui {

class Application {
public:
    Application();

    virtual ~Application();

    void update();
    void switchToWindow(const std::string& name);
    void addWindow(const std::string& name, Window *window);
    const std::string& getCurrentWindowName();
private:
    std::unordered_map<std::string, Window*> windows;
    Window* currentWindow = nullptr;
    std::string currentWindowName;
    int w;
    int h;
};

}

#endif //ADVENTURE2016_APPLICATION_HPP
