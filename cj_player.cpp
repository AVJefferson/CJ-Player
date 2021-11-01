#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <ncurses.h>

#include "library_structure.h"

// Attempt at Array of Functions
// typedef int (*IntFunction);
// IntFunction functions[] =
//   {
//     f1,
//     f2,
//     f3};
// };

struct winsize o, n;

const int tab_no = 8;
char tab_titles[][12] = {"Playing Now", "Queues", "Playlists", "Genres", "Folders", "Artists", "Albums", "Settings"};
int tab_title_len[tab_no];
int tab_title_len_cumm[tab_no] = {0};
int active_tab = 0;

WINDOW *TABS[tab_no];
WINDOW *BODY;
WINDOW *FOOTER;

// void resize()
// {
//   while (o.ws_col == n.ws_col && n.ws_row == o.ws_row)
//     ioctl(STDOUT_FILENO, TIOCGWINSZ, &n);
//   ioctl(STDOUT_FILENO, TIOCGWINSZ, &o);
// }

int waitForKeyPress()
{
  int key;
  while (key = getch())
  {
    if (key == 'q')
      return key;
    else if (key == KEY_LEFT)
      return key;
    else if (key == KEY_RIGHT)
      return key;
  }
  return key;
}

int tabs(int tab = 0)
{
  int i, j, press_key;

  // Remove Highlight from old tab
  wattroff(TABS[active_tab], A_STANDOUT);
  mvwprintw(TABS[active_tab], 1, 2, tab_titles[active_tab]);
  wrefresh(TABS[active_tab]);

  // Add Highlight to new tab
  wattron(TABS[tab], A_STANDOUT);
  mvwprintw(TABS[tab], 1, 2, tab_titles[tab]);
  wattroff(TABS[tab], A_STANDOUT);
  wrefresh(TABS[tab]);

  refresh();
  active_tab = tab;

  // Loop Waiting for key press
  press_key = waitForKeyPress();
  return press_key;
}

int main(int argc, char **argv)
{
  // Get Screen Size
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &o);

  // ncurses Init Functions
  initscr();
  noecho();             // Do Not Print Key Presses
  keypad(stdscr, TRUE); // Allow Arrow Keys
  refresh();
  curs_set(0); // Remove Blinking Cursor

  // VARIABLES USED
  int i, j, c_ret = 0;

  // String Lengths Buffer
  tab_title_len[0] = strlen(tab_titles[0]);
  for (i = 1; i < tab_no; ++i)
  {
    tab_title_len[i] = strlen(tab_titles[i]);
    tab_title_len_cumm[i] = tab_title_len_cumm[i - 1] + tab_title_len[i - 1] + 3;
  }

  // WINDOWS USED
  // Titles Windows
  for (i = 0; i < tab_no; ++i)
  {
    TABS[i] = newwin(3, tab_title_len[i] + 4, 0, tab_title_len_cumm[i]);
    wborder(TABS[i], '*', '*', '*', '*', '*', '*', '*', '*');
    mvwprintw(TABS[i], 1, 2, tab_titles[i]);
    wrefresh(TABS[i]);
  }

  // Body Window
  BODY = newwin(o.ws_row - 5, o.ws_col, 2, 0);
  wborder(BODY, '*', '*', '*', '*', '*', '*', '*', '*');
  wrefresh(BODY);

  // Footer Window
  FOOTER = newwin(4, o.ws_col, o.ws_row - 4, 0);
  wborder(FOOTER, '*', '*', '*', '*', '*', '*', '*', '*');
  mvwprintw(FOOTER, 1, 2, "%s - %s - %s", "Unknown Artists", "Unknown Album", "Unknown Title"); // Marquee Update

  mvwprintw(FOOTER, 2, 2, "Y[%04d]  Fav  C[%04d]", 0, 0);
  mvwprintw(FOOTER, 2, 30, "No Music in Queue"); // Player Status Update
  mvwprintw(FOOTER, 2, o.ws_col -21, "[HH:MM:SS/HH:MM:SS]"); // Live Update

  mvwprintw(FOOTER, 3, 2, "R[1>]**Q[1>]**Volume[01]**Shuffle");
  mvwprintw(FOOTER, 3, o.ws_col - 18, "Help(H)**Quit(Q)");
  wrefresh(FOOTER);

  // Loop Tabs
  c_ret = tabs(active_tab);
  while (c_ret != 'q')
  {
    if (c_ret == KEY_RIGHT)
    {
      if (active_tab != tab_no - 1)
        c_ret = tabs(active_tab + 1);
      else
        c_ret = tabs(0);
    }
    else if (c_ret == KEY_LEFT)
    {
      if (active_tab != 0)
        c_ret = tabs(active_tab - 1);
      else
        c_ret = tabs(tab_no - 2);
    }
  }

  // Deallocates memory from ncurses
  for (i = 0; i < tab_no; ++i)
    delwin(TABS[i]);
  delwin(BODY);
  delwin(FOOTER);
  endwin();
  return 0;
}

