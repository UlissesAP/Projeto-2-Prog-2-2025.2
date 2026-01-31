# 🎓 Sistema de Gerenciamento Escolar - Projeto 2 (Prog 2 - 2025.2)

## ⚠️ LEIA O ARQUIVO headers.h ⚠️

Este arquivo contém todas as estruturas de dados fundamentais do projeto. **Comece por lá antes de implementar qualquer módulo!**

---

## 📋 Sobre o Projeto

Este é um **Sistema de Gerenciamento Escolar Modular** desenvolvido em C++. Funciona como um portal unificado tipo SIGAA, onde diferentes usuários (alunos, professores, administradores, funcionários) fazem login e acessam funcionalidades conforme seus cargos.

O sistema foi arquitetado em **módulos independentes** que se comunicam entre si, permitindo escalabilidade e manutenção facilitada.

---

## 🔄 Fluxo Principal de Funcionamento

```
┌────────────────────────────────────────────────────────────────────────────────┐
│                              main() - Sistema Principal                         │
└────────────────────────────────────┬─────────────────────────────────────────┘
                                     │
                                     ▼
                    ┌────────────────────────────────────┐
                    │  mod_login_e_matricula/            │
                    │  servico_login.cpp                 │
                    │  ✓ Autentica usuário               │
                    │  ✓ Retorna person_role             │
                    │  ✓ Carrega dados do usuário        │
                    └────────────────┬───────────────────┘
                                     │
                    ▼────────────────┴────────────────┬────────────────┬──────────────┬──────────────┐
                    │                                  │                │              │              │
                    ▼                                  ▼                ▼              ▼              ▼
            ┌─────────────┐                   ┌──────────────┐   ┌──────────────┐ ┌────────────┐ ┌──────────┐
            │   ADMIN     │                   │    ALUNO     │   │  PROFESSOR   │ │  VENDEDOR  │ │ LOCADOR  │
            │ (Administrador)                 │  (Discente)  │   │   (Docente)  │ │(Lanchonete)│ │(Instru.) │
            └──────┬──────┘                   └──────┬───────┘   └──────┬───────┘ └────┬───────┘ └────┬─────┘
                   │                                 │                  │             │             │
                   ▼                                 ▼                  ▼             ▼             ▼
        ┌──────────────────────┐      ┌──────────────────────┐  ┌───────────┐  ┌──────────────┐ ┌────────────┐
        │ mod_adminitrativo/   │      │ mod_area_do_aluno/   │  │ mod_area_ │  │ mod_         │ │ mod_       │
        │ entrada_admin.cpp    │      │ entrada_aluno.cpp    │  │ do_profe- │  │ lanchonete/  │ │ instrumentos/
        │                      │      │                      │  │ ssor/     │  │ entrada_     │ │ entrada_   │
        │ ✓ Gerenciar usuários │      │ ✓ Ver histórico      │  │ entrada_  │  │ vendedor.cpp │ │ locador.cpp│
        │ ✓ Turmas/horários    │      │ ✓ Ver notas          │  │ professor │  │              │ │            │
        │ ✓ Movimentar alunos  │      │ ✓ Ver horários       │  │ .cpp      │  │ ✓ Cadastro   │ │ ✓ Cadastro │
        │ ✓ Operações especiais│      │                      │  │           │  │   produtos   │ │   instru-  │
        └──────────────────────┘      │ ⚠️ ACESSA             │  │ ⚠️ Criar   │  │ ✓ Preços e   │ │   mentos   │
                                      │ SERVIÇOS:            │  │   eventos  │  │   qtd        │ │            │
                │                     │ • servico_venda      │  │            │  │              │ │ ⚠️ ACESSA  │
                │                     │ • servico_emprestimo │  │ ⚠️ ACESSA  │  │ ⚠️ FORNECE   │ │ SERVIÇO:   │
                │                     │ • servico_inscricao_ │  │ SERVIÇOS:  │  │ SERVIÇO:     │ │ servico_   │
                │                     │   evento             │  │ • entrada_ │  │ servico_venda│ │ emprestimo │
                │                     │                      │  │   eventos  │  │              │ │            │
                │                     │                      │  │ • servico_ │  │              │ │            │
                │                     │                      │  │   venda    │  │              │ │            │
                │                     │                      │  │ • servico_ │  │              │ │            │
                │                     │                      │  │   emprést. │  │              │ │            │
                │                     │                      │  │ • servico_ │  │              │ │            │
                │                     │                      │  │   inscrição│  │              │ │            │
                │                     └──────────────────────┘  │            │  │              │ │            │
                │                                                └────────────┘  │              │ │            │
                │                                                                └──────────────┘ └────────────┘
                │                                                                                          │
                └──────────────────────────────────────────────────────────────────────────────────────────┘
                                              (Todos acessam serviços compartilhados)

════════════════════════════════════════ SERVIÇOS COMPARTILHADOS ════════════════════════════════════════
      • mod_lanchonete/servico_venda.cpp - Acessado por ALUNO, PROFESSOR, VENDEDOR, LOCADOR
      • mod_instrumentos/servico_emprestimo.cpp - Acessado por ALUNO, PROFESSOR, VENDEDOR, LOCADOR  
      • mod_eventos/servico_inscricao_evento.cpp - Acessado por ALUNO, PROFESSOR, VENDEDOR, LOCADOR
════════════════════════════════════════════════════════════════════════════════════════════════════════
```

### **Sequência Detalhada:**

1. **`main.cpp`** inicia a aplicação
2. **`mod_login_e_matricula/servico_login.cpp`** autentica o usuário
3. Retorna o `person_role` (cargo/identificador)
4. **`main.cpp`** chama a função adequada baseado no `person_role`:
   - **ADMIN** → `mod_adminitrativo/entrada_admin.cpp`
   - **ALUNO** → `mod_area_do_aluno/entrada_aluno.cpp`
   - **PROFESSOR** → `mod_area_do_professor/entrada_professor.cpp`
   - **VENDEDOR** → `mod_lanchonete/entrada_vendedor.cpp`
   - **LOCADOR** → `mod_instrumentos/entrada_locador.cpp`

5. **Todos os usuários** acessam **serviços compartilhados:**
   - `mod_lanchonete/servico_venda.cpp` - Compra de produtos
   - `mod_instrumentos/servico_emprestimo.cpp` - Empréstimo de instrumentos
   - `mod_eventos/servico_inscricao_evento.cpp` - Inscrição em eventos

---

## 👥 Tipos de Usuários (Cargos)

O sistema suporta os seguintes cargos com diferentes permissões:

1. **ALUNO** - Discente
   - Visualiza: histórico, notas, horários, turmas
   - Acessa: compra de produtos, empréstimo de instrumentos, inscrição em eventos

2. **PROFESSOR** - Docente
   - Visualiza: turmas, alunos, horários
   - Funcionalidades: lançar notas, registrar frequência
   - **Pode criar eventos** via `mod_eventos/entrada_eventos.cpp`
   - Acessa: compra de produtos, empréstimo de instrumentos

3. **ADMIN** - Administrador
   - Funcionalidades: gerenciar usuários, turmas, horários
   - Pode movimentar alunos entre turmas
   - Acesso a operações especiais

4. **VENDEDOR** - Gerencia Lanchonete
   - Funcionalidades: cadastro de produtos, preços, quantidades
   - Fornece: `servico_venda` para todos

5. **LOCADOR** - Gerencia Instrumentos
   - Funcionalidades: cadastro de instrumentos, quantidades
   - Fornece: `servico_emprestimo` para todos

⚠️ **IMPORTANTE:** 
- **EVENTOS NÃO É UM CARGO**, é um sub-módulo acessado por PROFESSOR (criar) e por todos via `servico_inscricao_evento` (se inscrever)
- Todos os cargos podem acessar os **serviços compartilhados** (venda, empréstimo, inscrição em eventos)

---

## 🏗️ Arquitetura dos Módulos

### **1. `mod_login_e_matricula/`**
**Responsabilidade:** Autenticação e Cadastro de Usuários

**Arquivos:**
- `servico_login.cpp / .h` - Autentica usuários e retorna dados de login
- `entrada_cadastro.cpp / .h` - Registra novos usuários no sistema

**Funcionalidades:**
- ✓ Validação de credenciais (CPF/email + senha)
- ✓ Retorno do esquema de informações do usuário autenticado
- ✓ Criação de novos usuários (alunos, professores, funcionários)
- ✓ Atualização de dados pessoais

**Interfaces com:** Todos os outros módulos (via dados de usuário autenticado)

---

### **2. `mod_adminitrativo/`**
**Responsabilidade:** Operações Administrativas e Gerência de Super Usuários

**Arquivos:**
- `entrada_admin.cpp / .h` - Interface do administrativo

**Funcionalidades:**
- ✓ Aponta para `mod_login_e_matricula/entrada_cadastro.cpp` para criar/editar usuários
- ✓ Gerenciamento de alunos (movimentação entre turmas, desativação, etc.)
- ✓ Cadastro e gerenciamento de turmas
- ✓ Definição de horários de aulas
- ✓ Operações especiais para super usuários (ADMIN)
- ✓ Coordenação de outros módulos (administrativamente)

**Interfaces com:** 
- `mod_login_e_matricula` (cadastro de pessoas)
- `mod_area_do_professor` (gerenciar professores)
- `mod_area_do_aluno` (gerenciar alunos)
- `mod_eventos` (operações gerais)
- `mod_instrumentos` (gerência)
- `mod_lanchonete` (gerência)

---

### **3. `mod_area_do_aluno/`**
**Responsabilidade:** Portal e Funcionalidades do Aluno

**Arquivos:**
- `entrada_aluno.cpp / .h` - Interface do aluno

**Funcionalidades:**
- ✓ Visualizar histórico acadêmico
- ✓ Consultar notas obtidas
- ✓ Visualizar horários de aulas
- ✓ Consultar turmas inscritas
- ✓ Sistema de compras (aponta para `mod_lanchonete`)
- ✓ Sistema de empréstimos de instrumentos (aponta para `mod_instrumentos/entrada_locador.cpp`)
- ✓ Visualizar eventos inscritos

**Interfaces com:**
- `mod_lanchonete` (compra de produtos)
- `mod_instrumentos` (empréstimo de instrumentos)
- `mod_eventos` (visualizar eventos)

---

### **4. `mod_area_do_professor/`**
**Responsabilidade:** Portal e Funcionalidades do Professor

**Arquivos:**
- `entrada_professor.cpp / .h` - Interface do professor

**Funcionalidades:**
- ✓ Cadastrar e atualizar notas dos alunos
- ✓ Registrar frequência das aulas
- ✓ Gerenciar turmas (visualizar alunos, horários)
- ✓ Criar e gerenciar eventos (provas, atividades, tarefas)
- ✓ Sistema de compras (aponta para `mod_lanchonete`)
- ✓ Sistema de empréstimos de instrumentos (aponta para `mod_instrumentos/entrada_locador.cpp`)

**Interfaces com:**
- `mod_eventos` (criar/gerenciar eventos)
- `mod_lanchonete` (compra de produtos)
- `mod_instrumentos` (empréstimo de instrumentos)

---

### **5. `mod_eventos/`** ⚠️ SUB-MÓDULO DO PROFESSOR
**Responsabilidade:** Gerenciamento de Eventos e Inscrições Compartilhadas

**Arquivos:**
- `entrada_eventos.cpp / .h` - Interface de eventos (chamada por `mod_area_do_professor`)
- `servico_inscricao_evento.cpp / .h` - **Serviço compartilhado** (acessado por alunos, professores e funcionários)

**Funcionalidades do Módulo de Eventos:**
- ✓ Criar eventos (provas, atividades, tarefas, apresentações)
- ✓ Definir datas e horários
- ✓ Gerenciar vagas e limite de participantes
- ✓ Cancelar/modificar eventos

**Funcionalidades do Serviço de Inscrição (compartilhado):**
- ✓ Sistema de inscrição em eventos gerais da escola
- ✓ Acessível por: ALUNO, PROFESSOR, VENDEDOR, LOCADOR
- ✓ Validação de vagas disponíveis
- ✓ Registro de inscrições

**Interfaces com:**
- `mod_area_do_professor` (criação via `entrada_eventos`)
- `mod_area_do_aluno` (inscrição via `servico_inscricao_evento`)
- Funcionários (VENDEDOR, LOCADOR) (inscrição via `servico_inscricao_evento`)

---

### **6. `mod_instrumentos/`**
**Responsabilidade:** Gerência de Instrumentos e Serviço de Empréstimos Compartilhado

**Arquivos:**
- `entrada_locador.cpp / .h` - Interface do locador
- `servico_emprestimo.cpp / .h` - **Serviço compartilhado** (acessado por alunos, professores e funcionários)

**Funcionalidades do Locador:**
- ✓ Cadastro e gerenciamento de instrumentos
- ✓ Definição de quantidade disponível
- ✓ Atualização de informações de instrumentos
- ✓ Controle de danos/perdas

**Funcionalidades do Serviço de Empréstimo (compartilhado):**
- ✓ Sistema de empréstimos para ALUNO, PROFESSOR, VENDEDOR, LOCADOR
- ✓ Validação de disponibilidade
- ✓ Registro de empréstimos
- ✓ Processamento de devoluções
- ✓ Controle de limite (máximo 5 itens por pessoa)

**Interfaces com:**
- `mod_area_do_aluno` (via `servico_emprestimo`)
- `mod_area_do_professor` (via `servico_emprestimo`)
- Funcionários (VENDEDOR, LOCADOR) (via `servico_emprestimo`)
- `mod_adminitrativo` (gerência geral)

---

### **7. `mod_lanchonete/`**
**Responsabilidade:** Gerência de Cantina e Serviço de Vendas Compartilhado

**Arquivos:**
- `entrada_vendedor.cpp / .h` - Interface do vendedor
- `servico_venda.cpp / .h` - **Serviço compartilhado** (acessado por alunos, professores e funcionários)

**Funcionalidades do Vendedor:**
- ✓ Cadastro de produtos (alimentos, bebidas, etc.)
- ✓ Definição de preços
- ✓ Controle de quantidade em estoque
- ✓ Atualização de produtos

**Funcionalidades do Serviço de Venda (compartilhado):**
- ✓ Sistema de vendas para todos (ALUNO, PROFESSOR, VENDEDOR, LOCADOR)
- ✓ Processamento de compras
- ✓ Atualização de saldo do usuário
- ❌ Sem relatórios
- ❌ Sem registro de transações detalhadas

**Interfaces com:**
- `mod_area_do_aluno` (via `servico_venda`)
- `mod_area_do_professor` (via `servico_venda`)
- Funcionários (VENDEDOR, LOCADOR) (via `servico_venda`)
- `mod_adminitrativo` (gerência de produtos)

---

##  Configuração de Login (servico_login.cpp)

⚠️ **IMPORTANTE:** O arquivo `mod_login_e_matricula/servico_login.cpp` atualmente retorna um login **engessado (hardcoded)** sempre com ADMIN.

### **Como Modificar para Testar Diferentes Tipos de Usuário:**

Edite a função `validar_login()` em `mod_login_e_matricula/servico_login.cpp`:

#### **Código Atual (Engessado - Sempre ADMIN):**

```cpp
login_info validar_login(const int* id_usuario, const char* senha) {
    login_info info;

    // ===== Login engessado =====
    info.status = VALIDO;
    info.chave_acesso = ADMIN;  // 👈 Sempre retorna ADMIN

    // ===== Preencher info_basica com dados fixos =====
    info.info_basica.id = 1;
    info.info_basica.ativo = 1;

    std::strcpy(info.info_basica.nome, "Administrador Padrao");
    std::strcpy(info.info_basica.cpf, "00000000000");

    info.info_basica.categoria = ADMIN;
    std::strcpy(info.info_basica.especialidade, "Administrador");

    return info;
}
```

#### **Para Mudar o Tipo de Usuário:**

Altere apenas 2 campos: `info.chave_acesso` e `info.info_basica.categoria`

```cpp
login_info validar_login(const int* id_usuario, const char* senha) {
    login_info info;

    info.status = VALIDO;
    
    // 👇 MUDE AQUI para testar diferentes tipos:
    info.chave_acesso = ALUNO;  // ← Altere este valor
    
    info.info_basica.id = 1;
    info.info_basica.ativo = 1;

    std::strcpy(info.info_basica.nome, "Aluno Teste");
    std::strcpy(info.info_basica.cpf, "00000000001");

    info.info_basica.categoria = ALUNO;  // ← E este também
    std::strcpy(info.info_basica.especialidade, "");

    return info;
}
```

### **Valores Disponíveis para `info.chave_acesso`:**

```cpp
ADMIN      // Administrador
ALUNO      // Aluno
PROFESSOR  // Professor (pode acessar mod_eventos para criar eventos)
VENDEDOR   // Vendedor de lanchonete
LOCADOR    // Locador de instrumentos

// ❌ EVENTOS NÃO É CARGO - é sub-módulo do professor
// Todos acessam eventos via servico_inscricao_evento
```

### **Tabela de Configuração Rápida:**

| Tipo | `chave_acesso` | `categoria` | `nome` | `especialidade` |
|------|----------------|-------------|--------|-----------------|
| **Admin** | `ADMIN` | `ADMIN` | "Administrador Padrao" | "Administrador" |
| **Aluno** | `ALUNO` | `ALUNO` | "Aluno Teste" | "" (vazio) |
| **Professor** | `PROFESSOR` | `PROFESSOR` | "Professor Teste" | "Piano" |
| **Vendedor** | `VENDEDOR` | `VENDEDOR` | "Vendedor Teste" | "Vendedor" |
| **Locador** | `LOCADOR` | `LOCADOR` | "Locador Teste" | "Locador" |

### **Exemplo Completo para Testar como PROFESSOR:**

```cpp
login_info validar_login(const int* id_usuario, const char* senha) {
    login_info info;

    info.status = VALIDO;
    info.chave_acesso = PROFESSOR;  // 👈 Professor

    info.info_basica.id = 2;
    info.info_basica.ativo = 1;

    std::strcpy(info.info_basica.nome, "Prof. Ana Oliveira");
    std::strcpy(info.info_basica.cpf, "12345678901");

    info.info_basica.categoria = PROFESSOR;
    std::strcpy(info.info_basica.especialidade, "Violão");

    return info;
}
```

### **Passos Rápidos para Testar:**

1. Abra `mod_login_e_matricula/servico_login.cpp`
2. Localize a função `validar_login()`
3. Altere `info.chave_acesso = TIPO_DESEJADO`
4. Altere `info.info_basica.categoria = TIPO_DESEJADO`
5. Atualize `nome`, `cpf` e `especialidade` conforme apropriado
6. Salve (`Ctrl + S`)
7. Compile (`Ctrl + Shift + B` ou execute build task)
8. Execute o programa - ele abrirá a área do tipo de usuário escolhido!

---

## �📦 Estruturas de Dados Principais (em `headers.h`)

### **Identidade**
Representa um usuário no sistema:
- ID, nome, CPF, senha, email
- Categoria/cargo
- Especialidade (para professores: Piano, Canto, etc.)

### **Class (Turma)**
Representa uma turma/disciplina:
- Código, ano letivo, nome (ex: "Violão I")
- Professor responsável e lista de alunos (até 50)
- Horários (até 4 por semana)
- Eventos (provas, atividades, etc.)
- Aulas ministradas (até 100)

### **historic_individual (Histórico)**
Registro acadêmico de um aluno em uma turma:
- Frequência (presença/ausência em cada aula)
- Notas obtidas (até 10 notas)
- Status final (ABERTO, APROVADO, REPROVADO)

### **RecursosIndividuais (Recursos)**
Gerencia recursos de um usuário:
- Saldo (para cantina)
- Itens emprestados (livros/instrumentos, máximo 5)
- Ativo/Inativo

### **ProdutoCantina**
Produtos vendidos na cantina:
- ID, nome, preço
- Quantidade em estoque

### **Biblioteca**
Livros/instrumentos da biblioteca:
- ID, título, quantidade total
- Controle de disponibilidade

### **EventoAgenda**
Eventos agendados na escola:
- Nome, data, vagas totais
- Lista de inscritos
- Status (agendado/cancelado)

### **RegistroNotas**
Registro de notas de um aluno:
- Aluno, turma, notas lançadas

## 👥 Tipos de Usuários (Cargos)

O sistema suporta os seguintes cargos com diferentes permissões e acessos:

| Cargo | Acesso | Módulos Associados |
|-------|--------|-------------------|
| **ALUNO** | Discente | `mod_area_do_aluno` |
| **PROFESSOR** | Docente | `mod_area_do_professor` |
| **ADMIN** | Administrador | `mod_adminitrativo` |
| **VENDEDOR** | Gerencia cantina | `mod_lanchonete` |
| **LOCADOR** | Gerencia empréstimos | `mod_instrumentos` |
| **EVENTOS** | Gerencia eventos | `mod_eventos` |

---

## 📦 Estruturas de Dados Principais (em `headers.h`)

### **Identidade**
Representa um usuário no sistema:
- ID, nome, CPF, senha, email
- Categoria/cargo (`person_role`)
- Especialidade (para professores: Piano, Canto, etc.)

### **Class (Turma)**
Representa uma turma/disciplina:
- Código, ano letivo, nome (ex: "Violão I")
- Professor responsável e lista de alunos (até 50)
- Horários (até 4 por semana)
- Eventos (provas, atividades, etc.)
- Aulas ministradas (até 100)

### **historic_individual (Histórico)**
Registro acadêmico de um aluno em uma turma:
- Frequência (presença/ausência em cada aula)
- Notas obtidas (até 10 notas)
- Status final (ABERTO, APROVADO, REPROVADO)

### **RecursosIndividuais (Recursos)**
Gerencia recursos de um usuário:
- Saldo (para cantina)
- Itens emprestados (instrumentos, máximo 5)
- Status (Ativo/Inativo)

### **ProdutoCantina**
Produtos vendidos na lanchonete:
- ID, nome, preço
- Quantidade em estoque

### **Instrumentos**
Instrumentos disponíveis para empréstimo:
- ID, nome, quantidade total
- Controle de disponibilidade
- Registro de empréstimos

### **EventoAgenda**
Eventos agendados na escola:
- Nome, data, vagas totais
- Lista de inscritos
- Status (agendado/cancelado)

---

## 🔐 Sistema de Autenticação

O login verifica o `person_role` (cargo) do usuário e libera acessos conforme:

```cpp
// Em main.cpp após autenticação:
if (person_role == ALUNO) {
    entrada_aluno();      // mod_area_do_aluno/entrada_aluno.cpp
} else if (person_role == PROFESSOR) {
    entrada_professor();  // mod_area_do_professor/entrada_professor.cpp
} else if (person_role == ADMIN) {
    entrada_admin();      // mod_adminitrativo/entrada_admin.cpp
} else if (person_role == VENDEDOR) {
    entrada_vendedor();   // mod_lanchonete/entrada_vendedor.cpp
} else if (person_role == LOCADOR) {
    entrada_locador();    // mod_instrumentos/entrada_locador.cpp
} else if (person_role == EVENTOS) {
    entrada_eventos();    // mod_eventos/entrada_eventos.cpp
}
```

---

## 📋 Checklist de Implementação por Módulo

### **mod_login_e_matricula/**
- [ ] `servico_login.cpp` - Função de autenticação
- [ ] `entrada_cadastro.cpp` - Interface de cadastro
- [ ] Validação de credenciais
- [ ] Persistência de usuários

### **mod_adminitrativo/**
- [ ] `entrada_admin.cpp` - Interface do admin
- [ ] Integração com cadastro de usuários
- [ ] Gerenciamento de turmas e horários
- [ ] Movimentação de alunos
- [ ] Operações de super usuário

### **mod_area_do_aluno/**
- [ ] `entrada_aluno.cpp` - Interface do aluno
- [ ] Visualização de histórico
- [ ] Chamar `servico_venda` para compras
- [ ] Chamar `servico_emprestimo` para empréstimos
- [ ] Chamar `servico_inscricao_evento` para inscrição

### **mod_area_do_professor/**
- [ ] `entrada_professor.cpp` - Interface do professor
- [ ] Lançamento de notas
- [ ] Registro de frequência
- [ ] Chamar `entrada_eventos` para criar eventos
- [ ] Chamar `servico_venda` para compras
- [ ] Chamar `servico_emprestimo` para empréstimos

### **mod_eventos/**
- [ ] `entrada_eventos.cpp` - Interface de eventos (sub-módulo de professor)
- [ ] Criação e gerenciamento de eventos
- [ ] `servico_inscricao_evento.cpp / .h` - Serviço compartilhado
- [ ] Sistema de inscrições (via serviço)
- [ ] Validação de vagas

### **mod_instrumentos/**
- [ ] `entrada_locador.cpp` - Interface do locador
- [ ] Cadastro e gerência de instrumentos
- [ ] `servico_emprestimo.cpp / .h` - Serviço compartilhado
- [ ] Sistema de empréstimos (via serviço)
- [ ] Processamento de devoluções

### **mod_lanchonete/**
- [ ] `entrada_vendedor.cpp` - Interface do vendedor
- [ ] Cadastro de produtos
- [ ] Definição de preços e quantidades
- [ ] `servico_venda.cpp / .h` - Serviço compartilhado
- [ ] Sistema de vendas (via serviço, acessível a todos)

---

## 📁 Estrutura de Diretórios Esperada

```
Projeto-2-Prog-2-2025.2/
├── main.cpp                              # Ponto de entrada
├── headers.h                             # Estruturas de dados
├── Readme.md                             # Este arquivo
├── database/                             # Camada de dados
│   └── (arquivos de persistência)
├── mod_login_e_matricula/
│   ├── servico_login.cpp / .h
│   └── entrada_cadastro.cpp / .h
├── mod_adminitrativo/
│   └── entrada_admin.cpp / .h
├── mod_area_do_aluno/
│   └── entrada_aluno.cpp / .h
├── mod_area_do_professor/
│   └── entrada_professor.cpp / .h
├── mod_eventos/
│   └── entrada_eventos.cpp / .h
├── mod_instrumentos/
│   └── entrada_locador.cpp / .h
└── mod_lanchonete/
    └── entrada_vendedor.cpp / .h
```

---

## 🔧 Compilação

Use o build task configurado no VS Code:

```bash
g++.exe -fdiagnostics-color=always -g ${file} -o ${fileDirname}\\${fileBasenameNoExtension}.exe
```

**Ou compile manualmente:**
```bash
g++ -g main.cpp mod_login_e_matricula/servico_login.cpp mod_login_e_matricula/entrada_cadastro.cpp ... -o main.exe
```

---

## 📝 Observações Importantes

- **Máximo 50 alunos por turma**
- **Máximo 100 aulas por turma**
- **Máximo 10 eventos por turma**
- **Máximo 5 itens emprestados por pessoa**
- **Máximo 4 horários por semana**
- **Cada função deve validar dados de entrada**
- **Módulos devem ser independentes e reutilizáveis**
- **Sempre consultar `headers.h` para estruturas de dados**

---

## 🤝 Comunicação Entre Módulos

As chamadas entre módulos devem seguir este padrão:

1. **Aluno compra na lanchonete:** 
   - `mod_area_do_aluno` → `mod_lanchonete/entrada_vendedor()`

2. **Aluno pega instrumento emprestado:**
   - `mod_area_do_aluno` → `mod_instrumentos/entrada_locador()`

3. **Admin cria novo usuário:**
   - `mod_adminitrativo` → `mod_login_e_matricula/entrada_cadastro()`

4. **Professor cria evento:**
   - `mod_area_do_professor` → `mod_eventos/entrada_eventos()`

---

## 📞 Suporte e Dúvidas

Antes de implementar:
1. ✓ Leia `headers.h`
2. ✓ Entenda o fluxo do seu módulo
3. ✓ Identifique as integrações necessárias
4. ✓ Siga a estrutura de pastas
5. ✓ Use nomes consistentes em arquivos `.cpp` e `.h`

---

**Versão:** 2025.2  
**Data:** Janeiro 2026  
**Linguagem:** C++  
**Status:** Em Desenvolvimento
