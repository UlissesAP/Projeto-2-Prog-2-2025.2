#include <iostream>

using namespace std;

void janela_locador(login_info info) {
    int escolha = -1;
    while (escolha != 0) {
        cout << "\nGerenciador de Empréstimos de Instrumentos\n";
        cout << "1 - Registrar Empréstimo\n";
        cout << "2 - Registrar Devolução\n";
        cout << "3 - Visualizar Empréstimos Ativos\n";
        cout << "4 - Listar Instrumentos Disponíveis\n";
        cout << "5 - Gerar Relatório de Atrasos\n";
        cout << "0 - Voltar\n";
        cout << "Escolha uma opção: ";
        cin >> escolha;
        cin.ignore();
        
        switch (escolha) {
            case 1:
                cout << "Registrando empréstimo...\n";
                break;
            case 2:
                cout << "Registrando devolução...\n";
                break;
            case 3:
                cout << "Visualizando empréstimos ativos...\n";
                break;
            case 4:
                cout << "Listando instrumentos disponíveis...\n";
                break;
            case 5:
                cout << "Gerando relatório de atrasos...\n";
                break;
            case 0:
                cout << "Voltando ao painel anterior...\n";
                break;
            default:
                cout << "Opção inválida. Tente novamente.\n";
        }
    }
}
