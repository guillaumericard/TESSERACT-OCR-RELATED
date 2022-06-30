/* Squelette pour classe générique Tableau<T>.
 * Lab3 -- Tableau dynamique générique
 * UQAM / Département d'informatique
 * INF3105 - Structures de données et algorithmes
 * http://ericbeaudry.uqam.ca/INF3105/lab3/
 */

#if !defined(_TABLEAU___H_)
#define _TABLEAU___H_
#include <assert.h>
#include <iostream>

template <class T>
class Tableau
{

public:
    Tableau(int capacite_initiale = 4); // tableau prend un parametre int, et si rien est entré alors le paramètre par defaut est 4;
    Tableau(const Tableau &);
    ~Tableau();

    // Ajouter un element à la fin
    void ajouter(const T &element);
    // Vider le tableau
    void vider();
    // Retourne le nombre d'éléments dans le tableau
    int taille() const;

    // Insère element à position index. Les éléments à partir de index sont décalés d'une position.
    void inserer(const T &element, int index = 0);

    // Enlève l'element à position index. Les éléments après index sont décalés d'une position.
    void enlever(int index = 0);
    void enlever_dernier();
    // Cherche et retourne la position de l'élément. Si non trouvé, retourne -1.
    int chercher(const T &element) const;
    int chercherPremier(const T &element) const;
    int chercherDernier(const T &element) const;
    bool contient(const T &element) const;

    T &operator[](int index);
    const T &operator[](int index) const;

    bool operator==(const Tableau<T> &autre) const;
    Tableau<T> &operator=(const Tableau<T> &autre);
    void redimensionner(int nouvCapacite);

    // Fonctionnalités spécifiques au TP1
    bool vide() const { return nbElements == 0; }
    int compter(const T &element); // retourne le nb d'occurrences de element
    void creer_index() const;      // cette fonction permet d'optimiser les fonctions chercher, contient et compter.

private:
    T *elements;
    int nbElements;
    int capacite;
    mutable bool indexValide;
    mutable int *tableauIndex = nullptr;

    void trier(T elementsCopie[], int debut, int fin) const;
    int partitionner(T elementsCopie[], int debut, int fin) const;
};

// ---------- Définitions -------------

template <class T>
Tableau<T>::Tableau(int capacite_)
{
    capacite = capacite_;
    nbElements = 0;
    elements = new T[capacite];
    indexValide = false;
}

template <class T>
Tableau<T>::Tableau(const Tableau &autre)
{
    capacite = autre.capacite;
    nbElements = autre.nbElements;
    elements = new T[capacite];
    for (int i = 0; i < nbElements; i++)
    {
        elements[i] = autre.elements[i];
    }
}

template <class T>
Tableau<T>::~Tableau()
{
    delete[] elements;
    elements = nullptr;
    delete[] tableauIndex;
    tableauIndex = nullptr;
}

template <class T>
int Tableau<T>::taille() const
{
    return nbElements;
}

template <class T>
void Tableau<T>::ajouter(const T &item)
{
    assert(nbElements <= capacite);
    indexValide = false;
    if (nbElements == capacite)
    {
        redimensionner(capacite * 2);
    }
    elements[nbElements++] = item;
}

template <class T>
void Tableau<T>::inserer(const T &element, int index)
{
    assert(index < nbElements && index >= 0);
    indexValide = false;
    if (nbElements > capacite)
        redimensionner(capacite * 2);
    T precedent = element;
    nbElements++;
    for (int i = index; i < nbElements; i++)
    {
        T temp = elements[i];
        elements[i] = precedent;
        precedent = temp;
    }
}

template <class T>
void Tableau<T>::enlever(int index)
{
    assert(index < nbElements && index >= 0);
    indexValide = false;
    if (index == nbElements - 1)
    {
        nbElements--;
    }
    else
    {
        for (int i = index; i < nbElements - 1; i++)
        {
            elements[i] = elements[i + 1];
        }
        nbElements--;
    }
}

template <class T>
void Tableau<T>::enlever_dernier()
{
    if (!vide())
    {
        nbElements--;
    }
}

template <class T>
int Tableau<T>::chercher(const T &element) const // retourne -1 si non présent
{
    if (indexValide == false)
    {
        for (int i = 0; i < nbElements; i++)
        {
            if (element == elements[i])
            {
                return i;
            }
        }
        return -1;
    }
    else
    {
        int debut = 0, fin = nbElements - 1, result = -1;

        while (debut <= fin)
        {

            int milieu = (fin + debut) / 2;
            int indiceIndex = tableauIndex[milieu];
            T elementMilieu = elements[indiceIndex];

            if (element == elementMilieu)
            {
                result = milieu;
                fin = milieu - 1;
            }
            else if (element < elementMilieu)
            {
                fin = milieu - 1;
            }
            else
            {
                debut = milieu + 1;
            }
        }
        return result == -1 ? -1 : tableauIndex[debut];
    }
}

template <class T>
bool Tableau<T>::contient(const T &element) const
{
    return chercher(element) >= 0;
}

template <class T>
int Tableau<T>::chercherDernier(const T &element) const
{
    int debut = 0, fin = nbElements - 1, result = -1;
    while (debut <= fin)
    {

        int milieu = (debut + fin) / 2;
        int indiceIndex = tableauIndex[milieu];
        T elementMilieu = elements[indiceIndex];
        if (element == elementMilieu)
        {
            result = milieu;
            debut = milieu + 1;
        }
        else if (elementMilieu < element)
        {
            debut = milieu + 1;
        }
        else
        {
            fin = milieu - 1;
        }
    }
    return result;
}

template <class T>
int Tableau<T>::chercherPremier(const T &element) const
{
    int debut = 0, fin = nbElements - 1, result = -1;
    while (debut <= fin)
    {
        int milieu = (debut + fin) / 2;
        int indiceIndex = tableauIndex[milieu];
        T elementMilieu = elements[indiceIndex];

        if (element == elementMilieu)
        {
            result = milieu;
            fin = milieu - 1;
        }
        else if (element < elementMilieu)
        {
            fin = milieu - 1;
        }
        else
        {
            debut = milieu + 1;
        }
    }
    return result;
}

template <class T>
void Tableau<T>::vider()
{
    capacite = 4;
    nbElements = 0;
    delete[] elements;
    elements = new T[capacite];
    indexValide = false;
}

template <class T>
void Tableau<T>::redimensionner(int nouvCapacite)
{
    capacite = nouvCapacite;
    T *temp = new T[capacite];

    for (int i = 0; i < nbElements; i++)
    {
        temp[i] = elements[i];
    }
    delete[] elements;
    elements = temp;
}

template <class T>
T &Tableau<T>::operator[](int index)
{
    assert(index < nbElements);
    indexValide = false;
    return elements[index];
}

template <class T>
const T &Tableau<T>::operator[](int index) const
{
    assert(index < nbElements);
    return elements[index];
}

template <class T>
Tableau<T> &Tableau<T>::operator=(const Tableau<T> &autre)
{
    if (this == &autre)
    {
        return *this;
    }
    nbElements = autre.nbElements;
    if (capacite < autre.nbElements)
    {
        delete[] elements;
        capacite = autre.nbElements;
        elements = new T[capacite];
    }
    for (int i = 0; i < nbElements; i++)
    {
        elements[i] = autre.elements[i];
    }
    return *this;
}

template <class T>
bool Tableau<T>::operator==(const Tableau<T> &autre) const
{
    if (this == &autre)
        return true;
    if (nbElements != autre.nbElements)
        return false;
    for (int i = 0; i < nbElements; i++)
    {
        if (elements[i] != autre.elements[i])
            return false;
    }
    return true;
}

template <class T>
int Tableau<T>::compter(const T &element)
{
    if (indexValide == false)
    {
        int count = 0;
        for (int i = 0; i < nbElements; i++)
        {
            if (elements[i] == element)
            {
                count++;
            }
        }
        return count;
    }
    else
    {
        int dernier = chercherDernier(element);
        int premier = chercherPremier(element);
        return contient(element) ? dernier - premier + 1 : 0;
    }
}

template <class T>
void Tableau<T>::creer_index() const
{
    delete[] tableauIndex;
    tableauIndex = new int[nbElements];
    for (int i = 0; i < nbElements; i++)
    {
        tableauIndex[i] = i;
    }
    T elementsCopie[nbElements];
    for (int i = 0; i < nbElements; i++)
    {
        elementsCopie[i] = elements[i];
    }
    trier(elementsCopie, 0, nbElements - 1);

    indexValide = true;
}

template <class T>
void Tableau<T>::trier(T elementsCopie[], int debut, int fin) const
{
    if (debut < fin)
    {
        int partition = partitionner(elementsCopie, debut, fin);
        trier(elementsCopie, debut, (partition - 1));
        trier(elementsCopie, (partition + 1), fin);
    }
}

template <class T>
int Tableau<T>::partitionner(T elementsCopie[], int debut, int fin) const
{
    T pivot = elementsCopie[fin];
    int indexPartition = debut;

    for (int i = debut; i < fin; i++)
    {
        if (elementsCopie[i] < pivot)
        {
            int temp = tableauIndex[i];
            tableauIndex[i] = tableauIndex[indexPartition];
            tableauIndex[indexPartition] = temp;

            T tempCopie = elementsCopie[i];
            elementsCopie[i] = elementsCopie[indexPartition];
            elementsCopie[indexPartition] = tempCopie;

            indexPartition++;
        }
    }
    int temp2 = tableauIndex[fin];
    tableauIndex[fin] = tableauIndex[indexPartition];
    tableauIndex[indexPartition] = temp2;

    T temp2Copie = elementsCopie[fin];
    elementsCopie[fin] = elementsCopie[indexPartition];
    elementsCopie[indexPartition] = temp2Copie;

    return indexPartition;
}

#endif // define _TABLEAU___H_
