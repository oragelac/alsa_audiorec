CC=gcc
CFLAGS=-Wall -O2
LDLIBS=-lasound -lusb-1.0 -lpthread -lm libbrams/src/.libs/libbrams.a libbrams/sndfile/src/.libs/libsndfile.a
EXEC=alsa_audiorec

all: $(EXEC)

$(EXEC): alsa.o audiodevice.o utils.o arguments.o data.o timestamps.o writefilethread.o ringbuffer.o timethread.o hid-libusb.o fcd.o main.o
	$(CC) -o $@ $^ $(LDLIBS)
	
main.o: main.c 
	$(CC) -o $@ -c $< $(CFLAGS) -I libbrams/src

alsa.o: alsa.c alsa.h audiodevice.h
	$(CC) -o $@ -c $< $(CFLAGS) -I libbrams/src
	
audiodevice.o: audiodevice.c audiodevice.h
	$(CC) -o $@ -c $< $(CFLAGS)
	
utils.o: utils.c utils.h
	$(CC) -o $@ -c $< $(CFLAGS) -I libbrams/src
	
arguments.o: arguments.c arguments.h
	$(CC) -o $@ -c $< $(CFLAGS)

data.o: data.c data.h
	$(CC) -o $@ -c $< $(CFLAGS)

timestamps.o: timestamps.c timestamps.h
	$(CC) -o $@ -c $< $(CFLAGS) -I libbrams/src
	
writefilethread.o: writefilethread.c writefilethread.h
	$(CC) -o $@ -c $< $(CFLAGS) -I libbrams/src

ringbuffer.o: ringbuffer.c ringbuffer.h
	$(CC) -o $@ -c $< $(CFLAGS)
	
timethread.o: timethread.c timethread.h
	$(CC) -o $@ -c $< $(CFLAGS) -I libbrams/src
	
hid-libusb.o: hid-libusb.c
	$(CC) -o $@ -c $< $(CFLAGS) -I/usr/include/libusb-1.0
	
fcd.o: fcd.c fcd.h 
	$(CC) -o $@ -c $< $(CFLAGS) -DFCDPP
	
libbrams/src/.libs/libbrams.a:
	cd libbrams && ./configure
	make -C libbrams
	
clean: 
	rm -rf ./*.o
	
mrproper: clean
	rm -rf $(EXEC)


