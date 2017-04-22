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
    memset(&ld, 0, sizeof(ld));
    memcpy(&ld, pld, 10);
    if(snprintf(str, 32, "%.*Lf", LDBL_DIG, ld) == 31)
        memcpy(str, "nan", 4);
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

//Converts a long double to a double.
//pld: Pointer to an 80-bit (10 byte) long double.
//pd: Pointer to a 64-bit (8 byte) double.
extern "C" __declspec(dllexport) void ld2d(const void* pld, void* pd)
{
    long double ld;
    memset(&ld, 0, sizeof(ld));
    memcpy(&ld, pld, 10);
    double d = double(ld);
    memcpy(pd, &d, 8);
}

//Converts a double to a long double.
//pd: Pointer to a 64-bit (8 byte) double.
//pld: Pointer to an 80-bit (10 byte) long double.
extern "C" __declspec(dllexport) void d2ld(const void* pd, void* pld)
{
    double d;
    memcpy(&d, pd, 8);
    long double ld = d;
    memcpy(pld, &ld, 10);
}
