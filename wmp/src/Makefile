all: check wmp_manager

check:
	-@which libusb-config > /dev/null ||  echo "*** Error: could not find libusb-config. you will need libusb >= 0.1.8-17 ***"

wmp_manager: casio_wmp.cpp usb_layer.cpp
	g++ -o wmp_manager casio_wmp.cpp usb_layer.cpp `libusb-config --libs --cflags`
