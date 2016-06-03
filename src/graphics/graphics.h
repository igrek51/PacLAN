#ifndef GRAPHICS_H
#define GRAPHICS_H

//wyrównanie tekstu
//TODO enum class
#define TEXT_LEFT -1
#define TEXT_CENTER 0
#define TEXT_RIGHT 1

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <vector>
#include <ctime>

class Graphics;

#include "animations/animation.h"
#include "../logic/game_engine.h"
#include "../language.h"

using namespace std;

/**
 * \brief Klasa silnika grafiki
 */
class Graphics{
public:
    ///inicjalizacja silnika grafiki
    Graphics();
    ///zamknięcie silnika grafiki
    ~Graphics();

    void setGameEngine(GameEngine* game_engine);
private:
    ///wskaźnik do okna SDL
    SDL_Window *sdl_win;
    ///wskaźnik do renderera grafiki
    SDL_Renderer *sdl_ren;

    GameEngine* game_engine;

    Language* lang;
    /**
     * inicjalizacja SDL
     * @return true - jeśli wszystko przebiegło pomyślnie
     */
    bool sdl_init();
    /**
     * wyświetla komunikat o błędzie grafiki
     * @param e komunikat
     */
    void sdl_error(string e);
    //  FPS
    /// liczba przeliczonych klatek
    int fps_frames;
    /// czas wystąpienia ostatniej klatki
    long long last_frame;
    /// wyznaczenie wartości FPS
    void calculate_fps();

    //  OKNO
public:
    /// liczba klatek na sekundę
    double fps;
    /// przełączenie w tryb pełnoekranowy lub wyjście z tego trybu
    void fullscreen_toggle();
    ///odrysowanie całej klatki
    void draw();
private:
    ///szerokość okna
    int screen_w;
    ///wysokość okna
    int screen_h;
    /// czy pełny ekran jest włączony
    bool fullscreen;
    ///narysowanie ekranu menu
    void draw_menu();
    ///narysowanie ekranu rundy
    void draw_round();
    ///narysowanie konsoli i wiersza poleceń na ekranie
    void draw_console();
    ///wyznacz nowe parametry animacji
    void animate();
    /**
     * ustaw ikonę okna
     * @param filename nazwa pliku z ikoną
     */
    void set_icon(string filename);

    //  TEKSTURY - WCZYTYWANIE
public:
    /**
     * znajdź teksturę po nazwie
     * @param name nazwa tekstury
     * @return wskaźnik na znalezioną teksturę lub NULL jeśli nie znaleziono
     */
    SDL_Texture* tex(string name);
    /// wczytanie wszystkich tekstur
    void reload_textures();
    /// zwolnienie pamięcie po teksturze
    void destroy_texture(SDL_Texture *&texture);
private:
    ///lista tekstur
    vector<SDL_Texture*> textures;
    ///lista nazw tekstur
    vector<string> textures_names;
    /**
     * załadowanie powierzchni
     * @param filename nazwa pliku
     * @return wskaźnik na obiekt powierzchni
     */
    SDL_Surface* load_surface(string filename);
    /**
     * załadowanie tekstury
     * @param filename nazwa pliku
     * @return wskaźnik na obiekt tekstury
     */
    SDL_Texture* load_texture(string filename);
    /**
     * dodanie tekstury do listy tekstur
     * @param texture wskaźnik na teksturę
     * @param name nazwa nowej tekstury
     */
    void add_texture(SDL_Texture *texture, string name);

    //  TEKSTURY - RYSOWANIE
public:
    /**
     * narysowanie obrazu wyśrodkowanego w poziomie i w pionie
     * @param image wskaźnik na teksturę
     * @param x współrzędna x środka obrazu na ekranie
     * @param y współrzędna y środka obrazu na ekranie
     */
    void draw_texture_center(SDL_Texture *image, int x, int y);
    /**
     * narysowanie wyciętego fragmentu obrazu i wyśrodkowanie w poziomie i w pionie
     * @param image wskaźnik na teksturę
     * @param x współrzędna x środka obrazu na ekranie
     * @param y współrzędna y środka obrazu na ekranie
     * @param clipx_i indeks klatki po osi x
     * @param clipx_n liczba klatek w osi x
     * @param clipy_i indeks klatki po osi y
     * @param clipy_n liczba klatek w osi y
     */
    void draw_texture_clip_center(SDL_Texture *image, int x, int y, int clipx_i, int clipx_n, int clipy_i, int clipy_n);
private:
    /**
     * zwraca szerokośc obrazka
     * @param image wskaźnik na teksturę
     * @return szerokośc obrazka
     */
    int get_w(SDL_Texture *image);
    /**
     * zwraca wysokość obrazka
     * @param image wskaźnik na teksturę
     * @return wysokość obrazka
     */
    int get_h(SDL_Texture *image);
    /**
     * zwraca szerokość i wysokość obrazka
     * @param image wskaźnik na teksturę
     * @param w zwrócona szerokośc obrazka
     * @param h zwrócona wysokość obrazka
     */
    void get_wh(SDL_Texture *image, int &w, int &h);
    /**
     * rysuj teksturę na ekranie
     * @param image wskaźnik na teksturę
     * @param x współrzędna x obrazu na ekranie
     * @param y współrzędna y obrazu na ekranie
     */
    void draw_texture(SDL_Texture *image, int x, int y);
    /**
     * narysowanie wyciętego fragmentu obrazu
     * @param image wskaźnik na teksturę
     * @param x współrzędna x obrazu na ekranie
     * @param y współrzędna y obrazu na ekranie
     * @param clipx_i indeks klatki po osi x
     * @param clipx_n liczba klatek w osi x
     * @param clipy_i indeks klatki po osi y
     * @param clipy_n liczba klatek w osi y
     */
    void draw_texture_clip(SDL_Texture *image, int x, int y, int clipx_i, int clipx_n, int clipy_i, int clipy_n);

    //  TEKSTURY - BLENDING
public:
    /**
     * wygenerowanie nowej, kolorowej tekstury na podstawie innej tekstury
     * @param texture tekstura bazowa, do przekształceń
     * @param color_blend kolor nowej tekstury
     * @return wskaźnik na nową teksturę
     */
    SDL_Texture* blend_texture(SDL_Texture* texture, SDL_Color color_blend);

    //  KOLORY
public:
    /**
     * zwraca strukturę koloru RGB (lub RGB z kanałem przezroczystości)
     * @param r wartość koloru czerwonego
     * @param g wartość koloru zielonego
     * @param b wartość koloru niebieskiego
     * @param a wartość kanału alpha (domyślnie - nieprzezroczyste)
     * @return struktura koloru
     */
    static SDL_Color rgba(int r, int g, int b, int a=255);
    /**
     * generuje losowy kolor dla gracza (omija ciemne kolory)
     * @return struktura koloru
     */
    static SDL_Color rand_bcolor();
private:
    ///aktualny kolor do rysowania w silniku grafiki
    SDL_Color ccolor;

    //  KSZTAŁTY
public:
    /**
     * rysuje pełny prostokąt
     * @param x położenie x na ekranie lewego wierzchołka
     * @param y położenie y na ekranie górnego wierzchołka
     * @param w szerokość prostokąta
     * @param h wysokość prostokąta
     * @param color kolor prostokąta
     */
    void draw_fill_rect(int x, int y, int w, int h, SDL_Color color);
    /**
     * rysuje linie między dwoma punktami na ekranie
     * @param x1 położenie x punktu 1
     * @param y1 położenie y punktu 1
     * @param x2 położenie x punktu 2
     * @param y2 położenie y punktu 2
     * @param color kolor linii
     */
    void draw_line(int x1, int y1, int x2, int y2, SDL_Color color);
private:

    //  TEKST, CZCIONKI
public:
    /// czcionka nr 1
    TTF_Font *font1;
    /// czcionka nr 2
    TTF_Font *font2;
    /**
     * rysuje tekst na ekranie i wyrównuje go
     * @param txt tekst do wyświetlenia
     * @param font czcionka
     * @param color kolor tekstu
     * @param x położenie x punktu odniesienia
     * @param y położenie y punktu odniesienia
     * @param align TEXT_LEFT - wyrównanie do lewej, punkt odniesienia = lewy górny róg tekstu, TEXT_RIGHT - wyrównanie do prawej, punkt odniesienia = prawy górny róg tekstu, TEXT_CENTER - wyrównanie do środka, punkt odniesienia = środkowy górny punkt tekstu
     */
    void draw_text(string txt, TTF_Font *font, SDL_Color color, int x, int y, int align=TEXT_LEFT);
private:
    /**
     * wczytuje czcionkę z pliku
     * @param filename nazwa pliku
     * @param fontsize rozmiar czcionki
     * @return wskaźnik na czcionkę
     */
    TTF_Font* load_ttf_font(string filename, int fontsize);

    //  ANIMACJE
public:
    /// lista wskaźników aktywnych animacji
    vector<Animation*> animations;
};

#endif
