#include "AuthenticationWindow.hpp"
#include <string>
#include <glog/logging.h>
#include <boost/algorithm/string.hpp>

namespace {
const int ENTRY_WIDTH = 30;
const int ENTRY_HEIGHT = 14;
const int WINDOW_WIDTH = ENTRY_WIDTH + 6; //padding
const int WINDOW_HEIGHT = ENTRY_HEIGHT + 6;
const int ENTRY_OFFSET_Y = 1;
const int ENTRY_OFFSET_X = (WINDOW_WIDTH - ENTRY_WIDTH) / 2;
const int FIELD_OFFSET_X = 0;
const int FIELD_WIDTH = ENTRY_WIDTH - FIELD_OFFSET_X * 2; // need space after offsetting

FIELD* createTitleField(const std::string& title) {
    FIELD* usernameLabelField = new_field(1, FIELD_WIDTH, ENTRY_OFFSET_Y, FIELD_OFFSET_X, 0, 0);
    CHECK(usernameLabelField) << "Error creating title label field";
    set_field_buffer(usernameLabelField, 0, title.c_str());
    set_field_just(usernameLabelField, JUSTIFY_CENTER);
    set_field_opts(usernameLabelField, O_STATIC | O_VISIBLE | O_PUBLIC);
    set_field_type(usernameLabelField, TYPE_ALPHA);
    set_field_fore(usernameLabelField, A_BOLD);
    field_opts_off(usernameLabelField, O_AUTOSKIP | O_EDIT | O_ACTIVE);

    return usernameLabelField;
}

FIELD* createUsernameLabelField() {
    FIELD* usernameLabelField = new_field(1, FIELD_WIDTH, ENTRY_OFFSET_Y + 1, FIELD_OFFSET_X, 0, 0);
    CHECK(usernameLabelField) << "Error creating username label field";
    set_field_buffer(usernameLabelField, 0, "Username:");
    set_field_opts(usernameLabelField, O_STATIC | O_VISIBLE | O_PUBLIC);
    set_field_type(usernameLabelField, TYPE_ALPHA);
    field_opts_off(usernameLabelField, O_AUTOSKIP | O_EDIT | O_ACTIVE);

    return usernameLabelField;
}

FIELD* createUsernameField() {
    FIELD* usernameField = new_field(1, FIELD_WIDTH, ENTRY_OFFSET_Y + 2, FIELD_OFFSET_X, 0, 0);
    CHECK(usernameField) << "Error creating username field";
    set_field_buffer(usernameField,0, "");
    set_field_opts(usernameField, O_STATIC | O_VISIBLE | O_PUBLIC | O_EDIT | O_ACTIVE);
    set_field_type(usernameField, TYPE_ALNUM);
    set_field_back(usernameField, A_UNDERLINE);
    field_opts_off(usernameField, O_AUTOSKIP);

    return usernameField;
}

FIELD* createPasswordLabelField() {
    FIELD* passwordLabelField = new_field(1, FIELD_WIDTH, ENTRY_OFFSET_Y + 4, FIELD_OFFSET_X, 0, 0);
    CHECK(passwordLabelField) << "Error creating password label field";
    set_field_buffer(passwordLabelField, 0, "Password:");
    set_field_opts(passwordLabelField, O_STATIC | O_VISIBLE | O_PUBLIC);
    set_field_type(passwordLabelField, TYPE_ALPHA);
    field_opts_off(passwordLabelField, O_AUTOSKIP | O_EDIT | O_ACTIVE);

    return passwordLabelField;
}

FIELD* createPasswordField() {
    FIELD* passwordField = new_field(1, FIELD_WIDTH, ENTRY_OFFSET_Y + 5, FIELD_OFFSET_X, 0, 0);
    CHECK(passwordField) << "Error creating password field";
    set_field_buffer(passwordField, 0, "");
    set_field_opts(passwordField, O_STATIC | O_VISIBLE | O_EDIT | O_ACTIVE);
    set_field_back(passwordField, A_UNDERLINE);
    //TODO password stars
    set_field_type(passwordField, TYPE_ALNUM);
    field_opts_off(passwordField, O_AUTOSKIP);

    return passwordField;
}

FIELD* createMessageField() {
    FIELD* messageField = new_field(2, FIELD_WIDTH, ENTRY_OFFSET_Y + 7, FIELD_OFFSET_X, 0, 0);
    CHECK(messageField) << "Error creating message field";
    set_field_buffer(messageField, 0, "");
    set_field_opts(messageField, O_STATIC | O_VISIBLE | O_PUBLIC | O_WRAP | O_EDIT);
    field_opts_off(messageField, O_AUTOSKIP | O_ACTIVE);

    return messageField;
}

}

namespace gui {

AuthenticationWindow::AuthenticationWindow(std::string title) : title{std::move(title)} {
    entryWindow = newwin(WINDOW_HEIGHT, WINDOW_WIDTH, 0, 0);
    CHECK(entryWindow) << "Error creating window";
    keypad(entryWindow, true);

    entrySubWindow = derwin(entryWindow, ENTRY_HEIGHT, ENTRY_WIDTH, 1, ENTRY_OFFSET_X);
    CHECK(entrySubWindow) << "Error creating sub window";

    createForm();
    resize(Size{COLS, LINES});
}

void AuthenticationWindow::update(int input) {
    switch(input) {
        case '\t':
        case KEY_DOWN:
            form_driver(form, REQ_NEXT_FIELD);
            form_driver(form, REQ_END_LINE);
            break;
        case KEY_UP:
            form_driver(form, REQ_PREV_FIELD);
            form_driver(form, REQ_END_LINE);
            break;
        case '\n':
        case KEY_ENTER:
            form_driver(form, REQ_VALIDATION);
            form_driver(form, REQ_NEXT_FIELD);
            form_driver(form, REQ_END_LINE);
            showMessage("");
            processCredentials();
            pos_form_cursor(form);
            break;
        case KEY_BACKSPACE:
        case 127:
            form_driver(form, REQ_DEL_PREV);
            form_driver(form, REQ_END_LINE);
            break;
        case KEY_DC:
            form_driver(form, REQ_DEL_CHAR);
            break;
        case 27:
            if (onCancelCallback) {
                onCancelCallback();
            }
        default:
            form_driver(form, input);
            break;
    }
}

void AuthenticationWindow::resize(const gui::Size& maxSize) {
    int x = (maxSize.width - WINDOW_WIDTH) / 2;
    int y = (maxSize.height - WINDOW_HEIGHT) / 2;
    mvwin(entryWindow, y, x);

    set_form_win(form, entryWindow);
    set_form_sub(form, entrySubWindow);
}

WINDOW* AuthenticationWindow::getCursesWindow() {
    return entryWindow;
}

AuthenticationWindow::~AuthenticationWindow() {
    unpost_form(form);
    free_form(form);
    for(const auto& field : fields) {
        free_field(field);
    }
}

void AuthenticationWindow::redraw() {
    box(entryWindow, 0, 0);
    wrefresh(entryWindow);
}

void AuthenticationWindow::createForm() {
    fields.push_back(createTitleField(title));
    fields.push_back(createUsernameLabelField());
    usernameInputField = createUsernameField();
    fields.push_back(usernameInputField);
    fields.push_back(createPasswordLabelField());
    passwordInputField = createPasswordField();
    fields.push_back(passwordInputField);
    messageField = createMessageField();
    fields.push_back(messageField);
    fields.push_back(nullptr);

    form = new_form(fields.data());
    set_form_win(form, entryWindow);
    set_form_sub(form, entrySubWindow);
    post_form(form);
}

void AuthenticationWindow::setOnCancel(std::function<void()> callback) {
    onCancelCallback = callback;
}

void AuthenticationWindow::setOnInput(std::function<void(const std::string& username, const std::string& password)> callback) {
    onInputCallback = callback;
}

void AuthenticationWindow::showMessage(const std::string& message) {
    set_field_buffer(messageField, 0, message.c_str());
    form_driver(form, REQ_END_LINE);
}

void AuthenticationWindow::processCredentials() {
    if(!onInputCallback) {
        showMessage("error");
        return;
    }

    auto username =  std::string{field_buffer(usernameInputField, 0), FIELD_WIDTH};
    boost::trim(username);
    if (username.empty()) {
        showMessage("Must enter username");
        return;
    }

    auto password =  std::string{field_buffer(passwordInputField, 0), FIELD_WIDTH};
    boost::trim(password);
    if (password.empty()) {
        showMessage("Must enter password");
        return;
    }

    onInputCallback(username, password);
}

void AuthenticationWindow::onEnter() {
    Window::onEnter();
    curs_set(true);
    pos_form_cursor(form);
}

}