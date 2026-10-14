#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstddef>
#define max_buf 16
int main(int argc, char *argv[])
{

    if(argc != 2)
    {
        std::cerr<<"the argument should be greater than 2\n";
        return EXIT_FAILURE;
    }

    std::FILE *file = std::fopen(argv[1],"r");
    
    if(file == NULL)
    {
        std::perror("fopen");
        return EXIT_FAILURE;
    }

    unsigned char buffer[max_buf];
    bool running = true;
    std::size_t offset = 0;
    while(running)
    {
        const std::size_t bytes_read = std::fread(buffer,1, sizeof(buffer),file);
        
        if(bytes_read > 0)
        {
            
            std::printf("%08zx  ", offset);

            for(std::size_t i = 0;i < sizeof(buffer); ++i)
            {
                if(i < bytes_read)
                {
                    std::printf("%02x", buffer[i]);
                } else {
                    std::printf("  ");
                }

                if(i == 7)
                {
                    std::printf(" ");
                }
            }

            std::printf(" | ");

            for(std::size_t i = 0; i < bytes_read; ++i)
            {
                const unsigned char byte = buffer[i];
                //check ascii range 
                if(byte >= 0x20 && byte <= 0x7e)
                {
                    std::printf("%c", byte);
                    
                } else {
                    std::printf(".");
                }
            }

            std::printf("|\n");
            offset += bytes_read;
        }

        if(bytes_read < sizeof(buffer))
        {
            if(ferror(file))
            {
                std::perror("fread");
                std::fclose(file);
                return EXIT_FAILURE;
            }
        }

        if(std::feof(file))
        {
            running = false;
        }
    }

    std::fclose(file);
    return EXIT_SUCCESS;
}
