#include "header.h"
inline bool exists_test(const std::string& name) {
	struct stat buffer;
	return (stat(name.c_str(), &buffer) == 0);
}

void createsavefile(std::string SaveFilePath, const char* username) {
	std::ofstream savefile(SaveFilePath, ios::binary);
	savefile << username << endl<<"THEME 0"<<endl << "MUTED 0" << endl;
	savefile << "VOLUME0 026" << endl;
	savefile << "VOLUME1 026" << endl;
	for (int i = 0; i < 4; i++) {
		savefile << "GAMESAVE" << i << ' '<<endl;
	}
	savefile.close();
}

bool readfile(std::ifstream &savefileIN, std::string SaveFilePath, const char* username, int*volume, bool &muted, char &theme) {
	bool badfile = false;
	std::string string;
	if (getline(savefileIN, string)) {
		if (string != username) {
			badfile = true;
		}
		else {
			if (getline(savefileIN, string)) {
				if (string.find("THEME") || string.length() < 7) {
					badfile = true;
				}
				else theme = string[6] - '0';
			}
			else badfile = true;
			if (getline(savefileIN, string)) {
				if (string.find("MUTED") || string.length()<7) {
					badfile = true;
				}
				else muted = string[6] - '0';
			}
			else badfile = true;
		}
	}
	else badfile = true;
	if (!savefileIN.good())badfile = true;
	if (getline(savefileIN, string)) {
		char volume0[9] = "VOLUME0 ";
		for (int i = 0; i < 8; i++) {
			if (string[i] != volume0[i]) badfile = true;
		}
		if (!badfile) {
			int sotka = 100;
			for (int i = 8; i < 11; i++) {
				if (string[i] >= '0' && string[i] <= '9') volume[0] += (string[i] - '0') * sotka;
				sotka /= 10;
			}
		}
		
	}
	if (getline(savefileIN, string)) {
		char volume1[9] = "VOLUME1 ";
		for (int i = 0; i < 8; i++) {
			if (string[i] != volume1[i]) badfile = true;
		}
		if (!badfile) {
			int sotka = 100;
			for (int i = 8; i < 11; i++) {
				if (string[i] >= '0' && string[i] <= '9') volume[1] += (string[i] - '0') *sotka;
				sotka /= 10;
			}
		}
	}
	int u = 0;
	while (getline(savefileIN, string)) {
		if (string.find("AMESAVE")) u++;
	}
	if (u != 4) badfile = true;
	return badfile;
}

int main(int argc, char** argv) {
	_mkdir("./Saves");

	std::string SaveFilePath = "Saves\\";
	char username[UNLEN+1];
	DWORD username_len = UNLEN+1;
	GetUserNameA(username, &username_len);
	SaveFilePath += username;
	SaveFilePath+=".sav";
	bool badfile = false;
	int volume[2] = {0,0};
	char theme;
	std::ifstream savefileIN;
	savefileIN.open(SaveFilePath, ios::binary);
	if (!exists_test(SaveFilePath)) {
		createsavefile(SaveFilePath, username);
	}
	bool muted = NULL;
	badfile = readfile(savefileIN, SaveFilePath, username, volume, muted, theme);
	if (badfile) {
	savefileIN.close();
	createsavefile(SaveFilePath, username);
	savefileIN.open(SaveFilePath, ios::binary);
	badfile = readfile(savefileIN, SaveFilePath, username, volume, muted, theme);
	}
	savefileIN.close();

	if (badfile) { cout << "Что-то не так с файлом сохранения/диском/файловой системой/хз с чем ещё"; return 0; }
	
	bool quit = 0;
	SDL_Event e;
	SDL_Texture* bgImg = NULL, * resource = NULL;
	int WIDTH = 1280;
	int HEIGHT = 720;
	int WIDTH1 = WIDTH, HEIGHT1 = HEIGHT;
	int x = 0, y = 0, ppp = -1;
	int bgw, bgh;
	
	Mix_Chunk* Sound = NULL;
	Mix_Music* fon[5];
	for (int i = 0; i < 5; i++) fon[i] = NULL;
	SDL_Texture* volumeICON;
	bool onstart = false, onsettings = false, onexit = false, onback = false, fullscreen = false, volumechange[2] = { false, false }, onmute = false, ontheme[3] = { false,false,false };
	char menu = 0;
	TTF_Init();
	Mix_Init(MIX_INIT_MP3);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
	IMG_Init(IMG_INIT_PNG);
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Window* window = SDL_CreateWindow(u8"Супер Мега Сапёр",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
	SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
	SDL_Cursor* cursor = NULL;
	if (!loadMedia(renderer, &bgImg, &resource, &volumeICON, &Sound, &*fon, theme,cursor)) return 0;
	if (muted) {
		Mix_Volume(-1, 0);
		Mix_VolumeMusic(0);
	}
	else {
		Mix_VolumeMusic(volume[0]);
		Mix_VolumeChunk(Sound, volume[1]);
	}
	SDL_QueryTexture(bgImg, NULL, NULL, &bgw, &bgh);
	double aspratio = (double)bgw / bgh;
	
	SDL_Rect start = { WIDTH / 2 - 160, HEIGHT / 2 - 120, 320, 80 },
		settings = { WIDTH / 2 - 160, HEIGHT / 2 - 20, 320, 80 },
		off = { WIDTH / 2 - 160, HEIGHT / 2 + 80, 320, 80 },
		back = { WIDTH - 160, HEIGHT - 60, 160, 60 };
	SDL_Rect format = { 0,0,bgw,bgh };
	SDL_Rect volumeSQ[2], volumeP[2], volumeNUM[2], volumeBG = {WIDTH/2 - 200,HEIGHT/10 - 40, 400,130},musicvolume,soundvolume, themeField = {volumeBG.x,HEIGHT/10 + 150, 400,180}, themepick[3], themePick = {WIDTH/2 - 80,themeField.y + 5,160,40};
	SDL_Rect MuteSQ = { 10,10,64,64 },MuteSQ1;
	SDL_Rect mute[2] = { { 0,0,256,256 }, { 256,0,256,256 } };
	for (int i = 0; i < 2; i++) {
		volumeSQ[i] = { WIDTH/2 - 96,HEIGHT/10 + 60*i,128,10 };
		volumeP[i] = { WIDTH/2 - 96,HEIGHT/10 + 60 * i,20,10 };
		volumeNUM[i] = { volumeSQ[i].x + volumeSQ[i].w + 10, volumeSQ[i].y - 15, 2 * 20,40 };
	}
	for (int i = 0; i < 3; i++) {
		themepick[i] = { themeField.x + 10 + i * 130, themeField.y + 45,120,120 };
	}
	MuteSQ1 = { volumeSQ[0].x - 74, volumeSQ[0].y,64,64 };
	musicvolume = {volumeSQ[0].x, volumeSQ[0].y - 40,180,30};
	soundvolume = { volumeSQ[1].x, volumeSQ[1].y - 40,180,30 };
	SDL_Texture* Green = loadTexture(renderer, "Res/1.png");
	SDL_SetTextureColorMod(Green, 50, 165, 50);
	SDL_SetTextureBlendMode(Green, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(Green, 100);

	SDL_Texture* Start = renderText(u8"   СТАРТ   ","Res\\Storopia.ttf", { 0,0,0,0 }, WIDTH / 5,renderer);
	SDL_Texture* Settings = renderText(u8"НАСТРОЙКИ", "Res\\Storopia.ttf", { 0,0,0,0 }, WIDTH / 5, renderer);
	SDL_Texture* Exit = renderText(u8"   ВЫХОД   ", "Res\\Storopia.ttf", { 0,0,0,0 }, WIDTH / 5, renderer);
	SDL_Texture* Back = renderText(u8"НАЗАД", "Res\\Storopia.ttf", { 0,0,0,0 }, WIDTH / 5, renderer);
	SDL_Texture* ThemePick = renderText(u8"Выбор темы", "Res\\Storopia.ttf", { 0,0,0,0 }, WIDTH / 5, renderer);
	SDL_Texture* MusicVolume = renderText(u8"Громкость музыки", "Res\\Storopia.ttf", { 0,0,0,0 }, WIDTH / 5, renderer);
	SDL_Texture* SoundVolume = renderText(u8"Громкость звуков", "Res\\Storopia.ttf", { 0,0,0,0 }, WIDTH / 5, renderer);
	SDL_Texture* themes[3];
	themes[0] = loadTexture(renderer, "Res\\Default.png");
	themes[1] = loadTexture(renderer, "Res\\Morgen.png");
	themes[2] = loadTexture(renderer, "Res\\Ezzhi.png");
	SDL_Rect themesFormat[3];
	for (int i = 0; i < 3; i++) {
		int wid, hei, min;
		SDL_QueryTexture(themes[i], NULL, NULL, &wid, &hei);
		min = wid < hei ? wid : hei;
		themesFormat[i] = { (wid - min) / 2,(hei - min) / 2 ,min,min };
	}
	SDL_Texture* Volume[2];
	srand(SDL_GetTicks());
	for (int i = 0; i < 2; i++) {
		std::string mama = std::to_string(int(volume[i] / 1.28f));
		volumeP[i].w = volume[i];
		Volume[i] = renderText(mama, "Res\\Storopia.ttf", { 0,0,0,0 }, WIDTH / 5, renderer);
		volumeNUM[i].w = (int)mama.length() * 20;
		volumechange[i] = true;
		for (int j = 0; j < mama.length(); j++) {
			if (mama[j] == '1') {
				volumeNUM[i].w -= 10;
			}
		}
	}
	SDL_SetWindowMinimumSize(window, 480, 480);
	SDL_Texture* white = loadTexture(renderer, "Res/1.png");
	SDL_SetTextureBlendMode(white, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(white, 150);
	SDL_Texture* Red = loadTexture(renderer, "Res/1.png");
	SDL_SetTextureColorMod(Red, 165, 50, 50);
	SDL_SetTextureBlendMode(Red, SDL_BLENDMODE_BLEND);
	SDL_SetTextureAlphaMod(Red, 150);

	format.x = 0;
	format.w = bgw;
	format.h = HEIGHT * (float(bgw) / WIDTH);
	format.y = (bgh - format.h) / 2;


	while (true) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case(SDL_MOUSEMOTION): {
					x = e.button.x;
					y = e.button.y;
					if (menu == 0) {
						if (InRect(x, y, MuteSQ)) onmute = true;
						else onmute = false;
					}
					else {
						if (InRect(x, y, MuteSQ1)) onmute = true;
						else onmute = false;
					}
					switch (menu) {
					case 0: {
						if (InRect(x, y, start)) onstart = true;
						else onstart = false;
						if (InRect(x, y, settings)) onsettings = true;
						else onsettings = false;
						if (InRect(x, y, off)) onexit = true;
						else onexit = false;
						
						break;
					}
					case 1: {
						for (int i = 0; i < 3; i++)
							if (InRect(x, y, themepick[i])) ontheme[i] = true;
							else ontheme[i] = false;
						if (ppp == -1) {
							for (int i = 0; i < 2; i++) {
								if (volumechange[i]) {
									ppp = i;
								}
							}
						}
						else {
							volume[ppp] = x - volumeSQ[ppp].x;
							volume[ppp] = volume[ppp] < 0 ? 0 : (volume[ppp] > 128 ? 128 : volume[ppp]);
							if (!muted) {
								if(ppp==1) Mix_Volume(-1, int(volume[1] / 1.28f)*1.28f);
								else if (ppp==0) Mix_VolumeMusic(int(volume[0] / 1.28f) * 1.28f);
							}
							std::string mama = std::to_string(int(volume[ppp] / 1.28f) );
							volumeP[ppp].w = volume[ppp];
							SDL_DestroyTexture(Volume[ppp]);
							Volume[ppp] = renderText(mama, "Res\\Storopia.ttf", { 0,0,0,0 }, WIDTH / 5, renderer);
							volumeNUM[ppp].w = (int)mama.length() * 20;
							for (int i = 0; i < mama.length(); i++) {
								if (mama[i] == '1') {
									volumeNUM[ppp].w -= 10;
								}
							}
						}
						if (InRect(x, y, back)) onback = true;
						else onback = false;
						break;
					}
					}
					break;
				}
			case(SDL_MOUSEBUTTONDOWN): {
					x = e.button.x;
					y = e.button.y;
					if (menu == 0) {
						if (InRect(x, y, MuteSQ)) {
							muted = !muted;
							if (!muted) {
								Mix_Volume(-1, volume[1]);
								Mix_VolumeMusic(volume[0]);
							}
							else {
								Mix_Volume(-1, 0);
								Mix_VolumeMusic(0);
							}
						}
					}
					else {
						if (InRect(x, y, MuteSQ1)) {
							muted = !muted;
							if (!muted) {
								Mix_Volume(-1, volume[1]);
								Mix_VolumeMusic(volume[0]);
							}
							else {
								Mix_Volume(-1, 0);
								Mix_VolumeMusic(0);
							}
						}
					}
					
					if (menu == 0) {
						if (InRect(x, y, settings)) menu = 1;
						else if (InRect(x, y, start)) {
							quit = Game(e, renderer, window, Sound, fon, WIDTH, HEIGHT, bgImg, resource, white, Green,Red, Back, aspratio, bgw, bgh, format, fullscreen, SaveFilePath, username, volume,muted, theme, volumeBG, themeField, themesFormat, themepick, ontheme, ThemePick, volumeSQ, Volume, volumeNUM, volumeP, MusicVolume, SoundVolume, musicvolume, soundvolume, onmute, MuteSQ1, volumeICON, themePick, mute, themes,back,cursor);
							if (!quit) {
								onstart = false;
								SDL_GetMouseState(&x, &y);
								SDL_WarpMouseInWindow(window, x, y);
								for (int i = 0; i < 2; i++) {
									volumeSQ[i].x = WIDTH / 2 - 96;
									volumeSQ[i].y = HEIGHT / 10 + 60 * i;
									volumeP[i].x = WIDTH / 2 - 96;
									volumeP[i].y = HEIGHT / 10 + 60 * i;
									volumeNUM[i].x = volumeSQ[i].x + volumeSQ[i].w + 10;
									volumeNUM[i].y = volumeSQ[i].y - 15;
								}
								volumeBG.x = WIDTH / 2 - 200;
								volumeBG.y = HEIGHT / 10 - 40;
								musicvolume.x = volumeSQ[0].x;
								musicvolume.y = volumeSQ[0].y - 40;
								soundvolume.x = volumeSQ[1].x;
								soundvolume.y = volumeSQ[1].y - 40;
								MuteSQ1.x = volumeSQ[0].x - 74;
								MuteSQ1.y = volumeSQ[0].y;
								themeField.x = volumeBG.x;
								themeField.y = HEIGHT / 10 + 150;
								themePick.x = WIDTH / 2 - 80;
								themePick.y = themeField.y + 5;
								for (int i = 0; i < 3; i++) {
									themepick[i].x = themeField.x + 10 + i * 130;
									themepick[i].y = themeField.y + 45;
								}
								if (WIDTH > 1280 && HEIGHT > 720) {
									start = { WIDTH / 2 - int(16 * HEIGHT / 72.f), HEIGHT / 2 - int(HEIGHT / 6.f), int(32 * HEIGHT / 72.f), int(HEIGHT / 9.f) };
									settings = { start.x, HEIGHT / 2 - int(HEIGHT / 36.f), start.w, start.h };
									off = { start.x, HEIGHT / 2 + int(8 * HEIGHT / 72.f), start.w, start.h };
								}
								else {
									start = { WIDTH / 2 - 160, HEIGHT / 2 - 120, 320, 80 };
									settings = { WIDTH / 2 - 160, HEIGHT / 2 - 20, 320, 80 };
									off = { WIDTH / 2 - 160, HEIGHT / 2 + 80, 320, 80 };
								}
								back = { WIDTH - 160, HEIGHT - 60, 160, 60 };
							}
						}
						else if (InRect(x, y, off))quit = true;
					}
					else if (menu == 1) {
						for (int i = 0; i < 3; i++) {
							if (InRect(x, y, themepick[i])) {
								SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_DISABLE);
								SDL_EventState(SDL_MOUSEBUTTONUP, SDL_DISABLE);
								theme = i;
								DestroyMedia(bgImg, resource, volumeICON, Sound, &*fon);
								loadMedia(renderer, &bgImg, &resource, &volumeICON, &Sound, &*fon, theme, cursor);
								SDL_QueryTexture(bgImg, NULL, NULL, &bgw, &bgh);
								double aspratio = (double)bgw / bgh;
								format = { 0,0,bgw,bgh };
								if (float(e.window.data1) / e.window.data2 > aspratio) {
									format.x = 0;
									format.w = bgw;
									format.h = HEIGHT * (float(bgw) / WIDTH);
									format.y = (bgh - format.h) / 2;
								}
								else {
									format.y = 0;
									format.h = bgh;
									format.w = WIDTH * (float(bgh) / HEIGHT);
									format.x = (bgw - format.w) / 2;
								}
								SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_ENABLE);
								SDL_EventState(SDL_MOUSEBUTTONUP, SDL_ENABLE);
							}
						}
						if (InRect(x, y, back)) {
							menu = 0; onback = false;
							break;
						}
						for (int i = 0; i < 2; i++) {
							if (InRect(x, y, volumeSQ[i])) {
								volume[i] = x - volumeSQ[i].x;
								if (!muted) {
									if (i == 1) Mix_Volume(-1, int(volume[1] / 1.28f) * 1.28f);
									else if (i == 0) Mix_VolumeMusic(int(volume[0] / 1.28f) * 1.28f);
								}
								std::string mama = std::to_string(int(volume[i] / 1.28f));
								SDL_DestroyTexture(Volume[i]);
								volumeP[i].w = volume[i];
								Volume[i] = renderText(mama, "Res\\Storopia.ttf", { 0,0,0,0 }, WIDTH / 5, renderer);
								volumeNUM[i].w = (int)mama.length() * 20;
								volumechange[i] = true;
								for (int j = 0; j < mama.length(); j++) {
									if (mama[j] == '1') {
										volumeNUM[i].w -= 10;
									}
								}
							}
							else volumechange[i] = false;
						}
					}
					break;
				}
			case(SDL_MOUSEBUTTONUP): {
				if (menu == 1) {
					for (int i = 0; i < 2; i++) {
						volumechange[i] = false;
					}
					ppp = -1;
				}
			}
			case (SDL_KEYDOWN): {
					switch (e.key.keysym.scancode) {
					case SDL_SCANCODE_F: {
						fullscreen = !fullscreen;
						if (fullscreen) {
							SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
						}
						else {
							SDL_SetWindowFullscreen(window, SDL_FALSE);
						}
						break;
					}
					case SDL_SCANCODE_ESCAPE: {
						if (menu == 1) {
							onmute = false;
							SDL_GetMouseState(&x, &y);
							SDL_WarpMouseInWindow(window, x, y);
							menu = 0;
						}
						break;
					}
					default:break;
					}
					break;
				}
			case(SDL_WINDOWEVENT): {
					if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
						WIDTH = e.window.data1; HEIGHT = e.window.data2;
						if (float(e.window.data1) / e.window.data2 > aspratio) {
							format.x = 0;
							format.w = bgw;
							format.h = HEIGHT * (float(bgw) / WIDTH);
							format.y = (bgh - format.h) / 2;
						}
						else {
							format.y = 0;
							format.h = bgh;
							format.w = WIDTH * (float(bgh) / HEIGHT);
							format.x = (bgw - format.w) / 2;
						}
						for (int i = 0; i < 2; i++) {
							volumeSQ[i].x = WIDTH / 2 - 96;
							volumeSQ[i].y = HEIGHT / 10 + 60 * i;
							volumeP[i].x = WIDTH / 2 - 96;
							volumeP[i].y = HEIGHT / 10 + 60 * i;
							volumeNUM[i].x = volumeSQ[i].x + volumeSQ[i].w + 10;
							volumeNUM[i].y = volumeSQ[i].y - 15;
						}
						volumeBG.x = WIDTH / 2 - 200;
						volumeBG.y = HEIGHT / 10 - 40;
						musicvolume.x = volumeSQ[0].x;
						musicvolume.y = volumeSQ[0].y - 40;
						soundvolume.x = volumeSQ[1].x;
						soundvolume.y = volumeSQ[1].y - 40;
						MuteSQ1.x = volumeSQ[0].x - 74;
						MuteSQ1.y = volumeSQ[0].y;
						themeField.x = volumeBG.x;
						themeField.y = HEIGHT / 10 + 150;
						themePick.x = WIDTH / 2 - 80;
						themePick.y = themeField.y + 5;
						for (int i = 0; i < 3; i++) {
							themepick[i].x = themeField.x + 10 + i * 130;
							themepick[i].y = themeField.y + 45;
						}
						if (WIDTH > 1280 && HEIGHT > 720) {
							start = { WIDTH / 2 - int(16 * HEIGHT / 72.f), HEIGHT / 2 - int(HEIGHT / 6.f), int(32 * HEIGHT / 72.f), int(HEIGHT / 9.f) };
							settings = { start.x, HEIGHT / 2 - int(HEIGHT / 36.f), start.w, start.h };
							off = { start.x, HEIGHT / 2 + int(8 * HEIGHT / 72.f), start.w, start.h };
							back = { WIDTH - int(HEIGHT / 6.f), HEIGHT - int(HEIGHT / 16.f), int(HEIGHT / 6.f), int(HEIGHT / 16.f) };
						}
						else {
							start = { WIDTH / 2 - 160, HEIGHT / 2 - 120, 320, 80 };
							settings = { WIDTH / 2 - 160, HEIGHT / 2 - 20, 320, 80 };
							off = { WIDTH / 2 - 160, HEIGHT / 2 + 80, 320, 80 };
							back = { WIDTH - 160, HEIGHT - 60, 160, 60 };
						}
					}
					break;
				}
			case(SDL_QUIT): {
					quit = true;
					break;
				}
			default:break;
			}
			
		}
		if (quit)break;
		if(!Mix_PlayingMusic()) Play_Music(fon, theme);
		SDL_RenderCopy(renderer, bgImg, &format, NULL);
		
		switch (menu) {
		case 0: {
			if (onmute) {
				SDL_SetTextureAlphaMod(white, 190);
				SDL_RenderCopy(renderer, white, NULL, &MuteSQ);
				SDL_SetTextureAlphaMod(white, 150);
			}
			else SDL_RenderCopy(renderer, white, NULL, &MuteSQ);
			SDL_RenderCopy(renderer, volumeICON, &mute[muted], &MuteSQ);
			SDL_RenderDrawRect(renderer, &MuteSQ);
			
			if (onstart) {
				SDL_SetTextureAlphaMod(white, 190);
				SDL_RenderCopy(renderer, white, NULL, &start);
				SDL_SetTextureAlphaMod(white, 150);
			}
			else SDL_RenderCopy(renderer, white, NULL, &start);
			if (onsettings) {
				SDL_SetTextureAlphaMod(white, 190);
				SDL_RenderCopy(renderer, white, NULL, &settings);
				SDL_SetTextureAlphaMod(white, 150);
			}
			else SDL_RenderCopy(renderer, white, NULL, &settings);
			if (onexit) {
				SDL_SetTextureAlphaMod(white, 190);
				SDL_RenderCopy(renderer, white, NULL, &off);
				SDL_SetTextureAlphaMod(white, 150);
			}
			else {
				SDL_RenderCopy(renderer, white, NULL, &off);
			}

			SDL_RenderCopy(renderer, Start, NULL, &start);
			SDL_RenderCopy(renderer, Settings, NULL, &settings);
			SDL_RenderCopy(renderer, Exit, NULL, &off);

			SDL_RenderDrawRect(renderer, &start);
			SDL_RenderDrawRect(renderer, &settings);
			SDL_RenderDrawRect(renderer, &off);
			break;
		}
		case 1: {
			SDL_RenderDrawRect(renderer, &volumeBG);
			SDL_RenderCopy(renderer, white, NULL, &volumeBG);
			SDL_RenderCopy(renderer, white, NULL, &themeField);
			for (int i = 0; i < 3; i++) {
				SDL_RenderCopy(renderer, themes[i], &themesFormat[i], &themepick[i]);
				if (ontheme[i])SDL_RenderCopy(renderer, white, NULL, &themepick[i]);
			}
			SDL_RenderCopy(renderer, ThemePick, NULL, &themePick);
			SDL_RenderDrawRects(renderer, themepick, 3);
			DrawBoldRect(renderer, themepick[theme]);
			SDL_RenderDrawRects(renderer, volumeSQ, 3);
			for (int i = 0; i < 2; i++) {
				SDL_RenderCopy(renderer, Volume[i], NULL, &volumeNUM[i]);
				SDL_RenderCopy(renderer, Green, NULL, &volumeP[i]);
			}
			onsettings = false;
			if (onback) {
				SDL_SetTextureAlphaMod(white, 240);
				SDL_RenderCopy(renderer, white, NULL, &back);
				SDL_SetTextureAlphaMod(white, 150);
			}
			else{
				SDL_RenderCopy(renderer, white, NULL, &back);
			}
			SDL_RenderCopy(renderer, MusicVolume, NULL, &musicvolume);
			SDL_RenderCopy(renderer, SoundVolume, NULL, &soundvolume);

			if (onmute) {
				SDL_RenderCopy(renderer, white, NULL, &MuteSQ1);
			}
			SDL_RenderCopy(renderer, volumeICON, &mute[muted], &MuteSQ1);
			SDL_RenderDrawRect(renderer, &MuteSQ1);

			SDL_RenderCopy(renderer, Back, NULL, &back);
			SDL_RenderDrawRect(renderer, &back);
			break;
		}
		default:break;
		}


		SDL_RenderPresent(renderer);
		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
		
	}
TTF_Quit();
IMG_Quit();
DestroyMedia(bgImg, resource, volumeICON, Sound, fon);
for (int i = 0; i < 2; i++) {
	SDL_DestroyTexture(Volume[i]);
	SDL_DestroyTexture(themes[i]);
}
SDL_DestroyTexture(themes[2]);
SDL_DestroyTexture(*themes);
SDL_DestroyTexture(*Volume);
SDL_DestroyTexture(Green);
SDL_DestroyTexture(Red);
SDL_DestroyTexture(white);
SDL_DestroyTexture(Start);
SDL_DestroyTexture(Settings);
SDL_DestroyTexture(Exit);
SDL_DestroyTexture(MusicVolume);
SDL_DestroyTexture(SoundVolume);
SDL_DestroyTexture(ThemePick);
SDL_DestroyRenderer(renderer);
SDL_DestroyWindow(window);
if (!exists_test(SaveFilePath)) {
	createsavefile(SaveFilePath, username);
}
std::ifstream savefile;
savefile.open(SaveFilePath, ios::binary);
std::ofstream tmp;
tmp.open(SaveFilePath+'e', ios::binary);
std::string num;
std::string line;
getline(savefile, line); tmp << line<<endl;
getline(savefile, line); tmp << "THEME "<< int(theme) << endl;
getline(savefile, line); tmp << "MUTED " << muted << endl;
for (int j = 0; j < 2; j++) {
	getline(savefile, line);
	num = std::to_string(volume[j]);
	int len = 3 - num.length();
	for (int i = 0; i < len; i++) {
		num = '0' + num;
	}
	num = "VOLUME" + to_string(j) + ' ' + num + '\n';
	tmp << num;
}
while (getline(savefile, line))tmp << line << endl;
tmp.close();
savefile.close();
std::remove(SaveFilePath.c_str());
std::rename((SaveFilePath + 'e').c_str(), SaveFilePath.c_str());
SDL_Quit();
return 0;
}

bool loadMedia(SDL_Renderer* renderer, SDL_Texture **img, SDL_Texture **rsrc,SDL_Texture ** vlm, Mix_Chunk** Sound, Mix_Music **fon, char ch, SDL_Cursor* cursor) {
	*vlm = loadTexture(renderer, "Res\\volume.png");
	SDL_Surface* surface = NULL;
	if (Mix_PlayingMusic())Mix_HaltMusic();
	switch (ch) {
	case 0: {
		*Sound = Mix_LoadWAV("Res\\Default.wav");
		for (int i = 0; i < 5; i++) {
			string str = "Res\\Default (";
			str += to_string(int(i + 1));
			str += ").mp3";
			fon[i] = Mix_LoadMUS(str.c_str());
		}
		*img = loadTexture(renderer,"Res\\Default.png");
		*rsrc = loadTexture(renderer, "Res\\DefaultRSRC.png");
		surface = SDL_LoadBMP("Res\\DefaultCur.bmp");
		break;
	}
	case 1: {
		*Sound = Mix_LoadWAV("Res\\Morgen.wav");
		for (int i = 0; i < 5; i++) {
			string str = "Res\\Morgen (";
			str += to_string(int(i + 1));
			str += ").mp3";
			fon[i] = Mix_LoadMUS(str.c_str());
		}
		*img = loadTexture(renderer, "Res\\Morgen.png");
		*rsrc = loadTexture(renderer, "Res\\MorgenRSRC.png");
		surface = SDL_LoadBMP("Res\\MorgenCur.bmp");
		break;
	}
	case 2: {
		*Sound = Mix_LoadWAV("Res\\Default.wav");
		for (int i = 0; i < 3; i++) {
			string str = "Res\\Ezzhi (";
			str += to_string(int(i + 1));
			str += ").mp3";
			fon[i] = Mix_LoadMUS(str.c_str());
		}
		for (int i = 3; i < 5; i++)fon[i] = NULL;
		*img = loadTexture(renderer, "Res\\Ezzhi.png");
		*rsrc = loadTexture(renderer, "Res\\EzzhiRSRC.png");
		surface = SDL_LoadBMP("Res\\MorgenCur.bmp");
		break;
	}
	default:return 0;
	}
	SDL_FreeCursor(cursor);
	cursor = SDL_CreateColorCursor(surface, 0, 0);
	SDL_SetCursor(cursor);
	SDL_FreeSurface(surface);
	return 1;
}
SDL_Texture* loadTexture(SDL_Renderer *renderer, std::string path)
{
	SDL_Texture* texture = NULL;
	SDL_Surface* surface = IMG_Load(path.c_str());
	if (surface == NULL)
	{
		printf("Unable to load image %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
	}
	else
	{
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_FreeSurface(surface);
	}
	return texture;
}
bool InRect(int& x, int& y, SDL_Rect& Rect) {
	return (Rect.w + Rect.x >= x && Rect.x <= x && Rect.h + Rect.y >= y && Rect.y <= y);
}
SDL_Texture* renderText(const std::string& message, const std::string& fontFile, SDL_Color color, int fontSize, SDL_Renderer* renderer)
{
	TTF_Font* font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr) {
		return nullptr;
	}
	SDL_Surface* surf = TTF_RenderUTF8_Blended(font, message.c_str(), color);
	if (surf == nullptr) {
		TTF_CloseFont(font);
		return nullptr;
	}
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr) {
		return nullptr;
	}
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
}
void DestroyMedia(SDL_Texture* img, SDL_Texture* rsrc, SDL_Texture* vlm, Mix_Chunk* Sound, Mix_Music** fon) {
	SDL_DestroyTexture(vlm);
	Mix_FreeChunk(Sound);
	for (int i = 0; i < 5; i++) {
		if(fon[i] != NULL) Mix_FreeMusic(fon[i]);
	}
	SDL_DestroyTexture(img);
	SDL_DestroyTexture(rsrc);
}
void Play_Music(Mix_Music** fon,char theme) {
	switch (theme) {
	case 0:
		Mix_PlayMusic(fon[rand() % 5], 1);
		break;
	case 1:
		Mix_PlayMusic(fon[rand() % 5], 1);
		break;
	case 2:
		Mix_PlayMusic(fon[rand() % 3], 1);
		break;
	default:
		break;
	}
}
void DrawBoldRect(SDL_Renderer* renderer,SDL_Rect rect) {
	int j = 2;
	for (int i = 0; i < j; i++) {
		rect.x++;
		rect.w -= 2;
		rect.y++;
		rect.h -= 2;
		SDL_RenderDrawRect(renderer, &rect);
	}
	rect.x -= j;
	rect.w += j*2;
	rect.y -= j;
	rect.h += j*2;
	for (int i = 0; i < j; i++) {
		rect.x--;
		rect.w += 2;
		rect.y--;
		rect.h += 2;
		SDL_RenderDrawRect(renderer, &rect);
	}
}