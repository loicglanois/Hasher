#include "sha256.h"
// #include "encryption.h"
#include <random>
#include <algorithm>


using namespace std;

bool are_all_allowed_characters_present(const string &hash, bool noLetters, bool noDigits, bool noCapitals, bool noSymbols, bool noLowercase) {
    bool hasLetter = false;
    bool hasDigit = false;
    bool hasCapital = false;
    bool hasSymbol = false;
    bool hasLowercase = false;
    for (char c : hash) {
        if (!noLetters && isalpha(c)) hasLetter = true;
        if (!noDigits && isdigit(c)) hasDigit = true;
        if (!noCapitals && isupper(c)) hasCapital = true;
        if (!noSymbols && !isalnum(c)) hasSymbol = true;
        if (!noLowercase && islower(c)) hasLowercase = true;

        if (hasLetter && hasDigit && hasCapital && hasSymbol && hasLowercase) {
            return true;
        }
    }

    return false;
}

void force_all_types_of_allowed_characters_in_hash(string &hash, bool noLetters, bool noDigits, bool noCapitals, bool noSymbols, bool noLowercase, unsigned int seed) {
    mt19937 gen(seed);
    string allowedChars;

    if (!noLetters) {
        allowedChars += "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }
    if (!noDigits) {
        allowedChars += "0123456789";
    }
    if (!noCapitals) {
        allowedChars += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    }
    if (!noSymbols) {
        allowedChars += "!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";
    }
    if (!noLowercase) {
        allowedChars += "abcdefghijklmnopqrstuvwxyz";
    }

    if (allowedChars.empty()) {
        hash = "NO_VALID_CHARACTERS";
        return;
    }

    while (hash.length() < 32) {
        hash += allowedChars[gen() % allowedChars.length()];
    }
}

void pre_hash_as_sha_256(string *input) {
    char hex[SHA256_HEX_SIZE];
    sha256_hex(input->c_str(), input->length(), hex);
    *input = string(hex);
}

char getRandomChar(mt19937& gen, int min, int max) {
    uniform_int_distribution<> dis(min, max);
    return static_cast<char>(dis(gen));
}

string filterCharacters(string& input, bool noLetters, bool noDigits, bool noCapitals, bool noSymbols, bool noLowercase = false) {
    string result;
    for (char c : input) {
        if (noLetters && isalpha(c)) continue;
        if (noDigits && isdigit(c)) continue;
        if (noCapitals && isupper(c)) continue;
        if (noSymbols && !isalnum(c)) continue;
        if (noLowercase && islower(c)) continue;
        result += c;
    }
    return result;
}

string customHash(const string& keyword1, const string& keyword2, int magicNumber, int truncateLength, 
                  bool noLetters, bool noDigits, bool noCapitals, bool noSymbols, bool noLowercase, mt19937& gen, unsigned int seed) {
    string combined = keyword1 + to_string(magicNumber) + keyword2;
    pre_hash_as_sha_256(&combined);
    if (combined.empty()) combined = "default";

    int targetLength = max(32, static_cast<int>(combined.length() * 1.5));
    while (combined.length() < targetLength) {
        int choice = gen() % 4;
        switch (choice) {
            case 0: combined += getRandomChar(gen, 'a', 'z'); break;
            case 1: combined += getRandomChar(gen, 'A', 'Z'); break;
            case 2: combined += getRandomChar(gen, '0', '9'); break;
            case 3: combined += getRandomChar(gen, 33, 47); break;
        }
    }

    shuffle(combined.begin(), combined.end(), gen);

    for (size_t i = 0; i < combined.size(); ++i) {
        char& c = combined[i];
        int transformation = (i * magicNumber + c) % 94 + 33;
        c = static_cast<char>(transformation);

        if (c < 33) c += 33;
        if (c > 126) c -= 94;
    }

    string filtered = filterCharacters(combined, noLetters, noDigits, noCapitals, noSymbols, noLowercase);

    if (truncateLength > 0 && filtered.length() > static_cast<size_t>(truncateLength)) {
        filtered = filtered.substr(0, truncateLength);
    }

    if (!are_all_allowed_characters_present(filtered, noLetters, noDigits, noCapitals, noSymbols, noLowercase)) {

        customHash(filtered, keyword2, magicNumber, truncateLength, noLetters, noDigits, noCapitals, noSymbols, noLowercase, gen, seed);
    }

    return filtered.empty() ? "NO_VALID_CHARACTERS" : filtered;
}