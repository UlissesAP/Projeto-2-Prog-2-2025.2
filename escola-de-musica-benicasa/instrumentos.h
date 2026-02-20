#ifndef INSTRUMENTOS_H
#define INSTRUMENTOS_H

#include <iostream>
// Inclui o header do módulo de login para acessar as structs e funções do aluno
#include "login_matricula.h" 

// Constantes
#define TAM_NOME 30

// Struct do Instrumento (Definição única)
struct Instrumento {
    int id;
    int ativo;
    int autorizado;
    char nome[TAM_NOME];
    char turma;        // Turma a que o instrumento pertence (A, B, C...)
    bool disponivel;   // Calculado com base no estoque
    int estoque;       // Quantidade física disponível
    int idAluno;       // ID do aluno que pegou o instrumento emprestado
};

// --- Funções de Sistema ---
void inicializarInstrumentos(); // Carrega dados do arquivo
void finalizarInstrumentos();   // Salva dados no arquivo

// --- Funções de CRUD ---
void cadastrarInstrumento();
void listarInstrumentos();
void excluirInstrumento();

// --- Funções de Empréstimo ---
void realizarEmprestimo();
void realizarDevolucao();

#endif
