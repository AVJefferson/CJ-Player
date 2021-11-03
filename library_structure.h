class node
{
  unsigned int id;
  node *edge;

public:
  node(int i)
  {
    id = i;
  }
};

class song : node
{
  char *title;
  char *lyrics;
  char *loc;
  char *artist;
  int year;
  int count;
  node *album;

  node *artists;
  node *genres;

public:
  song(int i) : node(i)
  {
  }
};

class artist : node
{
  char *name;

  node *songs;
  node *albums;
  node *genres;

public:
  artist(int i) : node(i)
  {
  }
};

class album : node
{
  int tag;

  node *songs;
  node *artists;
  node *genres;

public:
  album(int i) : node(i)
  {
  }
};

class genre : node
{
  int tag;

  node *songs;
  node *albums;
  node *artists;

public:
  genre(int i) : node(i)
  {
  }
};

class queue
{
  int tag;
  song *q;
};

class playlist
{
  int tag;
  song *q;
};

/*

UNDIRECTED GRAPH SRTUCTURE
  All can be stored as multiple sets.
  mapping is done through arrays of ids

queue is a queue of songs selected
playlist is a queue of pre-selectable songs
*/