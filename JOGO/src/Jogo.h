#ifndef JOGO_H_INCLUDED
#define JOGO_H_INCLUDED
#include "Jogador.h"
#include "Cena.h"

class Jogo {
private:
    Jogador jogador; // instancia um jogador no jogo
    int cenaAtual;
public:
    Jogo(); // construtor
    void telaAbertura();
    void novoJogo();
    bool salvarJogo() const;
    bool carregarJogo();
    void jogar();
};
#endif // JOGO_H_INCLUDED
