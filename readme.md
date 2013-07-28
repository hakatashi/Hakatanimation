* ニコニコチャンネルのアニメを自動でダウンロードする常駐プログラム
* Windows 32bit
* 習作
* 未完成

# Compile (my sake)

## <del>Borland C++ Compiler</del>

    bcc32 -tW HakataAnime.cpp
    brc32 Resource.RES HakataAnime.exe

## GNU Compiler Collection (MinGW with libcurl)

    windres Resource.RES myres.o
    g++ -c HakataAnime.cpp -o app.o -std=c++11
    g++ myres.o app.o -o HakataAnime.exe -lgdi32 -lcurl -lcurldll
