#include <iostream>
#include "../headers.h"

using namespace std;

void janela_gerenciar_eventos(login_info info) {
    int escolha = -1;
    while (escolha != 0) {
        cout << "\nGerenciamento de Eventos\n";
        cout << "1 - Criar Novo Evento\n";
        cout << "2 - Editar Evento\n";
        cout << "3 - Listar Eventos\n";
        cout << "0 - Voltar\n";
        cout << "Escolha uma opção: ";
        cin >> escolha;
        cin.ignore();
        
        switch (escolha) {
            case 1:
                cout << "Criar novo evento...\n";
                break;
            case 2:
                cout << "Editar evento...\n";
                break;
            case 3:
                cout << "Listar eventos...\n";
                break;
            case 0:
                cout << "Voltando ao painel anterior...\n";
                break;
            default:
                cout << "Opção inválida. Tente novamente.\n";
        }
    }
}

void janela_professor(login_info info) {
    int escolha = -1;
    while (escolha != 0) {
        cout << "\nPainel do Professor\n";
        cout << "1 - Gerenciar Eventos\n";
        cout << "2 - Lançar Notas\n";
        cout << "3 - Registrar Frequência\n";
        cout << "4 - Visualizar Turmas\n";
        cout << "0 - Sair\n";
        cout << "Escolha uma opção: ";
        cin >> escolha;
        cin.ignore();
        
        switch (escolha) {
            case 1:
                janela_gerenciar_eventos(info);
                break;
            case 2:
                cout << "Lançar notas...\n";
                break;
            case 3:
                cout << "Registrar frequência...\n";
                break;
            case 4:
                cout << "Visualizar turmas...\n";
                break;
            case 0:
                cout << "Saindo do painel do professor...\n";
                break;
            default:
                cout << "Opção inválida. Tente novamente.\n";
        }
    }
}
