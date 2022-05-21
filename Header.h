#pragma once
#ifndef bebra
#define bebra

#include <iostream>
#include <conio.h>
#include <string>
#include <windows.h>
#include <Lmcons.h>
#include "SDL.h"
#include "SDL_ttf.h"
#include"SDL_image.h"
#include"SDL_mixer.h"
#include <fstream>
#include <direct.h>
#include <algorithm>

bool checkFileCorrupion(std::string SaveFilePath, const char* username, char l);
inline bool exists_test(const std::string& name);
void createsavefile(std::string SaveFilePath, const char* username);
void sound(Mix_Chunk *Sound);
bool Game(SDL_Event& e, SDL_Renderer* renderer, SDL_Window* window, Mix_Chunk* Sound, Mix_Music** fon, int& WIDTH, int& HEIGHT, SDL_Texture*& bgImg, SDL_Texture*& resource, SDL_Texture*& white, SDL_Texture*& Green, SDL_Texture*& Red, SDL_Texture*& Back, double& aspratio, int& bgw, int& bgh, SDL_Rect& format, bool& fullscreen, std::string SaveFilePath, const char* username, int* volume, bool& muted, char& theme, SDL_Rect& volumeBG, SDL_Rect& themeField, SDL_Rect themesFormat[3], SDL_Rect themepick[3], bool ontheme[3], SDL_Texture*& ThemePick, SDL_Rect volumeSQ[2], SDL_Texture* Volume[2], SDL_Rect volumeNUM[2], SDL_Rect volumeP[3], SDL_Texture*& MusicVolume, SDL_Texture*& SoundVolume, SDL_Rect& musicvolume, SDL_Rect& soundvolume, bool& onmute, SDL_Rect& MuteSQ1, SDL_Texture*& volumeICON, SDL_Rect& themePick, SDL_Rect mute[2], SDL_Texture*themes[3], SDL_Rect& back, SDL_Cursor*& cursor);
bool Game1(SDL_Event& e, SDL_Renderer* renderer, SDL_Window* window, Mix_Chunk* Sound, Mix_Music** fon, int& WIDTH, int& HEIGHT, SDL_Texture*& bgImg, SDL_Texture*& resource, SDL_Texture*& white, double& aspratio, int& bgw, int& bgh, SDL_Rect& format, bool& fullscreen, int& mines1, int* pole, char l, std::string SaveFilePath, const char* username, bool load, int* volume, bool& muted, char& theme, SDL_Rect& volumeBG, SDL_Rect& themeField, SDL_Rect themesFormat[3], SDL_Rect themepick[3], bool ontheme[3], SDL_Texture*& ThemePick, SDL_Rect volumeSQ[2], SDL_Texture* Volume[2], SDL_Rect volumeNUM[2], SDL_Texture*& Green, SDL_Rect volumeP[3], SDL_Texture*& MusicVolume, SDL_Texture*& SoundVolume, SDL_Rect& musicvolume, SDL_Rect& soundvolume, bool& onmute, SDL_Rect& MuteSQ1, SDL_Texture*& volumeICON, SDL_Rect& themePick, SDL_Rect mute[2], SDL_Texture* themes[3], SDL_Texture*& Back1, SDL_Cursor*& cursor);
void DestroyMedia(SDL_Texture* img, SDL_Texture* rsrc, SDL_Texture* vlm, Mix_Chunk* Sound, Mix_Music** fon);
bool loadMedia(SDL_Renderer* renderer, SDL_Texture** img, SDL_Texture** vlm, SDL_Texture** rsrc, Mix_Chunk** Sound, Mix_Music** fon, char ch, SDL_Cursor* cursor);
bool InRect(int& x, int& y, SDL_Rect& Rect);
void init_square(SDL_Rect* square);
SDL_Texture* renderText(const std::string& message, const std::string& fontFile, SDL_Color color, int fontSize, SDL_Renderer* renderer);
SDL_Texture* loadTexture(SDL_Renderer *renderer,std::string path);
void Play_Music(Mix_Music** fon,char theme);
void DrawBoldRect(SDL_Renderer* renderer, SDL_Rect rect);
using namespace std;
#endif