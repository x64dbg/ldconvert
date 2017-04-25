#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <errno.h>

enum FltClass
{
    FltInfinity,
    FltSNaN,
    FltQNaN,
    FltNormalized,
    FltDenormal,
    FltZero,
    FltInvalid
};

typedef union my__mingw_ldbl_type_t
{
    long double x;
    struct
    {
        unsigned int low, high;
        int sign_exponent : 16;
        int res1 : 16;
        int res0 : 32;
    } lh;
} my__mingw_ldbl_type_t;

//Based on: https://en.wikipedia.org/wiki/Extended_precision#x86_extended_precision_format
static FltClass myclassify(long double ld)
{
    my__mingw_ldbl_type_t lds;
    lds.x = ld;
    bool bit63 = (lds.lh.high & 0x80000000) != 0;
    switch (lds.lh.sign_exponent & 0x7fff) //Exponent
    {
    case 0: //Exponent: All Zeros
    {
        if (!bit63) //Bit 63: Zero
        {
            if (lds.lh.low == 0 && (lds.lh.high & 0x7fffffff) == 0) //Bits 62-0: Zero
            {
                return FltZero; //Zero. The sign bit gives the sign of the zero.
            }
            else //Bits 62-0: Non-zero
            {
                return FltDenormal; //Denormal. The value is (−1)^s * m * 2^(−16382)
            }
        }
        else //Bit 63: One
        {
            return FltDenormal; //Pseudo Denormal. The 80387 and later properly interpret this value but will not generate it. The value is (−1)^s * m * 2^(−16382)
        }
    }

    case 0x7fff: //Exponent: All Ones
    {
        bool bit62 = (lds.lh.high & 0x40000000) != 0;
        bool bit61to0_zero = (lds.lh.high & 0x3fffffff) == 0 && lds.lh.low == 0;
        if (!bit63 && !bit62) //Bits 63,62: 00
        {
            if (bit61to0_zero) //Bits 61-0: Zero
            {
                return FltInvalid; //Pseudo-Infinity. The sign bit gives the sign of the infinity. The 8087 and 80287 treat this as Infinity. The 80387 and later treat this as an invalid operand.
            }
            else //Bits 61-0: Non-zero
            {
                return FltInvalid; //Pseudo Not a Number. The sign bit is meaningless. The 8087 and 80287 treat this as a Signaling Not a Number. The 80387 and later treat this as an invalid operand.
            }
        }
        else if (!bit63/* && bit62*/) //Bits 63,62: 01
        {
            //Bits 61-0: Anything
            return FltInvalid; //Pseudo Not a Number. The sign bit is meaningless. The 8087 and 80287 treat this as a Signaling Not a Number. The 80387 and later treat this as an invalid operand.
        }
        else if (/*bit63 && */!bit62) //Bits 63,62: 10
        {
            if (bit61to0_zero) //Bits 61-0: Zero
            {
                return FltInfinity; //Infinity. The sign bit gives the sign of the infinity. The 8087 and 80287 treat this as a Signaling Not a Number. The 8087 and 80287 coprocessors used the pseudo-infinity representation for infinities.
            }
            else //Bits 61-0: Non-zero
            {
                return FltSNaN; //Signalling Not a Number, the sign bit is meaningless.
            }
        }
        else /*if (bit63 && bit62)*/ //Bits 63,62: 11
        {
            if (bit61to0_zero) //Bits 61-0: Zero
            {
                return FltQNaN; //Floating-point Indefinite, the result of invalid calculations such as square root of a negative number, logarithm of a negative number, 0/0, infinity / infinity, infinity times 0, and others when the processor has been configured to not generate exceptions for invalid operands. The sign bit is meaningless. This is a special case of a Quiet Not a Number.
            }
            else //Bits 61-0: Non-zero
            {
                return FltQNaN; //Quiet Not a Number, the sign bit is meaningless. The 8087 and 80287 treat this as a Signaling Not a Number.
            }
        }
    }

    default: //Exponent: All other values
    {
        if (!bit63) //Bit 63: Zero
        {
            return FltInvalid; //Unnormal. Only generated on the 8087 and 80287. The 80387 and later treat this as an invalid operand. The value is (−1)^s * m * 2^(e−16383)
        }
        else //Bit 63: One
        {
            return FltNormalized; //Normalized value. The value is (−1)^s * m * 2^(e−16383)
        }
    }
    }
}

//Converts a long double to string.
//This function uses sprintf with the highest supported precision.
//pld: Pointer to an 80-bit (10 byte) long double.
//str: Buffer with at least 32 bytes of space.
extern "C" __declspec(dllexport) void ld2str(const void* pld, char* str)
{
    long double ld;
    memset(&ld, 0, sizeof(ld));
    memcpy(&ld, pld, 10);
    switch(myclassify(ld))
    {
    case FltInfinity: strcpy(str, "inf"); break;
    case FltSNaN: strcpy(str, "snan"); break;
    case FltQNaN: strcpy(str, "qnan"); break;
    case FltInvalid: strcpy(str, "invalid"); break;
    default: snprintf(str, 32, "%.*Lf", LDBL_DIG, ld);
    }
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
