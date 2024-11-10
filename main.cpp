#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <windows.h>
#include <random>

using namespace std;

class Preparate {
    int numarOrdine;
    string nume;
    int timpPreparare;
    double pret;

public:
    Preparate(int numarOrdine, string nume, int timpPreparare, double pret)
        : numarOrdine(numarOrdine), nume(std::move(nume)), timpPreparare(timpPreparare), pret(pret) {}

    Preparate(const Preparate& p) {
        this->numarOrdine = p.numarOrdine;
        this->nume = p.nume;
        this->timpPreparare = p.timpPreparare;
        this->pret = p.pret;
    }

    Preparate& operator=(const Preparate& p) {
        if (this != &p) {
            this->numarOrdine = p.numarOrdine;
            this->nume = p.nume;
            this->timpPreparare = p.timpPreparare;
            this->pret = p.pret;
        }
        return *this;
    }

    ~Preparate() = default;

    friend ostream& operator<<(ostream& os, const Preparate& p);
};

ostream& operator<<(ostream& os, const Preparate& p) {
    os << "Order #" << p.numarOrdine << ": " << p.nume << " " << p.timpPreparare << " min " << p.pret << " RON";
    return os;
}

class Meniu {
private:
    std::vector<Preparate> preparate;
    int nextOrderNumber = 1;
public:
    void adaugarePreparat(const Preparate& p) {
        preparate.push_back(p);
    }

    void afisareMeniu() const {
        for (const auto& preparat : preparate)
            std::cout << preparat << std::endl;
    }

    void citireDinFisier(const string& numeFisier) {
        ifstream file(numeFisier);
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                stringstream ss(line);
                string nume;

                vector<string> words;
                while (ss >> nume) {
                    words.push_back(nume);
                }

                if (words.size() >= 3) {
                    double pret = stod(words.back());
                    words.pop_back();
                    int timpPreparare = stoi(words.back());
                    words.pop_back();

                    nume = "";
                    for (const auto& word : words) {
                        if (!nume.empty()) nume += " ";
                        nume += word;
                    }
                    Preparate preparat(nextOrderNumber++, nume, timpPreparare, pret);
                    adaugarePreparat(preparat);
                } else {
                    cout << "Linie incorecta: \"" << line << "\"" << endl;
                }
            }
            file.close();
        }
    }
};

enum OptiuneServire {
    LaPachet,
    InRestaurant
};

class Client {
    string Nume;
    string Numar;
    string Adresa;
    OptiuneServire optiune;
    int numarMasa;

public:
    Client() : optiune(LaPachet), numarMasa(-1) {}  // Default initialization

    Client(string Nume, string Numar, string Adresa, OptiuneServire optiune, int numarMasa = -1)
        : Nume(std::move(Nume)), Numar(std::move(Numar)), Adresa(std::move(Adresa)), optiune(optiune), numarMasa(numarMasa) {
        if (optiune == InRestaurant && numarMasa == -1) {
            cout << "Avertizare: Nu a fost specificat un număr de masă pentru opțiunea 'In Restaurant'.\n";
        }
    }

    void informatii() {
        int optiuneInt;
        cout << "Doriti 0 pentru LaPachet sau 1 pentru InRestaurant: ";
        cin >> optiuneInt;
        optiune = static_cast<OptiuneServire>(optiuneInt);

        switch (optiune) {
            case LaPachet: {
                cout << "Numele: ";
                cin >> Nume;
                cout << "Numar: ";
                cin >> Numar;
                cout << "Adresa: ";
                cin >> Adresa;
                break;
            }
            case InRestaurant: {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> distrib(1, 10);
                numarMasa = distrib(gen);

                cout << "Numarul mesei este: " << numarMasa << endl;
                break;
            }
        }
    }
};

static void Culoare(int c) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, c);
}

void desenareBraz(int inaltime) {
    Culoare(10);
    for (int i = 1; i <= inaltime; i++) {
        for (int j = 1; j <= inaltime - i; j++) {
            cout << " ";
        }
        for (int j = 1; j <= (2 * i - 1); j++) {
            cout << "*";
        }
        cout << endl;
    }
    for (int i = 1; i <= 1; i++) {
        for (int j = 1; j <= inaltime - 1; j++) {
            cout << " ";
        }
        cout << "|" << endl << endl;
    }
}

void AfisareMesaj() {
    Culoare(9);

    string text = "                 BINE ATI VENIT LA OCHE!\n"
                  "                 -----------------------\n\n"
                  "->Locatie: Predeal\n"
                  "->Restaurantul OCHE exceleaza prin savoarea, diversitatea si calitatea produselor gatite cu pasiune \n"
                  "->Un loc perfect pentru relaxare alaturi de cei dragi, departe de agitatia urbana\n\n\n";

    cout << text;
    desenareBraz(5);
    Culoare(9);
    cout << "1 - daca vrei sa continui si sa vezi ofertele noastre, apasa tasta 1: ";

    char optiune;
    cin >> optiune;

    if (optiune == '1') {
        system("CLS");
        cout << "Meniu:\n";
        cout << "1 - Meniu Restaurant\n";
        cout << "2 - Meniu Bar\n";
        cout << "3 - Darts\n";
        cout << "Alege optiunea dorita (1/2/3): ";

        char alegere;
        cin >> alegere;

        switch (alegere) {
            case '1': {
                Meniu meniu;
                meniu.citireDinFisier("meniu.txt");
                meniu.afisareMeniu();
                Client client;
                client.informatii();

                break;
            }
            case '2':
                cout << "Ai ales Barul OCHE!\n";
                break;
            case '3':
                cout << "Ai ales Darts!\n";
                break;
            default:
                cout << "Optiune invalida!\n";
        }

    } else {
        system("CLS");
        cout << "A fost o placere sa te avem ca vizitator! La revedere!\n";
    }
}

int main() {
    AfisareMesaj();
    return 0;
}
