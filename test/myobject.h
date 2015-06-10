#pragma once

#include <string>
#include <iostream>

class MyObject
{
public:
    explicit MyObject(double value);
    explicit MyObject(std::string const& value);
    virtual ~MyObject() = default;

    double plus_one();

    double add(MyObject const& obj1, MyObject const& obj2)
    {
        return obj1.value_ + obj2.value_;
    }

    virtual double value()
    {
        return value_;
    }

private:
    double value_;
};

class MyObject2
{
public:
    MyObject2(double value, int value2);
    virtual ~MyObject2() = default;

    double plus_two();
    int plus_two(int x);

    void base_method()
    {
        std::cout << "hello!\n";
        std::cout.flush();
    }

    static void static_method()
    {
        std::cout << "static hello!\n";
        std::cout.flush();
    }

private:
    double value_;
};
