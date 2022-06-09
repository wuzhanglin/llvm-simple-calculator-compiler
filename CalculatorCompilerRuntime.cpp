#include <iostream>

// Export C++ functions as C-style functions
extern "C"
{
    void CalculatorWrite(int result)
    {
        std::cout << "The result is: " << result << std::endl;
    }

    int CalculatorRead(const char* variableName)
    {
        std::cout << "Enter a value for " << variableName << ": ";

        int value;
        std::cin >> value;

        return value;
    }
}
