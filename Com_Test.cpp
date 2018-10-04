#include "Fluke_8808A.hpp"

int main()
{
	Fluke_8808A* fluke = new Fluke_8808A();
	if(!fluke->connect("/dev/ttyUSB0"))
		{cout << "Unable to connect." << endl;
		return 0;}
/*
	fluke->print_event_status_register();
	//usleep(10000);
	fluke->print_status_byte_register();
	
	fluke->set_measurement_mode(Display::PRIMARY, MeasurementModes::VDC);
	fluke->set_measurement_mode(Display::SECONDARY, MeasurementModes::ADC);

	fluke->print_status_byte_register();
	fluke->print_event_status_register();*/

	double meas = fluke->fetch_measurement(Display::PRIMARY);
	cout << "Measurement value:  " << meas << endl;
	MeasurementModes::Mode m = fluke->get_measurement_mode(Display::PRIMARY);

//Flush buffer
	//fluke->test_func();

	//std::cout << "Press <ENTER> to exit." << std::endl;
	//getchar();
	fluke->disconnect();
	return 0;
}
