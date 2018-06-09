#include "config.h"
#include <fstream>
#include <iostream>

double Gr=0,Gg,Gb,Rr,Rg,Rb,Br,Bg,Bb,Or,Og,Ob,Yr,Yg,Yb,Wr,Wg,Wb;

config::config()
{

}

config::~config(void) {
   //cout << "Object is being deleted" << endl;
}



inline bool config::exists (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

void config::writeCalibrateColors(config* c, config* aux)
{
    std::fstream t;
    t.open("ColorValues.config", std::fstream::in | std::fstream::out);
    c->Gr = aux->Gr;
    t << c->Gr << std::endl;
    c->Gg = aux->Gg;
    t << c->Gg << std::endl;
    c->Gb = aux->Gb;
    t << c->Gb << std::endl;
    c->Rr = aux->Rr;
    t << c->Rr << std::endl;
    c->Rg = aux->Rg;
    t << c->Rg << std::endl;
    c->Rb = aux->Rb;
    t << c->Rb << std::endl;
    c->Br = aux->Br;
    t << c->Br << std::endl;
    c->Bg = aux->Bg;
    t << c->Bg << std::endl;
    c->Bb = aux->Bb;
    t << c->Bb << std::endl;
    c->Or = aux->Or;
    t << c->Or << std::endl;
    c->Og = aux->Og;
    t << c->Og << std::endl;
    c->Ob = aux->Ob;
    t << c->Ob << std::endl;
    c->Yr = aux->Yr;
    t << c->Yr << std::endl;
    c->Yg = aux->Yg;
    t << c->Yg << std::endl;
    c->Yb = aux->Yb;
    t << c->Yb << std::endl;
    c->Wr = aux->Wr;
    t << c->Wr << std::endl;
    c->Wg = aux->Wg;
    t << c->Wg << std::endl;
    c->Wb = aux->Wb;
    t << c->Wb << std::endl;

    std::cout << "Gr: " << c->Gr << std::endl;
    std::cout << "Gg: " << c->Gg << std::endl;
    std::cout << "Gb: " << c->Gb << std::endl;
    std::cout << "Rr: " << c->Rr << std::endl;
    std::cout << "Rg: " << c->Rg << std::endl;
    std::cout << "Rb: " << c->Rb << std::endl;
    std::cout << "Br: " << c->Br << std::endl;
    std::cout << "Bg: " << c->Bg << std::endl;
    std::cout << "Bb: " << c->Bb << std::endl;
    std::cout << "Or: " << c->Or << std::endl;
    std::cout << "Og: " << c->Og << std::endl;
    std::cout << "Ob: " << c->Ob << std::endl;
    std::cout << "Yr: " << c->Yr << std::endl;
    std::cout << "Yg: " << c->Yg << std::endl;
    std::cout << "Yb: " << c->Yb << std::endl;
    std::cout << "Wr: " << c->Wr << std::endl;
    std::cout << "Wg: " << c->Wg << std::endl;
    std::cout << "Wb: " << c->Wb << std::endl;
}

void config::readCalibrateColors(config* c)
{
    std::cout << "ENTRA AQUI PELO AMOR DE DEUS" << std::endl;
    if(exists("ColorValues.config") == true)
    {
        //double d = 0.0;
        //c.Gr = 0.1;
//        std::cout << "1" << std::endl;
        std::fstream t;
        t.open("ColorValues.config", std::fstream::in | std::fstream::out);
//        std::cout << "2" << std::endl;
        std::string line;
//        std::cout << "3" << std::endl;
        getline(t,line);
        c->Gr = std::stod(line);
        getline(t,line);
        c->Gg = std::stod(line);
        getline(t,line);
        c->Gb = std::stod(line);
        getline(t,line);
        c->Rr = std::stod(line);
        getline(t,line);
        c->Rg = std::stod(line);
        getline(t,line);
        c->Rb = std::stod(line);
        getline(t,line);
        c->Br = std::stod(line);
        getline(t,line);
        c->Bg = std::stod(line);
        getline(t,line);
        c->Bb = std::stod(line);
        getline(t,line);
        c->Or = std::stod(line);
        getline(t,line);
        c->Og = std::stod(line);
        getline(t,line);
        c->Ob = std::stod(line);
        getline(t,line);
        c->Yr = std::stod(line);
        getline(t,line);
        c->Yg = std::stod(line);
        getline(t,line);
        c->Yb = std::stod(line);
        getline(t,line);
        c->Wr = std::stod(line);
        getline(t,line);
        c->Wg = std::stod(line);
        getline(t,line);
        c->Wb = std::stod(line);
        std::cout << "Gr: " << c->Gr << std::endl;
        std::cout << "Gg: " << c->Gg << std::endl;
        std::cout << "Gb: " << c->Gb << std::endl;
        std::cout << "Rr: " << c->Rr << std::endl;
        std::cout << "Rg: " << c->Rg << std::endl;
        std::cout << "Rb: " << c->Rb << std::endl;
        std::cout << "Br: " << c->Br << std::endl;
        std::cout << "Bg: " << c->Bg << std::endl;
        std::cout << "Bb: " << c->Bb << std::endl;
        std::cout << "Or: " << c->Or << std::endl;
        std::cout << "Og: " << c->Og << std::endl;
        std::cout << "Ob: " << c->Ob << std::endl;
        std::cout << "Yr: " << c->Yr << std::endl;
        std::cout << "Yg: " << c->Yg << std::endl;
        std::cout << "Yb: " << c->Yb << std::endl;
        std::cout << "Wr: " << c->Wr << std::endl;
        std::cout << "Wg: " << c->Wg << std::endl;
        std::cout << "Wb: " << c->Wb << std::endl;
    }
    else
    {
        std::ofstream initial;
        initial.open("ColorValues.config");
        std::cout << "2" << std::endl;
        for(int i = 0; i < 18; i++)
        {
            initial << "0" << std::endl;
        }
        initial.close();
        c->Gr=0;
        c->Gg=0;
        c->Gb=0;
        c->Rr=0;
        c->Rg=0;
        c->Rb=0;
        c->Br=0;
        c->Bg=0;
        c->Bb=0;
        c->Or=0;
        c->Og=0;
        c->Ob=0;
        c->Yr=0;
        c->Yg=0;
        c->Yb=0;
        c->Wr=0;
        c->Wg=0;
        c->Wb=0;
    }
}
