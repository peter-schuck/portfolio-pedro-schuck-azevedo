# 🧩 Fundamentos de Banco de Dados <Nome da Disciplina>
**Período:** 2025/2  
**Projeto:** Base de Dados para o Youtube <Nome do projeto ou atividade principal>  
**Professor(a):** Karin Becker <Nome do professor>

---

## 🎯 Problema a ser resolvido

Modelar uma Base de Dados para as informações contidas no aplicativo Youtube, escolhendo quais dados manter e quais relações lógicas existem entre seus elementos.
Além disso, realizar consultas sobre estes elementos e criar um programa que permita a um usuário realizar estas mesmas consultas, preenchendo algumas com os parâmetros que desejar.

---

## 🏗️ Arquitetura e Tecnologias
Decisões técnicas e a estrutura do sistema:
- Linguagem utilizada: C++ 
- Banco de Dados relacional empregado: PostgreSQL
- Tipo de conexão com a Base de Dados feita: SQL embutida
- Diagrama conceitual usado para modelagem: Entidade-Relacionamento (ER)

---

## 🧱 Boas práticas aplicadas
Práticas de Engenharia de Software seguidas neste projeto:
- Comentários descritivos ao longo do código
- Modularização do programa em funções para preparar as consultas
- Uso de refatoração em consultas ambíguas
- Aplicação de testes manuais para garantir o funcionamento correto

---

## 🤝 Soft Skills e Trabalho em Equipe
Reflexão pessoal sobre como contribuiu no projeto:
> - Liderei o grupo na divisão de tarefas.  
> - Trabalhei de forma autônoma na construção de certas consultas.  
> - Busquei referências em sites e repositórios open-source.  
> - Redigi boa parte do Universo de Discurso e Dicionário de Dados.

---

## 🧪 Exemplos de Código

```c++
void mostrar_resultados(const db::result& r)
{
    cout << "\nExibindo resultados:" << endl;
    for (const auto& row : r) {
        for (const auto& field : row) {
            std::cout << field.c_str() << "\t";
        }
        std::cout << std::endl;
    }
}
```

---

## 📄 Documentação
Consulte o link abaixo para acessar o código da aplicação interativa:

https://github.com/FilipeMoura6416/Etapa_3_FBD.git

Fora isso, é possível encontrar nesta pasta
- Diagrama ER da modelagem do Youtube
- Universo de Discurso considerado
- Dicionário de Dados

---

## 📈 Resultados e Aprendizados
- O que funcionou bem
  - Criação de consultas diversas que abordam várias entidades e relacionamentos;
  - Programa funcional que se conecta devidamente com a base de dados;
  - Produção do diagrama ER considerando as entidades mais relevantes da plataforma. 
- O que poderia melhorar
  - Simplificar o Dicionário de Dados;
  - Melhorar e restringir a definição do Universo de Discurso;
  - Padronizar o nome de variáveis no código.
- Conceitos mais aplicados da disciplina
  - Modelagem ER;
  - Mapeamanto de um diagrama ER para tabelas em SQL;
  - SQL embutida com linguagem hospedeira C++.
- Lições para projetos futuros
  - Garantir que todos os membros da equipe contribuam para todas as partes do projeto;
  - Usar linugagens de progrmação que todos os integrantes tenham familiaridade;
  - Discutir de forma saudável sobre decisões que causem discordância entre o grupo.
