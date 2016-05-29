#ifndef DEATH_ANIMATION_H
#define DEATH_ANIMATION_H

#include "animation.h"

/**
 * \brief Animacja śmierci gracza (Pacmana lub Duszka)
 */
class DeathAnimation : public Animation {
public:
    /**
     * Utworzenie nowej animacji
     * \param x położenie wzdłuż osi x na ekranie wyrażone w pikselach
     * \param y położenie wzdłuż osi y na ekranie wyrażone w pikselach
     * \param texture tekstura będąca podstawą do animacji
     * \param color kolor animacji
     * \param clip tablica określająca parametry do wycięcia animacji
     */
    DeathAnimation(int x, int y, SDL_Texture *texture, SDL_Color color, int *clip);
    ~DeathAnimation();
private:
    /// kolor animacji
    SDL_Color color;
    /// wygenerowanie parametrów kolejnej klatki animacji
    void animate();
    /// narysowanie animacji na ekranie
    void draw();
};

#endif
