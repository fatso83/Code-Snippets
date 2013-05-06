#include <string>

using namespace std;

int usb_vendor_message_out(int device_handle, int receipent, int request_no, int request_value, unsigned char* buffer, int buffer_length, int timeout = 5000);
int usb_vendor_message_in(int device_handle, int receipent, int request_no, int request_value, unsigned char* buffer, int buffer_length, int timeout = 5000);
int usb_bulk_read(int device_handle, int interface, int endpoint, unsigned char* buffer, int length, int timeout = 5000);
int usb_bulk_write(int device_handle, int interface, int endpoint, unsigned char* buffer, int length, int timeout = 5000);
int wmp_usb_open(int vendor_id, int device_id, int enum_device);
int usb_device_set_configuration(int device_handle, int configuration_number);
