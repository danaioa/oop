#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <random>
#include <stdexcept>
///#include <rlutil.h>

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
    [[nodiscard]] int getNumarOrdine() const { return numarOrdine; }
    [[nodiscard]] const string& getNume() const { return nume; }
    [[nodiscard]] double getPret() const { return pret; }
    friend ostream& operator<<(ostream& os, const Preparate& p);
};

ostream& operator<<(ostream& os, const Preparate& p) {
    os << "Order #" << p.numarOrdine << ": " << p.nume << " " << p.timpPreparare << " min " << p.pret << " RON";
    return os;
}

class Meniu {
    vector<Preparate> preparate;

public:
    void adaugarePreparat(const Preparate& p) {
        preparate.push_back(p);
    }

    void afisareMeniu() const {
        cout << "\n--- Meniul Restaurantului ---\n";
        for (const auto& preparat : preparate)
            cout << preparat << endl;
    }

    [[nodiscard]] Preparate getPreparat(int numarOrdine) const {
        for (const auto& preparat : preparate) {
            if (preparat.getNumarOrdine() == numarOrdine) {
                return preparat;
            }
        }
        throw invalid_argument("Preparatul cu acest numar de ordine nu exista.");
    }

    void citireDinFisier(const string& numeFisier) {
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
                for (const auto& word : words) {
                    if (!nume.empty()) nume += " ";
                    nume += word;
                }
                Preparate preparat(static_cast<int>(preparate.size()) + 1, nume, timpPreparare, pret);
                adaugarePreparat(preparat);
            }
            else {
                cout << "Linie incorecta: \"" << line << "\"" << endl;
            }
        }
    }
};

class Bautura {
    int numarOrdine = 0;
    string nume;
    double pret;

public:
    Bautura() : nume(), pret(0.0) {}
    Bautura(int numarOrdine, string nume, double pret)
        : numarOrdine(numarOrdine), nume(std::move(nume)), pret(pret) {}

    [[nodiscard]] int getNumarOrdine() const { return numarOrdine; }
    [[nodiscard]] const string& getNume() const { return nume; }
    [[nodiscard]] double getPret() const { return pret; }

    friend ostream& operator<<(ostream& os, const Bautura& b);
};

ostream& operator<<(ostream& os, const Bautura& b) {
    os << "Order #" << b.numarOrdine << ": " << b.nume << " " << b.pret << " RON";
    return os;
}

class MeniuBar {
    Bautura* alegeri = nullptr;
    size_t nrBauturi = 0;
    size_t capacitate = 0;

public:

    MeniuBar() = default;
    MeniuBar(const MeniuBar& other)
        : nrBauturi(other.nrBauturi), capacitate(other.capacitate) {
        if (other.alegeri) {
            alegeri = new Bautura[capacitate];
            for (size_t i = 0; i < nrBauturi; ++i) {
                alegeri[i] = other.alegeri[i];
            }
        }
    }
    MeniuBar& operator=(const MeniuBar& other) {
        if (this != &other) {
            delete[] alegeri;
            nrBauturi = other.nrBauturi;
            capacitate = other.capacitate;
            if (other.alegeri) {
                alegeri = new Bautura[capacitate];
                for (size_t i = 0; i < nrBauturi; ++i) {
                    alegeri[i] = other.alegeri[i];
                }
            } else {
                alegeri = nullptr;
            }
        }
        return *this;
    }


    void adaugareBautura(const Bautura& b) {
        if (alegeri == nullptr) {
            alegeri = new (nothrow) Bautura[1]; // prima alocare
            if (alegeri == nullptr) {
                return;
            }
            capacitate = 1;
        }

        if (nrBauturi == capacitate) {
            size_t nouaCapacitate = capacitate * 2;
            auto* nou = new (nothrow) Bautura[nouaCapacitate];
            if (nou == nullptr) {
                return;
            }

            for (size_t i = 0; i < nrBauturi; ++i) {
                nou[i] = alegeri[i];
            }

            delete[] alegeri;
            alegeri = nou;
            capacitate = nouaCapacitate;
        }

        alegeri[nrBauturi++] = b;
    }



    void afisareMeniu() const {
        cout << "\n--- Meniul Barului ---\n";
        for (size_t i = 0; i < nrBauturi; ++i) {
            cout << alegeri[i] << endl;
        }
    }

    [[nodiscard]] Bautura getBautura(int numarOrdine) const {
        for (size_t i = 0; i < nrBauturi; ++i) {
            if (alegeri[i].getNumarOrdine() == numarOrdine) {
                return alegeri[i];
            }
        }
        throw invalid_argument("Bautura cu acest numar de ordine nu exista.");
    }

    void citireDinFisier(const string& numeFisier) {
        ifstream file(numeFisier);
        string line;
        while (getline(file, line)) {
            stringstream ss(line);
            vector<string> words;
            string cuvant;

            while (ss >> cuvant) {
                words.push_back(cuvant);
            }

            if (words.size() < 2) {
                cout << "Linie incorecta: \"" << line << "\"" << endl;
                continue;
            }

            try {
                double pret = stod(words.back());
                words.pop_back();
                string nume;
                for (const auto& word : words) {
                    if (!nume.empty()) nume += " ";
                    nume += word;
                }

                Bautura bautura(static_cast<int>(nrBauturi) + 1, nume, pret);
                adaugareBautura(bautura);
            }
            catch (const invalid_argument& e) {
                cout << "Eroare la conversia pretului pe linia: \"" << e.what() << "\"" << endl;
            }
        }
    }

    ~MeniuBar() {
        delete[] alegeri;
    }

};


enum OptiuneServire {
    LaPachet,
    InRestaurant
};

//class ClientNou;

int citesteUltimulNumar([[maybe_unused]] const string& filename){
    ifstream file("Clienti.txt");

    file.seekg(0, ios::end);

    int p = 1;
    int ultimul_numar = 0;
    char ch;

    while (file.tellg() > 0) {
        file.seekg(-2, ios::cur);
        file.get(ch);

        if (ch == '\n') {
            break;
        }
        if (ch >= '0' && ch <= '9') {
            ultimul_numar = (ch - '0') * p + ultimul_numar;
            p *= 10;

        }
    }
    return ultimul_numar;
}

int generareCod() {

    int n = citesteUltimulNumar("Clienti.txt");
    n++;
    return n;
}

class Client {
private:
    string Nume;
    string Telefon;
    string Adresa;
    OptiuneServire Optiune;
    int NrMasa = 0;
   int Id_Client;
public:

    explicit Client(int id_client) : Nume(), Telefon(), Adresa(), Optiune(InRestaurant), NrMasa(0), Id_Client(id_client) {}
    Client(std::string nume, std::string telefon, std::string adresa, int id_client) : Nume(std::move(nume)),
        Telefon(std::move(telefon)), Adresa(std::move(adresa)), Optiune(LaPachet), Id_Client(id_client) {
        cout << "Client creat cu ID-ul: " << id_client <<endl;
    }



    void informatii() {
        int optiuneInt = -1;
        cout << "Doriti 0 pentru LaPachet sau 1 pentru InRestaurant: ";
        cin >> optiuneInt;
        Optiune = static_cast<OptiuneServire>(optiuneInt);
        switch (Optiune) {
        case LaPachet: {
            cout << "Numele: ";
            cin >> Nume;
            cout << "Telefon: ";
            cin >> Telefon;
            cout << "Adresa: ";
            cin >> Adresa;
            break;
        }
        case InRestaurant: {
            random_device rd;
            mt19937 gen(rd());
            uniform_int_distribution<> distrib(1, 10);
            NrMasa = distrib(gen);

            cout << "Numarul mesei este: " << NrMasa << endl;
            break;
        }
        default:
            cout << "Eroare: Opțiune necunoscută.\n";
            break;
        }


    }


    static void verificareCod(int id, int& ok, double& reducere) {
        ifstream file("Clienti.txt");
        int idExistent, nrComenzi;
        reducere = 1;
        ok = 0;

        while (file >> idExistent >> nrComenzi) {
            if (idExistent == id) {
                ok = 1;
                if (nrComenzi == 0) { // Reducere 40%
                    reducere = 0.4;
                }
                else if (nrComenzi % 5 == 0) { // Reducere 10%
                    reducere = 0.2;
                }
                break;
            }
        }
        file.close();
    }





    static void CodClient(int& id, double& reducere, bool test_mode = false) {
        int ok = 0;
        reducere = 1;

        verificareCod(id, ok, reducere);

        if (test_mode) {

            if (ok == 0) {
                cout << "ID_Client invalid. Generăm automat un cont nou.\n";
                ofstream file("Clienti.txt", ios::app);
                id = generareCod();
                file <<endl << id << " " << '0';
                cout << "Cont creat cu succes pentru ID_Client: " << id << "\n";

            }
            return;
        }

        while (ok == 0) {
            cout << "ID_Client este incorect\n";
            cout << "1 - Creare cont\n";
            cout << "2 - Nu acum\n";
            cout << "3 - Mai incerca\n";

            int optiune;
            while (true) {
                cout << "Alege o opțiune (1/2/3): ";
                if (std::cin >> optiune && (optiune == 1 || optiune == 2 || optiune == 3)) {
                    break; // Input valid
                } else {
                    cout << "Opțiune invalidă! Te rugăm să alegi 1, 2 sau 3.\n";
                    cin.clear();
                    cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                }
            }

            if (optiune == 1) {
                ofstream file("Clienti.txt", ios::app);
                id = generareCod();
                file << std::endl << id << " " << '0';
                std::cout << "Creare cont nou pentru ID_Client: " << id << std::endl;
                std::cout << "Contul a fost creat cu succes\n";
                break;
            } else if (optiune == 2) {
                return;
            } else if (optiune == 3) {
                std::cout << "Introduceti ID_Client: ";
                std::cin >> id;
                verificareCod(id, ok, reducere);
            }
        }
    }




    [[nodiscard]] int getIdClient() const { return Id_Client; }
    [[nodiscard]] const string& getNume() const { return Nume; }
    [[nodiscard]] const string& getAdresa() const { return Adresa; }
    [[nodiscard]] const string& getTelefon() const { return Telefon; }
};

class Comanda {
    Client* client;
    Meniu meniu;
    MeniuBar meniuBar;
    vector<pair<Preparate, int>> preparateComandate;
    vector<pair<Bautura, int>> bauturiComandate;

public:
    Comanda(Client* client, Meniu meniu, const MeniuBar& meniuBar)
        : client(client), meniu(std::move(meniu)), meniuBar(meniuBar) {}

    void plasareComanda() {
        int optiuneMeniu = -1;
        string text = "Meniu:\n"
            "1-Restaurant\n"
            "2-Bar\n"
            "Alege optiunea dorita (1/2)";
        cout << text << endl;
        cin >> optiuneMeniu;

        if (optiuneMeniu == 1) {
            meniu.afisareMeniu();
            int nrOrdine = 0, cantitate = 0;
            cout << "\nIntroduceti numarul de ordine al preparatului dorit (sau -1 pentru a termina): ";
            while (cin >> nrOrdine && nrOrdine != -1) {
                try {
                    Preparate preparat = meniu.getPreparat(nrOrdine);
                    cout << "Introduceti cantitatea dorita: ";
                    cin >> cantitate;
                    preparateComandate.emplace_back(preparat, cantitate);
                }
                catch (const invalid_argument& e) {
                    cout << e.what() << endl;
                }
                cout << "Introduceti numarul de ordine al altui preparat (sau -1 pentru a finaliza comanda): ";
            }
        }
        else if (optiuneMeniu == 2) {
            meniuBar.afisareMeniu();
            int nrOrdine = 0, cantitate = 0;
            cout << "\nIntroduceti numarul de ordine al bauturii dorite (sau -1 pentru a termina): ";
            while (cin >> nrOrdine && nrOrdine != -1) {
                try {
                    Bautura bautura = meniuBar.getBautura(nrOrdine);
                    cout << "Introduceti cantitatea dorita: ";
                    cin >> cantitate;
                    bauturiComandate.emplace_back(bautura, cantitate);
                }
                catch (const invalid_argument& e) {
                    cout << e.what() << endl;
                }
                cout << "Introduceti numarul de ordine al altei bauturi (sau -1 pentru a termina): ";
            }
        }

        else {
            cout << "Optiune invalida. Incercati din nou.\n";
        }
    }
    void afisareComanda(double reducere = 1.0) const {
        cout << "\n--- Comanda Finala ---\n";
        cout << "Client: " << client->getNume() << "\n";
        cout<<"ID_CLIENT: "<<client->getIdClient()<<"\n";
        cout<<"Telefon: "<<client->getTelefon()<<"\n";
        cout<<"Adresa: "<<client->getAdresa()<<"\n";

        if (reducere != 1.0) {
            cout << "Reducere:" << reducere * 100 << "%" << "\n";
        }

        cout << "\nPreparate comandate:\n";
        for (const auto& [preparat, cantitate] : preparateComandate) {
            cout << preparat << " x" << cantitate << " = " << preparat.getPret() * cantitate << " RON\n";
        }

        cout << "\nBauturi comandate:\n";
        for (const auto& [bautura, cantitate] : bauturiComandate) {
            cout << bautura << " x" << cantitate << " = " << bautura.getPret() * cantitate << " RON\n";
        }

        if (reducere != 1.0) {
            cout << "\nTotal de plata (cu reducere): " << calculareTotal(reducere) << " RON\n";
        }
        else {
            cout << "\nTotal de plata: " << calculareTotal(reducere) << " RON\n";
        }
    }


    [[nodiscard]] double calculareTotal(double reducere = 1.0) const {
        double total = 0.0;

        for (const auto& [preparat, cantitate] : preparateComandate) {
            total += preparat.getPret() * cantitate;
        }
        for (const auto& [bautura, cantitate] : bauturiComandate) {
            total += bautura.getPret() * cantitate;
        }


        total = total - (total * reducere);

        return total;
    }


};



void Afisare() {
    Meniu restaurant;
    MeniuBar bar;
    int id_client;
    double reducere = 1; // Reducere implicită

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
        cout << "Răspuns invalid! Te rugăm să introduci 'y' sau 'n'." << endl;
        break;
    }
    }
}


int main() {

    Afisare();
    //cout<<generareCod();
    return 0;
}