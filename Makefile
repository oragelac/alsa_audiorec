CC=gcc
CFLAGS=-Wall -O2
LDLIBS=-lasound -lusb-1.0 -lpthread -lm
EXEC=record

all: $(EXEC)

record: record.o alsa.o audiodevice.o utils.o arguments.o data.o timestamps.o hid-libusb.o fcd.o
	$(CC) -o $@ $^ $(LDLIBS)
	
record.o: record.c
	$(CC) -o $@ -c $< $(CFLAGS)

alsa.o: alsa.c alsa.h audiodevice.h
	$(CC) -o $@ -c $< $(CFLAGS)
	
audiodevice.o: audiodevice.c audiodevice.h
	$(CC) -o $@ -c $< $(CFLAGS)
	
utils.o: utils.c utils.h
	$(CC) -o $@ -c $< $(CFLAGS)
	
arguments.o: arguments.c arguments.h
	$(CC) -o $@ -c $< $(CFLAGS)

data.o: data.c data.h
	$(CC) -o $@ -c $< $(CFLAGS)

timestamps.o: timestamps.c timestamps.h
	$(CC) -o $@ -c $< $(CFLAGS)
	
hid-libusb.o: hid-libusb.c
	$(CC) -o $@ -c $< $(CFLAGS) -I/usr/include/libusb-1.0
	
fcd.o: fcd.c fcd.h 
	$(CC) -o $@ -c $< $(CFLAGS) -DFCDPP
	
clean: 
	rm -rf ./*.o
	
mrproper: clean
	rm -rf $(EXEC)


