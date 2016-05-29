#include "pathfind.h"
#include "../log.h"
#include <cstdlib>

//TODO template na typ współrzędnej

Pathfind::Pathfind(){
    start_x = 0;
    start_y = 0;
    end_x = 0;
    end_y = 0;
    map_w = 0;
    map_h = 0;
    map = NULL;
}

Pathfind::~Pathfind(){
    if(map!=NULL){
        for(int w=0; w<map_h; w++)
            delete[] map[w];
        delete[] map;
    }
}

Path* Pathfind::find_path(){
    //  ALGORYTM A-STAR
    //zmienne pomocnicze
    vector<Node*> o_list; //lista otwartych
    vector<Node*> c_list; //lista zamkniętych
    Node *Q; //aktualne pole (o minimalnym F)
    Node *sasiad;
    int min_f_i; //indeks minium f na liście otwartych
    int s_x, s_y; //położenie sąsiada
    int nowe_g;
    //Dodajemy pole startowe (lub węzeł) do Listy Otwartych.
    o_list.push_back(new Node(start_x, start_y));
    //dopóki lista otwartych nie jest pusta
    while(o_list.size()>0){
        //Szukamy pola o najniższej wartości F na Liście Otwartych. Czynimy je aktualnym polem - Q.
        min_f_i = 0; //indeks minimum
        for(unsigned int i=1; i<o_list.size(); i++){
            if(o_list.at(i)->f < o_list.at(min_f_i)->f)
                min_f_i = i; //nowe minimum
        }
        Q = o_list.at(min_f_i);
        //jeśli Q jest węzłem docelowym
        if(Q->x==end_x && Q->y==end_y){
            //znaleziono najkrótszą ścieżkę
            Path *sciezka;
            //jeśli punkt docelowy jest punktem startowym - brak ścieżki
            if(start_x==end_x && start_y==end_y){
                sciezka = NULL;
            }else{
                //Zapisujemy ścieżkę. Krocząc w kierunku od pola docelowego do startowego, przeskakujemy z kolejnych pól na im przypisane pola rodziców, aż do osiągnięcia pola startowego.
                sciezka = new Path();
                while(Q!=NULL){
                    int *point = new int [2];
                    point[0] = Q->x;
                    point[1] = Q->y;
                    sciezka->points.insert(sciezka->points.begin(), point); //dopisanie na początek (odwrócenie listy)
                    Q = Q->parent;
                }
            }
            //posprzątanie
            for(unsigned int i=0; i<o_list.size(); i++)
                delete o_list.at(i);
            for(unsigned int i=0; i<c_list.size(); i++)
                delete c_list.at(i);
            return sciezka;
        }
        //Aktualne pole przesuwamy do Listy Zamkniętych.
        o_list.erase(o_list.begin() + min_f_i);
        c_list.push_back(Q);
        //Dla każdego z wybranych przyległych pól (sasiad) do pola aktualnego (Q) sprawdzamy:
        for(int s=0; s<4; s++){ //dla każdego sąsiada
            if(s==0){
                s_x = Q->x-1;
                s_y = Q->y;
            }else if(s==1){
                s_x = Q->x+1;
                s_y = Q->y;
            }else if(s==2){
                s_x = Q->x;
                s_y = Q->y-1;
            }else{
                s_x = Q->x;
                s_y = Q->y+1;
            }
            //jeśli jest poza mapą
            if(s_x<0 || s_y<0 || s_x>=map_w || s_y>=map_h){
                //continue; //ignorujemy je
                //zawijanie
                if(s_x<0)
                    s_x = map_w-1;
                if(s_y<0)
                    s_y = map_h-1;
                if(s_x>=map_w)
                    s_x = 0;
                if(s_y>=map_h)
                    s_y = 0;
            }
            //jeśli NIE-MOŻNA go przejść, ignorujemy je.
            if(map[s_y][s_x]!=1)
                continue;
            //jeśli pole sąsiada jest już na Liście Zamkniętych
            if(find_in_list(c_list,s_x,s_y)!=NULL)
                continue;
            //Jeśli pole sąsiada nie jest jeszcze na Liście Otwartych.
            sasiad = find_in_list(o_list,s_x,s_y);
            if(sasiad==NULL){
                //dodajemy je do niej
                sasiad = new Node(s_x, s_y);
                o_list.push_back(sasiad);
                //Aktualne pole (Q) przypisujemy sasiadowi jako “pole rodzica”
                sasiad->parent = Q;
                //i zapisujemy sasiada wartości F, G i H. (F = G + H)
                sasiad->g = policz_g(sasiad);
                sasiad->f = sasiad->g + policz_h(sasiad);
            }else{
                //jeśli pole było na liście otwartych
                //sprawdzamy czy aktualna ścieżka do tego pola (sasiad) (prowadząca przez Q) jest krótsza, poprzez porównanie sasiada wartości G dla starej i aktualnej ścieżki. Mniejsza wartość G oznacza, że ścieżka jest krótsza.
                nowe_g = 1 + Q->g;
                if(nowe_g < sasiad->g){
                    //Jeśli tak, zmieniamy przypisanie "pole rodzica" na aktualne pole (Q) i przeliczamy wartości G i F dla pola (sasiad). Jeśli wasza Lista Otwartych jest posortowana pod kątem wartości F, trzeba ją ponownie przesortować po wprowadzonej zmianie.
                    sasiad->parent = Q;
                    sasiad->g = nowe_g;
                    sasiad->f = nowe_g + policz_h(sasiad);
                }
            }
        }
    }
    //posprzątanie
    for(unsigned int i=0; i<c_list.size(); i++)
        delete c_list.at(i);
    //Lista Otwartych jest pusta. nie znaleziono pola docelowego, a ścieżka nie istnieje.
    return NULL;
}

Node *Pathfind::find_in_list(vector<Node *> list, int x, int y){
    for(unsigned int i=0; i<list.size(); i++){
        if(list.at(i)->x==x && list.at(i)->y==y)
            return list.at(i);
    }
    return NULL;
}

int Pathfind::policz_g(Node *item){
    return 1 + item->parent->g;
}

int Pathfind::policz_h(Node *item){
    //metoda Manhattan - odległość w metryce miejskiej
    //return abs(item->x - end_x) + abs(item->y - end_y);
    //metryka miejska zmodyfikowana o zawiajanie mapy
    return (abs(item->x - end_x) + abs(item->y - end_y))/2;
}

void Pathfind::set_xy(int start_x, int start_y, int end_x, int end_y){
    this->start_x = start_x;
    this->start_y = start_y;
    this->end_x = end_x;
    this->end_y = end_y;
}


Node::Node(int x, int y){
    this->x = x;
    this->y = y;
    g = 0;
    f = 0;
    parent = NULL;
}


Path::Path(){

}

Path::~Path(){
    for(unsigned int i=0; i<points.size(); i++){
        delete[] points.at(i);
    }
}

int Path::length(){
    if(points.size()<=1)
        return 0;
    return points.size()-1;
}
