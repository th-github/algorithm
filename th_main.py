#! /usr/bin/env python

# do: pyinstaller <myfile>.py to create a standalone executable file 
# the executable resides at ./dist/myfile/
# next chmod +x the executable then run: ./myfile

import th_algo_mod

if __name__ == "__main__":
    print "------------------------"

    mylist1 = [12, 8, 28, 9, 25, 0]
    print "original mylist1:"
    print mylist1        
    th_algo_mod.RotateA(mylist1, 6)
    print " rotated mylist1:"
    print mylist1
    print "\n------------------------"
        
    mylist2 = [8, 3, 2, 9, 1, 7, 4, 6, 10]
    print "find a missing number in mylist2:"
    print mylist2
    missingNum = th_algo_mod.FindMissing(mylist2, 10)
    print (" missing number: %s" % missingNum + '\n')
    print "------------------------"

    dup = list()
    mylist3 = [7, 19, 8, 0, 12, 15, 3, 8, 12, 5,
               5, 11, 2, 9,  6,  9, 17,13, 4, 0]
              
    print "find a duplicate number in mylist3:"       
    print mylist3
    count = th_algo_mod.FindRepeated(mylist3, dup)
    print (" found number of duplicated: %s" % count);
    print dup
    
    print "\r\n"


