#ifndef INSTRUMENTOS_H
#define INSTRUMENTOS_H

#include <iostream>
#include <fstream>
#include <cstring>
#include "login_matricula.h" // Dependência do módulo de alunos

using namespace std;

// Constantes de configuração
#define ARQUIVO_INSTRUMENTOS "instrumentos.dat"
#define CAPACIDADE_INICIAL 10
#define FATOR_CRESCIMENTO 2
#define TAM_NOME 30

// Constantes de Status
#define INATIVO 0
#define ATIVO 1

// --- Definição das Structs ---

// Nota: A struct Aluno vem de login_matricula.h, mas assumimos que ela tenha:
// int idInstrumento; char turma;

struct Instrumento {
    int id;
    int ativo;
    int autorizado;
    char nome[TAM_NOME];
    char turma;           // NOVO: Define qual turma pode pegar este instrumento (ex: 'A', 'B')
    bool disponivel;      // Definido pelo estoque
    int estoque;
    int idAluno;          // ID do último aluno que pegou (para rastreabilidade do item físico)
};

struct GerenciadorInstrumentos {
    Instrumento** lista;
    int quantidade;
    int capacidade;
};

// --- Funções de Memória ---
void inicializarGerenciador(GerenciadorInstrumentos* gerenciador);
void finalizarGerenciador(GerenciadorInstrumentos* gerenciador);

// --- Persistência ---
void carregarDoArquivo(GerenciadorInstrumentos* gerenciador);
void salvarNoArquivo(GerenciadorInstrumentos* gerenciador);

// --- CRUD ---
int cadastrarInstrumento(GerenciadorInstrumentos* gerenciador, const char* nome, char turma, int estoque);
Instrumento* buscarInstrumentoPorId(GerenciadorInstrumentos* gerenciador, int id);
int editarInstrumento(GerenciadorInstrumentos* gerenciador, int id, const char* novoNome);
int excluirInstrumento(GerenciadorInstrumentos* gerenciador, int id);

// --- Lógica de Empréstimo (Integração com Login_Mat) ---
int registrarEmprestimo(GerenciadorInstrumentos* gerenciador, int idInstrumento, int idAluno);
int registrarDevolucao(GerenciadorInstrumentos* gerenciador, int idAluno);

// --- Relatórios ---
void listarInstrumentos(GerenciadorInstrumentos* gerenciador);
void listarDisponiveis(GerenciadorInstrumentos* gerenciador);
void imprimirInstrumento(Instrumento* inst);

// --- Menu ---
void menuInstrumentos(GerenciadorInstrumentos* gerenciador);

#endif
