#ifndef LDR_H
#define LDR_H

#include <Arduino.h>

class Ldr {
public:
    Ldr();
    ~Ldr();

    int readLdr();

private:
    static const int ldrPin = 6;
};

extern Ldr ldr;

#endif
