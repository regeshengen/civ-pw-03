# 🧊 Extração de Superfícies 3D com Marching Cubes  
### Visualização de Isosuperfícies em OpenGL

---

## 🎓 Contexto Acadêmico

Este repositório apresenta o código desenvolvido para um **trabalho prático da disciplina Computação Interativa e Visualização**, no âmbito do **Mestrado em Engenharia Informática** da **Universidade da Beira Interior (UBI)**.

- **Aluno:** Reges Hengen  
- **Número de aluno:** M13521  
- **Ano letivo:** 2025  
- **Tema do trabalho:** *Marching Cubes para Extração de Isosuperfícies*  

---

## 🧠 Descrição do Projeto

O projeto implementa o algoritmo **Marching Cubes**, amplamente utilizado para a **extração de superfícies tridimensionais (isosuperfícies)** a partir de campos escalares definidos em grades volumétricas.

A aplicação gera um **campo escalar sintético em 3D** (funções implícitas) e aplica o algoritmo para construir uma malha poligonal que representa a superfície correspondente a um valor de iso-superfície definido.  
O resultado é renderizado em tempo real utilizando **OpenGL**, permitindo a visualização da geometria gerada.

O objetivo principal do trabalho é explorar **técnicas de visualização científica**, enfatizando a geração geométrica baseada em dados volumétricos e sua integração com pipelines gráficos modernos.

---

## 🧰 Tecnologias Utilizadas

- **Linguagem:** C++17  
- **API gráfica:** OpenGL 4.6  
- **Bibliotecas:**  
  - **GLFW** – Criação de janela e contexto OpenGL  
  - **GLEW** – Gerenciamento de extensões OpenGL  
  - **GLM** – Matemática vetorial e matricial (3D)  

---

## 📁 Estrutura do Projeto

```bash
📦 marching-cubes
├── main.cpp          # Implementação completa do Marching Cubes e renderização OpenGL
├── edgeTable.txt     # Tabela de arestas do algoritmo Marching Cubes
├── triTable.txt      # Tabela de triangulação do algoritmo Marching Cubes
└── README.md         # Documentação do projeto

```

## 💻 Rodar no terminal

```bash
g++ main.cpp -o marching_cubes.exe -std=c++17 -lglfw3 -lglew32 -lopengl32
./marching_cubes.exe
```

<img width="799" height="634" alt="image" src="https://github.com/user-attachments/assets/f8c34b18-e4d9-4584-8693-ba2831855644" />
