* ニコニコチャンネルのアニメを自動でダウンロードする常駐プログラム
* Windows 32bit
* 習作
* 未完成
* パスワードを平文保存する程度の能力

# Requirements

* Python 2.7

# Compile Requirements

* libcurl
* boost.regex

# Compile on MinGW

    windres Resource.RES myres.o
    g++ -c Hakatanimation.cpp -o app.o -std=c++11 -I"%BOOST_ROOT%"
    g++ myres.o app.o -o Hakatanimation.exe -L"%BOOST_ROOT%\stage\lib" -lgdi32 -lcurl -lcurldll -lboost_regex-mgw47-mt-s-1_54

* Environmental variable %BOOST_ROOT% have to be set to your boost root directory.
* The argument -lboost_regex-mgw47-mt-s-1_54 have to be adjusted to your own MinGW and boost library versions.

# Licensed BY

## [nicovideo-dl](http://sourceforge.jp/projects/nicovideo-dl/)

    Copyright (c) 2009 Keiichiro Nagano
    Copyright (c) 2009 Kimura Youichi
    Copyright (c) 2006-2008 Ricardo Garcia Gonzalez
    Copyright (c) 2008 Ying-Chun Liu (PaulLiu)
    
    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:
    
    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.
    
    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
    OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
    ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
    OTHER DEALINGS IN THE SOFTWARE.
    
    Except as contained in this notice, the name(s) of the above copyright
    holders shall not be used in advertising or otherwise to promote the
    sale, use or other dealings in this Software without prior written
    authorization.