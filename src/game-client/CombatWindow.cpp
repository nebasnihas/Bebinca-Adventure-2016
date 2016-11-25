#include <menu.h>
#include "CombatWindow.hpp"
#include <unordered_map>
#include <glog/logging.h>

namespace {
    const std::string LEVEL_LABEL = "Level ";
    const std::string HEALTH_LABEL = "HP";
    const std::string MANA_LABEL = "Mana";

    const int CHAR_WINDOW_WIDTH = 38;
    const int CHAR_WINDOW_HEIGHT = 6;
    const int BOTTOM_WINDOW_HEIGHT = 10;
    const int MESSAGE_SUBWINDOW_OFFSET_W = 5;
    const int MESSAGE_SUBWINDOW_OFFSET_H = 3;
    const int MESSAGE_SUBWINDOW_X = 3;
    const int MESSAGE_SUBWINDOW_Y = 2;
    const int OFFSET = 1;

    const int LABEL_X = 2;
    const int HEALTH_Y = 2;
    const int MANA_Y = 3;
    const int STATUS_BAR_X = 8;

    const char BAR = '|';
    const int BAR_VALUE = 5;
    const int FULL_BAR_PERCENT = 100;

    const std::string character1Ascii =
            "                               ,----.\n"
                    "                              '      |\n"
                    "                             /       '\n"
                    "                       __,..'         \"-._        _\n"
                    "                  _.-\"\"                   `-.   ,\" `\".\n"
                    "         ,-._  _.'                           `\"'      '\n"
                    "       ,'    `\"                                       |\n"
                    "      .                                               .\n"
                    "       `.          _.--..               ____          '\n"
                    "       /         ,'    . `           ,\"' .  `.         `.\n"
                    "      /         .         |         /         \\          \\\n"
                    "     /          `------...'        ._____      .          \\\n"
                    "    .                                    `'\"\"\"'            \\\n"
                    "    '                    ________                           .\n"
                    "   j           `.\"\"/'\"\"\"`        '\"\"\"'\"'--....,-            |\n"
                    "   |             `/.                      ,\\ /              `.\n"
                    "   |                `-._               _.'  '                 `-.\n"
                    "   |                    `\"-----------\"'                         |\n"
                    " .\"                         ____                                |\n"
                    "|                      ,-\"\"'    `\".                            ,'\n"
                    "|                     .   .----.   `.                        .\"\n"
                    "`.._                  |  '.____,'   |                        '\n"
                    "    |             ,\". `.           ,' _                     /\n"
                    "    '            '   `._`.'._\".__,' .' .                   /\n"
                    "     .            `'-._ `._     _.-'  _.'                 /\n"
                    "      `.               `.  `--'\" _,.-'                    `\n"
                    "        .               ,'     .\"                          '\n"
                    "         '        .-..-' _,.--._`\"-..,-.                 ,'\n"
                    "        /         \\    ,'       `-.    |           .-'\"-\"\n"
                    "        \\          `-.'            `..'         _,'\n"
                    "         `.,.-\"`._                           ,-'\n"
                    "                  `\"-.                       |\n"
                    "                      \\       ,..----.     _.'\n"
                    "                       `\"\"---\"        `..-\"";

    const std::string character2Ascii =
            "                        _,.------....___,.' ',.-.\n"
                    "                     ,-'          _,.--\"        |\n"
                    "                   ,'         _.-'              .\n"
                    "                  /   ,     ,'                   `\n"
                    "                 .   /     /                     ``.\n"
                    "                 |  |     .                       \\.\\\n"
                    "       ____      |___._.  |       __               \\ `.\n"
                    "     .'    `---\"\"       ``\"-.--\"'`  \\               .  \\\n"
                    "    .  ,            __               `              |   .\n"
                    "    `,'         ,-\"'  .               \\             |    L\n"
                    "   ,'          '    _.'                -._          /    |\n"
                    "  ,`-.    ,\".   `--'                      >.      ,'     |\n"
                    " . .'\\'   `-'       __    ,  ,-.         /  `.__.-      ,'\n"
                    " ||:, .           ,'  ;  /  / \\ `        `.    .      .'/\n"
                    " j|:D  \\          `--'  ' ,'_  . .         `.__, \\   , /\n"
                    "/ L:_  |                 .  \"' :_;                `.'.'\n"
                    ".    \"\"'                  \"\"\"\"\"'                    V\n"
                    " `.                                 .    `.   _,..  `\n"
                    "   `,_   .    .                _,-'/    .. `,'   __  `\n"
                    "    ) \\`._        ___....----\"'  ,'   .'  \\ |   '  \\  .\n"
                    "   /   `. \"`-.--\"'         _,' ,'     `---' |    `./  |\n"
                    "  .   _  `\"\"'--.._____..--\"   ,             '         |\n"
                    "  | .\" `. `-.                /-.           /          ,\n"
                    "  | `._.'    `,_            ;  /         ,'          .\n"
                    " .'          /| `-.        . ,'         ,           ,\n"
                    " '-.__ __ _,','    '`-..___;-...__   ,.'\\ ____.___.'\n"
                    " `\"^--'..'   '-`-^-'\"--    `-^-'`.''\"\"\"\"\"`.,^.`.";

    const int MENU_NUM_ITEMS = 2;

    const std::string OFFENSE_CHOICE = "Offense Spells";
    const std::string DEFENSE_CHOICE = "Defense Spells";

    const std::string MENU_ITEM_NAMES[MENU_NUM_ITEMS] = {
            OFFENSE_CHOICE,
            DEFENSE_CHOICE,
    };

    gui::SpellMenuChoice getChoice(const std::string &val) {
        if (val == OFFENSE_CHOICE) {
            return gui::SpellMenuChoice::OFFENSE;
        } else {
            return gui::SpellMenuChoice::DEFENSE;
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
}

namespace gui {

    CombatWindow::CombatWindow() {
        int w = 0;
        int h = 0;
        getmaxyx(stdscr, h, w);
        size = Size{w, h};

        combatWindow = newwin(0, 0, 0, 0);
        CHECK(combatWindow) << "Error creating window";
        keypad(combatWindow, true);

        int subWindowTopY = 1;

        character1Window = derwin(combatWindow, CHAR_WINDOW_HEIGHT, CHAR_WINDOW_WIDTH, subWindowTopY,
                                  (w / 4) - (CHAR_WINDOW_WIDTH / 2));
        CHECK(character1Window) << "Error creating player 1 window";
        box(character1Window, 0, 0);
        drawCharacterWindow(character1Window, "johndoe", 5, 60, 35);

        character2Window = derwin(combatWindow, CHAR_WINDOW_HEIGHT, CHAR_WINDOW_WIDTH, subWindowTopY,
                                  (w - (w / 4)) - (CHAR_WINDOW_WIDTH / 2));
        CHECK(character2Window) << "Error creating player 2 window";
        box(character2Window, 0, 0);
        drawCharacterWindow(character2Window, "Bulbasaur", 8, 90, 0);

        character1AsciiWindow = derwin(combatWindow,
                                       h - CHAR_WINDOW_HEIGHT - BOTTOM_WINDOW_HEIGHT - OFFSET,
                                       w / 2,
                                       CHAR_WINDOW_HEIGHT + OFFSET, 0);
        drawCharacterAscii(character1AsciiWindow, character1Ascii);

        character2AsciiWindow = derwin(combatWindow,
                                       h - CHAR_WINDOW_HEIGHT - BOTTOM_WINDOW_HEIGHT - OFFSET,
                                       w / 2,
                                       CHAR_WINDOW_HEIGHT + OFFSET,
                                       w / 2);
        drawCharacterAscii(character2AsciiWindow, character2Ascii);

        messageWindow = derwin(combatWindow, BOTTOM_WINDOW_HEIGHT, w / 2, h - BOTTOM_WINDOW_HEIGHT, 0);
        CHECK(messageWindow) << "Error creating message window";
        box(messageWindow, 0, 0);

        messageSubWindow = derwin(messageWindow,
                                  getWindowHeight(messageWindow) - MESSAGE_SUBWINDOW_OFFSET_H,
                                  getWindowWidth(messageWindow) - MESSAGE_SUBWINDOW_OFFSET_W,
                                  MESSAGE_SUBWINDOW_Y, MESSAGE_SUBWINDOW_X);
        CHECK(messageSubWindow) << "Error creating message subwindow";
        scrollok(messageSubWindow, true);
        box(messageWindow, 0, 0);
        appendText("You attack Bulbasaur and deal 10 damage");
        appendText("You take 10 damage from Bulbasaur");
        appendText("You cast protection on yourself and heal 25");
        appendText("You cast soul petrification on Bulbasaur for 30 damage");
        appendText("You attack Bulbasaur and deal 0 damage");
        appendText("You take 20 damage from Bulbasaur");

        menuWindow = derwin(combatWindow, BOTTOM_WINDOW_HEIGHT, w / 2, h - BOTTOM_WINDOW_HEIGHT, w / 2);
        CHECK(menuWindow) << "Error creating menu window";
        box(menuWindow, 0, 0);

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
                if (callback) {
                    callback(choice);
                }
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
        delwin(menuWindow);
        delwin(messageWindow);
        delwin(character1Window);
        delwin(character2Window);
        delwin(character1AsciiWindow);
        delwin(character2AsciiWindow);
        delwin(combatWindow);
    }

    void CombatWindow::redraw() {
        wrefresh(character1Window);
        wrefresh(character2Window);
        wrefresh(messageWindow);
        wrefresh(menuWindow);
//        wrefresh(combatWindow);
    }

    void CombatWindow::resize(const Size &size) {
        this->size = size;
        recreate();
    }

    void CombatWindow::createMenu() {
        for (const auto &name : MENU_ITEM_NAMES) {
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

        for (ITEM *item : menuItems) {
            free_item(item);
        }
        menuItems.clear();
    }

    void CombatWindow::recreate() {
        int menuX = size.width / 2;
        int menuY = size.height / 2;
        mvwin(combatWindow, menuY, menuX);

        set_menu_win(menu, combatWindow);
        set_menu_sub(menu, menuWindow);
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