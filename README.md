# CSC1103-Project
Setup Raylib Guide: 
1. Download raylib latest release from the github repo (https://github.com/raysan5/raylib/releases)

2. Download w64devkit-x.xx.x.zip from https://github.com/skeeto/w64devkit

3. Run the installer file and there should be a new w64devkit folder.

4. Navigate to the w64devkit folder and run w64devkit.exe. It should open up a console. It is a linux-based console.

5. Inside the console, navigate to the folder where the extracted raylib folder is located. Once at the dir, run “mingw32-make PLATFORM=PLATFORM_DESKTOP” 

6. Refer to https://youtu.be/HPDLTQ4J_zQ?t=354 , to learn how to setup VSCode Environment to code and compile using raylib locally.

How it Works:
- Develop code in main.c, then compile locally using gcc. 

- There are many compiler arguments that we can parsed when compiling, an example is: "gcc main.c -o game.exe -O1 -Wall -std=c99 -Wno-missing-braces -I include/ -L lib/ -lraylib -lopengl32 -lgdi32 -lwinmm" .

- As the command-line to compile can be quite long, I created a powershell script file ("run.ps1") that we can just run to help us compile easier instead of typing it out everytime we want to compile. (just run .\run.ps1 in any terminal to compile.)

To note: 
- I add the game.exe file into .gitignore.
