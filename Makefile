TARGET := /dev/ttyACM0
DEVICE := renesas_uno:unor4wifi
FILE := t.ino


all: build

build:
	arduino-cli compile --fqbn arduino:$(DEVICE) $(FILE)

deploy: build
	arduino-cli upload -p $(TARGET) --fqbn arduino:$(DEVICE) $(FILE)

deploy-serial: deploy
	arduino-cli monitor -p /dev/ttyACM0 --config baudrate=1200