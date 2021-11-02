struct node
{
  int id;
};

struct artist : node
{
  int tag;
};

struct album : node
{
  int tag;
};

struct genre : node
{
  int tag;
};

struct song : node
{
  int tag;
  char *title;
  char *lyrics;
  char *loc;
  int year;
  int count;

  artist *artists;
  album *albums;
  genre *genres;
};

struct queue
{
  int tag;
  song *q;
};

struct playlist
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

song mapped to multiple artists
song mapped to multiple genres
song mapped to single album

genre is mapped to multiple songs
genre is mapped to multiple albums
genre is mapped to multiple artists

artist is mapped to multiple songs
artist is mapped to multiple albums

album mapped to multiple artists
album mapped to multiple songs
*/