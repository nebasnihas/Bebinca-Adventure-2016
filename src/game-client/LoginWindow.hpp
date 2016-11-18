#ifndef ADVENTURE2016_LOGINWINDOW_HPP
#define ADVENTURE2016_LOGINWINDOW_HPP

#include <form.h>
#include <vector>
#include <functional>
#include <string>
#include "Window.hpp"

namespace gui{

class LoginWindow : public Window {
public:
    LoginWindow();
    ~LoginWindow();

    virtual void update(int input) override;
    virtual void resize(const Size& maxSize) override;
    virtual WINDOW* getCursesWindow() override;
    virtual void redraw() override;

    void showMessage(const std::string& message);
    void setOnCancel(std::function<void()> callback);
    void setOnInput(std::function<void(const std::string& username, const std::string& password)> callback);
private:
    void createForm();
    void processCredentials();

    WINDOW* entryWindow;
    WINDOW* entrySubWindow;
    WINDOW* messageWindow;
    FORM* form;
    std::vector<FIELD*> fields;
    FIELD* usernameInputField;
    FIELD* passwordInputField;

    std::function<void()> onCancelCallback;
    std::function<void(const std::string& username, const std::string& password)> onInputCallback;
};

}
#endif //ADVENTURE2016_LOGINWINDOW_HPP
