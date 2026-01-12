# 🧩 <Nome da Disciplina>
**Período:** 2024/1  
**Projeto:** INF Dash 3D

**Professor(a):** Marcelo Walter

---

## 🎯 Problema a ser resolvido
Descreva brevemente o problema que o projeto aborda.

Desenvolvimento de um jogo, similar ao Geometry Dash, porém em 3D, feito inteiramente em C com auxílio de bibliotecas visuais.

---

## 🏗️ Arquitetura e Tecnologias
Explique as decisões técnicas e a estrutura do sistema:
- Linguagens de programação utilizadas: C
- Biblioteca visual empregada: Raylib
- Tipos de arquivos usados: Binários e de texto
 
---

## 🧱 Boas práticas aplicadas
Liste práticas de Engenharia de Software seguidas neste projeto:
- Documentação com comentários ao longo do código
- Princípio de código linear
- Uso de reuniões para acompanhar o progresso de cada integrante da equipe

---

## 🤝 Soft Skills e Trabalho em Equipe
Reflexão pessoal sobre como contribuiu no projeto:
> - Revisei código e corrigi bugs de outros membros da equipe.  
> - Trabalhei de forma autônoma na transição entre telas.  
> - Criei e executei o planejamento da ordem das tarefas do projeto.

---

## 🧪 Exemplos de Código

```c
jogador Initjogador()
{
    jogador jogador;
    jogador.posicao = (Vector3){1.5f, NASCIMENTO, 0.0f};  // Posição inicial
    jogador.tamanho = (Vector3){1.0f, 1.0f, 1.0f};      // Tamanho do Jogador
    jogador.color = (Color){248, 220, 158, 255};       // Cor do Jogador
    jogador.velocidade = 0.0f; // Velocidade do jogador
    jogador.isGrounded = false; //Booleano para poder pular
    jogador.isAlive = true;
    jogador.contm = 0; // Contador de mortes//
    jogador.roll = 0.0f;  // Inicializa a variável de rotação
    jogador.transform = MatrixIdentity(); // Inicializa a transformação de matrizes
    jogador.corrida = true; //inicializa a variável de corrida para true (correr constantemente)
    return jogador;
}
```

---

## 📄 Documentação
Contida no arquivo do projeto, em meio ao código.

---

## 📈 Resultados e Aprendizados
- O que funcionou bem
  - Divisão das tarefas entre integrantes do grupo;
  - Respeito a prazos e datas de entrega;
  - Escolha do design e demais visuais do jogo.
- O que poderia melhorar
  - Leitura do mapa por meio de arquivos;
  - Reduzir acoplamento entre funções;
  - Renomear variáveis para tornar o código mais claro.
- Conceitos mais aplicados da disciplina
  - Separação de código em funções;
  - Utilização de leitura e escrita em arquivos binários e de texto;
  - Uso de constantes para valores centrais do código.
- Lições para projetos futuros
  - Utilizar nomes claros para variávies;
  - Tornar o código mais modular sempre que possível;
  - Padronizar nomes de funções e variáveis.
