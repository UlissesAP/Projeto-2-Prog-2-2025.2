#include <iostream>
#include "services.h"

using namespace std;

void janela_admin(login_info info) {
    int i = 0;
    int escolha = -1;
    while (escolha != 0) {
        cout << "\nPainel do Administrador\n";
        cout << "1 - Cadastrar Professor\n";
        cout << "2 - Cadastrar Aluno\n";
        cout << "3 - Cadastrar Vendedor\n";
        cout << "4 - Cadastrar Locador\n";
        cout << "5 - Gerenciar Eventos\n";
        cout << "0 - Sair do Painel Administrativo\n";
        cout << "Escolha uma opção: ";
        cin >> escolha;
        cin.ignore();
        switch (escolha)
        {
        case 1:
            cout << "Cadastrando professor...\n";
            break;
        case 2:
            cout << "Cadastrando aluno...\n";
            break;
        case 3:
            cout << "Cadastrando vendedor...\n";
            break;
        case 4:
            cout << "Cadastrando locador...\n";
            break;
        case 5:
            cout << "Gerenciando eventos...\n";
            break;
        case 0:
            cout << "Saindo do painel administrativo...\n";
            break;
        default:
            cout << "Opção inválida. Tente novamente.\n";
            break;
        }
    }
}

