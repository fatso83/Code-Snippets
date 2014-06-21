testPrint:  testPrint.o ../main.h ../printing.o ../check.o
	gcc ../printing.o ../check.o testPrint.o -o testPrint

testColl:  testColl.o ../main.h ../printing.o ../check.o
	gcc ../printing.o ../check.o testColl.o -o testColl

testPrint.o: testPrint.c
	gcc -c testPrint.c

testColl.o: testColl.c
	gcc -c testColl.c

printing.o: printing.c
	gcc -c printing.c

check.o: check.c
	gcc -c check.c
