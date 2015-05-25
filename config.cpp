#include "config.h"
#include "version.h"

const string Config::log_filename = "log.txt";

int Config::screen_w = 800;
int Config::screen_h = 480;
const char* Config::app_name = "PacLAN";
string Config::version = VERSION;
bool Config::fullscreen_start = false;

int Config::logic_timer_ms = 40;
int Config::logic_cycles_critical = 60;
int Config::fps_avg_frames = 10;

string Config::bitmap_names[] = {
    "gfx\\pacman.png", "pacman",
    "gfx\\ghost.png", "ghost",
    "gfx\\ghost_eatme.png", "ghost_eatme",
    "gfx\\dot1.png", "dot1",
    "gfx\\dot2.png", "dot2",
    "maps\\1\\background.png", "map1",
    ""
};

string Config::icon_name = "gfx\\icon.png";

string Config::fontfile1 = "fonts\\arialbd.ttf";
int Config::fontsize1 = 14;
string Config::fontfile2 = "fonts\\prstartk.ttf";
int Config::fontsize2 = 12;

bool Config::show_fps = false;
bool Config::show_grid = false;
bool Config::show_paths = false;
bool Config::show_names = true;
int Config::cmd_outputs = 20;

int Config::pacman_speed = 4;
int Config::ghost_speed = 4;
int Config::ghost_eatme_speed = 6;
bool Config::move_while_respawn = true;

int Config::ai_pacman_start_escape = 4;
int Config::ai_pacman_stop_escape = 6;

int Config::eating_time = 80;
int Config::eating_time_critical = 25;
int Config::respawn_time = 120;
int Config::next_round_time = 75;

int Config::points_kill_pacman = 50;
int Config::points_kill_ghost = 15;

int Config::zombie_death_time = 20;
int Config::zombie_dots_threshold = 25;
int Config::zombie_dots_add = 20;
int Config::zombie_points_small = 1;
int Config::zombie_points_large = 2;
int Config::zombie_points_kill_pacman = 50;
int Config::zombie_points_kill_ghost = 15;
int Config::zombie_eating_time = 65;
int Config::zombie_pacman_speed = 3;
int Config::zombie_ghost_speed = 4;
int Config::zombie_ghost_eatme_speed = 5;

int Config::network_refresh = 5;
int Config::max_clients = 32;
int Config::port = 4000;
int Config::max_attempts = 5;
int Config::wait_if_failed = 100;
int Config::buffer_size = 10240;

