#include "myobject.h"

MyObject::MyObject(double value)
{
    value_ = value;
}

MyObject::MyObject(std::string const& value)
{
    value_ = 1;
}

double MyObject::plus_one()
{
    value_ += 1;
    return value_;
}

MyObject2::MyObject2(double value, int value2)
{
    value_ = value;
}

double MyObject2::plus_two()
{
    value_ += 2;
    return value_;
}

int MyObject2::plus_two(int x)
{
    value_ += 2 + x;
    return value_;
}
