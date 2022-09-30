include config.mk


all: build
build:
	arduino-cli compile -b $(BOARD) canuino

upload: build
	arduino-cli upload -p  $(SPORT) -b $(BOARD) canuino

.PHONY: build
