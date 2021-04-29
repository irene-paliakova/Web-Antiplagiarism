#include <iostream>
//#include <conio.h>
//#include <Windows.h>
#include <cmath>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <cgicc/CgiDefs.h>
#include <cgicc/Cgicc.h>
#include <cgicc/HTTPHTMLHeader.h>
#include <cgicc/HTMLClasses.h>
using namespace std;
using namespace cgicc;

#define N 99999
#define N_WORD 32
#define TOTAL_WORDS 9999
#define SHINGLE 3
#define TO_BE_REMOVED "чтд кг км м г с а к но и да не или либо же а что чтобы как так, и т.д., и пр., и др. " 
#define SEPARATORS "., !:-—;?\"\'"
#define ENG_LETTERS "AaBCcEeHKkMmOoPpTXxYy"
#define RUS_LETTERS "АаВСсЕеНКкМмОоРрТХхУу"
#define RUS_UPP_TO_LOW_CASE 32

bool isSeparator(char c);
bool isEngLetter(char c);
int strLen(char str[]);
void strCat(char str1[], char str2[]);
void strCpy(char str1[], char str2[]);
void strCpyStrToChar(char str1[], string str2);
int strCmp(char str1[], char str2[]);
void emptyTextArray(char textSplitted[][N_WORD], int numberOfRows);
void splitText(char text[], char textSplitted[][N_WORD], char wordsToBeRemoved[][N_WORD]);
void createWordsToBeRemoved(char toBeRemoved[], char wordsToBeRemoved[][N_WORD]);
void replaceEngLetters(char textAr[]);
void replaceUpperCase(char textAr[]);
bool isWordForTransfer(char word[]);
bool isNumber(char word[]);
bool isToBeRemoved(char word[], char wordsToBeRemoved[][N_WORD]);
int calcTextUnits(char textSplitted[][N_WORD]);
double calcPlagiatRate(char textSplitted[][N_WORD], char fragmentSplitted[][N_WORD]);
double antiPlagiat(string text, string fragment);
string getDB();

int main()
{
    //SetConsoleCP(1251);
    //SetConsoleOutputCP(1251);
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
    	cout << round(antiPlagiat(getDB(), name)) << "\n";
    } else {
    	cout << "Text is not provided!\n";
    }	
    cout << "</p>\n";
    cout << "</body>\n";
    cout << "</html>\n";

    return 0;
}

double antiPlagiat(string text, string fragment) {
    char textAr[N], 
		fragmentAr[N], 
		toBeRemoved[] = TO_BE_REMOVED;
    char textSplitted[TOTAL_WORDS][N_WORD], 
		fragmentSplitted[TOTAL_WORDS][N_WORD], 
		wordsToBeRemoved[TOTAL_WORDS][N_WORD];
    int textUnits = 0, 
		fragmentUnits = 0, 
		toRemoveUnits = 0;

    strCpyStrToChar(textAr, text);
    strCpyStrToChar(fragmentAr, fragment);

    replaceEngLetters(textAr);
    replaceUpperCase(textAr);
    replaceEngLetters(fragmentAr);
    replaceUpperCase(fragmentAr);

    emptyTextArray(textSplitted, TOTAL_WORDS);
    emptyTextArray(fragmentSplitted, TOTAL_WORDS);
    emptyTextArray(wordsToBeRemoved, TOTAL_WORDS);

    createWordsToBeRemoved(toBeRemoved, wordsToBeRemoved);
    splitText(textAr, textSplitted, wordsToBeRemoved);
    splitText(fragmentAr, fragmentSplitted, wordsToBeRemoved);

    double pagiat = calcPlagiatRate(textSplitted, fragmentSplitted);

    /*cout << "Text:" << endl;
    for (int i = 0; i < calcTextUnits(textSplitted); i++)
        cout << i + 1 << ") \"" << textSplitted[i] << "\"" << endl;
    cout << endl<< "Fragment:" << endl;
    for (int i = 0; i < calcTextUnits(fragmentSplitted); i++)
        cout << i + 1 << ") \"" << fragmentSplitted[i] << "\"" << endl;*/

    return pagiat;
}

bool isSeparator(char c)
{
    char separator[] = SEPARATORS;

    for (int i = 0; separator[i] != '\0'; i++)
        if (c == separator[i])
            return true;
    return false;
}

bool isEngLetter(char c)
{
    return ('A' <= c and c <= 'Z') or ('a' <= c and c <= 'z');
}

int strLen(char str[])
{
    int len = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        len++;
    }

    return len;
}

void strCat(char str1[], char str2[])
{
    int len1 = strLen(str1);
    int i;

    for (i = 0; str2[i] != '\0'; i++)
    {
        str1[len1 + i] = str2[i];
    }

    str1[len1 + i] = '\0';
}

void strCpy(char str1[], char str2[])
{
    int i;

    for (i = 0; str2[i] != '\0'; i++)
    {
        str1[i] = str2[i];
    }

    str1[i] = '\0';
}

void strCpyStrToChar(char str1[], string str2)
{
    int i;

    for (i = 0; str2[i] != '\0'; i++)
    {
        str1[i] = str2[i];
    }

    str1[i] = '\0';
}

int strCmp(char str1[], char str2[])
{
    int i = 0;

    for (i = 0; ; i++)
    {
        if (str1[i] > str2[i])
            return 1;

        if (str1[i] < str2[i])
            return -1;

        if (str1[i] == '\0' and str2[i] == '\0')
            return 0;
    }
}

void emptyTextArray(char textSplitted[][N_WORD], int numberOfRows)
{
    for (int i = 0; i < numberOfRows; i++)
        textSplitted[i][0] = '\0';
}

void splitText(char text[], char textSplitted[][N_WORD], char wordsToBeRemoved[][N_WORD])
{
    char word[N] = "";
    int i = 0, iw = 0, iSp = 0;

    for (i = 0; text[i] != '\0'; i++) {
        if (!isSeparator(text[i])) {
            word[iw] = text[i];
            iw++;
            if (isSeparator(text[i + 1]) or text[i + 1] == '\0') {
                word[iw] = '\0';
                iw = 0;
                if (word[0] != '\0' and !isNumber(word) and !isToBeRemoved(word, wordsToBeRemoved)) {
                    strCpy(textSplitted[iSp], word);
                    iSp++;
                }
                word[0] = '\0'; //?
            }
        }
    }
}

void createWordsToBeRemoved(char toBeRemoved[], char wordsToBeRemoved[][N_WORD])
{
    char word[N_WORD] = "";
    int i = 0, iw = 0, iSp = 0;

    for (i = 0; toBeRemoved[i] != '\0'; i++) {
        if (!isSeparator(toBeRemoved[i])) {
            word[iw] = toBeRemoved[i];
            iw++;
            if (isSeparator(toBeRemoved[i + 1]) or toBeRemoved[i + 1] == '\0') {
                word[iw] = '\0';
                iw = 0;
                strCpy(wordsToBeRemoved[iSp], word);
                iSp++;
            }
        }
    }
}

void replaceEngLetters(char textAr[])
{
    char engLetters[] = ENG_LETTERS;
    char rusLetters[] = RUS_LETTERS;
    int i, j;

    for (i = 0; textAr[i] != '\0'; i++)
    {
        if (isEngLetter(textAr[i]))
        {
            for (j = 0; engLetters[j] != '\0'; j++)
            {
                if (textAr[i] == engLetters[j])
                {
                    textAr[i] = rusLetters[j];
                    break;
                }
            }
        }
    }
}

void replaceUpperCase(char textAr[])
{
    for (int i = 0; textAr[i] != '\0'; i++)
    {
        if ('А' <= textAr[i] and textAr[i] <= 'Я')
        {
            textAr[i] = textAr[i] + RUS_UPP_TO_LOW_CASE;
        }
    }
}

bool isWordForTransfer(char word[])
{
    return word[0] != '\0' and !isNumber(word);
}

bool isNumber(char word[])
{
    for (int i = 0; word[i] != '\0'; i++) {
        if (word[i] <= '0' or word[i] >= '9')
            return false;
    }

    return true;
}

bool isToBeRemoved(char word[], char wordsToBeRemoved[][N_WORD])
{
    int numberOfWordsToRemove = calcTextUnits(wordsToBeRemoved);

    for (int i = 0; i < numberOfWordsToRemove; i++)
        if (strCmp(word, wordsToBeRemoved[i]) == 0)
            return true;

    return false;
}

int calcTextUnits(char textSplitted[][N_WORD])
{
    int textUnits = 0;
    for (textUnits = 0; textSplitted[textUnits][0] != '\0'; textUnits++) {}

    return textUnits;
}

double calcPlagiatRate(char textSplitted[][N_WORD], char fragmentSplitted[][N_WORD])
{
    int shinglesTotal = 0, shinglesEquial = 0;
    double plagiat;
    bool isEquial;
    int iFr, iText, i;
    int textUnits = calcTextUnits(textSplitted);
    int fragmentUnits = calcTextUnits(fragmentSplitted);

    for (iFr = 0; iFr <= fragmentUnits - SHINGLE; iFr++)
    {
        for (iText = 0; iText <= textUnits - SHINGLE; iText++)
        {
            isEquial = true;

            for (i = 0; i < SHINGLE; i++)
            {
                if (strCmp(fragmentSplitted[iFr + i], textSplitted[iText + i]) != 0)
                {
                    isEquial = false;
                    break;
                }
            }

            if (isEquial)
            {
                shinglesEquial++;
                break;  
            }
        }

        shinglesTotal++;
    }

    plagiat = 100 - ((double)shinglesEquial / (double)shinglesTotal * 100);

    //cout << "Total: " << shinglesTotal << endl;
    //cout << "Eq: " << shinglesEquial << endl;

    return plagiat;
}

string getDB(){
	std::ifstream in("db.txt");

    string dbText = "", s1;
    while (getline(in, s1))
    	dbText += s1;
  
    in.close();
    
    return dbText;
}
