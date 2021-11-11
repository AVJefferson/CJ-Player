#save graph as .dat binary file for c program to read
#save image as acsii
#use a def image for unavailable image
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


file_error = open("out/error", "w")
# Create Library from input argumants
try:
    for musicPaths in sys.argv[1:]:
        for mp3 in getListOfFiles(musicPaths):
            if mp3.endswith("mp3"):
                MusicLibrary.createGraph(mp3, file_error)
except Exception as ex:
    file_error.write(str(ex) + " : Unable to Create Lib")

# Save Created Library
try:
    l_f = open("library/songs.list", "w")
    l_p = open("library/playlists/AllSongs.m3u", "w")
    l_p.write("#EXTM3U\n")
    for track in MusicLibrary.allSongs:
        l_f.write(str(track.id) + "\n")
        l_f.write(str(track.loc) + "\n")
        l_p.write(str(track.loc) + "\n")
    l_f.close()
    l_p.close()

    l_f = open("library/artists.list", "w")
    for musician in MusicLibrary.allArtists.values():
        l_f.write(str(musician.id) + "\t" + musician.name + "\n")
    l_f.close()

    l_f = open("library/albums.list", "w")
    for alb in MusicLibrary.allAlbums.values():
        l_f.write(str(alb.id) + "\t" + alb.name + "\n")
    l_f.close()

    l_f = open("library/genres.list", "w")
    for category in MusicLibrary.allGenres.values():
        l_f.write(str(category.id) + "\t" + category.name + "\n")
    l_f.close()
except Exception as ex:
    file_error.write(str(ex) + " : Unable to Write Lib")


file_error.close()
l_f.close()
l_p.close()
