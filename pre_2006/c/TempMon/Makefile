TempMon: main.o temperature.o alloc.o
	gcc -o TempMon main.o temperature.o alloc.o

#TestBattery: battery.o test.c
#	gcc --std=c99 -o TestBattery battery.o test.c;
alloc.o: ../alloc/alloc.c
	gcc -c ../alloc/alloc.c

main.o:	main.c 
	gcc --std=c99 -c main.c
temperature.o: temperature.c temperature.h
	gcc -c temperature.c
#test: TestBattery
#	./TestBattery
clean:
	rm *.o 
clean_o:
	rm *.o

#	ls *.o > /dev/null && rm *.o 2>/dev/null; \
#	[ -x TestBattery ] && rm TestBattery;\
#	[ -x BatteryMon ] && rm BatteryMon;
