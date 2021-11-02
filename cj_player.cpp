// Standard Library Header Files
#include <string.h>

#include <unistd.h>
#include <sys/ioctl.h>

// Used Header Files
#include <ncurses.h>
#include "library_structure.h"

// * GLOBAL VARIABLES
// Titles Tabs Variables
int i;
const int tab_no = 8;         // Total Tabs
int tab_default = 0;          // Default Tab During Startup
bool isInsideTab = false;     // If Inside a Tab
int tab_active = tab_default; // Current Active Tab
char tab_titles[][12] = {"Playing Now", "Queues", "Playlists", "Genres", "Folders", "Artists", "Albums", "Settings"};
int tab_title_len[tab_no];            // String Length of all tab titles
int tab_title_len_cumm[tab_no] = {0}; // Cummulative Distance from LHS to the x coordinate of resp. Windows

void initGlobalVariables()
{
  // String Lengths Buffer
  tab_title_len[0] = strlen(tab_titles[0]);
  for (i = 1; i < tab_no; ++i)
  {
    tab_title_len[i] = strlen(tab_titles[i]);
    tab_title_len_cumm[i] = tab_title_len_cumm[i - 1] + tab_title_len[i - 1] + 3;
  }
}

// * WINDOW AND TERMINAL GLOBAL VARIABLES
struct winsize terminal_size;
WINDOW *TABS[tab_no];
WINDOW *BODY[tab_no];
WINDOW *FOOTER;

// Array of Body Functions
typedef void (*voidFunctionPointers)(int tag);

void initGlobalScreens()
{
  // Get Screen Size of Terminal Window
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal_size);

  // Titles Windows
  for (i = 0; i < tab_no; ++i)
  {
    TABS[i] = newwin(3, tab_title_len[i] + 4, 0, tab_title_len_cumm[i]);
    wborder(TABS[i], '*', '*', '*', '*', '*', '*', '*', '*');
    mvwprintw(TABS[i], 1, 2, tab_titles[i]);
    wnoutrefresh(TABS[i]);
  }

  // Body Window
  for (i = 0; i < tab_no; ++i)
  {
    BODY[i] = newwin(terminal_size.ws_row - 5, terminal_size.ws_col, 2, 0);
    wborder(BODY[i], '*', '*', '*', '*', '*', '*', '*', '*');
    wnoutrefresh(BODY[i]);
  }

  // Footer Window
  FOOTER = newwin(4, terminal_size.ws_col, terminal_size.ws_row - 4, 0);
  wborder(FOOTER, '*', '*', '*', '*', '*', '*', '*', '*');
  mvwprintw(FOOTER, 1, 2, "%s - %s - %s", "Unknown Artists", "Unknown Album", "Unknown Title"); // Marquee Update

  mvwprintw(FOOTER, 2, 2, "Y[%04d]  Fav  C[%04d]", 0, 0);
  mvwprintw(FOOTER, 2, 30, "No Music in Queue");                          // Player Status <music no> in <queue name>
  mvwprintw(FOOTER, 2, terminal_size.ws_col - 21, "[HH:MM:SS/HH:MM:SS]"); // Live Update

  mvwprintw(FOOTER, 3, 2, "R[1>]**Q[1>]**Volume[01]**Shuffle");
  mvwprintw(FOOTER, 3, terminal_size.ws_col - 18, "Help(H)**Quit(Q)");
  wnoutrefresh(FOOTER);
}

// Keyboard Keys Accepted by waitForKeyPress()
#define KEY_ESC 27
// To Optimize, sort in order of likeliness to occur
int allowedKeys[] = {KEY_RIGHT, KEY_LEFT, KEY_DOWN, KEY_UP, '\n', KEY_ESC, 'q', -1}; // -1 is end of array

bool isInAllowedKeys(int key, int *deniedKeys = 0)
{
  if (deniedKeys)
    for (int *i = deniedKeys; *i != -1; ++i)
      if (key == *i)
        return false;

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

int waitForKeyPress(int *deniedKeys = 0)
{
  int key;
  while (key = getch())
  {
    if (isInAllowedKeys(key, deniedKeys))
      return key;
  }
}

void wprintwattr(WINDOW *w, int y, int x, int attr, const char *t, ...)
{
  va_list lst;

  wattron(w, attr);
  mvwprintw(w, y, x, t, lst);
  wattroff(w, attr);

  va_end(lst);
}

class body
{
public:
  static int initOnce;
  static void init(int tab);

  static void now(int tag);
  static void queue(int tag);
  static void playlist(int tag);
  static void genre(int tag);
  static void folder(int tag);
  static void artist(int tag);
  static void album(int tag);
  static void setting(int tag);
};

int body::initOnce = 0;

void body::init(int tab)
{
}

void body::now(int tag)
{
  int id = 0;
  if (!(initOnce & 0b00000001))
    mvwprintw(BODY[id], 1, 2, "NOW");
  wrefresh(BODY[id]);
}

void body::queue(int tag)
{
  int id = 1;
  if (!(initOnce & 0b00000010))
    mvwprintw(BODY[id], 1, 2, "QUEUE");
  wrefresh(BODY[id]);
}

void body::playlist(int tag)
{
  int id = 2;
  if (!(initOnce & 0b00000100))
    mvwprintw(BODY[id], 1, 2, "PLAYLIST");
  wrefresh(BODY[id]);
}

void body::genre(int tag)
{
  int id = 3;
  if (!(initOnce & 0b00001000))
    mvwprintw(BODY[id], 1, 2, "GENRE");
  wrefresh(BODY[id]);
}

void body::folder(int tag)
{
  int id = 4;
  if (!(initOnce & 0b00010000))
    mvwprintw(BODY[id], 1, 2, "FOLDER");
  wrefresh(BODY[id]);
}

void body::artist(int tag)
{
  int id = 5;
  if (!(initOnce & 0b00100000))
    mvwprintw(BODY[id], 1, 2, "ARTIST");
  wrefresh(BODY[id]);
}

void body::album(int tag)
{
  int id = 6;
  if (!(initOnce & 0b01000000))
    mvwprintw(BODY[id], 1, 2, "ALBUM");
  wrefresh(BODY[id]);
}

void body::setting(int tag)
{
  int id = 7;
  if (!(initOnce & 0b10000000))
    mvwprintw(BODY[id], 1, 2, "SETTINGS");
  wrefresh(BODY[id]);
}

voidFunctionPointers bodyFunctionArray[] =
    {
        body::now,
        body::queue,
        body::playlist,
        body::genre,
        body::folder,
        body::artist,
        body::album,
        body::setting};

void switchToTab(int toTab = tab_default, int tag = 0) // Default Tab
{
  // Remove Highlight from old tab
  mvwprintw(TABS[tab_active], 1, 2, tab_titles[tab_active]);
  wnoutrefresh(TABS[tab_active]);

  // Add Highlight to new tab
  wprintwattr(TABS[toTab], 1, 2, A_STANDOUT, tab_titles[toTab]);
  wnoutrefresh(TABS[toTab]);

  // Shift Body tab
  redrawwin(BODY[toTab]);
  wrefresh(BODY[toTab]);

  // Update Global Variable
  tab_active = toTab;

  // Print Changes made to physical screen
  doupdate();
}

void enterIntoTab(int toTab, int tag = 0)
{
  if (tab_active != toTab)
    switchToTab(toTab, tag);
  isInsideTab = true;

  // Blink Current Tab
  wprintwattr(TABS[tab_active], 1, 2, A_BOLD, tab_titles[tab_active]);
  wnoutrefresh(TABS[tab_active]);
  doupdate();

  // Init Body Of Tabs To the left and right for quick access as async process
  // Code Here

  // Shift Body tab
  bodyFunctionArray[toTab](tag);

  // Normalize Current Tab
  mvwprintw(TABS[tab_active], 1, 2, tab_titles[tab_active]);
  wnoutrefresh(TABS[tab_active]);
  isInsideTab = false;
}

void switchToBody(int toTab, int tag = 0)
{
  // Entering Not Currently Active Tab
  if (tab_active != toTab)
  {
    enterIntoTab(toTab, tag);
  }
  else
    bodyFunctionArray[toTab](tag);
}

void selectTabs()
{
  int deniedKeys[] = {KEY_UP, -1};
  switchToTab();
  int c_ret = waitForKeyPress(deniedKeys);

  // Loop Tab Switching
  while (c_ret != 'q')
  {
    if (c_ret == KEY_RIGHT)
    {
      if (tab_active != tab_no - 1)
        switchToTab(tab_active + 1);
      else
        switchToTab(0);
    }

    else if (c_ret == KEY_LEFT)
    {
      if (tab_active != 0)
        switchToTab(tab_active - 1);
      else
        switchToTab(tab_no - 2);
    }

    else if (c_ret == KEY_ESC && tab_active != tab_default)
      switchToTab(tab_default);

    else if (c_ret == '\n' || c_ret == KEY_DOWN)
      enterIntoTab(tab_active);

    // Loop Waiting for key press
    c_ret = waitForKeyPress();
  }
}

int main(int argc, char **argv)
{
  // ncurses Init Functions
  initscr();            // Init ncurses main screen
  noecho();             // Do Not Print Key Presses
  refresh();            // Update the 3 Global Windows in ncurses
  curs_set(0);          // Remove Blinking Cursor
  keypad(stdscr, TRUE); // Allow Arrow Keys
  ESCDELAY = 0;         // No delay in ESC action to occur

  // Run Once On Startup
  initGlobalVariables();
  initGlobalScreens();
  selectTabs();

  // Deallocates memory from ncurses and exit program and other processes
  for (int i = 0; i < tab_no; ++i)
  {
    delwin(TABS[i]);
    delwin(BODY[i]);
  }
  delwin(FOOTER);
  endwin();
  return 0;
}
