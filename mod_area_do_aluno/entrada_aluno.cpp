#include <iostream>
#include "../headers.h"

using namespace std;

void janela_aluno(login_info info) {
    int escolha = -1;
    while (escolha != 0) {
        cout << "\nPainel do Aluno\n";
        cout << "1 - Visualizar Notas\n";
        cout << "2 - Verificar Frequência\n";
        cout << "3 - Visualizar Turmas\n";
        cout << "4 - Acessar Lanchonete\n";
        cout << "5 - Visualizar Eventos\n";
        cout << "0 - Sair\n";
        cout << "Escolha uma opção: ";
        cin >> escolha;
        cin.ignore();
        
        switch (escolha) {
            case 1:
                cout << "Visualizando notas...\n";
                break;
            case 2:
                cout << "Verificando frequência...\n";
                break;
            case 3:
                cout << "Visualizando turmas...\n";
                break;
            case 4:
                cout << "Acessando lanchonete...\n";
                break;
            case 5:
                cout << "Visualizando eventos...\n";
                break;
            case 0:
                cout << "Saindo do painel do aluno...\n";
                break;
            default:
                cout << "Opção inválida. Tente novamente.\n";
        }
    }
}
