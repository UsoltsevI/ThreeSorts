# Main pionts
* [Description](#description)
* [Using](#using)
* [Results and comparison](#results-and-comparison)
* [Links](#links)

# Decription
## About program
This project includes three sort alghoritms: heapsort, introsort and patter-defeating quicksort (*pdqsort*). Also there is a insertion sort realization for introsort and pdqosrt. The main purpose is to compare these three alghoritms. The swap function is implemented using memcpy from <string.h>, but the memory for the element through which the exchange is carried out is allocated only once and freed also once, and then the pointer to the allocated memory block is passed to the swap function. Also, insertion sort provides the ability to pass it a pointer to a dedicated element for storing the key, because insertion sort can be run a very large number of times inside another sort, and allocating memory each time and freeing it is a very long process. 

## Files
Each sort lies in its own folder, but some sorts such as timsort, introsort and patdefqsort use other sorts from other folders. When using make, all sorts are combined into a single file along with test.c. This file was created for testing and comparing all sorts. There is also a debug.c file in the folder, which is required for debugging and checking the functionality of the program at each step. In the release version, it is cleaned everywhere. In General generates tests. For more information, see the usage section. The file ResAll.txt - these are the results of testing all sorts (except insertion sort, because it is too slow) on my work computer.

All the tests I have generated are in the Tests and yf folder. Each of these tests has answers in the same folder with names *ans.txt.

# Using
All work with the program can be done from the ThreeSorts folder. To start working with the program, run make, it will compile all the necessary files.

You can use each of the suggested sorts separately, or you can make static or dynamic libraries out of them using the command `make lib<sort prefix><dyn/st>`.Sort prefix means heap, ins, intro, pdq or tim, dyn = dynamic, st = static. For example `make libheapdyn` compiles libheapsort.so in HeapSort folder. Next, you can copy the header and .so file to the desired folder and use them if necessary.

The test.c file was created in order to test all these sorts and compare. A brief description of its use is in the briefdescription.txt and examples are in the ems.txt. You do not need to write long names of these files every time to view the contents. The contents of the briefdescription.txt file are output to the terminal each time the `make usage` command is run, and contents of the ems.txt are output to the terminal each time `make ems` command is run. From the main one, the program from the test.c file takes the amount of data from the standard input, then the data itself and sorts them in ascending order as int numbers. You can also tell the program the file with the answers, and it will output OK if everything matches, or it will output cell numbers where there are discrepancies.

Input data format: <num of elem> <elem itself>. For the response verification data, the output format is the same.

# Results and comparison
## Insertion sort
The slowest sort is obviously the insertion sort The slowest sort is obviously insertion sort, because it has O(n<sup>2</sup>) asymptotics. But it should be noted that on small arrays, insertion sort is the fastest and most efficient sorting. This is the reason why it is used in other algorithms when the amount of data falls below a certain threshold. TimSort, IntroSort, PatDefQsort apply it in their implementation. 

## Heapsort
HeapSort is the most memory-efficient, and at the same time has the asymptotics O(n logn). It is much faster than insertion sort and also does not require additional memory, except for allocating space for a variable for the swap function. But heapsort loses to qsort in time.

## Introsort
The IntroSort algorithm combines three sorts at once: inssort, heapsort and qsort. On small data (when the length of the array becomes less than a certain threshold), inssort is activated. Heapsort starts working when the recursion depth of qsort becomes greater than logn. This helps the inssort algorithm to preserve the asymptotics of O(n logn).

## Pattern-defeating quick sort
Pattern-defeating quicksort works effectively on data where there are many identical elements. For example, on the 14th test, where there are 32,000 data and always 5 accepted values, it outperforms qsort by about 20%. But, in contrast, on tests 9 and 10, where there is almost no duplicate data, pdqsort runs 2 times slower than qsort. This is due to the fact that there are quite a large number of additional checks.

## Timsort
Perhaps the most effective sorting from the presented list. It uses inssort on small data and merge sort on large data. There is also a balancing algorithm added, which significantly increases the efficiency of merge sort. It is stable, thus having an advantage over the rest. It also works by the asymptotics of O(n log n) in the worst case, and O(n) in the best.


Of all the proposed sorts, only Inssort and TimSort are stable. The stability of sorting allows you not to swap the same data, thereby performing fewer actions. This can also result when sorting a set by a specific element, when you need to preserve the previous order of sets matching a given element. So if you need it, I advise you to use TimSort. Of the well-known mergesort, it is also a stable algorithm, and it is used in the implementation of TimSort.

## What is the most universal sorting?
In conclusion, I want to say that in my opinion the most convenient and effective sorting for most cases is TimSort.  It also loses time to the usual qsort on completely random data, but random data is very rare in life, because of this TimSort becomes more effective more often. In addition, under no circumstances will it work according to the asymptotics of O(n<sup>2</sup>). For the same reason, this is the main sorting in Python and Java. 
I should note that this TimSort implementation is on average less efficient than std::sort (asort) from C++. This is justified by the fact that the C++ language allows you to use templates, and thanks to this, the comparison function spends significantly less time. I think that if we rewrite this algorithm in C++ using its advantages, TimSort will become more efficient than std::sort. I will have this in the further elaboration of the task, since I have not yet learned the C++ language.

# Links
- [Insertion sort](https://en.wikipedia.org/wiki/Insertion_sort)
- [Heapsort](https://en.wikipedia.org/wiki/Heapsort)
- [Introsort](https://en.wikipedia.org/wiki/Introsort)
- [Pattern-defeating quick sort](https://arxiv.org/pdf/2106.05123v1.pdf)
- [TimSort](https://en.wikipedia.org/wiki/Timsort)

### Postscript
If you find a bug or have ideas to improve this program, please start a discussion. 
