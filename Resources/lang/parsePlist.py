#!/usr/bin/python3

import argparse


def parsePlist(inputFile, outputFile):

    inFile = open(inputFile, 'r')
    outFile = open(outputFile, 'w')

    entry = False
    for line in inFile:
        # start with key
        if '<key>' in line:
            entry = True
            continue
        # if not entry active then skip line            
        if not entry:
            continue
        # init            
        entryStart = line
        if '<string>' in line:
            entryStart = entryStart.split('<string>')[1]
        # end
        if '</string>' in line:
            entry = False
            entryStart = entryStart.split('</string>')[0]+'\n'
        outFile.write(entryStart)


def main():
    parser = argparse.ArgumentParser()

    parser.add_argument("-i", "--inputFile", help="input plist")
    parser.add_argument("-o", "--outputFile", help="output txt")
    args = parser.parse_args()

    parsePlist(args.inputFile, args.outputFile)



if __name__ == "__main__":
    main()




