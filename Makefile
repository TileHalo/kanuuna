include config.mk

CC=avr-gcc
OBJCOPY=avr-objcopy
OBJS=util.o 

all: build
build:
	arduino-cli compile -b $(BOARD) kanuuna

upload: build
	arduino-cli upload -p  $(SPORT) -b $(BOARD) kanuuna

test:
	slcand -o -c -F -s6 $(SPORT)

monitor:
	picocom --imap spchex -b 9600 $(SPORT)

.PHONY: build upload test
