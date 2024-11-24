#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#ifdef _WIN32
#include <windows.h>
#endif
#include <random>
#include <stdexcept>
#include <rlutil.h>

using namespace std;

class Preparate {
    int numarOrdine;
    string nume;
    int timpPreparare;
    double pret;

public:
    Preparate(int numarOrdine, string nume, int timpPreparare, double pret)
        : numarOrdine(numarOrdine), nume(std::move(nume)), timpPreparare(timpPreparare), pret(pret) {
    }

    Preparate(const Preparate &p) {
        this->numarOrdine = p.numarOrdine;
        this->nume = p.nume;
        this->timpPreparare = p.timpPreparare;
        this->pret = p.pret;
    }

    Preparate &operator=(const Preparate &p) {
        if (this != &p) {
            this->numarOrdine = p.numarOrdine;
            this->nume = p.nume;
            this->timpPreparare = p.timpPreparare;
            this->pret = p.pret;
        }
        return *this;
    }

    ~Preparate() = default;

    [[nodiscard]] int getNumarOrdine() const { return numarOrdine; }
    [[nodiscard]] string getNume() const { return nume; }
    [[nodiscard]] double getPret() const { return pret; }

    friend ostream &operator<<(ostream &os, const Preparate &p);
};

ostream &operator<<(ostream &os, const Preparate &p) {
    os << "Order #" << p.numarOrdine << ": " << p.nume << " " << p.timpPreparare << " min " << p.pret << " RON";
    return os;
}

class Meniu {
    vector<Preparate> preparate;
    int nextOrderNumber = 1;

public:
    void adaugarePreparat(const Preparate &p) {
        preparate.push_back(p);
    }

    void afisareMeniu() const {
        for (const auto &preparat: preparate)
            cout << preparat << endl;
    }

    [[nodiscard]] Preparate getPreparat(int numarOrdine) const {
        for (const auto &preparat: preparate) {
            if (preparat.getNumarOrdine() == numarOrdine) {
                return preparat;
            }
        }
        throw invalid_argument("Preparatul cu acest numar de ordine nu exista.");
    }

    void citireDinFisier(const string &numeFisier) {
        ifstream file(numeFisier);
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

                nume.clear();
                for (const auto &word: words) {
                    if (!nume.empty()) nume += " ";
                    nume += word;
                }
                Preparate preparat(nextOrderNumber++, nume, timpPreparare, pret);
                adaugarePreparat(preparat);
            } else {
                cout << "Linie incorecta: \"" << line << "\"" << endl;
            }
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
    Client() : optiune(LaPachet), numarMasa(-1) {
    }

    Client(string Nume, string Numar, string Adresa, OptiuneServire optiune, int numarMasa = -1)
        : Nume(std::move(Nume)), Numar(std::move(Numar)), Adresa(std::move(Adresa)), optiune(optiune),
          numarMasa(numarMasa) {
        if (optiune == InRestaurant && numarMasa == -1) {
            cout << "Avertizare: Nu a fost specificat un număr de masă pentru opțiunea 'In Restaurant'.\n";
        }
    }

    void informatii() {
        int optiuneInt=-1;
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
                random_device rd;
                mt19937 gen(rd());
                uniform_int_distribution<> distrib(1, 10);
                numarMasa = distrib(gen);

                cout << "Numarul mesei este: " << numarMasa << endl;
                break;
            }
            default:
                cerr << "Eroare: Opțiune necunoscută.\n";
                break;
        }
    }

    [[nodiscard]] const string& getNume() const { return Nume; }
    [[nodiscard]] const string& getNumar() const { return Numar; }
    [[nodiscard]] const string& getAdresa() const { return Adresa; }

    [[nodiscard]] int getNumarMasa() const { return numarMasa; }
    [[nodiscard]] OptiuneServire getOptiune() const { return optiune; }
};


class BonFiscal {
    Client client;
    vector<Preparate> comenzi;
    double total = 0;

public:
    explicit BonFiscal(Client client) : client(std::move(client)) {
    }

    void adaugaPreparat(const Preparate &preparat) {
        comenzi.push_back(preparat);
        total += preparat.getPret();
    }

    void afisareBon() const {
        cout << "\n--- Bon Fiscal ---\n";
        cout << "Client: " << client.getNume() << "\n";
        if (client.getOptiune() == LaPachet) {
            cout << "Numar: " << client.getNumar() << "\n";
            cout << "Adresa: " << client.getAdresa() << "\n";
        } else {
            cout << "Numar masa: " << client.getNumarMasa() << "\n";
        }
        cout << "\nComenzi:\n";
        for (const auto &preparat: comenzi) {
            cout << preparat << "\n";
        }
        cout << "\nTotal de plata: " << total << " RON\n";
    }
};


<<<<<<< Updated upstream
void desenareBraz(int inaltime) {
    rlutil::setColor(rlutil::BLUE);
    for (int i = 1; i <= inaltime; i++) {
        for (int j = 1; j <= inaltime - i; j++) {
            cout << " ";
        }
        for (int j = 1; j <= (2 * i - 1); j++) {
            cout << "*";
        }
        cout << endl;
=======


void Afisare() {
    Meniu restaurant;
    MeniuBar bar;
    int id_client;
    double reducere = 1;

    restaurant.citireDinFisier("meniu.txt");
    bar.citireDinFisier("meniuBar");
    cout << endl;

    string text = "         Bine ai venit la HELLO FRESH!\n"
        "         ----------------------------";
    cout << text << endl;



    cout << "Introduceti ID_Client: ";
    cin >> id_client;

    Client client(id_client);
    Client::CodClient(id_client, reducere);
    client.informatii();
    cout << endl;

    Comanda comanda(&client, restaurant, bar);
    comanda.plasareComanda();

    cout << "Mai doriti ceva y/n:";
    char raspuns;
    cin >> raspuns;

    switch (raspuns) {
    case 'y': {
        comanda.plasareComanda();
        comanda.afisareComanda(reducere);
        break;
    }
    case 'n': {
        comanda.afisareComanda(reducere);
        break;
    }
    default: {
        cout << "Raspuns invalid! Te rugam să introduci 'y' sau 'n'." << endl;
        break;
>>>>>>> Stashed changes
    }
    for (int i = 1; i <= 1; i++) {
        for (int j = 1; j <= inaltime - 1; j++) {
            cout << " ";
        }
        cout << "|" << endl << endl;
    }


}

void AfisareMesaj() {
    rlutil::setColor(rlutil::BLUE);

    string text = "                 BINE ATI VENIT LA OCHE!\n"
            "                 -----------------------\n\n"
            "->Locatie: Predeal\n"
            "->Restaurantul OCHE exceleaza prin savoarea, diversitatea si calitatea produselor gatite cu pasiune \n"
            "->Un loc perfect pentru relaxare alaturi de cei dragi, departe de agitatia urbana\n\n\n";

    cout << text;
    desenareBraz(5);
    rlutil::setColor(rlutil::BLUE);;
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

                BonFiscal bon(client);

                cout << "Selectati numerele de ordine ale preparatelor dorite (0 pentru a finaliza):\n";
                int numarOrdine;
                while (true) {
                    cout << "Numar ordine: ";
                    cin >> numarOrdine;
                    if (numarOrdine == 0) break;
                    try {
                        Preparate preparat = meniu.getPreparat(numarOrdine);
                        bon.adaugaPreparat(preparat);
                    } catch (const std::exception &e) {
                        cout << e.what() << endl;
                    }
                }
                bon.afisareBon();
                break;
            }
            case '2':
            case '3':
                cout << "Optiune indisponibila!\n";
                break;
            default:
                cout << "Optiune invalida!\n";
                break;
        }
    } else {
        cout << "Multumim pentru vizita!\n";
    }
}

int main() {
    AfisareMesaj();
    return 0;
}
