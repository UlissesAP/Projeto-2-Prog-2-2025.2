#include "instrumentos.h"

// --- Funções Auxiliares de Memória ---

void expandirCapacidadeInstrumentos(GerenciadorInstrumentos* gerenciador) {
    int novaCapacidade = gerenciador->capacidade * FATOR_CRESCIMENTO;
    Instrumento** novaLista = new Instrumento*[novaCapacidade];
    
    for (int i = 0; i < gerenciador->quantidade; i++) {
        novaLista[i] = gerenciador->lista[i];
    }

    delete[] gerenciador->lista;
    gerenciador->lista = novaLista;
    gerenciador->capacidade = novaCapacidade;
}

// --- Inicialização e Encerramento ---

void inicializarGerenciador(GerenciadorInstrumentos* gerenciador) {
    gerenciador->lista = new Instrumento*[CAPACIDADE_INICIAL];
    gerenciador->quantidade = 0;
    gerenciador->capacidade = CAPACIDADE_INICIAL;
    carregarDoArquivo(gerenciador);
}

void finalizarGerenciador(GerenciadorInstrumentos* gerenciador) {
    salvarNoArquivo(gerenciador);
    for (int i = 0; i < gerenciador->quantidade; i++) {
        delete gerenciador->lista[i];
    }
    delete[] gerenciador->lista;
    gerenciador->lista = NULL;
    gerenciador->quantidade = 0;
}

// --- Persistência ---

void carregarDoArquivo(GerenciadorInstrumentos* gerenciador) {
    ifstream file(ARQUIVO_INSTRUMENTOS, ios::binary | ios::ate);
    if (!file.is_open()) return;

    streampos tamanho = file.tellg();
    int qtd_registros = tamanho / sizeof(Instrumento);
    file.seekg(0, ios::beg);

    while (gerenciador->capacidade < qtd_registros) {
        expandirCapacidadeInstrumentos(gerenciador);
    }

    for (int i = 0; i < qtd_registros; i++) {
        gerenciador->lista[i] = new Instrumento;
        file.read((char*)gerenciador->lista[i], sizeof(Instrumento));
        gerenciador->quantidade++;
    }
    file.close();
}

void salvarNoArquivo(GerenciadorInstrumentos* gerenciador) {
    ofstream file(ARQUIVO_INSTRUMENTOS, ios::binary | ios::trunc);
    if (!file.is_open()) return;
    
    for (int i = 0; i < gerenciador->quantidade; i++) {
        file.write((char*)gerenciador->lista[i], sizeof(Instrumento));
    }
    file.close();
}

// --- CRUD ---

int cadastrarInstrumento(GerenciadorInstrumentos* gerenciador, const char* nome, char turma, int estoque) {
    if (gerenciador->quantidade >= gerenciador->capacidade) {
        expandirCapacidadeInstrumentos(gerenciador);
    }

    Instrumento* novo = new Instrumento;
    
    int maxId = 0;
    for (int i = 0; i < gerenciador->quantidade; i++) {
        if (gerenciador->lista[i]->id > maxId) maxId = gerenciador->lista[i]->id;
    }
    novo->id = maxId + 1;

    strncpy(novo->nome, nome, TAM_NOME);
    novo->nome[TAM_NOME - 1] = '\0';
    
    novo->turma = turma; // Define a turma permitida
    novo->ativo = ATIVO;
    novo->autorizado = 1;
    novo->estoque = estoque;
    novo->disponivel = (estoque > 0);
    novo->idAluno = 0;

    gerenciador->lista[gerenciador->quantidade] = novo;
    gerenciador->quantidade++;

    salvarNoArquivo(gerenciador);
    return novo->id;
}

Instrumento* buscarInstrumentoPorId(GerenciadorInstrumentos* gerenciador, int id) {
    for (int i = 0; i < gerenciador->quantidade; i++) {
        if (gerenciador->lista[i]->id == id && gerenciador->lista[i]->ativo == ATIVO) {
            return gerenciador->lista[i];
        }
    }
    return NULL;
}

int editarInstrumento(GerenciadorInstrumentos* gerenciador, int id, const char* novoNome) {
    Instrumento* inst = buscarInstrumentoPorId(gerenciador, id);
    if (!inst) return 0;
    
    strncpy(inst->nome, novoNome, TAM_NOME);
    inst->nome[TAM_NOME - 1] = '\0';
    salvarNoArquivo(gerenciador);
    return 1;
}

int excluirInstrumento(GerenciadorInstrumentos* gerenciador, int id) {
    Instrumento* inst = buscarInstrumentoPorId(gerenciador, id);
    if (!inst) return 0;
    
    inst->ativo = INATIVO;
    salvarNoArquivo(gerenciador);
    return 1;
}

// --- Lógica de Empréstimo (Integrada) ---

int registrarEmprestimo(GerenciadorInstrumentos* gerenciador, int idInstrumento, int idAluno) {
    // 1. Validação do Instrumento
    Instrumento* inst = buscarInstrumentoPorId(gerenciador, idInstrumento);
    if (!inst) return -4; // Instrumento não encontrado
    if (inst->autorizado == 0) return -5; // Não autorizado
    if (inst->estoque <= 0) return -3;    // Sem estoque

    // 2. Leitura e Validação do Aluno (Via Módulo Externo)
    Aluno aluno = Login_mat::lerAluno(idAluno);
    
    // Verifica se o aluno existe (assumindo que ID <= 0 é inválido ou nome vazio)
    if (aluno.base.id == 0) return 0; // Aluno não encontrado

    // REGRA 1: Aluno já possui instrumento?
    if (aluno.idInstrumento != 0) {
        return -1; // Aluno já tem instrumento
    }

    // REGRA 2: A turma do aluno corresponde à do instrumento?
    if (aluno.turma != inst->turma) {
        return -2; // Turma incompatível
    }

    // 3. Execução do Empréstimo
    inst->estoque--;
    if (inst->estoque == 0) inst->disponivel = false;
    
    // Vincula o ID do aluno ao instrumento (para rastreabilidade do último uso)
    inst->idAluno = idAluno;

    // 4. Atualização do Aluno (Via Módulo Externo)
    aluno.idInstrumento = inst->id;
    Login_mat::atualizar(idAluno, aluno);

    salvarNoArquivo(gerenciador);
    return 1; // Sucesso
}

int registrarDevolucao(GerenciadorInstrumentos* gerenciador, int idAluno) {
    // 1. Leitura do Aluno
    Aluno aluno = Login_mat::lerAluno(idAluno);
    if (aluno.base.id == 0) return 0; // Aluno não encontrado

    // Verifica se o aluno tem instrumento para devolver
    if (aluno.idInstrumento == 0) return -1; // Não tem nada para devolver

    // 2. Busca o instrumento vinculado ao aluno
    Instrumento* inst = buscarInstrumentoPorId(gerenciador, aluno.idInstrumento);
    if (!inst) return -2; // Erro crítico: ID no aluno não existe na lista de instrumentos

    // 3. Execução da Devolução
    inst->estoque++;
    inst->disponivel = true;
    // Opcional: Limpar o idAluno do instrumento ou manter histórico. Vamos manter histórico do último.

    // 4. Atualização do Aluno
    aluno.idInstrumento = 0; // Limpa o vínculo
    Login_mat::atualizar(idAluno, aluno);

    salvarNoArquivo(gerenciador);
    return 1; // Sucesso
}

// --- Relatórios ---

void imprimirInstrumento(Instrumento* inst) {
    if (!inst) return;
    
    cout << "ID: " << inst->id 
         << " | Nome: " << inst->nome 
         << " | Turma: " << inst->turma
         << " | Estoque: " << inst->estoque
         << " | Status: " << (inst->disponivel ? "Disponivel" : "Esgotado");
    
    if (inst->idAluno != 0) {
        cout << " | Ultimo ID Aluno: " << inst->idAluno;
    }
    cout << (inst->ativo ? "" : " (INATIVO)") << endl;
}

void listarInstrumentos(GerenciadorInstrumentos* gerenciador) {
    cout << "\n--- Todos os Instrumentos ---" << endl;
    for (int i = 0; i < gerenciador->quantidade; i++) {
        imprimirInstrumento(gerenciador->lista[i]);
    }
}

void listarDisponiveis(GerenciadorInstrumentos* gerenciador) {
    cout << "\n--- Instrumentos Disponiveis ---" << endl;
    for (int i = 0; i < gerenciador->quantidade; i++) {
        if (gerenciador->lista[i]->disponivel && gerenciador->lista[i]->ativo == ATIVO) {
            imprimirInstrumento(gerenciador->lista[i]);
        }
    }
}

// --- Menu ---

void menuInstrumentos(GerenciadorInstrumentos* gerenciador) {
    int opcao = -1;
    while (opcao != 0) {
        cout << "\n=== MODULO INSTRUMENTOS ===" << endl;
        cout << "1. Cadastrar Instrumento" << endl;
        cout << "2. Listar Todos" << endl;
        cout << "3. Listar Disponiveis" << endl;
        cout << "4. Realizar Emprestimo" << endl;
        cout << "5. Realizar Devolucao (Por ID do Aluno)" << endl;
        cout << "6. Excluir Instrumento" << endl;
        cout << "0. Voltar" << endl;
        cout << "Opcao: ";
        cin >> opcao;

        int id, idAluno, res;
        char nome[TAM_NOME];
        char turma;
        int estoque;

        switch(opcao) {
            case 1:
                cout << "Nome do Instrumento: ";
                cin.ignore(); cin.getline(nome, TAM_NOME);
                cout << "Turma Destinada (A, B, C...): ";
                cin >> turma;
                cout << "Estoque Inicial: ";
                cin >> estoque;
                cadastrarInstrumento(gerenciador, nome, turma, estoque);
                cout << "Cadastrado com sucesso." << endl;
                break;

            case 2:
                listarInstrumentos(gerenciador);
                break;

            case 3:
                listarDisponiveis(gerenciador);
                break;

            case 4:
                cout << "ID do Instrumento: "; cin >> id;
                cout << "ID do Aluno: "; cin >> idAluno;
                res = registrarEmprestimo(gerenciador, id, idAluno);
                if (res == 1) cout << "Emprestimo realizado!" << endl;
                else if (res == -1) cout << "Erro: Aluno ja possui instrumento." << endl;
                else if (res == -2) cout << "Erro: Turma do aluno incompativel." << endl;
                else if (res == -3) cout << "Erro: Sem estoque." << endl;
                else if (res == -4) cout << "Erro: Instrumento nao encontrado." << endl;
                else if (res == 0) cout << "Erro: Aluno nao encontrado." << endl;
                break;

            case 5:
                cout << "ID do Aluno: "; cin >> idAluno;
                res = registrarDevolucao(gerenciador, idAluno);
                if (res == 1) cout << "Devolucao realizada!" << endl;
                else if (res == -1) cout << "Erro: Aluno nao possui instrumento para devolver." << endl;
                else if (res == 0) cout << "Erro: Aluno nao encontrado." << endl;
                break;

            case 6:
                cout << "ID do Instrumento: "; cin >> id;
                if (excluirInstrumento(gerenciador, id)) cout << "Instrumento inativado." << endl;
                else cout << "Nao encontrado." << endl;
                break;
        }
    }
}
