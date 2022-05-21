#include "Header.h";

void checksave(std::string SaveFilePath, const char* username, bool canload[4]) {
	std::ifstream file(SaveFilePath, ios::binary);
	std::string string;
	for (int i = 0; i < 5; i++) { getline(file, string); }
	for (int i = 0; i < 4; i++) { getline(file, string); if (string.length() < 21) canload[i] = false; else canload[i] = true; }
	file.close();
}

bool Game(SDL_Event& e, SDL_Renderer* renderer, SDL_Window* window, Mix_Chunk* Sound, Mix_Music** fon, int& WIDTH, int& HEIGHT, SDL_Texture*& bgImg, SDL_Texture*& resource, SDL_Texture*& white, SDL_Texture*& Green, SDL_Texture*& Red, SDL_Texture*& Back, double& aspratio, int& bgw, int& bgh, SDL_Rect& format, bool& fullscreen, std::string SaveFilePath, const char* username, int* volume, bool& muted, char& theme, SDL_Rect& volumeBG, SDL_Rect& themeField, SDL_Rect themesFormat[3], SDL_Rect themepick[3], bool ontheme[3], SDL_Texture*& ThemePick, SDL_Rect volumeSQ[2], SDL_Texture* Volume[2], SDL_Rect volumeNUM[2], SDL_Rect volumeP[3], SDL_Texture*& MusicVolume, SDL_Texture*& SoundVolume, SDL_Rect& musicvolume, SDL_Rect& soundvolume, bool& onmute, SDL_Rect& MuteSQ1, SDL_Texture*& volumeICON, SDL_Rect& themePick, SDL_Rect mute[2], SDL_Texture* themes[3], SDL_Rect& back, SDL_Cursor*& cursor){
	SDL_Rect menuSQ[4];
	std::string str, str1;
	int x,y, x1, x2, x3, otstup = 26, pole[2] = { 40,40 }, t,t1, iteration;
	float inersion = 0;
	bool drag = false, Postdrag = false, hru = false, gav = false, canload[4] = {true,true,true,true};
	bool onback = false;
	bool OnMenu[4] = { false, false, false, false };
	bool OnPlus[4] = { false, false, false, false };
	bool OnMinus[4] = { false, false, false, false };
	bool Polechange[4] = { false,false,false,false };
	bool OnStart[4] = { false,false,false,false };
	bool onload[4] = { false,false,false,false };
	int mines[4] = { 10,50,150,400 };

	if (exists_test(SaveFilePath)) {
		bool gffg = true;
		for(int l = 0;l<4;l++)
			if (checkFileCorrupion(SaveFilePath, username, l)) {gffg = false;}
		if (gffg)checksave(SaveFilePath, username, canload);
	}
	else {
		createsavefile(SaveFilePath, username);
	}

	for (int i = 0; i < 4; i++) {
		menuSQ[i] = { 26 + 282 * i,HEIGHT / 20,256 ,HEIGHT - 65 - HEIGHT/20 };
	}
	x3 = menuSQ[0].x;
	if (WIDTH - (menuSQ[3].x + menuSQ[3].w) != menuSQ[0].x) {
		menuSQ[0].x = (WIDTH - (menuSQ[3].x + menuSQ[3].w) + menuSQ[0].x) / 2;
		x3 = menuSQ[0].x;
		for (int i = 1; i < 4; i++) {
			menuSQ[i].x = menuSQ[0].x + 282 * i;
		}
	}
	SDL_Rect menuTXT[4], menuMODE[4], PlusBTN[4], MinusBTN[4], minesNUM[4], loadBTN[4], plus[2] = { {384,96,32,32}, {416,96,32,32} }, minus[2] = { {384,64,32,32}, {416,64,32,32} },startBTN[4];
	for (int i = 0; i < 4; i++) {
		PlusBTN[i] = { menuSQ[i].x, menuSQ[i].y + 130,40,40 };
		loadBTN[i] = { menuSQ[i].x ,menuSQ[i].y + menuSQ[i].h - 180,menuSQ[i].w,80 };
		startBTN[i] = { menuSQ[i].x ,menuSQ[i].y + menuSQ[i].h - 80,menuSQ[i].w,80 };
		MinusBTN[i] = { menuSQ[i].x + 216, menuSQ[i].y + 130,40,40 };
		menuTXT[i] = { menuSQ[i].x, menuSQ[i].y - 18,menuSQ[i].w,80 };
		menuMODE[i] = { menuSQ[i].x, menuSQ[i].y + 52,menuSQ[i].w,80 };
		minesNUM[i] = { menuSQ[i].x + 45, menuSQ[i].y + 130,menuSQ[i].w - 90,40 };
		OnMenu[i] = InRect(e.button.x, e.button.y, menuSQ[i]);
	}

	SDL_Rect PoleCHANGE[4] = {{ menuSQ[3].x, menuSQ[3].y + 48,40,40 }, { menuSQ[3].x, menuSQ[3].y + 88,40,40 }, {menuSQ[3].x + 216, menuSQ[3].y + 48,40,40}, {menuSQ[3].x + 216, menuSQ[3].y + 88,40,40}};
	SDL_Texture* easy = renderText(u8"   Легко   ", "Res\\Storopia.ttf", { 0,0,0,0 }, 100, renderer);
	SDL_Texture* normal = renderText(u8"Нормально", "Res\\Storopia.ttf", { 0,0,0,0 }, 100, renderer);
	SDL_Texture* hard = renderText(u8"  Сложно  ", "Res\\Storopia.ttf", { 0,0,0,0 }, 100, renderer);
	SDL_Texture* easy9x9 = renderText("    9x9    ", "Res\\Storopia.ttf", { 0,0,0,0 }, 100, renderer);
	SDL_Texture* normal18x18 = renderText("   18x18   ", "Res\\Storopia.ttf", { 0,0,0,0 }, 100, renderer);
	SDL_Texture* hard30x30 = renderText("   30x30   ", "Res\\Storopia.ttf", { 0,0,0,0 }, 100, renderer);
	SDL_Texture* start = renderText(u8"  Начать  ", "Res\\Storopia.ttf", { 0,0,0,0 }, 100, renderer);
	SDL_Texture* load = renderText(u8"Загрузить", "Res\\Storopia.ttf", { 0,0,0,0 }, 100, renderer);
	SDL_Texture* custom = renderText("     ...     ", "Res\\Storopia.ttf", { 0,0,0,0 }, 100, renderer);
	std::string Customstr = std::to_string(pole[0]) + 'x' + std::to_string(pole[1]);
	while (Customstr.length() < 11) Customstr = ' ' + Customstr + ' ';
	SDL_Texture* customXxX = renderText(Customstr, "Res\\Storopia.ttf", { 0,0,0,0 }, 100, renderer);
	SDL_Texture* Mines[4];
	for (int i = 0; i < 4; i++) {
		str = std::to_string(mines[i]);
		int u = 3 - str.length();
		if (u > 0) { str1 = u8"Кол-во мин:"; for (int i = 0; i < u; i++) str1 += ' '; str1 += str; }
		else str1 = u8"Кол-во мин:" + str;
		Mines[i] = renderText(str1, "Res\\Storopia.ttf", {0,0,0,0},100, renderer);
	}

	bool quit = false;
	bool firsttime = true;
	while (!quit) {
		while (SDL_PollEvent(&e)) {
			switch (e.type) {
			case(SDL_MOUSEMOTION): {
				x = e.button.x;
				y = e.button.y;
				for (int i = 0; i < 4; i++) {
					OnMenu[i] = InRect(e.button.x, e.button.y, menuSQ[i]);
					OnStart[i] = InRect(e.button.x, e.button.y, startBTN[i]);
					onload[i] = InRect(e.button.x, e.button.y, loadBTN[i]);
				}
				onback = InRect(e.button.x, e.button.y, back);
				if (drag) x2 = e.button.x;
				break;
			}
			case(SDL_MOUSEBUTTONDOWN): {
				x1 = e.button.x;
				x2 = e.button.x;
				drag = true;
				if(!Postdrag)
				for (int i = 0; i < 4; i++) {
					if (InRect(e.button.x, e.button.y, PlusBTN[i])) {
						OnPlus[i] = true;
						hru = true;
						t = SDL_GetTicks();
						t1 = SDL_GetTicks();
						iteration = 10;
						drag = false;
					}
					else if (InRect(e.button.x, e.button.y, MinusBTN[i])) {
						OnMinus[i] = true;
						hru = true;
						t = SDL_GetTicks();
						t1 = SDL_GetTicks();
						iteration = 10;
						drag = false;
					}
					else if (InRect(e.button.x, e.button.y, PoleCHANGE[i])) {
						Polechange[i] = true;
						gav = true;
						t = SDL_GetTicks();
						t1 = SDL_GetTicks();
						drag = false;
					}
					else if (InRect(e.button.x, e.button.y, startBTN[i]) || (InRect(e.button.x, e.button.y, loadBTN[i])&& canload[i])) {
						quit = Game1(e, renderer, window, Sound, fon, WIDTH, HEIGHT, bgImg, resource, white, aspratio, bgw, bgh, format, fullscreen, mines[i], pole, i, SaveFilePath, username, InRect(e.button.x, e.button.y, loadBTN[i]), volume, muted, theme, volumeBG, themeField, themesFormat, themepick,ontheme, ThemePick, volumeSQ, Volume, volumeNUM,Green, volumeP, MusicVolume, SoundVolume, musicvolume, soundvolume, onmute, MuteSQ1, volumeICON, themePick, mute, themes, Back, cursor);
						checksave(SaveFilePath, username, canload);
						if (!quit) {
							drag = false;
							SDL_GetMouseState(&x, &y);
							SDL_WarpMouseInWindow(window, x, y);
							for (int i = 0; i < 4; i++) {
								menuSQ[i] = { 26 + (282) * i,HEIGHT / 20,256 ,HEIGHT - 65 - HEIGHT / 20 };
							}
							if (WIDTH - (menuSQ[3].x + menuSQ[3].w) != menuSQ[0].x) {
								menuSQ[0].x = (WIDTH - (menuSQ[3].x + menuSQ[3].w) + menuSQ[0].x) / 2;
								x3 = menuSQ[0].x;
								for (int i = 1; i < 4; i++) {
									menuSQ[i].x = menuSQ[0].x + 282 * i;
								}
							}
							back = { WIDTH - 160, HEIGHT - 60, 160, 60 };
							for (int i = 0; i < 4; i++) {
								PoleCHANGE[i].x = menuSQ[3].x + (216 * (i >= 2));
								PoleCHANGE[i].y = menuSQ[3].y + 48 + (40 * (i % 2 == 1));
							}
							for (int i = 0; i < 4; i++) {
								startBTN[i] = { menuSQ[i].x ,menuSQ[i].y + menuSQ[i].h - 80,menuSQ[i].w,80 };
								menuTXT[i] = { menuSQ[i].x, menuSQ[i].y - 18,menuSQ[i].w,80 };
								loadBTN[i] = { menuSQ[i].x ,menuSQ[i].y + menuSQ[i].h - 180,menuSQ[i].w,80 };
								menuMODE[i] = { menuSQ[i].x, menuSQ[i].y + 52,menuSQ[i].w,80 };
								PlusBTN[i] = { menuSQ[i].x, menuSQ[i].y + 130,40,40 };
								MinusBTN[i] = { menuSQ[i].x + 216, menuSQ[i].y + 130,40,40 };
								minesNUM[i] = { menuSQ[i].x + 45, menuSQ[i].y + 130,menuSQ[i].w - 90,40 };
							}
						}
					}
				}
				if (InRect(e.button.x, e.button.y, back)) {
					onback = false;
					drag = false;
					for (int i = 0; i < 4; i++) {
						SDL_DestroyTexture(Mines[i]);
					}
					SDL_DestroyTexture(*Mines);
					SDL_DestroyTexture(custom);
					SDL_DestroyTexture(customXxX);
					SDL_DestroyTexture(easy);
					SDL_DestroyTexture(normal);
					SDL_DestroyTexture(hard);
					SDL_DestroyTexture(easy9x9);
					SDL_DestroyTexture(normal18x18);
					SDL_DestroyTexture(hard30x30);
					SDL_DestroyTexture(start);
					return 0;
				}
				break;
			}
			case(SDL_MOUSEBUTTONUP): {
				if (drag) {
					drag = false;
					Postdrag = menuSQ[0].x != x3;
					inersion = menuSQ[0].x -x3;
				}
				else if (hru) {
					for (int i = 0; i < 4; i++) {
						if (OnPlus[i]) {
							mines[i]++;
							OnPlus[i] = false;
						}
						if (OnMinus[i]) {
							mines[i]--;
							OnMinus[i] = false;
						}
						if (mines[i] == 0) mines[i] = 1;
						if (mines[0] == 80) mines[0] = 79;
						else if (mines[1] == 323) mines[0] = 322;
						else if (mines[2] == 899) mines[0] = 898;
						else if (mines[3] == pole[0] * pole[1]-1) mines[3] = pole[0] * pole[1] - 2;
						SDL_DestroyTexture(Mines[i]);
						str = std::to_string(mines[i]);
						int u = 3 - str.length();
						if (u > 0) { str1 = u8"Кол-во мин:"; for (int i = 0; i < u; i++) str1 += ' '; str1 += str; }
						else str1 = u8"Кол-во мин:" + str;
						Mines[i] = renderText(str1, "Res\\Storopia.ttf", { 0,0,0,0 }, 100, renderer);
						hru = false;
					}
				}
				else if (gav) {
					SDL_DestroyTexture(customXxX);
					gav = false;
					for (int i = 0; i < 4; i++) {
						if (Polechange[i]) {
							pole[i / 2] += (i % 2 ? -1 : 1);
							Polechange[i] = false;
						}
					}
					if (pole[0] == 1) pole[0] = 2;
					else if (pole[0] == 100) pole[0] = 99;
					else if (pole[1] == 1) pole[1] = 2;
					else if (pole[1] == 100) pole[1] = 99;
					if (mines[3] >= pole[0] * pole[1]-1){ 
						mines[3] = pole[0] * pole[1] - 2;
						SDL_DestroyTexture(Mines[3]);
						str = std::to_string(mines[3]);
						int u = 3 - str.length();
						if (u > 0) { str1 = u8"Кол-во мин:"; for (int i = 0; i < u; i++) str1 += ' '; str1 += str; }
						else str1 = u8"Кол-во мин:" + str;
						Mines[3] = renderText(str1, "Res\\Storopia.ttf", { 0,0,0,0 }, 100, renderer);
					}
					Customstr = std::to_string(pole[0]) + 'x' + std::to_string(pole[1]);
					while (Customstr.length() < 11) Customstr = ' ' + Customstr + ' ';
					customXxX = renderText(Customstr, "Res\\Storopia.ttf", { 0,0,0,0 }, 100, renderer);

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
					for (int i = 0; i < 4; i++) {
						SDL_DestroyTexture(Mines[i]);
					}
					SDL_DestroyTexture(custom);
					SDL_DestroyTexture(*Mines);
					SDL_DestroyTexture(customXxX);
					SDL_DestroyTexture(easy);
					SDL_DestroyTexture(normal);
					SDL_DestroyTexture(hard);
					SDL_DestroyTexture(easy9x9);
					SDL_DestroyTexture(normal18x18);
					SDL_DestroyTexture(hard30x30);
					SDL_DestroyTexture(start);
					SDL_DestroyTexture(load);
					return 0;
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
					for (int i = 0; i < 4; i++) {
						menuSQ[i] = { 26 + (282) * i,HEIGHT / 20,256 ,HEIGHT - 65 - HEIGHT/20 };
					}
					if (WIDTH - (menuSQ[3].x + menuSQ[3].w) != menuSQ[0].x) {
						menuSQ[0].x = (WIDTH - (menuSQ[3].x + menuSQ[3].w) + menuSQ[0].x) / 2;
						x3 = menuSQ[0].x;
						for (int i = 1; i < 4; i++) {
							menuSQ[i].x = menuSQ[0].x + 282 * i;
						}
					}
					back = { WIDTH - 160, HEIGHT - 60, 160, 60 };
					for (int i = 0; i < 4; i++) {
						PoleCHANGE[i].x = menuSQ[3].x + (216 * (i >= 2));
						PoleCHANGE[i].y = menuSQ[3].y + 48 + (40 * (i %2 ==1));
					}
					for (int i = 0; i < 4; i++) {
						startBTN[i] = { menuSQ[i].x ,menuSQ[i].y + menuSQ[i].h - 80,menuSQ[i].w,80 };
						menuTXT[i] = { menuSQ[i].x, menuSQ[i].y -18,menuSQ[i].w,80 };
						loadBTN[i] = { menuSQ[i].x ,menuSQ[i].y + menuSQ[i].h - 180,menuSQ[i].w,80 };
						menuMODE[i] = { menuSQ[i].x, menuSQ[i].y + 52,menuSQ[i].w,80 };
						PlusBTN[i] = { menuSQ[i].x, menuSQ[i].y + 130,40,40 };
						MinusBTN[i] = { menuSQ[i].x + 216, menuSQ[i].y + 130,40,40 };
						minesNUM[i] = { menuSQ[i].x + 45, menuSQ[i].y + 130,menuSQ[i].w - 90,40 };
					}
				}
				else if (e.window.event == SDL_WINDOWEVENT_LEAVE) {
					if (drag) {
						drag = false;
						Postdrag = menuSQ[0].x != x3;
						inersion = menuSQ[0].x - x3;
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
		if (!Mix_PlayingMusic()) Play_Music(fon, theme);
		if (hru) {
			if (SDL_GetTicks() - t > 300) {
				if (SDL_GetTicks() - t1 > 100) {
					iteration++;
					t1 = SDL_GetTicks();
					for (int i = 0; i < 4; i++) {
						if (OnPlus[i]) {
							mines[i]+=10*(iteration/10);
						}
						if (OnMinus[i]) {
							mines[i]-=10*(iteration/10);
						}
						if (mines[i] <= 0) mines[i] = 1;
						if (mines[0] >= 80) mines[0] = 79;
						else if (mines[1] >= 323) mines[0] = 322;
						else if (mines[2] >= 899) mines[0] = 898;
						else if (mines[3] >= pole[0] * pole[1] - 1) mines[3] = pole[0] * pole[1] - 2;
						SDL_DestroyTexture(Mines[i]);
						str = std::to_string(mines[i]);
						int u = 3 - str.length();
						if (u > 0) { str1 = u8"Кол-во мин:"; for (int i = 0; i < u; i++) str1 += ' '; str1 += str; }
						else str1 = u8"Кол-во мин:" + str;
						Mines[i] = renderText(str1, "Res\\Storopia.ttf", { 0,0,0,0 }, 100, renderer);
					}
				}
			}
		}
		if (gav) {
			if (SDL_GetTicks() - t > 300) {
				if (SDL_GetTicks() - t1 > 50) {
					SDL_DestroyTexture(customXxX);
					t1 = SDL_GetTicks();
					for (int i = 0; i < 4; i++) {
						if (Polechange[i]) {
							pole[i / 2] += (i % 2 ? -1 : 1);
						}
					}
					if (pole[0] == 1) pole[0] = 2;
					else if (pole[0] == 100) pole[0] = 99;
					else if (pole[1] == 1) pole[1] = 2;
					else if (pole[1] == 100) pole[1] = 99;
					if (mines[3] >= pole[0] * pole[1] - 1) {
						mines[3] = pole[0] * pole[1] - 2;
						SDL_DestroyTexture(Mines[3]);
						str = std::to_string(mines[3]);
						int u = 3 - str.length();
						if (u > 0) { str1 = u8"Кол-во мин:"; for (int i = 0; i < u; i++) str1 += ' '; str1 += str; }
						else str1 = u8"Кол-во мин:" + str;
						Mines[3] = renderText(str1, "Res\\Storopia.ttf", { 0,0,0,0 }, 100, renderer);
					}
					Customstr = std::to_string(pole[0]) + 'x' + std::to_string(pole[1]);
					while (Customstr.length() < 11) Customstr = ' ' + Customstr + ' ';
					customXxX = renderText(Customstr, "Res\\Storopia.ttf", { 0,0,0,0 }, 100, renderer);
				}
			}
		}

		if (drag) {
			menuSQ[0].x -= x1 - x2;
			menuTXT[0].x = menuSQ[0].x;
			menuMODE[0].x = menuSQ[0].x;
			PlusBTN[0].x = menuSQ[0].x;
			MinusBTN[0].x = menuSQ[0].x + 216;
			minesNUM[0].x = menuSQ[0].x + 45;
			startBTN[0].x = menuSQ[0].x;
			loadBTN[0].x = menuSQ[0].x;
			for (int i = 1; i < 4; i++) {
				menuSQ[i].x = menuSQ[0].x + (otstup + menuSQ[0].w) * i;
				startBTN[i].x = menuSQ[i].x;
				loadBTN[i].x = menuSQ[i].x;
				menuTXT[i].x = menuSQ[i].x;
				menuMODE[i].x = menuSQ[i].x;
				PlusBTN[i].x = menuSQ[i].x;
				MinusBTN[i].x = menuSQ[i].x + 216;
				minesNUM[i].x = menuSQ[i].x + 45;
			}
			PoleCHANGE[0].x = menuSQ[3].x;
			PoleCHANGE[1].x = menuSQ[3].x;
			PoleCHANGE[2].x = menuSQ[3].x + 216;
			PoleCHANGE[3].x = menuSQ[3].x + 216;
			x1 = x2;
		}
		else if (Postdrag) {
			for (int i = 0; i < 4; i++) {
				OnMenu[i] = InRect(x, y, menuSQ[i]);
				OnStart[i] = InRect(x, y, startBTN[i]);
			}
			if ((menuSQ[0].x - inersion / 50 < x3 && menuSQ[0].x >= x3) || (menuSQ[0].x - inersion / 50 > x3 && menuSQ[0].x <= x3)) Postdrag = false;
			else if(WIDTH < 1130 && !(menuSQ[0].x >= 10 || menuSQ[3].x + menuSQ[3].w <= WIDTH-10)) Postdrag = false;
			
			if (inersion < 0) {
				menuSQ[0].x -= inersion / 50 - 1;
			}else menuSQ[0].x -= inersion / 50;
			inersion -= inersion / 50;
			menuTXT[0].x = menuSQ[0].x;
			menuMODE[0].x = menuSQ[0].x;
			PlusBTN[0].x = menuSQ[0].x;
			MinusBTN[0].x = menuSQ[0].x + 216;
			minesNUM[0].x = menuSQ[0].x + 45;
			startBTN[0].x = menuSQ[0].x;
			loadBTN[0].x = menuSQ[0].x;
			for (int i = 1; i < 4; i++) {
				menuSQ[i].x = menuSQ[0].x + (menuSQ[0].w + otstup) * i;
				menuTXT[i].x = menuSQ[i].x;
				startBTN[i].x = menuSQ[i].x;
				loadBTN[i].x = menuSQ[i].x;
				menuMODE[i].x = menuSQ[i].x;
				PlusBTN[i].x = menuSQ[i].x;
				MinusBTN[i].x = menuSQ[i].x + 216;
				minesNUM[i].x = menuSQ[i].x + 45;
			}
			PoleCHANGE[0].x = menuSQ[3].x;
			PoleCHANGE[1].x = menuSQ[3].x;
			PoleCHANGE[2].x = menuSQ[3].x + 216;
			PoleCHANGE[3].x = menuSQ[3].x + 216;
		}
		SDL_RenderCopy(renderer, bgImg, &format, NULL);
		SDL_RenderDrawRects(renderer, menuSQ,4);
		for (int i = 0; i < 4; i++) {
			if (OnMenu[i]) {
				SDL_SetTextureAlphaMod(white, 190);
				SDL_RenderCopy(renderer, white, NULL, &menuSQ[i]);
				SDL_SetTextureAlphaMod(white, 150);
			}
			else SDL_RenderCopy(renderer, white, NULL, &menuSQ[i]);
			if (OnStart[i]) {
				SDL_SetTextureAlphaMod(Green, 240);
				SDL_RenderCopy(renderer, Green, NULL, &startBTN[i]);
				SDL_SetTextureAlphaMod(Green, 100);
			}
			else SDL_RenderCopy(renderer, Green, NULL, &startBTN[i]);
			if (canload[i]) {
				if (onload[i]) {
					SDL_SetTextureAlphaMod(Green, 240);
					SDL_RenderCopy(renderer, Green, NULL, &loadBTN[i]);
					SDL_SetTextureAlphaMod(Green, 100);
				}
				else SDL_RenderCopy(renderer, Green, NULL, &loadBTN[i]);
			}
			else {
				SDL_RenderCopy(renderer, Red, NULL, &loadBTN[i]);
			}
			SDL_RenderCopy(renderer, resource, &plus[OnPlus[i]], &PlusBTN[i]);
			SDL_RenderCopy(renderer, load, NULL, &loadBTN[i]);
			SDL_RenderCopy(renderer, resource, &minus[OnMinus[i]], &MinusBTN[i]);
			SDL_RenderCopy(renderer, Mines[i], NULL, &minesNUM[i]);
			
			SDL_RenderCopy(renderer, start, NULL, &startBTN[i]);
			SDL_RenderDrawRect(renderer, &loadBTN[i]);
			SDL_RenderDrawRect(renderer, &startBTN[i]);
		}
		if (onback) {
			SDL_SetTextureAlphaMod(white, 190);
			SDL_RenderCopy(renderer, white, NULL, &back);
			SDL_SetTextureAlphaMod(white, 150);
		}
		else SDL_RenderCopy(renderer, white, NULL, &back);
		SDL_RenderCopy(renderer, Back, NULL, &back);
		SDL_RenderDrawRect(renderer, &back);

		SDL_RenderCopy(renderer, resource,&plus[Polechange[0]], &PoleCHANGE[0]);
		SDL_RenderCopy(renderer, resource, &minus[Polechange[1]], &PoleCHANGE[1]);
		SDL_RenderCopy(renderer, resource, &plus[Polechange[2]], &PoleCHANGE[2]);
		SDL_RenderCopy(renderer, resource, &minus[Polechange[3]], &PoleCHANGE[3]);

		SDL_RenderCopy(renderer, easy, NULL, &menuTXT[0]);
		SDL_RenderCopy(renderer, normal, NULL, &menuTXT[1]);
		SDL_RenderCopy(renderer, hard, NULL, &menuTXT[2]);
		SDL_RenderCopy(renderer, custom, NULL, &menuTXT[3]);

		SDL_RenderCopy(renderer, easy9x9, NULL, &menuMODE[0]);
		SDL_RenderCopy(renderer, normal18x18, NULL, &menuMODE[1]);
		SDL_RenderCopy(renderer, hard30x30, NULL, &menuMODE[2]);
		SDL_RenderCopy(renderer, customXxX, NULL, &menuMODE[3]);


		SDL_RenderPresent(renderer);
		SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	}
	for (int i = 0; i < 4; i++) {
		SDL_DestroyTexture(Mines[i]);
	}
	SDL_DestroyTexture(custom);
	SDL_DestroyTexture(*Mines);
	SDL_DestroyTexture(customXxX);
	SDL_DestroyTexture(easy);
	SDL_DestroyTexture(normal);
	SDL_DestroyTexture(hard);
	SDL_DestroyTexture(easy9x9);
	SDL_DestroyTexture(normal18x18);
	SDL_DestroyTexture(hard30x30);
	SDL_DestroyTexture(start);
	SDL_DestroyTexture(load);
	return quit;
}
void sound(Mix_Chunk *(Sound)) {
	Mix_PlayChannel(-1, Sound, 0);
}