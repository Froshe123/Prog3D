# Programmation 3D — HAI719I

TP du module HAI719I « Programmation 3D » (Master Informatique, parcours IMAGINE, Université de Montpellier), en C++ avec OpenGL moderne.

Axel Kramer

## Contenu

| Dossier | TP | Notions |
|---|---|---|
| `TP1` | Introduction à OpenGL | compilation des shaders, VBO, EBO, structure `Mesh`, maillages `.off` |
| `TP2` | Vertex Array Objects et transformations | VAO, variables uniformes, matrices model et MVP |
| `TP3` | Transformations dépendantes et éclairage de Phong | transformations hiérarchiques, normales, éclairage et matériaux de Phong |
| `TP5_Textures` | Textures | coordonnées UV, chargement de textures, filtrage, mipmaps, normal mapping |

Les dossiers TP1 à TP3 contiennent aussi leur compte rendu en PDF (pour le TP3 : `tp3/tp3.pdf`, sources LaTeX dans `tp3_compte_rendu_latex`).

## Compilation

### TP1 à TP3 — Makefile (GLUT, GLEW, GLM)

Dépendances :
- Linux (Debian/Ubuntu) : `sudo apt install freeglut3-dev libglew-dev libglm-dev`
- macOS (TP2 et TP3) : `brew install glew glm` (OpenGL et GLUT viennent du système)

```bash
cd TP1          # ou TP2, ou TP3/tp3
make
./bin/tp
```

Lancer le programme depuis le dossier du TP : les shaders sont chargés par chemin relatif.

### TP5_Textures — CMake (GLFW, GLEW, GLM et stb_image fournis)

```bash
cd TP5_Textures
./make.sh       # configure, compile puis lance
```

En cas d'erreur de compilation, supprimer le dossier `build_files` puis relancer `./make.sh`.

Navigation : ZQSD ou flèches, Maj et Espace, clic gauche pour tourner la caméra.
