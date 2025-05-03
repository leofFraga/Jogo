#ifndef BATALHA_H_INCLUDED
#define BATALHA_H_INCLUDED
#include "Jogador.h"
#include "Cena.h"

class Batalha {
private:
    Jogador& jogador; // instancia um jogador como endereço do tipo Jogador
    Monstro monstro;
public:
    Batalha(Jogador& j, const Monstro& m); // construtor
    bool iniciar(); // retorna verdadeiro se venceu, false se perdeu ou fugiu
    bool testarSorte(int& sorteAtual);
};
#endif // BATALHA_H_INCLUDED
