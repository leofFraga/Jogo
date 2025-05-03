#include "Inventario.h"
#include <iostream>

// Adiciona um item ao inventário
void Inventario::adicionarItem(const Item& item) {
    itens.push_back(item);
}

// Lista todos os itens do inventário com seu nome e tipo
void Inventario::listarItens() const {
    if (itens.empty()) {
        std::cout << "O inventario esta vazio.\n";
        return;
    }
    std::cout << "\nInventario:\n";
    for (size_t i = 0; i < itens.size(); i++) {
        std::cout << i + 1 << ". " << itens[i].nome
                  << " (Tipo: " << itens[i].tipo << ")\n";
    }
}

// Retorna a referência constante ao vetor de itens
const std::vector<Item>& Inventario::getItens() const {
    return itens;
}

// Limpa todos os itens do inventário
void Inventario::limpar() {
    itens.clear();
}

// Remove um item pelo índice (começa em 0)
void Inventario::removerItem(size_t indice) {
    if (indice < itens.size()) {
        itens.erase(itens.begin() + indice);
    }
}

// Verifica se o inventário contém um item com o nome especificado
bool Inventario::possuiItem(const std::string& nomeItem) const {
    for (const auto& item : itens) {
        if (item.nome == nomeItem) {
            return true;
        }
    }
    return false;
}

// Usa a primeira provisão encontrada no inventário (tipo 'p') e a remove
bool Inventario::usarProvisao() {
    for (size_t i = 0; i < itens.size(); ++i) {
        if (itens[i].tipo == 'p') {
            itens.erase(itens.begin() + i); // remove a provisão
            return true;
        }
    }
    return false;
}
