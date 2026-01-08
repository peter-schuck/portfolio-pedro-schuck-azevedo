# 🧩 <Nome da Disciplina>
**Período:** 2025/2  
**Projeto:** Studify
**Professor(a):** Karina Kohl

---

## 🎯 Problema a ser resolvido
Descreva brevemente o problema que o projeto aborda.

Uma aplicação para estudantes universitários organizarem seu tempo livre para estudar para provas, trabalhos e outras atividades

---

## 🏗️ Arquitetura e Tecnologias
Explique as decisões técnicas e a estrutura do sistema:
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
Liste práticas de Engenharia de Software seguidas neste projeto:
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
public double calcularDesconto(Produto p) {
    return p.getPreco() * TAXA_DESCONTO;
}
```

---

## 📄 Documentação
Consulte [`docs/`](./docs) para:
- Diagramas UML  
- Planejamento de testes  
- Relatórios técnicos  
- Slides de apresentação  

---

## 📈 Resultados e Aprendizados
- O que funcionou bem  
- O que poderia melhorar  
- Conceitos mais aplicados da disciplina  
- Lições para projetos futuros
