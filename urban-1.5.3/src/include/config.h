#ifndef __CONFIG_H__
#define __CONFIG_H__

#define QUALITY_NORMAL 1
#define QUALITY_HIGH 2

struct KeyConf {
	int key_up;
	int key_down;
	int key_left;
	int key_right;
	int key_jump;
	int key_fire;
	int key_prevweapon;
	int key_nextweapon;
        int ctrl_type;
        int music_vol;
        int sfx_vol;
        int gfx_quality;
};

class Config {
	public:
		Config();
	        ~Config();
		void Start();
                struct KeyConf keyconf;
	private:
		void StartControls();
		void ChangeGFXQuality();
		void ChangeMusicVol();
		void ChangeSFXVol();
		int GetKey(char *Label);
		int GetButton(char *Label);
		void ConfigureKeyboard();
		void ConfigureGamepad();
        	void Load();
                void Save();
};

#endif
