#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <unistd.h>

using namespace std;

/* BAMAS BENDJEDDOU DE_SOUSA_GUEDES CILLEROS THIEL
 * Notre problème est inspiré du "Dining Savage Problem"
 * Le principe du problème est que des employés utilisent une imprimante a tour de role en utilisant une feuille
 * Lorsqu'un employé constate que l'imprimante est vide, il appel le technicien qui reremplit l'imprimante
 *
 * Cette solution correction à notre solution thread-safe donc sans interblocage
 */

mutex mut;
condition_variable imprimanteVide, imprimantePleine;
int feuilleRestante = 7;

void rechargerImprimante(unsigned M) {
    feuilleRestante += M;
}

void imprimerDocument() {
    feuilleRestante--;
}

void savage(unsigned id) {
    while (true) {
        unique_lock<mutex> lock(mut);
        if (feuilleRestante == 0) {
            imprimanteVide.notify_one();
            imprimantePleine.wait(lock);
        }

        cout << "L'employé n°" << id << " imprime un copie" << endl;

        imprimerDocument();
        lock.unlock();

        sleep(rand() % 10);
    }
}

void cook() {
    while (true) {
        unique_lock<mutex> lock(mut);
        imprimanteVide.wait(lock, [] { return feuilleRestante == 0; });

        cout << "L'imprimante est vide, le technicien la re-remplit " << endl;
        sleep(5);
        rechargerImprimante(7);
        cout << "Imprimante rechargée" << endl;

        imprimantePleine.notify_all();
    }
}

int main() {
    thread technicien(cook);
    thread employeUn(savage, 1);
    thread employeDeux(savage, 2);
    thread employeTrois(savage, 3);

    technicien.join();
    employeUn.join();
    employeDeux.join();
    employeTrois.join();

    return 0;
}
