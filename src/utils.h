/**
 * \file utils.h
 * Przydatne operacje na stringach i obsługa funkcji systemowych
 * \see utils.cpp
 */
#ifndef UTILS_H
#define UTILS_H

#include <sstream>

using namespace std;

/**
 * wyczyszczenie strumienia stringstream
 * @param sstream strumień
 */
void ss_clear(stringstream& sstream);

/**
 * sprawdza czy plik ma podane roszerzenie
 * @param filename nazwa pliku
 * @param ext rozszerzenie
 * @return true - jeśli plik ma podane rozszerzenie, w przeciwnym razie - false
 */
bool has_extension(string filename, string ext);

/**
 * zaalokowanie pamięci i skopiowanie zawartości stringa
 * @param s string do skopiowania
 * @return tablica znaków char będąca kopią znaków ze stringa, zakończona 0
 */
char* string_to_char(string s);

/**
 * zamienia liczbę na ciąg znaków
 * @param number liczba całkowita
 * @return tekstowa reprezentacja liczby
 */
string itos(int number);

/**
 * zwraca obecną godzinę jako tekst
 * @return obecna godzina w formacie HH:mm:ss
 */
string get_time();

/**
 * zaokrąglenie dodatniej liczby
 * @param d liczba dodatnia
 * @return zaokrąglona liczba całkowita
 */
int round_to_int(double d);

/**
 * odczekuje w aktualnym wątku określoną liczbę milisekund
 * @param ms liczba milisekund do odczekania
 */
void sleep_ms(int ms);

/**
 * zwraca aktualny timestamp w milisekundach
 * @return aktualny timestamp w milisekundach
 */
long long currentMillis();

#endif
