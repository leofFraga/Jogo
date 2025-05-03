#ifndef INVENTARIO_H_INCLUDED
#define INVENTARIO_H_INCLUDED

#include <vector>
#include "Item.h"

// Classe que representa o inventário do jogador
class Inventario {
private:
    std::vector<Item> itens; // vetor que armazena os itens do inventário

public:
    // Adiciona um item ao inventário
    void adicionarItem(const Item& item);

    // Exibe todos os itens do inventário
    void listarItens() const;

    // Retorna referência constante à lista de itens
    const std::vector<Item>& getItens() const;

    // Verifica se há itens no inventário
    bool temItens() const;

    // Remove todos os itens do inventário
    void limpar();

    // Remove um item pelo índice (posição no vetor)
    void removerItem(size_t indice);

    // Consome uma provisão (tipo 'p') e retorna true se usado com sucesso
    bool usarProvisao();

    // Verifica se há um item específico pelo nome
    bool possuiItem(const std::string& nomeItem) const;
};

#endif // INVENTARIO_H_INCLUDED
