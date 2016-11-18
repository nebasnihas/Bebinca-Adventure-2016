/////////////////////////////////////////////////////////////////////////////
//                         Single Threaded Networking
//
// This file is distributed under the MIT License. See the LICENSE file
// for details.
/////////////////////////////////////////////////////////////////////////////


#include "chatWindow.h"

#include <cassert>


ChatWindow::ChatWindow(std::function<void(std::string)> onTextEntry,
                       int updateDelay)
        : onTextEntry{onTextEntry} {
    initscr();
    noecho();
    halfdelay(updateDelay);

    getmaxyx(stdscr, parentY, parentX);

    view = newwin(parentY - entrySize, parentX, 0, 0);
    scrollok(view, TRUE);

    entry = newwin(entrySize, parentX, parentY - entrySize, 0);
    wborder(entry, ' ', ' ', '-', ' ', '+', '+', ' ', ' ');
    entrySub = derwin(entry, entrySize - 1, parentX, 1, 0);

    entryField = new_field(entrySize - 1, parentX, 0, 0, 0, 0);
    assert(entryField && "Error creating entry field.");
    set_field_buffer(entryField, 0, "");
    set_field_opts(entryField, O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);

    fields[0] = entryField;
    entryForm = new_form(fields);
    assert(entryForm && "Error creating entry form.");
    set_form_win(entryForm, entry);
    set_form_sub(entryForm, entrySub);
    post_form(entryForm);

    refresh();
    wrefresh(entry);
}

ChatWindow::ChatWindow(const std::string &text, std::vector<std::string> menu) {
    initscr();
    noecho();

    getmaxyx(stdscr, parentY, parentX);
    view = newwin(parentY, parentX, 0, 0);
    scrollok(view, FALSE);

    displayMenu(text, menu);

    refresh();
    wrefresh(view);
}

ChatWindow::~ChatWindow() {
    unpost_form(entryForm);
    free_form(entryForm);
    free_field(entryField);
    delwin(entry);
    delwin(view);
    endwin();
}


void ChatWindow::update() {
    resizeOnShapeChange();
    processInput(getch());
    wrefresh(view);
    wrefresh(entry);
}


void ChatWindow::displayText(const std::string &text) {
    wprintw(view, "%s", (text + "\n").c_str());
}


void ChatWindow::resizeOnShapeChange() {
    int newX, newY;
    getmaxyx(stdscr, newY, newX);

    if (newY != parentY || newX != parentX) {
        parentX = newX;
        parentY = newY;

        wresize(view, parentY - entrySize, parentX);
        wresize(entry, entrySize, parentX);
        mvwin(entry, parentY - entrySize, 0);

        wclear(stdscr);
        wborder(entry, ' ', ' ', '-', ' ', '+', '+', ' ', ' ');
    }
}


void ChatWindow::processInput(int key) {
    switch (key) {
        case KEY_ENTER:
        case '\n':
            // Requesting validation synchs the seen field & the buffer.
            form_driver(entryForm, REQ_VALIDATION);
            onTextEntry(getFieldString());
            move(1, 1);
            set_field_buffer(entryField, 0, "");
            refresh();
            pos_form_cursor(entryForm);
            break;
        case KEY_BACKSPACE:
        case 127:
            form_driver(entryForm, REQ_DEL_PREV);
            break;
        case KEY_DC:
            form_driver(entryForm, REQ_DEL_CHAR);
            break;
        case ERR:
            // swallow
            break;
        default:
            form_driver(entryForm, key);
            break;
    }
}


size_t
ChatWindow::getFieldSize() const {
    size_t x, y;
    getyx(entrySub, y, x);
    return y * parentX + x;
}


std::string
ChatWindow::getFieldString() const {
    return std::string{field_buffer(entryField, 0), getFieldSize()};
}

void ChatWindow::setOnTextEntry(std::function<void(std::string)> onTextEntry)
{
    this->onTextEntry = onTextEntry;
}

void ChatWindow::displayMenu(const std::string &text, std::vector<std::string> menu) {
    displayText(text);
    int selectedItemIndex = 0;

    // Configure user input settings
    noecho();
    keypad(view, TRUE);
    curs_set(0);

    // Print initial menu
    for (int i = 0; i < menu.size(); i++) {
        // Highlight first item
        if (i == selectedItemIndex) {
            wattron(view, A_STANDOUT);
        } else {
            wattroff(view, A_STANDOUT);
        }

        displayMenuItem(menu, i);
    }
    box(view, 0, 0);
    displayMenuPrompt(text);

    // Update menu highlighting based on user input
    int input = 0;
    while ((input = wgetch(view)) != SELECT) {
        displayMenuItem(menu, selectedItemIndex);
        int lastItemIndex = (int) (menu.size() - 1);

        switch(input) {

            case KEY_UP:
                selectedItemIndex--;

                if (selectedItemIndex < 0) {
                    selectedItemIndex = lastItemIndex;
                }

                break;

            case KEY_DOWN:
                selectedItemIndex++;

                if (selectedItemIndex > lastItemIndex) {
                    selectedItemIndex = 0;
                }

                break;

            default:
                break;
        }

        // Highlight corresponding menu item
        wattron(view, A_STANDOUT);
        displayMenuItem(menu, selectedItemIndex);
        wattroff(view, A_STANDOUT);

        box(view, 0, 0);
        displayMenuPrompt(text);
    }

    // TODO send data instead of displaying it it for feedback
    displayText("You selected " + menu[selectedItemIndex]);
}

void ChatWindow::displayMenuPrompt(const std::string &text) {
    mvwprintw(view, PADDING_TOP, PADDING_LEFT, text.c_str());
}

void ChatWindow::displayMenuItem(std::vector<std::string> menu, int index) {
    int topPadding = index + 7;
    const char* item = (menu[index] + "\n").c_str();
    mvwprintw(view, topPadding, PADDING_LEFT, item);
}
