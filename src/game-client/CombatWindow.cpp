#include "CombatWindow.hpp"

namespace {
    const std::string LEVEL_LABEL = "Level ";
    const std::string HEALTH_LABEL = "Health";
    const std::string MANA_LABEL = "Mana";
    const std::string OFFENSE_SPELLS_LABEL = "Offense Spells";
    const std::string DEFENSE_SPELLS_LABEL = "Defense Spells";

    const int OFFSET = 1;
    const int CHAR_WINDOW_WIDTH = 39;
    const int CHAR_WINDOW_HEIGHT = 6;
    const int BOTTOM_WINDOW_HEIGHT = 10;
    const int MESSAGE_SUBWINDOW_OFFSET_W = 5;
    const int MESSAGE_SUBWINDOW_OFFSET_H = 3;
    const int MESSAGE_SUBWINDOW_X = 3;
    const int MESSAGE_SUBWINDOW_Y = 2;

    const int LABEL_X = 2;
    const int STATUS_BAR_X = 9;
    const int HEALTH_Y = 2;
    const int MANA_Y = 3;

    const char BAR = '|';
    const int BAR_VALUE = 5;
    const int FULL_BAR_PERCENT = 100;

    const int MENU_WIDTH = 14;
    const int SUBMENU_WIDTH = MENU_WIDTH * 2;
    const int SUBMENU_ROWS = 5;
    const int SUBMENU_COLS = 1;

    const std::string OFFENSE_CHOICE = "Offense Spells";
    const std::string DEFENSE_CHOICE = "Defense Spells";
    const int NUM_MENU_ITEMS = 2;
    const std::string SPELL_DELIMITER = "=";
    const int CORRECT_NUM_SPELL_PROPERTIES = 2;

    const std::vector<std::string> SPELL_TYPE_NAMES = {
        OFFENSE_CHOICE,
        DEFENSE_CHOICE,
    };

    // TODO replace hardcoded values
    std::vector<std::string> offenseSpells = {
            "blasphemy=10",
            "caustic touch=20",
            "magic missile=10",
            "pestilence=15",
            "soul petrification=30",
            "solomonic invocation=35",
            "extradimensional portal=12",
            "test1=100",
            "test2=20",
            "test3=30",
            "test4=40",
    };

    // TODO replace hardcoded values
    std::vector<std::string> defenseSpells = {
            "cure poison=10",
            "elven beauty=20",
            "heal=15",
            "iceshield=30",
            "sanctuary=15",
            "test1=100",
            "test2=20",
            "test3=30",
            "test4=40",
    };

    std::vector<std::string> currentMenuItems = SPELL_TYPE_NAMES;
//    std::vector<std::string> currentMenuItems = offenseSpells;

    gui::SpellMenuChoice getChoice(const std::string &val) {
        if (val == OFFENSE_CHOICE) {
            return gui::SpellMenuChoice::OFFENSE;
        } else if (val == DEFENSE_CHOICE){
            return gui::SpellMenuChoice::DEFENSE;
        } else {
            return gui::SpellMenuChoice::SUBMENU;
        }
    }

    void formatSpellSubMenuItems() {
        for (auto &item : currentMenuItems) {
            std::vector<std::string> spells;
            boost::split(spells, item, boost::is_any_of(SPELL_DELIMITER));

            if (spells.size() == CORRECT_NUM_SPELL_PROPERTIES) {
                std::string spellMana = spells.back();
                spells.pop_back();
                std::string spellName = spells.back();

                item = spellName;
                int numSpaces = SUBMENU_WIDTH - spellName.length() - spellMana.length();
                for (int i = 0; i < numSpaces; i++) {
                    item += " ";
                }
                item += spellMana;
            }
        }
    }

    int getWindowWidth(WINDOW *window) {
        int width = 0;
        int height = 0;
        getmaxyx(window, height, width);

        return width;
    }

    int getWindowHeight(WINDOW *window) {
        int width = 0;
        int height = 0;
        getmaxyx(window, height, width);

        return height;
    }

    void drawStatusBar(WINDOW *window, bool isHealth, int statusPercent) {
        init_pair(1, COLOR_GREEN, COLOR_BLACK);
        init_pair(2, COLOR_BLUE, COLOR_BLACK);
        init_pair(3, COLOR_RED, COLOR_BLACK);

        std::string currentStatusBar = "";
        int colourCode = (isHealth) ? 1 : 2;
        int y = (isHealth) ? HEALTH_Y : MANA_Y;

        // Current status (green or blue)

        for (int i = 1; i <= statusPercent; i += BAR_VALUE) {
            currentStatusBar += BAR;
        }

        wattron(window, COLOR_PAIR(colourCode));
        mvwprintw(window, y, STATUS_BAR_X, currentStatusBar.c_str());
        wattroff(window, COLOR_PAIR(colourCode));

        std::string statusNumbers = std::to_string(statusPercent) + "/" + std::to_string(FULL_BAR_PERCENT);
        mvwprintw(window, y, getWindowWidth(window) - statusNumbers.length() - LABEL_X, statusNumbers.c_str());

        // Missing status (red)

        std::string missingStatusBar = "";
        for (int i = 1; i <= FULL_BAR_PERCENT - statusPercent; i += BAR_VALUE) {
            missingStatusBar += BAR;
        }

        int x = STATUS_BAR_X + currentStatusBar.length();
        wattron(window, COLOR_PAIR(3));
        mvwprintw(window, y, x, missingStatusBar.c_str());
        wattroff(window, COLOR_PAIR(3));

        statusNumbers = std::to_string(statusPercent) + "/" + std::to_string(FULL_BAR_PERCENT);
        mvwprintw(window, y, getWindowWidth(window) - statusNumbers.length() - LABEL_X, statusNumbers.c_str());
    }

    void drawCharacterWindow(
            WINDOW *window,
            const std::string &characterName,
            int level,
            int health,
            int mana
    ) {
        std::string levelString = LEVEL_LABEL + std::to_string(level);

        // Non-changing labels
        wattron(window, A_BOLD);
        mvwprintw(window, 0, LABEL_X, characterName.c_str());
        wattroff(window, A_BOLD);
        mvwprintw(window, 0, getWindowWidth(window) - levelString.length() - LABEL_X, levelString.c_str());
        mvwprintw(window, HEALTH_Y, LABEL_X, HEALTH_LABEL.c_str());
        mvwprintw(window, MANA_Y, LABEL_X, MANA_LABEL.c_str());

        // Health and mana bars
        drawStatusBar(window, true, health);
        drawStatusBar(window, false, mana);
    }

    void drawCharacterAscii(WINDOW *window, const std::string &ascii) {
        mvwprintw(window, 0, 0, ascii.c_str());
    }

    // TODO use
    WINDOW* drawMenuSubWindow(WINDOW* menuWindow, WINDOW* menuSubWindow) {
        int menuHeight = 0;
        int menuWidth = 0;
        int menuY = 0;
        int menuX = 0;

        if (currentMenuItems.size() == NUM_MENU_ITEMS) {
            menuHeight = NUM_MENU_ITEMS;
            menuWidth = MENU_WIDTH;
            menuY = (getWindowHeight(menuWindow) / 2) - (NUM_MENU_ITEMS / 2);
            menuX = (getWindowWidth(menuWindow) / 2) - (MENU_WIDTH / 2);
        } else {
            menuHeight = SUBMENU_ROWS;
            menuWidth = SUBMENU_WIDTH;
            menuY = (getWindowHeight(menuWindow) / 2) - (SUBMENU_ROWS / 2);
            menuX = (getWindowWidth(menuWindow) / 2) - (SUBMENU_WIDTH / 2);
        }

        delwin(menuSubWindow);
        menuSubWindow = derwin(menuWindow, menuHeight, menuWidth, menuY, menuX);
        return menuSubWindow;
    }
}

namespace gui {

    CombatWindow::CombatWindow() {
        int w = 0;
        int h = 0;
        getmaxyx(stdscr, h, w);
        size = Size{w, h};

        combatWindow = newwin(0, 0, 0, 0);
        CHECK(combatWindow) << "Error creating combat window";
        keypad(combatWindow, true);

        character1Window = derwin(combatWindow,
                                  CHAR_WINDOW_HEIGHT, CHAR_WINDOW_WIDTH,
                                  OFFSET, (w / 4) - (CHAR_WINDOW_WIDTH / 2));
        CHECK(character1Window) << "Error creating player 1 window";
        box(character1Window, 0, 0);
        drawCharacterWindow(character1Window, "johndoe", 5, 60, 35);

        character2Window = derwin(combatWindow,
                                  CHAR_WINDOW_HEIGHT, CHAR_WINDOW_WIDTH,
                                  OFFSET, (w - (w / 4)) - (CHAR_WINDOW_WIDTH / 2));
        CHECK(character2Window) << "Error creating player 2 window";
        box(character2Window, 0, 0);
        drawCharacterWindow(character2Window, "Bulbasaur", 8, 90, 0);

        character1AsciiWindow = derwin(combatWindow,
                                       h - CHAR_WINDOW_HEIGHT - BOTTOM_WINDOW_HEIGHT - OFFSET, w / 2,
                                       CHAR_WINDOW_HEIGHT + OFFSET, 0);
        CHECK(character1AsciiWindow) << "Error creating char1 ascii window";
        drawCharacterAscii(character1AsciiWindow, ascii::getAscii("Jack"));

        character2AsciiWindow = derwin(combatWindow,
                                       h - CHAR_WINDOW_HEIGHT - BOTTOM_WINDOW_HEIGHT - OFFSET, w / 2,
                                       CHAR_WINDOW_HEIGHT + OFFSET, w / 2);
        CHECK(character2AsciiWindow) << "Error creating char2 ascii window";
        drawCharacterAscii(character2AsciiWindow, ascii::getAscii("Mother Goose"));

        messageWindow = derwin(combatWindow, BOTTOM_WINDOW_HEIGHT, w / 2, h - BOTTOM_WINDOW_HEIGHT, 0);
        CHECK(messageWindow) << "Error creating message window";
        box(messageWindow, 0, 0);

        messageSubWindow = derwin(messageWindow,
                                  getWindowHeight(messageWindow) - MESSAGE_SUBWINDOW_OFFSET_H,
                                  getWindowWidth(messageWindow) - MESSAGE_SUBWINDOW_OFFSET_W,
                                  MESSAGE_SUBWINDOW_Y,
                                  MESSAGE_SUBWINDOW_X);
        CHECK(messageSubWindow) << "Error creating message subwindow";
        box(messageWindow, 0, 0);
        scrollok(messageSubWindow, true);

        // TODO remove placeholder text
        appendText("You attack Bulbasaur and deal 10 damage");
        appendText("You take 10 damage from Bulbasaur");
        appendText("You cast protection on yourself and heal 25");
        appendText("You cast soul petrification on Bulbasaur for 30 damage");
        appendText("You attack Bulbasaur and deal 0 damage");
        appendText("You take 20 damage from Bulbasaur");

        menuWindow = derwin(combatWindow, BOTTOM_WINDOW_HEIGHT, w / 2, h - BOTTOM_WINDOW_HEIGHT, w / 2);
        CHECK(menuWindow) << "Error creating menu window";
        box(menuWindow, 0, 0);

        menuSubWindow = derwin(menuWindow, NUM_MENU_ITEMS, MENU_WIDTH,
                               (getWindowHeight(menuWindow) / 2) - (NUM_MENU_ITEMS / 2),
                               (getWindowWidth(menuWindow) / 2) - (MENU_WIDTH / 2));
        CHECK(menuSubWindow) << "Error creating menu subwindow";
        box(menuSubWindow, 0, 0);

        createMenu();

        resize(Size{w, h});
    }

    void CombatWindow::update(int input) {
        switch (input) {
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

                if (choice == gui::SpellMenuChoice::OFFENSE) {
                    removeMenu();
                    currentMenuItems = offenseSpells;
                    formatSpellSubMenuItems();
                    createMenu();

                    int x = 0;
                    int y = 0;
                    getparyx(menuSubWindow, y, x);

                    wattron(menuWindow, A_BOLD);
                    mvwprintw(menuWindow, OFFSET * 2, x, OFFENSE_SPELLS_LABEL.c_str());
                    mvwprintw(menuWindow, OFFSET * 2, getWindowWidth(menuWindow) - x - MANA_LABEL.length(), MANA_LABEL.c_str());
                    wattroff(menuWindow, A_BOLD);

                    menuSubWindow = drawMenuSubWindow(menuWindow, menuSubWindow);
                }

//                if (callback) {
//                    callback(choice);
//                }
                break;
            }
            default:
                break;
        }
    }

    WINDOW *CombatWindow::getCursesWindow() {
        return combatWindow;
    }

    CombatWindow::~CombatWindow() {
        removeMenu();
        delwin(menuSubWindow);
        delwin(menuWindow);
        delwin(messageSubWindow);
        delwin(messageWindow);
        delwin(character1AsciiWindow);
        delwin(character2AsciiWindow);
        delwin(character1Window);
        delwin(character2Window);
        delwin(combatWindow);
    }

    void CombatWindow::redraw() {
        wrefresh(combatWindow);
        wrefresh(character1Window);
        wrefresh(character2Window);
        wrefresh(character1AsciiWindow);
        wrefresh(character2AsciiWindow);
        wrefresh(messageWindow);
        wrefresh(messageSubWindow);
        wrefresh(menuWindow);
        wrefresh(menuSubWindow);
    }

    void CombatWindow::resize(const Size &size) {
        this->size = size;
        recreate();
    }

    void CombatWindow::createMenu() {
        for (const auto &item : currentMenuItems) {
            menuItems.emplace_back(new_item(item.c_str(), nullptr));
        }

        menuItems.emplace_back(nullptr);

        menu = new_menu(menuItems.data());
        set_menu_win(menu, menuWindow);
        set_menu_sub(menu, menuSubWindow);
        set_menu_format(menu, SUBMENU_ROWS, SUBMENU_COLS);
        set_menu_mark(menu, "");

        post_menu(menu);
    }

    void CombatWindow::removeMenu() {
        unpost_menu(menu);
        free_menu(menu);

        for (ITEM *item : menuItems) {
            free_item(item);
        }
        menuItems.clear();
    }

    void CombatWindow::recreate() {
//        wresize(messageWindow, BOTTOM_WINDOW_HEIGHT, size.width / 2);
//        wresize(messageSubWindow,
//                getWindowHeight(messageWindow) - MESSAGE_SUBWINDOW_OFFSET_H,
//                getWindowWidth(messageWindow) - MESSAGE_SUBWINDOW_OFFSET_W);
//        wresize(menuWindow, BOTTOM_WINDOW_HEIGHT, size.width / 2);
//        wresize(menuSubWindow,
//                (getWindowHeight(menuWindow) / 2) - (currentMenuItems.size() / 2),
//                (getWindowWidth(menuWindow) / 2) - (MENU_WIDTH / 2));
//
//        mvwin(character1Window, OFFSET, (getWindowWidth(combatWindow) / 4) - (CHAR_WINDOW_WIDTH / 2));
//        mvwin(character2Window, OFFSET,
//              (getWindowWidth(combatWindow) - (getWindowWidth(combatWindow) / 4)- CHAR_WINDOW_WIDTH / 2));
//        mvwin(character1AsciiWindow, CHAR_WINDOW_HEIGHT + OFFSET, 0);
//        mvwin(character2AsciiWindow, CHAR_WINDOW_HEIGHT + OFFSET, getWindowWidth(combatWindow) / 2);
//        mvwin(messageWindow, getWindowHeight(combatWindow) - BOTTOM_WINDOW_HEIGHT, 0);
//        mvwin(menuWindow, getWindowHeight(combatWindow) - BOTTOM_WINDOW_HEIGHT, getWindowWidth(combatWindow) / 2);
    }

    void CombatWindow::appendText(const std::string &text) {
        wprintw(messageSubWindow, "%s\n", text.c_str());
    }

    void CombatWindow::setOnSelection(std::function<void(SpellMenuChoice)> callback) {
        this->callback = callback;
    }

    void CombatWindow::onEnter() {
        Window::onEnter();
        curs_set(false);
    }

}