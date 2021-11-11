import eyed3

allSongs = []
allArtists = {}
allAlbums = {}
allGenres = {}

nextId = 0


class Node:
    def __init__(self):
        global nextId
        self.id = nextId
        nextId += 1


class Artist(Node):
    def __init__(self, name: str):
        Node.__init__(self)
        global allArtists
        allArtists[name] = self
        self.name = name
        self.songs = []
        self.albums = []


class Album(Node):
    def __init__(self, name: str):
        Node.__init__(self)
        global allAlbums
        allAlbums[name] = self
        self.name = name
        self.songs = []
        self.artists = []


class Genre(Node):
    def __init__(self, name: str):
        Node.__init__(self)
        global allGenres
        allGenres[name] = self
        self.name = name
        self.songs = []


class Song(Node):
    def __init__(self, location, title, lyrics, artist, year, count=0):
        Node.__init__(self)
        global allSongs
        allSongs.append(self)
        self.loc = location
        self.title = title
        self.lyrics = lyrics
        self.artist = artist
        self.year = year
        self.count = count
        self.album = None
        self.artists = []
        self.genres = []


def createGraph(mp3path: str, f):
    global allAlbums, allArtists, allSongs, allGenres
    mp3file = eyed3.load(mp3path)

    try:
        t = str(mp3file.tag.title)
        if t == "None" or t == "":
            t = "Unknown"

        al = str(mp3file.tag.album)
        if al == "None" or al == "":
            "Unknown"

        try:
            y = int(str(mp3file.tag.recording_date))
        except:
            y = 0

        try:
            l = mp3file.tag.lyrics[0].text
            if l == "":
                l = "Embedded Lyrics Not Found"
        except:
            l = "Embedded Lyrics Not Found"

        try:
            g = str((mp3file.tag.genre.name)).split(' | ')
            if g == "":
                g = ['Unknown']
        except:
            g = ['Unknown']

        ar = str(mp3file.tag.artist)
        art = []
        if ar == "None" or ar == "":
            art = ["Unknown"]
        else:
            art = ar.replace(' Ft.', ',').replace(' &', ',').split(', ')

        # All data Availabe for graph creation - song, artists, genres, albums
        track = Song(mp3path, t, l, ar, y)

        # Create and link and/or add artists, genres and album
        for musician in art:
            if musician in allArtists:
                temp = allArtists[musician]
                track.artists.append(temp)
                temp.songs.append(track)
            else:
                temp = Artist(musician)
                track.artists.append(temp)
                temp.songs.append(track)

        for cat in g:
            if cat in allGenres:
                temp = allGenres[cat]
                track.genres.append(temp)
                temp.songs.append(track)
            else:
                temp = Genre(cat)
                track.genres.append(temp)
                temp.songs.append(track)

        if al in allAlbums:
            temp = allAlbums[al]
            track.album = temp
            temp.songs.append(track)
            for musician in track.artists:
                musician.albums.append(temp)
                temp.artists.append(musician)
        else:
            temp = Album(al)
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
