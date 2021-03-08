// CPSC 323 Assignment 1 - Due 3/7/21


//Include statements, namespace std
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

//This function will take in a single character, and return true 
//if it is an operator (array of valid operators is within function), and false if it is not.
bool isOperator(char charToCheck)
{
    char operators[8] = { '*', '+', '-', '=', '/', '>', '<', '%' };

    for (int j = 0; j < 8; j++) {
        if (charToCheck == operators[j]) {
            return true;
        }
    }

    return false;
}

//This function will take in a single character, and return true 
//if it is a separator (array of valid separators is within function), and false if it is not.
bool isSeparator(char charToCheck)
{
    char separators[10] = { '(', ')', '{', '}', '[', ']', ',', ':', ';' };

    for (int j = 0; j < 9; j++) {
        if (charToCheck == separators[j]) {
            return true;
        }
    }
    return false;
}

//This function will take in a single string, and return true 
//if it is a keyword (array of valid keywords is within function), and false if it is not.
bool isKeyword(string stringToCheck)
{
    string keywords[21] = { "int", "float", "bool", "True", "False", "if", "else", "then", "endif", "endelse", "while", "whileend", "do", "enddo", "for", "endfor", "STDinput", "STDoutput", "and", "or", "not" };

    for (int j = 0; j < 21; j++) {
        if (stringToCheck == keywords[j]) {
            return true;
        }
    }

    return false;
}

//This function will take in a single string, and return true 
//if it is a legal identifer, and false if it is not.
bool isLegalIdentifier(string stringToCheck)
{
    if (!((stringToCheck[0] >= 'A' && stringToCheck[0] <= 'Z') || (stringToCheck[0] >= 'a' && stringToCheck[0] <= 'z'))) {
        return false;
    }

    for (int j = 0; j < stringToCheck.length(); j++) {
        char currentCharTemp = stringToCheck[j];

        if (!((currentCharTemp >= 'A' && currentCharTemp <= 'Z') || (currentCharTemp >= 'a' && currentCharTemp <= 'z') || (currentCharTemp >= '0' && currentCharTemp <= '9') || (currentCharTemp == '_') || (currentCharTemp == '$'))) {
            return false;
        }
    }

    return true;
}

//This function will take in a single string, and return true 
//if it is an integer, and false if it is not.
bool isInt(string stringToCheck)
{
    if (stringToCheck == "") {
        return false;
    }
    for (int j = 0; j < stringToCheck.length(); j++) {
        char currentCharTemp = stringToCheck[j];

        if (!isdigit(currentCharTemp)) {
            return false;
        }
    }

    return true;
}

//This function will take in a single string, and return true 
//if it is an float, and false if it is not.
bool isFloat(string stringToCheck)
{
    if (stringToCheck == "") {
        return false;
    }
    int numDecimals = 0;
    for (int j = 0; j < stringToCheck.length(); j++) {
        char currentCharTemp = stringToCheck[j];

        if (!isdigit(currentCharTemp) && currentCharTemp != '.') {
            return false;
        }

        if (currentCharTemp == '.') {
            numDecimals++;
        }
    }

    if (numDecimals == 1) {
        return true;
    }
    else {
        return false;
    }
}

//This is our main lexer function.  This function is called once for
//every line of input.txt.  It parses through the line, and calls the
//functions above to determine what each token is.
void lexer(fstream& outputFile, string line) {
    //Create an empty string variable
    string tempString = "";

    //Begin looping through the string character by character
    for (int i = 0; i < line.length(); i++) {

        char currentChar = line[i];
        if ((i == line.length() - 1 || isspace(currentChar)) && !isOperator(currentChar) && !isSeparator(currentChar)) {
            if (!isspace(currentChar)) {
                tempString += currentChar;
            }
            if (isKeyword(tempString)) {
                //Write to file
                outputFile << "KEYWORD -> " << tempString << "\n";
            }
            else if (isLegalIdentifier(tempString)) {
                outputFile << "IDENTIFIER -> " << tempString << "\n";
            }
            else if (isInt(tempString)) {
                outputFile << "INTEGER -> " << tempString << "\n";

            }
            else if (isFloat(tempString)) {
                outputFile << "FLOAT -> " << tempString << "\n";
            }
            else {
                if (isOperator(currentChar) || isSeparator(currentChar)) {
                    string outputLine;
                    if (isOperator(currentChar)) {
                        outputLine = "OPERATOR -> ";
                        outputLine += currentChar;
                        outputLine += '\n';
                    }
                    else {
                        outputLine = "SEPARATOR -> ";
                        outputLine += currentChar;
                        outputLine += '\n';
                    }
                    outputFile << outputLine;
                }
                else {
                    //Will get here if the string is unknown
                }
            }
            tempString = "";
        } 
        else if (isOperator(currentChar) || isSeparator(currentChar)) {
            string outputLine;
            if (isOperator(currentChar)) {
                outputLine = "OPERATOR -> ";
                outputLine += currentChar;
                outputLine += '\n';
            }
            else {
                outputLine = "SEPARATOR -> ";
                outputLine += currentChar;
                outputLine += '\n';
            }
          
            if (isKeyword(tempString)) {
                //Write to file
                outputFile << "KEYWORD -> " << tempString << "\n";
                outputFile << outputLine;
            }
            else if (isLegalIdentifier(tempString)) {
                if (tempString != "") {
                    outputFile << "IDENTIFIER -> " << tempString << "\n";
                }
                outputFile << outputLine;
            }
            else if (isInt(tempString)) {
                if (tempString != "") {
                    outputFile << "INTEGER -> " << tempString << "\n";
                }
                outputFile << outputLine;
            }
            else if (isFloat(tempString)) {
                if (tempString != "") {
                    outputFile << "FLOAT -> " << tempString << "\n";
                }
                outputFile << outputLine;
            }
            else {
                outputFile << outputLine;
            }
            tempString = "";
        }
        else {
            tempString += currentChar;  
        }

        
    }

}

int main() {

    // Create a text string, which is used to output the text file
    string textString;

    // Read from the text file
    ifstream MyReadFile("input.txt");

    //Create output textfile
    fstream MyFile;
    MyFile.open("output.txt", fstream::out);

    MyFile << "Output:\n============================\n";
    MyFile << "token            lexeme\n";
    MyFile << "============================\n";

    // Use a while loop together with the getline() function to read the file line by line
    while (getline(MyReadFile, textString)) {
        lexer(MyFile, textString);
    }

    // Close the file
    MyReadFile.close();
    MyFile.close();

    cout << "Output.txt has been created within the root directory of this assignment, containing a list of tokens and lexemes" << endl;

    return 0;
}


