#include <stdio.h>   /* Standard input/output definitions */
#include <string.h>  /* String function definitions */
#include <unistd.h>  /* UNIX standard function definitions */
#include <fcntl.h>   /* File control definitions */
#include <errno.h>   /* Error number definitions */
#include <termios.h> /* POSIX terminal control definitions */
#include <iostream>

#define FLUKE_TIMEOUT_US 3000000 //How long to wait between sending command and recieving response

#define FLUKE_RANGE_200mV		1
#define FLUKE_RANGE_2V			2
#define FLUKE_RANGE_20V			3
#define FLUKE_RANGE_200V		4
#define FLUKE_RANGE_1000V_DC	5
#define FLUKE_RANGE_750V_AC		5

#define FLUKE_RANGE_200OHMS		1
#define FLUKE_RANGE_2kOHMS		2
#define FLUKE_RANGE_20kOHMS		3
#define FLUKE_RANGE_200kOHMS	4
#define FLUKE_RANGE_2MOHMS		5
#define FLUKE_RANGE_20MOHMS		6
#define FLUKE_RANGE_100MOHMS	7

#define FLUKE_RANGE_20mA_DC		1
#define FLUKE_RANGE_200mA_DC	2
#define FLUKE_RANGE_2A_DC		3
#define FLUKE_RANGE_10A_DC		4

#define FLUKE_RANGE_2kHz		1
#define FLUKE_RANGE_20kHz		2
#define FLUKE_RANGE_200kHz		3
#define FLUKE_RANGE_1000kHz		4

#define FLUKE_RANGE_200uA_AC	1
#define FLUKE_RANGE_2000uA_AC	2
#define FLUKE_RANGE_20mA_AC		3
#define FLUKE_RANGE_200mA_AC	4
#define FLUKE_RANGE_2A_AC		5
#define FLUKE_RANGE_10A_AC		6


using namespace std;

namespace Fluke_8808A_namespace
{
	struct StatusRegisters
	{	enum Name{STB,SRE,ESR,ESE}; };
	struct MeasurementModes
	{ enum Mode{			AAC,	//AC current
							AACDC,	//AC plus DC rms current
							ADC,	//DC current
							CONT,	//Continuity test
							DIODE,	//Diode test
							FREQ,	//Frequency
							OHMS,	//Resistance
							WIRE2,	//2-wire resistance (primary display only)
							WIRE4,	//4-wire resistance (primary display only)
							VAC,	//AC volts
							VACDC,	//AC plus DC rms volts
							VDC};};	//DC volts
	struct Display
	{	enum Num {PRIMARY,SECONDARY};};
	struct Rate
	{	enum Speed {SLOW,MEDIUM,FAST};};
}
using namespace Fluke_8808A_namespace;
class Fluke_8808A
{

public:
	bool valid_connection;
	int com_port;
	Fluke_8808A();
	bool connect(string com_port_str);
	void disconnect();
	void set_measurement_mode(Display::Num disp_num, MeasurementModes::Mode mode);
	MeasurementModes::Mode get_measurement_mode(Display::Num disp_num);
	unsigned char get_register_value(StatusRegisters::Name reg_name);
	void print_status_byte_register();
	void print_event_status_register();
	void clear_status_register();
	void test_func();
	float fetch_measurement(Display::Num disp_num);
	void set_range();

private:
	//Serial Port functions.  This should probably be made into a different class
	int open_port(string com_);
	int initialize_port_settings(int port);
	void send_command(string cmd, int port);
	string send_command_and_receive_string(string cmd, int port);
};
