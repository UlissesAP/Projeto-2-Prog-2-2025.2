#include <iostream>
#include <clocale>
#include "headers.h"
#include "mod_login_e_matricula/servico_login.h"
#include "mod_adminitrativo/entrada_admin.h"
#include "mod_lanchonete/entrada_vendedor.h"
#include "mod_instrumentos/entrada_locador.h"
#include "mod_area_do_professor/entrada_professor.h"
#include "mod_area_do_aluno/entrada_aluno.h"
#include <cstdlib> // ou stdlib.h
using namespace std;

void limpar_tela() {
    #ifdef _WIN32
        system("cls");   // Windows
    #else
        system("clear"); // Linux/macOS
    #endif
}

int main() {

    setlocale(LC_ALL, "pt_BR.UTF-8"); //Não funcina bem
    while (true)
    {
        limpar_tela();

        cout << "Bem-vindo ao Sistema de Gestão Escolar\n";
        cout << "1 - Fazer Login\n";
        cout << "0 - Sair\n";
        cout << "Escolha uma opção: ";
        int escolha;
        cin >> escolha;
        cin.ignore();

        if (escolha == 0) {
            cout << "Saindo do sistema. Até logo!\n";
            break;
        } else if (escolha == 1) {
            int id_usuario;
            char senha[30];
            cout << "Digite seu ID de usuário: ";
            cin >> id_usuario;
            cin.ignore();
            cout << "Digite sua senha: ";
            cin.getline(senha, 30);
            cout << "Fazendo verificação de login...\n";
            login_info info = validar_login(&id_usuario, senha);
            cout << "Login bem-sucedido!\n";
            system("pause");
            switch (info.info_basica.categoria) {
                case ALUNO:
                    cout << "Aluno\n";
                    janela_aluno(info);
                    break;
                case PROFESSOR:
                    cout << "Professor\n";
                    janela_professor(info);
                    break;
                case ADMIN:
                    cout << "Administrador\n";
                    janela_admin(info);
                    break;
                case VENDEDOR:
                    cout << "Vendedor\n";
                    janela_vendedor(info);
                    break;
                case LOCADOR:
                    cout << "Locador\n";
                    janela_locador(info);
                    break;  
                default:
                    cout << "TIPO DE USUARIO DESCONHECIDO\n";
                    break;
            }
        } else {

            cout << "Opção inválida. Tente novamente.\n";
        }
    }

    return 0;
}
