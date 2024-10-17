
#include "addition.h"
#include "subtraction.h"
#include "multiplication.h"
#include "division.h"
#include "constants.h"
#include <iostream>
#include <dlfcn.h>
#include <cmath>
#include <cctype>
#include <cstring>
#define MAX_SIZE 100


typedef double (*Functionpointer)(double, double);

char *trimSpaces(char *input)
{
    char *end = input + strlen(input) - 1;
    while (*input && isspace(*input))
        input++;
    while (isspace(*end))
        *end-- = '\0';
    return input;
}

double parseIntegralPart(char *input, int &index)
{
    double integralPart = 0;
    while (input[index] != '\0' && input[index] != '.')
    {
        if (isdigit(input[index]))
        {
            integralPart = integralPart * 10 + (input[index] - '0');
            index++;
        }
        else
        {
            integralPart = error_invild_input;
            break;
        }
    }
    return integralPart;
}

double parseDecimalPart(char *input, int &index)
{
    double decimalPart = 0.0;
    double multiplicationFactor = 0.1;
    int count = 1;
    while (input[index] != '\0' && input[index] != '.')
    {
        if (isdigit(input[index]))
        {
            decimalPart = decimalPart + (input[index] - '0') * pow(multiplicationFactor, count);
            count++;
            index++;
        }
        else
        {
            decimalPart = error_invild_input;
            break;
        }
    }
    return decimalPart;
}

void displayMenu()
{
    std::cout << "Select an operation:" << '\n';
    std::cout << " a: Addition" << '\n';
    std::cout << " s: Subtraction" << '\n';
    std::cout << " m: Multiplication" << '\n';
    std::cout << " d: Division" << '\n';
    std::cout << " e: Exit" << '\n';
}

Functionpointer getFunction(void *libraryHandler, char choice)
{
    Functionpointer function = nullptr;
    switch (choice)
    {
    case 'a':
        function = (Functionpointer)dlsym(libraryHandler, "addition");
        break;
    case 's':
        function = (Functionpointer)dlsym(libraryHandler, "subtraction");
        break;
    case 'm':
        function = (Functionpointer)dlsym(libraryHandler, "multiplication");
        break;
    case 'd':
        function = (Functionpointer)dlsym(libraryHandler, "division");
        break;
    default:
        std::cout << "Invalid choice!" << '\n';
        break;
    }
    if (!function)
    {
        std::cerr << "Function not found:" << dlerror() << '\n';
    }
    return function;
}

bool validateChoice(char *choice)
{
    bool isValid = false;
    if (choice != nullptr && choice[1] == '\0')
    {
        if (choice[0] == 'a' || choice[0] == 's' || choice[0] == 'm' || choice[0] == 'd' || choice[0] == 'e')
        {
            isValid = true;
        }
    }
    return isValid;
}

double validateInput(char *input)
{
    double result = 0;
    int sign = 1;
    double integralPart = 0;
    double decimalPart = 0;

    int index = 0;
    if (input[0] == '-')
    {
        sign = -1;
        index++;
    }
    else if (input[0] == '+')
    {
        index++;
    }

    if (!isdigit(input[index]))
    {
        result = error_invild_input;
    }
    integralPart = parseIntegralPart(input, index);

    if (input[index] == '.')
    {
        index++;
        decimalPart = parseDecimalPart(input, index);
    }
    if (input[index] == '.')
    {
        result = error_invild_input;
    }
    if (integralPart != error_invild_input && decimalPart != error_invild_input)
    {
        result = sign * (integralPart + decimalPart);
    }
    else
        result = error_invild_input;

    return result;
}

double getInput()
{
    char input[MAX_SIZE];
    std::cout << "Enter the number: ";
    std::cin.getline(input, MAX_SIZE);

    double value = validateInput(trimSpaces(input));
    if (value != error_invild_input)
    {
        return value;
    }
    else
    {
        std::cout << "Invalid input. Please try again." << '\n';
        return getInput();
    }
}
void processChoice(char *choice, void *libraryHandler)
{
    double operand1, operand2;
    double output;
    Functionpointer functionPointer = nullptr;
    functionPointer = getFunction(libraryHandler, *choice);
    if (functionPointer)
    {
        operand1 = getInput();
        operand2 = getInput();
        output = functionPointer(operand1, operand2);
        if (output == INFINITY)
        {
            std::cout << "Error : Division by zero" << '\n'<<'\n';
        }
        else if ((std::isnan(output)))
        {
            std::cout << "Error : Indeterminate form" << '\n'<<'\n';
        }
        else
            std::cout << "Result: " << output << '\n'
                      << '\n';
    }
}

void start(){

    char choice[MAX_SIZE];

    void *libraryHandler = nullptr;
    char *error;

    libraryHandler = dlopen("/opt/lib/libmaths.so", RTLD_LAZY);
    if (!libraryHandler)
    {
        std::cerr << "Error opening library: " << dlerror() << std::endl;
        return ;
    }
    while (true)
    {
        displayMenu();
        std::cout << "Enter your choice: ";
        std::cin.getline(choice, MAX_SIZE);
        char *trimmedchoice = trimSpaces(choice);

        if (trimmedchoice[0] == 'e' && trimmedchoice[1] == '\0')
        {
            std::cout << "Exiting from the program." << '\n';
            break;
        }

        if (validateChoice(trimmedchoice))
        {
            processChoice(trimmedchoice, libraryHandler);
        }
        else
        {
            std::cout << "Invalid Choice!!" << '\n'<<'\n';
            std::cout << "Please enter your choice from the menu" << '\n';
        }
    }
    dlclose(libraryHandler);
}
int main()
{

 start();
return 0;
}
