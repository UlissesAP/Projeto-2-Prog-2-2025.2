#include "instrumentos.h"
#include <fstream>
#include <cstring>

using namespace std;

// --- Variáveis Globais do Módulo (Simplificação) ---
// Em vez de um struct Gerenciador complexo, usamos um array estático simples.
const int MAX_INSTRUMENTOS = 100; // Limite simples
Instrumento listaInstrumentos[MAX_INSTRUMENTOS];
int quantidadeInstrumentos = 0;

const char* ARQUIVO_DADOS = "instrumentos.dat";

// --- Funções Auxiliares de Arquivo ---

void carregarDados() {
    ifstream file(ARQUIVO_DADOS, ios::binary);
    if (!file.is_open()) return; // Arquivo não existe ainda

    // Lê a quantidade primeiro (salvamos no início do arquivo)
    file.read((char*)&quantidadeInstrumentos, sizeof(int));

    // Lê o array inteiro de uma vez (mais simples e rápido)
    if (quantidadeInstrumentos > 0) {
        file.read((char*)listaInstrumentos, quantidadeInstrumentos * sizeof(Instrumento));
    }
    file.close();
}

void salvarDados() {
    ofstream file(ARQUIVO_DADOS, ios::binary | ios::trunc);
    if (!file.is_open()) {
        cout << "Erro ao salvar arquivo de instrumentos!" << endl;
        return;
    }

    // Salva a quantidade primeiro
    file.write((char*)&quantidadeInstrumentos, sizeof(int));
    
    // Salva o array inteiro
    if (quantidadeInstrumentos > 0) {
        file.write((char*)listaInstrumentos, quantidadeInstrumentos * sizeof(Instrumento));
    }
    file.close();
}

// --- Inicialização e Finalização ---

void inicializarInstrumentos() {
    carregarDados();
}

void finalizarInstrumentos() {
    salvarDados();
}

// --- Funções de Busca Auxiliares ---

int buscarIndicePorId(int id) {
    for (int i = 0; i < quantidadeInstrumentos; i++) {
        if (listaInstrumentos[i].id == id && listaInstrumentos[i].ativo == 1) {
            return i;
        }
    }
    return -1; // Não encontrado
}

// --- CRUD ---

void cadastrarInstrumento() {
    if (quantidadeInstrumentos >= MAX_INSTRUMENTOS) {
        cout << "Limite de instrumentos atingido!" << endl;
        return;
    }

    Instrumento novo;
    
    // Geração de ID simples (sequencial baseado no último)
    novo.id = (quantidadeInstrumentos == 0) ? 1 : listaInstrumentos[quantidadeInstrumentos - 1].id + 1;
    novo.ativo = 1;
    novo.autorizado = 1; // Autorizado por padrão
    
    cout << "\n--- Cadastro de Instrumento ---" << endl;
    cout << "Nome: ";
    cin.ignore();
    cin.getline(novo.nome, TAM_NOME);
    
    cout << "Turma (A, B, C...): ";
    cin >> novo.turma;
    
    cout << "Quantidade em Estoque: ";
    cin >> novo.estoque;
    
    novo.disponivel = (novo.estoque > 0);
    novo.idAluno = 0; // Nenhum aluno vinculado inicialmente

    listaInstrumentos[quantidadeInstrumentos] = novo;
    quantidadeInstrumentos++;
    
    salvarDados(); // Salva imediatamente
    cout << "Instrumento cadastrado com sucesso! ID: " << novo.id << endl;
}

void listarInstrumentos() {
    cout << "\n--- Lista de Instrumentos ---" << endl;
    if (quantidadeInstrumentos == 0) {
        cout << "Nenhum instrumento cadastrado." << endl;
        return;
    }

    for (int i = 0; i < quantidadeInstrumentos; i++) {
        Instrumento& it = listaInstrumentos[i];
        if (it.ativo == 1) {
            cout << "ID: " << it.id 
                 << " | Nome: " << it.nome 
                 << " | Turma: " << it.turma
                 << " | Estoque: " << it.estoque << endl;
        }
    }
}

void excluirInstrumento() {
    int id;
    cout << "Digite o ID do instrumento a excluir: ";
    cin >> id;

    int idx = buscarIndicePorId(id);
    if (idx != -1) {
        listaInstrumentos[idx].ativo = 0; // Exclusão lógica
        salvarDados();
        cout << "Instrumento excluido com sucesso." << endl;
    } else {
        cout << "Instrumento nao encontrado." << endl;
    }
}

// --- Lógica de Empréstimo (Integrada com Login_Mat) ---

void realizarEmprestimo() {
    int idInstrumento, idAluno;
    cout << "\n--- Emprestimo ---" << endl;
    cout << "ID do Instrumento: "; cin >> idInstrumento;
    cout << "ID do Aluno: "; cin >> idAluno;

    // 1. Validar Instrumento
    int idx = buscarIndicePorId(idInstrumento);
    if (idx == -1) {
        cout << "Erro: Instrumento nao encontrado." << endl;
        return;
    }
    
    Instrumento& inst = listaInstrumentos[idx];

    if (inst.estoque <= 0) {
        cout << "Erro: Estoque esgotado." << endl;
        return;
    }

    // 2. Validar Aluno (Usando o módulo Login_mat)
    Aluno aluno = Login_mat::lerAluno(idAluno);
    
    // Verifica se o aluno existe (assumindo ID 0 é inválido)
    if (aluno.base.id == 0) {
        cout << "Erro: Aluno nao encontrado no sistema." << endl;
        return;
    }

    // 3. Regra: Aluno já tem instrumento?
    if (aluno.idInstrumento != 0) {
        cout << "Erro: Este aluno ja possui um instrumento emprestado." << endl;
        return;
    }

    // 4. Regra: Turma compatível?
    if (aluno.turma != inst.turma) {
        cout << "Erro: Instrumento destinado a turma " << inst.turma 
             << ", mas o aluno pertence a turma " << aluno.turma << "." << endl;
        return;
    }

    // 5. Executar Empréstimo
    inst.estoque--;
    if (inst.estoque == 0) inst.disponivel = false;
    inst.idAluno = idAluno; // Registra quem pegou

    // Atualiza o Aluno no outro módulo
    aluno.idInstrumento = inst.id;
    Login_mat::atualizar(idAluno, aluno);

    salvarDados();
    cout << "Emprestimo realizado com sucesso!" << endl;
}

void realizarDevolucao() {
    int idAluno;
    cout << "\n--- Devolucao ---" << endl;
    cout << "ID do Aluno: "; cin >> idAluno;

    // 1. Verificar Aluno
    Aluno aluno = Login_mat::lerAluno(idAluno);
    if (aluno.base.id == 0) {
        cout << "Erro: Aluno nao encontrado." << endl;
        return;
    }

    if (aluno.idInstrumento == 0) {
        cout << "Erro: Este aluno nao possui instrumento para devolver." << endl;
        return;
    }

    // 2. Encontrar o Instrumento que o aluno tem
    int idx = buscarIndicePorId(aluno.idInstrumento);
    if (idx == -1) {
        cout << "Erro critico: ID do instrumento no cadastro do aluno nao existe mais." << endl;
        return;
    }

    // 3. Executar Devolução
    Instrumento& inst = listaInstrumentos[idx];
    inst.estoque++;
    inst.disponivel = true;
    inst.idAluno = 0; // Limpa

    // Atualiza o Aluno
    aluno.idInstrumento = 0;
    Login_mat::atualizar(idAluno, aluno);

    salvarDados();
    cout << "Devolucao realizada com sucesso!" << endl;
}

// --- Menu ---

void menuInstrumentos() {
    int opcao = -1;
    while (opcao != 0) {
        cout << "\n=== MODULO INSTRUMENTOS ===" << endl;
        cout << "1. Cadastrar Instrumento" << endl;
        cout << "2. Listar Instrumentos" << endl;
        cout << "3. Realizar Emprestimo" << endl;
        cout << "4. Realizar Devolucao" << endl;
        cout << "5. Excluir Instrumento" << endl;
        cout << "0. Voltar" << endl;
        cout << "Opcao: ";
        cin >> opcao;

        switch(opcao) {
            case 1: cadastrarInstrumento(); break;
            case 2: listarInstrumentos(); break;
            case 3: realizarEmprestimo(); break;
            case 4: realizarDevolucao(); break;
            case 5: excluirInstrumento(); break;
            case 0: break;
            default: cout << "Opcao invalida." << endl;
        }
    }
}
