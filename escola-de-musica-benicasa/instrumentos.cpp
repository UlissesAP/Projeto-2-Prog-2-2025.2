#include "instrumentos.h"
#include <fstream>
#include <cstring>

using namespace std;

// --- Variáveis Globais do Módulo ---
const int MAX_INSTRUMENTOS = 100; // Limite do array
Instrumento listaInstrumentos[MAX_INSTRUMENTOS];
int quantidadeInstrumentos = 0;

const char* ARQUIVO_DADOS = "instrumentos.dat";

// --- Funções Auxiliares de Arquivo (Internas) ---

// Função para carregar dados do arquivo binário para a memória
void carregarDadosInstrumentos() {
    ifstream file(ARQUIVO_DADOS, ios::binary);
    if (!file.is_open()) return; // Se não existir, não faz nada

    // Lê a quantidade de registros
    file.read((char*)&quantidadeInstrumentos, sizeof(int));

    // Lê o array inteiro
    if (quantidadeInstrumentos > 0) {
        file.read((char*)listaInstrumentos, quantidadeInstrumentos * sizeof(Instrumento));
    }
    file.close();
}

// Função para salvar dados da memória para o arquivo binário
void salvarDadosInstrumentos() {
    ofstream file(ARQUIVO_DADOS, ios::binary | ios::trunc);
    if (!file.is_open()) {
        cout << "Erro ao salvar arquivo de instrumentos!" << endl;
        return;
    }

    // Salva a quantidade primeiro
    file.write((char*)&quantidadeInstrumentos, sizeof(int));
    
    // Salva o array
    if (quantidadeInstrumentos > 0) {
        file.write((char*)listaInstrumentos, quantidadeInstrumentos * sizeof(Instrumento));
    }
    file.close();
}

// --- Inicialização e Finalização ---

void inicializarInstrumentos() {
    carregarDadosInstrumentos();
}

void finalizarInstrumentos() {
    salvarDadosInstrumentos();
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
    
    // Geração de ID sequencial simples
    novo.id = (quantidadeInstrumentos == 0) ? 1 : listaInstrumentos[quantidadeInstrumentos - 1].id + 1;
    novo.ativo = 1;
    novo.autorizado = 1; 
    
    cout << "\n--- Cadastro de Instrumento ---" << endl;
    cout << "Nome: ";
    cin.ignore(); // Limpa buffer residual
    cin.getline(novo.nome, TAM_NOME);
    
    cout << "Turma (A, B, C...): ";
    cin >> novo.turma;
    
    cout << "Quantidade em Estoque: ";
    cin >> novo.estoque;
    
    novo.disponivel = (novo.estoque > 0);
    novo.idAluno = 0; 

    listaInstrumentos[quantidadeInstrumentos] = novo;
    quantidadeInstrumentos++;
    
    salvarDadosInstrumentos(); // Salva imediatamente após cadastro
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
        salvarDadosInstrumentos();
        cout << "Instrumento excluido com sucesso." << endl;
    } else {
        cout << "Instrumento nao encontrado." << endl;
    }
}

// --- Lógica de Empréstimo ---

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

    // 2. Validar Aluno (Usando o módulo Login_mat do projeto)
    Aluno aluno = Login_mat::lerAluno(idAluno);
    
    // Verifica se o aluno existe (ID > 0 geralmente indica sucesso na leitura)
    if (aluno.id == 0) { // Ajuste 'id' conforme a struct real do seu projeto
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
    inst.idAluno = idAluno; 

    // Atualiza o Aluno no módulo de Login
    aluno.idInstrumento = inst.id;
    Login_mat::atualizar(idAluno, aluno);

    salvarDadosInstrumentos();
    cout << "Emprestimo realizado com sucesso!" << endl;
}

void realizarDevolucao() {
    int idAluno;
    cout << "\n--- Devolucao ---" << endl;
    cout << "ID do Aluno: "; cin >> idAluno;

    // 1. Verificar Aluno
    Aluno aluno = Login_mat::lerAluno(idAluno);
    if (aluno.id == 0) { // Ajuste 'id' conforme necessário
        cout << "Erro: Aluno nao encontrado." << endl;
        return;
    }

    if (aluno.idInstrumento == 0) {
        cout << "Erro: Este aluno nao possui instrumento para devolver." << endl;
        return;
    }

    // 2. Encontrar o Instrumento vinculado ao aluno
    int idx = buscarIndicePorId(aluno.idInstrumento);
    if (idx == -1) {
        cout << "Erro critico: ID do instrumento no cadastro do aluno nao existe mais." << endl;
        return;
    }

    // 3. Executar Devolução
    Instrumento& inst = listaInstrumentos[idx];
    inst.estoque++;
    inst.disponivel = true;
    inst.idAluno = 0; 

    // Atualiza o Aluno
    aluno.idInstrumento = 0;
    Login_mat::atualizar(idAluno, aluno);

    salvarDadosInstrumentos();
    cout << "Devolucao realizada com sucesso!" << endl;
}
