#pragma once

//Converts a long double to string.
//This function uses sprintf with the highest supported precision.
//pld: Pointer to an 80-bit (10 byte) long double.
//str: Buffer with at least 32 bytes of space.
extern "C" __declspec(dllimport) void ld2str(const void* pld, char* str);

//Converts a string to a long double.
//This function uses http://en.cppreference.com/w/c/string/byte/strtof
//str: The string to convert.
//pld: Pointer to an 80-bit (10 byte) long double.
extern "C" __declspec(dllimport) bool str2ld(const char* str, void* pld);
