#include <iostream>
#include <string>
#include <vector>

class Preparat
{
private:
    std::string nume;
    int timpPreparare;
    double pret;
/// ana are mere
public:
    Preparat(std::string nume, double pret, int timpPreparare)
        : nume(std::move(nume)), timpPreparare(timpPreparare), pret(pret) {}

    Preparat(const Preparat& other) = default;

    Preparat& operator=(const Preparat& other)
    {
        if (this != &other)
        {
            nume = other.nume;
            timpPreparare = other.timpPreparare;
            pret = other.pret;
        }
        return *this;
    }

    ~Preparat() = default;

    friend std::ostream& operator<<(std::ostream& COUT, const Preparat& p);
};

std::ostream& operator<<(std::ostream& COUT, const Preparat& p)
{
    COUT << "NUME: " << p.nume << ", PRET: " << p.pret << ", TIMP PREPARARE: " << p.timpPreparare << " minute.";
    return COUT;
}

class Meniu
{
private:
    std::vector<Preparat> preparate;

public:
    void adaugarePreparat(const Preparat& p);
    void afisareMeniu() const;
};

void Meniu::adaugarePreparat(const Preparat& p)
{
    preparate.push_back(p);
}

void Meniu::afisareMeniu() const
{
    for (const auto& preparat : preparate)
        std::cout << preparat << std::endl;
}

class Client
{
private:
    int NrMasa;

public:
    explicit Client(int nrmasa) : NrMasa(nrmasa) {}

    void afisareDetalii() const
    {
        std::cout << "NrMasa: " << NrMasa << std::endl;
    }
};

class Comanda
{
private:
    Client client;
    std::vector<Preparat> preparateComandate;

public:
    explicit Comanda(const Client& client) : client(client) {}

    void adaugaPreparat(const Preparat& preparat)
    {
        preparateComandate.push_back(preparat);
    }

    void afisareComanda() const
    {
        client.afisareDetalii();
        std::cout << "Preparatele comandate:" << std::endl;
        for (const auto& preparat : preparateComandate)
            std::cout << preparat << std::endl;
    }
};

int main()
{
    Meniu meniu;
    meniu.adaugarePreparat(Preparat("Pizza Margherita", 29.99, 15));
    meniu.adaugarePreparat(Preparat("Salată Caesar", 19.99, 10));
    meniu.adaugarePreparat(Preparat("Tiramisu", 15.99, 5));

    std::cout << "Meniul disponibil:" << std::endl;
    meniu.afisareMeniu();

    Client client(5);
    Comanda comanda(client);
    comanda.adaugaPreparat(Preparat("Pizza Margherita", 29.99, 15));
    comanda.adaugaPreparat(Preparat("Tiramisu", 15.99, 5));

    std::cout << std::endl << "Detalii comanda:" << std::endl;
    comanda.afisareComanda();

    return 0;
}