

#include <string>
#include <cctype>
#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

    // TÄNKTE PRÖVA EN ENUM
    enum class Language
    {
        english,
        french,
        swedish,
        german,
        max_languages
    };

// GLOBAL CONSTANTS
const int ANTAL_BOKSTAVER = 26;  //A-Z
const int ANTAL_SPRAK = 4;

// GLOBAL ARRAY HISTOGRAM
const double TOLK_HJALP[ANTAL_SPRAK][ANTAL_BOKSTAVER]=
       {{8.27,1.48,2.94,4.03,11.78,2.22,1.72,6.77, //engelska
         7.39,0.12,0.81,3.76,2.85,6.71,7.79,1.54,
         0.05,5.95,6.69,9.07,2.66,1.13,2.14,0.19,
         1.89,0.03},
        {7.97,1.40,3.55,3.79,16.89,1.02,1.00,0.75, //franska
         7.08,0.38,0.04,5.51,2.82,8.11,5.19,2.78,
         1.01,6.69,8.35,7.22,6.09,1.35,0.02,0.54,
         0.30,0.15},
        {9.50,1.11,1.53,5.30,8.94,1.74,3.57,3.94,  //svenska
         3.98,0.89,3.26,4.93,3.41,8.46,5.01,1.77,
         0.00,6.73,5.56,9.20,1.94,2.42,0.00,0.05,
         0.45,0.00},
        {5.12,1.95,3.57,5.07,16.87,1.35,3.00,5.79, //tyska
         8.63,0.19,1.14,3.68,3.12,10.64,1.74,0.42,
         0.01,6.30,6.99,5.19,3.92,0.77,1.79,0.01,
         0.69,1.24}};

// =======================================================
// CLASS DECLARATIONS
class Text
{
    private:

        // PRIVATA VARIABLER
        int charFreqAbs[ANTAL_BOKSTAVER] = {}, charCount;
        double charFreqRel[ANTAL_BOKSTAVER] = {}, similarities[ANTAL_SPRAK] = {};
        string fullText;

        // EGET BEKVÄMLIGHETSTILLÄGG, OFFSETVARIABEL TILL ARRAY
        const int charOffset = static_cast<int>('a');

    public:

        // KONSTRUKTOR
        Text();

        // ÄNDRA KLASSENS TEXTVARIABEL
        void setText(string nyText);

        // BERÄKNA ABSOLUT HISTOGRAM
        bool beraknaHistogramAbs();

        // SKRIV UT ABSOLUT HISTOGRAM
        void skrivHistogramAbs();

        // BERÄKNA RELATIVT HISTOGRAM
        void absTillRel();

        // PLOTTA RELATIVT HISTOGRAM
        void plottaHistogramRel();

        // BERÄKNA ABSOLUT OCH RELATIVT HISTOGRAM
        void beraknaHistogram();

        // TOLKA RELATIVT HISTOGRAM
        string tolka();

        // KOM ÅT VÄRDEN I ARRAY LIKHETER
        double getSum(int index);
};

// =======================================================
// FUNCTION DECLARATIONS

// Funktionen namn_pa_fil
string namn_pa_fil();

// Funktionen inlasning
string inlasning(string filename);

// REDOVISA RESULTAT
void redovisa(string probLanguage, Text object);

// ============================================================
// ============================================================
// ============================================================

// MAIN

int main()
{
    setlocale(LC_ALL, "");

    // Deklarera variabler
    string inputText,filnamn,svar;
    bool histOK;

    // SKAPA TEXTOBJEKT
    Text minText;

    // LÄS IN FILNAMNET
    filnamn = namn_pa_fil();

    // LÄS IN FILEN
    inputText = inlasning( filnamn );

    // SKICKA INLÄST TEXT TILL OBJEKTET
    minText.setText( inputText );

    // BERÄKNA HISTOGRAM
    minText.beraknaHistogram();

    // TOLKA HISTOGRAM
    svar = minText.tolka();

    // REDOVISA RESULTAT
    redovisa( svar, minText );

    // PLOTTA HISTOGRAM
    minText.plottaHistogramRel();

    return 0;
}

// =======================================================
// =======================================================
// =======================================================

// CLASS IMPLEMENTATIONS

// =======================================================

// TEXT
// =======================================================
// KONSTRUKTOR
Text::Text()
{
    // NOLLA VARIABLER
    fullText = "";
    charCount = 0;

    // NOLLA ARRAYER
    for (int i = 0; i < ANTAL_BOKSTAVER; i++)
    {
        charFreqAbs[i] = 0;
        charFreqRel[i] = 0;
    }

    // SÄG HEJ
    cout << endl;
    cout << '\t' << "Nytt objekt av klassen 'Text' skapat" << endl << endl;
}

// =======================================================
// SET TEXT
void Text::setText( string nyText )
{
    fullText = nyText;
}

// =======================================================
// BERÄKNA HISTOGRAM
bool Text::beraknaHistogramAbs()
{
    // DEKLARERA VARIABLER
    int charIndex;
    char currentChar;

    // LOOPA IGENOM DEN INMATADE TEXTEN
    // LAGRA ANTALET BOKSTÄVER I HISTOGRAM ARRAY
    for(int i = 0; i < fullText.length(); i++)
    {
        // AKTUELL BOKSTAV -> LOWER CASE
        currentChar = tolower( fullText.at(i) );

        // BERÄKNA INDEX I HISTOGRAM ARRAY
        charIndex = ( static_cast<int>(currentChar) ) - charOffset;

        // OM TECKNET FINNS MELLAN A-Z
        // UPPDATERA HISTOGRAM ARRAY & ANTAL TECKEN
        if ( (charIndex >= 0) && (charIndex < ANTAL_BOKSTAVER) )
        {
            charFreqAbs[charIndex]++;
            charCount++;
        }
    }

    // FICK VI NÅGON TEXT?
    if (charCount > 0) { return 1; }
    else { return 0; }
}

// =======================================================
// SKRIV HISTOGRAM
void Text::skrivHistogramAbs()
{
    // DEKLARERA VARIABLER
    char letter;

    // LOOPA GENOM ARRAY OCH SKRIV UT BOKSTAV + FREKVENS
    for ( int i = 0; i < ANTAL_BOKSTAVER; i++ )
    {
        letter = toupper( static_cast<char>(i + charOffset) );
        cout << '\t' << letter << ": " << charFreqAbs[i] << endl;
    }

    // SKRIV UT TOTALT ANTAL BOKSTÄVER
    cout << endl;
    cout << '\t' << "Totalt antal bokstäver: " << charCount << endl;
    cout << endl;
}

// =======================================================
// ABSOLUT TILL RELATIVT
void Text::absTillRel()
{
    // LOOPA GENOM ARRAY + BERÄKNA RELATIV FREKVENS
    for ( int i = 0; i < ANTAL_BOKSTAVER; i++ )
    {
        // FREKVENSEN SPARAS SOM PROCENT
        charFreqRel[i] = ( (double)charFreqAbs[i] / charCount ) * 100;
    }
}

// =======================================================
// PLOTTA RELATIVT HISTOGRAM
void Text::plottaHistogramRel()
{
    // DEKLARERA VARIABLER
    char letter;
    int rpt;

    // SKRIV UT ANTAL TECKEN
    cout << '\t' << "Totalt har " << charCount << " tecken lästs in." << endl;
    cout << endl;
    cout << '\t' << "Bokstavsfördelning:" << endl;
    cout << endl;

    // LOOPA GENOM ARRAY OCH SKRIV UT BOKSTAV + FREKVENS
    for (int i = 0; i < ANTAL_BOKSTAVER; i++)
    {
        // RÄKNA ANTAL REPETITIONER
        rpt = (int)( (charFreqRel[i] * 2) + 0.5 );

        // SKRIV UT BOKSTAV UPPER CASE
        letter = toupper( static_cast<char>(i + charOffset) );
        cout << '\t' << letter << " ";

        // SKRIV UT SYMBOLER
        while (rpt > 0)
        {
            cout << '*';
            rpt--;
        }

        cout << endl;
    }
}

// =======================================================
// BERÄKNA HISTOGRAM
void Text::beraknaHistogram()
{
    // BERÄKNA ABSOLUT HISTOGRAM
    // OM ALLT OK, ÄVEN DET RELATIVA
    if ( beraknaHistogramAbs() )
    {
        absTillRel();
    }
    else
    {
        cout << '\t' << "Histogrammet innehåller inga bokstäver..." << endl;
    }
}

// =======================================================
// TOLKA HISTOGRAM
string Text::tolka()
{
    // VARIABLER
    double refFreq,curFreq;
    int bestMatch = 0;
    string probLanguage = "Voynich";

    // LOOPA ANTAL SPRÅK
    for(int i = 0; i < ANTAL_SPRAK; i++)
    {
        // LOOPA ANTAL TECKEN
        // LAGRA KVADRATSUMMAN AV SKILLNADERNA
        // FÖR VARJE SPRÅK I EXTERN ARRAY
        for(int j = 0; j < ANTAL_BOKSTAVER; j++)
        {
            refFreq = TOLK_HJALP[i][j];
            curFreq = charFreqRel[j];

            similarities[i] += ((refFreq - curFreq) * (refFreq - curFreq));
        }

        // KOLLA EFTER LÄGSTA KVADRATSUMMA
        if (similarities[i] < similarities[bestMatch])
        {
            bestMatch = i;
        }
    }

    // FIXA STRING MED SPRÅKETS NAMN
    switch (bestMatch)
    {
        case (int)Language::english:
        probLanguage = "Engelska";
        break;

        case (int)Language::french:
        probLanguage = "Franska";
        break;

        case (int)Language::swedish:
        probLanguage = "Svenska";
        break;

        case (int)Language::german:
        probLanguage = "Tyska";
        break;
    }

    return probLanguage;
}

// =======================================================
// KOM ÅT VÄRDEN I ARRAY LIKHETER
double Text::getSum(int index)
{
    // SKICKA VÄRDEN FRÅN ARRAY[INDEX]
    if ( (index >= 0) && (index < ANTAL_SPRAK) )
    {
        return similarities[index];
    }
    else
    {
        return 0;
    }
}

// =======================================================
// =======================================================
// =======================================================

// FUNCTION IMPLEMENTATIONS

// =======================================================
// FUNKTION HÄMTA NAMN PÅ FIL
string namn_pa_fil()
{
    // VARIABLER
    string filename, filetype = ".txt";

    // SKRIV IN FILNAMN
    cout << '\t' << "Skriv in filnamn: ";

    getline(cin, filename);

    cout << endl;
    cout << "  ==============================================================" << endl << endl;

    // KOLLA OM FILTYP ÄR .TXT
    if ( (filename.rfind(filetype) == string::npos) || (filename.rfind(filetype) < ( filename.length() - 4 ) ) )
    {
        // LÄGG TILL .TXT
        filename.append(filetype);
    }

    // SKICKA TILLBAKA STRING MED FILNAMNET
    return filename;
}

// =======================================================
// FUNKTION LÄS IN FIL
string inlasning(string filename)
{
    // VARIABLER
    string fileText;

    // LÄS IN FIL
    ifstream fin( filename.c_str() );

    // KOLLA OM ALLT GICK BRA
    if (!fin)
    {
        // FILEN FINNS INTE...
        cout << '\t' << "Filen \'" << filename << "\' kunde inte hittas..." << endl << endl;
        exit (EXIT_FAILURE);
    }
    else
    {
        // FILEN FINNS!
        // LÄS IN FILENS TEXT TECKEN FÖR TECKEN TILL EOB
        char c;
        fin.get(c);

        while ( !fin.eof() )
        {
            fileText += c;
            fin.get(c);
        }
    }

    // SKICKA TILLBAKA FILTEXTEN
    return fileText;
}

// =======================================================
// FUNKTION REDOVISA
void redovisa(string probLanguage, Text object)
{
    // REDOVISA RESULTAT TILL ANVÄNDAREN
    cout << '\t' << "Följande skillnadskvoter kan konstateras" << endl;
    cout << endl;
    cout << '\t' << "Engelska: " << object.getSum((int)Language::english) << endl;
    cout << '\t' << "Franska: " << object.getSum((int)Language::french) << endl;
    cout << '\t' << "Svenska: " << object.getSum((int)Language::swedish) << endl;
    cout << '\t' << "Tyska: " << object.getSum((int)Language::german) << endl;
    cout << endl;
    cout << '\t' << "Det är troligast att texten är skriven på " << probLanguage << "." << endl;
    cout << endl;
}