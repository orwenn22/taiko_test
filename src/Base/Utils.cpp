#include "Utils.h"

#include <chrono>
#include <cmath>
#include <cstdio>
#include <cstring>

#include "../Raylib/main.h"


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


char *ParseStrOsu(const char *str, size_t *location) {
    if (str == nullptr || location == nullptr) return nullptr;

    bool between_quotes = (str[*location] == '"');

    size_t start_location = *location + (size_t) between_quotes; //skip first quote if necessary
    size_t end_location = start_location;
    char stop_char = between_quotes ? '"' : ',';

    while (str[end_location] != stop_char && str[end_location] != 0) ++end_location;
    if (between_quotes && str[end_location] == 0) return nullptr;  //didn't find a second double quote  TODO: better handle that case? return the string anyway?

    size_t length = end_location - start_location + 1; //+1 for null char
    char *result = (char *) malloc(sizeof(char) * (length));
    for (size_t i = start_location; i < end_location; ++i) result[i - start_location] = str[i];
    result[length-1] = 0;

    *location = end_location + (size_t) between_quotes; //skip last quote if necessary
    return result;
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

void RemoveNewline(char *str) {
    if (str == nullptr) return;

    size_t len = strlen(str);
    while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
        str[len - 1] = 0;
        --len;
    }
}


bool EndsWith(const char *str, const char *suffix) {
    if (str == nullptr || suffix == nullptr) return false;
    size_t len = strlen(str);
    size_t suffix_len = strlen(suffix);
    if (len < suffix_len) return false;

    for (int i = 0; i < suffix_len; ++i) {
        if (str[len-1 - i] != suffix[suffix_len-1 - i]) return false;
    }

    return true;
}
