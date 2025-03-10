# Ray Tracing

### Grupo 7
- Samuell Henrique Carvalho Paes Costa
- Pedro Calheiros
- Pedro Henrique Moraes de Sousa Silva
- Henrique Melo

<hr>

Pra rodar a geração de imagens

```
g++ .\main.cpp -O3 -fopenmp
 .\a.exe
```

O arquivo PPM será criado em out.ppm e a imagem será renderizada automaticamente em renders/.

<hr>

Pra rodar o Space (cmake):

Faça isso da primeira vez que for rodar o projeto
```
   mkdir build 
   cd .\build\ 
   cmake -G "MinGW Makefiles" ..
   cd ..
   cmake -B build
```

Depois disso você pode fazer as builds e rodar o programa assim

```
   cmake --build build
   .\build\bin\space.exe
```

<hr>