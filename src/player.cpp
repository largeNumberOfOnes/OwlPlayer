#include "player.h"
#include "had/had.h"
#include "had/had_interface.h"
#include "had/had_types.h"

#include <cstring>
#include <string>
#include <string_view>



static had::Dem const vol_min_width = 12;
static had::Dem const vol_max_width = 20;
static had::Dem const tim_min_width = 26;
static had::Dem const tim_half_width = 40;
static had::Dem const vol_tim_pad = 3;
static had::Dem const min_width = vol_min_width + tim_min_width +
                                                        vol_tim_pad;
static had::Dem const min_height = 5;

static had::Res player_initializer_audio_res_code = had::Res::success;
Player::Player(had::Drawer& drawer, const had::Logger& log)
    : audio(player_initializer_audio_res_code, log)
    , drawer(drawer)
    , log(log)
{}

Player::Player(had::Drawer& drawer, CallOnPlayEnd call_on_play_end,
                                                    const had::Logger& log)
    : Player(drawer, log)
{
    set_on_play_end(call_on_play_end);
}

Player::~Player() {
    if (audio.is_playing()) {
        audio.stop();
    }
    audio.drop();
}

had::Res Player::calc_grid() {
    // From tim_min_width tim_w increases throw tim_half_width, while
    //      vol_w is constant.
    // When vol_w increases from vol_min_width throw vol_max_width. It is 
    //      vol_w maximum. tim_w is constant.
    // When only tim_w increases, vol_w is constant forever.

    had::Dem w = drawer.get_width();

    if (w < min_width) {
        log.log_err("Cannot calculate vol_tim: width is too small");
        return had::Res::error;
    }

    if (w < vol_min_width + vol_tim_pad + tim_half_width){
        grid.vol_w = vol_min_width;
        grid.tim_w = w - vol_min_width - vol_tim_pad;
    } else if (w < vol_max_width + vol_tim_pad + tim_half_width) {
        grid.vol_w = w - tim_half_width - vol_tim_pad;
        grid.tim_w = tim_half_width;
    } else {
        grid.vol_w = vol_max_width;
        grid.tim_w = w - vol_max_width - vol_tim_pad;
    }

    grid.tim_x = grid.vol_w + vol_tim_pad;
    grid.vol_y = 2;
    grid.tim_y = 2;

    return had::Res::success;
}

had::Res Player::draw_volume() {
    // vol     -----0--      77%
    // <--p1-->        <-p2>
    //         <---h-->
    // <-----------w----------->
    had::Dem x = grid.vol_x;
    had::Dem y = grid.vol_y;
    had::Dem w = grid.vol_w;
    had::Dem const p1 = 4;
    had::Dem const p2 = 1;
    had::Dem h = w - p1 - p2 - 4;

    had::Volume vol = audio.get_volume();

    if (w < vol_min_width) {
        log.log_err("Cannot draw volume: width is too small!");
        return had::Res::error;
    }

    char str_vol[15] = {};
    if (0 <= vol && vol < 1000) {
        sprintf(str_vol, "%3d%%", vol);
    } else {
        strcpy(str_vol, " inf");
    }


    had::Dem slider_pos = h*vol/100;
    if (slider_pos >= h) slider_pos = h-1;

    if (false
        || drawer.draw_text(x, y, "vol ")
        || drawer.draw_slider(x + p1, y, h, slider_pos)
        || drawer.draw_text(x + p1 + h, y, " ")
        || drawer.draw_text(x + p1 + h + p2, y, str_vol)
    ) {
        log.log_err("Cannot draw volume");
        return had::Res::error;
    }

    return had::Res::success;
}

static std::string format_time(had::seconds time) {
    char str[6] = {0};
    // sprintf(str, "%2d:%02d", time/60, time-(time/60));
    sprintf(str, "%2d:%02d", time / 60, time % 60);
    return std::string(str);
}

had::Res Player::draw_timeline() {
    //  0:21   -----0--      3:21      <  &  >
    // <--p1-->        <-p2>     <-p3->
    //         <---h-->
    // <-----------w------------------------->
    had::Dem x = grid.tim_x;
    had::Dem y = grid.tim_y;
    had::Dem w = grid.tim_w;
    had::Dem const p1 = 5 + 1;
    had::Dem const p2 = 1;
    had::Dem const p3 = 4;
    had::Dem h = w - p1 - p2 - p3 - 7 - 5;
    
    if (w < tim_min_width) {
        log.log_err("Cannot draw timeline: width is too small!");
        return had::Res::error;
    }

    // had::seconds cur_time = 0;
    // had::seconds duration = 0;
    had::seconds cur_time = audio.get_cur_time();
    had::seconds duration = audio.get_duration();
    had::seconds slider_pos =  (duration) ? ((h-1)*cur_time/duration) : 0;
    if (false
        || drawer.draw_text(x, y, format_time(cur_time).c_str())
        || drawer.draw_slider(x + p1, y, h, slider_pos)
        || drawer.draw_text(x + p1 + h + p2, y, format_time(duration).c_str())
        || drawer.draw_text(x + p1 + h + p2 + 5 + p3, y, "<  &  >")
    ) {
        log.log_err("Cannot draw timeline");
        return had::Res::error;
    }

    return had::Res::success;
}

had::Res Player::draw_composition_name() {
    const std::string& printed_comp_name =
        is_comp_name_hidden ? DEFAULT_COMPOSITION_PATH
                            : cur_composition_path;
    if (false
        || drawer.draw_sp_symbol(0, 0, had::SpSymbol::back_arrow_symbol)
        || drawer.draw_text(3, 0, printed_comp_name.c_str())
    ) {
        log.log_err("Cannot draw timeline");
        return had::Res::error;
    }

    return had::Res::success;
}

had::Res Player::draw_source_str() {
    std::string_view SOURCE = "Source: ";
    // return drawer.draw_text(
    //     drawer.get_width() - source_str_offset,
    //     0,
    //     source_str
    // );
    return drawer.draw_text(
        drawer.get_width() - source_str_offset,
        0,
        source_str
    );
}

had::Res Player::draw() {

    Grid grid;
    if (is_enougth_space()) {
        log.log_err("Not enought space to draw player");
        return had::Res::error;
    }

    if (calc_grid()) {
        log.log_err("Cannot calc grid");
        return had::Res::error;
    }

    if (false
        || drawer.cls()
        || draw_volume()
        || draw_timeline()
        || draw_composition_name()
        || draw_source_str()
    ) {
        log.log_err("Cannot draw player");
        return had::Res::error;
    }

    if (audio.was_finalized()) {
        cur_composition_path = DEFAULT_COMPOSITION_PATH;
        call_on_play_end();
    }

    drawer.draw_text(0, drawer.get_height() - 1, "-----");

    return had::Res::success;
}

had::Res Player::resize() {
    if (drawer.cls()) {
        return had::Res::error;
    }
    return had::Res::success;
}

bool Player::is_enougth_space() {
    return drawer.get_width() < min_width
        || drawer.get_height() < min_height;
}

had::Dem Player::get_height() {
    return min_height;
}

had::Res Player::load_and_play(std::string_view path) {
    if (audio.is_playing()) {
        if (audio.stop() != had::Audio::res_code::success) {
            return had::Res::error;
        }
    }
    if (audio.load(path) == had::Audio::res_code::success
        && audio.play() == had::Audio::res_code::success
    ) {
        cur_composition_path = path;
        return had::Res::success;
    } else {
        return had::Res::error;
    }
}

had::Res Player::play_or_stop() {
    had::Audio::res_code ret;
    if (audio.is_playing()) {
        ret = audio.stop();
    } else if (audio.is_stoped()) {
        ret = audio.play();
    } else {
        return had::Res::error;
    }

    if (ret == had::Audio::res_code::success) {
        return had::Res::success;
    } else {
        return had::Res::error;
    }
}

had::Res Player::jump(had::seconds pos) {
    if (audio.jump(pos) == had::Audio::res_code::success) {
        return had::Res::success;
    } else {
        return had::Res::error;
    }
}

had::Res Player::jump_rel(had::seconds pos_rel) {
    if (audio.jump_rel(pos_rel) == had::Audio::res_code::success) {
        return had::Res::success;
    } else {
        return had::Res::error;
    }
}

void Player::get_cur_samples(std::vector<std::complex<float>>& ret) {
    audio.get_samples(ret);
}

void Player::hide_comp_name() {
    is_comp_name_hidden = !is_comp_name_hidden;
}

void Player::set_source_str(std::string_view str) {
    source_str = str;
}