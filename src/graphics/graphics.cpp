#include <cmath>
#include <sstream>
#include "graphics.h"
#include "../config.h"
#include "../log/log.h"
#include "../app.h"
#include "../system.h"
#include "../utils.h"

//FIXME zjebany toggle trybu fullscreen, zależny od ekranu

Graphics::Graphics() {
    App::graphics = this;
    screen_w = Config::screen_w;
    screen_h = Config::screen_h;
    fullscreen = false;
    if (!sdl_init())
        return;
    font1 = load_ttf_font(Config::fontfile1, Config::fontsize1);
    font2 = load_ttf_font(Config::fontfile2, Config::fontsize2);
    Log::debug("Ładowanie tekstur...");
    for (unsigned int i = 0; Config::bitmap_names[2 * i].length() > 0; i++) {
        add_texture(load_texture(Config::bitmap_names[2 * i]), Config::bitmap_names[2 * i + 1]);
    }
    set_icon(Config::icon_name);
    fps = 0;
    fps_frames = 0;
    last_frame = currentMillis();
}

Graphics::~Graphics() {
    Log::info("Zamykanie silnika grafiki...");
    Log::debug("Usuwanie wszystkich tekstur");
    for (unsigned int i = 0; i < textures.size(); i++) {
        destroy_texture(textures[i]);
    }
    //usuwanie niezakończonych animacji
    for (unsigned int i = 0; i < animations.size(); i++) {
        delete animations[i];
    }
    if (font1 != nullptr) TTF_CloseFont(font1);
    if (font2 != nullptr) TTF_CloseFont(font2);
    if (sdl_ren != nullptr) SDL_DestroyRenderer(sdl_ren);
    if (sdl_win != nullptr) SDL_DestroyWindow(sdl_win);
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

void Graphics::setGameEngine(GameEngine* engine){
    this->game_engine = engine;
}

void Graphics::sdl_error(string e) {
    stringstream ss;
    ss << "Błąd SDL (SDL_Error: " << SDL_GetError() << "): " << e;
    Log::criticalError(ss.str());
    App::exit = true;
    delete this;
}

void Graphics::calculate_fps() {
    fps_frames++;
    if (fps_frames >= Config::fps_avg_frames) {
        long long time0 = currentMillis();
        if (time0 > last_frame)
            fps = ((double) Config::fps_avg_frames * 1000) / (time0 - last_frame);
        last_frame = time0;
        fps_frames = 0;
    }
}

bool Graphics::sdl_init() {
    sdl_win = nullptr;
    sdl_ren = nullptr;
    font1 = nullptr;
    Log::info("Start grafiki SDL...");
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        sdl_error("SDL_Init");
        return false;
    }
    stringstream ss;
    ss << Config::app_name << " v" << Config::version;
    sdl_win = SDL_CreateWindow(ss.str().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               Config::screen_w, Config::screen_h, SDL_WINDOW_SHOWN);
    if (sdl_win == nullptr) {
        sdl_error("SDL_CreateWindow");
        return false;
    }
    sdl_ren = SDL_CreateRenderer(sdl_win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (sdl_ren == nullptr) {
        sdl_error("SDL_CreateRenderer");
        return false;
    }
    SDL_SetRenderDrawBlendMode(sdl_ren, SDL_BLENDMODE_BLEND);
    Log::debug("Start SDL_TTF...");
    if (TTF_Init() != 0) {
        sdl_error("TTF_Init");
        return false;
    }
    Log::debug("Start SDL_Image...");
    int imgFlags = IMG_INIT_PNG;
    if (!(IMG_Init(imgFlags) & imgFlags)) {
        sdl_error("SDL_image Init");
        return false;
    }
    return true;
}

void Graphics::draw() {
    calculate_fps();
    animate();
    //wyczyszczenie ekranu
    SDL_SetRenderDrawColor(sdl_ren, 0, 0, 0, 255);
    SDL_RenderClear(sdl_ren);
    if (game_engine->menu > 0) { //MENU
        draw_menu();
    } else {  //GRA - RUNDA
        draw_round();
    }
    //FPSy
    if (Config::show_fps) {
        stringstream ss;
        ss << fps << " FPS";
        draw_text(ss.str(), font1, rgba(0, 255, 0), 0, screen_h - 15, TEXT_LEFT);
    }
    //wiersz poleceń i konsola
    if (game_engine->cmd_on) {
        draw_console();
    }
    //wyświetlenie na ekranie (double buffering)
    SDL_RenderPresent(sdl_ren);
}

void Graphics::fullscreen_toggle() {
    Log::debug("Przełączanie fullscreen...");
    fullscreen = !fullscreen;
    if (fullscreen) {
        SDL_SetWindowFullscreen(sdl_win, SDL_WINDOW_FULLSCREEN_DESKTOP);
    } else {
        SDL_SetWindowFullscreen(sdl_win, 0);
    }
    //przeskalowanie ekranu
    int window_w, window_h;
    SDL_Rect viewport;
    SDL_GetWindowSize(sdl_win, &window_w, &window_h);

    if(!fullscreen){
        window_w = Config::screen_w;
        window_h = Config::screen_h;
    }

    float scale = ((float) window_w) / Config::screen_w;
    viewport.w = window_w;
    viewport.h = window_w * Config::screen_h / Config::screen_w;
    if (scale * Config::screen_h > window_h) {
        scale = ((float) window_h) / Config::screen_h;
        viewport.w = window_h * Config::screen_w / Config::screen_h;
        viewport.h = window_h;
    }
    //wyśrodkowanie okna
    viewport.x = (window_w - viewport.w) / 2;
    viewport.y = (window_h - viewport.h) / 2;
    SDL_RenderSetViewport(sdl_ren, &viewport);
    SDL_RenderSetScale(App::graphics->sdl_ren, scale, scale);
    reload_textures();
}

SDL_Texture* Graphics::tex(string name) {
    for (unsigned int i = 0; i < textures.size(); i++) {
        if (textures_names[i] == name) {
            return textures[i];
        }
    }
    sdl_error("Nie znaleziono tekstury o nazwie: " + name);
    return nullptr;
}

SDL_Surface* Graphics::load_surface(string filename) {
    SDL_Surface* nowy_surface;
    if (has_extension(filename, ".bmp")) {
        nowy_surface = SDL_LoadBMP(filename.c_str());
        if (nowy_surface == nullptr) {
            sdl_error("SDL_LoadBMP: " + filename);
            return nullptr;
        }
    } else {
        nowy_surface = IMG_Load(filename.c_str());
        if (nowy_surface == nullptr) {
            sdl_error("IMG_Load: " + filename);
            return nullptr;
        }
    }
    return nowy_surface;
}

SDL_Texture* Graphics::load_texture(string filename) {
    SDL_Surface* nowy_surface = load_surface(filename);
    if (nowy_surface == nullptr) {
        sdl_error("load_texture: zerowy wskaźnik SDL_Surface");
        return nullptr;
    }
    SDL_Texture* nowy_tex = SDL_CreateTextureFromSurface(sdl_ren, nowy_surface);
    SDL_FreeSurface(nowy_surface);
    if (nowy_tex == 0) {
        sdl_error("load_texture: SDL_CreateTextureFromSurface");
        return nullptr;
    }
    return nowy_tex;
}

void Graphics::add_texture(SDL_Texture* texture, string name) {
    if (texture == nullptr) {
        sdl_error("add_texture: zerowy wskaźnik tekstury");
        return;
    }
    textures.push_back(texture);
    textures_names.push_back(name);
    Log::debug("Dodano teksturę: " + name);
}

void Graphics::destroy_texture(SDL_Texture*& texture) {
    if (texture == nullptr) {
        Log::error("Usuwanie tekstury o zerowym wskaźniku.");
        return;
    }
    SDL_DestroyTexture(texture);
    texture = nullptr;
}

void Graphics::reload_textures() {
    Log::debug("Przeładowanie tekstur...");
    for (unsigned int i = 0; i < game_engine->players.size(); i++) {
        game_engine->players[i]->reload_texture();
    }
    if (game_engine->menu_pacman != nullptr)
        game_engine->menu_pacman->reload_texture();
    if (game_engine->menu_ghost != nullptr)
        game_engine->menu_ghost->reload_texture();
}


void Graphics::get_wh(SDL_Texture* image, int& w, int& h) {
    if (image == nullptr) {
        w = 0;
        h = 0;
        return;
    }
    SDL_QueryTexture(image, nullptr, nullptr, &w, &h);
}

int Graphics::get_w(SDL_Texture* image) {
    if (image == nullptr)
        return 0;
    int w, h;
    get_wh(image, w, h);
    return w;
}

int Graphics::get_h(SDL_Texture* image) {
    if (image == nullptr)
        return 0;
    int w, h;
    get_wh(image, w, h);
    return h;
}

void Graphics::set_icon(string filename) {
    SDL_Surface* nowy_surface = load_surface(filename);
    if (nowy_surface == nullptr) {
        sdl_error("set_icon: zerowy wskaźnik SDL_Surface");
        return;
    }
    SDL_SetWindowIcon(sdl_win, nowy_surface);
    SDL_FreeSurface(nowy_surface);
}


SDL_Texture* Graphics::blend_texture(SDL_Texture* texture, SDL_Color color_blend) {
    if (texture == nullptr) {
        sdl_error("blend_texture: zerowy wskaźnik tekstury");
        return nullptr;
    }
    int w, h;
    get_wh(texture, w, h);
    SDL_Texture* new_texture = SDL_CreateTexture(sdl_ren, SDL_PIXELFORMAT_RGBA8888,
                                                 SDL_TEXTUREACCESS_TARGET, w, h);
    if (new_texture == nullptr) {
        sdl_error("blend_texture: blad tworzenia nowej tekstury");
        return nullptr;
    }
    SDL_SetRenderTarget(sdl_ren, new_texture);
    //przezroczysta tekstura
    SDL_SetTextureBlendMode(new_texture, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(sdl_ren, 0, 0, 0, 0);
    SDL_RenderClear(sdl_ren);
    //zapamiętanie poprzedniego trybu przenikania
    SDL_BlendMode blend_mode;
    if (SDL_GetRenderDrawBlendMode(sdl_ren, &blend_mode) != 0) {
        sdl_error("blend_texture: SDL_GetRenderDrawBlendMode");
        return nullptr;
    }
    SDL_SetRenderDrawBlendMode(sdl_ren, SDL_BLENDMODE_MOD);
    //obrazek pierwotny
    if (SDL_RenderCopy(sdl_ren, texture, nullptr, nullptr) != 0) {
        sdl_error("blend_texture: SDL_RenderCopy");
        return nullptr;
    }
    //nałożenie półprzezroczystej maski
    SDL_Rect dest;
    dest.x = 0;
    dest.y = 0;
    dest.w = w;
    dest.h = h;
    SDL_SetRenderDrawColor(sdl_ren, color_blend.r, color_blend.g, color_blend.b, 255);
    SDL_RenderFillRect(sdl_ren, &dest);
    //przywrócenie rysowania na ekranie i trybu przenikania
    SDL_SetRenderDrawBlendMode(sdl_ren, blend_mode);
    SDL_SetRenderTarget(sdl_ren, nullptr);
    return new_texture;
}

void Graphics::draw_texture(SDL_Texture* image, int x, int y) {
    if (image == nullptr) {
        sdl_error("draw_texture: image null pointer");
        return;
    }
    SDL_Rect src;
    src.x = 0;
    src.y = 0;
    get_wh(image, src.w, src.h);
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = src.w;
    dest.h = src.h;
    if (SDL_RenderCopy(sdl_ren, image, &src, &dest) != 0) {
        sdl_error("draw_texture: SDL_RenderCopy");
    }
}

void Graphics::draw_texture_center(SDL_Texture* image, int x, int y) {
    if (image == nullptr) {
        sdl_error("draw_texture_center: image null pointer");
        return;
    }
    int w, h;
    get_wh(image, w, h);
    draw_texture(image, x - w / 2, y - h / 2);
}

void Graphics::draw_texture_clip(SDL_Texture* image, int x, int y, int clipx_i, int clipx_n,
                                 int clipy_i, int clipy_n) {
    if (image == nullptr) {
        sdl_error("draw_texture_clip: image null pointer");
        return;
    }
    if (clipx_n == 0) clipx_n = 1;
    if (clipy_n == 0) clipy_n = 1;
    //rozmiary przyciętego obrazka
    int w, h;
    get_wh(image, w, h);
    int clip_w = w / clipx_n;
    int clip_h = h / clipy_n;
    SDL_Rect src, dest;
    src.x = clipx_i * clip_w;
    src.y = clipy_i * clip_h;
    src.w = clip_w;
    src.h = clip_h;
    dest.x = x;
    dest.y = y;
    dest.w = clip_w;
    dest.h = clip_h;
    if (SDL_RenderCopy(sdl_ren, image, &src, &dest) != 0) {
        sdl_error("draw_texture_clip: SDL_RenderCopy");
    }
}

void Graphics::draw_texture_clip_center(SDL_Texture* image, int x, int y, int clipx_i, int clipx_n,
                                        int clipy_i, int clipy_n) {
    if (image == nullptr) {
        sdl_error("draw_texture_clip_center: image null pointer");
        return;
    }
    if (clipx_n == 0) clipx_n = 1;
    if (clipy_n == 0) clipy_n = 1;
    //rozmiary przyciętego obrazka
    int w, h;
    get_wh(image, w, h);
    int clip_w = w / clipx_n;
    int clip_h = h / clipy_n;
    SDL_Rect src, dest;
    src.x = clipx_i * clip_w;
    src.y = clipy_i * clip_h;
    src.w = clip_w;
    src.h = clip_h;
    dest.x = x - clip_w / 2;
    dest.y = y - clip_h / 2;
    dest.w = clip_w;
    dest.h = clip_h;
    if (SDL_RenderCopy(sdl_ren, image, &src, &dest) != 0) {
        sdl_error("draw_texture_clip: SDL_RenderCopy");
    }
}

TTF_Font* Graphics::load_ttf_font(string filename, int fontsize) {
    TTF_Font* font = TTF_OpenFont(filename.c_str(), fontsize);
    if (font == nullptr) {
        sdl_error("load_ttf_font: TTF_OpenFont");
        return nullptr;
    }
    return font;
}

void Graphics::draw_text(string txt, TTF_Font* font, SDL_Color color, int x, int y, int align) {
    if (font == nullptr) {
        sdl_error("draw_text: font null pointer");
        return;
    }
    if (txt.length() == 0)
        return;
    //SDL_Surface *surf = TTF_RenderText_Blended(font, txt.c_str(), color);
    SDL_Surface* surf = TTF_RenderUTF8_Blended(font, txt.c_str(), color);
    if (surf == nullptr) {
        sdl_error("draw_text: TTF_RenderText_Blended");
        return;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(sdl_ren, surf);
    SDL_SetTextureAlphaMod(texture, color.a); //kanał alpha
    SDL_FreeSurface(surf);
    if (texture == nullptr) {
        sdl_error("draw_text: SDL_CreateTextureFromSurface");
        return;
    }
    int x_pos = x;
    int w, h;
    get_wh(texture, w, h);
    if (align == TEXT_CENTER) {
        x_pos -= w / 2;
    } else if (align == TEXT_RIGHT) {
        x_pos -= w;
    }
    draw_texture(texture, x_pos, y);
    SDL_DestroyTexture(texture);
}


SDL_Color Graphics::rgba(int r, int g, int b, int a) {
    SDL_Color color;
    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;
    return color;
}

SDL_Color Graphics::rand_bcolor() {
    int prog = 90;
    int r, g, b;
    do {
        r = rand() % 256;
        g = rand() % 256;
        b = rand() % 256;
    } while (r < prog && g < prog && b < prog);
    return rgba(r, g, b);
}


void Graphics::draw_fill_rect(int x, int y, int w, int h, SDL_Color color) {
    SDL_Rect dest;
    dest.x = x;
    dest.y = y;
    dest.w = w;
    dest.h = h;
    SDL_SetRenderDrawColor(sdl_ren, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(sdl_ren, &dest);
}

void Graphics::draw_line(int x1, int y1, int x2, int y2, SDL_Color color) {
    SDL_SetRenderDrawColor(sdl_ren, color.r, color.g, color.b, color.a);
    SDL_RenderDrawLine(sdl_ren, x1, y1, x2, y2);
}
