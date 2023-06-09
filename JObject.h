#pragma once
#include "Reflectable.h"

class JObject : public Reflectable
{
public:
    JObject(Reflectable* parent = nullptr) 
    :   Reflectable(parent)
    ,   serial_number { counter++ }
    {
    }

    int getSerialNumber() const 
    {
        return serial_number;
    }

private:
    const int serial_number;
    static int counter;
};

int JObject::counter = 0;