#ifndef __MENU_H__
#define __MENU_H__


class Menu {
	public:
        	Menu();
                ~Menu();
                int show_menu(int type);
	private;
        	int current_choice;
                int num_choices;
};

#endif
