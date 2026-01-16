# 🧩 Estruturas de Dados <Nome da Disciplina>
**Período:** 2024/2  
**Projeto:** Análise de Desempenho de Lista Simplesmente Encadeada e Árvore Binária de Pesquisa <Nome do projeto ou atividade principal>  
**Professor(a):** Renata de Matos Galante

---

## 🎯 Problema a ser resolvido

Realizar uma comparação, utilizando vários conjuntos extensos de dados distintos, entre uma ABP e uma LSE para verificar qual estrutura é mais adequada em cada cenário.

---

## 🏗️ Arquitetura e Tecnologias
Decisões técnicas e a estrutura do sistema:
- Linguagens utilizadas: C
- Estruturas de dados escolhidas: Lista Simplesmente Encadeada (LSE) e Árvore Binária de Pesquisa (ABP)
- Tipos de arquivos com dados usados: Textuais
- Site usado para gerar os gráficos: https://charts.livegap.com/?lan=pt#TypesofCharts

---

## 🧱 Boas práticas aplicadas
Práticas de Engenharia de Software seguidas neste projeto:
- Comentários descritivos
- Separação do projeto com arquivo de header para proteger código
- Uso de refatoração para facilitar testes com os dados

---

## 🤝 Soft Skills e Trabalho em Equipe
Reflexão pessoal sobre como contribuiu no projeto:
> - Fiz testes antes de decidir quais seriam as estruturas analisadas. 
> - Gerei todos os conjuntos de dados utilizados na análise.
> - Redigi o relatório detalhando e explicando os resultados. 
> - Criei os gráficos para ilustrar os resultados no relatório.

---

## 🧪 Exemplos de Código

```c
int Verifica_senhalse(PtNo* l, int user, char senha[20], int *complse)
{
    int x=0;
    PtNo* aux = l;
    while (aux != NULL)                          //enquanto aux não chegar ao final
    {
        *complse += 1;
        if (aux->user == user)                   //se o user de aux for igual ao user passado como parâmetro
        {
            *complse += 1;
            if (strcmp(aux->senha, senha) == 0)          //se a senha de aux for a mesma que a passada como parâmetro
            {
                *complse += 1;
                x = 1;                                        //retorna 1
                return x;
            }
            else
            {
                *complse += 1;
                x = -1;                                       //senão, a senha não é a mesma e retorna -1
                return x;
            }
        }
        *complse += 1;
        aux = aux->prox;                            //aux passa para o próximo elemento
    }
    *complse += 1;
    return x;                                    //caso não encontre o usuário, retorna 0
}
```

---

## 📄 Documentação
Contida em comentários em meio ao código e principalmente no relatório, que possui todas as análises relevantes da comparação.

---

## 📈 Resultados e Aprendizados
- O que funcionou bem
  - Criação dos dados usando manipulação de arquivos de texto em C;
  - Produção de gráficos usando as medidas obtidas;
  - Explicação coerente e detalhada dos resultados adquiridos.
- O que poderia melhorar
  - Uso de outras estruturas de dados mais complexas;
  - Alterar o nome das variáveis para que sejam mais expressivos;
  - Remover comentários desnecessários.
- Conceitos mais aplicados da disciplina
  - Separação do projeto em três arquivos distintos;
  - Uso de operações nas estruturas ABP e LSE;
  - Análise crítica e ilustrada por meio de gráficos dos resultados encontrados.  
- Lições para projetos futuros
  - Verificar a complexidade de certos trechos de código antes de testá-los;
  - Manter a calma quando não é possível avançar o projeto;
  - Dado um problema, pensar em e explorar múltiplas soluções para ele.
