#include "Ldr.h"

Ldr::Ldr() {}

Ldr::~Ldr() {}

int Ldr::readLdr() {
    return analogRead(ldrPin);
}
