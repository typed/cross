#include <iostream>

#ifdef _WIN32
//Windows
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "Secur32.lib")
#pragma comment(lib, "Bcrypt.lib")
#endif

int main_simplest_ffmpeg_decoder(int argc, char* argv[]);
int main_simplest_ffmpeg_helloworld(int argc, char* argv[]);
int main_simplest_ffmpeg_player(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    //main_simplest_ffmpeg_decoder(argc, argv);
    //main_simplest_ffmpeg_helloworld(argc, argv);
    main_simplest_ffmpeg_player(argc, argv);
    return 0;
}
