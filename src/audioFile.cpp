#include "audioFile.h"
#include "glob_types.h"

#include <cstring>
#include <mpg123.h>
#include <sndfile.h>



static char const* file_path_pcm = "/tmp/audplay_77777.pcm";
static size_t const buffer_size = 1024;

res AudioFile::read_mp3(char const* path) {
    log_step("Loading mp3 file");

    mpg123_handle *mh = nullptr;
    unsigned char *buffer = nullptr;
    size_t buffer_size = 0;
    size_t done = 0;
    FILE *outfile = nullptr;
    int err = -1;
    int encoding = 0;
    long lrate = 0;
    res ret = res::success;

    if (mpg123_init() != MPG123_OK) {
        goto label_error;
    }
    mh = mpg123_new(NULL, &err);
    if (err != MPG123_OK) {
        goto label_error;
    }

    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*) malloc(buffer_size);
    if (buffer == nullptr) {
        log_err("Cannot allocate buffer");
        goto label_error;   
    }

    err = mpg123_open(mh, path);
    if (err != MPG123_OK) {
        log_err("Cannot open input file");
        goto label_error;   
    }

    if (mpg123_getformat(mh, &lrate, &channels, &encoding) != MPG123_OK) {
        log_err("Cannot get format: %s", mpg123_strerror(mh));
        goto label_error;
    }
    rate = static_cast<int>(lrate);
    samples = mpg123_length(mh);
    if (samples == MPG123_ERR) {
        log_err("Cannot get samples: %s", mpg123_strerror(mh));
        goto label_error;
    }

    outfile = fopen(file_path_pcm, "wb");
    if (!outfile) {
        log_err("Cannot open output file: %s", strerror(errno));
        goto label_error;
    }

    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK) {
        fwrite(buffer, 1, done, outfile);
    }

    goto label_success;
    label_error:
        ret = res::error;
    label_success:
        if (outfile != nullptr) {
            fclose(outfile);
        }
        free(buffer);
        if (mh != nullptr) {
            mpg123_close(mh);
            mpg123_delete(mh);
        }
        mpg123_exit(); // tribute to the past

        return ret;
}

res AudioFile::read_wav(char const* path) {
    log_step("Loading wav file");

    SNDFILE *infile;
    SF_INFO sfinfo;
    FILE *outfile;
    res ret = res::success;

    short buffer[buffer_size];

    infile = sf_open(path, SFM_READ, &sfinfo);
    if (!infile) {
        char err_str[100];
        sf_error_str(infile, err_str, 100);
        log_err("Cannot open input file: %s", err_str);
        goto label_error;
    }

    rate     = sfinfo.samplerate;
    channels = sfinfo.channels;
    samples  = sfinfo.frames;

    outfile = fopen(file_path_pcm, "wb");
    if (!outfile) {
        log_err("Cannot open output file: %s", strerror(errno));
        goto label_error;
    }

    sf_count_t count;
    while ((count = sf_read_short(infile, buffer, 1024))) {
        fwrite(buffer, sizeof(short), count, outfile);
    }

    goto label_success;
    label_error:
        ret = res::error;
    label_success:
        if (infile != nullptr) {
            sf_close(infile);
        }
        if (outfile != nullptr) {
            fclose(outfile);
        }
        return ret;
}

AudioFile::AudioFile(char const* path, res& err) {
    log_step("Creating AudioFile");

    if (path == nullptr) {
        log_err("Path refers to nullptr");
        err = res::error;
        return;
    }
    size_t len = strlen(path);
    if (len < 4) {
        log_err("Invalid path");
        err = res::error;
        return;
    }
    
    if (!strncmp(path + len - 4, ".wav", 4)) {
        err = read_wav(path);
    } else if (!strncmp(path + len - 4, ".mp3", 4)) {
        err = read_mp3(path);
    } else {
        log_err("Unknown file type");
        err = res::error;
        return;
    }
    if (err == res::error) {
        log_err("Cannot conver file to pcm");
        return;
    }

    err = res::success;
}

AudioFile::~AudioFile() {
    log_step("Destroying AudioFile")
    remove(file_path_pcm);
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
