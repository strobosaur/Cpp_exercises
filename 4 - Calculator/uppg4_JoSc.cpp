#include <iostream>

using namespace std;

// =========================================================
// FUNCTION IMPLEMENTATIONS
// =========================================================

// FUNKTION FÖR ATT SKRIVA UT BASINFO
void printInfo()
{
    cout << endl << endl;
    cout << "  Det här programmet utför beräkningar med två" << endl;
    cout << "  av dig valda tal. När du skrivit in talen" << endl;
    cout << "  kommer du få välja en av tre operationer: " << endl;
    cout << endl;
    cout << "   a = addition" << endl;
    cout << "   m = multiplikation" << endl;
    cout << "   k = kvadratsummering" << endl;
    cout << endl;
    cout << "  Programmet kommer därefter att ordna dina tal " << endl;
    cout << "  i storleksordning och utföra den valda operationen " << endl;
    cout << "  på det minsta angivna talet, samt på varje efterföljande" << endl;
    cout << "  heltal upp till och med det största av de angivna talen." << endl;
    cout << endl;
    cout << "   a adderar med nästkommande tal, m multiplicerar med" << endl;
    cout << "   nästkommande tal, och k lägger samman kvadraten av" << endl;
    cout << "   det mindre talet med kvadraten av nästkommande tal, osv" << endl;
    cout << endl;
    cout << "  =========================================================" << endl;
    cout << endl;
}

// FUNKTION FÖR ATT MATA IN TAL
int inputNr()
{
    int getNr;

    cout << "  Skriv in ett heltal: ";
    cin >> getNr;
    cout << endl;
    cout << "  =========================================================" << endl;
    cout << endl;

    return getNr;
}

// FUNKTION FÖR ATT BESTÄMMA OPERATION
char inputOp()
{
    char getOp = 'x';

    cout << "  Välj en av följande operationer:" << endl;
    cout << endl;
    cout << "   (a)ddition" << endl;
    cout << "   (m)ultiplikation" << endl;
    cout << "   (k)vadratsummering" << endl;
    cout << endl;

    // SKRIV IN OPERATION + KOLLA OM DEN ÄR VALID
    while (getOp == 'x')
    {
        cout << "  Operation: ";
        cin >> getOp;

        // KOLLA OM VALD OPERATION ÄR OK
        if ((getOp != 'a') && (getOp != 'm') && (getOp != 'k'))
        {
            getOp = 'x';

            cout << endl << endl;
            cout << "  Error. Skriv in 'a' , 'm' eller 'k'...";
        }

        cout << endl << endl;
    }

    // SKICKA TILLBAKA VALD OPERATION
    return getOp;
}

// FUNKTION FÖR ATT ORDNA DE TVÅ TALEN I STORLEKSORDNING
void sortNr(int a, int b, int &varA, int &varB)
{
    // SKRIV ÖVER VARIABLER OM A ÄR STÖRRE ÄN B
    if (a > b)
    {
        varA = b;
        varB = a;
    }
}

// FUNKTION FÖR ATT UTFÖRA VALD BERÄKNING
int doMath(int a, int b, char op)
{
    // DEKLARERA VARIABLER
    long returnSum = 0;

    // MULTIPLIKATIONEN KAN INTE BÖRJA MED 0
    if (op == 'm')
    {
        returnSum = 1;
    }

    // UTFÖR OPERATIONEN FÖR VARJE HELTAL
    for(int i = a; i <= b; i++)
    {
        // ADDITION
        if (op == 'a')
        {
            returnSum += i;
        }

        // MULTIPLIKATION
        if (op == 'm')
        {
            returnSum *= i;
        }

        // KVADRATSUMMERING
        if (op == 'k')
        {
            returnSum += (i*i);
        }
    }

    // SKICKA TILLBAKA RESULTATET
    return returnSum;
}

// FUNKTION FÖR ATT REDOVISA SLUTRESULTAT
void printResult(long endResult)
{
    cout << "  =========================================================" << endl;
    cout << endl;
    cout << "  Slutresultat: " << endResult;
    cout << endl << endl << endl;
}

// =========================================================
// MAIN
// =========================================================

int main()
{
    setlocale(LC_ALL, "");

    // DEKLARERA VARIABLER
    int lowNr, highNr;
    long finalSum;
    char chosenOp;

    // SKRIV INFO
    printInfo();

    // HÄMTA TAL
    lowNr = inputNr();
    highNr = inputNr();

    // VÄLJ OPERATION
    chosenOp = inputOp();

    // SORTERA TAL
    sortNr(lowNr, highNr, lowNr, highNr);

    // TROLLERI
    finalSum = doMath(lowNr, highNr, chosenOp);

    // REDOVISA RESULTAT
    printResult(finalSum);

    return 0;
}
