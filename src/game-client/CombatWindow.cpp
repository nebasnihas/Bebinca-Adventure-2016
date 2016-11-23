#include <menu.h>
#include "CombatWindow.hpp"
#include <unordered_map>
#include <glog/logging.h>

namespace {
    const std::string LEVEL_LABEL = "Level ";
    const std::string HEALTH_LABEL = "Health";
    const std::string MANA_LABEL = "Mana";

    const int LABEL_X = 5;
    const int LEVEL_X = 5;
    const int HEALTH_Y = 2;
    const int MANA_Y = 3;
    const int STATUS_BAR_X = 13;

    const char BAR = '|';
    const int BAR_VALUE = 5;
    const int FULL_BAR_PERCENT = 100;

    const int MENU_NUM_ITEMS = 2;

    const std::string OFFENSE_CHOICE = "Offense Spells";
    const std::string DEFENSE_CHOICE = "Defense Spells";

    const std::string MENU_ITEM_NAMES[MENU_NUM_ITEMS] = {
            OFFENSE_CHOICE,
            DEFENSE_CHOICE,
    };

    gui::SpellMenuChoice getChoice(const std::string& val) {
        if (val == OFFENSE_CHOICE) {
            return gui::SpellMenuChoice::OFFENSE;
        } else {
            return gui::SpellMenuChoice::DEFENSE;
        }
    }

    int getWindowWidth(WINDOW* window) {
        int width = 0;
        int height = 0;
        getmaxyx(window, height, width);
        return width;
    }

    void drawStatusBar(WINDOW* window, bool isHealth, int statusPercent) {
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        init_pair(2, COLOR_BLUE, COLOR_BLACK);
        init_pair(3, COLOR_RED, COLOR_BLACK);

        std::string statusBar = "";
        int colourCode = (isHealth) ? 1 : 2;
        int y = (isHealth) ? HEALTH_Y : MANA_Y;

        // Current status (green or blue)
        for (int i = 1; i <= statusPercent; i += BAR_VALUE) {
            statusBar += BAR;
        }
        wattron(window, COLOR_PAIR(colourCode));
        mvwprintw(window, y, STATUS_BAR_X, statusBar.c_str());
        wattroff(window, COLOR_PAIR(colourCode));

        // Missing status (red)
        statusBar = "";
        for (int i = 1; i <= FULL_BAR_PERCENT - statusPercent; i += BAR_VALUE) {
            statusBar += BAR;
        }
        int x = getWindowWidth(window) - LABEL_X - statusBar.length();
        wattron(window, COLOR_PAIR(3));
        mvwprintw(window, y, x, statusBar.c_str());
        wattroff(window, COLOR_PAIR(3));
    }

    // TODO replace string playerName with Character character?
    void drawPlayerWindow(WINDOW* window, const std::string& playerName) {
        std::string level = LEVEL_LABEL + std::to_string(10);

        // Labels
        wattron(window, A_BOLD);
        mvwprintw(window, 0, LABEL_X, playerName.c_str());
        wattroff(window, A_BOLD);
        mvwprintw(window, 0, getWindowWidth(window) - level.length() - LEVEL_X, level.c_str());
        mvwprintw(window, HEALTH_Y, LABEL_X, HEALTH_LABEL.c_str());
        mvwprintw(window, MANA_Y, LABEL_X, MANA_LABEL.c_str());

        // Health and mana bars
        drawStatusBar(window, true, 85);
        drawStatusBar(window, false, 30);
    }
}

namespace gui {

    // CombatWindow::CombatWindow(Character& player1, Character& player2) { TODO
    CombatWindow::CombatWindow() {
        int w = 0;
        int h = 0;
        getmaxyx(stdscr, h, w);
        size = Size{w, h};

        combatWindow = newwin(0, 0, 0, 0);
        CHECK(combatWindow) << "Error creating window";
        keypad(combatWindow, true);

        int subWindowWidth = w / 2;
        int subWindowHeight = h / 4;
        int subWindowTopY = 1;

        player1Window = derwin(combatWindow, subWindowHeight, subWindowWidth, subWindowTopY, 0);
        CHECK(player1Window) << "Error creating player 1 window";
        box(player1Window, 0, 0);
        drawPlayerWindow(player1Window, "johndoe");

        player2Window = derwin(combatWindow, subWindowHeight, subWindowWidth, subWindowTopY, w - subWindowWidth);
        CHECK(player2Window) << "Error creating player 2 window";
        box(player2Window, 0, 0);
        drawPlayerWindow(player2Window, "Little Bo-Peep");

        subWindowHeight = (h / 4) + 1;
        subWindowTopY = h - subWindowHeight;

        messageWindow = derwin(combatWindow, subWindowHeight, subWindowWidth, subWindowTopY, 0);
        CHECK(messageWindow) << "Error creating message window";
        scrollok(messageWindow, true);
        box(messageWindow, 0, 0);
        std::string placeholderMessage = "You attack Little Bo-Peep and deal 10 damage";
        appendText(placeholderMessage);

        menuWindow = derwin(combatWindow, subWindowHeight, subWindowWidth, subWindowTopY, w / 2);
        CHECK(menuWindow) << "Error creating menu window";
        box(menuWindow, 0, 0);



        createMenu();

        resize(Size{w, h});
    }

    void CombatWindow::update(int input) {
        switch(input) {
            case KEY_UP:
                menu_driver(menu, REQ_UP_ITEM);
                break;
            case KEY_DOWN:
                menu_driver(menu, REQ_DOWN_ITEM);
                break;
            case '\n':
            case KEY_ENTER: {
                auto currentItem = current_item(menu);
                auto val = std::string{item_name(currentItem)};
                auto choice = getChoice(val);
                if (callback) {
                    callback(choice);
                }
                break;
            }
            default:
                break;
        }
    }

    WINDOW* CombatWindow::getCursesWindow() {
        return combatWindow;
    }

    CombatWindow::~CombatWindow() {
        removeMenu();
        delwin(menuWindow);
        delwin(messageWindow);
        delwin(player1Window);
        delwin(player2Window);
        delwin(combatWindow);
    }

    void CombatWindow::redraw() {
        wrefresh(combatWindow);
    }

    void CombatWindow::resize(const Size& size) {
        this->size = size;
        recreate();
    }

    void CombatWindow::createMenu() {
        for (const auto& name : MENU_ITEM_NAMES) {
            auto itemName = name.c_str();
            menuItems.emplace_back(new_item(itemName, nullptr));
        }
        menuItems.emplace_back(nullptr);

        menu = new_menu(menuItems.data());
        set_menu_win(menu, combatWindow);
        set_menu_sub(menu, menuWindow);
        set_menu_mark(menu, "");

        post_menu(menu);
    }

    void CombatWindow::removeMenu() {
        unpost_menu(menu);
        free_menu(menu);

        for (ITEM* item : menuItems) {
            free_item(item);
        }
        menuItems.clear();
    }

    void CombatWindow::recreate() {
        int menuX = size.width / 2 ;
        int menuY = size.height / 2 ;
        mvwin(combatWindow, menuY, menuX);

        set_menu_win(menu, combatWindow);
        set_menu_sub(menu, menuWindow);
    }


    void CombatWindow::appendText(const std::string& text) {
        mvwprintw(messageWindow, 1, 2, "%s\n", text.c_str());
    }

    void CombatWindow::setOnSelection(std::function<void(SpellMenuChoice)> callback) {
        this->callback = callback;
    }

    void CombatWindow::onEnter() {
        Window::onEnter();
        curs_set(false);
    }

}