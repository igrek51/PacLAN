/**
 * \file config.h
 * \see config.cpp
 */
#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include "log/log_level.h"

using namespace std;

//TODO zamienić na singleton

/**
 * \brief Ustawienia konfiguracyjne
 */
class Config {
public:
    //  OKNO
    /// szerokość okna aplikacji
    static int window_w;
    /// wysokość okna aplikacji
    static int window_h;
    /// tytuł okna
    static const char* app_name;
    /// wersja aplikacji
    static string version;
    /// uruchomienie trybu pełnoekranowego podczas startu (true - pełny ekran podczas startu)
    static bool fullscreen_start;

    //  SYNCHRONIZACJA
    /// okres timera cykli logicznych
    static int logic_timer_ms;
    /// krytyczna liczba niewykonanych cykli
    static int logic_cycles_critical;
    /// liczba klatek do uśredniania FPS
    static int fps_avg_frames;

    //  BITMAPY
    /// tablica nazw plików obrazów oraz odpowiadającym im nazwom
    static string bitmap_names[];
    /// nazwa pliku ikony aplikacji
    static string icon_name;

    //  CZCIONKI
    /// nazwa pliku czcionki nr 1
    static string fontfile1;
    /// rozmiar czcionki nr 1
    static int fontsize1;
    /// nazwa pliku czcionki nr 2
    static string fontfile2;
    /// rozmiar czcionki nr 2
    static int fontsize2;

    //  MAPA
    /// nazwa pliku definicji mapy
    static string mapDefault;

    //  INFO
    /// wyświetlanie liczby klatek na sekundę (true - włączone)
    static bool show_fps;
    /// wyświetlanie siatki pól mapy (true - włączone)
    static bool show_grid;
    /// wyświetlanie ścieżek do celów graczy (true - włączone)
    static bool show_paths;
    /// wyświetlanie nazw graczy (true - włączone)
    static bool show_names;

    //  LOG, OUTPUT
    /// nazwa pliku dziennika zdarzeń
    static const string log_filename;
    /// poziom logów zapisywanych w pliku
    static LogLevel file_log_level;
    /// poziom logów wyświetlanych w konsoli
    static LogLevel echo_log_level;
    /// maksymalna liczba wierszy wyjścia w konsoli
    static unsigned int echoes_max;

    //  POSTACIE
    /// domyślna szybkość pacmana (im mniesza wartość, tym większa szybkość)
    static int pacman_speed;
    /// domyślna szybkość duszka (im mniesza wartość, tym większa szybkość)
    static int ghost_speed;
    /// domyślna szybkość duszka w trybie zjadania duszków (im mniesza wartość, tym większa szybkość)
    static int ghost_eatme_speed;
    /// możliwość ruchu w trakcie odradzania się (true - włączone)
    static bool move_while_respawn;

    //  SZTUCZNA INTELIGENCJA
    /// AI pacmana: odległość od duszka, przy której pacman zaczyna uciekać
    static int ai_pacman_start_escape;
    /// AI pacmana: minimalna odległość od duszka, przy której pacman przestaje uciekać
    static int ai_pacman_stop_escape;

    //  GRA
    /// czas trwania trybu zjadania duszków (w cyklach)
    static int eating_time;
    /// czas trwania migania duszków oznamiającego, że kończy się tryb zjadania duszków
    static int eating_time_critical;
    /// czas oczekiwania na ponowne odrodzenie (w cyklach)
    static int respawn_time;
    /// czas oczekiwania do następnej rundy i zakończenia rundy (w cyklach)
    static int next_round_time;

    //  PUNKTY
    /// domyślna wartość punktów za zabicie pacmana
    static int points_kill_pacman;
    /// domyślna wartość punktów za zabicie duszka
    static int points_kill_ghost;

    //  TRYBY GRY: ZOMBIE
    /// w trybie zombie: czas do kolejnej śmierci pacmana (w sekundach)
    static int zombie_death_time;
    /// w trybie zombie: minimalna liczba kropek na mapie, przy której zostają dodane nowe kropki
    static int zombie_dots_threshold;
    /// w trybie zombie: ilość dodawanych kropek
    static int zombie_dots_add;
    /// w trybie zombie: liczba punktów za zjedzenie małej kropki
    static int zombie_points_small;
    /// w trybie zombie: liczba punktów za zjedzenie dużej kropki
    static int zombie_points_large;
    /// w trybie zombie: liczba punktów za zabicie pacmana
    static int zombie_points_kill_pacman;
    /// w trybie zombie: liczba punktów za zabicie duszka
    static int zombie_points_kill_ghost;
    /// w trybie zombie: czas trwania trybu zjadania duszków
    static int zombie_eating_time;
    /// w trybie zombie: szybkość pacmana
    static int zombie_pacman_speed;
    /// w trybie zombie: szybkość duszka
    static int zombie_ghost_speed;
    /// w trybie zombie: szybkosć duszka w trybie zjadania duszków
    static int zombie_ghost_eatme_speed;

    //  SIEĆ
    /// maksymalna liczba klientów
    static int max_clients;
    /// numer portu przy połączeniu
    static int port;
    /// maksymalna liczba powtórzeń w przypadku niewysłanego pakietu
    static int max_attempts;
    /// czas oczekiwania po niepowodzeniu wysłania pakietu (w ms)
    static int wait_if_failed;
    /// rozmiar bufora wejściowego przychodzących pakietów oraz bufora wyjściowego wysyłanych pakietów
    static int buffer_size;
};

#endif
