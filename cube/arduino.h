#pragma once

#include <QSerialPort>
#include <QSerialPortInfo>
#include <memory>
#include "microcontroller.h"
#include "utils.h"

#define  ARDUINO_UNO_VENDOR_ID 9025
#define  ARDUINO_UNO_PRODUCT_ID 67

class Arduino : public Microcontroller
{
    public:
        Arduino();
        void runCommand(Command command) override;
        bool isAvailable() override;
    private:
        std::unique_ptr<QSerialPort> arduino = std::make_unique<QSerialPort>();
        std::string arduinoPortName;
        void delayByCommand(Command command);
        std::string command2str(Command command);
};
