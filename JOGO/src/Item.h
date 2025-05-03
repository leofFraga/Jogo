#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED
#include <string>

class Item {
public:
    std::string nome;
    char tipo; // c: item comum, r: armadura, w: arma
    bool combate; // se pode ser usado em combate ou não
    int FA; // bônus de força de ataque
    int dano; // bônus de dano

    Item() : nome(""), tipo('c'), combate(false), FA(0), dano(0) {}
    Item(std::string n, char t, bool c, int fa, int d) : nome(n), tipo(t), combate(c), FA(fa), dano(d) {} // Construtor
};
#endif // ITEM_H_INCLUDED
