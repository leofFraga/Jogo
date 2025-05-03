#include "Batalha.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <limits>

// Construtor da classe Batalha, inicializa com uma referência ao jogador e o monstro
Batalha::Batalha(Jogador& j, const Monstro& m)
    : jogador(j), monstro(m)
{
    // Semente para gerar números aleatórios diferentes a cada execução
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

// Função principal que executa a batalha entre jogador e monstro
bool Batalha::iniciar() {
    // Inicializa variáveis locais com os atributos atuais do jogador e do monstro
    int energiaMonstro = monstro.energia;
    int energiaJogador = jogador.getEnergia();
    int sorteAtual     = jogador.getSorte();

    std::cout << "\nBATALHA INICIADA contra " << monstro.nome << "!\n";

    // Define o comportamento do turno do monstro (lambda function)
    auto turnoMonstro = [&]() {
        int ataque = rand() % 10 + 1 + monstro.habilidade;         // rola 1d10 + habilidade
        int defesa = rand() % 10 + 1 + jogador.getHabilidade();    // rola 1d10 + habilidade do jogador
        if (ataque > defesa) {
            energiaJogador -= 2;
            std::cout << monstro.nome << " ataca e causa 2 de dano!\n";
        } else {
            std::cout << monstro.nome << " tentou atacar mas voce defendeu!\n";
        }
        jogador.setEnergia(energiaJogador); // atualiza energia do jogador
    };

    // Loop principal da batalha
    while (energiaJogador > 0 && energiaMonstro > 0) {
        // Exibe status
        std::cout << "\nSua energia: " << energiaJogador
                  << " | Energia do " << monstro.nome << ": " << energiaMonstro << "\n";

        // Exibe menu de ações
        std::cout << "1) Atacar\n";
        std::cout << "2) Fugir\n";
        if (jogador.ehMago) std::cout << "3) Magia\n";
        if (jogador.ehCavaleiro) std::cout << "3) Ataque do cavaleiro\n";
        std::cout << "4) Provisao\n";
        std::cout << "Opção: ";

        int escolha;
        std::cin >> escolha;

        // Caso o jogador digite algo inválido (ex: letra)
        if (std::cin.fail()) {
            std::cin.clear(); // limpa o erro
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignora o restante da entrada
            std::cout << "Entrada invalida.\n";
            continue;
        }

        bool goblinAtaca = false; // define se o monstro vai contra-atacar no final

        // --- FUGA ---
        if (escolha == 2) {
            std::cout << "Você tentou fugir...\n";
            if (rand() % 2 == 1) {
                std::cout << "Fuga bem-sucedida!\n";
                jogador.setEnergia(energiaJogador);
                return false; // fuga bem-sucedida termina a batalha sem vitória
            }
            std::cout << "Fuga falhou! O monstro ataca!\n";
            goblinAtaca = true;
        }

        // --- ATAQUE FÍSICO ---
        else if (escolha == 1) {
            int dadoJ = rand() % 10 + 1;
            int FAJ   = dadoJ + jogador.getHabilidade();
            int dadoM = rand() % 10 + 1;
            int FAM   = dadoM + monstro.habilidade;

            std::cout << "Voce rolou " << dadoJ << " + hab(" << jogador.getHabilidade()
                      << ") = " << FAJ << "\n";
            std::cout << monstro.nome << " rolou " << dadoM << " + hab(" << monstro.habilidade
                      << ") = " << FAM << "\n";

            if (FAJ > FAM) {
                energiaMonstro -= 1;
                std::cout << "Voce acertou! -1 de energia no " << monstro.nome
                          << " (restam " << energiaMonstro << ")\n";

                // O jogador pode tentar usar sorte para causar mais dano
                std::cout << "Testar Sorte? (s/n): ";
                char r; std::cin >> r;
                if ((r=='s'||r=='S') && testarSorte(sorteAtual)) {
                    energiaMonstro -= 2;
                    std::cout << "Sorte boa! +2 de dano (restam "
                              << energiaMonstro << ")\n";
                }
                jogador.setSorte(sorteAtual); // atualiza a sorte gasta
            }
            else if (FAM > FAJ) {
                energiaJogador -= 2;
                std::cout << monstro.nome << " acertou voce! Energia sua: "
                          << energiaJogador << "\n";
            }
            else {
                std::cout << "Empate! Nenhum dano causado.\n";
            }
        }

        // --- ATAQUE MÁGICO ---
        else if (escolha == 3 && jogador.ehMago) {
            const int custo = 1;
            if (energiaJogador >= custo) {
                int dadoJ = rand() % 10 + 1;
                int FAJ = dadoJ + jogador.getHabilidade() + 2; // bônus mágico
                int dadoM = rand() % 10 + 1;
                int FAM = dadoM + monstro.habilidade;

                std::cout << "Voce conjurou magia: " << dadoJ << " + hab(" << jogador.getHabilidade()
                          << ") + bonus(2) = " << FAJ << "\n";
                std::cout << monstro.nome << " rolou " << dadoM << " + hab(" << monstro.habilidade
                          << ") = " << FAM << "\n";

                if (FAJ > FAM) {
                    energiaMonstro -= 3;
                    std::cout << "Magia bem-sucedida! -3 no " << monstro.nome
                              << " (restam " << energiaMonstro << ")\n";

                    std::cout << "Testar Sorte? (s/n): ";
                    char r; std::cin >> r;
                    if ((r == 's' || r == 'S') && testarSorte(sorteAtual)) {
                        energiaMonstro -= 2;
                        std::cout << "Sorte boa! +2 de dano (restam "
                                  << energiaMonstro << ")\n";
                    }
                    jogador.setSorte(sorteAtual);
                } else if (FAM > FAJ) {
                    energiaJogador -= 2;
                    std::cout << monstro.nome << " resistiu e contra-atacou! Energia sua: "
                              << energiaJogador << "\n";
                    goblinAtaca = false;
                } else {
                    std::cout << "Empate! Nenhum dano causado.\n";
                }

                energiaJogador -= custo; // custo da magia
                jogador.setEnergia(energiaJogador);
                goblinAtaca = true;
            } else {
                std::cout << "Energia insuficiente para conjurar.\n";
            }
        }

        // --- ATAQUE ESPECIAL DO CAVALEIRO ---
        else if (escolha == 3 && jogador.ehCavaleiro) {
            const int custo = 1;
            if (energiaJogador >= custo) {
                int dadoJ = rand() % 10 + 1;
                int FAJ = dadoJ + jogador.getHabilidade() + 2;
                int dadoM = rand() % 10 + 1;
                int FAM = dadoM + monstro.habilidade;

                std::cout << "Voce usou ataque especial: " << dadoJ << " + hab(" << jogador.getHabilidade()
                          << ") + bonus(1) = " << FAJ << "\n";
                std::cout << monstro.nome << " rolou " << dadoM << " + hab(" << monstro.habilidade
                          << ") = " << FAM << "\n";

                if (FAJ > FAM) {
                    energiaMonstro -= 3;
                    std::cout << "Ataque especial acertou! -3 no " << monstro.nome
                              << " (restam " << energiaMonstro << ")\n";

                    std::cout << "Testar Sorte? (s/n): ";
                    char r; std::cin >> r;
                    if ((r == 's' || r == 'S') && testarSorte(sorteAtual)) {
                        energiaMonstro -= 2;
                        std::cout << "Sorte boa! +2 de dano (restam "
                                  << energiaMonstro << ")\n";
                    }
                    jogador.setSorte(sorteAtual);
                } else if (FAM > FAJ) {
                    energiaJogador -= 2;
                    std::cout << monstro.nome << " contra-atacou! Energia sua: "
                              << energiaJogador << "\n";
                    goblinAtaca = false;
                } else {
                    std::cout << "Empate! Nenhum dano causado.\n";
                }

                energiaJogador -= custo;
                jogador.setEnergia(energiaJogador);
                goblinAtaca = true;
            } else {
                std::cout << "Energia insuficiente para o ataque especial.\n";
            }
        }

        // --- PROVISÃO ---
        else if (escolha == 4) {
            if (jogador.inventario.usarProvisao()) {
                energiaJogador += 4;
                std::cout << "Provisao usada! +4 de energia.\n";
            } else {
                std::cout << "Sem provisoes disponiveis.\n";
            }
            goblinAtaca = true;
        }

        // --- OPÇÃO INVÁLIDA ---
        else {
            std::cout << "Opção inválida. Tente novamente.\n";
            continue;
        }

        jogador.setEnergia(energiaJogador); // atualiza energia

        // Monstro ataca se ainda estiver vivo e for turno de resposta
        if (goblinAtaca && energiaMonstro > 0) {
            turnoMonstro();
        }
    }

    // Resultado da batalha
    if (energiaMonstro <= 0) {
        std::cout << "Você derrotou o " << monstro.nome << "! Parabéns!\n";
        return true;
    } else {
        std::cout << "Você foi derrotado pelo " << monstro.nome << "...\n";
        return false;
    }
}

// ============================================================================
// Função que testa a sorte do jogador. Consome 1 ponto de sorte.
// Se o valor rolado (1-12) for menor ou igual à sorte restante, sucesso.
// ============================================================================
bool Batalha::testarSorte(int& sorteAtual) {
    if (sorteAtual <= 0) {
        std::cout << "Sem sorte restante.\n";
        return false;
    }

    int v = rand() % 12 + 1;
    --sorteAtual; // consome 1 ponto

    bool sucesso = (v <= sorteAtual);
    std::cout << "Rolagem de sorte: " << v
              << " vs sorte restante: " << sorteAtual
              << (sucesso ? " -> Sucesso!\n" : " -> Falhou.\n");
    return sucesso;
}
