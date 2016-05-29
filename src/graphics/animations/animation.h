#ifndef ANIMATION_H
#define ANIMATION_H

#include "../graphics.h"

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

#endif
