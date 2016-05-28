/**
 * \file animation.h
 * Moduł obsługi animacji w grze
 * \see animation.cpp
 */
#ifndef ANIMATION_H
#define ANIMATION_H

#include "graphics.h"

/**
 * \brief Klasa rysująca animacje i generująca kolejne klatki animacji
 */
class Animation{
public:
    /**
     * Utworzenie nowej animacji w określonym położeniu
     * \param x położenie wzdłuż osi x na ekranie wyrażone w pikselach
     * \param y położenie wzdłuż osi y na ekranie wyrażone w pikselach
     */
    Animation(int x, int y);
    virtual ~Animation();
    /// Podstawowa tekstura wyświetlana jako animacja
    SDL_Texture *texture;
    /// tablica 4 liczb całkowitych określająca parametry wycięcia fragmentu obrazka z tekstury
    int *clip;
    /// położenie animacji na ekranie wzdłuż osi x
    int x;
    /// położenie animacji na ekranie wzdłuż osi y
    int y;
    /// liczba klatek do zakończenia animacji
    int ends;
    /**
     * Wygenerowanie parametrów kolejnej klatki animacji
     */
    virtual void animate() = 0;
    /**
     * Narysowanie animacji na ekranie
     */
    virtual void draw() = 0;
};

//TODO utworzyć nowy plik dla klasy DeathAnimation
/**
 * \brief Animacja śmierci gracza (Pacmana lub Duszka)
 */
class AnimationDeath : public Animation {
public:
    /**
     * Utworzenie nowej animacji
     * \param x położenie wzdłuż osi x na ekranie wyrażone w pikselach
     * \param y położenie wzdłuż osi y na ekranie wyrażone w pikselach
     * \param texture tekstura będąca podstawą do animacji
     * \param color kolor animacji
     * \param clip tablica określająca parametry do wycięcia animacji
     */
    AnimationDeath(int x, int y, SDL_Texture *texture, SDL_Color color, int *clip);
    ~AnimationDeath();
private:
    /// kolor animacji
    SDL_Color color;
    /// wygenerowanie parametrów kolejnej klatki animacji
    void animate();
    /// narysowanie animacji na ekranie
    void draw();
};

#endif
