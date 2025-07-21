#include <iostream>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <limits>
#include <conio.h>
#include <vector>
#include <cmath>
#include <random>
#include "sha256.h"

// Commande pour compiler : g++ -o main main.cpp sha256.h

using namespace std;

struct OPTIONS {
    string *keyword1;
    string *keyword2;
    int *magicNumber;
    int *truncateLength;
    bool *noLetters;
    bool *noDigits;
    bool *noCapitals;
    bool *noSymbols;
    bool *noLowercase;
};

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
    // pre_hash_as_sha_256(&combined);
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

void parse_arguments(int argc, char *argv[], OPTIONS &options) {
    for (int i = 1; i < argc; ++i) {
        string arg = argv[i];
        if (arg == "--keyword1" && i + 1 < argc) {
            *options.keyword1 = argv[++i];
        } else if (arg == "--keyword2" && i + 1 < argc) {
            *options.keyword2 = argv[++i];
        } else if (arg == "--magicNumber" && i + 1 < argc) {
            try {
                *options.magicNumber = stoi(argv[++i]);
            } catch (...) {
                *options.magicNumber = 0;
            }
        } else if (arg == "--truncateLength" && i + 1 < argc) {
            try {
                *options.truncateLength = stoi(argv[++i]);
            } catch (...) {
                *options.truncateLength = 20;
            }
        } else if (arg == "--noLetters") {
            *options.noLetters = true;
        } else if (arg == "--noDigits") {
            *options.noDigits = true;
        } else if (arg == "--noCapitals") {
            *options.noCapitals = true;
        } else if (arg == "--noSymbols") {
            *options.noSymbols = true;
        } else if (arg == "--noLowercase") {
            *options.noLowercase = true;
        }
    }
}

void get_keyword_as_hidden_input(string &keyword) {
    char ch;
    keyword.clear();
    while (true) {
        ch = _getch();
        if (ch == '\r' || ch == '\n') {
            cout << endl;
            break;
        } else if (ch == '\b' || ch == 127) {
            if (!keyword.empty()) {
                keyword.pop_back();
                cout << "\b \b";
            }
        } else if (isprint(static_cast<unsigned char>(ch))) {
            keyword.push_back(ch);
        }
    }
}

void prompts_questions(OPTIONS &options) {
    
    string input;
    for (auto &keyword : {options.keyword1, options.keyword2}) {
        cout << "Enter keyword number " << (keyword == options.keyword1 ? "1" : "2") << ": ";
        get_keyword_as_hidden_input(*keyword);
        if (keyword->empty()) {
            *keyword = "";
        }
    }

    if (options.keyword1->empty() && options.keyword2->empty()) {
        cerr << "Error: Both keywords are required" << endl;
        exit(EXIT_FAILURE);
    }

    for (auto &number : {options.magicNumber, options.truncateLength}) {
        cout << "Enter " << (number == options.magicNumber ? "magic number" : "truncate length") << ": ";
        get_keyword_as_hidden_input(input);
        if (input.empty()) {
            *number = (number == options.magicNumber ? 0 : 20); // Default values
        } else {
            try {
                *number = stoi(input);
            } catch (...) {
                *number = (number == options.magicNumber ? 0 : 20); // Default values on error
            }
        }
    }
        
    for (auto &option : {options.noLetters, options.noDigits, options.noCapitals, options.noSymbols, options.noLowercase}) {
        cout << "Exclude " << (option == options.noLetters ? "letters" :
                              option == options.noDigits ? "digits" :
                              option == options.noCapitals ? "capitals" :
                              option == options.noSymbols ? "symbols" : "lowercase") << "? (y/n): ";
        get_keyword_as_hidden_input(input);
        *option = !(input.empty() || input.at(0) == 'y' || input.at(0) == 'Y');
    }

}

void display_answers(const OPTIONS &options) {
    cout << "Keyword 1: " << *options.keyword1 << endl;
    cout << "Keyword 2: " << *options.keyword2 << endl;
    cout << "Magic Number: " << *options.magicNumber << endl;
    cout << "Truncate Length: " << *options.truncateLength << endl;
    cout << "No Letters: " << (*options.noLetters ? "Yes" : "No") << endl;
    cout << "No Digits: " << (*options.noDigits ? "Yes" : "No") << endl;
    cout << "No Capitals: " << (*options.noCapitals ? "Yes" : "No") << endl;
    cout << "No Symbols: " << (*options.noSymbols ? "Yes" : "No") << endl;
    cout << "No Lowercase: " << (*options.noLowercase ? "Yes" : "No") << endl;
}

int main(int argc, char * argv[]) {
    struct OPTIONS options {
        new string(""),
        new string(""),
        new int(0),
        new int(20),
        new bool(false),
        new bool(false),
        new bool(false),
        new bool(false),
        new bool(false)
    };

    if (argc > 0) {
        prompts_questions(options);
    } else {
        parse_arguments(argc, argv, options);
    }

    std::random_device rd;
    unsigned int seed = 0;
    for (char c : *options.keyword1) {
        seed = seed * 31 + c + *options.magicNumber;
    }
    mt19937 gen(seed);

    string hash = customHash(*options.keyword1, *options.keyword2, *options.magicNumber, *options.truncateLength, 
                            *options.noLetters, *options.noDigits, *options.noCapitals, *options.noSymbols, *options.noLowercase, gen, seed);

    cout << "Hash: " << hash << endl;

    delete options.keyword1;
    delete options.keyword2;
    delete options.magicNumber;
    delete options.truncateLength;
    delete options.noLetters;
    delete options.noDigits;
    delete options.noCapitals;
    delete options.noSymbols;
    
    return 0;
}