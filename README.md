# PWJC 26Z — projekt

Projekt zaliczeniowy z przedmiotu PWJC (C++20).

## Budowanie i uruchomienie

```bash
cmake -S . -B build
cmake --build build
./build/prog
```

## Uruchamianie testów

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure --verbose
```


## Autor

Jędrzej Olechowski
