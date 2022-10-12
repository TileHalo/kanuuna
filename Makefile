include config.mk

CC=avr-gcc
OBJCOPY=avr-objcopy
OBJS=util.o 

all: build
build:
	arduino-cli compile -b $(BOARD) kanuuna

upload: build
	arduino-cli upload -p  $(PORT) -b $(BOARD) kanuuna

test:
	slcand -o -c -F -s6 $(PORT)

monitor:
	picocom -c --imap spchex,crcrlf --omap lfcr -b 9600 $(PORT)

.PHONY: build upload test
