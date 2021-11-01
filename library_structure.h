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
/*

Multiple Artists and Genre

song mapped to multiple artists, genre, 
song linked to single album

album linked to multiple artists
album linked to multiple songs
album linked to multiple genre
*/