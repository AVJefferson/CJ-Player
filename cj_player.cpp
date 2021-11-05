// Standard Header Files
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>

// Non-Standard Header Files
#include <ncurses.h>
#include "MusicLibrary.h"

// * GLOBAL VARIABLES
// Titles Tabs Variables

const int tabDefault = 0;
const int tabNos = 8; // Total Tabs
const char tabTitles[tabNos][12] = {"Playing Now", "Queues", "Playlists", "Genres", "Folders", "Artists", "Albums", "Settings"};
int tabTiitleLen[tabNos]; // String Length of all tab titles

int i;
bool isInsideTab = false;
int titleScreenXPos[tabNos] = {0}; // Cummulative Distance from LHS to the x coordinate of resp. Windows
int tabActive = tabDefault;        // Current Active Tab
struct winsize terminal_size;

void initGlobalVariables()
{
  // Get Screen Size of Terminal Window
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &terminal_size);

  // Calculate string lengths and use it to find x coordinates of TITLE WINDOWS
  tabTiitleLen[0] = strlen(tabTitles[0]);
  for (i = 1; i < tabNos; ++i)
  {
    tabTiitleLen[i] = strlen(tabTitles[i]);
    titleScreenXPos[i] = titleScreenXPos[i - 1] + tabTiitleLen[i - 1] + 3;
  }
  // Separate last lab to the end
  titleScreenXPos[i - 1] = terminal_size.ws_col - tabTiitleLen[i - 1] - 4;
}

// * GLOBAL WINDOW VARIABLES
WINDOW *TABS[tabNos];
WINDOW *BODY[tabNos];
WINDOW *FOOTER;

// Array of Body Functions
typedef void (*voidFuncPointers)(int tag);

void initGlobalScreens()
{
  wborder(stdscr, '*', '*', '*', '*', '*', '*', '*', '*');
  refresh();

  // Titles Windows
  for (i = 0; i < tabNos; ++i)
  {
    TABS[i] = newwin(3, tabTiitleLen[i] + 4, 0, titleScreenXPos[i]);
    wborder(TABS[i], '*', '*', '*', '*', '*', '*', '*', '*');
    mvwprintw(TABS[i], 1, 2, tabTitles[i]);
    wnoutrefresh(TABS[i]);
  }

  // Body Window
  for (i = 0; i < tabNos; ++i)
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

// ! exit() from stdlib.h does not work well with ncurses ... learnt the hard way...
int waitForKeyPress(int *deniedKeys = 0)
{
  int key;
  while (key = getch())
  {
    if (isInAllowedKeys(key, deniedKeys))
      return key;
  }
  return key;
}

void wprintwattr(WINDOW *w, int y, int x, int attribute, const char *t, ...)
{
  // ? Not Sure if this is wa to forward a variable list
  va_list lst;

  wattron(w, attribute);
  mvwprintw(w, y, x, t, lst);
  wattroff(w, attribute);

  va_end(lst);
}

class body
{
  static __int8_t id_n;
  static __int8_t id_q;
  static __int8_t id_p;
  static __int8_t id_g;
  static __int8_t id_f;
  static __int8_t id_ar;
  static __int8_t id_al;
  static __int8_t id_s;

  // Each lower order bit is a true/false
  static int toBeInit;

public:
  static void init_now(int tag);
  static void init_queue(int tag);
  static void init_playlist(int tag);
  static void init_genre(int tag);
  static void init_folder(int tag);
  static void init_artist(int tag);
  static void init_album(int tag);
  static void init_setting(int tag);

  static void now(int tag);
  static void queue(int tag);
  static void playlist(int tag);
  static void genre(int tag);
  static void folder(int tag);
  static void artist(int tag);
  static void album(int tag);
  static void setting(int tag);
};

__int8_t body::id_n = 0;
__int8_t body::id_q = 1;
__int8_t body::id_p = 2;
__int8_t body::id_g = 3;
__int8_t body::id_f = 4;
__int8_t body::id_ar = 5;
__int8_t body::id_al = 6;
__int8_t body::id_s = 7;

int body::toBeInit = 0;

void body::init_now(int tag)
{
  if (!((toBeInit >> id_n) & 1))
    mvwprintw(BODY[id_n], 1, 2, "NOW");
}

void body::init_queue(int tag)
{
  if (!((toBeInit >> id_q) & 1))
    mvwprintw(BODY[id_q], 1, 2, "QUEUE");
}

void body::init_playlist(int tag)
{
  if (!((toBeInit >> id_p) & 1))
    mvwprintw(BODY[id_p], 1, 2, "PLAYLIST");
}

void body::init_genre(int tag)
{
  if (!((toBeInit >> id_g) & 1))
    mvwprintw(BODY[id_g], 1, 2, "GENRE");
}

void body::init_folder(int tag)
{
  if (!((toBeInit >> id_f) & 1))
    mvwprintw(BODY[id_f], 1, 2, "FOLDER");
}

void body::init_artist(int tag)
{
  if (!((toBeInit >> id_ar) & 1))
    mvwprintw(BODY[id_ar], 1, 2, "ARTIST");
}

void body::init_album(int tag)
{
  if (!((toBeInit >> id_al) & 1))
    mvwprintw(BODY[id_al], 1, 2, "ALBUM");
}

void body::init_setting(int tag)
{
  if (!((toBeInit >> id_s) & 1))
    mvwprintw(BODY[id_s], 1, 2, "SETTINGS");
}

void body::now(int tag)
{
  wrefresh(BODY[id_n]);
}

void body::queue(int tag)
{
  wrefresh(BODY[id_q]);
}

void body::playlist(int tag)
{
  wrefresh(BODY[id_p]);
}

void body::genre(int tag)
{
  wrefresh(BODY[id_g]);
}

void body::folder(int tag)
{
  wrefresh(BODY[id_f]);
}

void body::artist(int tag)
{
  wrefresh(BODY[id_ar]);
}

void body::album(int tag)
{
  wrefresh(BODY[id_al]);
}

void body::setting(int tag)
{
  wrefresh(BODY[id_s]);
}

voidFuncPointers bodyFunctionArray[] =
    {
        body::now,
        body::queue,
        body::playlist,
        body::genre,
        body::folder,
        body::artist,
        body::album,
        body::setting};

voidFuncPointers bodyInitFunctionArray[] =
    {
        body::init_now,
        body::init_queue,
        body::init_playlist,
        body::init_genre,
        body::init_folder,
        body::init_artist,
        body::init_album,
        body::init_setting};

void switchToTab(int toTab = tabDefault, int tag = 0) // Default Tab
{
  // Remove Highlight from old tab
  mvwprintw(TABS[tabActive], 1, 2, tabTitles[tabActive]);
  wnoutrefresh(TABS[tabActive]);

  // Add Highlight to new tab
  wprintwattr(TABS[toTab], 1, 2, A_STANDOUT, tabTitles[toTab]);
  wnoutrefresh(TABS[toTab]);

  // Shift Body tab
  bodyInitFunctionArray[toTab](0);
  redrawwin(BODY[toTab]);
  wrefresh(BODY[toTab]);

  // Update Global Variable
  tabActive = toTab;

  // Print Changes made to physical screen
  doupdate();
}

void enterIntoTab(int toTab, int tag = 0)
{
  if (tabActive != toTab)
  {
    // Add light Highlight to old tab
    wprintwattr(TABS[tabActive], 1, 2, A_BOLD, tabTitles[tabActive]);
    wnoutrefresh(TABS[tabActive]);
    tabActive = toTab;
  }
  isInsideTab = true;

  // Blink Current Tab
  wprintwattr(TABS[tabActive], 1, 2, A_BLINK, tabTitles[tabActive]);
  wnoutrefresh(TABS[tabActive]);
  doupdate();

  // Init current BODY WINDOW with needed content
  bodyInitFunctionArray[toTab](0);
  // todo Init Right and Left Window (as threads) for 0 lag access;

  // Loop Inside Body
  bodyFunctionArray[toTab](tag);

  // Normalize Current Tab on exit
  mvwprintw(TABS[tabActive], 1, 2, tabTitles[tabActive]);
  wnoutrefresh(TABS[tabActive]);
  isInsideTab = false;
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
      if (tabActive != tabNos - 1)
        switchToTab(tabActive + 1);
      else
        switchToTab(0);
    }

    else if (c_ret == KEY_LEFT)
    {
      if (tabActive != 0)
        switchToTab(tabActive - 1);
      else
        switchToTab(tabNos - 2);
    }

    else if (c_ret == KEY_ESC && tabActive != tabDefault)
      switchToTab(tabDefault);

    else if (c_ret == '\n' || c_ret == KEY_DOWN)
      enterIntoTab(tabActive);

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

  // Loop accross availabe tabs
  selectTabs();

  // Deallocates memory from ncurses and exit program and other processes
  for (int i = 0; i < tabNos; ++i)
  {
    delwin(TABS[i]);
    delwin(BODY[i]);
  }
  delwin(FOOTER);
  endwin();
  return 0;
}
