#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s path/to/file\n", argv[0]);
        exit(1);
    }

    try
    {
        const auto source = fs::path(argv[1]);
        const auto hidden_dir = fs::path(".hidden");
        if (!fs::is_directory(hidden_dir))
        {
            fs::create_directory(hidden_dir);
            fs::permissions(hidden_dir, fs::perms::owner_write | fs::perms::owner_exec);
        }

        fs::rename(source, hidden_dir / source.filename());
    }
    catch (fs::filesystem_error &error)
    {
        std::cerr << error.what() << std::endl;
        exit(1);
    }

    return 0;
}