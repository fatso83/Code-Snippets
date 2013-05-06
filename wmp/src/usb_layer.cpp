#include <usb.h>
#include <vector>
#include "usb_layer.h"

using namespace std;

vector<usb_dev_handle*> handles;

int usb_vendor_message_out(int device_handle, int receipent, int request_no, int request_value, unsigned char* buffer, int buffer_length, int timeout) {
	receipent &= 0x1F;
	
	int ret = usb_control_msg(
		handles[device_handle],
		USB_ENDPOINT_OUT | USB_TYPE_VENDOR | receipent, 
		request_no, 
		request_value, 
		0, 
		(char*)buffer, 
		buffer_length, 
		timeout
	);
	if(ret < 0) {
		perror("usb_vendor_message_out()");
		printf("return value: %d\n", ret);
	}
	return ret;
}

int usb_vendor_message_in(int device_handle, int receipent, int request_no, int request_value, unsigned char* buffer, int buffer_length, int timeout) {
	receipent &= 0x1F;

	int ret = usb_control_msg(
		handles[device_handle],
		USB_ENDPOINT_IN | USB_TYPE_VENDOR | receipent, 
		request_no, 
		request_value, 
		0, 
		(char*)buffer,
		buffer_length, 
		timeout
	);

	if(ret < 0) {
		perror("usb_vendor_message_in()");
		printf("return value: %d\n", ret);
	}
	return ret;
}


int usb_bulk_read(int device_handle, int interface, int endpoint, unsigned char* buffer, int length, int timeout) {
	return usb_bulk_read(handles[device_handle], endpoint, /* todo interface,*/ (char*)buffer, length, timeout);
}

int usb_bulk_write(int device_handle, int interface, int endpoint, unsigned char* buffer, int length, int timeout) {
	return usb_bulk_write(handles[device_handle], endpoint, /*todo interface,*/ (char*)buffer, length, timeout);
}

int wmp_usb_open(int vendor_id, int device_id, int enum_device) {
	usb_init();

	usb_find_busses();
    	usb_find_devices();
    
	struct usb_bus *busses = usb_get_busses();

	// find device
    	struct usb_bus *bus;
    	int c, i, a;
    
    	for (bus = busses; bus; bus = bus->next) {
    		struct usb_device *dev;
    
    		for (dev = bus->devices; dev; dev = dev->next) {
    			/* Check if this device is a printer */
    			if (dev->descriptor.idProduct == device_id && dev->descriptor.idVendor == vendor_id) {
				usb_dev_handle* h = usb_open(dev); 
				if(!h) {
					perror("could not get device handle\n");
					return -1;
				}
				handles.push_back(h);
				if(usb_claim_interface(h, 0) < 0) {
					perror("could not claim interface 0\n");
					return -1;
				}
				
				return handles.size() - 1;
    			}
    
    		}
    	}
	return -1;
}

int usb_device_set_configuration(int device_handle, int configuration_number) {
	int last_rc = usb_set_configuration(handles[device_handle], configuration_number);
	
	if (!last_rc)
		usleep(200000);
	
	return last_rc;
}
