#pragma once
#include <string>
#include <stdexcept>

class Property 
{
public:
    virtual ~Property() = default;
    virtual std::string toString() const = 0;
};

template<typename T>
class TypedProperty : public Property 
{
public:
    T value;
    std::string toString() const override { throw std::runtime_error("Not implemented."); }
};

template<>
class TypedProperty<std::string> : public Property 
{
public:
    std::string value;

    std::string toString() const override
    {
        return value;
    }
};

template<>
class TypedProperty<double> : public Property 
{
public:
    double value;

    std::string toString() const override
    {
        return std::to_string(value);
    }
};

template<>
class TypedProperty<int> : public Property 
{
public:
    int value;

    std::string toString() const override
    {
        return std::to_string(value);
    }
};