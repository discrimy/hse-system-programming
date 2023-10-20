#include <iostream>
#include <filesystem>
#include <fstream>
#include <vector>
#include <cstring>

namespace fs = std::filesystem;

void print_usage_and_exit(char *command_name)
{
    fprintf(stderr, "Usage: %s write|read|delete path/to/image.jpg [message to write]\n", command_name);
    exit(1);
}

std::vector<char> read_all_bytes(fs::path filename)
{
    std::ifstream ifs(filename, std::ios::binary | std::ios::ate);
    std::ifstream::pos_type pos = ifs.tellg();

    if (pos == 0)
    {
        return std::vector<char>{};
    }

    std::vector<char> result(pos);

    ifs.seekg(0, std::ios::beg);
    ifs.read(&result[0], pos);

    return result;
}

void write(fs::path image_path, std::string message)
{
    std::ofstream f_image(image_path, std::ios::app);
    f_image << (char)0xC0;
    f_image.write(message.c_str(), message.size());
    f_image.close();
}

const char *read(fs::path image_path)
{
    std::vector<char> file_buffer = read_all_bytes(image_path);
    std::size_t size = file_buffer.size();

    int message_index = size - 1;
    while (message_index >= 0)
    {
        if (file_buffer[message_index] == (char)0xC0)
            break;
        message_index -= 1;
    }
    message_index += 1;

    if (message_index == 0)
    {
        return "";
    }

    auto message_size = size - message_index;
    if (message_size == 0)
    {
        return "";
    }

    char *message_buffer = new char[message_size + 1];
    for (int i = 0; i < message_size; i++)
    {
        char file_char = file_buffer[i + message_index];
        message_buffer[i] = file_char;
    }
    message_buffer[message_size] = '\0';

    return message_buffer;
}

int main(int argc, char *argv[])
{
    if (argc <= 2)
    {
        print_usage_and_exit(argv[0]);
    }
    const std::string command = argv[1];
    const fs::path image_path = argv[2];

    if (command.compare("write") == 0)
    {
        if (argc != 4)
        {
            print_usage_and_exit(argv[0]);
        }
        const std::string message = argv[3];
        write(image_path, message);
    }
    else if (command.compare("read") == 0)
    {
        const char *message = read(image_path);
        if (strcmp(message, "") == 0)
        {
            fprintf(stderr, "Message not found!\n");
            exit(2);
        }
        printf("Message: %s\n", message);
    }
    else if (command.compare("delete") == 0)
    {
        write(image_path, "");
    }
    else
    {
        print_usage_and_exit(argv[0]);
    }

    return 0;
}
