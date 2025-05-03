#include "Jogador.h"
#include "Item.h"
#include <iostream>

// Construtor padrão: inicializa os atributos mínimos e define que não é mago nem cavaleiro
Jogador::Jogador() : habilidade(6), energia(12), sorte(6), ehMago(false), ehCavaleiro(false) {

}

// Função para distribuir os 12 pontos adicionais entre os atributos
void Jogador::distribuirPontos() {
    int pontos = 12; // pontos a serem distribuídos
    int h; // habilidade
    int e; // energia
    int s; // sorte

    std::cout << "\nDistribua 12 pontos entre Habilidade, Energia e Sorte\n";

    while (true) {
        std::cout << "Habilidade (6 a 12): ";
        std::cin >> h;
        if (h < 6 || h > 12) {
            std::cout << "Valor inválido.\n";
            continue;
        }

        std::cout << "Energia (12 a 24): ";
        std::cin >> e;
        if (e < 12 || e > 24) {
            std::cout << "Valor inválido.\n";
            continue;
        }

        std::cout << "Sorte (6 a 12): ";
        std::cin >> s;
        if (s < 6 || s > 12) {
            std::cout << "Valor inválido.\n";
            continue;
        }

        // verifica se os pontos extras foram corretamente distribuídos
        if ((h - 6) + (e - 12) + (s - 6) == pontos) break;
        else std::cout << "Você deve alocar todos os 12 pontos. Tente novamente.\n";
    }

    // aplica os valores definidos
    habilidade = h;
    energia = e;
    sorte = s;
}

// Retorna o valor da habilidade
int Jogador::getHabilidade() const {
    return habilidade;
}

// Retorna a energia atual
int Jogador::getEnergia() const {
    return energia;
}

// Retorna a sorte atual
int Jogador::getSorte() const {
    return sorte;
}

// Define um novo valor de habilidade
void Jogador::setHabilidade(int h) {
    habilidade = h;
}

// Define um novo valor de energia
void Jogador::setEnergia(int e) {
    energia = e;
}

// Define um novo valor de sorte
void Jogador::setSorte(int s) {
    sorte = s;
}

// Mostra os status do jogador e o conteúdo do inventário
void Jogador::mostrarStatus() const {
    std::cout << "\nStatus:\n";
    std::cout << "Habilidade: " << habilidade << "\n";
    std::cout << "Energia: " << energia << "\n";
    std::cout << "Sorte: " << sorte << "\n";
    inventario.listarItens();  // exibe todos os itens
}
