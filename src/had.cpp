#include <alsa/pcm.h>
#include <cstddef>
#include <cstdlib>
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
#include <pthread.h>



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
    // static char* buff;
    // static size_t buff_size;
    // static snd_pcm_uframes_t frames;
    // static unsigned int rate, channels, duration;
    // // static int loops;
    // static int fd;
    // static bool is_playing = true;

    res aud_start() {
        int err = snd_pcm_open(
            &pcm_handle,
            "default",
            SND_PCM_STREAM_PLAYBACK,
            SND_PCM_NONBLOCK
        );

        if (err < 0) {
            log_err("Cannot open PCM device: %s", snd_strerror(err));
            return res::error;
        }
        
        return res::success;
    }

    res aud_end() {
        int err = snd_pcm_drop(pcm_handle);
        if (err < 0) {
            log_err("Cannot drom frames: %s", snd_strerror(err));
        }
        err = snd_pcm_close(pcm_handle);
        if (err < 0) {
            log_err("Cannot close pcm: %s", snd_strerror(err));
        }

        return res::success;
    }

    res aud_load(const char *path) {
        int err = 0;

        snd_pcm_hw_params_t *params;
        snd_pcm_hw_params_alloca(&params);
	    snd_pcm_hw_params_any(pcm_handle, params);

        if (snd_pcm_hw_params_set_access(
                pcm_handle, params,
                SND_PCM_ACCESS_RW_INTERLEAVED
            ) < 0
        ) log_err("Cannot set interleaved mode. %s\n", snd_strerror(err));
        if (snd_pcm_hw_params_set_format(
                pcm_handle,
                params,
                SND_PCM_FORMAT_S16_LE
            ) < 0
        ) log_err("Cannot set format. %s\n", snd_strerror(pcm));


        //     
        // if ((pcm = snd_pcm_hw_params_set_channels(pcm_handle, params, channels)) < 0) 
        //     printf("ERROR: Can't set channels number. %s\n", snd_strerror(pcm));
        // if ((pcm = snd_pcm_hw_params_set_rate_near(pcm_handle, params, &rate, 0)) < 0) 
        //     printf("ERROR: Can't set rate. %s\n", snd_strerror(pcm));
        // /* Write parameters */
        // if ((pcm = snd_pcm_hw_params(pcm_handle, params)) < 0)
        //     printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(pcm));

        return res::success;
    }

    res player_update() {
        int pcm;
        for (int loops = (duration * 1000000) / rate; loops > 0; loops--) {
        // if (loops <= 0) {
        //     return res::error;
        // }
        // if (!is_playing) {
        //     return res::success;    
        // }

            if ((pcm = read(fd, buff, buff_size)) == 0) {
                printf("buff %p\n", buff);
                printf("buff_size %lu\n", buff_size);
                printf("fd %d\n", fd);
                printf("Early end of file.\n");
                // return res::error;
                return res::error;
            }

            if ((pcm = snd_pcm_writei(pcm_handle, buff, frames)) == -EPIPE) {
                printf("XRUN.\n");
                snd_pcm_prepare(pcm_handle);
            } else if (pcm < 0) {
                printf("ERROR. Can't write to PCM device. %s\n", snd_strerror(pcm));
            }
            
            // std::cout << "loop" << std::endl;
            // loops--;

        }
        return res::success;
    }

    res play_stop_audio() {
        snd_pcm_pause(pcm_handle, is_playing);
        is_playing = !is_playing;
        return res::success;
    }

    res load(char const* path) {
        struct stat statbuf;

        unsigned int pcm, tmp, dir;

        if (stat(path, &statbuf)) {
            log_err("Cannot read file stat");
            return res::error;
        }

        size_t size = static_cast<size_t>(statbuf.st_size);
        fd = open(path, O_RDONLY);
        if (fd == -1) {
            log_err("Cannot open file");
            return res::error;
        }

        snd_pcm_hw_params_t *params;
        snd_pcm_hw_params_alloca(&params);
	    snd_pcm_hw_params_any(pcm_handle, params);

        if ((pcm = snd_pcm_hw_params_set_access(pcm_handle, params, SND_PCM_ACCESS_RW_INTERLEAVED)) < 0) 
		    printf("Cannot set interleaved mode. %s\n", snd_strerror(pcm));
        if ((pcm = snd_pcm_hw_params_set_format(pcm_handle, params, SND_PCM_FORMAT_S16_LE)) < 0)
            printf("ERROR: Can't set format. %s\n", snd_strerror(pcm));
        if ((pcm = snd_pcm_hw_params_set_channels(pcm_handle, params, channels)) < 0) 
            printf("ERROR: Can't set channels number. %s\n", snd_strerror(pcm));
        if ((pcm = snd_pcm_hw_params_set_rate_near(pcm_handle, params, &rate, 0)) < 0) 
            printf("ERROR: Can't set rate. %s\n", snd_strerror(pcm));
        /* Write parameters */
        if ((pcm = snd_pcm_hw_params(pcm_handle, params)) < 0)
            printf("ERROR: Can't set harware parameters. %s\n", snd_strerror(pcm));

        snd_pcm_hw_params_get_period_size(params, &frames, 0);

        buff_size = frames * channels * 2;
        std::cout << "frames: " << frames << std::endl;
        std::cout << "channels: " << channels << std::endl;
	    buff = (char*)malloc(buff_size);
        if (buff == nullptr) {
            log_err("Buffer is not allocated");
        }

        // loops = (duration * 1000000) / rate;
        // std::cout << "loops: " << loops << std::endl;
        
        return res::success;
    }

    res remove() {
        snd_pcm_drain(pcm_handle);
	    snd_pcm_close(pcm_handle);
        free(buff);
        close(fd);

        return res::success;
    }

}
