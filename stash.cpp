#include <iostream>
#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: %s distort|restore path/to/source path/to/destination\n", argv[0]);
        exit(1);
    }

    const std::string command = argv[1];
    if (command.compare("distort") != 0 && command.compare("restore") != 0)
    {
        fprintf(stderr, "Usage: %s distort|restore path/to/source path/to/destination\n", argv[0]);
        exit(1);
    }

    // Так как в нашем вариант шифрование и дешифрование - это одно и то же,
    // то и поведение для обеих команд будет одинаковым
    const fs::path source = argv[2];
    const fs::path destination = argv[3];

    constexpr auto buffer_size = 32;
    char *buffer = new char[buffer_size];
    std::ifstream f_source(source, std::ios::binary);
    std::ofstream f_destination(destination, std::ios::binary);
    while (f_source)
    {
        f_source.read(buffer, buffer_size);
        const auto count = f_source.gcount();
        if (!count)
            break;

        for (int i = 0; i < count; i++)
        {
            buffer[i] = buffer[i] xor 0x11111111;
        }

        f_destination.write(buffer, count);
    }
    f_source.close();
    f_destination.close();

    delete[] buffer;

    return 0;
}
