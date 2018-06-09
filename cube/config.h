#ifndef CONFIG_H
#define CONFIG_H
#include <fstream>
#include <fstream>
#include <iostream>

class config
{
public:
    config();
    ~config();
    void readCalibrateColors(config *c);
    void writeCalibrateColors(config* c, config* aux);
    void sumCalibrationVector(std::string s, double r, double g, double b);
    inline bool exists (const std::string& name);
    double Gr,Gg,Gb,Rr,Rg,Rb,Br,Bg,Bb,Or,Og,Ob,Yr,Yg,Yb,Wr,Wg,Wb;
};



#endif // CONFIG_H
