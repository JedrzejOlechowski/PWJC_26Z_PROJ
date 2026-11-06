# PWJC 26Z — projekt

Projekt zaliczeniowy z przedmiotu PWJC (C++20).

## Budowanie i uruchomienie

```bash
g++ -std=c++20 -Wall -Wextra main.cpp -o main
./main
```

## Uruchamianie testów

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure --verbose
```


## Autor

Jędrzej Olechowski
