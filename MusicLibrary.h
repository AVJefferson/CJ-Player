#include <vector>
#include <string>

char *UNKNOWN = "Unknown";

class node
{
  static unsigned int nextId;
  unsigned int id;

public:
  node()
  {
    id = nextId++;
  }
};
unsigned int node::nextId = 0;

class song : node
{
  char *title;
  char *lyrics;
  char *loc;
  char *artist;
  int year;
  int count;

  int album;
  int *artists;
  int *genres;

public:
  song()
  {
  }
};

class artist : node
{
  char *name;

  int *songs;
  int *albums;

public:
  artist()
  {
  }
};

class album : node
{
  char *name;

  int *songs;
  int *artists;

public:
  album()
  {
  }
};

class genre : node
{
  char *name;

  int *songs;
  int *artists;

public:
  genre()
  {
  }
};

class queue
{
  int id;
  int *q;
};

class playlist
{
  int id;
  int *q;
};

/*

queue is a queue of songs selected
playlist is a queue of pre-selectable songs
*/