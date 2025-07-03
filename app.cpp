#include <iostream>
#include <string>
#include "device_driver.h"

class App {
public:
	App(DeviceDriver* driver);
	void readAndPrint(int startAddr, int endAddr) {
		int buf;
		std::cout << "0x";
		for (int addr = startAddr; addr <= endAddr; addr++) {
			buf = driver->read(addr);
			printf("%x", buf);
		}
	}
	void writeAll(int value) {

	}
private:
	DeviceDriver* driver;
};