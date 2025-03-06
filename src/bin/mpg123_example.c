#include <mpg123.h>
#include <stdio.h>

int main() {
    mpg123_handle *mh;
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;
    int err;

    mpg123_init();
    mh = mpg123_new(NULL, &err);
    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*) malloc(buffer_size);

    // Открываем MP3-файл
    mpg123_open(mh, "input.mp3");

    // Открываем выходной файл
    FILE *out = fopen("output.pcm", "wb");

    // Декодируем MP3 в PCM
    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK) {
        fwrite(buffer, 1, done, out);
    }

    // Освобождаем ресурсы
    fclose(out);
    free(buffer);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();

    return 0;
}
