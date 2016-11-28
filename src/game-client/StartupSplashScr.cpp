//
//
//

#include "StartupSplashScr.hpp"


void StartupSplashScr::animateSplash() {

    WINDOW *win;
    struct winsize wind;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &wind);

    //int beg_y = wind.ws_row /8;
     int beg_x = (wind.ws_col /2)- 45;

    win = newwin(wind.ws_row,wind.ws_col,0,beg_x);

    //initscr();
    int width = 0;
    int height = 0;

    start_color();
    init_pair(1,COLOR_WHITE, COLOR_BLACK);
    wattron(win,COLOR_PAIR(1));
    const char *cr = AsciiConverter::convertObject0("def","source").c_str();
    print_middle(win, 0,0,80,0, cr);
    wattroff(win,COLOR_PAIR(1));
    wgetch(win);
    this_thread::sleep_for(chrono::milliseconds(500));


}

void StartupSplashScr::print_middle(WINDOW *win, int starty, int startx, int width, int height, const char* str) {

    int length,y,x;
    float calc;

    getyx(win, y,x);
    std::string ss(str);

    length = ss.length();
    calc = (width-length)/2;
    x = startx + (int) calc;
    mvwprintw(win,starty,startx,"%s", str);
    //refresh();


}