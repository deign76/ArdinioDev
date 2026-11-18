TARGET := /dev/ttyACM0
DEVICE := renesas_uno:unor4wifi
FILE := t.ino


all: build

build:
	arduino-cli compile --fqbn arduino:$(DEVICE) $(FILE)

deploy: build
	arduino-cli upload -p $(TARGET) --fqbn arduino:$(DEVICE) $(FILE)

