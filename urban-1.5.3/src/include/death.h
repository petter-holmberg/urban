#ifndef __DEATH_H__
#define __DEATH_H__

#define SPLAT_SOUND "samples/gutsplat.wav"

#define _X_ (x + width / 2)
#define _Y_ (y + height / 2)

#define DEATH_BY_EXPLOSION { \
int r = random() % 3;          \
int i; \
for(i = 0;i < 10;i++) \
	ENGINE.create_effect(new blood_o(_X_, y, z, random() % 8 + 2, -(random() % 4) - 2)); \
for(i = 0;i < 10;i++) \
	ENGINE.create_effect(new blood_o(_X_, y, z, -(random() % 8) - 2, -(random() % 4) - 2)); \
switch (r) {                    \
	case 0:                  \
		ENGINE.create_effect(new bodyparts_o(_X_, _Y_, z, "meat/arm", 5, -4, -6, 0)); \
		ENGINE.create_effect(new bodyparts_o(_X_, _Y_, z, "meat/arm", 5, 4, -6, 0)); \
		ENGINE.create_effect(new bodyparts_o(_X_, _Y_, z, "meat/head", 5, 2, -4, 0)); \
		ENGINE.create_effect(new bodyparts_o(_X_, _Y_, z, "meat/leg", 5, -1, -2, 0));  \
		ENGINE.create_effect(new bodyparts_o(_X_, _Y_, z, "meat/leg", 5, 3, -4, 0));    \
		ENGINE.create_effect(new bodyparts_o(_X_, _Y_, z, "meat/ribs", 5, 0, 0, 0));     \
		for (i = 0;i < 5;i++)                                                         \
			ENGINE.create_effect(new bodyparts_o(_X_, _Y_, z, "meat/meat", 3, random() % 5, -(random() % 6), 0)); \
	        SOUND.PlaySFX(SPLAT_SOUND); \
        	break; \
        case 1:         \
		ENGINE.create_effect(new bodyparts_o(_X_, _Y_, z, "meat/head", 5, 2, -4, 0)); \
		ENGINE.create_effect(new bodyparts_o(_X_, _Y_, z, "meat/ribs", 5, 0, 0, 0));     \
		for (i = 0;i < 5;i++)                                                         \
			ENGINE.create_effect(new bodyparts_o(_X_, _Y_, z, "meat/meat", 3, random() % 4, -(random() % 4) - 2, 0)); \
		for (i = 0;i < 5;i++)                                                         \
			ENGINE.create_effect(new bodyparts_o(_X_, _Y_, z, "meat/meat", 3, -(random() % 4), -(random() % 4) - 2, 0)); \
	        SOUND.PlaySFX(SPLAT_SOUND); \
        	break; \
        case 2:         \
        	for (i = 0;i < 3;i++) \
			ENGINE.create_effect(new bodyparts_o(_X_, _Y_, z, "meat/bigmeat", 5, random() % 5, -(random() % 4) - 3, 0)); \
        	for (i = 0;i < 3;i++) \
			ENGINE.create_effect(new bodyparts_o(_X_, _Y_, z, "meat/bigmeat", 5, -(random() % 5), -(random() % 4) - 3, 0)); \
	        SOUND.PlaySFX(SPLAT_SOUND); \
        	break; \
        case 3: \
        default: \
        	break; \
} \
}

/*#define DEATH_BY_EXPLOSION { \
ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/arm", 5, -4, -6, 0)); \
ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/arm", 5, 4, -6, 0)); \
ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/head", 5, 2, -4, 0)); \
ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/leg", 5, -1, -2, 0));  \
ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/leg", 5, 3, -4, 0));    \
ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/ribs", 5, 0, 0, 0));     \
for (int i = 0;i < 5;i++)                                                         \
	ENGINE.create_effect(new bodyparts_o(x, y, z, "meat/meat", 3, random() % 5, -(random() % 6), 0)); \
}*/
#endif
