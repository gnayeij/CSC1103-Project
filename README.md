# CSC1103-Project
Setup Raylib Guide: 
1. Download raylib latest release from the github repo (https://github.com/raysan5/raylib/releases), download the latest release's "source.zip".

2. Download w64devkit-x.xx.x.zip from https://github.com/skeeto/w64devkit, download "w64devkit-x64-2.0.0.exe".

3. Run the w64devkit.exe installer file and there should prompt you to choose a destination extraction folder.

4. Navigate to the w64devkit folder and run w64devkit.exe. It should open up a console. It is a linux-based console.

5. Inside the console, navigate to the folder where the extracted raylib folder is located and cd to /src. Once at the dir (/src), run “mingw32-make PLATFORM=PLATFORM_DESKTOP” 

6. Refer to https://youtu.be/HPDLTQ4J_zQ?t=354 , to learn how to setup VSCode Environment to code and compile using raylib locally.

7. For the "include" & "lib" folder. Go to your raylib/src folder and copy the files, "raylib.h" and "libraylib.a" and paste into their own folders on the repo respectively. 

How it Works:
- Develop code in main.c, then compile locally using gcc. 

- There are many compiler arguments that we can parsed when compiling, an example is: "gcc main.c -o game.exe -O1 -Wall -std=c99 -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm" .

- As the command-line to compile can be quite long, I created a powershell script file ("run.ps1") that we can just run to help us compile easier instead of typing it out everytime we want to compile. (just run .\run.ps1 in any terminal to compile.)

- If you get unauthorizedAccess error when running the .ps1 file, open up a powershell terminal and run this: "Set-ExecutionPolicy -ExecutionPolicy Unrestricted -Scope CurrentUser" 

To note: 
- Finalised game.exe has been added into repo.

To run game:
- run game.exe

If need to recompile game:
- run script file: .\run.ps1
