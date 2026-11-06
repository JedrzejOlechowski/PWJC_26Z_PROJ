PWJC — przykładowe dane do projektu asocjacji punktów

Zawartość:
- data/: pliki zbiorów A i B, zgodne ze specyfikacją Word;
- expected/: jeden przykładowy wynik globalnie optymalny dla każdego małego testu;
- cases.csv: parametry 11 poprawnych scenariuszy i oczekiwane koszty;
- invalid/: 7 celowo błędnych plików;
- commands_windows.txt i commands_linux.txt: przykładowe polecenia.

Uruchamiaj polecenia z katalogu tej paczki. Nazwę prog.exe / prog dostosuj
jedynie do nazwy zbudowanego programu. Polecenia nie nadpisują wzorców expected/.

WAŻNE
Wyniki expected/ są wzorcami dla trybu exact. Tryb basic może dać inny koszt.
Tryb local może dać wynik gorszy od optimum, ale musi być poprawny i nie może
pogarszać własnego rozwiązania początkowego. Kolejność par nie ma znaczenia.
Przy remisach porównuj koszt i ograniczenia, a nie identyczność tekstu.
Dla 08_single_tie poprawne są 0 lub 1 para; dla 10_duplicates istnieją dwa
równoważne pełne przypisania. Współrzędne mogą się powtarzać, identyfikatory
w jednym zbiorze — nie. Pusty zbiór ma nagłówek i points 0. Pusty plik jest błędem.
Koszty małych scenariuszy zweryfikowano pełnym przeglądem dopuszczalnych
skojarzeń i niezależnie solverem przydziału. Paczka nie zawiera kodu rozwiązania.

DUŻY PRZYKŁAD
large_4d_A.txt: 1000 punktów, large_4d_B.txt: 1200 punktów, dimension 4.
Przykładowe parametry: threshold 0.75, unmatched_penalty 0.4.
Ziarno generatora: 20261017. A w [0,10]^4. B zawiera 800 zaburzonych kopii
punktów A (szum normalny, odchylenie 0.05 w każdym wymiarze) i 400 nowych
punktów, a kolejność B została przetasowana. Współrzędne zaokrąglono do
6 miejsc po przecinku. Dla tego przykładu nie podano wzorca optimum.
Pochodzenie punktu z generatora nie jest dowodem, że dana para należy do optimum.

PLIKI BŁĘDNE
- dimension_mismatch_B.txt: użyj z data/basic_A.txt; niezgodne wymiary zbiorów;
- missing_coordinate_B.txt: użyj z data/3d_A.txt; za mało współrzędnych;
- duplicate_id_A.txt: użyj z data/basic_B.txt; powtórzony identyfikator;
- wrong_count_A.txt: użyj z data/basic_B.txt; inna liczba rekordów niż points;
- nonfinite_A.txt: użyj z data/basic_B.txt; niedozwolona wartość nan;
- dimension_one_A.txt: użyj z data/basic_B.txt; dimension < 2;
- empty_file_A.txt: użyj z data/basic_B.txt; brak wymaganych nagłówków.
Każdy taki test powinien zakończyć się diagnostyką i niezerowym kodem procesu,
a nie plikiem udającym poprawny wynik. Samo matches 0 nie oznacza błędu.

W plikach liczbowych separatorem dziesiętnym jest kropka. Koszty należy
porównywać z tolerancją wskazaną w specyfikacji, a nie jako ciągi znaków.
