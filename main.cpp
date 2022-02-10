#include <iostream>
#include <string>
#include <ctime> // do generowania seed
#include <random> // losowanie
#include <windows.h>  // do dziwkeu
#include <fstream> // pliki

using namespace std;

// definicja kolorow do wyswietlania w konsoli (ansi escape code)
const string reset = "\u001B[0m";
const string czerwony = "\u001B[31m";
const string zielony = "\u001B[32m";
const string zolty = "\u001B[33m";
const string niebieski = "\u001B[36m";

const int rozmiarPlanszy = 11;

struct Pole {
    int x;
    int y;
};

struct PStartowe {
    int x;
    int y;
};

struct Gracz {
    int x;
    int y;
    int druzyna;
    int indeks;
    int id;
    string nazwa;
};

// definicje funkcji
void stworzPlansze(char plansza[][rozmiarPlanszy]);
void menu(int& wyborMenu);
void stworzGre(Pole pole[40], PStartowe pstart[][4], Gracz gracz[4][4]);
int graj(char plansza[][rozmiarPlanszy], Pole pole[40], PStartowe pstart[][4], Gracz gracz[4][4], int iloscGraczy);
void wyswietlAktualnaPlansze(char plansza[][rozmiarPlanszy], Gracz gracz[4][4], int iloscGraczy);
string kolorGracza(int id);
int indeksPola(int ruch);
void doBazy(Gracz gracz[4][4], int ruch, int wyborPionka);
bool czyDoBazy(int ruch, int indeks);

int main() {

    int wyborMenu{};
    int iloscGraczy{};
    int zwyciezca{};
    string pmc{};
    char plansza[][rozmiarPlanszy]{};

    Pole pole[40];
    PStartowe pstart[4][4];
    Gracz gracz[4][4];

    cout << "  _____ _    _ _____ _   _  _____ ________     ___  __" << endl;
    cout << " / ____| |  | |_   _| \\ | |/ ____|___  /\\ \\   / / |/ /" << endl;
    cout << "| |    | |__| | | | |  \\| | |       / /  \\ \\_/ /| ' / " << endl;
    cout << "| |    |  __  | | | | . ` | |      / /    \\   / |  <  " << endl;
    cout << "| |____| |  | |_| |_| |\\  | |____ / /__    | |  | . \\ " << endl;
    cout << " \\_____|_|  |_|_____|_| \\_|\\_____/_____|   |_|  |_|\\_\\" << endl;

    while (true) {
    
    // wywolanie funkcji pokazujacej menu i zwrocenie wybranej opcji 
    menu(wyborMenu);

    // wywolanie odpowiednich funkcji programu zaleznie od wybranej opcji w menu
    if (wyborMenu == 1) {
        stworzPlansze(plansza);
        stworzGre(pole, pstart, gracz);
        zwyciezca = graj(plansza, pole, pstart, gracz, iloscGraczy);
        cout << endl << "Wielkie brawa! Wygral gracz: " << kolorGracza(zwyciezca) << gracz[zwyciezca][zwyciezca].nazwa << reset << endl;

        // jakies dziwne fikolki bo c++ nie chce wspolpracowac
        int iloscGraczy2 = 0;
        for (int i = 0; i < 4; i++) {
            if (!((gracz[i][i].nazwa).empty())) {
                iloscGraczy2++;
            }
        }

        //zapisz do pliku rezultaty gry;
        fstream plik;
        plik.open("wyniki.txt", ios::out | ios::app);
        if (plik.is_open()) {
            plik << endl << gracz[zwyciezca][zwyciezca].nazwa << endl;
            for (int i = 0; i < iloscGraczy2; i++) {
                if (i != zwyciezca) {
                    plik << gracz[i][i].nazwa << " ";
                }
            }
        } else {
            cout << "Blad otwarcia pliku!";
        }
        plik.close();

    } else if (wyborMenu == 2) {
        wyborMenu = 0;
        cout << endl << "Historia wynikow: " << endl << endl;
        cin.ignore();

        // wyswietlanie historii wynikow z plikow
        fstream plik;
        string s_plik;
        int licznik_m = 1;
        int licznik_s = 0;
        plik.open("wyniki.txt", ios::in);
        if (plik.is_open()) {
            while (!plik.eof()) {
                getline(plik, s_plik);
                if (licznik_s != 0) {
                    if (licznik_m % 2 == 1) {
                        cout << "Wygral: " << s_plik;
                    } else {
                        cout << ", przeciwnikami byli: " << s_plik << endl;
                    }
                    licznik_m++;
                }
                licznik_s++;
            }
        } else {
            cout << "Blad otwierania pliku!" << endl;
        }
        cout << endl;
        plik.close();

        while (pmc != "x") {
            cout << "Wcisnij 'x' aby wrocic do menu." << endl;
            getline(cin, pmc);
        }
        pmc = "";
        menu(wyborMenu);
    } else if (wyborMenu == 3) {
        wyborMenu = 0;
        cout << endl << "Zasady gry: " << endl << endl;
        cout << "Aby wyjsc pionkiem z bazy musisz wylosowac liczbe 6 i wybrac pionek, pojawi sie on na polu startowym." << endl;
        cout << "Twoim zadaniem jest jak najszybciej przekroczyc jakimkolwiek pinkiem linie mety." << endl;
        cout << "W trakcie gry mozna zbijac pionki innych graczy! Wroca oni wtedy zbitym pionkiem do bazy." << endl;
        cout << "Jesli sprobujesz wykonac niedozwolonyruch (zbicie samego siebie) tez wrocisz do bazy!" << endl;
        cout << "Uwaga! Wyjatkiem jest pole startowe, mozesz tam umiescic kilka pionkow jednoczesnie." << endl << endl;
        cin.ignore();
        while (pmc != "x") {
            cout << "Wcisnij 'x' aby wrocic do menu." << endl;
            getline(cin, pmc);
        }
        pmc = "";
        menu(wyborMenu);
    }
    }

}

void menu(int& wyborMenu) {

    while (wyborMenu < 1 || wyborMenu > 4) {

        cout << endl << "Wybierz co chcesz robic: " << endl;
        cout << "1 - Graj!" << endl;
        cout << "2 - Tablica wynikow" << endl;
        cout << "3 - Zasady gry" << endl;
        cout << "4 - Wyjscie" << endl;
        cin >> wyborMenu;

        if (wyborMenu >= 1 && wyborMenu <=3) {
            wyborMenu;
        } else if (wyborMenu == 4) {
            exit(0);
        } else {
            cout << endl << "Wybrales niepoprawna opcje, sprobuj ponownie!" << endl;
            cin.clear();
            fflush(stdin);
        }
    }

}

void stworzPlansze(char plansza[][rozmiarPlanszy]) {

    // Wzór planszy do gry 11x11
    char planszaWzor[][rozmiarPlanszy] = {
            {'r','r',' ','O','O','O','O','g',' ','g','g'},
            {'r','r',' ','O',' ','g',' ','O',' ','g','g'},
            {' ',' ',' ','O',' ','g',' ','O',' ',' ',' '},
            {'r','O','O','O',' ','g',' ','O','O','O','O'},
            {'O',' ',' ',' ',' ','g',' ',' ',' ',' ','O'},
            {'O','r','r','r','r','0','b','b','b','b','O'},
            {'O',' ',' ',' ',' ','y',' ',' ',' ',' ','O'},
            {'O','O','O','O',' ','y',' ','O','O','O','b'},
            {' ',' ',' ','O',' ','y',' ','O',' ',' ',' '},
            {'y','y',' ','O',' ','y',' ','O',' ','b','b'},
            {'y','y',' ','y','O','O','O','O',' ','b','b'},
    };

    // Tworzenie planszy na ktorej beda wprowadzane zmiany w rozgrywce
    for (int i = 0; i < rozmiarPlanszy; i++) {
        for (int j = 0; j < rozmiarPlanszy; j++) {
            plansza[i][j] = planszaWzor[i][j];
        }
    }
}

void stworzGre(Pole pole[40], PStartowe pstart[][4], Gracz gracz[4][4]) {

    // Tablica zawierajca wzor wspolrzednych pol po ktorej poruszaja sie pionki
    int wszystkiePola[][2] = {
        {3,0},{3,1},{3,2},{3,3},
        {2,3},{1,3},{0,3},
        {0,4},{0,5},{0,6},{0,7},
        {1,7},{2,7},{3,7},
        {3,8},{3,9},{3,10},
        {4,10},{5,10},{6,10},{7,10},
        {7,9},{7,8},{7,7},
        {8,7},{9,7},{10,7},
        {10,6},{10,5},{10,4},{10,3},
        {9,3},{8,3},{7,3},
        {7,2},{7,1},{7,0},
        {6,0},{5,0},{4,0}
    };

    //Tworzenie 40 struktur (tyle ile pol) i przypisanie im wspolrzednych ze wzoru
    for (int i = 0; i < 40; i++) {
        pole[i].y = wszystkiePola[i][0];
        pole[i].x = wszystkiePola[i][1];
    }

    //Tablica zawierajaca pola startowe pionkow kazdej z druzyny
    int polaStartowe[][4][2] = {
            {{0,0},{0,1},{1,1},{1,0}},
            {{0,9},{0,10},{1,10},{1,9}},
            {{9,9},{9,10},{10,10},{10,9}},
            {{9,0},{9,1},{10,1},{10,0}},
    };

    // Przypisanie kazdemu polu startowemu wspolrzednych
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            pstart[i][j].y = polaStartowe[i][j][0];
            pstart[i][j].x = polaStartowe[i][j][1];
        }
    }

    // Przypisanie kazdemu wszystkim pionkom ich wspolrzednych poczatkowych, druzyny i id
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            gracz[i][j].x = pstart[i][j].x;
            gracz[i][j].y = pstart[i][j].y;
            gracz[i][j].indeks = -1;
            gracz[i][j].druzyna = i;
            gracz[i][j].id = j + 1;
        }
    }
}

int graj(char plansza[][rozmiarPlanszy], Pole pole[40], PStartowe pstart[][4], Gracz gracz[4][4], int iloscGraczy) {

    // gracz[0][0].indeks = 38;
    // gracz[0][0].x = 0;
    // gracz[0][0].y = 5;

    //Pobranie ilosci graczy
    while (iloscGraczy > 4 || iloscGraczy < 2) {
        cout << endl << "Ilu jest graczy? (od 2 do 4) " << endl;
        cin >> iloscGraczy;

        if (iloscGraczy > 4 || iloscGraczy < 2) {
            cout << endl << "Podaj prawidlowa liczbe graczy!" << endl;
            cin.clear();
            fflush(stdin);
        }
    }
    
    // zmienna przechowujaca wartosc odpowiadajaca ktorego gracza jest kolej
    int ruch{};

    // Pobierz nazwy graczow
    cin.ignore();
    for (int i = 0; i < iloscGraczy; i++) {
        cout << endl << "Wpisz nazwe gracza nr " << i + 1 << endl;
        getline(cin, gracz[i][i].nazwa);
    }

    cin.clear();
    fflush(stdin);

    // zmienna pomocnicza do wykonania warunkowego kodu podczas pierwszego obiegu petli rozgrywki
    bool czyPierwszyObiegPetli = true;

    // Petla w ktorej toczy sie wlasciwa rozgrywka (przesuwanie pionami itp.)
    while(true) {
        
        // Po kazdym ruchu wyswietl plansze z obecna lokalizacja pionkow
        wyswietlAktualnaPlansze(plansza, gracz, iloscGraczy);

        if (!czyPierwszyObiegPetli) {
            // Odtworz dziwek ruszenia pionkiem z wyjatkiem pierwszego obiegu petli
            PlaySound(TEXT("sound\\move.wav"), NULL, SND_SYNC);
        } else {
            czyPierwszyObiegPetli = false;
        }

        cout << endl << "Kolej gracza: " << kolorGracza(ruch) << gracz[ruch][ruch].nazwa << reset << endl;

        // zmienna przechowujaca wybor pionka gracza
        int wyborPionka{};

        // Rzut kostka, losowanie o ile pol przesunie sie pionek gracza
        mt19937 mt;
        mt.seed(time(nullptr)); 
        uniform_int_distribution<int> d(1,6);
        int wynik = d(mt);
        //int wynik = 6;

        cout << "Nastepuje rzut kostka..." << endl;

        // funkcja sleep, zatrzymuje na chwile program
        Sleep(2500);

        // Odtwarzanie dzwieku losowania kostka
        PlaySound(TEXT("sound\\roll.wav"), NULL, SND_SYNC);

        cout << "Wynik: " << wynik << endl;

        // Pobranie od uzytkwonika ktorym pionkiem chce ruszyc
        while (wyborPionka > 4 || wyborPionka < 1) {
            cout << "Ktory pionkiem chcesz ruszyc?" << endl;
            cin >> wyborPionka;

            if (wyborPionka > 4 || wyborPionka < 1) {
                cout << endl << "Wybierz prawidlowy pionek!" << endl << endl;
                cin.clear();
                fflush(stdin);
            }
        }

        // algorytm przesuwania pionkiem
        // poczatkowo wszystkie pionki sa w bazie wiec maja indeks -1
        // zasady pozwalaja na wyjscie z bazy tylko wtedy gdy uzytkownik wyrzuci kostka 6
        // gdy tak sie stanie pionek zostanie ustawiony na polu startowym zaleznym od koloru druzyny
        if (gracz[ruch][wyborPionka - 1].indeks == -1 && wynik == 6) {
            gracz[ruch][wyborPionka - 1].indeks = indeksPola(ruch);
            gracz[ruch][wyborPionka - 1].x = pole[indeksPola(ruch)].x;
            gracz[ruch][wyborPionka - 1].y = pole[indeksPola(ruch)].y;

        // jesli uzytkownik ma juz w grze jakies pionki to moze nimi dowolnie poruszac 
         } else if (gracz[ruch][wyborPionka - 1].indeks != -1) {

             // pomocnicza zmienna sluzaca do przesuwania pionka krok po kroku
             // nie mozna przesunac pionka odrazu o kilka pozycji, poniewaz mogloby sie zdarzyc
             // ze zaczalby robic kolejne okrazenie zamaist wejsc do bazy
             int licznik = wynik;

             // petla przeuswjaca pionek co jedno pole
             while (licznik > 0) {

                 // sprawdz czy pionek nie znajduje sie na ostatnim polu, jesli tak nadaj odpowiedni indeks
                 if (czyDoBazy(ruch, gracz[ruch][wyborPionka - 1].indeks)) {
                     gracz[ruch][wyborPionka - 1].indeks = 40;
                 }

                 // jesli pionek nie jest na ostanim polu przesun go o jeden
                 if (gracz[ruch][wyborPionka - 1].indeks < 40) {
                     gracz[ruch][wyborPionka - 1].indeks = gracz[ruch][wyborPionka - 1].indeks + 1;
                     gracz[ruch][wyborPionka - 1].x = pole[gracz[ruch][wyborPionka - 1].indeks].x;
                     gracz[ruch][wyborPionka - 1].y = pole[gracz[ruch][wyborPionka - 1].indeks].y;
                 } else {

                     // jesli pionek jest na ostatnim polu to wprowadz go do bazy
                     doBazy(gracz, ruch, wyborPionka);

                     // (pierwszy) pionek ktory przekroczy mete wygrywa
                     if (gracz[ruch][wyborPionka - 1].x == 5 && gracz[ruch][wyborPionka - 1].y == 5) {
                         wyswietlAktualnaPlansze(plansza, gracz, iloscGraczy);
                         return ruch;
                     }
                 }
                 licznik--;
             }

            // Petla w ktorej sprawdzane jest ewentualne zbicie przeciwnika 
            // a w razie zbicia przenoszenie spowrotem do bazy
            for (int i = 0; i < 4; i++) {
                if (i != ruch) {
                    for (int j = 0; j < 4; j++) {
                        if (gracz[i][j].x == gracz[ruch][wyborPionka - 1].x && gracz[i][j].y == gracz[ruch][wyborPionka - 1].y) {
                            gracz[i][j].indeks = -1;
                            gracz[i][j].x = pstart[i][j].x;
                            gracz[i][j].y = pstart[i][j].y;

                            // Odtorz dzwiek zbicia przeciwnika
                            PlaySound(TEXT("sound\\hit.wav"), NULL, SND_SYNC);
                        }
                    }
                } else {

                    // w przypadku gdy gracz sprobuje wykonac niedozwolony ruch (zbicie samego siebie)
                    // rowniez jest przenoszony spowrotem do bazy
                    // nie dotyczy pola startowego
                    for (int j = 0; j < 4; j++) {
                        if ((j != (wyborPionka - 1)) && (gracz[ruch][wyborPionka - 1].x == gracz[ruch][j].x && gracz[ruch][wyborPionka - 1].y == gracz[ruch][j].y)) {
                            cout << "Niedozwolony ruch!" << endl << "Za kare wracasz do bazy :(";
                            gracz[ruch][wyborPionka - 1].indeks = -1;
                            gracz[ruch][wyborPionka - 1].x = pstart[ruch][wyborPionka - 1].x;
                            gracz[ruch][wyborPionka - 1].y = pstart[ruch][wyborPionka - 1].y;

                            // Odtworz dzwiek zbicia samego siebie
                            PlaySound(TEXT("sound\\hityf.wav"), NULL, SND_SYNC);
                        }
                    }
                }
            }
        }

        // kolejka nastepnego gracza
        if (ruch != iloscGraczy - 1) {
            ruch++;
        } else {
            ruch = 0;
        }
    }

    return -1;
}

void wyswietlAktualnaPlansze(char plansza[][rozmiarPlanszy], Gracz gracz[4][4], int iloscGraczy) {
    

    // tablica na kopie planszy
    char aktualnaPlansza[11][11]{};

    cout << endl << endl;

    // petla kopiujaca plansze
    for (int i = 0; i < 11; i++){
        for (int j = 0; j < 11; j++) {
            aktualnaPlansza[i][j] = plansza[i][j];
        }
    }

    // petla nanoszaca graczy na plansze
    for (int i = 0; i < iloscGraczy; i++){
        for (int j = 0; j < 4; j++) {
            aktualnaPlansza[gracz[i][j].y][gracz[i][j].x] = static_cast<char>(gracz[i][j].id + 48);
        }
    }

    cout << endl;

    // Petle nanoszaca kolory na plansze 
    for (int i = 0; i < 11; i++) {
        cout << " ";
        for (int j = 0; j < 11; j++) {
            string s = "";
            s = s + aktualnaPlansza[i][j] + " ";

            if (aktualnaPlansza[i][j] == 'r') {
                s = "O ";
                s = czerwony + s + reset;
            } else if (aktualnaPlansza[i][j] == 'g') {
                s = "O ";
                s = zielony + s + reset;
            } else if (aktualnaPlansza[i][j] == 'b') {
                s = "O ";
                s = niebieski + s + reset;
            } else if (aktualnaPlansza[i][j] == 'y') {
                s = "O ";
                s = zolty + s + reset;
            } else if (aktualnaPlansza[i][j] == '1' || 
                       aktualnaPlansza[i][j] == '2' || 
                       aktualnaPlansza[i][j] == '3' || 
                       aktualnaPlansza[i][j] == '4') {
                
                s = aktualnaPlansza[i][j];
                s += " ";

                for (int a = 0; a < 4; a++){
                    for (int b = 0; b < 4; b++) {
                        if (i == gracz[a][b].y && j == gracz[a][b].x) {

                            if (gracz[a][b].druzyna == 0) {
                                s = czerwony + s + reset;
                            } else if (gracz[a][b].druzyna == 1) {
                                s = zielony + s + reset;
                            } else if (gracz[a][b].druzyna == 2) {
                                s = niebieski + s + reset;
                            } else if (gracz[a][b].druzyna == 3) {
                                s = zolty + s + reset;
                            }

                        }
                    }
                }
            }

            cout << s;
        }
        cout << endl;
    }
    cout << endl;
}

// funkcja zwracajaca kolor zaleznie od druzyny
string kolorGracza(int id) {
    if (id == 0) {
        return czerwony;
    } else if (id == 1) {
        return zielony;
    } else if (id == 2) {
        return niebieski;
    } else if (id == 3) {
        return zolty;
    } else {
        return "";
    }
}

// funkcja zwracajaca indek pola potrzebny do obliczenia wspolrzednych
int indeksPola(int ruch) {
    return ruch*10;
}

// funkcja sprawdzajaca czy pionek danej druzyny jest na ostnim mozliwym polu
bool czyDoBazy(int ruch, int indeks) {
    switch (ruch) {
        case 0:
            if (indeks == 38) {
                return true;
            } else {
                return false;
            }
        case 1:
            if (indeks == 8) {
                return true;
            } else {
                return false;
            }
        case 2:
            if (indeks == 18) {
                return true;
            } else {
                return false;
            }
        case 3:
            if (indeks == 28) {
                return true;
            } else {
                return false;
            }
        default:
            return false;
    }
    return false;
}

// jesli pionek jest w bazie poruszaj nim w prawo/lewo/gora/dol 
void doBazy(Gracz gracz[4][4], int ruch, int wyborPionka) {

    // ruch w prawo czerwonej druzyny (zgodnie z plansza) itd...
    if (ruch == 0) {
        gracz[ruch][wyborPionka - 1].x += 1;
    } else if (ruch == 1) {
        gracz[ruch][wyborPionka - 1].y += 1;
    } else if (ruch == 2) {
        gracz[ruch][wyborPionka - 1].x -= 1;
    } else if (ruch == 3) {
        gracz[ruch][wyborPionka - 1].y -= 1;
    }
}