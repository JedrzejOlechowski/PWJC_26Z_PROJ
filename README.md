# PWJC 26Z — projekt

Projekt zaliczeniowy z przedmiotu PWJC (C++20).

## Budowanie i uruchomienie

```bash
cmake -S . -B build
cmake --build build
```

## Uruchomienie programu

### Uruchomienie z domyślnymi danymi
```bash
./build/prog
```
### Uruchomienie z podaniem adresu danych

```bash

./build/prog ścieżka_do_pliku_A ścieżka_do_pliku_B 

```
np.

```bash
./build/prog PWJC_przyklady/data/3d_A.txt PWJC_przyklady/data/3d_B.txt 
```

## Uruchamianie testów

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure --verbose
```


## Autor

Jędrzej Olechowski
Piotr Kozłowski
