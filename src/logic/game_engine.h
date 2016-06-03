#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

//pozycja menu
#define MENU_NAME 1
#define MENU_SUBCLASS 2
#define MENU_COLOR 3
#define MENU_HOST 4
#define MENU_JOIN 5
#define MENU_LANGUAGE 6
#define MENU_EXIT 7
#define MENU_MAX 7 //wartość ostatniego

//tryby gry
#define MODE_CLASSIC 0
#define MODE_ZOMBIE 1
#define MODE_MAX 1 //ostatnia wartość trybu gry

class GameEngine;

#include <vector>
#include <sstream>
#include "../graphics/graphics.h"
#include "../map/map.h"
#include "../map/pathfind/pathfind.h"
#include "../map/item.h"
#include "../players/player.h"
#include "../players/pacman.h"
#include "../players/ghost.h"
#include "../network/network.h"

/**
 * \brief logika gry
 */
class GameEngine{
public:
    /// inicjalizacja silnika gry
    GameEngine(Graphics* graphics);
    /// zamknięcie silnika gry
    ~GameEngine();

    void setNetwork(Network* network);
    /**
     * wykonanie logicznego cyklu i zmniejszenie licznika cykli
     * @param logic_cycles zmienna zwiększana przez timer
     */
    void logic(volatile int &logic_cycles);
    /**
     * zdarzenia klawiatury (powtarzane po pewnym czasie podczas przytrzymania)
     * @param keysym kod klawisza
     */
    void keyboard_event(SDL_Keysym keysym);
    ///liczba cykli od uruchomienia gry
    unsigned long long int cycles;
private:

    Graphics* graphics;

    Network* network;
    /// stan wciśnięcia każdego z klawiszy
    const Uint8 *keystate;
    /// zdarzenie SDL
    SDL_Event event;
    /// podprogram logic(): wybór w menu
    void logic_menu();
    /// podprogram logic(): gra na mapce
    void logic_game();

    //  CMD
public:
    ///zmienna określająca czy wiersz poleceń/konsola jest włączony (true - włączony)
    bool cmd_on;
    ///aktualnie wpisywane polecenie wiersza poleceń
    string cmd_in;
private:
    /**
     * wykonanie polecenia wiersza poleceń
     * @param cmd polecenie do wykonania
     */
    void exec_cmd(string cmd);
    /**
     * dopisanie znaku (lub usunięcie) do aktualnie wpisywanego polecenia
     * @param key kod klawisza
     */
    void cmd_input_char(SDL_Keycode key);
    /**
     * zamiana kodu klawisza na znak
     * @param key kod klawisza
     * @return znak odpowiadający klawiszowi
     */
    char key_to_char(SDL_Keycode key);
    /// ostatnio wpisane polecenie
    string cmd_last;

    //  GRACZE
public:
    /// lista wszystkich graczy
    vector<Player*> players;
    ///tymczasowa lista pacmanów
    vector<Pacman*> pacmans;
    ///tymczasowa lista duszków
    vector<Ghost*> ghosts;
    /**
     * podaje liczbę graczy danej podklasy
     * @param subclass podklasa do policzenia liczebności
     * @return liczba graczy o danej podklasie
     */
    int count_players(int subclass);
    /**
     * konwersja współrzędnych mapy na współrzędne ekranu
     * @param xmap współrzędna x w osi mapy
     * @return współrzędna x w osi ekranu
     */
    int xmap_to_x(int xmap);
    /**
     * konwersja współrzędnych mapy na współrzędne ekranu
     * @param ymap współrzędna y w osi mapy
     * @return współrzędna y w osi ekranu
     */
    int ymap_to_y(int ymap);
    /**
     * konwersja współrzędnych mapy na położenie na ekranie dla odpowiedniego gracza
     * @param player wskaźnik na gracza
     */
    void xymap_to_xy(Player *player);
    /**
     * sprawdzenie poprawności wciśniętych kierunków
     * @param player1 wskaźnik na gracza
     * @param moving stan obecnego ruchu gracza
     * @param direction obecny kierunek gracza
     * @param next_direction następny planowany kierunek gracza
     */
    void check_next(Player *player1, int &moving, int &direction, int next_direction);
    /**
     * zwraca kierunek, aby podążać ścieżką
     * @param xmap współrzędna x w układzie mapy
     * @param ymap współrzędna y w układzie mapy
     * @param direction kierunek gracza
     * @param sciezka wskaźnik na ścieżkę, którą ma podążać gracz
     */
    void follow_path(int xmap, int ymap, int &direction, Path<int> *sciezka);
    /**
     * przybliżona odległość 2 graczy
     * @param player1 wskaźnik na gracza 1
     * @param player2 wskaźnik na gracza 2
     * @return przybliżona odległość w jednostkach mapy
     */
    int distance_m(Player *player1, Player *player2);
private:
    /**
     * zwraca następne pole gracza
     * @param player wskaźnik na gracza
     * @param direction obecny kierunek gracza
     * @return wartość następnego pola w jakim znajdzie się gracz
     */
    char next_grid(Player *player, int direction);
    /**
     * usuń gracza i utwórz animację zanikania
     * @param index numer gracza
     */
    void kill_player(int index);
    /**
     * wykrywa kolizję dwóch graczy
     * @param player1 gracz 1
     * @param player2 gracz 2
     * @return true - jeśli wystąpiła kolizja
     */
    static bool is_collision(Player *player1, Player *player2);
    ///stwórz nowe tymczasowe listy duszków i pacmanów
    void update_players_lists();
    ///posortuj listę graczy według punktów
    void sort_players(vector<Player*> *lista);
    ///posortuj obie listy: duszków i pacmanów według punktów
    void sort_players();
    /**
     * zmienia podklasę gracza
     * @param index numer gracza
     * @param subclass P_PACMAN - pacman, P_GHOST - duszek, -1 - przeciwna klasa
     * @return
     */
    Player *change_subclass(int index, int subclass=-1);
    /**
     * dodaje nowego gracza
     * @param subclass podklasa gracza
     * @param name nazwa gracza
     * @param color kolor gracza
     * @param controlby zmienna określająca, przez co gracz jest sterowany: P_KEYBOARD - klawiatura, P_AI - sztuczna inteligencja, P_LAN - komunikaty sieciowe z serwera
     * @param ai_level poziom inteligencji gracza (jeśli jest sterowany przez sztuczną inteligencję)
     * @return wskaźnik na nowo utworzonego gracza
     */
    Player* add_player(int subclass, string name, SDL_Color color, int controlby, int ai_level=-1);
    /**
     * usuń gracza z listy graczy
     * @param index numer gracza
     */
    void kick_player(int index);

    //  MAPA
public:
    ///wskaźnik na aktualną mapę
    Map *map;
    /**
     * wyznacza losowe położenie na mapie spełniające warunki
     * @param x wynik - położenie x na mapie
     * @param y wynik - położenie y na mapie
     * @param pattern ciąg znaków, określających warunki wylosowanego pola
     */
    void random_field(int &x, int &y, string pattern);
    ///silnik szukania drogi
    Pathfind<int> *pathfind;
private:
    /**
     * sprawdza czy pole mapy spełnia określone warunki pól
     * @param x położenie x na mapie
     * @param y położenie y na mapie
     * @param pattern ciąg znaków zawierający pola spełniające warunki
     * @return true - jeśli pole spełnia warunki
     */
    bool is_field_correct(int x, int y, string pattern);
    /**
     * sprawdza czy pole jest puste, czy zawiera jakiś obiekt
     * @param x położenie x na mapie
     * @param y położenie y na mapie
     * @return true - jeśli pole jest puste
     */
    bool is_field_empty(int x, int y);
    ///inicjalizacja algorytmu znajdowania drogi
    void pathfind_init();

    //  OBIEKTY
public:
    ///lista obiektów na mapie
    vector<Item*> items;
    /**
     * odległość gracza od obiektu
     * @param player1 wskaźnik na gracza
     * @param item2 wskaźnik na obiekt
     * @return odległość w jednostkach mapy
     */
    int distance_m(Player *player1, Item *item2);
private:
    /// wstawienie obiektów na mapę
    void insert_items();
    /**
     * wykrywa kolizję gracza z obiektem
     * @param player1 wskaźnik na gracza
     * @param item2 wskaźnik na obiekt na mapie
     * @return true - jeśli wystąpiła kolizja
     */
    static bool is_collision(Player *player1, Item *item2);
    /**
     * liczy poszczególne obiekty na mapie
     * @param subclass podklasa obiektu do policzenia
     * @return liczba obiektów na mapie
     */
    int item_count(int subclass);
    /**
     * dodaje przedmiot w pustym losowym miejscu opisanym wzorem
     * @param subclass podklasa obiektu
     * @param pattern ciąg znaków zawierający pola spełniające warunki
     */
    void item_add(int subclass, string pattern);

    //  GRA
public:
    /// określa, czy włączona jest pauza (true - włączona)
    bool pause;
    /// pozostały czas trwania trybu zjadania duszków (0 - wyłączony)
    int eating;
    /// tryb gry: MODE_CLASSIC lub MODE_ZOMBIE
    int mode;
    ///licznik pomocniczy 1 do trybu gry
    int game_c1;
    ///licznik pomocniczy 2 do trybu gry
    int game_c2;
    ///licznik pomocniczy 3 do trybu gry
    int game_c3;
    /// czas odliczania do następnej rundy
    int round_next;
private:
    /**
     * usunięcie obiektów, reinicjalizacja rundy, bez usuwania graczy
     */
    void clear_map();
    ///usunięcie wszystkiego na mapie
    void clear_all();
    /**
     * restart nowej rundy
     */
    void restart_map();
    ///początkowe ustawienia rundy dla wybranego trybu
    void mode_init();
    /**
     * aktualizacja całej mapki i graczy dla wybranego klienta
     * @param client_id numer klienta
     */
    void update_map(int client_id);

    //  SYNCHRONIZACJA SIECIOWA
private:
    /**
     * dzieli pakiety i wykonuje je (\n dzieli pakiety, \r dzieli polecenia w obrębie pakietu)
     * @param from numer nadawcy pakietu
     * @param packet zawartość pakietu
     */
    void network_packets_split(int from, string packet);
    /**
     * wykonanie pojedynczego pakietu
     * @param from numer nadawcy pakietu
     * @param packet zawartość pakietu
     */
    void network_packet_process(int from, string packet);
    /**
     * wysyła pakiet do wszystkich kientów, lub jednego klienta
     * @param packet zawartość pakietu
     * @param to indeks pojedynczego klienta lub -1 - do wszystkich klientów
     */
    void network_send_to_clients(string packet, int to=-1);
    /**
     * wysyła pakiet do serwera
     * @param packet zawartość pakietu
     */
    void network_send_to_server(string packet);
    /// strumień pakietu synchronizującego
    stringstream synchro;
    /**
     * synchronizacja położenia graczy u klientów
     * @param to indeks pojedynczego klienta lub -1 - do wszystkich klientów
     */
    void synchronize_players(int to=-1);

    //  MENU
public:
    ///pozycja w menu (0 - wyłączone menu)
    int menu;
    ///pomocniczy obiekt pacmana rysowany tylko w menu
    Pacman *menu_pacman;
    ///pomocniczy obiekt duszka rysowany tylko w menu
    Ghost *menu_ghost;
    ///wybrane imię gracza
    string menu_name;
    ///wybrany kolor gracza
    SDL_Color menu_color;
    ///wybrana podklasa gracza: pacman lub duszek
    int menu_subclass;
    ///adres ip lub nazwa komputera do połączenia
    string menu_ip;
private:
    ///odświeżenie kolorów tekstur po zmianie koloru gracza
    void menu_refresh_color();
    ///wybranie z menu założenia serwera
    void menu_host();
    ///wybranie z menu dołączenia do hosta
    void menu_join();

    //  JĘZYK
public:
    void language_refresh();
};

#endif
