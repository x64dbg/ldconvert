#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <errno.h>

//Converts a long double to string.
//This function uses sprintf with the highest supported precision.
//pld: Pointer to an 80-bit (10 byte) long double.
//str: Buffer with at least 32 bytes of space.
extern "C" __declspec(dllexport) void ld2str(const void* pld, char* str)
{
    long double ld;
    memcpy(&ld, pld, 10);
    sprintf(str, "%.*Lf", LDBL_DIG, ld);
}

//Converts a string to a long double.
//This function uses http://en.cppreference.com/w/c/string/byte/strtof
//str: The string to convert.
//pld: Pointer to an 80-bit (10 byte) long double.
extern "C" __declspec(dllexport) bool str2ld(const char* str, void* pld)
{
    errno = 0;
    char* pos;
    long double ld = strtold(str, &pos);
    memcpy(pld, &ld, 10);
    return !(str == pos && errno == ERANGE);
}
