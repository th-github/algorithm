

# -------------------------------------
def RotateA(alist, n):   
    length = n - 1
    m = n / 2

    for i in (range(len(alist)/2)):
        temp = alist[length]
        alist[length] = alist[i]
        alist[i] = temp
        length -= 1
    
# -------------------------------------
# the total of all elements must be the exact difference than the sum of 1 ... n
#     so the sum of [1:n] subtract the total renders the difference
def  FindMissing(alist, n):  # from the array having an empty spot   
    # assume positive values in array
    missingNum = -255
    total = 0

    sum = (n * (n+1)) / 2    # add all numbers in the range of size

    for i in (range(len(alist))):
        total += alist[i]

    missingNum = sum - total

    return missingNum

# -------------------------------------
def  FindRepeated(mylist3, dup):
   # assum value in the array is >= 1 and <= array size
    count = 0
    index = 0
    n = 0;  # count for element value == 0
    for i in (range(len(mylist3))):   
        if (mylist3[i] == 0):  # handle special case       
            n += 1
            continue
        
        # borrow a position of the array. the element value is assumed >= 1
        # The position created would be 0 minimum thus subtract 1 so the first
        # index would be 0
        index = abs(mylist3[i])  # need to make positive, see negation below
        if (mylist3[index] < 0):   # check if the content has been negated
           # yes, it is, save this duplicate to the output array
            dup.append(abs(mylist3[i]))    
            count += 1
        
        else:
            mylist3[index] = -mylist3[index]   # negate the content
    
    # don't forget the zeros if found:
    if (n > 1): # the pointer was incremeneted when found duplicates zeros 
        dup.append(0)   
        count += 1
    
    return count


# block for unit test:
if __name__ == "__main__":

    print "__ data structure and alorithm practices __\r\n"
       
    mylist1 = [12, 8, 28, 9, 25, 0]
    print "original mylist1:"
    print mylist1        
    RotateA(mylist1, 6)
    print " rotated mylist1:"
    print mylist1
    print "\n------------------------"

    mylist2 = [8, 3, 2, 9, 1, 7, 4, 6, 10]
    print "find a missing number in mylist2:"
    print mylist2  
    missingNum = FindMissing(mylist2, 10)
    print (" missing number: %s" % missingNum + '\n')
    print "------------------------"

    dup = list()
    mylist3 = [7, 19, 8, 0, 12, 15, 3, 8, 12, 5,
               5, 11, 2, 9,  6,  9, 17,13, 4, 0]
    print "find a duplicate number in mylist3:"       
    print mylist3
    count = FindRepeated(mylist3, dup)
    print (" found number of duplicated: %s" % count);
    print dup
    print "\r\n"











        

                        
                        

