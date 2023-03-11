#ifndef BITMAP_H
#define BITMAP_H
#include <stdio.h>
#include <string>
#include <string_view>
#include <vector>

struct Bitmap
{
    size_t width;
    size_t height;
    bool compressed;
    size_t size;

    Bitmap(size_t width, size_t height) : width(width), height(height)
    {
        data = new uint8_t[width * height];
        compressed = false;
        size = width * height;
    }
    Bitmap(size_t width, size_t height, uint8_t *data, size_t compressed_size = 0) : width(width), height(height), data(data), compressed(compressed_size != 0)
    {
        if (compressed)
            size = compressed_size;
        else
            size = width * height;
    }
    Bitmap(Bitmap &&);
    ~Bitmap() { delete[] data; }
    inline uint8_t read(size_t x, size_t y) const { return read(y * width + x); }
    inline void write(size_t x, size_t y, const uint8_t value) { write(y * width + x, value); }
    inline uint8_t read(size_t ind) const { return data[ind]; }
    inline void write(size_t ind, const uint8_t value) { data[ind] = value; }

    std::string dumps() const
    {
        auto result = std::to_string(width) + dump_sep + std::to_string(height) + dump_sep + std::to_string(size);
        for (auto ind = 0; ind < size; ind++)
        {
            result += dump_sep + std::to_string((int)read(ind));
        }
        return result;
    }

    static Bitmap loads(std::string_view dump)
    {
        const auto first_sep_ind = dump.find(dump_sep);
        size_t width = std::stol(std::string(dump.substr(0, first_sep_ind)));
        const auto sec_sep_ind = dump.find(dump_sep, first_sep_ind + 1);
        size_t height = std::stol(std::string(dump.substr(first_sep_ind + 1, sec_sep_ind - first_sep_ind - 1)));
        const auto third_sep_ind = dump.find(dump_sep, sec_sep_ind + 1);
        size_t size = std::stol(std::string(dump.substr(sec_sep_ind + 1, third_sep_ind - sec_sep_ind - 1)));
        auto data_str = std::string(dump.substr(third_sep_ind + 1));
        std::vector<uint8_t> data;
        for (auto sep_ind = data_str.find(dump_sep); sep_ind != data_str.npos;)
        {
            data.push_back(std::stoi(data_str.substr(0, sep_ind)));
            data_str = data_str.substr(sep_ind + 1);
            sep_ind = data_str.find(dump_sep);
        }
        data.push_back(std::stoi(data_str.substr(0)));
        uint8_t *raw_data = new uint8_t[data.size()];
        auto ind = 0;
        for (auto byte : data)
        {
            raw_data[ind++] = byte;
        }
        if (size == width * height)
        {
            size = 0;
        }
        return Bitmap(width, height, raw_data, size);
    }

    // we are storing raw pointer (for some reason), so need to clear it in destructor
    Bitmap &operator=(const Bitmap &) = delete;
    Bitmap(const Bitmap &) = delete;

private:
    uint8_t *data;

    static constexpr const char *dump_sep = "|";
};

#endif // BITMAP_H
