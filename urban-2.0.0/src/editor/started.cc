#include "editor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Editor* editor;

int main(int argc, char* argv[])
{
    int ret;
    int xs = 0, ys = 0;
    editor = new Editor;

    if (argc == 1) {
        printf("Error: No map specified\n");
        printf("Usage: %s <mapname> [<xsize> <ysize>]\n", argv[0]);
        exit(1);
    }
    if (argc > 2 && argc != 4) {
        printf("Error: No size specified\n");
        printf("Usage: %s <mapname> <xsize> <ysize>\n", argv[0]);
        exit(2);
    }
    if (argc == 4) {
        xs = atoi(argv[2]);
        ys = atoi(argv[3]);
        if (xs == 0 || ys == 0) {
            printf("Error: Size cannot be 0\n");
            printf("Usage: %s <mapname> <xsize> <ysize>\n", argv[0]);
            exit(3);
        }
    }

    allegro_init();
    install_keyboard();
    install_timer();
    set_gfx_mode(GFX_MODEX, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0);

    ret = editor->EditLevel(argv[1], xs, ys);
    /*        ret = editor->EditLevel(argv[1],
            atoi(strdup(argv[2])),
            atoi(strdup(argv[3])));*/

    set_gfx_mode(GFX_TEXT, 80, 25, 0, 0);

    return ret;
}
