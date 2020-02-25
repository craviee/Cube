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
        case Command::MOVE_FRONT: Utils::delay(5000); break;
        case Command::MOVE_DOWN: Utils::delay(5000); break;
        case Command::MOVE_BACK: Utils::delay(5000); break;
        case Command::MOVE_RIGHT: Utils::delay(9000); break;
        case Command::MOVE_LEFT: Utils::delay(7000); break;
        case Command::MOVE_UP: Utils::delay(12000); break;
        case Command::DO_U_ROTATION: Utils::delay(35000); break;
        case Command::DO_UA_ROTATION: Utils::delay(20000); break;
        case Command::DO_F_ROTATION: Utils::delay(35000); break;
        case Command::DO_FA_ROTATION: Utils::delay(20000); break;
        case Command::DO_D_ROTATION: Utils::delay(31000); break;
        case Command::DO_DA_ROTATION: Utils::delay(20000); break;
        case Command::DO_B_ROTATION: Utils::delay(27000); break;
        case Command::DO_BA_ROTATION: Utils::delay(19000); break;
        case Command::DO_R_ROTATION: Utils::delay(39000); break;
        case Command::DO_RA_ROTATION: Utils::delay(29000); break;
        case Command::DO_L_ROTATION: Utils::delay(39000); break;
        case Command::DO_LA_ROTATION: Utils::delay(21000); break; 
    }
}

std::string Arduino::command2str(Command command)
{
    switch (command)
    {
        case Command::MOVE_FRONT: return "01";
        case Command::MOVE_DOWN: return "02";
        case Command::MOVE_BACK: return "03";
        case Command::MOVE_RIGHT: return "04";
        case Command::MOVE_LEFT: return "05";
        case Command::MOVE_UP: return "06";
        case Command::DO_U_ROTATION: return "07";
        case Command::DO_UA_ROTATION: return "08";
        case Command::DO_F_ROTATION: return "09";
        case Command::DO_FA_ROTATION: return "010";
        case Command::DO_D_ROTATION: return "011";
        case Command::DO_DA_ROTATION: return "012";
        case Command::DO_B_ROTATION: return "013";
        case Command::DO_BA_ROTATION: return "014";
        case Command::DO_R_ROTATION: return "015";
        case Command::DO_RA_ROTATION: return "016";
        case Command::DO_L_ROTATION: return "017";
        case Command::DO_LA_ROTATION: return "018"; 
        default: return "ERROR";
    }
}
