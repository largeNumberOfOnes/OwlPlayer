#include <cstddef>
#include <cstdlib>
#include "audioFile.h"
#include "glob_types.h"
#include "had.h"
#include "ncurses.h"
#include <cmath>
#include <curses.h>
#include <fcntl.h>
#include <iterator>

#include <alsa/asoundlib.h>
#include <stdio.h>

#include <sys/stat.h>
// #include <sys/mman.h>
// #include <pthread.h>
#include <thread>



namespace had::Colors {
    Color const black = COLOR_BLACK;
    Color const red = COLOR_RED;
    Color const green = COLOR_GREEN;
    Color const yellow = COLOR_YELLOW;
    Color const blue = COLOR_BLUE;
    Color const magenta = COLOR_MAGENTA;
    Color const cyan = COLOR_CYAN;
    Color const white = COLOR_WHITE;
}

res had::start_window() {
    initscr();
    start_color();
    noecho();
    // halfdelay(1);
    timeout(0);
    cbreak();
    curs_set(0);
    keypad(stdscr, true);
    return res::success;
}

res had::end_window() {
    endwin();
    return res::success;
}

had::key had::get_key() {
    int a = getch();
    switch (a) {
        case KEY_LEFT:  return key::arrow_left;
        case KEY_RIGHT: return key::arrow_rigth;
        case KEY_UP:    return key::arrow_up;
        case KEY_DOWN:  return key::arrow_down;
        case ' ':       return key::space;
        case 'q':       return key::q;
        default:        return key::nothing;
    }

}

void had::set_color() {
    // pass
}

void had::move() {
    // pass
}

res had::draw_slider(dem x, dem y, dem len, dem val) {
    std::string slider(len, '-');
    slider[val] = '0';
    mvprintw(y, x, "%s", slider.c_str());
    return res::success;
}

res had::draw_text(dem x, dem y, char const* str){
    if (mvprintw(y, x, "%s", str) == OK)
        return res::success;
    else
        return res::error;
}

res had::draw_symbol(dem x, dem y, char ch) {
    if (mvaddch(y, x, ch) == OK)
        return res::success;
    else
        return res::error;
}

res had::draw_wide_symbol(dem x, dem y, wchar_t ch) {
    cchar_t cch = {0, ch};
    if (mvadd_wch(y, x, &cch) == OK)
        return res::success;
    else
        return res::error;
}

res had::update() {
    if (refresh() == OK)
        return res::success;
    else
        return res::error;
}

res had::cls() {
    if (clear() == OK)
        return res::success;
    else
        return res::error;
}

had::dem had::get_width() {
    int w = 0, h = 0;
    getmaxyx(stdscr, h, w);
    return w;
}

had::dem had::get_height() {
    int w = 0, h = 0;
    getmaxyx(stdscr, h, w);
    return h;
}

namespace had { // Color support
    static unsigned int color_count = 10;
    static unsigned int pair_count = 1;

    res set_new_color(Color& col, int r, int g, int b) {
        if (color_count >= COLORS) {
            log_err("Too many colors");
            return res::error;
        }
        int ret = init_color(
            color_count, r*1000/256, g*1000/256, b*1000/256
        );
        if (ret == OK) {
            col = color_count++;
            return res::success;
        } else {
            col = 0;
            return res::error;
        }
    }

    res change_color(Color col, int r, int g, int b) {
        int ret = init_color(
            col, r*1000/256, g*1000/256, b*1000/256
        );
        if (ret == OK)
            return res::success;
        else
            return res::error;
    }

    res set_new_color_pair(ColorPair& pair, Color t, ColorPair b) {
        if (pair_count >= COLOR_PAIRS) {
            log_err("Too many color pairs");
            return res::error;
        }
        if (init_pair(pair_count, t, b) == OK) {
            pair = pair_count++;
            return res::success;
        } else {
            pair = 0;
            return res::error;
        }
    }

    res change_color_pair(ColorPair pair, Color t, ColorPair b) {
        if (init_pair(pair, t, b) == OK)
            return res::success;
        else
            return res::error;
    }

    res use_color_pair(ColorPair pair) {
        if (attron(COLOR_PAIR(pair)) == OK)
            return res::success;
        else
            return res::error;
    }

    res use_no_color_pair() {
        if (attroff(~0) == OK)
            return res::success;
        else
            return res::error;
    }
}

namespace had { // Audio support
    static snd_pcm_t *pcm_handle;
    static AudioFile audioFile{};
    static snd_pcm_uframes_t frames;
    static bool is_playing = true;
    static volatile bool stop_player_update = false;
    static volatile volume vol = 100;

    res aud_set_volume(volume new_vol) {
        vol = new_vol;
        return res::success;
    }
    volume aud_get_volume() {
        return vol;
    }

    res aud_start() {
        int err = snd_pcm_open(
            &pcm_handle,
            "default",
            SND_PCM_STREAM_PLAYBACK,
            SND_PCM_ASYNC
        );

        if (err < 0) {
            log_err("Cannot open PCM device: %s", snd_strerror(err));
            return res::error;
        }
        
        return res::success;
    }

    res aud_end() {
        aud_drop();
        if (!pcm_handle) {
            return res::error;
        }
        int err = snd_pcm_close(pcm_handle);
        if (err < 0) {
            log_err("Cannot close pcm: %s", snd_strerror(err));
            return res::error;
        }

        return res::success;
    }

    res aud_drop() {
        int err = snd_pcm_drop(pcm_handle);
        if (err < 0) {
            log_err("Cannot drom frames: %s", snd_strerror(err));
            return res::error;
        }
        stop_player_update = true;
        return res::success;
    }

    static res set_params() {
        if (snd_pcm_hw_free(pcm_handle) < 0) {
            log_err("Cannot reset pcm congiguration");
            return res::error;
        };
        snd_pcm_hw_params_t *params;
        snd_pcm_hw_params_alloca(&params);
	    snd_pcm_hw_params_any(pcm_handle, params);
        unsigned int rate = audioFile.get_rate();

        int err;
        if (false
            || (err = snd_pcm_hw_params_set_access(
                    pcm_handle, params,
                    SND_PCM_ACCESS_RW_INTERLEAVED
                ))
            || (err = snd_pcm_hw_params_set_format(
                    pcm_handle,
                    params,
                    SND_PCM_FORMAT_S16_LE
                ))
            || (err = snd_pcm_hw_params_set_channels(
                    pcm_handle,
                    params,
                    audioFile.get_channels()
                ))
            || (err = snd_pcm_hw_params_set_rate_near(
                    pcm_handle,
                    params,
                    &rate,
                    0
                ))
            || (err = snd_pcm_hw_params(
                    pcm_handle,
                    params
                ))
        ) {
            log_err(
                "Cannot set harware parameters. %s",
                snd_strerror(err)
            );
            return res::error;
        }

        if ((err = snd_pcm_hw_params_get_period_size(
                params,
                &frames,
                0
            )) < 0
        ) {
            log_err(
                "The configuration space does not contain a single"
                                                            "value: %s",
                snd_strerror(err)
            );
            return res::error;
        }

        return res::success;
    }

    res aud_load(const char *path) {
        int err = 0;

        if (audioFile.init(path)) {
            log_err("Cannot init audioFile");
            return res::error;
        }
        if (set_params()) {
            log_err("Cannot set params");
            audioFile.dstr();
            return res::error;
        }

        return res::success;
    }

    void player_update() {
        log_step("Start player_update()");
        size_t buff_size = frames*audioFile.get_channels()*2;
        char* buff = (char*) malloc(buff_size); // DEV (molloc -> new)
        int q = 0;
        while (true) {
            size_t retcount = 0;
            res err = audioFile.read_file(buff, buff_size, retcount);
            if (err) {
                log_err("Cannot read audioFile");
            }
            if (stop_player_update) {
                stop_player_update = !stop_player_update;
                break;
            }
            if (retcount == 0) {
                break;
            }

            int pret = snd_pcm_writei(pcm_handle, buff, frames);
            std::cout << "wri" << std::endl;
            if (pret == -EBADFD) {
                log_err(
                    "Cannot write to PCM device. %s",
                    snd_strerror(pret)
                );
            }

            if (pret == -EPIPE) {
                snd_pcm_prepare(pcm_handle);
            }
        }

        free(buff);
        log_step("End player_update()");
    }

    res aud_run() {
        std::thread th(player_update);
        th.detach();
        return res::success;
    }

    res aud_play_stop() {
        snd_pcm_pause(pcm_handle, is_playing);
        is_playing = !is_playing;
        return res::success;
    }

}
