# convert.py
# Loop through a file of raw rgb data and create a new file 
# containing only the r values.
# Author: C.Wilson - abitofcode

import sys, struct

if len(sys.argv) != 2:  
  # the program name and the filename
  # stop the program and print an error message
  sys.exit("Must provide a filename")

# use the value we passed in as the source filename
infile = sys.argv[1]
# prepend '_' to create an output file name
outfile = '_' + infile
# open the output file for write binary (wb)
o = open(outfile, "wb")
#openf the input file for read binary (rb)
with open(infile, "rb") as f:
    # grab the first 3 bytes from the file	
    pixel = f.read(3)
    # loop through 3 bytes at a time until complete
    while pixel:
        # split the bytes into the individual color components
        r,g,b = struct.unpack("BBB", pixel)
        #print str(r) + ':' + str(g) + ':' + str(b)
        # pack the first byte (red) so it can be written
        p = struct.pack("B",r)
        # write the byte to the output file
        o.write(p)
        # read the next 3 bytes from the file
        pixel = f.read(3)
# close the source file
f.close()
# close the output file
o.close()
