#ifndef ADVENTURE2016_CHATWINDOW_HPP
#define ADVENTURE2016_CHATWINDOW_HPP

#include <form.h>
#include <string>
#include <functional>
#include "Window.hpp"

namespace gui {

enum class SoftKey {
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
};

class ChatWindow : public Window {
public:
    ChatWindow();
    ~ChatWindow();

    virtual void update(int input) override;
    virtual void redraw() override;
    virtual void resize(const Size& maxSize) override;
    virtual WINDOW* getCursesWindow() override;
    virtual void onEnter() override;

    void setOnInput(std::function<void(const std::string& text)> callback);
    void setOnSoftKeyPressed(std::function<void(SoftKey key)> callback);
    void showText(const std::string& text);
private:
    std::string getEntryText();
    void clearEntryText();
    void onSoftKeyPressed(SoftKey key);

    WINDOW* displayWindow;
    WINDOW* entryWindow;
    WINDOW* entrySubWindow;
    FIELD* entryField = nullptr;
    FIELD* entryFields[2] = {nullptr, nullptr};
    FORM* entryForm;

    std::function<void(const std::string& text)> callback;
    std::function<void(SoftKey key)> softkeyPressed;
};

}

#endif //ADVENTURE2016_CHATWINDOW_HPP
