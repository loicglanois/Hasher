#ifndef ENCRYPTION_H
#define ENCRYPTION_H

#include <string>
#include <random>

void pre_hash_as_sha_256(std::string *input);
char getRandomChar(std::mt19937& gen, int min, int max);
void force_all_types_of_allowed_characters_in_hash(std::string &hash, bool noLetters, bool noDigits, bool noCapitals, bool noSymbols, bool noLowercase, unsigned int seed);
std::string filterCharacters(std::string& input, bool noLetters, bool noDigits, bool noCapitals, bool noSymbols, bool noLowercase = false);
std::string customHash(const std::string& keyword1, const std::string& keyword2, int magicNumber, int truncateLength,
                  bool noLetters, bool noDigits, bool noCapitals, bool noSymbols, bool noLowercase, std::mt19937& gen, unsigned int seed);

#endif