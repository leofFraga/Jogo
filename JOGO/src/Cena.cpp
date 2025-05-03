#include "Cena.h"
#include <iostream>
#include <fstream>
#include <sstream>

// Construtor da classe Cena, inicializa o ID da cena e indica que inicialmente não há monstro
Cena::Cena(int ID) : ID(ID), isMonstro(false) {}

// Função que carrega os dados de uma cena a partir de um arquivo de texto
bool Cena::carregarCena(Jogador* jogador) {
    // Abre o arquivo da cena com base no ID
    std::ifstream arquivo("../cenas/" + std::to_string(ID) + ".txt");
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir arquivo de cena " << ID << "\n";
        return false;
    }

    std::string linha;
    texto = "";              // Limpa o texto da cena
    opcoes.clear();          // Limpa as opções anteriores
    isMonstro = false;       // Assume que não é uma cena com monstro
    itemCena = std::nullopt; // Reseta item da cena

    // Lê a primeira linha do arquivo
    if (!getline(arquivo, linha)) return false;

    // Se a cena é de monstro (identificada pela letra 'm' na primeira linha)
    if (linha == "m") {
        isMonstro = true;
        texto = "";

        // Lê o texto de introdução da cena até encontrar os dados do monstro
        while (getline(arquivo, linha)) {
            if (linha.empty()) continue;
            if (linha[0] == 'N' || linha[0] == 'M' || linha[0] == 'H' ||
                linha[0] == 'S' || linha[0] == 'E' || linha[0] == 'T' ||
                linha[0] == 'P' || linha[0] == 'I') {
                break; // Chegou na parte dos dados do monstro ou item
            }
            texto += linha + "\n";
        }

        // Lê os dados do monstro e possivelmente o item que ele carrega
        do {
            if (linha.empty()) break;

            if (linha[0] == 'N') {
                monstro.nome = linha.substr(2);
            } else if (linha[0] == 'M') {
                monstro.podeUsarMagia = (linha.substr(3,1) == "S");
            } else if (linha[0] == 'H') {
                monstro.habilidade = std::stoi(linha.substr(3));
            } else if (linha[0] == 'S') {
                monstro.sorte = std::stoi(linha.substr(3));
            } else if (linha[0] == 'E') {
                monstro.energia = std::stoi(linha.substr(3));
            } else if (linha[0] == 'T') {
                monstro.tesouro = std::stoi(linha.substr(3));
            } else if (linha[0] == 'P') {
                monstro.provisoes = std::stoi(linha.substr(3));
            } else if (linha[0] == 'I') {
                // Trata o item do monstro, separado por ponto e vírgula
                std::stringstream ss(linha.substr(3));
                std::string nome, tipoStr, combateStr, faStr, danoStr;

                std::getline(ss, nome, ';');
                std::getline(ss, tipoStr, ';');
                std::getline(ss, combateStr, ';');
                std::getline(ss, faStr, ';');
                std::getline(ss, danoStr, ';');

                char tipo = tipoStr.empty() ? 'c' : tipoStr[0];
                bool combate = (combateStr == "1");
                int fa = std::stoi(faStr);
                int dano = std::stoi(danoStr);

                itemCena = Item(nome, tipo, combate, fa, dano);
                jogador->inventario.adicionarItem(*itemCena);
            } else {
                break;
            }
        } while (getline(arquivo, linha));

        // Lê o ID da próxima cena em caso de vitória e derrota
        if (!linha.empty()) {
            size_t pos = linha.find(';');
            if (pos != std::string::npos) {
                int IDVitoria = std::stoi(linha.substr(0, pos));
                int IDDerrota = std::stoi(linha.substr(pos + 1));
                opcoes.push_back({"Venceu o combate", IDVitoria});
                opcoes.push_back({"Perdeu o combate", IDDerrota});
            }
        }

    } else {
        // Se não for cena de monstro, lê normalmente as linhas de texto da cena
        texto = "";
        while (getline(arquivo, linha)) {
            if (linha.empty()) continue;

            if (linha.rfind("I:", 0) == 0) {
                // Processa um item encontrado na cena
                std::stringstream ss(linha.substr(3));
                std::string nome, tipoStr, combateStr, faStr, danoStr;

                std::getline(ss, nome, ';');
                std::getline(ss, tipoStr, ';');
                std::getline(ss, combateStr, ';');
                std::getline(ss, faStr, ';');
                std::getline(ss, danoStr, ';');

                char tipo = tipoStr.empty() ? 'c' : tipoStr[0];
                bool combate = (combateStr == "1");
                int fa = std::stoi(faStr);
                int dano = std::stoi(danoStr);

                itemCena = Item(nome, tipo, combate, fa, dano);
                jogador->inventario.adicionarItem(*itemCena);
                texto += "\nVocê encontrou um item: " + itemCena->nome + "\n";
            }
            else if (linha[0] == '#') {
                break; // Começo das opções
            }
            else {
                texto += linha + "\n"; // Texto descritivo da cena
            }
        }

        // Leitura das opções da cena
        do {
            if (linha.empty()) continue;
            if (linha[0] == '#') {
                size_t pos = linha.find(':');
                if (pos == std::string::npos) continue;
                int proxID = std::stoi(linha.substr(1, pos - 1));
                std::string desc = linha.substr(pos + 1);
                if (!desc.empty() && desc[0] == ' ') desc.erase(0, 1);
                opcoes.push_back({desc, proxID});
            }
        } while (getline(arquivo, linha));
    }

    arquivo.close();
    return true;
}

// Retorna o item da cena (se existir)
std::optional<Item> Cena::getItem() const {
    return itemCena;
}

// Mostra o texto da cena e as opções disponíveis ao jogador
void Cena::mostrarCena(const Jogador& jogador) const {
    std::cout << "\n" << texto << "\n";
    for (size_t i = 0; i < opcoes.size(); i++) {
        // Oculta opções que requerem chave, se o jogador não a possuir
        if (opcoes[i].first.find("[Usar Chave]") != std::string::npos &&
            !jogador.inventario.possuiItem("Chave Antiga")) {
            continue;
        }
        std::cout << i + 1 << ") " << opcoes[i].first << "\n";
    }
    std::cout << "0) Abrir inventário\n";
}

// Permite o jogador escolher uma das opções da cena
int Cena::escolherOpcao(Jogador& jogador) const {
    if (opcoes.empty()) return -1;
    int escolha = -1;
    while (true) {
        std::cout << "Escolha uma opção: ";
        std::cin >> escolha;
        if (escolha == 0) {
            jogador.mostrarStatus();
        } else if (escolha >= 1 && escolha <= (int)opcoes.size()) {
            return opcoes[escolha - 1].second;
        } else {
            std::cout << "Opção inválida.\n";
        }
    }
}

// Retorna o ID da cena atual
int Cena::getID() const {
    return ID;
}

// Retorna as opções da cena
std::vector<std::pair<std::string, int>> Cena::getOpcoes() {
    return opcoes;
}

// Indica se essa cena é uma cena de monstro
bool Cena::ehMonstro() const {
    return isMonstro;
}

// Retorna o monstro da cena (caso exista)
Monstro Cena::getMonstro() const {
    return monstro;
}
