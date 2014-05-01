BatteryMon: battery.o main.o
	gcc -o BatteryMon battery.o main.o

TestBattery: battery.o test.c
	gcc --std=c99 -o TestBattery battery.o test.c;
main.o:	main.c battery.h
	gcc -c main.c
battery.o: battery.c battery.h
	gcc -c battery.c
test: TestBattery
	./TestBattery
clean:
	rm *.o TestBattery BatteryMon
clean_o:
	rm *.o

#	ls *.o > /dev/null && rm *.o 2>/dev/null; \
#	[ -x TestBattery ] && rm TestBattery;\
#	[ -x BatteryMon ] && rm BatteryMon;
