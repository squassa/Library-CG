# Biblioteca CG

Este projeto é uma simulação gráfica em **OpenGL** que representa a biblioteca do campus da Unesp de São José do Rio Preto (IBILCE) renderizada em 3D.  
Ele utiliza **GLUT** para gerenciar janelas, eventos de teclado e mouse, permitindo navegação interativa pela cena.

---

## Funcionalidades

- Renderização de:
  - Prédio em 3D (com porta e paredes detalhadas);
  - Chão com jardim;
  - Estrutura de prateleiras (função disponível, mas comentada).
- Controle da câmera via **mouse** e **teclado**:
  - Mudança entre projeção ortogonal e perspectiva;
  - Movimentação em primeira pessoa (WASD);
  - Controle de altura (Page Up / Page Down);
  - Rotação da câmera com movimento do mouse.

---

## Tecnologias

- Linguagem **C**
- **OpenGL**
- **GLUT** (OpenGL Utility Toolkit)
- **GLU**

---

## Requisitos

Antes de compilar, instale as bibliotecas necessárias:  

### Ubuntu/Debian
```bash
sudo apt-get update
sudo apt-get install freeglut3-dev
```

## Compilação e execução
```bash
gcc biblio.c -o biblio -lglut -lGL -lGLU -lm
./biblio
```

## Controles
### Teclado
W → andar para frente  
S → andar para trás  
A → mover para a esquerda  
D → mover para a direita  
Page Up → aumentar altura da câmera  
Page Down → diminuir altura da câmera  
ESC → sair do programa  
  
### Mouse
Movimento → rotaciona a visão da câmera  
Botão esquerdo → muda para projeção ortogonal (vista próxima)  
Botão direito → muda para projeção em perspectiva (vista distante)
