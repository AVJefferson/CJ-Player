#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <ncurses.h>

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

// todo NOW PLAYING
// *************************************************************************************
// * Playing Now * Queues * Playlists * Genres * Folders * Artists * Albums * Settings * // Highlight Open Tab using Color
// *************************************************************************************
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx *                 (LYRIC)                 * // Current LIne in Bold
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx *                                         *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx *                                         *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx *                                         *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx *                                         *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx *                                         *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx *                                         *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx *                                         *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx *                                         *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx *                                         *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx *                                         *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx *                                         *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx *                                         *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx *                                         *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx *                                         *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx *                                         *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx *                                         *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx *                                         *
// *******************************************                                         *
// * Title:                                  *                                         * // Marquee
// * Artists:                                *                                         * // Marquee
// * Album:                                  *                                         * // Marquee
// ************************************************************************************* // Color coded to indicate progress
// * Year[YYYY]            Previous(H) * Play/Pause( ) * Next(L)   [HH:MM:SS/HH:MM:SS] *
// * Favourite (F) * Help (H) * Info (I) * Artists (A) * Album (Z) * AddToPlaylist (P) * // Highlight Fav if its fav
// * Toggle Repeat(R)[1] * Toggle Jump Queue(Q)[1] * Volume[09] * Shuffle(S) * Find(S) *
// *************************************************************************************

// todo Queues
// *************************************************************************************
// * Playing Now * Queues * Playlists * Genres * Folders * Artists * Albums * Settings * // Highlight Open Tab using Color
// *************************************************************************************
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx *                   ***                   * // Start of List
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <0001> <artist> - <title>               *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <0002> <artist> - <title>               *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <0003> <artist> - <title>               *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <0004> <artist> - <title>               *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <0005> <artist> - <title>               * // Current Music in Bold
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <0006> <artist> - <title>               * // Cursor Music in highlight
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <0007> <artist> - <title>               * // Highlighted music data in LHS
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <0008> <artist> - <title>               *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <0009> <artist> - <title>               *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <0010> <artist> - <title>               *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <0011> <artist> - <title>               *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <0012> <artist> - <title>               *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <0013> <artist> - <title>               *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <0014> <artist> - <title>               *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <0015> <artist> - <title>               *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <0016> <artist> - <title>               *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <0017> <artist> - <title>               *
// ******************************************* <0018> <artist> - <title>               *
// * Title:                                  * <0019> <artist> - <title>               * // Marquee
// * Artists:                                * <0020> <artist> - <title>               * // Marquee
// * Album:                                  *                   ***                   * // Marquee, Reach end of list
// ************************************************************************************* // Color coded to indicate progress
// * Year[YYYY]            Previous(H) * Play/Pause( ) * Next(L)   [HH:MM:SS/HH:MM:SS] *
// * Favourite (F) * Help (H) * Info (I) * Artists (A) * Album (Z) * AddToPlaylist (P) * // Highlight Fav if its fav
// * Toggle Repeat(R)[1] * Toggle Jump Queue(Q)[1] * Volume[09] * Shuffle(S) * Find(S) * // Q for quiting
// *************************************************************************************

// todo Playlist
// *************************************************************************************
// * Playing Now * Queues * Playlists * Genres * Folders * Artists * Albums * Settings * // Highlight Open Tab using Color
// *************************************************************************************
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx *                   ***                   * // Start of List
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <artist> - <title>                      *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <artist> - <title>                      *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <artist> - <title>                      *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <artist> - <title>                      *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <artist> - <title>                      * // Current Music in Bold
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <artist> - <title>                      * // Cursor Music in highlight
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <artist> - <title>                      * // Highlighted music data in LHS
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <artist> - <title>                      *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <artist> - <title>                      *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <artist> - <title>                      *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <artist> - <title>                      *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <artist> - <title>                      *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <artist> - <title>                      *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <artist> - <title>                      *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <artist> - <title>                      *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <artist> - <title>                      *
// * xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx * <artist> - <title>                      *
// ******************************************* <artist> - <title>                      *
// * Title:                                  * <artist> - <title>                      * // Marquee
// * Artists:                                * <artist> - <title>                      * // Marquee
// * Album:                                  *                   ***                   * // Marquee, Reach end of list
// ************************************************************************************* // Color coded to indicate progress
// * Year[YYYY]    Fav     Previous(H) * Play/Pause( ) * Next(L)   [HH:MM:SS/HH:MM:SS] *
// **R[1>]**Q[1>]**Volume[01]**Shuffle********************************Help(H)**Quit(Q)**

void resize()
{
  while (o.ws_col == n.ws_col && n.ws_row == o.ws_row)
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &n);
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &o);
}

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

int title_menus(int tab = 0)
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

  // Memory Allocation for ncurses
  initscr();
  noecho();
  keypad(stdscr, TRUE);
  refresh();

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
  BODY = newwin(o.ws_row - 4, o.ws_col, 2, 0);
  wborder(BODY, '*', '*', '*', '*', '*', '*', '*', '*');
  wrefresh(BODY);

  // Footer Window
  FOOTER = newwin(3, o.ws_col, o.ws_row - 3, 0);
  wborder(FOOTER, '*', '*', '*', '*', '*', '*', '*', '*');
  mvwprintw(FOOTER, 1, 2, "Y[0000]  Fav  C[NNNN]  <Title> - <artist>"); // Marquee
  mvwprintw(FOOTER, 1, o.ws_col - 15, "[MM:SS/MM:SS]");
  mvwprintw(FOOTER, 2, 2, "R[1>]**Q[1>]**Volume[01]**Shuffle");
  mvwprintw(FOOTER, 2, o.ws_col - 18, "Help(H)**Quit(Q)");
  wrefresh(FOOTER);

  // Loop Tabs
  c_ret = title_menus(active_tab);
  while (c_ret != 'q')
  {
    if (c_ret == KEY_RIGHT)
    {
      if (active_tab != tab_no - 1)
        c_ret = title_menus(active_tab + 1);
      else
        c_ret = title_menus(0);
    }
    else if (c_ret == KEY_LEFT)
    {
      if (active_tab != 0)
        c_ret = title_menus(active_tab - 1);
      else
        c_ret = title_menus(tab_no - 2);
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