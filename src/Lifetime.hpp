#pragma once

#include <iostream>
#include <source_location>

struct Lifetime
{
    Lifetime()
    {
        print("Lifetime()");
    }

    Lifetime(
        const Lifetime&)
    {
        print("Lifetime(const Lifetime&)");
    }

    Lifetime(
        Lifetime&&)
    {
        print("Lifetime(Lifetime&&)");
    }

    Lifetime& operator=(
        const Lifetime&)
    {
        print("operator=(const Lifetime&)");
        return *this;
    }

    Lifetime& operator=(
        Lifetime&&)
    {
        print("operator=(Lifetime&&)");
        return *this;
    }

    ~Lifetime()
    {
        print("~Lifetime()");
    }

private:
    static void print(
        const char* operation)
    {
        std::cout
            << operation << '\n';
    }
};