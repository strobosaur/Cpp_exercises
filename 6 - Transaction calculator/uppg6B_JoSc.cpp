#include <string>
#include <cctype>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cmath>
#include <cstdlib>
using namespace std;

// ====================================================
// DEKLARATION AV KLASSER
// ====================================================

// ====================================================
// TRANSACTION
class Transaction
{
private:
    string date;
    string type;
    string paidBy;
    double sum;
    int personCount;
    string *nameList;

public:
    Transaction();
    ~Transaction();
    Transaction& operator=(const Transaction& t);
    string getPayer();
    string getMoocher(int listIndex);
    double getSum();
    int getPersons();
    bool findName( string friendName );
    bool readTransaction( istream &is );
    void writeTransaction( ostream &os );
    void resize(int n);
};

// ====================================================
// PERSON
class Person
{
private:
    string name;
    string reputation;
    double totalLent;
    double totalDebt;

public:
    Person();
    Person(string inName, double lent, double debt);
    ~Person();
    double getLent();
    double getDebt();
    void printInfo();
    string getName();
};

// ====================================================
// PERSON LIST
class PersonList
{
private:
    int personCount;
    Person *pList;

public:
    PersonList();
    ~PersonList();
    PersonList& operator=(const PersonList& p);
    void addPerson( Person &newPerson );
    void printSummary();
    double allLent();
    double allDebt();
    bool findPerson( const string &name );
    int getPersonCount();
    void printPerson(string inName);
    void resize(int n);
};

// ====================================================
// TRANSACTION LIST
class TransactionList
{
private:
    Transaction *tList;
    int transactionCount;

public:
    TransactionList();
    ~TransactionList();
    void readTransaction( istream &is );
    void writeTransaction( ostream &os );
    void addTransaction( Transaction &t );
    double getSum();
    double getLent( string friendName );
    double getDebt( string friendName );
    int getTransactions();
    PersonList fixPersonList();
    void resize(int n);
};


// ====================================================
// DEKLARATION AV FUNKTIONER
// ====================================================

// ====================================================
// MENU
bool menuMain(TransactionList &tList, PersonList &pList, ifstream &fin, ofstream &fout);
int menuGetIndex(int minChoice, int maxChoice);

// FUNKTION ÖPPNA FIL IN
bool openFileIn(ifstream &fin, string fname);

// FUNKTION ÖPPNA FIL UT
bool openFileOut(ofstream &fout, string fname);

// FORMATTERA STRÄNG TILL FÖRSTA VERSAL, SEDAN GEMENER
void formatName(string &name);


// ====================================================
// ====================================================
// Huvudprogram:
// ====================================================
// ====================================================

int main()
{
    setlocale(LC_ALL, "");

    ifstream fin;
    ofstream fout;

    // VARIABLER
    bool menuRun = 1;
    TransactionList tList;
    PersonList pList;

    cout << endl;

    // LÄS IN KVITTON FRÅN RESA.TXT
    if (openFileIn(fin, "resa.txt"))
    {
        tList.readTransaction(fin);
        fin.close();
    }

    // KÖR MENYN TILLS ANVÄNDAREN AVSLUTAR
    while (menuRun)
    {
        menuRun = menuMain( tList, pList, fin, fout );
    }

    return 0;
}


// ====================================================
// IMPLEMENTATION AV KLASSER
// ====================================================

// ====================================================
// IMPLEMENTATION AV TRANSACTION
// ====================================================

// ====================================================
// CONSTRUCTOR
Transaction::Transaction()
{
    date = "";
    type = "";
    paidBy = "";
    sum = 0;
    personCount = 0;
    nameList = new string[1];

    //cout << "\tNy Transaktion skapad" << endl;
    //cout << endl;
}

// ====================================================
// DESTRUCTOR
Transaction::~Transaction()
{
    delete[] nameList;
    //cout << "\tTransaktion borttagen..." << endl;
    //cout << endl;
}

// ====================================================
// COPY OPERATOR
Transaction& Transaction::operator=(const Transaction& t)
{
    // ÄR T VERKLIGEN ETT ANNAT OBJEKT?
    if (this != &t)
    {
        delete[] nameList;
        date = t.date;
        type = t.type;
        paidBy = t.paidBy;
        sum = t.sum;
        personCount = t.personCount;
        nameList = new string[personCount];

        // KOPIERA ARRAY
        for (int i = 0; i < personCount; i++)
        {
            nameList[i] = t.nameList[i];
        }
    }

    return *this;
}

// ====================================================
// GET PAYER
string Transaction::getPayer()
{
    return paidBy;
}

// ====================================================
// GET MOOCHER
string Transaction::getMoocher(int listIndex)
{
    // KOLLA ATT INPARAMETERN ÄR ETT RIMLIGT INDEX
    if ( (listIndex >= 0) && (listIndex < personCount) && (nameList != 0) )
    {
        return nameList[listIndex];
    }
    else
    {
        return "Error...";
    }
}

// ====================================================
// GET SUM
double Transaction::getSum()
{
    return sum;
}

// ====================================================
// GET NUMBER OF PERSONS
int Transaction::getPersons()
{
    return personCount;
}

// ====================================================
// CHECK FOR NAME IN NAME LIST
bool Transaction::findName( string friendName )
{
    if (personCount > 0)
    {
        // KOLLA I TRANSAKTIONENS NAMNLISTA
        for(int i = 0; i < personCount; i++)
        {
            if (friendName == nameList[i])
            {
                return 1;
            }
        }
    }

    return 0;
}

// ====================================================
// READ TRANSACTION
bool Transaction::readTransaction( istream &is )
{
    // VARIABEL
    string tmpName;
    int tmpCount;

    // INLÄSNING
    is >> date;
    is >> type;
    is >> tmpName;
    is >> sum;

    // FORMATERA NAMN
    formatName(tmpName);
    paidBy = tmpName;

    // LÄS IN TILL NAMNARRAY
    if (is >> tmpCount)
    {
        // FÖRSTORA ARRAY
        resize(tmpCount);

        if ( personCount > 0 )
        {
            // LÄS IN NAMN PÅ SNYLTARNA
            for (int i = 0; i < personCount; i++)
            {
                if (is >> tmpName)
                {
                    // ORDNA FORMATTERING AV NAMNET
                    formatName(tmpName);

                    // SPARA NAMN TILL TRANSAKTIONENS NAMN ARRAY
                    nameList[i] = tmpName;
                }
            }
        }
    }

    // GICK ALLT BRA?
    return !is.fail();
}

// ====================================================
// WRITE TRANSACTION
void Transaction::writeTransaction( ostream &os )
{
    os << std::left
       << setw(8) << date
       << setw(8) << type
       << setw(8) << paidBy
       << setw(8) << sum
       << setw(3) << personCount;

    for (int i = 0; i < personCount; i++)
    {
        os << left << setw(8) << nameList[i];
    }

    os << endl;
}

// ====================================================
// RESIZE ARRAY
void Transaction::resize(int n)
{
    string *temp = 0;

    // OM STORLEK ÄR STÖRRE ÄN 0
    if ( n > 0 )
    {
        // SKAPA ARRAY
        temp = new string[n];

        // KOPIERA ELEMENT
        int i;
        for (i = 0; (i < n) && (i < personCount); i++)
        {
            temp[i] = nameList[i];
        }

        // FYLL ÖVRIGA MED 0
        for( ; i < n; i++)
        {
            temp[i] = "";
        }
    }

    // AVALLOKERA TIDIGARE ARRAY
    delete[] nameList;

    // ERSÄTT MED NY ARRAY
    nameList = temp;

    personCount = n;
}

// ====================================================
// IMPLEMENTATION AV TRANSACTION LIST
// ====================================================

// ====================================================
// CONSTRUCTOR
TransactionList::TransactionList()
{
    transactionCount = 0;
    tList = new Transaction[1];

    /*cout << "\tNy Transaktionslista" << endl
         << endl;*/
}

// ====================================================
// DESTRUCTOR
TransactionList::~TransactionList()
{
    delete[] tList;
    /*cout << "\tTransaktionslista raderad" << endl
         << endl;*/
}

// ====================================================
// LÄS IN TRANSAKTIONER
void TransactionList::readTransaction( istream &is )
{
    int counter = 0;
    Transaction t;

    // LÄS IN SÅ LÄNGE DET GÅR...
    while ( t.readTransaction( is ) )
    {
        addTransaction(t);
        counter++;
    }

    // SKRIV INFO
    cout << "\t================================================" << endl;
    cout << endl;
    cout << "\t" << counter << " st transaktioner har lästs in" << endl;
    cout << endl;
}

// ====================================================
// SKRIV UT TRANSAKTIONER
void TransactionList::writeTransaction( ostream &os )
{
    // FINNS TRANSAKTIONER I LISTAN?
    if (transactionCount > 0)
    {
        for (int i = 0; i < transactionCount; i++)
        {
            tList[i].writeTransaction( os );
        }
    }
    else
    {
        cout << "\tInga transaktioner i listan..." << endl
             << endl;
    }
}

// ====================================================
// LÄGG TILL TRANSAKTION I LISTAN
void TransactionList::addTransaction( Transaction &t )
{
    // FÖRSTORA ARRAY OCH LÄGG TILL NYTT OBJEKT
    resize(transactionCount + 1);
    tList[transactionCount - 1] = t;
}

// ====================================================
// SKICKA UT TOTALSUMMA
double TransactionList::getSum()
{
    // VARIABEL
    double summa = 0;

    // LOOPA IGENOM TRANSAKTIONSLISTAN OCH ADDERA SUMMOR
    for(int i = 0; i < transactionCount; i++)
    {
        summa += tList[i].getSum();
    }

    // SKICKA UT SUMMA
    return summa;
}

// ====================================================
// SKICKA UT EN PERSONS SAMMANLAGDA UTLÄGG
double TransactionList::getLent( string friendName )
{
    // VARIABEL
    double hasLent = 0;

    // LOOPA IGENOM TRANSAKTIONSLISTAN
    for (int i = 0; i < transactionCount; i++)
    {
        // OM PERSONEN FINNS I TRANSAKTIONENS NAMNLISTA
        if ( tList[i].getPayer() == friendName )
        {
            hasLent += ( tList[i].getSum() - ( tList[i].getSum() / static_cast<double>( tList[i].getPersons() + 1 ) ) );
        }
    }

    // SKICKA UT SUMMA
    return hasLent;
}

// ====================================================
// SKICKA UT EN PERSONS SAMMANLAGDA SKULD
double TransactionList::getDebt( string friendName )
{
    // VARIABEL
    double debt = 0;

    // LOOPA IGENOM TRANSAKTIONSLISTAN
    for (int i = 0; i < transactionCount; i++)
    {
        // OM PERSONEN FINNS I TRANSAKTIONENS NAMNLISTA
        if ( tList[i].findName(friendName) )
        {
            debt += ( tList[i].getSum() / static_cast<double>( tList[i].getPersons() + 1 ) );
        }
    }

    // SKICKA SUMMA
    return debt;
}

// ====================================================
// SKICKA UT TOTALT ANTAL TRANSAKTIONER
int TransactionList::getTransactions()
{
    return transactionCount;
}

// ====================================================
// FIXA PERSONLISTA
PersonList TransactionList::fixPersonList()
{
    PersonList pList;
    string tempName;

    double lentSum;
    double debtSum;

    // KOLLA EFTER NYA NAMN I SAMTLIGA KVITTON
    for ( int i = 0; i < transactionCount; i++ )
    {
        // SKAPA PERSON
        Person mrX;

        // KOLLA BLAND BETALARE
        tempName = tList[i].getPayer();
        formatName( tempName );

        // OM NAMNET INTE FINNS I PERSONLISTAN,
        // KOLLA TOTALA BETALNINGAR & SKULDER
        // SKAPA PERSON, OCH LÄGG TILL
        if ( !pList.findPerson( tempName ) )
        {
            // SKAFFA EKONOMISK HISTORIK
            lentSum = getLent( tempName );
            debtSum = getDebt( tempName );

            // SKAPA PERSON OCH LÄGG TILL I LISTAN
            mrX = Person( tempName, lentSum, debtSum );
            pList.addPerson( mrX );
        }

        // KOLLA BLAND SNYLTARE
        for ( int j = 0; j < tList[i].getPersons(); j++ )
        {
            tempName = tList[i].getMoocher( j );
            formatName( tempName );

            // OM NAMNET INTE FINNS I PERSONLISTAN,
            // KOLLA TOTALA BETALNINGAR & SKULDER
            // SKAPA PERSON, OCH LÄGG TILL
            if ( !pList.findPerson( tempName ) )
            {
                // SKAFFA EKONOMISK HISTORIK
                lentSum = getLent( tempName );
                debtSum = getDebt( tempName );

                // SKAPA PERSON OCH LÄGG TILL I LISTAN
                mrX = Person( tempName, lentSum, debtSum );
                pList.addPerson( mrX );
            }
        }
    }

    // SKICKA TILLBAKA LISTA
    return pList;
}

// ====================================================
// RESIZE ARRAY
void TransactionList::resize(int n)
{
    Transaction *temp = 0;

    // OM STORLEK ÄR STÖRRE ÄN 0
    if ( n > 0 )
    {
        // SKAPA ARRAY
        temp = new Transaction[n];

        // KOPIERA ELEMENT
        int i;
        for (i = 0; (i < n) && (i < transactionCount); i++)
        {
            temp[i] = tList[i];
        }

        // FYLL ÖVRIGA MED 0
        for( ; i < n; i++)
        {
            Transaction t;
            temp[i] = t;
        }
    }

    // AVALLOKERA TIDIGARE ARRAY
    delete [] tList;

    // ERSÄTT MED NY ARRAY
    tList = temp;

    transactionCount = n;
}

// ====================================================
// IMPLEMENTATION AV PERSON
// ====================================================

// ====================================================
// CONSTRUCTOR DEFAULT
Person::Person()
{
    name = "";
    reputation = "";
    totalLent = 0;
    totalDebt = 0;

    /*cout << "\tNy person X skapad" << endl
         << endl;*/
}

// ====================================================
// CONSTRUCTOR OVERLOAD I
Person::Person(string inName, double inLent, double inDebt)
{
    name = inName;
    reputation = "";
    totalLent = inLent;
    totalDebt = inDebt;

    /*cout << "\tNy person " << inName << " skapad" << endl
         << endl;*/
}

// ====================================================
// DESTRUCTOR DEFAULT
Person::~Person()
{
    /*cout << "\t" << name << " har avlivats" << endl
         << endl;*/
}

// ====================================================
// SKICKA UT NAMN
string Person::getName()
{
    return name;
}

// ====================================================
// SKICKA UT TOTALA BETALNINGAR
double Person::getLent()
{
    return totalLent;
}

// ====================================================
// SKICKA UT TOTAL SKULD
double Person::getDebt()
{
    return totalDebt;
}

// ====================================================
// SKRIV UT INFO
void Person::printInfo()
{
    // VARIABLER
    double rate = ( (max(totalDebt,1.0)) / (max(totalLent,1.0)) );
    double balance = totalLent - totalDebt;
    //double tempLent, tempDebt, tempBalance;
    string status;

    // FÖR KUL:
    // GE PERSONERNA ETT EPITET
    if (rate > 10)
    {
        status = "   Snål++ | Legendarisk snyltare";
    }
    else if (rate > 4)
    {
        status = "   Snål+  | Går till jobbet för att spara dasspapper";
    }
    else if (rate > 1.25)
    {
        status = "   Snål   | Handlar alla julklappar på Lidl";
    }
    else if (rate > 0.8)
    {
        status = "  Lagom   | Pedantiskt rättvis";
    }
    else if (rate > 0.25)
    {
        status = "Generös   | Lever för dagen";
    }
    else if (rate >= 0.1)
    {
        status = "Generös+  | Tar sms-lån i sina barns namn";
    }
    else if (rate < 0.1)
    {
        status = "Generös++ | Sålde sin njure för lyxkonsumtion";
    }

    // AVRUNDA SUMMORNA TILL FEMTIOÖRINGAR
    /*tempLent = double(int((totalLent * 2) + 0.5)) / 2;
    tempDebt = double(int((totalDebt * 2) + 0.5)) / 2;
    tempBalance = double(int((balance * 2) + 0.5)) / 2;*/

    // SKRIV UT INFO
    cout << setiosflags(ios::fixed) << setprecision(2) << left;
    cout << '\t' << setw(8) << name << right << setw(16) << "ligger ute med: " << right << setw(8) << totalLent << " kr" << endl;
    cout << '\t' << setw(24) << right << "och är skyldig: " << right << setw(8) << totalDebt << left << " kr" << endl;

    // SKALL LÄGGA TILL POTTEN
    if (balance < 0)
    {
        cout << '\t' << setw(24) << right << "Skall lägga: " << right << setw(8) << (balance * -1)
             << left << " kr till potten." << endl;
    }
    // SKALL TA IFRÅN POTTEN
    else
    {
        cout << '\t' << setw(24) << right << "Skall ta: " << right << setw(8) << balance
             << left << " kr ifrån potten." << endl;
    }

    // GE ETT KARAKTÄRSOMDÖMME
    cout << endl;
    cout << '\t' << left << "Rykte: "
         << left << status << endl;
    cout << endl << endl;

    // ÅTERSTÄLL IOS FLAGGOR
    cout << resetiosflags(cout.flags()) << setprecision(8) << endl;
}

// ====================================================
// IMPLEMENTATION AV PERSON LIST
// ====================================================

// ====================================================
// CONSTRUCTOR DEFAULT
PersonList::PersonList()
{
    personCount = 0;
    pList = new Person[1];

    /*cout << "\tNy Personlista skapad" << endl
         << endl;*/
}

// ====================================================
// COPY OPERATOR
PersonList& PersonList::operator=(const PersonList& p)
{
    // ÄR P VERKLIGEN ETT ANNAT OBJEKT?
    if (this != &p)
    {
        // AVALLOKERA GAMMAL ARRAY OM FINNS
        if (pList != 0)
        {
            delete[] pList;
        }

        // KOPIERA VÄRDEN
        personCount = p.personCount;
        pList = new Person[personCount];

        // KOPIERA ARRAY
        for (int i = 0 ; i < personCount ; i++)
        {
            pList[i] = p.pList[i];
        }
    }

    return *this;
}

// ====================================================
// DESTRUCTOR
PersonList::~PersonList()
{
    delete[] pList;
    /*cout << "\tPersonlista raderad" << endl
         << endl;*/
}

// ====================================================
// ADD PERSON
void PersonList::addPerson( Person &newPerson )
{
    // FÖRSTORA ARRAY OCH LÄGG TILL NYTT OBJEKT
    resize(personCount + 1);
    pList[personCount - 1] = newPerson;
}

// ====================================================
// SAMMANSTÄLL ALLA PERSONERS UTLÄGG
double PersonList::allLent()
{
    double lentSum = 0;

    // LOOPA GENOM PERSONLISTA
    for (int i = 0; i < personCount; i++ )
    {
        lentSum += pList[i].getLent();
    }

    // SKICKA TILLBAKA SUMMA
    return lentSum;
}

// ====================================================
// SAMMANSTÄLL ALLA PERSONERS SKULDER
double PersonList::allDebt()
{
    double debtSum = 0;

    // LOOPA GENOM PERSONLISTA
    for (int i = 0; i < personCount; i++ )
    {
        debtSum += pList[i].getDebt();
    }

    // SKICKA TILLBAKA SUMMA
    return debtSum;
}

// ====================================================
// SKRIV UT INFO OCH KOLLA BETALAT MOT SKULDER
void PersonList::printSummary()
{
    // VARIABLER
    double lentSum = allLent();
    double debtSum = allDebt();
    int balance;

    // LOOPA IGENOM PERSONLISTAN
    for (int i = 0; i < personCount; i++)
    {
        // SKRIV UT INFO
        pList[i].printInfo();
    }

    // GÅR SIFFORNA JÄMT UT?
    // UTLÄGG - SKULDER, AVRUNDAT TILL HELTAL
    balance = ( lentSum - debtSum ) + 0.5;

    // SKRIV UT VARNING OM DET DIFFAR
    if (balance != 0)
    {
        cout << "\tVarning! Totala Utlägg och Skulder går inte jämt ut" << endl;
        cout << "\tDifferens: " << balance << " kr" << endl;
        cout << endl;
    }
}

// ====================================================
// SÖK EFTER NAMN I PERSONLISTAN
bool PersonList::findPerson( const string &name )
{
    // INGA PERSONER I LISTAN
    if ( (personCount > 0) && (pList != 0) )
    {
        // SÖK IGENOM LISTAN EFTER NAMNET
        for (int i = 0; i < personCount; i++)
        {
            if ( pList[i].getName() == name )
            {
                return 1;
            }
        }
    }

    return 0;
}

// ====================================================
// SKICKA ANTAL PERSONER I LISTAN
int PersonList::getPersonCount()
{
    return personCount;
}

// ====================================================
// SKRIV UT INFO FÖR EN PERSON
void PersonList::printPerson(string inName)
{
    // KOLLA EFTER GIVET NAMN I LISTAN
    for (int i = 0; i < personCount; i++)
    {
        if (pList[i].getName() == inName)
        {
            pList[i].printInfo();
        }
    }
}

// ====================================================
// RESIZE ARRAY
void PersonList::resize(int n)
{
    Person *temp = 0;

    // OM STORLEK ÄR STÖRRE ÄN 0
    if ( n > 0 )
    {
        // SKAPA ARRAY
        temp = new Person[n];

        // KOPIERA ELEMENT
        int i;
        for (i = 0; (i < n) && (i < personCount); i++)
        {
            temp[i] = pList[i];
        }

        // FYLL ÖVRIGA MED 0
        for( ; i < n; i++)
        {
            Person p;
            temp[i] = p;
        }
    }

    // AVALLOKERA TIDIGARE ARRAY
    delete[] pList;

    // ERSÄTT MED NY ARRAY
    pList = temp;

    personCount = n;
}

// ====================================================
// DEFINITION AV FUNKTIONER
// ====================================================

// ====================================================
// FUNKTION ÖPPNA FIL IN
bool openFileIn(ifstream &fin, string fname)
{
    fin.open( fname.c_str() );
    if ( fin )
    {
        return 1;
    }
    else
    {
        cout << "\tFilen kunde inte hittas" << endl;
        cout << endl;

        return 0;
    }
}

// ====================================================
// FUNKTION ÖPPNA FIL UT
bool openFileOut(ofstream &fout, string fname)
{
    fout.open( fname.c_str() );
    if ( fout )
    {
        return 1;
    }
    else
    {
        cout << "\tFilen kunde inte hittas" << endl;
        cout << endl;

        return 0;
    }
}

// ====================================================
// MENU GET INDEX
int menuGetIndex(int minChoice, int maxChoice)
{
    int choice;

    cout << "\tMenyval (" << minChoice << " - " << maxChoice << "): ";

    // LOOPA TILLS KORREKT INPUT
    while ( !(cin >> choice) || (choice < minChoice) || (choice > maxChoice) )
    {
        cin.clear();
        cin.ignore(256, '\n');
        cout << "\tMenyval (" << minChoice << " - " << maxChoice << "): ";
    }

    // PLOCKA RETUR
    cin.get();
    cout << endl;

    // SKICKA INDEX
    return choice;
}

// ====================================================
// MENU MAIN
bool menuMain(TransactionList &tList, PersonList &pList, ifstream &fin, ofstream &fout)
{
    int index = 0;
    double sumLent, sumDebt, sumTot, balance;
    string inName;

    Transaction nyTrans;

    // SKRIV UT MENYTEXT
    cout << "\t================================================" << endl;
    cout << endl;
    cout << "\t* MENY *" << endl;
    cout << endl;
    cout << "\tTotalt finns " << tList.getTransactions() << " transaktioner i listan." << endl;
    cout << endl;
    cout << "\t0. Avsluta - Transaktioner sparas till \'kvitto_default.txt\'" << endl;
    cout << "\t1. Läs in transaktion från tangentbordet" << endl;
    cout << "\t2. Skriv ut information om alla transaktioner" << endl;
    cout << "\t3. Beräkna totala kostnaden" << endl;
    cout << "\t4. Hur mycket är en viss person skyldig?" << endl;
    cout << "\t5. Hur mycket ligger en viss person ute med?" << endl;
    cout << "\t6. Lista alla personer mm och FIXA!!!" << endl;
    cout << endl;

    // VÄLJ NÄSTA MENYINDEX
    index = menuGetIndex(0,6);

    // EXEKVERA VALT MENYINDEX
    switch (index)
    {
    case (0):
        // AVSLUTA
        // SKRIV KVITTON, OM DET FINNS NÅGRA
        if ( openFileOut(fout, "kvitto_default.txt") && (tList.getTransactions() > 0) )
        {
            tList.writeTransaction(fout);
            fout.close();

            cout << "\t================================================" << endl;
            cout << endl;
            cout << "\t" << tList.getTransactions() << " st transaktioner sparade till kvitto_default.txt" << endl;
            cout << endl;
        }

        // BREAK
        break;

    case (1):
        // LÄS IN TRANSAKTION FRÅN TANGENTBORD
        cout << "\tDatum, Typ, Betalare, Summa, Snyltare, Namn, (Namn, etc)" << endl;
        cout << "\t";

        // LÄS IN TRANSAKTIONSDATA
        nyTrans.readTransaction(cin);
        cin.clear();
        cin.ignore(512, '\n');

        // LÄGG TILL INLÄST TRANSAKTION I TRANSAKTIONSLISTAN
        tList.addTransaction(nyTrans);

        // BREAK
        break;

    case (2):
        // SKRIV UT TRANSAKTIONER TILL SKÄRM
        cout << "\t================================================" << endl;
        cout << endl;

        tList.writeTransaction(cout);

        cout << endl;

        // BREAK
        break;

    // SKRIV UT TOTALKOSTNAD
    case (3):

        sumTot = tList.getSum();

        // SKRIV UT SUMMERING AV PERSONER VIA PERSONLISTAN
        cout << "\t================================================" << endl;
        cout << endl;

        // SKRIV UT TOTALSUMMA
        cout << setiosflags(ios::fixed) << setprecision(2) << right;
        cout << '\t' << setw(16) << "Total kostnad: " << setw(8) << sumTot << " kr" << endl;
        cout << resetiosflags(cout.flags()) << setprecision(8) << endl;

        // BREAK
        break;

    // SKRIV UT PERSONS SKULDER TILL SKÄRM
    case (4):

        // SKRIV IN NAMN
        cout << "\tNamn: ";
        cin >> inName;
        cin.clear();
        cin.ignore(256, '\n');
        cout << endl;

        formatName(inName);

        // SKAFFA VÄRDENA FRÅN TRANSAKTIONSLISTAN
        sumLent = tList.getLent(inName);
        sumDebt = tList.getDebt(inName);
        balance = sumLent - sumDebt;

        // OM PERSONEN HAR SKULDER, SKRIV UT
        if (balance < 0)
        {
            // SKRIV UT INFO OM SKULDER
            cout << "\t================================================" << endl;
            cout << setiosflags(ios::fixed) << setprecision(2) << endl;
            cout << '\t' << right << setw(12) << inName << endl;
            cout << '\t' << right << setw(14) << "Skall ge: " << setw(8) <<  (balance * -1) << " kr till potten" << endl;
            cout << resetiosflags(cout.flags()) << setprecision(8) << endl;
        }
        else
        {
            // ANNARS SKRIV UT INFO OM SKULDFRIHET...
            cout << "\t================================================" << endl;
            cout << setiosflags(ios::fixed) << setprecision(2) << endl;
            cout << '\t' << right << setw(12) << inName << endl;
            cout << '\t' << right << setw(14) << "Har inga skulder..." << endl;
            cout << resetiosflags(cout.flags()) << setprecision(8) << endl;
        }

        // BREAK
        break;

    // SKRIV UT PERSONS SKULDER TILL SKÄRM
    case (5):

        // SKRIV IN NAMN
        cout << "\tNamn: ";
        cin >> inName;
        cin.clear();
        cin.ignore(256, '\n');
        cout << endl;

        formatName(inName);

        // SKAFFA VÄRDENA FRÅN TRANSAKTIONSLISTAN
        sumLent = tList.getLent(inName);
        sumDebt = tList.getDebt(inName);
        balance = sumLent - sumDebt;

        // OM PERSONEN HAR SKULDER, SKRIV UT
        if (balance > 0)
        {
            // SKRIV UT INFO OM UTLÄGG
            cout << "\t================================================" << endl;
            cout << setiosflags(ios::fixed) << setprecision(2) << endl;
            cout << '\t' << right << setw(12) << inName << endl;
            cout << '\t' << right << setw(14) << "Skall ta: " << setw(8) <<  balance << " kr ifrån potten" << endl;
            cout << resetiosflags(cout.flags()) << setprecision(8) << endl;
        }
        else
        {
            // ANNARS SKRIV UT INFO OM SKULDFRIHET...
            cout << "\t================================================" << endl;
            cout << setiosflags(ios::fixed) << setprecision(2) << endl;
            cout << '\t' << right << setw(12) << inName << endl;
            cout << '\t' << right << setw(14) << "Skall inte ta något ifrån potten..." << endl;
            cout << resetiosflags(cout.flags()) << setprecision(8) << endl;
        }

        // BREAK
        break;

    // SKRIV UT OCH FIXA
    case (6):
        // FIXA PERSONLISTAN
        pList = tList.fixPersonList();

        // SKRIV UT SUMMERING AV PERSONER VIA PERSONLISTAN
        cout << "\t================================================" << endl;
        cout << endl;

        pList.printSummary();

        // BREAK
        break;

    }

    //AVSLUTA
    return index;
}

// =======================================================
// FORMATTERA STRÄNG TILL FÖRSTA VERSAL, SEN GEMENER
void formatName(string &name)
{
    int length = name.length();

    // FORMATTERA NAMN TILL "Namn"
    if ( length > 0 )
    {
        // HELA STRÄNGEN GEMENER
        for (int i = 1 ; i < length ; i++)
        {
            name.at(i) = tolower(name.at(i));
        }

        // FÖRSTA TECKNET VERSAL
        name.at(0) = toupper(name.at(0));
    }
}