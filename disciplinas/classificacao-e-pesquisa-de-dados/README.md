# 🧩 Classificação e Pesquisa de Dados <Nome da Disciplina>
**Período:** 2025/1  
**Projeto:** Catálogo de filmes <Nome do projeto ou atividade principal>  
**Professor(a):** Leandro Krug Wives

---

## 🎯 Problema a ser resolvido

Desenvolver um catálogo de filmes que possua mais de 300 mil obras e permita ordenar, procurar, adicionar e ver estatísticas e dados sobre elas.

---

## 🏗️ Arquitetura e Tecnologias
Decisões técnicas e a estrutura do sistema:
- Linguagem utilizada: C
- Tipos de arquivos usados: binários
- Interface em linha de comando
- Sistema de arquivos modelado: FAT
- Estrutura de dados empregada: Árvores B

---

## 🧱 Boas práticas aplicadas
Práticas de Engenharia de Software seguidas neste projeto:
- Comentários descritivos em meio ao código
- Criação de funções gerais para evitar repetição de código
- Divisão em módulos com funcionalidades distintas
- Uso de refatoração contínua

---

## 🤝 Soft Skills e Trabalho em Equipe
Reflexão pessoal sobre como contribuiu no projeto:
> - Trabalhei de forma autônoma na implementação das árvores B.  
> - Busquei referências de código em sites e repositórios open-source.  
> - Organizei a divisão das tarefas entre os integrantes e acompanhei suas entregas.
> - Redigi uma parte considerável do relatório.

---

## 🧪 Exemplos de Código

```c
//localiza determinado titulo dentro da arvore B
long busca_titulo(BTreeNode *root, char titulo[256], long x) {
    if (root != NULL) {                                                            //se a raiz não é nula,
        int i=0;
        while(i < root->num_keys && strcmp(root->keys[i].titulo, titulo) < 0) {    //verifica a posição i onde o título do nodo é menor ou igual ao buscado
            i++;
        }
        if (strcmp(root->keys[i].titulo, titulo) == 0) {                          //se for igual, salva o offset do arquivo original binário em x
            x = root->keys[i].pos;
        } else {
            return busca_titulo(root->children[i], titulo, x);                     //se não, procura recursivamente em um dos filhos do nodo, carregando x junto durante a recursão
        }
    } else {
        return x;                                                                  //se o ponteiro é nulo, retorna x, que por padrão é -1
    }
}
```

---

## 📄 Documentação
Feita em comentários ao longo de todos os arquivos com código. Além disso, é possível acessar o repositório com as primeiras implementações do projeto com ideias iniciais e esboços de código no link abaixo.

https://github.com/FilipeMoura6416/Trabalho_Final-CPD.git

---

## 📈 Resultados e Aprendizados
- O que funcionou bem
  - Extração de dados para o catálogo;
  - Busca por títulos de filmes e exibição de seus dados;
  - Ordenamento de filmes e visualização paginada.
- O que poderia melhorar
  - Utilização de estatísticas mais elaboradas;
  - Atribuir nomes mais significativos para as variáveis;
  - Adicionar um sistema de filtragem de filmes mais robusto e incremental.
- Conceitos mais aplicados da disciplina
  - Operações em árvores B;
  - Planejamento do projeto com diagramas ER;
  - Análise assintótica para verificar tempo médio de operações do programa.
- Lições para projetos futuros
  - Garantir que todos membros do grupo comecem o projeto desde cedo;
  - Estabelecer prazos rígidos para as entregas de todos integrantes;
  - Manter um diálogo constante com toda a equipe ao longo do projeto.
