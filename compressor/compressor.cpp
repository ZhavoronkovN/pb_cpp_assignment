#include "compressor.h"
#include <vector>
#include <iostream>

constexpr uint8_t white = 0xff;
constexpr uint8_t black = 0;
constexpr size_t group_size = 4;

inline uint8_t read_bit(uint8_t &current_bit_ind, size_t &compressed_data_ind, const Bitmap &compressed_bitmap) noexcept
{
    const auto bit = (compressed_bitmap.read(compressed_data_ind) >> current_bit_ind++) & 1;
    if (current_bit_ind > 7)
    {
        current_bit_ind = 0;
        compressed_data_ind += 1;
    }
    return bit;
}

inline uint8_t read_byte(uint8_t &current_bit_ind, size_t &compressed_data_ind, const Bitmap &compressed_bitmap) noexcept
{
    uint8_t byte = 0;
    for (auto i = 0; i < 8; i++)
    {
        byte |= read_bit(current_bit_ind, compressed_data_ind, compressed_bitmap) << i;
    }
    return byte;
}

Bitmap Compressor::decompress(const Bitmap &compressed_bitmap) noexcept
{
    if (!compressed_bitmap.compressed)
        throw std::runtime_error("Cannot decompress uncrompressed bitmap");

    for (auto i = 0; i < compressed_bitmap.size; i++)
    {
    }
    const auto decompressed_size = compressed_bitmap.width * compressed_bitmap.height;
    std::vector<uint8_t> decompressed_data;
    uint8_t current_bit_ind = 0;
    uint8_t last_bit = 0;

    for (size_t compressed_data_ind = 0; compressed_data_ind < compressed_bitmap.size && decompressed_data.size() < decompressed_size;)
    {
        const auto bit = read_bit(current_bit_ind, compressed_data_ind, compressed_bitmap);
        if (bit == 0)
        {
            if (last_bit == 0)
            {
                for (auto i = 0; i < group_size; i++)
                {
                    decompressed_data.push_back(white);
                }
            }
            else
            {
                for (auto i = 0; i < group_size; i++)
                {
                    decompressed_data.push_back(black);
                }
                last_bit = 0;
            }
        }
        else
        {
            if (last_bit == 0)
            {
                last_bit = 1;
            }
            else
            {
                for (auto i = 0; i < group_size; i++)
                {
                    const auto byte = read_byte(current_bit_ind, compressed_data_ind, compressed_bitmap);
                    decompressed_data.push_back(byte);
                    if (decompressed_data.size() == decompressed_size)
                        break;
                }
                last_bit = 0;
            }
        }
    }
    uint8_t *raw_data = new uint8_t[decompressed_size];
    auto ind = 0;
    for (const auto byte : decompressed_data)
    {
        raw_data[ind++] = byte;
    }
    return Bitmap(compressed_bitmap.width, compressed_bitmap.height, raw_data);
}

inline uint8_t write_bit(const uint8_t bit, uint8_t &current_index, std::vector<uint8_t> &storage) noexcept
{
    *storage.rbegin() |= ((bit & 1) << current_index++);
    if (current_index > 7)
    {
        storage.push_back(0);
        current_index = 0;
    }
    return current_index;
}

inline uint8_t write_byte(const uint8_t byte, uint8_t &current_index, std::vector<uint8_t> &storage) noexcept
{
    auto result = current_index;
    for (auto i = 0; i < 8; i++)
    {
        result = write_bit(byte >> i, current_index, storage);
    }
    return result;
}

Bitmap Compressor::compress(const Bitmap &bitmap) noexcept
{
    if (bitmap.compressed)
        throw std::runtime_error("Cannot compress compressed bitmap");

    std::vector<uint8_t> compressed_data;
    compressed_data.push_back(0);
    uint8_t current_byte_ind = 0;
    auto ind = 0;
    for (; ind + group_size - 1 < bitmap.size; ind += group_size)
    {
        const auto b0 = bitmap.read(ind);
        bool pixels_same = true;
        for (auto byte_ind = ind + 1; byte_ind < ind + group_size; byte_ind++)
        {
            if (bitmap.read(byte_ind) != b0)
            {
                pixels_same = false;
                break;
            }
        }
        if (pixels_same && (b0 == black || b0 == white))
        {
            if (b0 == black)
            {
                write_bit(1, current_byte_ind, compressed_data);
                write_bit(0, current_byte_ind, compressed_data);
            }
            else
            {
                write_bit(0, current_byte_ind, compressed_data);
            }
        }
        else
        {
            write_bit(1, current_byte_ind, compressed_data);
            write_bit(1, current_byte_ind, compressed_data);
            for (auto byte_ind = ind; byte_ind < ind + group_size; byte_ind++)
            {
                write_byte(bitmap.read(byte_ind), current_byte_ind, compressed_data);
            }
        }
    }
    if (ind < bitmap.size)
    {
        write_bit(1, current_byte_ind, compressed_data);
        write_bit(1, current_byte_ind, compressed_data);
        for (; ind < bitmap.size; ind++)
        {
            write_byte(bitmap.read(ind), current_byte_ind, compressed_data);
        }
    }
    uint8_t *raw_data = new uint8_t[compressed_data.size()];
    ind = 0;
    for (const auto byte : compressed_data)
    {
        raw_data[ind++] = byte;
    }
    return Bitmap(bitmap.width, bitmap.height, raw_data, compressed_data.size());
}