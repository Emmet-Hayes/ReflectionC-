#pragma once
#include <functional>

class Method
{
public:
    virtual ~Method() = default;
};

template<typename Ret, typename... Args>
class TypedMethod : public Method
{
public:
    TypedMethod(std::function<Ret(Args...)> func) : func_(func) {}
    Ret operator()(Args... args) { return func_(args...); }
private:
    std::function<Ret(Args...)> func_;
};