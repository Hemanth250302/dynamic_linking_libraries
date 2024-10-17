#include "addition.h"
#include "subtraction.h"
#include "multiplication.h"
#include "division.h"
#include "constants.h"
#include <iostream>
#include <cmath>
#include <cctype>
#include <cstring>
#define MAX_SIZE 100

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

double performOperation(char choice, double operand1, double operand2)
{
    double result;
    switch (choice)
    {
    case 'a':
        result = addition(operand1, operand2);
        break;
    case 's':
        result = subtraction(operand1, operand2);
        break;
    case 'm':
        result = multiplication(operand1, operand2);
        break;
    case 'd':
        if (operand1 == 0 && operand2 == 0)
        {
            std::cout << "Error: Indeterminate form." << '\n'
                      << '\n';
            result = error_invild_result;
            break;
        }
        else if (operand2 == 0)
        {
            std::cout << "Error: Division by zero." << '\n'
                      << '\n';
            result = error_invild_result;
            break;
        }

        result = division(operand1, operand2);
        break;
    default:
        std::cout << "Invalid choice!" << '\n';
        result = error_invild_result;
        break;
    }
    return result;
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
        result = sign * (integralPart + decimalPart);
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

void start()
{
    double operand1, operand2;
    char choice[MAX_SIZE];
    double output;

    while (true)
    {
        displayMenu();
        std::cout << "Enter your choice: ";
        std::cin.getline(choice, MAX_SIZE);
        char *trimmedchoice = trimSpaces(choice);
        if (trimmedchoice[0] == 'e' && trimmedchoice[1] == '\0')
        {
            std::cout << "Exiting the program." << '\n';
            return;
        }
        if (validateChoice(trimmedchoice))
        {

            operand1 = getInput();
            operand2 = getInput();
            output = performOperation(*trimmedchoice, operand1, operand2);
            if (output != error_invild_result)
                std::cout << "Result: " << output << '\n'
                          << '\n';
        }
        else
        {
            std::cout << "Invalid Choice!!" << '\n'
                      << '\n';
            std::cout << "Please enter your choice from the menu" << '\n';
        }
    }
}
int main()
{

    start();

    return 0;
}
