#include "Header.h"

void isblank(int i, int j, int*& ppp, char**& bombs, char**& field) {
	if (i + 1 < ppp[0]) {
		if (field[i + 1][j] == 9) {
			field[i + 1][j] = bombs[i + 1][j];
			if (bombs[i + 1][j] == 0) {
				isblank(i + 1, j, ppp, bombs, field);
			}
		}
		if (j + 1 < ppp[1]) {
			if (field[i + 1][j + 1] == 9) {
				field[i + 1][j + 1] = bombs[i + 1][j + 1];
				if (bombs[i + 1][j + 1] == 0) {
					isblank(i + 1, j + 1, ppp, bombs, field);
				}
			}
		}
		if (j - 1 >= 0) {
			if (field[i + 1][j - 1] == 9) {
				field[i + 1][j - 1] = bombs[i + 1][j - 1];
				if (bombs[i + 1][j - 1] == 0) {
					isblank(i + 1, j - 1, ppp, bombs, field);
				}
			}
		}
	}
	if (i - 1 >= 0) {
		if(field[i - 1][j] == 9) {
			field[i - 1][j] = bombs[i - 1][j];
			if (bombs[i - 1][j] == 0) {
				isblank(i - 1, j, ppp, bombs, field);
			}
		}
		if (j + 1 < ppp[1]) {
			if (field[i - 1][j + 1] == 9) {
				field[i - 1][j + 1] = bombs[i - 1][j + 1];
				if (bombs[i - 1][j + 1] == 0) {
					isblank(i - 1, j + 1, ppp, bombs, field);
				}
			}
		}
		if (j - 1 >= 0) {
			if (field[i - 1][j - 1] == 9) {
				field[i - 1][j - 1] = bombs[i - 1][j - 1];
				if (bombs[i - 1][j - 1] == 0) {
					isblank(i - 1, j - 1, ppp, bombs, field);
				}
			}
		}
	}
	if (field[i][j] == 9) {
		field[i][j] = bombs[i][j];
		if (bombs[i][j] == 0) {
			isblank(i, j, ppp, bombs, field);
		}
	}
	if (j + 1 < ppp[1]) {
		if (field[i][j + 1] == 9) {
			field[i][j + 1] = bombs[i][j + 1];
			if (bombs[i][j + 1] == 0) {
				isblank(i, j + 1, ppp, bombs, field);
			}
		}
	}
	if (j - 1 >= 0) {
		if (field[i][j - 1] == 9) {
			field[i][j - 1] = bombs[i][j - 1];
			if (bombs[i][j - 1] == 0) {
				isblank(i, j - 1, ppp, bombs, field);
			}
		}
	}
}

bool savegame(std::string SaveFilePath, const char* username, char l, int ppp[2], char **field, char **bombs, int time) {
	std::ifstream file(SaveFilePath, ios::binary);
	std::ofstream fileout(SaveFilePath + 'e', ios::binary);
	std::string string;
	if (l > 3) return 1;
	for (int i = 0; i < 5 + l; i++) { getline(file, string); fileout << string << endl; }
	getline(file, string);
	bool badfile = false;
	std::string hjhj;
	string = "GAMESAVE" + to_string(int(l)) + ' ';
	for (int i = 0; i < 2; i++) {
		hjhj = to_string(ppp[i]);
		string += hjhj.length() < 2 ? '0' + hjhj: hjhj;
	}
	hjhj = to_string(time);
	for (int i = hjhj.length(); i < 7; i++) {
		hjhj = '0' + hjhj;
	}
	fileout << string<<hjhj;
	for (int i = 0; i < ppp[0];i++) {
		for (int j = 0; j < ppp[1]; j++) {
			fileout << char(field[i][j] + 48);
		}
	}
	for (int i = 0; i < ppp[0]; i++) {
		for (int j = 0; j < ppp[1]; j++) {
			fileout << char(bombs[i][j] + 48);
		}
	}
	fileout << endl;
	for (int i = 5 + l; i < 8; i++) {
		{ getline(file, string); fileout << string << endl; }
	}
	file.close();

	fileout.close();
	if (!badfile) {
		if (std::remove(SaveFilePath.c_str())) badfile = true;
		if (std::rename((SaveFilePath + 'e').c_str(), SaveFilePath.c_str())) badfile = true;
	}
	return badfile;
}

bool checkFileCorrupion(std::string SaveFilePath, const char* username, char l) {
	std::ifstream file(SaveFilePath, ios::binary);
	std::string string,hjhj;
	if (l > 3) return 1;
	bool badfile = false;
	for (int i = 0; i < 5 + l; i++) { if (!getline(file, string))badfile = true; }
	if (!getline(file, string))badfile = true;
	if (!badfile) {
		hjhj = "SAVE" + to_string(int(l));
		if (!string.find(hjhj)) badfile = true;
		if (!badfile) {
			for (int i = 5 + l; i < 8; i++) {
				{ if (!getline(file, string))badfile = true; }
			}
		}
	}
	file.close();
	if (badfile) {
		std::remove(SaveFilePath.c_str());
		std::remove((SaveFilePath + 'e').c_str());
	}
	return badfile;
}

bool deletesave(std::string SaveFilePath, const char* username, char l) {
	std::ifstream file(SaveFilePath, ios::binary);
	std::ofstream fileout(SaveFilePath + 'e', ios::binary);
	std::string string;
	if (l > 3) return 1;
	for (int i = 0; i < 5 + l; i++) { getline(file, string); fileout << string << endl; }
	getline(file, string);
	string = "GAMESAVE" + to_string(int(l)) + ' ';
	fileout << string<<endl;
	for (int i = 5 + l; i < 8; i++) {
		{ getline(file, string); fileout << string << endl; }
	}
	file.close();
	fileout.close();
	bool badfile = false;
	if (std::remove(SaveFilePath.c_str())) badfile = true;
	if (std::rename((SaveFilePath + 'e').c_str(), SaveFilePath.c_str())) badfile = true;
	return badfile;
}

bool readsave(std::string SaveFilePath, const char* username, char l, int ppp[2], char**& field, char**& bombs,int &mines,unsigned int &time, int &rightmines, int &UraPobeda) {
	bool badfile = false;
	std::string string;
	std::string hjhj = "";
	std::ifstream file1(SaveFilePath, ios::binary);
	for (int i = 0; i < 5 + l; i++) getline(file1, string);
	getline(file1, string);
	mines = 0;
	if (string.length() < 21) return 1;
	for (int i = 10; i < 14; i++) {
		if (i % 2 == 0) ppp[i >= 12] = 0;
		ppp[i >= 12] += (string[i] - '0') * (i % 2 ==0?10:1);
	}
	field = (char**)malloc(sizeof(char*) * ppp[0]);
	bombs = (char**)malloc(sizeof(char*) * ppp[0]);
	for (int i = 0; i < ppp[0]; i++) {
		field[i] = (char*)malloc(sizeof(char) * ppp[1]);
		bombs[i] = (char*)malloc(sizeof(char) * ppp[1]);
	}
	int jj = 1000000;
	for (int i = 14; i < 21; i++) {
		time += jj * (string[i] -'0');
		jj /= 10;
	}
	int m = 0, n = 0;
	bool p = true;
	for (int i = 21; i < string.length(); i++) {
		if (p) {
			field[m][n++] = string[i] - 48;
			if (n == ppp[1]) { m++; n = 0; }
			if (m == ppp[0]) { p = false; m = 0; }
		}
		else {
			bombs[m][n++] = string[i] - 48;
			if (string[i] == 57) {
				if (field[m][n - 1] == 10) rightmines++;
				mines++;
			}
			if (n == ppp[1]) { m++; n = 0; }
			if (m == ppp[0]) break;
		}
		if (m == ppp[0] && n == 0) badfile = false;
	}
	for (int i1 = 0; i1 < 1; i1++) {
		for (int i = 0; i < ppp[0]; i++) {
			for (int j = 0; j < ppp[1]; j++) {
				if (field[i][j] >= 0 && field[i][j] < 9) i1++;
			}
		}
		UraPobeda = ppp[0] * ppp[1] - mines - i1;
	}
	file1.close();
	return badfile;
}

bool Game1(SDL_Event& e, SDL_Renderer* renderer, SDL_Window* window, Mix_Chunk* boom, Mix_Music** fon, int& WIDTH, int& HEIGHT, SDL_Texture*& bgImg, SDL_Texture*& resource, SDL_Texture*& white, double& aspratio, int& bgw, int& bgh, SDL_Rect& format, bool& fullscreen, int& mines1, int* pole, char l, std::string SaveFilePath, const char* username, bool load, int* volume, bool& muted, char& theme, SDL_Rect& volumeBG, SDL_Rect& themeField, SDL_Rect themesFormat[3], SDL_Rect themepick[3], bool ontheme[3], SDL_Texture*& ThemePick, SDL_Rect volumeSQ[2], SDL_Texture* Volume[2], SDL_Rect volumeNUM[2], SDL_Texture*& Green, SDL_Rect volumeP[3], SDL_Texture*& MusicVolume, SDL_Texture*& SoundVolume, SDL_Rect& musicvolume, SDL_Rect& soundvolume, bool& onmute, SDL_Rect& MuteSQ1, SDL_Texture*& volumeICON, SDL_Rect& themePick, SDL_Rect mute[2], SDL_Texture* themes[3], SDL_Texture*& Back1, SDL_Cursor*& cursor) {
	int mines = 0, ppp1 = -1;
	bool quit = false, volumechange[2] = { false, false }, onback1 = false;
	unsigned int t = 0, t1 = 0, time = 0, startticks, totalticks, stopticks = 0, second = 0, loadedtime = 0, questtime = NULL;
	int x, y, x1, y1, x2, y2, ij[2] = { -10,-10 }, mmm = 0, tm = 0, flags = mines, rightflags = 0, UraPobeda;
	int* ppp = (int*)malloc(sizeof(int) * 2);
	char** field = NULL, ** bombs = NULL;
	if (load) {
		if (exists_test(SaveFilePath) == false) {
			load = false;
			createsavefile(SaveFilePath, username);
		}
		else
			if (checkFileCorrupion(SaveFilePath, username, l)) {
				createsavefile(SaveFilePath, username);
				load = false;
			}
	}
	if (load) {
		if (readsave(SaveFilePath, username, l, ppp, field, bombs, mines, loadedtime, rightflags, UraPobeda)) {
			load = false;
			deletesave(SaveFilePath, username, l);
		}
		else {
			startticks = SDL_GetTicks() - loadedtime;
		}
	}
	if (!load) {
		switch (l) {
		case 0: {
			ppp[0] = 9;
			ppp[1] = 9;
			break;
		}
		case 1: {
			ppp[0] = 18;
			ppp[1] = 18;
			break;
		}
		case 2: {
			ppp[0] = 30;
			ppp[1] = 30;
			break;
		}
		case 3: {
			ppp[0] = pole[0];
			ppp[1] = pole[1];
			break;
		}
		default: {
			return 1;
		}
		}
		mines = mines1;
		field = (char**)malloc(sizeof(char*) * ppp[0]);
		bombs = (char**)malloc(sizeof(char*) * ppp[0]);
		for (int i = 0; i < ppp[0]; i++) {
			field[i] = (char*)malloc(sizeof(char) * ppp[1]);
			bombs[i] = (char*)malloc(sizeof(char) * ppp[1]);
			for (int j = 0; j < ppp[1]; j++) {
				field[i][j] = 9;
				bombs[i][j] = 0;
			}
		}
		UraPobeda = ppp[0] * ppp[1] - mines;
	}
	flags = mines;
	char menu = 0;
	SDL_Rect square[16];
	init_square(square);

	char muzhik_sost = 0;

	int height1 = (float)HEIGHT / ppp[1];
	int width1 = (float)WIDTH / ppp[0];
	int minim = height1 < width1 ? height1 : width1;
	if (minim < 8) minim = 8;
	else if (minim > 64) minim = 64;
	char bombs_num[4] = { 10,10,10,10 }, time_num[4] = { 10,10,10,10 };
	int u = 0, g = 0;
	tm = 1000;
	for (int i = 0; i < 4; i++) {
		bombs_num[i] = (mines % (tm * 10) - mines % tm) / tm;
		tm /= 10;
	}
	SDL_Rect questionMark = { WIDTH - 32,0,32,32 };
	SDL_Rect questRSRC[2];
	for (int i = 0; i < 2; i++) {
		questRSRC[i] = { 32*6 + 32 * i,32,32,32 };
	}
	SDL_Texture* LOX = renderText(u8"ÂÛ  ÏÐÎÈÃÐÀËÈ", "Res\\Storopia.ttf", { 0,0,0,0 }, HEIGHT/4, renderer);
	SDL_Texture* NotLOX = renderText(u8"ÂÛ  ÂÛÈÃÐÀËÈ", "Res\\Storopia.ttf", { 0,0,0,0 }, HEIGHT / 4, renderer);
	int jkjk = 1200;
	if (WIDTH < 1200) {
		jkjk = WIDTH;
	}
	SDL_Rect LoX = {(WIDTH - jkjk)/2,(HEIGHT - jkjk/6)/2, jkjk,jkjk/6};
	SDL_Rect** GameSQ = (SDL_Rect**)malloc(sizeof(SDL_Rect*)*ppp[0]);
	SDL_Rect GameField = { int((WIDTH - minim * ppp[0]) / 2.f), int((HEIGHT - minim * ppp[1]) / 2.f), minim * ppp[0],minim * ppp[1] };
	SDL_Rect back1 = { WIDTH - 160, HEIGHT - 60, 160, 60 };
	for (int i = 0; i < ppp[0]; i++) {
		GameSQ[i] = (SDL_Rect*)malloc(sizeof(SDL_Rect) * ppp[1]);
		for (int j = 0; j < ppp[1]; j++) {
			GameSQ[i][j] = {GameField.x + i*minim,GameField.y+j*minim,minim,minim};
		}
	}
	SDL_Rect nums[11];
	for (int i = 0; i < 10; i++) {
		nums[i] = { 32 + i * 32,64,32,64 };
	}
	nums[10] = {0,64,32,64};

	SDL_Rect panel_bombs[4], panel_time[4];
	SDL_Rect Clocks = {448,64,32,32}, Bomb = {480,64,32,32};
	SDL_Rect panel_clocks = { WIDTH - WIDTH/20 -43 - 32*4, 16,32,32  }, panel_bomb = { WIDTH/20 + 11 + 32*4,16,32,32 };
	SDL_Rect panel = { WIDTH / 20 + 8,-1,9 * WIDTH / 10 - 16,67 };
	SDL_Rect muzhiki[4];
	for (int i = 0; i < 4;i++) {
		muzhiki[i] = { 256 + 64 * i, 0,64,64 };
		panel_bombs[i] = { WIDTH / 20 + 10 + 32 * i,0,32,64 };
		panel_time[i] = { WIDTH - WIDTH / 20 - 42 - 32 * i,0,32,64 };
	}
	SDL_Rect muzhikplace = { WIDTH / 2 - 32,1,64,64 };
	
	SDL_Rect save = { WIDTH / 2 - 160, HEIGHT / 2 - 120, 320, 80 },
		settings = { WIDTH / 2 - 160, HEIGHT / 2 - 20, 320, 80 },
		back = { WIDTH / 2 - 160, HEIGHT / 2 + 80, 320, 80 };

	SDL_Texture* Save = renderText(u8"ÑÎÕÐÀÍÈÒÜ", "Res\\Storopia.ttf", { 0,0,0,0 }, WIDTH / 5, renderer);
	SDL_Texture* Settings = renderText(u8"ÍÀÑÒÐÎÉÊÈ", "Res\\Storopia.ttf", { 0,0,0,0 }, WIDTH / 5, renderer);
	SDL_Texture* Back = renderText(u8"   Â ÌÅÍÞ   ", "Res\\Storopia.ttf", { 0,0,0,0 }, WIDTH / 5, renderer);
	int** loose = (int**)malloc(sizeof(int*) * mines);
	for (int i = 0; i < mines; i++) loose[i] = (int*)malloc(sizeof(int) * 2);

	bool onsave = false, onsettings = false, onback = false;
	bool drag = false;
	bool mousedown = false;
	bool openmenu = false;
	bool quested = false;
	bool onfield = false;
	bool WasFullScreen = false;
	bool LooseAnimation = false;
	bool alreadyloose = false;
	bool firsttime = true;
	bool realquit = false;
	bool WinAnimation = false;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case(SDL_MOUSEMOTION): {
				x = e.button.x;
				y = e.button.y;
				if (!InRect(x, y, panel) && menu == 0) {
					if (!LooseAnimation) {
						if (drag) {
							for (int i = 0; i < ppp[0]; i++) {
								for (int j = 0; j < ppp[1]; j++) {
									GameSQ[i][j].x += x - x1;
								}
							}
							GameField.x += x - x1;
							x1 = x;
							for (int i = 0; i < ppp[0]; i++) {
								for (int j = 0; j < ppp[1]; j++) {
									GameSQ[i][j].y += y - y1;
								}
							}
							GameField.y += y - y1;
							y1 = y;
							if (GameField.x + GameField.w < 4) {
								GameField.x = 4 - GameField.w;
							}
							else if (GameField.x > WIDTH - 4) {
								GameField.x = WIDTH - 4;
							}
							if (GameField.y + GameField.h < 4) {
								GameField.y = 4 - GameField.h;
							}
							else if (GameField.y > HEIGHT - 4) {
								GameField.y = HEIGHT - 4;
							}
							for (int i = 0; i < ppp[0]; i++) {
								for (int j = 0; j < ppp[1]; j++) {
									GameSQ[i][j].x = GameField.x + i * minim;
									GameSQ[i][j].y = GameField.y + j * minim;
								}
							}
						}
						else if (mousedown && (abs(x - x2) > 10 || abs(y - y2) > 10)) {
							drag = true;
							openmenu = false;
						}
					}
				}else if (menu == 1) {
					onsave = InRect(x, y, save);
					onsettings = InRect(x, y, settings);
					onback = InRect(x, y, back);
				}
				else if (menu == 2) {
				for (int i = 0; i < 3; i++)
					if (InRect(x, y, themepick[i])) ontheme[i] = true;
					else ontheme[i] = false;
				if (ppp1 == -1) {
					for (int i = 0; i < 2; i++) {
						if (volumechange[i]) {
							ppp1 = i;
						}
					}
				}
				else {
					volume[ppp1] = x - volumeSQ[ppp1].x;
					volume[ppp1] = volume[ppp1] < 0 ? 0 : (volume[ppp1] > 128 ? 128 : volume[ppp1]);
					if (!muted) {
						if (ppp1 == 1) Mix_Volume(-1, int(volume[1] / 1.28f) * 1.28f);
						else if (ppp1 == 0) Mix_VolumeMusic(int(volume[0] / 1.28f) * 1.28f);
					}
					std::string mama = std::to_string(int(volume[ppp1] / 1.28f));
					volumeP[ppp1].w = volume[ppp1];
					SDL_DestroyTexture(Volume[ppp1]);
					Volume[ppp1] = renderText(mama, "Res\\Storopia.ttf", { 0,0,0,0 }, WIDTH / 5, renderer);
					volumeNUM[ppp1].w = (int)mama.length() * 20;
					for (int i = 0; i < mama.length(); i++) {
						if (mama[i] == '1') {
							volumeNUM[ppp1].w -= 10;
						}
					}
				}
				if (InRect(x, y, back1)) onback1 = true;
				else onback1 = false;
				}
				break;
			}
			case(SDL_MOUSEBUTTONDOWN): {
				x1 = e.button.x;
				y1 = e.button.y;
				if (menu == 0) {
					if (alreadyloose && e.button.button == SDL_BUTTON_RIGHT) {
						openmenu = true;
					}
					if (!InRect(x1, y1, panel)) {
						if (!alreadyloose) {
							if (InRect(x1, y1, questionMark) && e.button.button == SDL_BUTTON_LEFT) {
								quested = true;
								break;
							}
							if (!InRect(x1, y1, GameField) && e.button.button == SDL_BUTTON_RIGHT) {
								openmenu = true;
							}
							x2 = e.button.x;
							y2 = e.button.y;
							for (int i = 0; i < ppp[0]; i++) {
								for (int j = 0; j < ppp[1]; j++) {
									if (InRect(x1, y1, GameSQ[i][j])) {
										onfield = true;
										ij[0] = i;
										ij[1] = j;
									}
								}
							}
							if (e.button.button == SDL_BUTTON_LEFT) {
								if (!InRect(x1, y1, GameField)) {
									drag = true;
								}
								else {
									mousedown = true;
								}
							}
						}
						else {
							if (e.button.button == SDL_BUTTON_LEFT) {
								drag = true;
							}
						}
					}
					else if (InRect(x1, y1, muzhikplace) && e.button.button == SDL_BUTTON_LEFT) {
						muzhik_sost = 1;
					}
					
				}
				else if (menu == 1) {
					if (e.button.button == SDL_BUTTON_LEFT) {
						if (InRect(x1, y1, save)) {
							if (!firsttime && !LooseAnimation && !WinAnimation && !alreadyloose)
								savegame(SaveFilePath, username, l, ppp, field, bombs, stopticks - startticks);
						}
						if (InRect(x1, y1, settings)) {
							menu = 2;
						}
						if (InRect(x1, y1, back)) {
							if (LooseAnimation) {
								SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_ENABLE);
								SDL_EventState(SDL_MOUSEBUTTONUP, SDL_ENABLE);
								SDL_EventState(SDL_MOUSEWHEEL, SDL_ENABLE);
							}
							quit = true;
						}
					}
					else if(e.button.button == SDL_BUTTON_RIGHT) {
						openmenu = true;
					}
				}
				else if (menu == 2) {
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
					for (int i = 0; i < 3; i++) {
						if (InRect(x, y, themepick[i])) {
							SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_DISABLE);
							SDL_EventState(SDL_MOUSEBUTTONUP, SDL_DISABLE);
							theme = i;
							DestroyMedia(bgImg, resource, volumeICON, boom, &*fon);
							loadMedia(renderer, &bgImg, &resource, &volumeICON, &boom, &*fon, theme, cursor);
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
					if (InRect(x, y, back1)) {
						menu--;
						onback1 = false;
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
				mousedown = false;
				if (quested) {
					quested = false;
					if (InRect(x, y, questionMark)) {
						if (time - questtime >= 30) {
							questtime = time;
							SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
							SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
							SDL_EventState(SDL_MOUSEWHEEL, SDL_IGNORE);
							SDL_EventState(SDL_KEYDOWN, SDL_IGNORE);
							int u1 = 0, u2 = rand() % mines;
							bool quity = false;
							for (int i = 0; i < ppp[0]; i++) {
								for (int j = 0; j < ppp[1]; j++) {
									if (int(field[i][j]) != 10) {
										if (int(bombs[i][j]) == 9) {
											if (u1 >= u2) {
												u2 = field[i][j];
												field[i][j] = 13;
												SDL_RenderCopy(renderer, bgImg, &format, NULL);
												for (int i = 0; i < ppp[0]; i++) {
													for (int j = 0; j < ppp[1]; j++) {
														SDL_RenderCopy(renderer, resource, &square[field[i][j]], &GameSQ[i][j]);
													}
												}
												SDL_RenderDrawRect(renderer, &GameField);
												SDL_RenderCopy(renderer, resource, &questRSRC[quested], &questionMark);
												SDL_RenderCopy(renderer, white, NULL, &panel);
												SDL_RenderDrawRect(renderer, &panel);
												SDL_RenderCopy(renderer, resource, &muzhiki[muzhik_sost], &muzhikplace);
												for (int i = 0; i < 4; i++) {
													SDL_RenderCopy(renderer, resource, &nums[bombs_num[i]], &panel_bombs[i]);
													SDL_RenderCopy(renderer, resource, &nums[time_num[i]], &panel_time[i]);
												}
												SDL_RenderCopy(renderer, resource, &Clocks, &panel_clocks);
												SDL_RenderCopy(renderer, resource, &Bomb, &panel_bomb);
												field[i][j] = u2;
												SDL_RenderPresent(renderer);
												SDL_Delay(200);
												quity = true;
												break;
											}
											else {
												u1++;
											}
										}
									}
									else {
										u1++;
									}
								}if (quity)break;
							}
							SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_ENABLE);
							SDL_EventState(SDL_MOUSEBUTTONUP, SDL_ENABLE);
							SDL_EventState(SDL_MOUSEWHEEL, SDL_ENABLE);
							SDL_EventState(SDL_KEYDOWN, SDL_ENABLE);
						}
						
					}
				}
				if (menu == 2) {
					for (int i = 0; i < 2; i++) {
						volumechange[i] = false;
					}
					ppp1 = -1;
				}
				if (InRect(x, y, muzhikplace) && e.button.button == SDL_BUTTON_LEFT) {
					muzhik_sost = 0;
				}
				else if (!InRect(x, y, muzhikplace) && e.button.button == SDL_BUTTON_LEFT) {
					if (alreadyloose) {
						if(rightflags == mines)muzhik_sost = 3;
						else muzhik_sost = 2;
					}
					else {
						muzhik_sost = 0;
					}
				}
				if ((e.button.button == SDL_BUTTON_RIGHT && alreadyloose) || (openmenu && !alreadyloose)) {
					openmenu = false;
					if (menu == 0) {
						stopticks = SDL_GetTicks();
						if (LooseAnimation)SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_ENABLE);
						menu = 1;
					}
					else {
						stopticks = SDL_GetTicks() - stopticks;
						if (LooseAnimation)SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_DISABLE);
						menu--;
						for (int i = 0; i < 2; i++) {
							volumechange[i] = false;
						}
						ppp1 = -1;
					}
					SDL_GetMouseState(&x, &y);
					SDL_WarpMouseInWindow(window, x, y);
				}
				else if (!InRect(x, y, panel)&&menu==0&&InRect(x,y,GameField) && muzhik_sost == 0) {
					if (!alreadyloose) {
						if (!drag && onfield) {
							if (e.button.button == SDL_BUTTON_LEFT) {
								if (firsttime) {
									second = SDL_GetTicks();
									startticks = second;
									firsttime = false;
									if (!load) {
										srand(SDL_GetTicks());
										if (mines > ppp[0] * ppp[1] / 2) {
											mmm = ppp[0] * ppp[1] - mines;
											for (int i = 0; i < ppp[0]; i++) {
												for (int j = 0; j < ppp[1]; j++) {
													bombs[i][j] = 9;
												}
											}
											bombs[ij[0]][ij[1]] = 0;
											mmm--;
											while (mmm > 0) {
												int i1 = rand() % ppp[0], j1 = rand() % ppp[1];
												if (bombs[i1][j1] == 9) {
													bombs[i1][j1] = 0;
													mmm--;
												}
											}
										}
										else {
											while (mmm < mines) {
												int i1 = rand() % ppp[0], j1 = rand() % ppp[1];
												if (bombs[i1][j1] != 9 && (i1 != ij[0] && j1 != ij[1])) {
													bombs[i1][j1] = 9;
													mmm++;
												}
											}
										}
										for (int i1 = 0; i1 < ppp[0]; i1++) {
											for (int j1 = 0; j1 < ppp[1]; j1++) {
												if (bombs[i1][j1] >= 9) {
													if (i1 - 1 >= 0 && j1 - 1 >= 0)bombs[i1 - 1][j1 - 1]++;
													if (i1 - 1 >= 0)bombs[i1 - 1][j1]++;
													if (j1 - 1 >= 0)bombs[i1][j1 - 1]++;
													if (i1 + 1 < ppp[0])bombs[i1 + 1][j1]++;
													if (j1 + 1 < ppp[1])bombs[i1][j1 + 1]++;
													if (i1 - 1 >= 0 && j1 + 1 < ppp[1])bombs[i1 - 1][j1 + 1]++;
													if (i1 + 1 < ppp[0] && j1 - 1 >= 0)bombs[i1 + 1][j1 - 1]++;
													if (i1 + 1 < ppp[0] && j1 + 1 < ppp[1])bombs[i1 + 1][j1 + 1]++;
												}
											}
										}
										for (int i1 = 0; i1 < ppp[0]; i1++) {
											for (int j1 = 0; j1 < ppp[1]; j1++) {
												if (bombs[i1][j1] >= 9) {
													bombs[i1][j1] = 9;
												}
											}
										}
									}
								}
								if (bombs[ij[0]][ij[1]] > 0 && bombs[ij[0]][ij[1]] < 9) {
									UraPobeda--;
									field[ij[0]][ij[1]] = bombs[ij[0]][ij[1]];
								}
								else if (bombs[ij[0]][ij[1]] == 0) {
									isblank(ij[0], ij[1], ppp, bombs, field);
									for (int i1 = 0; i1 < 1; i1++) {
										for (int i = 0; i < ppp[0]; i++) {
											for (int j = 0; j < ppp[1]; j++) {
												if (field[i][j] >= 0 && field[i][j] < 9) i1++;
											}
										}
										UraPobeda = ppp[0] * ppp[1] - mines - i1;
									}
								}
								else if (bombs[ij[0]][ij[1]] == 9) {
									int k = 0;
									deletesave(SaveFilePath, username, l);
									loose[k][0] = ij[0];
									loose[k++][1] = ij[1];
									for (int i = 0; i < ppp[0]; i++) {
										for (int j = 0; j < ppp[1]; j++) {
											if (bombs[i][j] == 9 && !(i == ij[0] && j == ij[1])) {
												if (field[i][j] == 10) {
													field[i][j] = 13;
												}
												else {
													loose[k][0] = i;
													loose[k++][1] = j;
												}
											}
										}
									}
									muzhik_sost = 2;
									totalticks = SDL_GetTicks();
									LooseAnimation = true;
								}
							}
							else if (e.button.button == SDL_BUTTON_RIGHT && ((!firsttime) || load)) {
								firsttime = false;
								if (field[ij[0]][ij[1]] == 9) {
									tm = 1000;
									if (flags >= 1) {
										flags--;
										if (bombs[ij[0]][ij[1]] == 9) {
											rightflags++;
										}
										for (int i = 0; i < 4; i++) {
											bombs_num[i] = (flags % (tm * 10) - flags % tm) / tm;
											tm /= 10;
										}
										field[ij[0]][ij[1]] = 10;
									}
								}
								else if (field[ij[0]][ij[1]] == 10) {
									tm = 1000;
									flags++;
									if (bombs[ij[0]][ij[1]] == 9) {
										rightflags--;
									}
									for (int i = 0; i < 4; i++) {
										bombs_num[i] = (flags % (tm * 10) - flags % tm) / tm;
										tm /= 10;
									}
									field[ij[0]][ij[1]] = 9;
								}
							}
							if (rightflags == mines && UraPobeda == 0) {
								muzhik_sost = 3;
								totalticks = SDL_GetTicks();
								alreadyloose = true;
								WinAnimation = true;
							}
						}
					}
				}
				else if (InRect(x, y, muzhikplace) && e.button.button == SDL_BUTTON_LEFT) {
				firsttime = true;
				UraPobeda = ppp[0] * ppp[1] - mines;
				rightflags = 0;
				flags = mines;
				tm = 1000;
				t = 0, t1 = 0, time = 0, startticks, totalticks, stopticks = 0, second = 0, loadedtime = 0, questtime = NULL;
				for (int i = 0; i < 4; i++) {
					bombs_num[i] = (flags % (tm * 10) - flags % tm) / tm;
					tm /= 10;
				}
				mmm = 0;
				for (int i = 0; i < ppp[0]; i++) {
					for (int j = 0; j < ppp[1]; j++) {
						field[i][j] = 9;
						bombs[i][j] = 0;
					}
				}
				for (int i = 0; i < 4; i++) {
					time_num[i] = 10;
				}
				u = 0;
				alreadyloose = false;
				}
				drag = false;
				break;
			}
			case(SDL_MOUSEWHEEL): {
				if (menu == 0) {
					if ((minim + e.wheel.y > 8 || e.wheel.y > 0) && ((minim < WIDTH && minim < HEIGHT) || e.wheel.y < 0)) {
						if (minim < 8) minim = 8;
						e.wheel.y *= minim / 8;
						minim += e.wheel.y;
						if (InRect(x, y, GameField)) {
							GameField.x -= e.wheel.y * ppp[0] * (x - GameField.x) / (float)GameField.w;
							GameField.y -= e.wheel.y * ppp[1] * (y - GameField.y) / (float)GameField.h;
						}
						else {
							GameField.x -= e.wheel.y * ppp[0] / 2.f;
							GameField.y -= e.wheel.y * ppp[1] / 2.f;
						}
						GameField.w = minim * ppp[0];
						GameField.h = minim * ppp[1];
						if (e.wheel.y < 0) {
							if (GameField.x + GameField.w < 4) {
								GameField.x = 4 - GameField.w;
							}
							else if (GameField.x > WIDTH - 4) {
								GameField.x = WIDTH - 4;
							}
							if (GameField.y + GameField.h < 4) {
								GameField.y = 4 - GameField.h;
							}
							else if (GameField.y > HEIGHT - 4) {
								GameField.y = HEIGHT - 4;
							}
						}
						for (int i = 0; i < ppp[0]; i++) {
							for (int j = 0; j < ppp[1]; j++) {
								GameSQ[i][j].x = GameField.x + i * minim;
								GameSQ[i][j].y = GameField.y + j * minim;
								GameSQ[i][j].w += e.wheel.y;
								GameSQ[i][j].h += e.wheel.y;
							}
						}
					}
				}
				break;
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
					if (!LooseAnimation && !WinAnimation) {
						if (menu == 0) {
							stopticks = SDL_GetTicks();
							if (LooseAnimation)SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_ENABLE);
							menu = 1;
						}
						else {
							stopticks = SDL_GetTicks() - stopticks;
							if (LooseAnimation)SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_DISABLE);
							menu--;
							if (menu == 1) {
								for (int i = 0; i < 2; i++) {
									volumechange[i] = false;
								}
								ppp1 = -1;
							}
						}
						SDL_GetMouseState(&x, &y);
						SDL_WarpMouseInWindow(window, x, y);
					}
					break;
				}
				case(SDL_SCANCODE_SPACE): {
					if (LooseAnimation) {
						for (int i = 0; i < ppp[0]; i++) {
							for (int j = 0; j < ppp[1]; j++) {
								if (bombs[i][j] == 9) {
									field[i][j] = 12;
								}
							}
						}
						SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_ENABLE);
						SDL_EventState(SDL_MOUSEBUTTONUP, SDL_ENABLE);
						SDL_EventState(SDL_MOUSEWHEEL, SDL_ENABLE);
						LooseAnimation = false;
						stopticks = 0;
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
					panel = { WIDTH / 20 + 8,-1,9 * WIDTH / 10 - 16,67 };
					muzhikplace = { WIDTH / 2 - 32,1,64,64 };
					for (int i = 0; i < 4; i++) {
						panel_bombs[i] = { WIDTH / 20 + 10 + 32 * i,0,32,64 };
						panel_time[i] = { WIDTH - WIDTH / 20 - 42 - 32 * i,0,32,64 };
					}
					panel_clocks = { WIDTH - WIDTH / 20 - 43 - 32 * 4, 16,32,32 };
					panel_bomb = { WIDTH / 20 + 11 + 32 * 4,16,32,32 };
					if (WIDTH > 1280 && HEIGHT > 720) {
						save = { WIDTH / 2 - int(16 * HEIGHT / 72.f), HEIGHT / 2 - int(HEIGHT / 6.f), int(32 * HEIGHT / 72.f), int(HEIGHT / 9.f) };
						settings = { save.x, HEIGHT / 2 - int(HEIGHT / 36.f), save.w, save.h };
						back = { save.x, HEIGHT / 2 + int(8 * HEIGHT / 72.f), save.w, save.h };
					}
					else {
						save = { WIDTH / 2 - 160, HEIGHT / 2 - 120, 320, 80 };
						settings = { WIDTH / 2 - 160, HEIGHT / 2 - 20, 320, 80 };
						back = { WIDTH / 2 - 160, HEIGHT / 2 + 80, 320, 80 };
					}
					back1 = { WIDTH - 160, HEIGHT - 60, 160, 60 };
					questionMark.x = WIDTH - 32;
					if ((SDL_GetWindowFlags(window) & SDL_WINDOW_FULLSCREEN_DESKTOP) || (SDL_GetWindowFlags(window) & SDL_WINDOW_MAXIMIZED) || WasFullScreen) {
						GameField = { int((WIDTH - minim * ppp[0]) / 2.f), int((HEIGHT - minim * ppp[1]) / 2.f), minim * ppp[0],minim * ppp[1] };
						for (int i = 0; i < ppp[0]; i++) {
							for (int j = 0; j < ppp[1]; j++) {
								GameSQ[i][j] = { GameField.x + i * minim,GameField.y + j * minim,minim,minim };
							}
						}
						WasFullScreen = !WasFullScreen;
					}
					if (GameField.x + GameField.w < 4) {
						GameField.x = 4 - GameField.w;
					}
					else if (GameField.x > WIDTH - 4) {
						GameField.x = WIDTH - 4;
					}
					if (GameField.y + GameField.h < 4) {
						GameField.y = 4 - GameField.h;
					}
					else if (GameField.y > HEIGHT - 4) {
						GameField.y = HEIGHT - 4;
					}
					for (int i = 0; i < ppp[0]; i++) {
						for (int j = 0; j < ppp[1]; j++) {
							GameSQ[i][j].x = GameField.x + i * minim;
							GameSQ[i][j].y = GameField.y + j * minim;
						}
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
				}
				break;
			}
			case(SDL_QUIT): {
				if (LooseAnimation) {
					SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_ENABLE);
					SDL_EventState(SDL_MOUSEBUTTONUP, SDL_ENABLE);
					SDL_EventState(SDL_MOUSEWHEEL, SDL_ENABLE);
				}
				realquit = true;
				quit = true;
				break;
			}
			default:break;
			}
		}
		
		if (!Mix_PlayingMusic()) Play_Music(fon, theme);
		if (LooseAnimation && SDL_GetTicks() - t > (mines > 10 ? 5000 / mines : 300) &&menu==0) {
			t = SDL_GetTicks();
			if (u == 0) {
				SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
				SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
				SDL_EventState(SDL_MOUSEWHEEL, SDL_IGNORE);
				alreadyloose = true;
				if (flags > 1)random_shuffle(&loose[1], &loose[flags - 1]);
			}
			else {
				if (field[loose[u - 1][0]][loose[u - 1][1]] != 13)
				field[loose[u - 1][0]][loose[u - 1][1]] = 12;
			}
			field[loose[u][0]][loose[u][1]] = 11;
			u++;
			if (SDL_GetTicks() - t1 > 200 + ((theme == 0 || theme == 2) * 150)) {
				t1 = SDL_GetTicks();
				Mix_PlayChannel(-1, boom, 0);
			}
			for (int i = 0; i < (mines > 10 ? 500 / mines : 20); i++) {
				int gg1 = GameField.x, gg2 = GameField.y, bg1 = format.x, bg2 = format.y;
				format.x += (rand() % 90 - 45) / 4;
				format.y += (rand() % 90 - 45) / 4;
				GameField.x += (rand() % 32 - 16) / 4;
				GameField.y += (rand() % 32 - 16) / 4;
				for (int i = 0; i < ppp[0]; i++) {
					for (int j = 0; j < ppp[1]; j++) {
						GameSQ[i][j].x = GameField.x + i * minim;
						GameSQ[i][j].y = GameField.y + j * minim;
					}
				}
				SDL_RenderCopy(renderer, bgImg, &format, NULL);
				for (int i = 0; i < ppp[0]; i++) {
					for (int j = 0; j < ppp[1]; j++) {
						SDL_RenderCopy(renderer, resource, &square[field[i][j]], &GameSQ[i][j]);
					}
				}
				SDL_RenderDrawRect(renderer, &GameField);
				SDL_RenderCopy(renderer, white, NULL, &panel);
				SDL_RenderDrawRect(renderer, &panel);
				SDL_RenderCopy(renderer, resource, &questRSRC[quested], &questionMark);
				SDL_RenderCopy(renderer, resource, &muzhiki[2], &muzhikplace);
				for (int i = 0; i < 4; i++) {
					SDL_RenderCopy(renderer, resource, &nums[bombs_num[i]], &panel_bombs[i]);
					SDL_RenderCopy(renderer, resource, &nums[time_num[i]], &panel_time[i]);
				}
				SDL_RenderCopy(renderer, resource, &Clocks, &panel_clocks);
				SDL_RenderCopy(renderer, resource, &Bomb, &panel_bomb);
				SDL_RenderPresent(renderer);
				SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0);
				SDL_RenderClear(renderer);
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
				SDL_Delay(10 + 40 / mines);
				format.x = bg1;
				format.y = bg2;
				GameField.x = gg1;
				GameField.y = gg2;
				for (int i = 0; i < ppp[0]; i++) {
					for (int j = 0; j < ppp[1]; j++) {
						GameSQ[i][j].x = GameField.x + i * minim;
						GameSQ[i][j].y = GameField.y + j * minim;
					}
				}
			}

			if (u == flags) {
				stopticks = 0;
				if (WIDTH < 1200) {
					jkjk = WIDTH;
				}
				LoX = { (WIDTH - jkjk) / 2,(HEIGHT - jkjk / 6) / 2, jkjk,jkjk / 6 };
				if(field[loose[u-1][0]][loose[u-1][1]]!=13)
				field[loose[u - 1][0]][loose[u - 1][1]] = 12;
				LooseAnimation = false;
				SDL_RenderCopy(renderer, bgImg, &format, NULL);
				for (int i = 0; i < ppp[0]; i++) {
					for (int j = 0; j < ppp[1]; j++) {
						SDL_RenderCopy(renderer, resource, &square[field[i][j]], &GameSQ[i][j]);
					}
				}
				SDL_RenderDrawRect(renderer, &GameField);
				SDL_RenderCopy(renderer, white, NULL, &panel);
				SDL_RenderDrawRect(renderer, &panel);
				SDL_RenderCopy(renderer, resource, &muzhiki[2], &muzhikplace);
				SDL_RenderCopy(renderer, resource, &questRSRC[quested], &questionMark);
				for (int i = 0; i < 4; i++) {
					SDL_RenderCopy(renderer, resource, &nums[bombs_num[i]], &panel_bombs[i]);
					SDL_RenderCopy(renderer, resource, &nums[time_num[i]], &panel_time[i]);
				}
				SDL_RenderCopy(renderer, resource, &Clocks, &panel_clocks);
				SDL_RenderCopy(renderer, resource, &Bomb, &panel_bomb);
				SDL_RenderCopy(renderer, white, NULL, &LoX);
				SDL_RenderCopy(renderer, LOX, NULL, &LoX);
				SDL_RenderPresent(renderer);
				SDL_Delay(2000);
				SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_ENABLE);
				SDL_EventState(SDL_MOUSEBUTTONUP, SDL_ENABLE);
				SDL_EventState(SDL_MOUSEWHEEL, SDL_ENABLE);
			}
		}
		else if (WinAnimation) {
			SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_IGNORE);
			SDL_EventState(SDL_MOUSEBUTTONUP, SDL_IGNORE);
			SDL_EventState(SDL_MOUSEWHEEL, SDL_IGNORE);
			if (WIDTH < 1200) {
				jkjk = WIDTH;
			}
			LoX = { (WIDTH - jkjk) / 2,(HEIGHT - jkjk / 6) / 2, jkjk,jkjk / 6 };
			SDL_RenderCopy(renderer, bgImg, &format, NULL);
			for (int i = 0; i < ppp[0]; i++) {
				for (int j = 0; j < ppp[1]; j++) {
					SDL_RenderCopy(renderer, resource, &square[field[i][j]], &GameSQ[i][j]);
				}
			}
			SDL_RenderDrawRect(renderer, &GameField);
			SDL_RenderCopy(renderer, white, NULL, &panel);
			SDL_RenderDrawRect(renderer, &panel);
			SDL_RenderCopy(renderer, resource, &muzhiki[3], &muzhikplace);
			SDL_RenderCopy(renderer, resource, &questRSRC[quested], &questionMark);
			for (int i = 0; i < 4; i++) {
				SDL_RenderCopy(renderer, resource, &nums[bombs_num[i]], &panel_bombs[i]);
				SDL_RenderCopy(renderer, resource, &nums[time_num[i]], &panel_time[i]);
			}
			SDL_RenderCopy(renderer, resource, &Clocks, &panel_clocks);
			SDL_RenderCopy(renderer, resource, &Bomb, &panel_bomb);
			SDL_RenderCopy(renderer, white, NULL, &LoX);
			SDL_RenderCopy(renderer, NotLOX, NULL, &LoX);
			SDL_RenderPresent(renderer);
			SDL_Delay(2000);
			SDL_EventState(SDL_MOUSEBUTTONDOWN, SDL_ENABLE);
			SDL_EventState(SDL_MOUSEBUTTONUP, SDL_ENABLE);
			SDL_EventState(SDL_MOUSEWHEEL, SDL_ENABLE);
			WinAnimation = false;
		}
		else if (!firsttime && SDL_GetTicks() - second > 999 && menu ==0 ) {
			if (stopticks && !alreadyloose) {
				startticks += stopticks;
				stopticks = 0;
			}
			second = SDL_GetTicks();
			if (LooseAnimation || alreadyloose) {
				time = (totalticks - startticks + loadedtime) / 1000;
			}
			else {
				time = (SDL_GetTicks() - startticks + loadedtime) / 1000;
			}
			if (time == 10000) time = 9999;
			tm = 1000;
			for (int i = 3; i >= 0; i--) {
				time_num[i] = (time % (tm * 10) - time % tm) / tm;
				tm /= 10;
			}
		}

		SDL_RenderCopy(renderer, bgImg, &format, NULL);
		if (menu == 0 || menu == 1) {
			for (int i = 0; i < ppp[0]; i++) {
				for (int j = 0; j < ppp[1]; j++) {
					SDL_RenderCopy(renderer, resource, &square[field[i][j]], &GameSQ[i][j]);
				}
			}
			SDL_RenderDrawRect(renderer, &GameField);
			SDL_RenderCopy(renderer, resource,&questRSRC[quested],&questionMark);
			SDL_RenderCopy(renderer, white, NULL, &panel);
			SDL_RenderDrawRect(renderer, &panel);
			SDL_RenderCopy(renderer, resource, &muzhiki[muzhik_sost], &muzhikplace);
			for (int i = 0; i < 4; i++) {
				SDL_RenderCopy(renderer, resource, &nums[bombs_num[i]], &panel_bombs[i]);
				SDL_RenderCopy(renderer, resource, &nums[time_num[i]], &panel_time[i]);
			}
			SDL_RenderCopy(renderer, resource, &Clocks, &panel_clocks);
			SDL_RenderCopy(renderer, resource, &Bomb, &panel_bomb);

			if (menu == 1) {
				SDL_SetTextureColorMod(white, 0, 0, 0);
				SDL_RenderCopy(renderer, white, NULL, NULL);
				SDL_SetTextureColorMod(white, 0xff, 0xff, 0xff);
				if (onsave) {
					SDL_SetTextureAlphaMod(white, 190);
					SDL_RenderCopy(renderer, white, NULL, &save);
					SDL_SetTextureAlphaMod(white, 150);
				}
				else SDL_RenderCopy(renderer, white, NULL, &save);
				if (onsettings) {
					SDL_SetTextureAlphaMod(white, 190);
					SDL_RenderCopy(renderer, white, NULL, &settings);
					SDL_SetTextureAlphaMod(white, 150);
				}
				else SDL_RenderCopy(renderer, white, NULL, &settings);
				if (onback) {
					SDL_SetTextureAlphaMod(white, 190);
					SDL_RenderCopy(renderer, white, NULL, &back);
					SDL_SetTextureAlphaMod(white, 150);
				}
				else {
					SDL_RenderCopy(renderer, white, NULL, &back);
				}
				SDL_RenderCopy(renderer, Save, NULL, &save);
				SDL_RenderCopy(renderer, Settings, NULL, &settings);
				SDL_RenderCopy(renderer, Back, NULL, &back);
			}
		}
		else if (menu == 2) {
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
			if (onback1) {
				SDL_SetTextureAlphaMod(white, 240);
				SDL_RenderCopy(renderer, white, NULL, &back1);
				SDL_SetTextureAlphaMod(white, 150);
			}
			else {
				SDL_RenderCopy(renderer, white, NULL, &back1);
			}
			SDL_RenderCopy(renderer, MusicVolume, NULL, &musicvolume);
			SDL_RenderCopy(renderer, SoundVolume, NULL, &soundvolume);

			if (onmute) {
				SDL_RenderCopy(renderer, white, NULL, &MuteSQ1);
			}
			SDL_RenderCopy(renderer, volumeICON, &mute[muted], &MuteSQ1);
			SDL_RenderDrawRect(renderer, &MuteSQ1);

			SDL_RenderCopy(renderer, Back1, NULL, &back1);
			SDL_RenderDrawRect(renderer, &back1);
		}
		SDL_RenderPresent(renderer);
		SDL_RenderClear(renderer);
	}
	SDL_DestroyTexture(Save);
	SDL_DestroyTexture(Settings);
	SDL_DestroyTexture(Back);
	SDL_DestroyTexture(LOX);
	SDL_DestroyTexture(NotLOX);
	for (int i = 0; i < mines; i++) std::free(loose[i]);
	std::free(loose);
	for (int i = 0; i < ppp[0]; i++) {
		std::free(field[i]);
		std::free(GameSQ[i]);
		std::free(bombs[i]);
	}
	std::free(bombs);
	std::free(GameSQ);
	std::free(field);
	std::free(ppp);

	return realquit;
}
void init_square(SDL_Rect* square) {
	for (int i = 0; i < 16; i++) {
		square[i] = {(i%8)*32,(i/8)*32,32,32};
	}
}
