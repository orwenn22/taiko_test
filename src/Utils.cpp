#include "Utils.h"

#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>

#include "main.h"


int ParseIntStrict(const char *str, size_t *location) {
    if (str == nullptr || location == nullptr) return 0;

    int result = 0;
    while (str[*location] != 0) {
        if (str[*location] < '0' || str[*location] > '9') break;
        result = result * 10 + (str[*location] - '0');
        ++(*location);
    }
    return result;
}

//Accept floats and convert back to int
int ParseInt(const char *str, size_t *location) {
    if (str == nullptr || location == nullptr) return 0;

    char buf[32] = {0};
    size_t i = 0;
    while (str[*location] != 0) {
        char c = str[*location];
        if ((c < '0' || c > '9') && c != '.' && c != '-') break;
        buf[i] = c;
        ++(*location);
        ++i;
    }
    return (int) std::round(atof(buf));
}

float ParseFloat(const char *str, size_t *location) {
    if (str == nullptr || location == nullptr) return 0;

    char buf[32] = {0};
    size_t i = 0;
    while (str[*location] != 0) {
        char c = str[*location];
        if ((c < '0' || c > '9') && c != '.' && c != '-') break;
        buf[i] = c;
        ++(*location);
        ++i;
    }
    return (float) std::round(atof(buf));
}


int CountCharUntil(const char *str, char c, size_t start_location, char stop_char) {
    if (str == nullptr) return 0;
    size_t len = strlen(str);
    if (start_location >= len) return 0;

    int count = 0;
    for (size_t i = start_location; i < len; ++i) {
        if (str[i] == stop_char) return count;
        if (str[i] == c) ++count;
    }

    return count;
}
