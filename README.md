## Setup Enviorment 
- download and Installing the MinGW-w64 toolchain
# Inside MSYS2  UCRT64

    - intall toolchain: pacman -S --needed base-devel mingw-w64-ucrt-x86_64-toolchain
    - install SDL + GLEW for UCRT64: pacman -S mingw-w64-ucrt-x86_64-SDL2 mingw-w64-ucrt-x86_64-glew
    - check installation : which g++
    - cd :cd /c/Users/Kavishka/Desktop/Assesments/Projects/CG_assigment/game
    - Bult app.exe : g++ -std=c++17 *.cpp -o RPGShooter.exe -DSDL_MAIN_HANDLED -lSDL2 -lglew32 -lopengl32 -mconsole
    - g++ -std=c++17 src/*.cpp -o RPGShooter.exe -DSDL_MAIN_HANDLED -lSDL2 -lglew32 -lopengl32 -mconsole -I include
    - If exeucuted without error then :
        - run: ./RPGShooter.exe