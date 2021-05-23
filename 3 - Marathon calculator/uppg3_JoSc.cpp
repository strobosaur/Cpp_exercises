#include <iostream>

using namespace std;

// =============================================
// FUNCTION IMPLEMENTATIONS
// =============================================

// FUNKTION FÖR ATT HÄMTA IN TID
int getTime()
{
    // DEKLARERA VARIABLER
    int hrs = -1,
        mins = -1,
        secs = -1,
        totalSecs = -1;

    while (totalSecs < 0)
    {
        // 1. HÄMTA TIMMAR
        while (hrs < 0)
        {
            cout << "  Timmar: ";
            cin >> hrs;

            if ((hrs < 0) || (hrs > 23))
            {
                cout << "Kan du inte klockan?" << endl;
                hrs = -1;
            }
        }

        // 2. HÄMTA MINUTER
        while (mins < 0)
        {
            cout << "  Minuter: ";
            cin >> mins;

            if ((mins < 0) || (mins > 59))
            {
                cout << "Kan du inte klockan?" << endl;
                mins = -1;
            }
        }

        // 3. HÄMTA SEKUNDER
        while (secs < 0)
        {
            cout << "  Sekunder: ";
            cin >> secs;

            if ((secs < 0) || (secs > 59))
            {
                cout << "Kan du inte klockan?" << endl;
                secs = -1;
            }
        }

        // 4. BERÄKNA TOTALTID I SEKUNDER
        totalSecs = (hrs * 3600) + (mins * 60) + secs;

        cout << endl;
    }

    // SKICKA TILLBAKA TID I SEKUNDER
    return totalSecs;
}

// =============================================
// HUVUDPROGRAMMET
// =============================================

int main()
{
    // DEKLARERA KONSTANT, SEKUNDER PÅ ETT DYGN
    const int daySecs = (24*60*60);

    // DEKLARERA VARIABLER
    int runnerNr = 1,
        masterNr = -1,
        masterTime = daySecs * 365,
        runnerTotal = 0,
        masterHrs = 0,
        masterMins = 0,
        masterSecs = 0,
        startTime,
        endTime,
        totTime;

    // SKRIV UT BASINFO
    cout << endl << endl ;
    cout << "     Välkommen till löptidskalkylatorn!" << endl << endl;
    cout << " Mata in nummer, starttid och sluttid för" << endl;
    cout << " loppets deltagare." << endl << endl;
    cout << " Vinnaren kommer att meddelas när du" << endl;
    cout << " matar in ett löparnummer lägre än 1." << endl << endl;
    cout << " ===================================" << endl;

    // SAMLA IN LÖPARDATA
    while (runnerNr > 0)
    {
        // NOLLSTÄLL VARIABLER
        startTime = 0;
        endTime = 0;
        totTime = 0;

        // MATA IN LÖPARENS NUMMER
        cout << endl;
        cout << "      * Deltagare nr. " << (runnerTotal + 1) << " *" << endl;
        cout << endl << " Ange löparens nummer: ";
        cin >> runnerNr;

        if (runnerNr >= 1)
        {
            runnerTotal++;

            // MATA IN STARTTID
            cout << endl;
            cout << " ===================================" << endl;
            cout << endl << " Ange löparens starttid: " << endl << endl;
            startTime = getTime();

            // MATA IN SLUTTID
            cout << endl;
            cout << " ===================================" << endl;
            cout << endl << " Ange löparens sluttid: " << endl << endl;
            endTime = getTime();

            // KONTROLLERA TID PÅ DYGNET
            if (endTime < startTime)
            {
                endTime += daySecs;
            }

            // RÄKNA UT TOTALTID
            totTime = (endTime - startTime);

            // KOLLA OM DET ÄR NYTT REKORD
            if (totTime < masterTime)
            {
                masterNr = runnerNr;
                masterTime = totTime;
            }

            // BEKRÄFTA ATT DENNA LÖPARE HAR REGISTRERATS
            cout << endl;
            cout << " ===================================" << endl;
            cout << endl;
            cout << " * Löpare nr. " << runnerNr << " har registrerats! *" << endl;
            cout << endl;
            cout << " ===================================" << endl << endl;
            cout << " Registrera nästa deltagare, " << endl << " eller mata in 0 för sammanställning." << endl;
        }
    }

    // KOLLA OM VI HAR NÅGON ANVÄNDBAR DATA
    if ((masterNr < 1) || (runnerTotal < 1))
    {
        cout << endl << endl;
        cout << " ===================================" << endl;
        cout << " Något måste gått fel, försök igen!" << endl;
        cout << " ===================================" << endl << endl << endl;

        return 0;
    }

    // RÄKNA OM REKORDTIDEN TILL TIMMAR MINUTER OCH SEKUNDER
    masterSecs = ((masterTime % 3600) % 60);
    masterMins = ((masterTime % 3600) - masterSecs) / 60;
    masterHrs =  (masterTime - (masterMins * 60) - masterSecs) / 3600;

    // REDOVISA SLUTRESULTAT
    cout << endl << endl;
    cout << " ===================================" << endl << endl;
    cout << " Loppets vinnare är löpare nummer " << masterNr << "," << endl;
    cout << " med den helt ofattbara tiden: " << endl << endl;
    cout << "  " << masterHrs << " timmar" << endl;
    cout << "  " << masterMins << " minuter" << endl;
    cout << "  " << masterSecs << " sekunder" << endl << endl;
    cout << " loppet hade sammanlagt " << runnerTotal << " deltagare," << endl;
    cout << " men ingen kunde mäta sig med" << endl;
    cout << " mästaren, nr " << masterNr << "!" << endl;
    cout << endl ;
    cout << " ===================================" << endl;
    cout << " =          # GAME OVER #          =" << endl;
    cout << " ===================================" << endl;
    cout << endl << endl << endl << endl;

    return 0;
}
