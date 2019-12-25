#include <iostream>
#include <objbase.h>

#ifdef _WIN32
//Windows
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Secur32.lib")
#pragma comment(lib, "Bcrypt.lib")
#pragma comment(lib, "Ole32.lib")
#endif

int main_simplest_ffmpeg_decoder(int argc, char* argv[]);
int main_simplest_ffmpeg_helloworld(int argc, char* argv[]);
int main_simplest_ffmpeg_player(int argc, char* argv[]);
extern "C" int main_ffplay(int argc, char **argv);

int main(int argc, char** argv)
{
    //main_simplest_ffmpeg_decoder(argc, argv);
    //main_simplest_ffmpeg_helloworld(argc, argv);
    //main_simplest_ffmpeg_player(argc, argv);
    main_ffplay(argc, argv);
    return 0;
}
