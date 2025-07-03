#include "device_driver.h"
#include <exception>

class ReadFailException : public std::exception {};
class WriteFailException : public std::exception {};

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{}

int DeviceDriver::read(long address)
{
    int ret = (int)m_hardware->read(address);
    // TODO: implement this method properly
    for (int i = 0; i < 4; i++) {
        int tmp = (int)m_hardware->read(address);
        if (ret != tmp && i != 0)
            throw ReadFailException();
    }
    return ret;
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    if (m_hardware->read(address) != 0xFF)
        throw WriteFailException();

    m_hardware->write(address, (unsigned char)data);
}