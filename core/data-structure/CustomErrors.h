#ifndef ERROR_H
#define ERROR_H

#include <string>

typedef struct {
    int code;
    std::string message;
} Error;


typedef enum {
    OK = 0,
    IndexOutOfRange = 1,
    NotValidArgument = 2,
    EmptyContainer = 3,
    NullPointerPassedAsArgument = 4,
    NotImplement = 5,
    ChangeImmutableType = 6,
    TypeMismatchError = 7
} Errors;
 
extern const Error errors[];
void checkErrors(int errorIndex);

#endif