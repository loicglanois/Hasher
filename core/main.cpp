#include <iostream>
#include <algorithm>      // Pour std::remove_if
#include <cctype>         // Pour isalpha, isdigit, etc.
#include <fstream>        // Pour écrire dans un fichier

using namespace std;

#include <conio.h> // Ajoutez ceci en haut du fichier si ce n'est pas déjà fait

void get_keyword_as_hidden_input(string *keyword) {
    char ch;
    keyword->clear();
    while (true) {
        ch = _getch();
        if (ch == '\r' || ch == '\n') {
            cout << endl;
            break;
        } else if (ch == '\b' || ch == 127) {
            if (!keyword->empty()) {
                keyword->pop_back();
                cout << "\b \b";
            }
        } else if (isprint(static_cast<unsigned char>(ch))) {
            keyword->push_back(ch);
            cout << '*';
        }
    }
}

void prompts_questions(string *keyword1, string *keyword2, string *outputFile, 
                      int *magicNumber, int *truncateLength, 
                      bool *noLetters, bool *noDigits, 
                      bool *noCapitals, bool *noSymbols) {
    cout << "Enter first keyword: ";
    get_keyword_as_hidden_input(keyword1);
    
    cout << "Enter second keyword: ";
    get_keyword_as_hidden_input(keyword2);
    
    cout << "Enter magic number : ";
    cin >> *magicNumber;
    
    cout << "Enter truncate length (20 by default): ";
    cin >> *truncateLength;
    
    char choice;
    cout << "Exclude letters? (y/n ; n by default): ";
    cin >> choice;
    *noLetters = (choice == 'y' || choice == 'Y');
    
    cout << "Exclude digits? (y/n): ";
    cin >> choice;
    *noDigits = (choice == 'y' || choice == 'Y');
    
    cout << "Exclude capitals? (y/n): ";
    cin >> choice;
    *noCapitals = (choice == 'y' || choice == 'Y');
    
    cout << "Exclude symbols? (y/n): ";
    cin >> choice;
    *noSymbols = (choice == 'y' || choice == 'Y');
}

void display_answers(const string &keyword1, const string &keyword2, 
                     const string &outputFile, int magicNumber, 
                     int truncateLength, bool noLetters, 
                     bool noDigits, bool noCapitals, bool noSymbols) {
    cout << "Keyword 1: " << keyword1 << endl;
    cout << "Keyword 2: " << keyword2 << endl;
    cout << "Output file: " << outputFile << endl;
    cout << "Magic number: " << magicNumber << endl;
    cout << "Truncate length: " << truncateLength << endl;
    cout << "No letters: " << (noLetters ? "Yes" : "No") << endl;
    cout << "No digits: " << (noDigits ? "Yes" : "No") << endl;
    cout << "No capitals: " << (noCapitals ? "Yes" : "No") << endl;
    cout << "No symbols: " << (noSymbols ? "Yes" : "No") << endl;

    // Write to output file
    ofstream outFile(outputFile);
    if (outFile.is_open()) {
        outFile << keyword1 << "\n" 
                << keyword2 << "\n"
                << magicNumber << "\n"
                << truncateLength << "\n"
                << (noLetters ? "No letters" : "") << "\n"
                << (noDigits ? "No digits" : "") << "\n"
                << (noCapitals ? "No capitals" : "") << "\n"
                << (noSymbols ? "No symbols" : "") << "\n";
        outFile.close();
        cout << "Answers written to file: " << outputFile << endl;
    } else {
        cerr << "Error opening file for writing." << endl;
    }
}

int main(int argc, char * argv[]) {
    string keyword1, keyword2, outputFile;
    int magicNumber = 0;
    int truncateLength = 0;
    bool noLetters = false, noDigits = false, noCapitals = false, noSymbols = false;

    prompts_questions(&keyword1, &keyword2, &outputFile, &magicNumber, &truncateLength, 
                      &noLetters, &noDigits, &noCapitals, &noSymbols);

    display_answers(keyword1, keyword2, outputFile, magicNumber, truncateLength, 
                      noLetters, noDigits, noCapitals, noSymbols);
    
    return 0;
}