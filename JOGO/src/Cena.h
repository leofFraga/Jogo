#ifndef CENA_H_INCLUDED
#define CENA_H_INCLUDED

#include <string>
#include <vector>
#include <optional>
#include "Item.h"
#include "Jogador.h"

// Estrutura que representa um monstro que pode aparecer em uma cena
struct Monstro {
    std::string nome;          // nome do monstro
    bool podeUsarMagia;        // se o monstro pode usar magia
    int habilidade;            // valor da habilidade do monstro
    int sorte;                 // valor da sorte do monstro
    int energia;               // energia atual do monstro
    int tesouro;               // quantidade de tesouro carregado
    int provisoes;             // número de provisões carregadas
    std::optional<Item> item;  // item que o monstro pode carregar (opcional)
};

// Classe que representa uma cena do jogo
class Cena {
private:
    int ID;  // identificador único da cena (ex: 1, 2, 3...)
    std::string texto;  // texto descritivo da cena (narrativa ou combate)
    std::vector<std::pair<std::string, int>> opcoes; // pares de opção de texto + ID da próxima cena
    std::optional<Item> itemCena; // item encontrado na cena (se houver)
    bool isMonstro;  // define se a cena é de monstro
    Monstro monstro; // dados do monstro, se a cena for de combate

public:
    Cena(int ID); // construtor, recebe o ID da cena
    bool carregarCena(Jogador* jogador); // carrega os dados da cena do arquivo correspondente
    void mostrarCena(const Jogador& jogador) const; // exibe texto da cena e opções disponíveis
    int escolherOpcao(Jogador& jogador) const; // lê e trata a escolha do jogador
    int getID() const; // retorna o ID da cena
    std::vector<std::pair<std::string, int>> getOpcoes(); // retorna a lista de opções da cena
    std::optional<Item> getItem() const; // retorna o item da cena (se existir)
    bool ehMonstro() const; // verifica se é uma cena de combate
    Monstro getMonstro() const; // retorna os dados do monstro da cena
};

#endif // CENA_H_INCLUDED
