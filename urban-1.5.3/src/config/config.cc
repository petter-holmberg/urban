#include <allegro.h>
#include <stdio.h>
#include <stdlib.h>
#ifdef DJGPP
	#include <dos.h>
#else
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <fcntl.h>
	#include <unistd.h>
#endif
/**************************************************************************************/
#include "urbfont.h"
#include "game.h"
#include "ctrls.h"
#include "engine.h"
#include "config.h"
#include "keyinfo.h"
/**************************************************************************************/
void Config::Load() {
	char filename[1024];
	FILE *fd;

#ifdef DJGPP
	sprintf(filename, "ctrl.dat");
#else
	sprintf(filename, "%s/.urban/ctrl.dat", getenv("HOME"));
#endif

        if(!(fd = fopen(filename, "rb")))
        	return;

	fread(&keyconf, sizeof(struct KeyConf), 1, fd);

        fclose(fd);
}
/**************************************************************************************/
void Config::Save() {
	char filename[1024];
	FILE *fd;

#ifdef DJGPP
	sprintf(filename, "ctrl.dat");
#else
	/* Create dir */
	sprintf(filename, "%s/.urban", getenv("HOME"));
	mkdir(filename, S_IRUSR | S_IWUSR | S_IXUSR);

	sprintf(filename, "%s/.urban/ctrl.dat", getenv("HOME"));
#endif
        if(!(fd = fopen(filename, "wb")))
        	return;

	fwrite(&keyconf, sizeof(struct KeyConf), 1, fd);

        fclose(fd);
}
/**************************************************************************************/
Config::Config() {
	// Default values

	keyconf.key_up = KEY_UP;
	keyconf.key_down = KEY_DOWN;
	keyconf.key_left = KEY_LEFT;
	keyconf.key_right = KEY_RIGHT;
	keyconf.key_jump = KEY_SPACE;
	keyconf.key_fire = KEY_LCONTROL;
	keyconf.key_prevweapon = KEY_LSHIFT;
	keyconf.key_nextweapon = KEY_RSHIFT;
        keyconf.ctrl_type = CONTROLLER_KEYBOARD;
        keyconf.music_vol = 90;
        keyconf.sfx_vol = 128;
        keyconf.gfx_quality = QUALITY_NORMAL;

        Load();
}
/**************************************************************************************/
#ifdef DJGPP
void Config::ConfigureGamepad() {
	PALETTE Pal;
	BITMAP *bmp = icache.GetImage("ibild.pcx", Pal);
        int Key = 0;
	int temp = 0;

        set_palette (Pal);
        UrbanFont fnt(SMALL_FONT2);


        blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

	/* Ask for the number of buttons */

        fnt.print_centre("Gamepad type:", 160, 100);
	fnt.print_centre("  1. 2 buttons
  2. 4 buttons
  3. 6 buttons
  4. 8 buttons", 160, 140);
	int running = 1;

	while(running) {
        	Key = readkey();

                if((Key	>> 8) == KEY_ESC)
                	return;

		if(!((Key & 0xff) < '1'	|| (Key	& 0xff)	> '4'))	{

			switch((Key & 0xff)) {
                        	case '1':
                                	keyconf.ctrl_type = CONTROLLER_GAMEPAD2;
		                        install_joystick(JOY_TYPE_STANDARD);
                                        break;
                        	case '2':
                                	keyconf.ctrl_type = CONTROLLER_GAMEPAD4;
		                        install_joystick(JOY_TYPE_4BUTTON);
                                        break;
                        	case '3':
                                	keyconf.ctrl_type = CONTROLLER_GAMEPAD6;
		                        install_joystick(JOY_TYPE_6BUTTON);
                                        break;
                        	case '4':
                                	keyconf.ctrl_type = CONTROLLER_GAMEPAD8;
		                        install_joystick(JOY_TYPE_8BUTTON);
                                        break;

			};
                        running = 0;
                        break;
		}
        }
        /* Clear screen */
        blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

        if((temp = GetButton("UP")) != (-1))
                keyconf.key_up = temp;
	else
        	return;
        blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

        if((temp = GetButton("DOWN")) != (-1))
                keyconf.key_down = temp;
	else
        	return;
        blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

        if((temp = GetButton("LEFT")) != (-1))
                keyconf.key_left = temp;
	else
        	return;
        blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

        if((temp = GetButton("RIGHT")) != (-1))
                keyconf.key_right = temp;
	else
        	return;
        blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

        if((temp = GetButton("FIRE")) != (-1))
                keyconf.key_fire = temp;
	else
        	return;
        blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

        if((temp = GetButton("JUMP")) != (-1))
                keyconf.key_jump = temp;
	else
        	return;
        blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

	if((Key & 0xff) > '1') {

	        if((temp = GetButton("Prev Weapon")) != (-1))
	                keyconf.key_prevweapon = temp;
		else
	        	return;
	        blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

	        if((temp = GetButton("Next Weapon")) != (-1))
	                keyconf.key_nextweapon = temp;
		else
	        	return;

	        blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

	} else {
	                keyconf.key_prevweapon = keyconf.key_nextweapon = 0;

        }
        icache.FreeImage(bmp);

        remove_joystick();
}
#endif
/**************************************************************************************/
void Config::ConfigureKeyboard() {
	PALETTE Pal;
	int temp = 0;
	BITMAP *bmp = icache.GetImage("ibild.pcx", Pal);
        set_palette (Pal);

        blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

        if((temp = GetKey("UP")) != (-1))
                keyconf.key_up = temp;
	else
        	return;
        blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

        if((temp = GetKey("DOWN")) != (-1))
                keyconf.key_down = temp;
	else
        	return;
        blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

        if((temp = GetKey("LEFT")) != (-1))
                keyconf.key_left = temp;
	else
        	return;
        blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

        if((temp = GetKey("RIGHT")) != (-1))
                keyconf.key_right = temp;
	else
        	return;
        blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

        if((temp = GetKey("FIRE")) != (-1))
                keyconf.key_fire = temp;
	else
        	return;
        blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

        if((temp = GetKey("JUMP")) != (-1))
                keyconf.key_jump = temp;
	else
        	return;
        blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

        if((temp = GetKey("Prev Weapon")) != (-1))
                keyconf.key_prevweapon = temp;
	else
        	return;
        blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

        if((temp = GetKey("Next Weapon")) != (-1))
                keyconf.key_nextweapon = temp;
	else
        	return;
        blit(bmp, screen, 0, 0, 0, 0, bmp->w, bmp->h);

        keyconf.ctrl_type = CONTROLLER_KEYBOARD;

        icache.FreeImage(bmp);
}
/**************************************************************************************/
void Config::ChangeMusicVol() {
	PALETTE pal;
	BITMAP *backg = icache.GetImage("ibild.pcx", pal);
        int running = 1;
        int vol = keyconf.music_vol;

	blit(backg, screen, 0, 0, 0, 0, backg->w, backg->h);

	UrbanFont m(LARGE_FONT);
        m.SetScale(65);
	m.print_centre("MUSIC VOLUME", SCREEN_WIDTH / 2, 50);

        rectfill(screen, (SCREEN_WIDTH - 256) / 2 + 2, 102,
        	vol + (SCREEN_WIDTH - 256) / 2 + 2, 122, 207);

        rectfill(screen, (SCREEN_WIDTH - 256) / 2, 100,
        	vol + (SCREEN_WIDTH - 256) / 2, 120, 192);


	while(running) {
#ifdef DJGPP
		vsync();
#endif

		blit(backg, screen, 0, 100, 0, 100, backg->w, 23);

	        rectfill(screen, (SCREEN_WIDTH - 256) / 2 + 2, 102,
	        	vol + (SCREEN_WIDTH - 256) / 2 + 2, 122, 207);

	        rectfill(screen, (SCREEN_WIDTH - 256) / 2, 100,
	        	vol + (SCREEN_WIDTH - 256) / 2, 120, 192);

		switch((readkey() >> 8)) {                      // 207 192

		   	case KEY_LEFT:
		   	case KEY_DOWN:

                        	if(vol > 1)
                                	vol -= 2;

                                SOUND.SetMusicVolume(vol);
                                keyconf.music_vol = vol;
                        	break;

		   	case KEY_UP:
		   	case KEY_RIGHT:

                        	if(vol < 254)
                                	vol += 2;

                                SOUND.SetMusicVolume(vol);
                                keyconf.music_vol = vol;
                        	break;

			default:
                        	running = 0;
                                Save();
                                break;
		};
	};
        icache.FreeImage(backg);
}
/**************************************************************************************/
void Config::ChangeSFXVol() {
	PALETTE pal;
	BITMAP *backg = icache.GetImage("ibild.pcx", pal);
        int running = 1;
        int vol = keyconf.sfx_vol;

	blit(backg, screen, 0, 0, 0, 0, backg->w, backg->h);

	UrbanFont m(LARGE_FONT);
        m.SetScale(65);
	m.print_centre("SFX VOLUME", SCREEN_WIDTH / 2, 50);

        rectfill(screen, (SCREEN_WIDTH - 256) / 2 + 2, 102,
        	vol + (SCREEN_WIDTH - 256) / 2 + 2, 122, 207);

        rectfill(screen, (SCREEN_WIDTH - 256) / 2, 100,
        	vol + (SCREEN_WIDTH - 256) / 2, 120, 192);


	while(running) {

#ifdef DJGPP
		vsync();
#endif
		blit(backg, screen, 0, 100, 0, 100, backg->w, 23);

	        rectfill(screen, (SCREEN_WIDTH - 256) / 2 + 2, 102,
	        	vol + (SCREEN_WIDTH - 256) / 2 + 2, 122, 207);

	        rectfill(screen, (SCREEN_WIDTH - 256) / 2, 100,
	        	vol + (SCREEN_WIDTH - 256) / 2, 120, 192);

		switch((readkey() >> 8)) {                      // 207 192

		   	case KEY_LEFT:
		   	case KEY_DOWN:

                        	if(vol > 1)
                                	vol -= 2;

                                keyconf.sfx_vol = vol;
                        	break;

		   	case KEY_UP:
		   	case KEY_RIGHT:

                        	if(vol < 254)
                                	vol += 2;

                                keyconf.sfx_vol = vol;
                        	break;

			default:
                        	running = 0;
                                break;
		};
	};
        icache.FreeImage(backg);
}
/**************************************************************************************/
void Config::ChangeGFXQuality() {

	int tmp = Do_Menu("NORMAL\nHIGH", 2, keyconf.gfx_quality);

        if((tmp > 0) && (tmp < 3))
	        keyconf.gfx_quality = tmp;
}
/**************************************************************************************/
#define MENY_POS_X 60
#define MENY_POS_Y 85
#define FONT_H 26
void Config::Start() {
        int pos = 1;

	while((pos = Do_Menu("CONTROLS\nGRAPHICS\nMUSIC VOL\nSFX VOL\nBACK", 5, pos)) && pos != 5) {
        	switch(pos) {
			case 1:
				StartControls();
				break;

                        case 2:
                        	ChangeGFXQuality();
                        	break;

			case 3:
				ChangeMusicVol();
				break;

			case 4:
				ChangeSFXVol();
				break;
                };
        };
}
/**************************************************************************************/
void Config::StartControls() {
#ifdef DJGPP
	if (install_joystick(JOY_TYPE_AUTODETECT) != 0 || num_joysticks == 0) {
		ConfigureKeyboard();
                Save();
                return;
        }
        remove_joystick();

        switch(Do_Menu("KEYBOARD\nGAMEPAD\nBACK", 3)) {
        	case 0:
        	case 3:
                	return;
                        break;

		case 1:
		        ConfigureKeyboard();
			break;

		case 2:
			ConfigureGamepad();
			break;
        };
#else
	ConfigureKeyboard();
#endif
        Save();
}
/**************************************************************************************/
#ifdef DJGPP
int Config::GetButton(char *Label) {
        char buffer[128];
        UrbanFont fnt(SMALL_FONT2);

	sprintf(buffer, "Please press your button for '%s'", Label);
	fnt.print_centre(buffer, 320 / 2, 90);

        for(;;) {

		poll_joystick();

                if(keypressed()) {
                	while(keypressed())
                        	readkey();

                        return -1;
                }

                if(joy_left) {
			fnt.print_centre("LEFT", 320 / 2, 120);
                        rest(500);
                	while(joy_left)
                		poll_joystick();
                        return JOYSTICK_LEFT;
                }
                if(joy_right) {
			fnt.print_centre("RIGHT", 320 / 2, 120);
                        rest(500);
                	while(joy_right)
                		poll_joystick();
                        return JOYSTICK_RIGHT;
                }
                if(joy_up) {
			fnt.print_centre("UP", 320 / 2, 120);
                        rest(500);
                	while(joy_up)
                		poll_joystick();
                        return JOYSTICK_UP;
                }
                if(joy_down) {
			fnt.print_centre("DOWN", 320 / 2, 120);
                        rest(500);
                	while(joy_down)
                		poll_joystick();
                        return JOYSTICK_DOWN;
                }
                if(joy_b1) {
			fnt.print_centre("BUTTON 1", 320 / 2, 120);
                        rest(500);
                	while(joy_b1)
                		poll_joystick();
                        return JOYSTICK_B1;
                }
                if(joy_b2) {
			fnt.print_centre("BUTTON 2", 320 / 2, 120);
                        rest(500);
                	while(joy_b2)
                		poll_joystick();
                        return JOYSTICK_B2;
                }
                if(joy_b3) {
			fnt.print_centre("BUTTON 3", 320 / 2, 120);
                        rest(500);
                	while(joy_b3)
                		poll_joystick();
                        return JOYSTICK_B3;
                }
                if(joy_b4) {
			fnt.print_centre("BUTTON 4", 320 / 2, 120);
                        rest(500);
                	while(joy_b4)
                		poll_joystick();
                        return JOYSTICK_B4;
                }
                if(joy_b5) {
			fnt.print_centre("BUTTON 5", 320 / 2, 120);
                        rest(500);
                	while(joy_b5)
                		poll_joystick();
                        return JOYSTICK_B5;
                }
                if(joy_b6) {
			fnt.print_centre("BUTTON 6", 320 / 2, 120);
                        rest(500);
                	while(joy_b6)
                		poll_joystick();
                        return JOYSTICK_B6;
                }
                if(joy_b7) {
			fnt.print_centre("BUTTON 7", 320 / 2, 120);
                        rest(500);
                	while(joy_b7)
                		poll_joystick();
                        return JOYSTICK_B7;
                }
                if(joy_b8) {
			fnt.print_centre("BUTTON 8", 320 / 2, 120);
                        rest(500);
                	while(joy_b8)
                		poll_joystick();
                        return JOYSTICK_B8;
                }
        }

	return -1;
}
#endif
/**************************************************************************************/
int Config::GetKey(char *Label) {
        int i;
	int temp = 0;
        char buffer[128];
        UrbanFont fnt(SMALL_FONT2);

	sprintf(buffer, "Please press your key for '%s'", Label);
	fnt.print_centre(buffer, 320 / 2, 90);

        clear_keybuf();
#if DJGPP
        while (!keypressed()) {
        	if(key_shifts & KB_SHIFT_FLAG) {
			disable();
                	if(key[KEY_LSHIFT])
	                	temp = KEY_LSHIFT;
                        else
	                	temp = KEY_RSHIFT;
			enable();
                        break;
                }

        	if(key_shifts & KB_CTRL_FLAG) {
			disable();
                	if(key[KEY_LCONTROL])
	                	temp = KEY_LCONTROL;
                        else
	                	temp = KEY_RCONTROL;
			enable();
                        break;
                }

        	if(key_shifts & KB_ALT_FLAG) {
			disable();
                	if(key[KEY_ALT])
	                	temp = KEY_ALT;
                        else
	                	temp = KEY_ALTGR;
			enable();
                        break;
                }
        }
#endif
	if (!temp) {
	        temp = readkey();
	        temp >>= 8;
        }
        i = 0;
        while (keyinfo[i].num) {
        	if (keyinfo[i].num == temp) {

			fnt.print_centre(keyinfo[i].description, 320 / 2, 120);
                        while (key[temp]);
                        rest(500);
                	return temp;
                }
                i++;
	}
	return -1;
}

Config::~Config() {
}
/**************************************************************************************/





