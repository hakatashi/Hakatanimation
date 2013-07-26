* �j�R�j�R�`�����l���̃A�j���������Ń_�E�����[�h�����풓�v���O����
* Windows 32bit
* �K��
* ������

# Compile

## <del>Borland C++ Compiler</del>

    bcc32 -tW HakataAnime.cpp
    brc32 Resource.RES HakataAnime.exe

## GNU Compiler Collection (MinGW with libcurl)

    windres Resource.RES myres.o
    g++ -c HakataAnime.cpp -o app.o -std=c++11
    g++ myres.o app.o -o HakataAnime.exe -lgdi32 -lcurl -lcurldll
