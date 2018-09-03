#include <SerialStream.h>
#include <iostream>
#include <string.h>

SerialStream serial_port;

void print_status_byte_register()
{//Print status byte register to console
	serial_port << "*STB?\r";//Query register
	const int BUFFER_SIZE = 256 ;
	char input_buffer[BUFFER_SIZE] ;
	memset(input_buffer, 0, BUFFER_SIZE);
	serial_port.read( input_buffer, BUFFER_SIZE ) ;
	std::cout << "Status Byte Register:  " << input_buffer << std::endl;
	//Convert string to number
	unsigned char status_byte;
	scanf(input_buffer, "%hhu", &status_byte);
	//Mask MAV bit
	const char MAV_MASK = 0x10;
	std::cout << "Message Available:  ";
	if(MAV_MASK & status_byte)
		std::cout << "Yes" << std::endl;
	else
		std::cout << "No" << std::endl;
	//Mask Event Status bit
	const char EVENT_STATUS_MASK = 0x20;
	std::cout << "Active Event:  "
	if(EVENT_STATUS_MASK & status_byte)
		std::cout << "Yes" << std::endl;
	else
		std::cout << "No" << std::endl;
	//Mask Master Summary Status bit
	const char MASTER_SUMMARY_STATUS_MASK = 0x40;
		std::cout << "Master Summary Status:  "
	if(EVENT_STATUS_MASK & status_byte)
		std::cout << "Yes" << std::endl;
	else
		std::cout << "No" << std::endl;

}


int main()
{
	const char* const SERIAL_PORT_DEVICE = "/dev/ttyUSB0";
using namespace LibSerial;
serial_port.Open(SERIAL_PORT_DEVICE);
if(!serial_port.good())
{
	std::cout << "Unable to open serial port." << std::endl;
}
else
	std::cout << "Serial port open!" << std::endl;
	serial_port.SetBaudRate(SerialStreamBuf::BAUD_9600);
	serial_port.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
	serial_port.SetNumOfStopBits(1);
	serial_port.SetParity(SerialStreamBuf::PARITY_NONE);



std::cout << "Press <ENTER> to exit." << std::endl;
getchar();
serial_port.Close();
return 0;
}
