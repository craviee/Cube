#pragma once

#include <map>
#include <string>
#include "arduino.h"
#include "square.h"
#include "constants.h"
#include "utils.h"
#include "identification.cpp"

class ColorReader
{
    public:
        ColorReader(std::map<std::string, Square> squares, std::map<std::string, double> configValues
        , std::shared_ptr<Microcontroller> microcontroller);
        void read();
    private:
        std::map<std::string, Square> squares;
        std::map<std::string, double> configValues;
        std::shared_ptr<Microcontroller> microcontroller;
        void updateModel(Face currentFace, std::vector<Color> identifiedColors);
};