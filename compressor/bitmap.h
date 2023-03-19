#ifndef BITMAP_H
#define BITMAP_H
#include <stdio.h>
#include <string>

struct Bitmap
{
    size_t width;
    size_t height;
    bool compressed;
    size_t size;

    Bitmap(size_t width, size_t height);
    Bitmap(size_t width, size_t height, uint8_t *data, size_t compressed_size = 0);
    Bitmap(Bitmap &&);
    ~Bitmap();
    inline uint8_t read(size_t x, size_t y) const;
    inline void write(size_t x, size_t y, const uint8_t value);
    inline uint8_t read(size_t ind) const;
    inline void write(size_t ind, const uint8_t value);
    std::string dumps() const;
    bool dump(const char *full_path) const noexcept;
    static Bitmap loads(const std::string &dump);
    static Bitmap load(const char *full_path);
    Bitmap &operator=(const Bitmap &) = delete;
    Bitmap(const Bitmap &) = delete;

private:
    uint8_t *data;
    static constexpr const char *dump_sep = "|";
};

#endif // BITMAP_H
