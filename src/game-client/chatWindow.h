/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <functional>

#include <form.h>
#include <ncurses.h>
#include <vector>


class ChatWindow {
public:
    int const SELECT = '\n';
    int const PADDING_TOP = 2;
    int const PADDING_LEFT = 3;

    explicit ChatWindow(std::function<void(std::string)> onTextEntry)
            : ChatWindow{onTextEntry, 1} {}

    ChatWindow(std::function<void(std::string)> onTextEntry, int updateDelay);

    ChatWindow(const std::string &text, std::vector<std::string> menu);

    ~ChatWindow();

    ChatWindow(const ChatWindow &cw) = delete;

    ChatWindow &operator=(ChatWindow &cw) = delete;

    void update();

    void displayText(const std::string &text);

    void setOnTextEntry(std::function<void(std::string)> onTextEntry);

    void displayMenu(const std::string &text, std::vector<std::string> menu);

    void displayMenuItem(std::vector<std::string> menu, int index);

    void displayMenuPrompt(const std::string &text);

private:
    void resizeOnShapeChange();

    void processInput(int key);

    size_t getFieldSize() const;

    std::string getFieldString() const;

    std::function<void(std::string)> onTextEntry;

    int parentX = 0;
    int parentY = 0;
    int entrySize = 3;

    WINDOW *view = nullptr;
    WINDOW *entry = nullptr;
    WINDOW *entrySub = nullptr;

    FIELD *fields[2] = {nullptr, nullptr};
    FIELD *entryField = nullptr;

    FORM *entryForm = nullptr;
};


#endif
