#include <openssl/md5.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <openssl/md5.h>
#include <boost/thread.hpp>
#include "ctpl.h"

std::string generateMD5(const std::string &str)
{
    unsigned char hash[MD5_DIGEST_LENGTH];

    MD5_CTX md5;
    MD5_Init(&md5);
    MD5_Update(&md5, str.c_str(), str.size());
    MD5_Final(hash, &md5);

    std::stringstream ss;

    for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
    {
        ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(hash[i]);
    }
    return ss.str();
}

std::string generateString(int index)
{
    std::string charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::string result;
    while (true)
    {
        result += charset[index % charset.size()];
        index /= charset.size();
        if (index == 0)
            break;
    }
    return result;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Usage: %s <hash>\n", argv[0]);
        return 1;
    }

    auto concurrency = boost::thread::hardware_concurrency();
    printf("Concurrency: %d\n", concurrency);
    ctpl::thread_pool pool(concurrency);
    auto per_thread = 65536;

    std::string hash = argv[1];
    unsigned long index = 0;
    std::vector<std::future<void>> futures(pool.size());
    std::vector<std::string> results(pool.size());
    while (true)
    {
        printf("Trying to find (index=%d)\n", index);
        for (int self_index = 0; self_index < pool.size(); self_index++)
        {
            auto index_start = index;
            futures[self_index] = pool.push([index_start, per_thread, hash, &results, self_index](int)
                                            {
                          for (int thread_index = index_start; thread_index < index_start + per_thread; thread_index++)
                          {
                              std::string initial = generateString(thread_index);
                            //   std::cout << initial << std::endl;
                              if (generateMD5(initial) == hash)
                              {
                                  std::cout << "Found! " << initial << std::endl;
                                  results[self_index] = initial;
                                  return;
                              }
                          }
                          results[self_index] = ""; });
            index += per_thread;
        }
        for (int self_index = 0; self_index < pool.size(); self_index++)
        {
            futures[self_index].get();
        }
        for (int self_index = 0; self_index < pool.size(); self_index++)
        {
            if (results[self_index] != "")
            {
                std::string result = results[self_index];
                printf("Found initial string: %s\n", result.c_str());
                return 0;
            }
        }

        // std::string initial = generateString(index);
        // std::cout << initial << std::endl;
        // if (generateMD5(initial) == hash)
        // {
        //     std::cout << "Found! " << initial << std::endl;
        //     return 0;
        // }
    }
    return 1;
}