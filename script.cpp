#include <iostream>
#include <iomanip>
#include <string>
#include <stdio.h>
#include <stdlib.h>
//#include <Windows.h>
#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
//#include <conio.h>
using namespace std;
using namespace cgicc;

#define N 3
#define ENG_LETTERS "AaBCcEeHKkMmOoPpTXxYy"
#define RUS_LETTERS "АаВСсЕеНКкМмОоРрТХхУу"
#define NEED_SYMBOLS_TO_MAKE_LOWERCASE 32
#define TO_BE_REMOVED "чтд кг км м г с а  в к но и да не или либо же а что чтобы как так т д пр др "

//double antiPlagiarism(string text, string fragment); !!!!!!
string antiPlagiarism(string text, string fragment);

int calcNumberOfWordsInString(string str1);

bool isDigit(char c);
bool isEngLetter(char c);
bool isSeparator(char character);

string symbolProcessing(string str1);

void extractWordsFromStringIntoArray(string source, string *destination);

int calcNumberOfMatchingShingles(string fragment[], int wordsInFragment, string text[], int wordsInText);

double calcPercentageOfCoincidenceShinglesFromTotalNumber(int hitCounter, int totalWords);
string getDB();
string removeWord(string str, string toBeRemoved);
bool wordsAreEqual(string word1, string words);
void changeString(string &newString, string &currentWord);
int lettersCounter(string c);


string replaceEngLetters(string str1);

string toLowerCase(string str1);

int main()
{
	//SetConsoleCP(1251);       // Set Cyrillic encoding console input
    //SetConsoleOutputCP(1251); // Set Cyrillic encoding console output
        
    Cgicc form;
    string name;

    cout << "Content-type:text/html\r\n\r\n";
    cout << "<html>\n";
    cout << "<head>\n";
    cout << "<title>Ggi Server</title>\n";
    cout << "</head>\n";
    cout << "<body>\n";
    cout << "<p>";
	    
    name = form("name");
    if (!name.empty()) {
	    cout << antiPlagiarism(getDB(), name) << "\n";
    	//cout << setprecision(4) <<100 - antiPlagiarism(getDB(), name) << "\n"; !!!!!
    } else {
    	cout << "Text is not provided!\n";
    }	
    cout << "</p>\n";
    cout << "</body>\n";
    cout << "</html>\n";

    return 0;
}

//double antiPlagiarism(string text, string fragment) {
string antiPlagiarism(string text, string fragment) {
   /* int hitCounter = 0, numberOfWordsInFragment = 0, numberOfWordsInText = 0;

    fragment = symbolProcessing(fragment);
    text = symbolProcessing(text);
    
    fragment = replaceEngLetters(fragment);
    text = replaceEngLetters(text);
    
    fragment = toLowerCase(fragment);
    text = toLowerCase(text);
	
	string toBeRemoved = TO_BE_REMOVED;
	
	fragment = removeWord(fragment, toBeRemoved);
    text = removeWord(text, toBeRemoved);
	
    numberOfWordsInFragment = calcNumberOfWordsInString(fragment);
    numberOfWordsInText = calcNumberOfWordsInString(text);

    string wordsFromFragment[numberOfWordsInFragment];
    string wordsFromText[numberOfWordsInText];

    extractWordsFromStringIntoArray(fragment, wordsFromFragment);
    extractWordsFromStringIntoArray(text, wordsFromText);

    hitCounter = calcNumberOfMatchingShingles(wordsFromFragment, numberOfWordsInFragment, wordsFromText,
                                              numberOfWordsInText);
	
    return calcPercentageOfCoincidenceShinglesFromTotalNumber(hitCounter, numberOfWordsInFragment);   */
	/*string test;
	for (int i = 0; i < 5; i++) {
		test[i] = text[i];
		test[i+5] = fragment[i];
	}
	test[10] = '\0';*/
	return concat("TEST", fragment, text);
}

string getDB(){
	std::ifstream in("db.txt");

    string dbText = "", s1;
    while (getline(in, s1))
    	dbText += s1;
  
    in.close();
    
    return dbText;
}

bool isDigit(char c) {
    return c >= '0' and c <= '9';
}

int calcNumberOfWordsInString(string str1) {
    int counter = 0;
    bool flag = false;

    for (int i = 0; str1[i]; ++i) {
        if (str1[i] != ' ' and !flag) {
            flag = true;
        } else if (str1[i] == ' ' and flag or str1[i + 1] == '\0' and flag) {
            flag = false;
            counter++;
        }
    }
    return counter;
}

bool isSeparator(char character) {
    char separators[] = " .,-:;!?()+=/*\n";

    for (int i = 0; separators[i]; ++i) {
        if (character == separators[i]) {
            return true;
        }
    }
    return false;
}

string symbolProcessing(string str1) {
    for (int i = 0; str1[i]; ++i) {
        if (isDigit(str1[i]) or isSeparator(str1[i])) {
            str1[i] = ' ';
        } else {}
    }
    return str1;
}

string replaceEngLetters(string str1)
{
    string engLetters = ENG_LETTERS;
    string rusLetters = RUS_LETTERS;
    int i, j;    
	string s = "";
	
    for (i = 0; str1[i]; i++) {
        if (isEngLetter(str1[i])) {
            for (j = 0; str1[j]; j++) {
                if (str1[i] == engLetters[j]) {
                    s += rusLetters[j];
                }
            }
        }
        else 
        	s += str1[i];
    }
    return s;
}

bool isEngLetter(char c)
{
    return ('A' <= c and c <= 'Z') or ('a' <= c and c <= 'z');
}

string toLowerCase(string str1) {

    for(int i = 0; str1[i]; i++) {
        if(str1[i] >= 'А' and str1[i] <= 'Я') {
            str1[i] += NEED_SYMBOLS_TO_MAKE_LOWERCASE; 
        }
        if(str1[i] == 'Ё') {
            str1[i] = 'ё';
        }
    }
    return str1;  
}


void extractWordsFromStringIntoArray(string source, string *destination) {
    int wordCounter = 0;

    for (int i = 0; source[i]; i++) {
        if (!isSeparator(source[i]) and isSeparator(source[i + 1])) {
            destination[wordCounter] += source[i];
            wordCounter++;
        } else if (!isSeparator(source[i]) and !isSeparator(source[i + 1])) {
            destination[wordCounter] += source[i];
        } else {}
    }

}

int calcNumberOfMatchingShingles(string fragment[], int wordsInFragment, string text[], int wordsInText) {
    int counter = 0, numberOfMatchedShingles = 0;

    for (int i = 0; i < wordsInFragment - N + 1; ++i) {
        for (int j = 0; j < wordsInText - N + 1; ++j) {
            for (int k = 0; k < N; ++k) {
                if (fragment[i + k] == text[j + k]) {
                    numberOfMatchedShingles++;
                }
            }
            if (numberOfMatchedShingles == N) {
                counter++;
            }
            numberOfMatchedShingles = 0;
        }
    }
    return counter;
}

double calcPercentageOfCoincidenceShinglesFromTotalNumber(int hitCounter, int totalWords) {
    int totalNumberOfShingles = totalWords - N + 1;
    return ((double) hitCounter / totalNumberOfShingles) * 100;
}

string removeWord(string str1, string toBeRemoved)
{
	string currentWord = "";	
	string newString = "";
	
	for(int i = 0; str1[i] ; i++) {
		if(str1[i] >= 'а' and str1[i] <= 'я'){
			currentWord += str1[i];
		} 
		if(str1[i]== ' '){	
			if(!wordsAreEqual(currentWord, toBeRemoved)) {
				changeString(newString, currentWord);
			}
			if(wordsAreEqual(currentWord, toBeRemoved))
				currentWord = "";	
		}
	}
	
	if(!wordsAreEqual(currentWord, toBeRemoved)){
		newString += ' ' + currentWord;
	}
	
	return newString;
}

bool wordsAreEqual(string word1, string words) {	
	int counterWord1 = lettersCounter(word1),
		counterWords = 0;
	string currentWord = "";
	
	bool areEqual = false; 	
	
	for(int i = 0; words[i]; i++) {
		if(words[i] != ' ') {
			counterWords++;
			currentWord += words[i];
		}
		else if(words[i] == ' ' and counterWord1 == counterWords) {
			if(currentWord == word1) {
				areEqual = true;
			}
			currentWord = "";
			counterWords = 0;
		}
		else if(words[i] == ' ' and counterWord1 != counterWords) {
			currentWord = "";
			counterWords = 0;
		}
		else{}			
	}
	return areEqual;
}

int lettersCounter(string c)
{	
	int counter = 0;
	
	for(int i = 0; c[i]; i++){
		counter++;
	}
	
	return counter;
}

void changeString(string &newString, string &currentWord)
{
	if(newString == ""){
		newString += currentWord;
		currentWord = "";
	}					
	else {
		newString += ' ' + currentWord;
		currentWord = "";
	}
}
