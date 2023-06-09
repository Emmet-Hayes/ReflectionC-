#include <unordered_map>
#include <string>
#include <iostream>
#include <memory>
#include <random>
#include <functional>

#include "TestAgent.h"

std::string generate_random_string(size_t length) {
    static const std::string characters = "abcdefghijklmnopqrstuvwxyz";
    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> distribution(0, characters.size()-1);

    std::string random_string;
    for (std::size_t i = 0; i < length; ++i)
        random_string += characters[distribution(generator)];

    return random_string;
}

const int num_reflectable_test_objs = 10;

int ReflectableTest() 
{
    JObject obj[num_reflectable_test_objs];
    TestAgent agent[num_reflectable_test_objs];

    std::random_device rd;
    std::default_random_engine generator(rd());
    std::uniform_int_distribution<int> age_distribution(20, 50);

    try 
    {
        for (int i = 0; i < num_reflectable_test_objs; ++i) 
        {
            obj[i].setProperty<int>("age", age_distribution(generator));
            obj[i].setProperty<std::string>("name", generate_random_string(5));
            obj[i].addMethod<void>("PrintAllProperties", std::function<void(void)>([&obj, i]() {
                for (const auto& prop : obj[i].getProperties())
                    std::cout << prop.first << " : " << prop.second->toString() << "\n";
            }));

            obj[i].addMethod<void>("PrintSerialNumber", std::function<void(void)>([&obj, i]() {
                std::cout << "Serial Number: " << obj[i].getSerialNumber() << "\n";
            }));

            obj[i].addMethod<std::string>("GetAllProperties", std::function<std::string(void)>([&obj, i]() {
                std::string propertiesString;
                for (const auto& prop : obj[i].getProperties())
                    propertiesString += prop.first + ": " + prop.second->toString() + "\n";
                return propertiesString;
            }));

            obj[i].callMethod<void>("PrintAllProperties");
            obj[i].callMethod<void>("PrintSerialNumber");
            std::string propertiesString = obj[i].callMethod<std::string>("GetAllProperties");
            std::cout << "Properties of object " << i << ":\n" << propertiesString << "Done.\n";

            // now testing parental hierarchy
            if (i > 0)
            {
                obj[i].setParentClass(&obj[i-1]);
                std::cout << "Parent of object " << i << " is object " << i-1 << "\nParent Info:\n";
                obj[i].getParentClass()->callMethod<void>("PrintAllProperties");
                obj[i].callMethod<void>("PrintSerialNumber");
                std::cout << "Done with parent info.\n\n";
            }
            else 
                std::cout << "\n";
        }
        for (int i = 0; i < num_reflectable_test_objs; ++i) 
        {   
            agent[i].setProperty<int>("age", age_distribution(generator));
            agent[i].setProperty<std::string>("name", generate_random_string(5));

            agent[i].addMethod<void>("PrintAllProperties", std::function<void(void)>([&agent, i]() {
                for (const auto& prop : agent[i].getProperties())
                    std::cout << prop.first << " : " << prop.second->toString() << "\n";
            }));

            agent[i].addMethod<void>("PrintSerialNumber", std::function<void(void)>([&agent, i]() {
                std::cout << "Serial Number: " << agent[i].getSerialNumber() << "\n";
            }));

            agent[i].addMethod<std::string>("GetAllProperties", std::function<std::string(void)>([&agent, i]() {
                std::string propertiesString;
                for (const auto& prop : agent[i].getProperties())
                    propertiesString += prop.first + ": " + prop.second->toString() + "\n";
                return propertiesString;
            }));

            agent[i].callMethod<void>("PrintAllProperties");
            agent[i].callMethod<void>("PrintSerialNumber");
            std::string propertiesString = obj[i].callMethod<std::string>("GetAllProperties");
            std::cout << "Properties of agent " << i << ":\n" << propertiesString << "Done.\n";

            agent[i].addMethod<std::string>("introduce", std::function<std::string(void)>([=]() {
                return agent[i].introduce();
            }));

            agent[i].addMethod<std::string>("saySomething", std::function<std::string(std::string)>([agent, i](std::string phrase) {
                return agent[i].saySomething(phrase);
            }));
            
            agent[i].addMethod<std::string>("doMath", 
                std::function<std::string(const int, const int, std::string)>([=](const int num1, const int num2, std::string operation) {
                return agent[i].doMath(num1, num2, operation);
            }));

            std::cout << "methods in agent: " << agent[i].getMethods().size() << "\n";
            for (const auto& method : agent[i].getMethods())
                std::cout << method.first << "\n";
            std::cout << "Introduce: " << agent[i].callMethod<std::string>("introduce") << "\n";
            std::string arg1 = "Hello World!", arg2 = "add", arg3 = "subtract", arg4 = "multiply", arg5 = "divide";
            std::cout << "saySomething: " << agent[i].callMethod<std::string>("saySomething", arg1) << "\n";
            std::cout << "DoMath (add): " << agent[i].callMethod<std::string>("doMath", 5, 3, arg2) << "\n";
            std::cout << "DoMath (subtract): " << agent[i].callMethod<std::string>("doMath", 5, 3, arg3) << "\n";
            std::cout << "DoMath (multiply): " << agent[i].callMethod<std::string>("doMath", 5, 3, arg4) << "\n";
            std::cout << "DoMath (divide): " << agent[i].callMethod<std::string>("doMath", 5, 3, arg5) << "\n";
        }
        
        std::cout << "Occupation: " << obj[0].getProperty<std::string>("occupation") << "\n";
    } 
    catch (const std::runtime_error& e) 
    {
        std::cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}

int main()
{
    ReflectableTest();
}