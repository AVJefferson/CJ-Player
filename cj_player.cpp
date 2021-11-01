#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <ncurses.h>

struct winsize o, n;

const int title_no = 8;
char titles[][12] = {"Playing Now", "Queues", "Playlists", "Genres", "Folders", "Artists", "Albums", "Settings"};
int title_len[title_no];
int title_len_cumm[title_no] = {0};
int active_tab = 0;

WINDOW *TITLES[title_no];
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

int now()
{

  int i, j, press_key;

  // Tab Switch
  mvwprintw(TITLES[active_tab], 1, 1, titles[active_tab]);
  wrefresh(TITLES[active_tab]);
  attrset(A_REVERSE);
  mvwprintw(TITLES[0], 1, 1, titles[0]);
  attrset(A_NORMAL);
  wrefresh(TITLES[0]);
  active_tab = 0;

  while (press_key = getch())
  {
    if (press_key == 'q')
      break;
  }

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

  // VARIABLES USED
  char c_ret;
  int i, j;

  // String Lengths Buffer
  title_len[0] = strlen(titles[0]);
  for (i = 1; i < title_no; ++i)
  {
    title_len[i] = strlen(titles[i]);
    title_len_cumm[i] = title_len_cumm[i - 1] + title_len[i - 1] + 3;
  }

  // WINDOWS USED
  // Titles Windows
  for (i = 0; i < title_no; ++i)
  {
    TITLES[i] = newwin(3, title_len[i] + 4, 0, title_len_cumm[i]);
    box(TITLES[i], 0, 0);
  }

  // Body Window
  BODY = newwin(o.ws_row - 4, o.ws_col, 3, 0);
  box(BODY, 0, 0);

  // Footer Window
  FOOTER = newwin(3, o.ws_col, o.ws_row - 3, 0);
  box(FOOTER, 0, 0);

  // Default Tab
  now();

  // Deallocates memory from ncurses
  //delete[] TITLES;
  //delete BODY;
  //delete FOOTER;
  endwin();
  return 0;
}