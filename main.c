#include "oseroplus.h"
int main(){
    srand((unsigned int)time(NULL));
    initscr();
    noecho();
    cbreak();
    curs_set(0);
    timeout(500);
    keypad(stdscr,true);
    menu();
    reset_board();
    game();
    endwin();
    return 0;
}