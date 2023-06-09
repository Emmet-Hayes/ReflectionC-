#pragma once
#include "JObject.h"

class TestAgent : public JObject
{
public:
    TestAgent(Reflectable* parent = nullptr) 
    :   JObject(parent)
    {
    }

    std::string introduce() const
    {
        return "Hi, I'm " + getProperty<std::string>("name") + " and I am " + std::to_string(this->getProperty<int>("age")) + " years old.";
    }

    std::string saySomething(const std::string& phrase) const 
    {
        return getProperty<std::string>("name") + " says " + phrase;
    }

    std::string doMath(const int num1, const int num2, std::string operation) const
    {
        if (operation == "add")
            return std::to_string(num1 + num2);
        else if (operation == "subtract")
            return std::to_string(num1 - num2);
        else if (operation == "multiply")
            return std::to_string(num1 * num2);
        else if (operation == "divide")
            if (num2 != 0)
                return std::to_string(static_cast<double>(num1) / static_cast<double>(num2));
            else
                return "Cannot divide by zero.";
        else
            return "Unknown operation.";
    }
};