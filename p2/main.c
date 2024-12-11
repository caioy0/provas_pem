/*--------------------------------------------------------*
* Disciplina: Programaçao Estruturada e Modular           *
*          Prof. Carlos Veríssimo                         *
*-------------------------------------------------------- *
* Objetivo do Programa: Atividade P2                      *
* Data - 06/12/2024                                       *
* Autores: Caio Yugo Saeda Uemura                         *
*--------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>

// Pacote
typedef struct {
    long idPacote;
    char conteudoPacote[100];
    char cidadeOrigem[50];
    char cidadeDestino[50];
    int status; // 1: Em processamento, 2: Em trânsito, 3: Em espera, 4: Entregue, 99: Extraviado
    char historico[5][200]; // Histórico de alterações de status
    int totalAlteracoes;    // Contador de alterações no histórico
} Pacote;

// Status
const char* statusParaTexto(int status) {
    switch (status) {
        case 1: return "Em processamento";
        case 2: return "Em trânsito";
        case 3: return "Em espera";
        case 4: return "Entregue";
        case 99: return "Extraviado";
        default: return "Status desconhecido";
    }
}

// Função para criar e inicializar um pacote
Pacote* criarPacote(long idPacote, char* conteudo, char* origem, char* destino) {
    Pacote* novoPacote = (Pacote*)malloc(sizeof(Pacote));
    if (novoPacote != NULL) {
        novoPacote->idPacote = idPacote;
        strcpy(novoPacote->conteudoPacote, conteudo);
        strcpy(novoPacote->cidadeOrigem, origem);
        strcpy(novoPacote->cidadeDestino, destino);
        novoPacote->status = 1; // Em processamento
        novoPacote->totalAlteracoes = 0;

        // Histórico
        sprintf(novoPacote->historico[novoPacote->totalAlteracoes++],
                "Status inicial: %s", statusParaTexto(1));
    }
    return novoPacote;
}

// Iniciar status
void incluirPacote(Pacote* pacotes[], int* totalPacotes, long* idSequencial) {
    char conteudo[100], origem[50], destino[50];
    
    printf("\n----------------------------\n");
    printf("Digite o conteúdo do pacote: ");
    scanf(" %[^\n]s", conteudo);
    printf("Digite a cidade de origem: ");
    scanf(" %[^\n]s", origem);
    printf("Digite a cidade de destino: ");
    scanf(" %[^\n]s", destino);

    pacotes[*totalPacotes] = criarPacote((*idSequencial)++, conteudo, origem, destino);
    if (pacotes[*totalPacotes] != NULL) {
        (*totalPacotes)++;
        printf("Pacote adicionado com sucesso!\n");
    } else {
        printf("Erro ao adicionar pacote. Memória insuficiente!\n");
    }
}

// Função para validar se a entrada contém apenas números
bool entradaNumero(const char* entrada) {
    for (int i = 0; entrada[i] != '\0'; i++) {
        if (!isdigit(entrada[i])) {
            return false; // Encontrou um caractere não numérico
        }
    }
    return true;
}

// Função para alterar o status de um pacote
void alterarStatus(Pacote* pacotes[], int totalPacotes) {
    char entradaID[50];
    long id;
    int novoStatus;
    bool encontrado = false;

    printf("Digite o ID do pacote que deseja alterar: ");
    scanf(" %[^\n]s", entradaID);

    if (!entradaNumero(entradaID)) {
        printf("\nErro: O ID deve conter apenas números.\n");
        return;
    }

    id = atol(entradaID);
    printf("Digite o novo status (1-Em processamento, 2-Em trânsito, 3-Em espera, 4-Entregue, 99-Extraviado): ");
    scanf("%d", &novoStatus);

    for (int i = 0; i < totalPacotes; i++) {
        if (pacotes[i]->idPacote == id) {
            // Verificar as regras de transição de status
            int statusAtual = pacotes[i]->status;
            bool transicaoValida = false;

            switch (statusAtual) {
                case 1: // Em processamento
                    if (novoStatus == 2 || novoStatus == 99) {
                        transicaoValida = true;
                    }
                    break;
                case 2: // Em trânsito
                    if (novoStatus == 3 || novoStatus == 4 || novoStatus == 99) {
                        transicaoValida = true;
                    }
                    break;
                case 3: // Em espera
                    if (novoStatus == 99) {
                        transicaoValida = true;
                    }
                    break;
                case 4: // Entregue
                    if (novoStatus == 99) {
                        transicaoValida = true;
                    }
                    break;
                case 99: // Extraviado
                    // Nenhuma alteracao possível a partir do status 99
                    printf("O pacote está extraviado. Não é possível alterar o status.\n");
                    return;
            }

            // Se a alteracao for válida, altera o status
            if (transicaoValida) {
                // Se houver espaço no histórico, adiciona a alteração de status
                if (pacotes[i]->totalAlteracoes < 5) {
                    sprintf(pacotes[i]->historico[pacotes[i]->totalAlteracoes++],
                            "Mudou de %s para %s",
                            statusParaTexto(pacotes[i]->status),
                            statusParaTexto(novoStatus));
                }
                pacotes[i]->status = novoStatus;
                printf("Status do pacote alterado com sucesso!\n");
            } else {
                printf("Transição inválida do status %s para %s.\n",
                        statusParaTexto(statusAtual),
                        statusParaTexto(novoStatus));
            }
            encontrado = true;
            break;
        }
    }
    if (!encontrado) {
        printf("\nPacote com ID %ld não encontrado.\n", id);
    }
}


// Rastrear pacote
void rastrearPacote(Pacote* pacotes[], int totalPacotes) {
    char entradaID[50];
    long id;
    bool encontrado = false;

    printf("Digite o ID do pacote que deseja rastrear: ");
    scanf(" %[^\n]s", entradaID);

    if (!entradaNumero(entradaID)) {
        printf("\nErro: O ID deve conter apenas números.\n");
        return;
    }

    id = atol(entradaID);

    for (int i = 0; i < totalPacotes; i++) {
        if (pacotes[i]->idPacote == id) {
            printf("\n----------------------------\n");
            printf("Pacote ID: %ld\n", pacotes[i]->idPacote);
            printf("Conteúdo: %s\n", pacotes[i]->conteudoPacote);
            printf("Origem: %s\n", pacotes[i]->cidadeOrigem);
            printf("Destino: %s\n", pacotes[i]->cidadeDestino);
            printf("Status atual: %s\n", statusParaTexto(pacotes[i]->status));
            printf("\nHistórico de alterações:\n");
            for (int j = 0; j < pacotes[i]->totalAlteracoes; j++) {
                printf("  - %s\n", pacotes[i]->historico[j]);
            }
            printf("\n----------------------------\n");
            encontrado = true;
            break;
        }
    }
    if (!encontrado) {
        printf("\nPacote com ID %ld não encontrado.\n", id);
    }
}

// Função para listar todos os pacotes em ordem decrescente
void listarPacotes(Pacote* pacotes[], int totalPacotes) {
    if (totalPacotes == 0) {
        printf("Não há pacotes cadastrados.\n");
    } else {
        for (int i = totalPacotes - 1; i >= 0; i--) {
            printf("\n----------------------------\n");
            printf("Pacote ID: %ld\n", pacotes[i]->idPacote);
            printf("Conteúdo: %s\n", pacotes[i]->conteudoPacote);
            printf("Origem: %s\n", pacotes[i]->cidadeOrigem);
            printf("Destino: %s\n", pacotes[i]->cidadeDestino);
            printf("Status atual: %s\n", statusParaTexto(pacotes[i]->status));
            printf("\n----------------------------\n");
        }
    }
}

// Função principal
int main() {
    Pacote* pacotes[100]; // Array de ponteiros para pacotes
    int totalPacotes = 0;
    long idSequencial = 2040482312014; // ID inicial
    int opcao;

    do {
        printf("\nMenu de opções:\n");
        printf("1 - Incluir pacote\n");
        printf("2 - Alterar status do pacote\n");
        printf("3 - Rastrear pacote\n");
        printf("4 - Listar todos os pacotes\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: // Adiciona um pacote
                incluirPacote(pacotes, &totalPacotes, &idSequencial);
                break;
            case 2: // altera o Status
                alterarStatus(pacotes, totalPacotes);
                break;
            case 3: // Rastreia o pacote
                rastrearPacote(pacotes, totalPacotes);
                break;
            case 4: // Lista todos os pacotes
                listarPacotes(pacotes, totalPacotes);
                break;
            case 0: // Sai do programa
                printf("Obrigado por utilizar o sistema!\n");
                return 0;
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
    } while (true);

    return 0;
}
