#include "audioFile.h"

#include <cassert>
#include <cstddef>
#include <cstdio>
#include <curses.h>
#include <string>
#include <cstring>
#include <fcntl.h>
#include <mpg123.h>
#include <sndfile.h>
#include <unistd.h>

#include "had_logger.h"
#include "had_types.h"


static char const* file_path_pcm = "/tmp/audplay_77777.pcm";

namespace had {
    Res AudioFile::read_mp3(char const* path) {
        log.log_info("Loading mp3 file");

        mpg123_handle *mh = nullptr;
        unsigned char *buffer = nullptr;
        FILE *outfile = nullptr;
        size_t buffer_size = 0;
        size_t done = 0;
        long int lrate = 0;
        int err = -1;
        int encoding = 0;
        Res ret = Res::success;

        if (mpg123_init() != MPG123_OK) {
            goto label_error;
        }
        mh = mpg123_new(NULL, &err);
        if (err != MPG123_OK) {
            goto label_error;
        }

        buffer_size = mpg123_outblock(mh);
        buffer = new unsigned char[buffer_size];
        if (buffer == nullptr) {
            log.log_err("Cannot allocate buffer");
            goto label_error;   
        }

        err = mpg123_open(mh, path);
        if (err != MPG123_OK) {
            log.log_err("Cannot open input file");
            goto label_error;   
        }

        if (mpg123_getformat(mh, &lrate, &channels, &encoding) !=
                                                            MPG123_OK) {
            log.log_err(std::string("Cannot get file format: ") +
                                                    mpg123_strerror(mh));
            goto label_error;
        }
        rate = static_cast<int>(lrate);
        samples = mpg123_length(mh);
        if (samples == MPG123_ERR) {
            log.log_err(std::string("Cannot get length in samples: ") + 
                                                    mpg123_strerror(mh));
            goto label_error;
        }

        outfile = fopen(file_path_pcm, "wb");
        if (!outfile) {
            log.log_err(std::string("Cannot open output file: ") +
                                                        strerror(errno));
            goto label_error;
        }

        while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK) {
            fwrite(buffer, 1, done, outfile);
        }

        goto label_success;
        label_error:
            ret = Res::error;
        label_success:
            if (outfile != nullptr) {
                fclose(outfile);
            }
            delete [] buffer;
            if (mh != nullptr) {
                mpg123_close(mh);
                mpg123_delete(mh);
            }
            mpg123_exit(); // tribute to the past

            return ret;
    }

    Res AudioFile::read_wav(char const* path) { // Whis is also works for mp3 files...
        log.log_info("Loading wav file");

        SNDFILE *infile = nullptr;
        FILE *outfile = nullptr;
        SF_INFO sfinfo;
        Res ret = Res::success;

        constexpr std::size_t buffer_size = 1024;
        // short buffer[buffer_size];
        int buffer[buffer_size];

        infile = sf_open(path, SFM_READ, &sfinfo);
        if (!infile) {
            char err_str[100];
            sf_error_str(infile, err_str, 100);
            log.log_err(std::string("Cannot open input file: ") + err_str);
            goto label_error;
        }

        rate     = sfinfo.samplerate;
        channels = sfinfo.channels;
        samples  = sfinfo.frames;

        outfile = fopen(file_path_pcm, "wb");
        if (!outfile) {
            log.log_err(std::string("Cannot open output file: ")
                                                        + strerror(errno));
            goto label_error;
        }

        sf_count_t count;
        // while ((count = sf_read_short(infile, buffer, buffer_size))) {
        while ((count = sf_read_int(infile, buffer, buffer_size))) {
            // fwrite(buffer, sizeof(short), count, outfile);
            fwrite(buffer, sizeof(int), count, outfile);
        }

        goto label_success;
        label_error:
            ret = Res::error;
        label_success:
            if (infile != nullptr) {
                sf_close(infile);
            }
            if (outfile != nullptr) {
                fclose(outfile);
            }
            return ret;
    }

    AudioFile::res_code AudioFile::load(const std::string& path) {
        log.log_info("init() file");

        if (is_inited) {
            erase();
        }


        // if (path.length() < 4) {
        //     log.log_err("Cannot init: path has no proper extension");
        //     return Res::error;
        // }
        
        Res err;
        // if (!strncmp(path + len - 4, ".wav", 4)) {
        //     err = read_wav(path);
        // } else if (!strncmp(path + len - 4, ".mp3", 4)) {
        //     err = read_mp3(path);
        // } else {
        //     log.log_err("Unknown file type");
        //     return Res::error;
        // }
        // err = read_wav(path.c_str());
        err = read_mp3(path.c_str());
        if (err == Res::error) {
            log.log_err("Cannot conver file to pcm");
            return res_code::other_error;
        }

        file = fopen(file_path_pcm, "rb");
        if (!file) {
            log.log_err(std::string("Cannot open file: %s") +
                                                        strerror(errno));
            remove(file_path_pcm);
            return res_code::other_error;
        }

        cur_pos = 0;
        is_end_reached = false;
        is_inited = true;
        return res_code::success;
    }

    AudioFile::res_code AudioFile::erase() {
        log.log_info("Erasing file");

        if (!is_inited) {
            is_end_reached = false;
            return res_code::success;
        }

        fclose(file);
        file = nullptr;
        remove(file_path_pcm);
        is_inited = false;
        is_end_reached = false;

        // delete [] buf;
        // delete [] buf2;
        // buf = nullptr;
        // buf2 = nullptr;
        // buf_size = 0;

        return res_code::success;
    }

    AudioFile::AudioFile(const Logger& log) : log(log) {
        log.log_info("Creating an empty AudioFile");
    }

    void AudioFile::copy_to_buf(short* ref_buf, int size) {
        // if (!buf || buf_size != size) {
        //     buf = new short[size];
        //     buf_size = size;
        // }
        buf.clear();

        for (int q = 0; q < size; ++q) {
            // buf[q] = static_cast<int>(ref_buf[q]);
            if (q % 100 == 0) {
                buf.push_back(static_cast<int>(ref_buf[q]));
            }
        }
    }

    AudioFile::res_code AudioFile::read_file(void* buf, size_t byte_count,
                                                        size_t& retcount) {
        if (!is_inited) {
            log.log_err("File is not initialized");
            return res_code::other_error;
        }
        if (!buf) {
            log.log_err("buf is derefering nullptr");
            return res_code::other_error;
        }

        // std::cout << "get_max_pos() " << get_max_pos() << std::endl;
        // std::cout << "cur_pos " << cur_pos << std::endl;
        // if (cur_pos == get_max_pos()) {
        if (is_end_reached) {
            retcount = 0;
            return res_code::end_of_file;
        }
        std::size_t size = fread(buf, 1, byte_count, file);
        if (size == 0) {
            retcount = 0;
            is_end_reached = true;
            return res_code::success;
            // log.log_err(std::string("Cannot read file: ") +
            //                                             strerror(errno));
        }
        cur_pos += size;
        retcount = size;

        copy_to_buf(reinterpret_cast<short*>(buf), byte_count / 2);

        return res_code::success;
    }

    AudioFile::res_code AudioFile::set_position(std::size_t position) {
        if (!is_inited) {
            log.log_err("File is not initialized");
            return res_code::not_initialized;
        }

        int err = fseek(file, position, SEEK_SET);
        if (err) {
            // log.log_warn("Cannot seek file");
            is_end_reached = true;
            return res_code::success;
            // return res_code::cannot_set_position;
        }
        cur_pos = position;
        is_end_reached = false;

        return res_code::success;
    }

    AudioFile::~AudioFile() {
        log.log_info("Destroying AudioFile");
        if (is_inited) {
            erase();
        }
    }

    std::size_t AudioFile::get_cur_pos() {
        return cur_pos;
    }

    std::size_t AudioFile::get_max_pos() {
        return samples * channels * 2; // DEV [mult depends on lib]
    }

    int AudioFile::get_rate() {
        return rate;
    }

    int AudioFile::get_channels() {
        return channels;
    }

    int AudioFile::get_samples() {
        return samples;
    }

    const std::vector<int>& AudioFile::get_buf() {
        return buf;
    }
}
