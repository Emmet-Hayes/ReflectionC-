#pragma once
#include "TypedProperty.h"
#include "TypedMethod.h"

class Reflectable 
{
public:
    Reflectable() : parentClass(nullptr) {}
    Reflectable(Reflectable* parentClass) : parentClass { parentClass } {}

    Reflectable* getParentClass() const { return parentClass; }
    void setParentClass(Reflectable* parent) { parentClass = parent; }

    template<typename T>
    void setProperty(const std::string& name, const T& value) 
    {
        auto it = properties.find(name);
        if (it == properties.end())
            properties[name] = std::make_shared<TypedProperty<T>>();
        else 
            if (dynamic_cast<TypedProperty<T>*>(it->second.get()) == nullptr)
                throw std::runtime_error("A property already exists with the name '" + name + "' but it is of a different type.");
        
        static_cast<TypedProperty<T>*>(properties[name].get())->value = value;
    }

    template<typename T>
    T getProperty(const std::string& name) const 
    {
        auto it = properties.find(name);
        if (it != properties.end())
        {
            TypedProperty<T>* prop = dynamic_cast<TypedProperty<T>*>(it->second.get());
            if (prop == nullptr)
                throw std::runtime_error("Property '" + name + "' exists but is of a different type.");
            return prop->value;
        }
        else if (parentClass != nullptr)
            return parentClass->getProperty<T>(name);
        else
            throw std::runtime_error("Property '" + name + "' does not exist.");
    }

    template<typename Ret, typename... Args>
    void addMethod(const std::string& name, std::function<Ret(Args...)> func)
    {
        auto it = methods.find(name);
        if (it == methods.end())
            methods[name] = std::make_shared<TypedMethod<Ret, Args...>>(std::move(func));
    }

    template<typename Ret, typename... Args>
    Ret callMethod(const std::string& name, Args... args)
    {
        auto it = methods.find(name);
        if (it != methods.end()) 
        {
            std::cout << "Method: " << it->first << "\n";
            std::cout << "What is your method?\n" << it->second.get() << "\n";
            TypedMethod<Ret, Args...>* method = dynamic_cast<TypedMethod<Ret, Args...>*>(it->second.get());
            if (method == nullptr)
                throw std::runtime_error("Method '" + name + "' exists but has different arguments.");
            return (*method)((args)...);
        } 
        else if (parentClass != nullptr)
            return parentClass->callMethod<Ret, Args...>(name, std::forward<Args>(args)...);
        else
            throw std::runtime_error("Method '" + name + "' does not exist.");
    }

    std::unordered_map<std::string, std::shared_ptr<Property>>& getProperties() { return properties; }
    std::unordered_map<std::string, std::shared_ptr<Method>>& getMethods() { return methods; }

private:
    std::unordered_map<std::string, std::shared_ptr<Property>> properties;
    std::unordered_map<std::string, std::shared_ptr<Method>> methods;
    Reflectable* parentClass = nullptr;
};