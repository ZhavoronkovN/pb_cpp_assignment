#include "compressor/compressor.h"
#include <iostream>

int main(int argc, char *argv[])
{
    Bitmap b(2, 5);
    b.write(4, 0xff);
    b.write(5, 0xff);
    b.write(6, 0xff);
    b.write(7, 0xff);
    auto cb = Compressor::compress(b);
    auto db = Compressor::decompress(Bitmap::loads(cb.dumps()));

    std::cout << b.dumps() << std::endl;
    std::cout << cb.dumps() << std::endl;
    std::cout << Bitmap::loads(cb.dumps()).dumps() << std::endl;
    std::cout << db.dumps() << std::endl;
    return 0;
}