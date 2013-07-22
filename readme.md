* ニコニコチャンネルのアニメを自動でダウンロードする常駐プログラム
* Windows 32bit
* 習作
* 未完成

# Compile

## Borland C++ Compiler

    bcc32 -tW HakataAnime.cpp
    brc32 Resource.RES HakataAnime.exe

## GNU Compiler Collection (MinGW)

    windres Resource.RES myres.o
    g++ -c HakataAnime.cpp -o app.o
    g++ myres.o app.o -o HakataAnime.exe -lgdi32

# Licenses

## libcURL

COPYRIGHT AND PERMISSION NOTICE
 
Copyright (c) 1996 - 2013, Daniel Stenberg, <daniel@haxx.se>.
 
All rights reserved.
 
Permission to use, copy, modify, and distribute this software for any purpose
with or without fee is hereby granted, provided that the above copyright
notice and this permission notice appear in all copies.
 
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT OF THIRD PARTY RIGHTS. IN
NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
OR OTHER DEALINGS IN THE SOFTWARE.
 
Except as contained in this notice, the name of a copyright holder shall not
be used in advertising or otherwise to promote the sale, use or other dealings
in this Software without prior written authorization of the copyright holder.