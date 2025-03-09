#include "audioFile.h"
#include "glob_types.h"

#include <cstring>
#include <fcntl.h>
#include <mpg123.h>
#include <sndfile.h>
#include <unistd.h>



static char const* file_path_pcm = "/tmp/audplay_77777.pcm";

res AudioFile::read_mp3(char const* path) {
    log_step("Loading mp3 file");

    mpg123_handle *mh = nullptr;
    unsigned char *buffer = nullptr;
    FILE *outfile = nullptr;
    size_t buffer_size = 0;
    size_t done = 0;
    long int lrate = 0;
    int err = -1;
    int encoding = 0;
    res ret = res::success;

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
        log_err("Cannot allocate buffer");
        goto label_error;   
    }

    err = mpg123_open(mh, path);
    if (err != MPG123_OK) {
        log_err("Cannot open input file");
        goto label_error;   
    }

    if (mpg123_getformat(mh, &lrate, &channels, &encoding) != MPG123_OK) {
        log_err("Cannot get file format: %s", mpg123_strerror(mh));
        goto label_error;
    }
    rate = static_cast<int>(lrate);
    samples = mpg123_length(mh);
    if (samples == MPG123_ERR) {
        log_err("Cannot get length in samples: %s", mpg123_strerror(mh));
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
        delete [] buffer;
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
    FILE *outfile;
    SF_INFO sfinfo;
    res ret = res::success;

    short buffer[1024];

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

res AudioFile::init(char const* path) {
    log_step("init() file");

    if (path == nullptr) {
        log_err("Cannot init: Path refers to nullptr");
        return res::error;
    }
    size_t len = strlen(path);
    if (len < 4) {
        log_err("Cannot init: path has no proper extension");
        return res::error;
    }
    
    res err;
    if (!strncmp(path + len - 4, ".wav", 4)) {
        err = read_wav(path);
    } else if (!strncmp(path + len - 4, ".mp3", 4)) {
        err = read_mp3(path);
    } else {
        log_err("Unknown file type");
        return res::error;
    }
    if (err == res::error) {
        log_err("Cannot conver file to pcm");
        return res::error;
    }

    fd = open(file_path_pcm, O_RDONLY);
    if (fd == -1) {
        log_err("Cannot open file: %s", strerror(errno));
        remove(file_path_pcm);
        return res::error;
    }

    is_init = true;
    return res::success;
}

res AudioFile::dstr() {
    log_step("dstr() file");

    if (!is_init) {
        log_err("dstr an uninitialized object");
        return res::error;
    }

    close(fd);
    fd = -1;
    remove(file_path_pcm);
    is_init = false;

    return res::success;
}

AudioFile::AudioFile(char const* path, res& err) {
    log_step("Creating AudioFile");
    err = init(path);
}

AudioFile::AudioFile() {
    log_step("Creating empty AudioFile");
}

res AudioFile::read_file(void* buf, size_t count, size_t& retcount) {
    if (!is_init) {
        log_err("Cannot read file: File is not initialized")
        return res::error;
    }
    if (!buf) {
        log_err("Cannot read file: buf is refering nullptr");
        return res::error;
    }

    ssize_t size = read(fd, buf, count);
    if (size == -1) {
        log_err("Cannot read file: %s", strerror(errno));
        retcount = 0;
        return res::error;
    }
    retcount = size;
    return res::success;
}

AudioFile::~AudioFile() {
    log_step("Destroying AudioFile")
    dstr();
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
