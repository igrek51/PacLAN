/**
 * \file log.h
 * Moduł obsługi dziennika zdarzeń oraz przydatnych funkcji
 * \see log.cpp
 */
#ifndef LOG_H
#define LOG_H

#include <iostream>

using namespace std;

/**
 * zwraca obecną godzinę jako tekst
 * @return obecna godzina w formacie HH:mm:ss
 */
string get_time();

/// wyczyszczenie zawartości pliku dziennika
void log_clear();
/**
 * zapisanie do dziennika ciągu znaków
 * @param l ciąg znaków do zapisania
 */
void log(string l);
/**
 * zapisanie do dziennika liczby całkowitej
 * @param l liczba całkowita do zapisania
 */
void log(int l);
/**
 * wyświetlenie komunikatu o błędzie
 * @param s treść komunikatu
 */
void error(string s);

//przydatne funkcje
/**
 * wyczyszczenie strumienia
 * @param sstream strumień
 */
void ss_clear(stringstream &sstream);
/**
 * zaokrąglenie dodatniej liczby
 * @param d liczba dodatnia
 * @return zaokrąglona liczba całkowita
 */
int round_to_int(double d);
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

#endif
