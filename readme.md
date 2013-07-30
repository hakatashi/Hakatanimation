* ニコニコチャンネルのアニメを自動でダウンロードする常駐プログラム
* Windows 32bit
* 習作
* 未完成

# Requirements
* libcurl
* boost.regex

# Compile on MinGW

    windres Resource.RES myres.o
    g++ -c HakataAnime.cpp -o app.o -std=c++11 -I"%BOOST_ROOT%"
    g++ myres.o app.o -o HakataAnime.exe -L"%BOOST_ROOT%\stage\lib" -lgdi32 -lcurl -lcurldll -lboost_regex-mgw47-mt-s-1_54

* Environmental variable %BOOST_ROOT% have to be set to your boost root directory.
* The argument -lboost_regex-mgw47-mt-s-1_54 have to be adjusted to your MinGW and boost library varsions.
