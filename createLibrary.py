'''

  read mp3 tags
    create graph
      save graph as .dat binary file for c program to read
    save image as acsii
      use a def image for unavailable image
'''

import eyed3
import sys
import os

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

    def __init__(name: str):
        Node.__init__()
        self.name = name
        allArtists[self] = name


class Album(Node):

    name = "Unknown"
    songs = []
    artists = []
    genres = []

    def __init__(name: str):
        Node.__init__()
        self.name = name
        allAlbums[self] = name


class Genre(Node):

    name = "Unknown"
    songs = []
    albums = []
    artists = []

    def __init__(name):
        Node.__init__()
        self.name = name
        allGenres[self] = name


class Song(Node):
    title = "Unknown"
    lyrics = "No Lyrics"
    loc = "/"
    artist = "Unknown"
    year = 0
    count = 0
    album = "Unknown"
    artists = []
    genres = []

    def __init__(self, title, location):
        Node.__init__(self)
        global allSongs
        allSongs[self] = location
        self.loc = location


f = open("error.out", "w")


def addSongToLibrary(mp3path: str):
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
        if ar == "None":
            ar = ["Unknown"]
        else:
            ar = ar.replace(' Ft.', ',').replace(' &', ',').split(', ')

        # No More Errors, All data Availabe for graph creation

    except:
        f.write(mp3path)
        f.write("\n")


def getListOfFiles(dirName):
    listOfFile = os.listdir(dirName)
    allFiles = list()
    for entry in listOfFile:
        fullPath = os.path.join(dirName, entry)
        if os.path.isdir(fullPath):
            allFiles = allFiles + getListOfFiles(fullPath)
        else:
            allFiles.append(fullPath)
    return allFiles


# Create Library from input argumants
for musicPaths in sys.argv[1:]:
    for mp3 in getListOfFiles(musicPaths):
        if mp3.endswith("mp3"):
            addSongToLibrary(mp3)

f.close()
