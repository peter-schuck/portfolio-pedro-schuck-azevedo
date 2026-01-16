# 🧩 Técnicas de Construção de Programas <Nome da Disciplina>
**Período:** 2025/2  
**Projeto:** Studify

**Professor(a):** Karina Kohl

---

## 🎯 Problema a ser resolvido

Uma aplicação para estudantes universitários organizarem seu tempo livre para estudar para provas, trabalhos e outras atividades

---

## 🏗️ Arquitetura e Tecnologias
Decisões técnicas e a estrutura do sistema:
- Linguagens e frameworks utilizados: Java e Swing
- Padrões de projeto aplicados: MVC
- Estrutura de pastas e responsabilidades:

tcp-20252-final-grupo-04/

├── src/                     # Código fonte principal

│   ├── Studify.java        # Classe principal da aplicação

│   ├── model/              # Camada de dados e entidades

│   │   ├── entities/       # Entidades principais (Aluno, Disciplina, etc.)

│   │   ├── atividades/     # Tipos de atividades (Prova, Exercício, Trabalho)

│   │   ├── config/         # Configurações (DiaSemana, Impedimento, etc.)

│   │   └── allocation/     # Sistema de alocação de atividades

│   ├── controller/         # Camada de controle e lógica de negócio

│   │   ├── agenda/         # Geração de agendas

│   │   ├── atividades/     # Gerenciamento de atividades

│   │   └── comunicacao/    # Comunicação entre telas

│   ├── view/               # Interface gráfica (Swing)

│   │   ├── TelaInicial.java

│   │   ├── TelaRegistrarSemana.java

│   │   ├── TelaRegistrarTimeSlot.java

│   │   ├── TelaRegistrarAtividade.java

│   │   └── TelaAgenda.java

│   ├── utils/              # Utilitários (BinarySearchUtils)

│   └── test/               # Testes unitários (JUnit)

├── dist/                   # JAR executável

│   └── studify.jar        # Aplicação empacotada

├── build/                  # Arquivos compilados (.class)

├── lib/                    # Bibliotecas externas

├── docs/                   # Documentação do projeto

├── resources/              # Recursos e configurações

└── scripts de build        # compile.bat, build-jar.bat, run-tests.bat


- Estratégias de testes e verificação: Testes unitários usando Junit e TDD

- Diagrama UML: https://lucid.app/lucidchart/fd8c572a-eaba-4c25-a790-32c134a90f13/edit?view_items=x6O18qJcPzNJ&page=IYhW1RiEHMkl&invitationId=inv_ce44c2cb-6f27-4a37-bded-bc4249e2a028

---

## 🧱 Boas práticas aplicadas
Práticas de Engenharia de Software seguidas neste projeto:
- Commits descritivos
- Princípios **SOLID**
- Uso de ferramentas de logging

---

## 🤝 Soft Skills e Trabalho em Equipe
Reflexão pessoal sobre como contribuiu no projeto:
> - Auxiliei o grupo com ideias para o projeto, incluindo o conceito inicial da aplicação.  
> - Trabalhei de forma autônoma na implementação alguns módulos e boa parte da interface.  
> - Pesquisei referências de código em artigos e repositórios open-source.  
> - Organizei o andamento das entregas de cada membro do grupo e apresentei progresso.
> - Redigi uma quantia considerável do relatório sobre o programa e dos slides apresentando-o.

---

## 🧪 Exemplos de Código

```java
    public boolean validaDisciplina(String disciplinaNome, String disciplinaPrioridade) {
        boolean validacao = false;
        if (disciplinaNome.isEmpty()) {
            Logger.warn("Nenhuma disciplina foi inserida.");
        } else if (disciplinasAtividade.contains(disciplinaNome)) {
            Logger.warn("Disciplina já existe: " + disciplinaNome);
        }
        try {
            Double.parseDouble(disciplinaPrioridade);
            validacao = true;
        } catch (NumberFormatException e) {
            Logger.warn("Prioridade inválida para a disciplina: " + disciplinaNome);
            return validacao;
        }
        return validacao;
    }
```

---

## 📄 Documentação
Consulte [`docs/`](./docs) no link a seguir para:
- Diagramas UML  
- Planejamento do projeto  
- Relatórios técnicos  
- Slides de apresentação  

Fora isso, o link permite acessar o repositório com todo o código da aplicação, além de mais informações e detalhes sobre ela:

[https://github.com/SW-Engineering-Courses-Karina-Kohl/tcp-20252-final-grupo-04](https://github.com/SW-Engineering-Courses-Karina-Kohl/tcp-20252-final-grupo-04.git)

---

## 📈 Resultados e Aprendizados
- O que funcionou bem
   - Modularização das classes;
   - Organização dos arquivos do projeto;
   - Comunicação entre interface visual e restante do código.
- O que poderia melhorar
   - Diminuição do acoplamento entre certas classes;
   - Separação das tarefas entre os membros do grupo;
   - Remoção de code smells no código, como números mágicos.
- Conceitos mais aplicados da disciplina
   - Nomes de variáveis, métodos e classes significativos, expressivos e padronizados;
   - Uso de testes para verificar casos incomuns;
   - Refatoração de código sempre que possível.
- Lições para projetos futuros
   - Não subestimar um projeto, por mais simples que pareça ser a ideia;
   - Pensar na interface e em como criá-la desde cedo;
   - Definir prazos mais rígidos para entrega de código pelos membros da equipe.
