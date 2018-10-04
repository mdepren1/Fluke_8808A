#include "Fluke_8808A.hpp"
using namespace Fluke_8808A_namespace;


string delete_all_occurances_of_characters(string input_str, string chars_to_delete)
{
	size_t loc = input_str.find_first_of(chars_to_delete);
	while(loc != string::npos)
	{
		input_str.erase(loc, 1);
		loc = input_str.find_first_of(chars_to_delete);
	}
	return input_str;
}


Fluke_8808A::Fluke_8808A(){};
bool Fluke_8808A::connect(string com_port_str)
{//Open serial coms to Fluke instrument
	open_port(com_port_str);
	if(valid_connection)
		initialize_port_settings(com_port);
	return valid_connection;
}
void Fluke_8808A::disconnect()
{
	if(valid_connection)
	{
		close(com_port);
		valid_connection = false;
	}
}

void Fluke_8808A::set_measurement_mode(Display::Num disp_num, MeasurementModes::Mode mode)
{//Set Fluke measurement mode
	switch(disp_num)
	{
		case(Display::PRIMARY):
			switch(mode)
			{
				case(MeasurementModes::AAC):
					send_command_and_receive_string("AAC",com_port);break;
				case(MeasurementModes::AACDC):
					send_command_and_receive_string("AACDC",com_port); break;
				case(MeasurementModes::ADC):
					send_command_and_receive_string("ADC",com_port); break;
				case(MeasurementModes::CONT):
					send_command_and_receive_string("CONT",com_port); break;
				case(MeasurementModes::DIODE):
					send_command_and_receive_string("DIODE",com_port); break;
				case(MeasurementModes::FREQ):
					send_command_and_receive_string("FREQ",com_port); break;
				case(MeasurementModes::OHMS):
					send_command_and_receive_string("OHMS",com_port); break;
				case(MeasurementModes::WIRE2):
					send_command_and_receive_string("WIRE2",com_port); break;
				case(MeasurementModes::WIRE4):
					send_command_and_receive_string("WIRE4",com_port); break;
				case(MeasurementModes::VAC):
					send_command_and_receive_string("VAC",com_port); break;
				case(MeasurementModes::VACDC):
					send_command_and_receive_string("VACDC",com_port); break;
				case(MeasurementModes::VDC):
					send_command_and_receive_string("VDC",com_port); break;
			}
		break;
		case(Display::SECONDARY):
			switch(mode)
			{
				case(MeasurementModes::AAC):
					send_command_and_receive_string("AAC2",com_port); break;
				case(MeasurementModes::AACDC):
					break;
				case(MeasurementModes::ADC):
					send_command_and_receive_string("ADC2",com_port); break;
				case(MeasurementModes::CONT):
					break;
				case(MeasurementModes::DIODE):
					break;
				case(MeasurementModes::FREQ):
					send_command_and_receive_string("FREQ2",com_port); break;
				case(MeasurementModes::OHMS):
					send_command_and_receive_string("OHMS2",com_port); break;
				case(MeasurementModes::WIRE2):
					break;
				case(MeasurementModes::WIRE4):
					break;
				case(MeasurementModes::VAC):
					send_command_and_receive_string("VAC2",com_port); break;
				case(MeasurementModes::VACDC):
					break;
				case(MeasurementModes::VDC):
					send_command_and_receive_string("VDC2",com_port); break;
				}
		break;
		default:
			break;
	}
}
MeasurementModes::Mode Fluke_8808A::get_measurement_mode(Display::Num disp_num)
{
	string meas_mode_request;
	switch(disp_num)
	{
		case(Display::PRIMARY):
			meas_mode_request = "FUNC1?";
			break;
		case(Display::SECONDARY):
			meas_mode_request = "FUNC2?";
			break;
		default:
			meas_mode_request = "FUNC1?";
	}

	string meas_mode = send_command_and_receive_string(meas_mode_request,com_port);
	meas_mode = delete_all_occurances_of_characters(meas_mode, "\r\n=>");
	if(meas_mode.compare("AAC") == 0)
		return MeasurementModes::AAC;
	if(meas_mode.compare("AACDC") == 0)
		return MeasurementModes::AACDC;
	if(meas_mode.compare("ADC") == 0)
		return MeasurementModes::ADC;
	if(meas_mode.compare("CONT") == 0)
		return MeasurementModes::CONT;
	if(meas_mode.compare("DIODE") == 0)
		return MeasurementModes::DIODE;
	if(meas_mode.compare("OHMS") == 0)
		return MeasurementModes::OHMS;
	if(meas_mode.compare("WIRE2") == 0)
		return MeasurementModes::WIRE2;
	if(meas_mode.compare("WIRE4") == 0)
		return MeasurementModes::WIRE4;
	if(meas_mode.compare("VAC") == 0)
		return MeasurementModes::VAC;
	if(meas_mode.compare("VACDC") == 0)
		return MeasurementModes::VACDC;
	if(meas_mode.compare("VDC") == 0)
		return MeasurementModes::VDC;

	return MeasurementModes::AAC;

}
unsigned char Fluke_8808A::get_register_value(StatusRegisters::Name reg_name)
{//Retrieve Fluke register value
	string reg_str;
	switch(reg_name)
	{
		case(StatusRegisters::STB):
			reg_str = send_command_and_receive_string("*STB?",com_port); break;
		case(StatusRegisters::SRE):
			reg_str = send_command_and_receive_string("*SRE?",com_port); break;
		case(StatusRegisters::ESR):
			reg_str = send_command_and_receive_string("*ESR?",com_port); break;
		case(StatusRegisters::ESE):
			reg_str = send_command_and_receive_string("*ESE?",com_port); break;
		default:
		return -1;
	}
	unsigned char status_byte;
	int n = sscanf(reg_str.c_str(), "%hhu", &status_byte);
	if(n!=1)
		cout << "ERROR PARSING REGISTER STRING" << endl;
	return status_byte;
}
void Fluke_8808A::print_status_byte_register()
{//Print status byte register to console
	unsigned char status_byte = get_register_value(StatusRegisters::STB);
	std::cout << endl << "Status Byte Register:  " << (int)status_byte << std::endl;
	//Mask MAV bit
	const char MAV_MASK = 0x10;
	std::cout << "Message Available:  ";
	if(MAV_MASK & status_byte)
		std::cout << "Yes" << std::endl;
	else
		std::cout << "No" << std::endl;
	//Mask Event Status bit
	const char EVENT_STATUS_MASK = 0x20;
	std::cout << "Active Event:  ";
	if(EVENT_STATUS_MASK & status_byte)
		std::cout << "Yes" << std::endl;
	else
		std::cout << "No" << std::endl;
	//Mask Master Summary Status bit
	const char MASTER_SUMMARY_STATUS_MASK = 0x40;
		std::cout << "Master Summary Status:  ";
	if(EVENT_STATUS_MASK & status_byte)
		std::cout << "Yes" << std::endl;
	else
		std::cout << "No" << std::endl;

}
void Fluke_8808A::print_event_status_register()
{//Print event status register to console
	unsigned char status_byte = get_register_value(StatusRegisters::ESR);
	std::cout << endl << "Event Status Register:  " << (int)status_byte << std::endl;
	//Mask OPC bit
	const char OPC_MASK = 0x01;
	std::cout << "Operation Complete:  ";
	if(OPC_MASK & status_byte)
		std::cout << "Yes" << std::endl;
	else
		std::cout << "No" << std::endl;
	//Mask QYE bit
	const char QYE_MASK = 0x04;
	std::cout << "Query Error:  ";
	if(QYE_MASK & status_byte)
		std::cout << "Yes" << std::endl;
	else
		std::cout << "No" << std::endl;
	//Mask DDE bit
	const char DDE_MASK = 0x08;
		std::cout << "Device-Dependent Error:  ";
	if(DDE_MASK & status_byte)
		std::cout << "Yes" << std::endl;
	else
		std::cout << "No" << std::endl;
	//Mask EXE bit
	const char EXE_MASK = 0x10;
	std::cout << "Execution Error:  ";
	if(EXE_MASK & status_byte)
		std::cout << "Yes" << std::endl;
	else
		std::cout << "No" << std::endl;
	//Mask CME bit
	const char CME_MASK = 0x20;
	std::cout << "Command Error:  ";
	if(CME_MASK & status_byte)
	std::cout << "Yes" << std::endl;
	else
	std::cout << "No" << std::endl;
	//Mask PO bit
	const char PO_MASK = 0x80;
	std::cout << "Power On:  ";
	if(PO_MASK & status_byte)
		std::cout << "Yes" << std::endl;
	else
		std::cout << "No" << std::endl;
}
void Fluke_8808A::clear_status_register()
{
	send_command_and_receive_string("*CLS",com_port);
}

float Fluke_8808A::fetch_measurement(Display::Num disp_num)
{
	string cmd;
	switch(disp_num)
	{
		case(Display::PRIMARY):
			cmd = "MEAS1?";
			break;
		case(Display::SECONDARY):
			cmd = "MEAS2?";
			break;
		default:
			return 0;
	}
	string val_str = send_command_and_receive_string(cmd,com_port);
	float val;
	int n = sscanf(val_str.c_str(),"%f",&val);
	if(n!= 1)
		val = 0;
	return val;
}

int Fluke_8808A::open_port(string port_str)
{
  /* File descriptor for the port */
  com_port = open(port_str.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
  if (com_port == -1)
  {
   /*
    * Could not open the port.
    */
  	valid_connection = false;
    perror("open_port: Unable to open /dev/ttyUSB0 - ");
  }
  else
  {
      fcntl(com_port, F_SETFL, FNDELAY);
    //fcntl(com_port, F_SETFL, 0);
    valid_connection = true;
  }
  return (com_port);
}


int Fluke_8808A::initialize_port_settings(int port)
{

  if(!valid_connection)
    return -1;

  //Set baud rate
  struct termios options;

  /*
   * Get the current options for the port...
   */
  tcgetattr(port, &options);
  /// Set the baud rates to 9600...
  cfsetispeed(&options, B9600);
  cfsetospeed(&options, B9600);
  //Enable the receiver and set local mode...
  options.c_cflag |= (CLOCAL | CREAD);

  options.c_cflag &= ~PARENB;//no parity
  options.c_cflag &= ~CSTOPB;//1 stop bit
  options.c_cflag &= ~CSIZE;//mask for data bits
  options.c_cflag |= CS8;//8 data bits

  //options.c_cflag &= ~CNEW_RTSCTS;//disable hardware flow control
  options.c_iflag &= ~(IXON | IXOFF | IXANY);//disable software flow control


  options.c_oflag &= ~OPOST;//raw output mode

  //Set the new options for the port...
  tcsetattr(port, TCSANOW|TCSAFLUSH, &options);
  return 0;
}

void Fluke_8808A::send_command(string cmd, int port)
{//Appends line feed and sends command
  if(!valid_connection)
  	return;

  //Append terminator to command and write to port
  cmd += '\r';
  int n = write(port,cmd.c_str(),cmd.length());
}


string Fluke_8808A::send_command_and_receive_string(string cmd, int port)
{//Send command and return response in string.
  send_command(cmd, port);
  usleep(FLUKE_TIMEOUT_US);
  //return read_string_until_line_feed(port);
  char buf[256];
  ssize_t s = read(port, buf, 256);
//  return read_string_until_prompt(port);
  return string(buf);
}
void Fluke_8808A::test_func()
{
  //Flush buffer
  //Send command
  int n = write(com_port,"*STB?\r",6);
  if(n!=6)
  	cout << "SEND FAILURE" << endl;
  else
  	cout << "SEND SUCCESSFUL" << endl;


  char buf[256];
  usleep(FLUKE_TIMEOUT_US);
  ssize_t s = read(com_port, buf, 256);
  cout << "Bytes read = " << s << endl;
  cout << buf;


cout << "END" << endl;
}
