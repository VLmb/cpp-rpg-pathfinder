#include "CustomErrors.h"
#include <iostream>

const Error errors[8] = {
    {0, "Operation completed successfully."},
    {1, "Error: Index is out of range. Please provide a valid index."},
    {2, "Error: Invalid argument provided. Please check the input values."},
    {3, "Error: The container is empty. No elements to process."},
    {4, "Error: Null pointer passed as an argument. Please provide a valid pointer."},
    {5, "Error: This method haven't implemented in the class from which you are calling it."},
    {6, "Error: An attempt to change an immutable object."},
    {7, "Error: Type mismatch. Please ensure the types are compatible."}
};

void checkErrors(int errorIndex) {
    std::cout << errors[errorIndex].message << std::endl;
}