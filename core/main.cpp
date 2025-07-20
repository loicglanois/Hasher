#include <iostream>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <limits>
#include <conio.h>

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
};

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
    cout << "Enter first keyword : ";
    get_keyword_as_hidden_input(*options.keyword1);
    
    cout << "Enter second keyword : ";
    get_keyword_as_hidden_input(*options.keyword2);
    
    cout << "Enter magic number : ";
    get_keyword_as_hidden_input(input);
    if (input.empty()) {
        *options.magicNumber = 0; // Valeur par défaut si vide
    } else {
        try {
            *options.magicNumber = stoi(input);
        } catch (...) {
            *options.magicNumber = 0; // Valeur par défaut en cas d'erreur
        }
    }
    
    cout << "Enter truncate length (20 by default): ";
    get_keyword_as_hidden_input(input);
    if (input.empty()) {
        *options.truncateLength = 20;
    } else {
        try {
            *options.truncateLength = stoi(input);
        } catch (...) {
            *options.truncateLength = 20;
        }
    }
        
    cout << "Exclude letters? (y/n): ";
    get_keyword_as_hidden_input(input);
    *options.noLetters = (input.empty() || input.at(0) == 'y' || input.at(0) == 'Y');
    cout << "Exclude digits? (y/n): ";
    get_keyword_as_hidden_input(input);
    *options.noDigits = (input.empty() || input.at(0) == 'y' || input.at(0) == 'Y');
    cout << "Exclude capitals? (y/n): ";
    get_keyword_as_hidden_input(input);
    *options.noCapitals = (input.empty() || input.at(0) == 'y' || input.at(0) == 'Y');
    cout << "Exclude symbols? (y/n): ";
    get_keyword_as_hidden_input(input);
    *options.noSymbols = (input.empty() || input.at(0) == 'y' || input.at(0) == 'Y');

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
        new bool(false)
    };

    if (argc > 0) {
        prompts_questions(options);
    } else {
        parse_arguments(argc, argv, options);
    }

    display_answers(options);


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