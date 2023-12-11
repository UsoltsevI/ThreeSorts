all: test.o HeapSort/heapsort.o InsSort/inssort.o IntroSort/introsort.o PatDefQSort/pdqsort.o TimSort/timsort.o
	gcc -O2 test.o HeapSort/heapsort.o InsSort/inssort.o IntroSort/introsort.o PatDefQSort/pdqsort.o TimSort/timsort.o -lm -o ST.x
	@echo 'To see the usage enter command "make usage"'
	
usage:
	cat briefdescription.txt

ems:
	cat ems.txt

testinfo:
	cat Tests/TestsInfo.txt

test.o: test.c 
	gcc -O2 test.c -c

clean:
	rm *.o *.x

cleanall:
	rm *.o *.x HeapSort/heapsort.o InsSort/inssort.o IntroSort/introsort.o PatDefQSort/pdqsort.o TimSort/timsort.o

cgen:
	gcc IntGener.c -o IG.x 
	@echo ''
	@echo 'Usage: ./IG.x <num_elem> <max_num>'
	@echo 'The generated sequence is output to stdout'
	@echo 'Usage example: ./IG.x 1000 32000 > Tests/Test60.txt'
	@echo ''

HeapSort/heapsort.o: HeapSort/heapsort.c 
	gcc -O2 -Wall -DNDEBUG HeapSort/heapsort.c -c -o HeapSort/heapsort.o

InsSort/inssort.o: InsSort/inssort.c 
	gcc -O2 -Wall InsSort/inssort.c -c -o InsSort/inssort.o

IntroSort/introsort.o: IntroSort/introsort.c
	gcc -O2 -Wall IntroSort/introsort.c -c -o IntroSort/introsort.o

PatDefQSort/pdqsort.o: PatDefQSort/pdqsort.c 
	gcc -O2 -Wall PatDefQSort/pdqsort.c -c -o PatDefQSort/pdqsort.o

TimSort/timsort.o: TimSort/timsort.c 
	gcc -O2 -Wall TimSort/timsort.c -c -o TimSort/timsort.o

SPLIT_LINES = "----------------------------------------------------------"

testall:
	for i in $$(seq 0 1 14); do echo Tests/Test$$i.txt: && ./ST.x all < Tests/Test$$i.txt && echo ${SPLIT_LINES}; done
	@echo 'ALREADY SORTED DATA:'
	for i in $$(seq 0 1 14); do echo Tests/Test$${i}ans.txt: && ./ST.x all < Tests/Test$${i}ans.txt && echo ${SPLIT_LINES}; done 

testallans:
	for i in $$(seq 0 1 14); do echo Tests/Test$$i.txt: && ./ST.x all Tests/Test$${i}ans.txt < Tests/Test$$i.txt && echo ${SPLIT_LINES}; done
	@echo 'ALREADY SORTED DATA:'
	for i in $$(seq 0 1 14); do echo Tests/Test$${i}ans.txt: && ./ST.x all Tests/Test$${i}ans.txt < Tests/Test$${i}ans.txt && echo ${SPLIT_LINES}; done 

test:
	for i in $$(seq 0 1 14); do echo -n "Tests/Test$$i.txt: | " && ./ST.x $$sn Tests/Test$${i}ans.txt < Tests/Test$$i.txt && echo ${SPLIT_LINES}; done 
	@echo 'ALREADY SORTED DATA:'
	for i in $$(seq 0 1 13); do echo -n "Tests/Test$${i}ans.txt: | " && ./ST.x $$sn Tests/Test$${i}ans.txt < Tests/Test$${i}ans.txt && echo ${SPLIT_LINES}; done 

testo:
	@echo "Tests/Test$$i.txt:"
	./ST.x $$sn out Tests/Test$${nt}ans.txt < Tests/Test$${nt}.txt


libheapdyn: 
	gcc -O2 -fPIC HeapSort/heapsort.c -c -o HeapSort/heapsort.o 
	gcc -shared HeapSort/heapsort.o -o HeapSort/libheapsort.so

libinsdyn:
	gcc -O2 -fPIC InsSort/inssort.c -c -o InsSort/inssort.o 
	gcc -shared InsSort/inssort.o -o InsSort/libinssort.so 

libintrodyn:
	gcc -O2 -fPIC InsSort/inssort.c -c -o InsSort/inssort.o 
	gcc -O2 -fPIC HeapSort/heapsort.c -c -o HeapSort/heapsort.o 
	gcc -O2 -fPIC IntroSort/introsort.c -c -o IntroSort/introsort.o 
	gcc -shared InsSort/inssort.o HeapSort/heapsort.o IntroSort/introsort.o -o IntroSort/libintrosort.so 

libpdqdyn:
	gcc -O2 -fPIC InsSort/inssort.c -c -o InsSort/inssort.o 
	gcc -O2 -fPIC PatDefQSort/pdqsort.c -c -o PatDefQSort/pdqsort.o
	gcc -shared InsSort/inssort.o PatDefQSort/pdqsort.o -o PatDefQSort/libpdqsort.so 

libtimdyn:
	gcc -O2 -fPIC InsSort/inssort.c -c -o InsSort/inssort.o 
	gcc -O2 -fPIC TimSort/timsort.c -c -o TimSort/timsort.o 
	gcc -shared InsSort/inssort.o TimSort/timsort.o -o TimSort/libtimsort.so 


libheapst: HeapSort/heapsort.o 
	ar -cr HeapSort/libheapsort.a HeapSort/heapsort.o

libinsst: InsSort/inssort.o 
	ar -cr InsSort/libinssort.a InsSort/inssort.o 

libintrost: InsSort/inssort.o HeapSort/heapsort.o IntroSort/introsort.o 
	ar -cr IntroSort/libintrosort.a InsSort/inssort.o HeapSort/heapsort.o IntroSort/introsort.o

libpdqst: InsSort/inssort.o PatDefQSort/pdqsort.o
	ar -cr PatDefQSort/libpdqsort.a InsSort/inssort.o PatDefQSort/pdqsort.o

libtimst: InsSort/inssort.o TimSort/timsort.o
	ar -cr TimSort/libtimsort.a InsSort/inssort.o TimSort/timsort.o
	