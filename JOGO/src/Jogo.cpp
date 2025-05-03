#include "Jogo.h"
#include "Batalha.h"
#include <iostream>
#include <fstream>
#include <sstream>

// Construtor: inicia jogo na cena 1
Jogo::Jogo() : cenaAtual(1) {}

// Tela de abertura e menu principal
void Jogo::telaAbertura() {
    int opcao = 0;
    do {
        std::cout << "===== Aventura na Masmorra =====\n";
        std::cout << "----- O Amuleto do Poder -----\n";
        std::cout << "1) Novo Jogo\n";
        std::cout << "2) Carregar Jogo\n";
        std::cout << "3) Créditos\n";
        std::cout << "4) Sair\n";
        std::cout << "\nEscolha: ";  // prompt de escolha
        std::cin >> opcao;

        switch (opcao) {
            case 1:
                novoJogo();      // inicia nova partida
                break;
            case 2:
                if (carregarJogo()) {  // carrega estado salvo
                    jogador.mostrarStatus();
                    jogar();            // retoma jogo salvo
                }
                break;
            case 3:
                std::cout << "Desenvolvido por Bernardo Krzysczak e Leonardo Frigas de Fraga\n\n";  // créditos
                break;
            case 4:
                std::cout << "Saindo...\n";  // encerra
                break;
            default:
                std::cout << "Opção inválida.\n";
        }
    } while (opcao != 4);
}

// Configura novo jogo: mago opcional, distribuição de pontos, itens iniciais
void Jogo::novoJogo() {
    char c;
    // pergunta até resposta válida
    do {
        std::cout << "Voce tem a opção de aprender a com um mago(s) ou com um cavaleiro(n); (s/n): " << std::flush;
        std::cin >> c;
        c = std::tolower(c);
        if (c != 's' && c != 'n')
            std::cout << "Opção inválida! Digite 's' ou 'n'.\n";
    } while (c != 's' && c != 'n');

    jogador.ehMago = (c == 's');  // define tipo de personagem
    jogador.ehCavaleiro = (c == 'n');  // define tipo de personagem

    if (jogador.ehMago) {
        // adiciona magia inicial
        jogador.inventario.adicionarItem(Item("Bola de Fogo", 'm', true, 0, 5));
    } else if (jogador.ehCavaleiro) {
        // arma inicial do cavaleiro
        jogador.inventario.adicionarItem(Item("Espada do Cavaleiro", 'm', true, 0, 5));
    }

    jogador.distribuirPontos();  // aloca 12 pontos entre H, E, S

    // itens básicos de combate
    if (jogador.ehMago) {
        jogador.inventario.adicionarItem(Item("Espada Curta", 'w', true, 1, 2));
    }

    jogador.inventario.adicionarItem(Item("Tunica", 'r', true, 0, 0));
    jogador.inventario.adicionarItem(Item("Provisões", 'p', false, 0, 0));

    jogador.mostrarStatus();  // exibe atributos e inventário
    jogar();                  // entra no loop de jogo
}

// Salva estado em save.txt
bool Jogo::salvarJogo() const {
    std::ofstream arquivo("save.txt");
    if (!arquivo.is_open()) {
        std::cerr << "Erro ao abrir arquivo de salvamento.\n";
        return false;
    }

    // seção jogador
    arquivo << "#JOGADOR\n";
    arquivo << "habilidade=" << jogador.getHabilidade() << "\n";
    arquivo << "energia=" << jogador.getEnergia() << "\n";
    arquivo << "sorte=" << jogador.getSorte() << "\n\n";

    // seção inventário
    arquivo << "#INVENTARIO\n";
    for (const auto& item : jogador.inventario.getItens()) {
        arquivo << "item=" << item.nome << ";" << item.tipo << ";"
                << (item.combate ? "1" : "0") << ";" << item.FA << ";" << item.dano << "\n";
    }
    arquivo << "\n";

    // seção cena
    arquivo << "#CENA\n";
    arquivo << "id=" << cenaAtual << "\n";
    arquivo.close();
    return true;
}

// Carrega estado de save.txt
bool Jogo::carregarJogo() {
    std::ifstream arquivo("save.txt");
    if (!arquivo.is_open()) {
        std::cerr << "Nenhum jogo salvo encontrado!\n";
        return false;
    }

    // leitura por seção (usando enum para saber qual parte está lendo)
    enum class Secao { NONE, JOGADOR, INVENTARIO, CENA } secao = Secao::NONE;
    std::string linha;
    int h=6, e=12, s=6, cenaID=1;
    std::vector<Item> itensCarregados;

    while (std::getline(arquivo, linha)) {
        if (linha == "#JOGADOR") secao = Secao::JOGADOR;
        else if (linha == "#INVENTARIO") secao = Secao::INVENTARIO;
        else if (linha == "#CENA") secao = Secao::CENA;
        else if (!linha.empty()) {
            auto pos = linha.find('=');
            std::string chave = linha.substr(0, pos);
            std::string valor = linha.substr(pos+1);

            switch (secao) {
                case Secao::JOGADOR:
                    if (chave == "habilidade") h = std::stoi(valor);
                    else if (chave == "energia") e = std::stoi(valor);
                    else if (chave == "sorte") s = std::stoi(valor);
                    break;

                case Secao::INVENTARIO:
                    if (linha.rfind("item=", 0) == 0) {
                        std::stringstream ss(linha.substr(5));
                        std::string nome, tipo, cmb, fa, dano;
                        std::getline(ss, nome, ';');
                        std::getline(ss, tipo, ';');
                        std::getline(ss, cmb, ';');
                        std::getline(ss, fa, ';');
                        std::getline(ss, dano, ';');
                        itensCarregados.emplace_back(nome, tipo[0], cmb == "1", std::stoi(fa), std::stoi(dano));
                    }
                    break;

                case Secao::CENA:
                    if (chave == "id") cenaID = std::stoi(valor);
                    break;

                default: break;
            }
        }
    }
    arquivo.close();

    // recria jogador e inventário
    jogador = Jogador();
    jogador.setHabilidade(h);
    jogador.setEnergia(e);
    jogador.setSorte(s);
    jogador.inventario.limpar();
    for (auto& it : itensCarregados)
        jogador.inventario.adicionarItem(it);

    cenaAtual = cenaID;
    std::cout << "Jogo Carregado.\n\n";
    return true;
}

// Loop principal: carrega cena, batalha ou narrativa
void Jogo::jogar() {
    while (true) {
        Cena cena(cenaAtual);  // cria objeto Cena

        if (!cena.carregarCena(&jogador)) {
            std::cout << "Cena não encontrada. FIM DE JOGO\n";
            break;
        }

        // se for batalha, entra em Batalha::iniciar()
        if (cena.ehMonstro()) {
            Batalha batalha(jogador, cena.getMonstro());
            bool venceu = batalha.iniciar();

            // Se vencer e houver item, mostrar imediatamente
            if (venceu && cena.getItem()) {
                std::cout << "Você encontrou: " << cena.getItem()->nome << "\n";
            }

            // Transição de cena baseada no resultado
            for (auto& op : cena.getOpcoes()) {
                if ((venceu && op.first == "Venceu o combate") ||
                    (!venceu && (op.first == "Perdeu o combate" || op.first == "Fuga bem-sucedida!"))) {
                    cenaAtual = op.second;
                    break;
                }
            }
            continue;
        }

        // narrativa normal
        cena.mostrarCena(jogador);
        int prox = cena.escolherOpcao(jogador);
        if (prox == -1) {
            std::cout << "Sem opções! FIM DE JOGO\n";
            break;
        }

        cenaAtual = prox;
        salvarJogo();  // grava após cada turno
    }
}
