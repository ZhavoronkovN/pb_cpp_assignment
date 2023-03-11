#ifndef COMPRESSOR_H
#define COMPRESSOR_H
#include "bitmap.h"

class Compressor
{
    Compressor &operator=(const Compressor &) = delete;
    Compressor(const Compressor &) = delete;

public:
    static Bitmap decompress(const Bitmap &compressed_bitmap) noexcept;
    static Bitmap compress(const Bitmap &bitmap) noexcept;
};

#endif // COMPRESSOR_H
