#ifndef UTILS_H
#define UTILS_H

#include <cstddef> //for size_t

//This file basically acts as a place for everything I didn't know where to place in the codebase.
//It'll probably be a huge mess in the future (well, it already is), but that's an issue for future me :)


//inline float BeatlengthToBPM(float beat_length) {
//    return 60000 / beat_length;
//}

int ParseIntStrict(const char *str, size_t *location);
int ParseInt(const char *str, size_t *location);
float ParseFloat(const char *str, size_t *location);

/**
 * Ok, so apparently strings in osu files can be between double quotes, but sometimes they are not??
 * This function should be able to handle both cases.
 * If the string isn't between double quotes, the cursor will be before the first non-comma character from the start
 * location after parsing.
 * If the string is between double quotes, the cursor will be after the second double quote.
 *
 * @param str the string containing the thing we want to parse
 * @param location the starting index at which we want to parse
 * @return heap-allocated string (or nullptr on failure)
 */
char *ParseStrOsu(const char *str, size_t *location);

/**
 * Count the amount of a specific character in a string until stop_char (or \0) is reached
 * @param str the string in which we want to count the character
 * @param c the character we want to count
 * @param start_location the position in the string at which we start counting
 * @param stop_char we stop counting if this character is encountered in the string
 * @return the amount of time c appears
 */
int CountCharUntil(const char *str, char c, size_t start_location, char stop_char);

/**
 * Remove newline characters (\r and \n) from a string
 * @param str the string in which we want to remove the newline characters (IT WILL BE MODIFIED)
 */
void RemoveNewline(char *str);

/**
 *
 * @param str
 * @param suffix
 * @return true if str ends with suffix
 */
bool EndsWith(const char *str, const char *suffix);


#endif //UTILS_H
