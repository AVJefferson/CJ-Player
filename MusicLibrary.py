import eyed3

allSongs = {}
allArtists = {}
allAlbums = {}
allGenres = {}

lastId = 0


class Node:
    id

    def __init__(self):
        global lastId
        self.id = lastId
        lastId += 1


class Artist(Node):
    name = "Unknown"
    songs = []
    albums = []
    genres = []

    def __init__(self, name: str):
        Node.__init__(self)
        global allArtists
        allArtists[name] = self
        self.name = name


class Album(Node):
    name = "Unknown"
    songs = []
    artists = []
    genres = []

    def __init__(self, name: str):
        Node.__init__(self)
        global allAlbums
        allAlbums[name] = self
        self.name = name


class Genre(Node):
    name = "Unknown"
    songs = []
    albums = []
    artists = []

    def __init__(self, name: str):
        Node.__init__(self)
        global allGenres
        allGenres[name] = self
        self.name = name


class Song(Node):
    loc = "/"
    title = "Unknown"
    lyrics = "Embedded Lyrics Not Found"
    artist = "Unknown"
    year = 0
    count = 0
    album = "Unknown"
    artists = []
    genres = []

    def __init__(self, location, title, lyrics, artist, year, count=0):
        Node.__init__(self)
        global allSongs
        allSongs[location] = self
        self.loc = location
        self.title = title
        self.lyrics = lyrics
        self.artist = artist
        self.year = year
        self.count = count


def addSongToLibrary(mp3path: str, f):
    global allAlbums, allArtists, allSongs, allGenres
    mp3file = eyed3.load(mp3path)

    try:
        t = str(mp3file.tag.title)
        if t == "None":
            t = "unknown"

        al = str(mp3file.tag.album)
        if al == "None":
            "Unknown"

        try:
            y = int(str(mp3file.tag.recording_date))
        except:
            y = 0

        try:
            ly = mp3file.lyrics[0].text
        except:
            ly = "Embedded Lyrics Not Found"

        try:
            g = str((mp3file.tag.genre.name)).split(' | ')
        except:
            g = ['Unknown']

        ar = str(mp3file.tag.artist)
        art = []
        if ar == "None":
            art = ["Unknown"]
        else:
            art = ar.replace(' Ft.', ',').replace(' &', ',').split(', ')

        # All data Availabe for graph creation - song, artists, genres, albums
        track = Song(mp3path, t, ly, ar, y)

        # Create and/or add artists, genres and album and link
        for musician in art:
            if musician not in allArtists.keys():
                temp = Artist(str(musician))
                track.artists.append(temp)
                temp.songs.append(track)
            else:
                temp = allArtists[musician]
                track.artists.append(temp)
                temp.songs.append(track)

        for cat in g:
            if cat not in allGenres.keys():
                temp = Genre(str(cat))
                track.genres.append(temp)
                temp.songs.append(track)
            else:
                temp = allGenres[cat]
                track.genres.append(temp)
                temp.songs.append(track)

        if al not in allAlbums.keys():
            temp = Album(str(al))
            track.album = temp
            temp.songs.append(track)
            for musician in track.artists:
                musician.albums.append(temp)
                temp.artists.append(musician)
        else:
            temp = allAlbums[al]
            track.album = temp
            temp.songs.append(track)
            for musician in track.artists:
                musician.albums.append(temp)
                temp.artists.append(musician)

    except Exception as inst:
        try:
            f.writelines(str(type(inst)) + " error at " + mp3path[-20:] + "\n")
        except:
            print(str(type(inst)) + " error at " + mp3path[-10:])
