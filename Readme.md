# Ray Tracing

### Grupo 7
- Samuell Henrique Carvalho Paes Costa
- Pedro Calheiros
- Pedro Henrique Moraes de Sousa Silva
- Henrique Melo

<hr>

Pra rodar o cmake:

```
mkdir build 
cd .\build\ 
cmake -G "MinGW Makefiles" ..
```

After: <code> cd .. </code>

```
   cmake -B build
   cmake --build build
```

<hr>

### Entrega 1

#### Nessa fase, os grupos implementarão um Ray-Caster básico. Para isso, é preciso:

<details>
<summary> Definir pontos e operações sob pontos.</summary>
  - O grupo pode usar bibliotecas para trabalhar com pontos. Mas, também, é possível definir pontos como se fossem objetos e usar métodos para definir as operações.
</details>
<details>
<summary> Definir vetores e operações sob vetores </summary>
    - O grupo pode usar bibliotecas para trabalhar com vetores. Mas, também, é possível definir vetores como se fossem objetos e usar métodos para definiras operações.
</details>
<details>
<summary> Definir a câmera </summary> 
    
    Ela pode ser uma classe composta por:
    
    - Um ponto, que definirá sua localização no mundo $\small \longrightarrow C({c_1}, {c_2}, {c_3}) \; onde,  \;{c_1}, \;{c_2}, \;{c_3} \in \mathbb{R};$
    - Um outro ponto no mundo que será para onde a câmera aponta (ela sempre aponta para o centro da tela) $\small \longrightarrow M(x, y, z) \; onde, \; x, \; y, \; z \in \mathbb{R};$
    - Um vetor que parte da câmera e aponta para cima $\small \longrightarrow \; {V_{up}}({v_1}, {v_2}, {v_3}) \;onde, \;{v_1}, \;{v_2}, \;{v_3} \in \mathbb{R} \; e \; \;{v_{up}} \neq (vetor \;nulo);$
    - Três vetores ortonormais $\small \longrightarrow W({w_1}, {w_2}, {w_3}), \; V({v_1}, {v_2}, {v_3}) e \; U({u_1}, {u_2}, {u_3}) \; onde \; {w_1}, \; {w_2}, \; {w_3}, \; {v_1}, \; {v_2}, \; {v_3}, \; {u_1}, \; {u_2}, \; {u_3} \in \mathbb{R}.$  Por convenção, um desses vetores deve ter a mesma direção que (M − C), mas sentido oposto; o ponto da mira e o ponto da localização da câmera, respectivamente;
    - A distância entre a câmera e a tela $\small \longrightarrow d \in \mathbb{R_+}^*$
    - Um número que definirá a altura da tela $\small \longrightarrow {v_{res}} \in \mathbb{Z_+}^*;$
    - Um número que definirá a largura da tela $\small \longrightarrow {h_{res}} \in \mathbb{Z_+}^*.$
    
    OBS:  Utilizamos a resolução padrão dos pixels como 1x1. Isso quer dizer que, por exemplo, se olharmos apenas para a resolução horizontal da tela, ela será dividida em pixels de tamanho $\small \frac{1}{{h_{res}}}$ Neste projeto não será útil trabalhar com variantes dessa resolução.
    
    ***OBS:*** O ângulo de visão da câmera, no geral, é uma consequência direta da definição da resolução da tela e da distância que a câmera se encontra dela.
    
    - Com todos esses atributos o grupo deverá construir uma câmera móvel, ou seja, podendo ser posicionada em qualquer lugar e apontada para diferentes localizações. Ao mesmo tempo, esses atributos permitirão mapear todos os pixels da tela, que poderão ser encontrados pela soma de uma combinação de vetores da base da câmera pela localização da câmera.
</details>
<details>
<summary> Testar interseções </summary>
    - Esferas
        - Um ponto que determina seu centro $\small \longrightarrow \; {C_{\varepsilon}}(x, y, z) \; onde \; x, \; y, \; z \in \mathbb{R};$
        - O raio $\longrightarrow {r} \in \mathbb{R_+}^*;$
        - Cor RGB normalizada $\small \longrightarrow {O_d} \in [0,1]^3$
    - Planos
        - Um ponto pertencente ao plano $\small \longrightarrow {P_p}(x, y, z) \; onde \; x, \; y, \; z \in \mathbb{R}$
        - Um vetor normal ao plano $\small \longrightarrow {V}({v_1}, {v_2}, {v_3}) \; onde \; {v_1}, \; {v_2}, \; {v_3} \in \mathbb{R}$
        - Cor RGB normalizada $\small \longrightarrow {O_d} \in [0, 1]^3$
</details>

OBS: Será necessário renderizar a cena com os objetos (esferas e planos).
<hr>
Repositório base em C++ para o projeto da disciplina de **Processamento Gráfico**.

Este repositório fornece um ponto de partida para a implementação de um sistema de Ray Tracing. Inclui um sistema de leitura de arquivos `.obj` e `.mtl` para facilitar a criação de casos de testes **a partir da segunda entrega**.

## Organização do Projeto

A implementação do projeto é livre para a decisão do grupo, abrangendo:
- Organização do código;
- Estratégias e métodos de implementação;
- Contanto que sigam a **teoria ensinada em sala de aula**.

## Entregas

Informações detalhadas sobre as entregas estão disponíveis no **[Notion](https://alabaster-rodent-725.notion.site/Processamento-Gr-fico-156f7123bb6a80d3a4fbf351828ae42e)**. Em resumo:
- São **4 entregas**, cada uma acompanhada de vídeos explicativos sobre a teoria;
- Os vídeos podem ser acessados através do link no Notion.

### **Avaliação**
A avaliação de cada entrega incluirá:
1. Apresentação demonstrando o funcionamento do código.
2. Explicação teórica individual de cada integrante.

## Requisitos

Certifique-se de ter os seguintes pré-requisitos configurados:
- **Compilador C++** (suporte para C++11 ou superior);

## Como executar

1. Abra o terminal no mesmo diretório do arquivo main.cpp
2. Digite `g++ main.cpp` 
3. Execute o arquivo gerado no terminal, chamando `./nome_do_arquivo`

Obs: Ao compilar, é possível adicionar o parâmetero `-O3` que deixa a execução do código mais rápido, ficando `g++ main.cpp -O3`.
