#include "../../types.h"

void selconn_Render()
{
	DrawArtImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, pPcxTitleImage);
	RenderDiabloLogoSm();

	PrintText30Silver(-1, 161, "Multi Player Game", JustCentre);

	int w = 334;
	int x = 280;
	int y = 261;

	PrintText30Silver(x, 211, "Select Connection", JustCentre, w);

	char *connections[5] = {
		"Battle.net",
		"Local Area Network (IPC)",
		"Modem",
		"Direct Cable Connection",
		"Local Area Network (UDP)"
	};

	int selectorTop = y;

	for (int i = 0; i < 5; i++) {
		PrintText16Gold(x - 1, y, connections[i], JustCentre, w);
		y += 26;
	}

	DrawSelector16(x, selectorTop - 2, w, 35, 26);

	PrintText16Silver(35, 218, "Players Supported: 4");

	PrintText16Silver(35, 256, "Requirements:");
	// TODO need a word wrap function
	PrintText16Silver(35, 275, "All computers must be");
	PrintText16Silver(35, 291, "connected to an");
	PrintText16Silver(35, 307, "IPX-compatible network.");

	PrintText24Silver(26, 356, "no gateway needed", JustCentre, 226);

	PrintText30Gold(349, 429, "OK");
	PrintText30Gold(476, 429, "Cancel");
}

void selconn_Loade()
{
	LoadTitelArt("ui_art\\selgame.pcx");
}

void selconn_Free()
{
	mem_free_dbg(pPcxTitleImage);
	pPcxTitleImage = NULL;
}

int __stdcall UiSelectProvider(int a1, _SNETPROGRAMDATA *client_info, _SNETPLAYERDATA *user_info, _SNETUIDATA *ui_info,
    _SNETVERSIONDATA *file_info, int *type)
{
	selconn_Loade();

	SelectedItem = 1;
	SelectedItemMax = 5;
	SDL_Event event;

	bool done = false;
	while (done == false) {
		selconn_Render();
		if (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym) {
				case SDLK_UP:
					SelectedItem--;
					if (SelectedItem < MAINMENU_SINGLE_PLAYER) {
						SelectedItem = SelectedItemMax;
					}
					effects_play_sound("sfx\\items\\titlemov.wav");
					break;
				case SDLK_DOWN:
					SelectedItem++;
					if (SelectedItem > SelectedItemMax) {
						SelectedItem = MAINMENU_SINGLE_PLAYER;
					}
					effects_play_sound("sfx\\items\\titlemov.wav");
					break;
				case SDLK_ESCAPE:
					if (PreviousItem[submenu]) {
						SetMenu(PreviousItem[submenu]);
						break;
					}

					return FALSE;
				case SDLK_RETURN:
				case SDLK_KP_ENTER:
				case SDLK_SPACE:
					switch (SelectedItem) {
					case 1:
						effects_play_sound("sfx\\items\\titlslct.wav");
						done = SNetInitializeProvider('ipc', client_info, user_info, ui_info, file_info);
						SelectedItem = 1;
						SelectedItemMax = 3;
						break;
					case 2:
						effects_play_sound("sfx\\items\\titlslct.wav");
						done = SNetInitializeProvider('dial', client_info, user_info, ui_info, file_info);
						SelectedItem = 1;
						SelectedItemMax = 3;
						break;
					case 3:
						effects_play_sound("sfx\\items\\titlslct.wav");
						done = SNetInitializeProvider('null', client_info, user_info, ui_info, file_info);
						SelectedItem = 1;
						SelectedItemMax = 3;
						break;
					}
					break;
				}
				break;
			case SDL_QUIT:
				exit(0);
			}
		}

		DrawMouse();

		if (!done) {
			UiFadeIn();
		} else if (UiFadeOut()) {
			break;
		}
	}

	selconn_Free();

	return TRUE;
}
