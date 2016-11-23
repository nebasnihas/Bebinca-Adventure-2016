#ifndef ADVENTURE2016_COMBATWINDOW_HPP
#define ADVENTURE2016_COMBATWINDOW_HPP

#include <memory>
#include <vector>
#include <menu.h>
#include <functional>
#include "Window.hpp"

namespace gui {

enum class SpellMenuChoice {
    OFFENSE,
    DEFENSE,
};

class CombatWindow : public Window {
public:
    CombatWindow();
    virtual ~CombatWindow();

    virtual void update(int input) override;
    virtual WINDOW* getCursesWindow() override;
    virtual void resize(const Size& size) override;
    virtual void redraw() override;
    virtual void onEnter() override;

    void setOnSelection(std::function<void(SpellMenuChoice)> callback);
private:
    void createMenu();
    void removeMenu();
    void recreate();
    void appendText(const std::string& text);

    WINDOW* combatWindow;
    WINDOW* player1Window;
    WINDOW* player2Window;
    WINDOW* messageWindow;
    WINDOW* menuWindow;
    std::vector<ITEM*> menuItems;
    MENU* menu;
    Size size;
    std::function<void(SpellMenuChoice)> callback;
};

}

#endif //ADVENTURE2016_COMBATWINDOW_HPP
