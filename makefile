all: test.o HeapSort/heapsort.o InsSort/inssort.o IntroSort/introsort.o PatDefQSort/pdqsort.o
	gcc -O2 test.o HeapSort/heapsort.o InsSort/inssort.o IntroSort/introsort.o PatDefQSort/pdqsort.o -lm -o ST.x
	cat briefdescription.txt

ems:
	cat ems.txt

test.o: test.c 
	gcc -O2 test.c -c

clean:
	rm *.o *.x

cleanall:
	rm *.o *.x HeapSort/heapsort.o InsSort/inssort.o IntroSort/introsort.o PatDefQSort/pdqsort.o

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
	gcc -O2 -Wall -DNDEBUG PatDefQSort/pdqsort.c -c -o PatDefQSort/pdqsort.o

testall:
	for i in $$(seq 0 1 2); do echo Tests/Test$$i.txt: && ./ST.x all < Tests/Test$$i.txt && echo ''; done
	@echo 'file Tests/Test3.txt is too large for inssort :)'
	@echo ''
	for i in $$(seq 4 1 14); do echo Tests/Test$$i.txt: && ./ST.x all < Tests/Test$$i.txt && echo ''; done

testallans:
	for i in $$(seq 0 1 2); do echo Tests/Test$$i.txt: && ./ST.x all Tests/Test$${i}ans.txt < Tests/Test$$i.txt && echo ''; done
	@echo 'file Tests/Test3.txt is too large for inssort :)'
	@echo ''
	for i in $$(seq 4 1 14); do echo Tests/Test$$i.txt: && ./ST.x all Tests/Test$${i}ans.txt < Tests/Test$$i.txt && echo ''; done

test:
	for i in $$(seq 0 1 14); do echo Tests/Test$$i.txt: && ./ST.x $$sn Tests/Test$${i}ans.txt < Tests/Test$$i.txt && echo ''; done 

testo:
	@echo "Tests/Test$$i.txt:"
	./ST.x $$sn out Tests/Test$${nt}ans.txt < Tests/Test$${nt}.txt
