#include "arduino.h"

Arduino::Arduino()
{
    foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts())
    {
        if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier() &&
        serialPortInfo.vendorIdentifier() == ARDUINO_UNO_VENDOR_ID &&
        serialPortInfo.productIdentifier() == ARDUINO_UNO_PRODUCT_ID)
        {
            arduino->setPortName(serialPortInfo.portName());
            arduino->setBaudRate(QSerialPort::Baud9600);
            arduino->setDataBits(QSerialPort::Data8);
            arduino->setParity(QSerialPort::NoParity);
            arduino->setStopBits(QSerialPort::OneStop);
            arduino->setFlowControl(QSerialPort::NoFlowControl);
            arduino->open(QSerialPort::ReadWrite);
            // Wait 1s for the initialization to be completed
            Utils::delay(1000);
        }
    }
}

void Arduino::runCommand(Command command)
{
    const char* commandFormated = command2str(command).c_str();
    arduino->write(commandFormated);
    delayByCommand(command);
}

bool Arduino::isAvailable()
{
    return arduino->isReadable() && arduino->isWritable();
}

void Arduino::delayByCommand(Command command)
{
    switch (command)
    {
        // Start with UP, so no delay
        case Command::MOVE_UP: Utils::delay(0); break;
        
        case Command::MOVE_FRONT: Utils::delay(5000); break;
        case Command::MOVE_DOWN: Utils::delay(5000); break;
        case Command::MOVE_BACK: Utils::delay(5000); break;
        case Command::MOVE_RIGHT: Utils::delay(9000); break;
        case Command::MOVE_LEFT: Utils::delay(7000); break;
    }
}

std::string Arduino::command2str(Command command)
{
    switch (command)
    {
        case Command::MOVE_UP: return "00";
        case Command::MOVE_FRONT: return "01";
        case Command::MOVE_DOWN: return "02";
        case Command::MOVE_BACK: return "03";
        case Command::MOVE_RIGHT: return "04";
        case Command::MOVE_LEFT: return "05";
        default: return "ERROR";
    }
}
