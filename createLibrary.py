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



file_error.close()
