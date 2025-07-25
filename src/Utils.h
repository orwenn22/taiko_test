#ifndef UTILS_H
#define UTILS_H
#include <cstdint>
#include <cstdlib>



inline float BeatlengthToBPM(float beatlength) {
    return 60000 / beatlength;
}

int ParseIntStrict(const char *str, size_t *location);
int ParseInt(const char *str, size_t *location);
float ParseFloat(const char *str, size_t *location);
int CountCharUntil(const char *str, char c, size_t start_location, char stop_char);



#endif //UTILS_H
