#ifndef JOGADOR_H_INCLUDED
#define JOGADOR_H_INCLUDED

#include "Inventario.h"

// Classe que representa o jogador no jogo
class Jogador {
private:
    int habilidade; // valor da habilidade (usado em combate)
    int energia;    // energia (vida) atual do jogador
    int sorte;      // valor atual de sorte (usado para testes de sorte)

public:
    Inventario inventario; // inventário do jogador contendo itens, armas, etc.

    bool ehMago;        // define se o jogador é um mago
    bool ehCavaleiro;   // define se o jogador é um cavaleiro

    Jogador();          // construtor padrão

    void distribuirPontos();     // distribui os 12 pontos entre habilidade, energia e sorte

    // métodos de acesso (getters)
    int getHabilidade() const;
    int getEnergia() const;
    int getSorte() const;

    // métodos de modificação (setters)
    void setHabilidade(int h);
    void setEnergia(int e);
    void setSorte(int s);

    // exibe o status do jogador na tela
    void mostrarStatus() const;
};

#endif // JOGADOR_H_INCLUDED
