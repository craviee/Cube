#include "arduino.h"

Arduino::Arduino()
{
        // foreach(const QSerialPortInfo &serialPortInfo, QSerialPortInfo::availablePorts()){
        //     if(serialPortInfo.hasVendorIdentifier() && serialPortInfo.hasProductIdentifier()){
        //         if(serialPortInfo.vendorIdentifier() == arduino_uno_vendor_id){
        //             if(serialPortInfo.productIdentifier() == arduino_uno_product_id){
        //                 arduino_port_name = "ttyACM1";
        //                 std::cout << arduino_port_name.toStdString() << std::endl;
        //                 arduino_is_available = true;
        //                 std::cout << "arduino_is_available = true" << std::endl;
        //             }
        //         }
        //     }
        // }
        // if(arduino_is_available){
            
        //     arduino->setPortName(arduino_port_name);
        //     arduino->setBaudRate(QSerialPort::Baud9600);
        //     arduino->setDataBits(QSerialPort::Data8);
        //     arduino->setParity(QSerialPort::NoParity);
        //     arduino->setStopBits(QSerialPort::OneStop);
        //     arduino->setFlowControl(QSerialPort::NoFlowControl);
        //     if(arduino->open(QSerialPort::WriteOnly))
        //         std::cout << "arduino aberto" << std::endl;
        //     else
        //         std::cout << "arduino não aberto" << std:: endl;
        // }else{
            
        //     std::cout << "Arduido não está ativo" << std::endl;
        // }
}

void Arduino::runCommand(const char* command)
{

}