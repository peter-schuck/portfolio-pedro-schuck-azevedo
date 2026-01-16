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
- Linguagens e frameworks utilizados: C++, PostgresSQL
- Padrões de projeto aplicados
- Estrutura de pastas e responsabilidades
- Estratégias de testes e verificação

---

## 🧱 Boas práticas aplicadas
Práticas de Engenharia de Software seguidas neste projeto:
- Versionamento semântico e commits descritivos
- Princípios **SOLID**,
- Uso de ferramentas de linting, revisão e CI

---

## 🤝 Soft Skills e Trabalho em Equipe
Reflexão pessoal sobre como contribuiu no projeto:
> - Liderei o grupo nas definições de arquitetura.  
> - Trabalhei de forma autônoma na implementação de um módulo.  
> - Busquei referências em artigos e repositórios open-source.  
> - Organizei reuniões e apresentei progresso.

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
  -  
- O que poderia melhorar
  -   
- Conceitos mais aplicados da disciplina
  - 
- Lições para projetos futuros
  - 
