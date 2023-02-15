CC=gcc
CFLAGS=-lncurses -g #-O2

goodcal: goodcal.o utils.o appt.o
	$(CC) $(CFLAGS) $^ -o $@

goodcal.o: goodcal.c
	$(CC) $(CFLAGS) -c $^

utils.o: utils.c
	$(CC) $(CFLAGS) -c $^

appt.o: appt.c
	$(CC) $(CFLAGS) -c $^

clean:
	rm *.o goodcal

install: goodcal
	cp goodcal /usr/bin/
