# create graph
#   save graph as .dat binary file for c program to read
# save image as acsii
#   use a def image for unavailable image
import sys
import os

import MusicLibrary


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


file_error = open("out.error", "w")
# Create Library from input argumants
for musicPaths in sys.argv[1:]:
    for mp3 in getListOfFiles(musicPaths):
        if mp3.endswith("mp3"):
            MusicLibrary.addSongToLibrary(mp3, file_error)
file_error.close()

with open("out.songs", "w") as f:
    for x in MusicLibrary.allSongs.values():
        f.write(str(x.title) + "\n")

with open("out.artists", "w") as f:
    for x in MusicLibrary.allArtists.values():
        f.write(str(x.name) + "\n")

with open("out.albums", "w") as f:
    for x in MusicLibrary.allAlbums.values():
        f.write(str(x.name) + "\n")

with open("out.genres", "w") as f:
    for x in MusicLibrary.allGenres.values():
        f.write(str(x.name) + "\n")
