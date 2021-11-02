#include <string.h>
#include <ncurses.h>

#include <unistd.h>
#include <sys/ioctl.h>

#include "library_structure.h"

// Attempt at Array of Functions
// typedef int (*IntFunction);
// IntFunction functions[] =
//   {
//     f1,
//     f2,
//     f3};
// };

// Titles Tabs Variables
const int tab_no = 8;         // Total Tabs
int tab_default = 0;          // Default Tab During Startup
int tab_active = tab_default; // Current Active Tab
char tab_titles[][12] = {"Playing Now", "Queues", "Playlists", "Genres", "Folders", "Artists", "Albums", "Settings"};
int tab_title_len[tab_no];            // String Length of all tab titles
int tab_title_len_cumm[tab_no] = {0}; // Cummulative Distance from LHS to the x coordinate of resp. Windows

// WINDOW AND TERMINAL GLOBAL VARIABLES
struct winsize terminal_size;
WINDOW *TABS[tab_no];
WINDOW *BODY;
WINDOW *FOOTER;

// Keyboard Keys Accepted by waitForKeyPress()
#define KEY_ESC 27
// To Optimize, sort in order of UNlikeliness to occur
int allowedKeys[] = {KEY_RIGHT, KEY_LEFT, KEY_ESC, 'q', -1}; // -1 Indicates end of array

bool isInAllowedKeys(int key)
{
  for (int *i = allowedKeys; *i != -1; ++i)
    if (key == *i)
      return true;
  return false;
}

// void resize()
// {
//   while (o.ws_col == n.ws_col && n.ws_row == o.ws_row)
//     ioctl(STDOUT_FILENO, TIOCGWINSZ, &n);
//   ioctl(STDOUT_FILENO, TIOCGWINSZ, &o);
// }

int waitForKeyPress()
{
  int key;

  // Loop Wait Till you get allowed Keys
  while (key = getch())
  {
    if (isInAllowedKeys(key))
      return key;
  }
}

void switchTabTo(int toTab = tab_default) // Default Tab
{
  // Remove Highlight from old tab
  wattroff(TABS[tab_active], A_STANDOUT);
  mvwprintw(TABS[tab_active], 1, 2, tab_titles[tab_active]);
  wrefresh(TABS[tab_active]);

  // Add Highlight to new tab
  wattron(TABS[toTab], A_STANDOUT);
  mvwprintw(TABS[toTab], 1, 2, tab_titles[toTab]);
  wattroff(TABS[toTab], A_STANDOUT);
  wrefresh(TABS[toTab]);

  // Update Global Variable
  tab_active = toTab;
}

int main(int argc, char **argv)
{
  // VARIABLES USED
  int i, c_ret = 0;

  // Get Screen Size of Terminal Window
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal_size);

  // ncurses Init Functions
  initscr();
  noecho();             // Do Not Print Key Presses
  keypad(stdscr, TRUE); // Allow Arrow Keys
  refresh();
  curs_set(0); // Remove Blinking Cursor

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
  BODY = newwin(terminal_size.ws_row - 5, terminal_size.ws_col, 2, 0);
  wborder(BODY, '*', '*', '*', '*', '*', '*', '*', '*');
  wrefresh(BODY);

  // Footer Window
  FOOTER = newwin(4, terminal_size.ws_col, terminal_size.ws_row - 4, 0);
  wborder(FOOTER, '*', '*', '*', '*', '*', '*', '*', '*');
  mvwprintw(FOOTER, 1, 2, "%s - %s - %s", "Unknown Artists", "Unknown Album", "Unknown Title"); // Marquee Update

  mvwprintw(FOOTER, 2, 2, "Y[%04d]  Fav  C[%04d]", 0, 0);
  mvwprintw(FOOTER, 2, 30, "No Music in Queue");                          // Player Status Update
  mvwprintw(FOOTER, 2, terminal_size.ws_col - 21, "[HH:MM:SS/HH:MM:SS]"); // Live Update

  mvwprintw(FOOTER, 3, 2, "R[1>]**Q[1>]**Volume[01]**Shuffle");
  mvwprintw(FOOTER, 3, terminal_size.ws_col - 18, "Help(H)**Quit(Q)");
  wrefresh(FOOTER);

  switchTabTo(); // Run Once On Startup
  // Loop Tabs
  while (c_ret != 'q')
  {
    // Loop Waiting for key press
    c_ret = waitForKeyPress();

    if (c_ret == KEY_RIGHT)
    {
      if (tab_active != tab_no - 1)
        switchTabTo(tab_active + 1);
      else
        switchTabTo(0);
    }
    else if (c_ret == KEY_LEFT)
    {
      if (tab_active != 0)
        switchTabTo(tab_active - 1);
      else
        switchTabTo(tab_no - 2);
    }
  }

  // Deallocates memory from ncurses and exit program and other processes
  for (i = 0; i < tab_no; ++i)
    delwin(TABS[i]);
  delwin(BODY);
  delwin(FOOTER);
  endwin();
  return 0;
}
