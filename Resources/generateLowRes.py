#!/usr/bin/python3

import os, PIL, shutil

from PIL import Image


def resizeImage( fileName, originalPath, pathToResizedImage, resolution):
    try:
        original = Image.open(originalPath+'/'+fileName)
    except IOError:
        print('file:',fileName,'is not an image')
        return

    width = int(float(original.size[0]*resolution))
    height = int(float(original.size[1])*resolution)
    print('resizing image %s from %d/%d to %d/%d' %(fileName, original.size[0], original.size[1], width, height))
    converted = original.resize((width, height), PIL.Image.ANTIALIAS)
    converted.save(pathToResizedImage+'/'+fileName)

def main():
    # base is resolution 2048x1536 (ipad air)
    listResolutions = {'02_threeQuarters':3/4, '03_fifthEights':5/8, '04_half':1/2}
    baseDir = '01_hd'


    for newDir,resolution in listResolutions.items():
        print('new dir:',newDir,'resolution:',resolution)
        if os.path.exists(newDir):
            print('\tremoving existing dir:',newDir)
            shutil.rmtree(newDir)

    for dirName, subdirList, fileList in os.walk(baseDir):
        for newDirRoot,resolution in listResolutions.items():
            newPath = dirName.replace(baseDir,newDirRoot)
            print('copy dir:',dirName,'to',newPath)
            os.makedirs(newPath)
            for fileName in fileList:
                resizeImage(fileName,dirName,newPath,resolution)

if __name__ == "__main__":
    main()

