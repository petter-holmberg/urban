U.R.B.A.N - The Cyborg Project
==============================

https://github.com/petter-holmberg/urban

History
-------

"U.R.B.A.N - The Cyborg Project" is a 2D platform game that was developed by some of my friends and I during our last year of high school. My recollections of the project is that we spent virtually every break during schooldays in the CAD computer room either working on the game or playing a game of multiplayer Descent over the IPX network.

We weren't really supposed to have access to the computer room outside of scheduled lessons but had organized a system where people would stay behind and let us in. Eventually I think we even managed to pursuade some of our favorite teachers to unlock the door for us (hence some of the honorary mentions in the game credits).

During development a game design competition was announced in the Swedish edition of PC Gamer Magazine, and we submitted it. It came in third place and was released on the CD-rom accompanying the July 1999 issue.

Technically it's a very basic platformer, the main innovation being a 3-layer level design where you can walk in front of and behind objects and have to line up on the same level as enemies to hit them. But really, its main "feature" is the excessive blood and gore, which at the time we thought was very funny. We also added animated intro and outro cutscenes which were pretty advanced for an indie game of its time. Obviously, the game doesn't take itself very seriously.

The game was originally developed in MS-DOS with DJGPP, using the popular Allegro library for graphics, controller input, and audio. It was also ported to Linux, where Allegro was replaced by GGI and mikmod, and the source code was released as free software under GNU GPL v2.

The source code for U.R.B.A.N 1.5.3 (the final original Linux version), along with its audio files (which were offered as a separate download since not everyone had a sound card back in the day) are included here for reference.

The original U.R.B.A.N website is no longer online, but a snapshot can be found at:

http://web.archive.org/web/20010614001827/http://www.bengburken.net/~jonas/urban/

Revival
-------

Since this game has not been updated in over 20 years by the original programmers, it is no longer easy to compile on a modern Linux system. The GGI library in particular is dead and no longer available on default package manager repositories.

I thought it would be a fun project to try and make the game work again in Ubuntu 20 with a modern C++ compiler, and clean up the legacy code a bit to use more modern features. Hopefully this will make it much easier to revive the game again in the future, possibly on other platforms.

And so U.R.B.A.N 2.0 awakens!

When looking at the original source code you have to realize that this game was developed in a pre-ISO C++ environment and without knowledge of some C++ features that are common knowledge today. It frequently uses manual memory management (rife with memory leaks of course) and C-style interfaces, with a sprinkle of OOP, as was common practice at the time. There was no use of standard library headers except for the C-style headers. There is quite a lot of copy-paste code duplication and questionable macro usage, but overall the code is pretty clean and easy to follow.

To make the game run again, GGI has been replaced by SFML for the basic 2D graphics and control functions. Originally the game was running in VGA Mode X, which had a 320x240 pixel resolution using a 256-color palette. Now the game runs in full sRGBA and is upscaled to run in a 1280x960 window. Otherwise the gameplay experience should be the same as in the original version.

Some of the main updates to the source code in U.R.B.A.N 2.0 include:
- Getting rid of macros, as modern C++ offers superior alternatives in almost all situations
- Getting rid of system headers where the C++ standard library offers better, portable alternatives
- Getting rid of C-style headers where the C++ standard library offers better alternatives
- Getting rid of legacy core language features in favor of modern alternatives
- Getting rid of manual memory management

On Ubuntu, U.R.B.A.N 2.0 can be installed as follows:

    cd urban-2.0
    ./configure
    make
    sudo make install

To install the music and sound effects, simply copy the `snd` directory to `/usr/local/share/urban/` and you should be good to go.

If the installation was successful the game should be installed under `/usr/local/bin/urban` and can be started by typing `urban` in a console. Game configuration, highscores and savegame files are located in `~/.urban/`.

Pictures of the original script (in Swedish) are included in the repository.

Story
-----

During the end of the 20:th century, the american public started to get tired of seeing their sons and daughters returning in bags from wars in Europe and the Far East.
In order to continue the struggle for political stability and a secure economic flow, without the insight from an increasingly irritated population, new invincible soldiers was needed.
The best scientists available were assigned to work on the creation of such a soldier.
Far below the surface of the ground, in a military base in the Nevada desert, they started to develop a warrior, half man, half robot - a Cyborg.
After a couple of months of hard work, the project was very close to completion.
The only thing that remained was to find a human being, ready to be rebuilt to better serve his country.

But it turned out to be harder than expected to find such a human.
After trying through all the regular channels, they realized that it would be impossible to get volunteers within the military community.
The politicians expected results, and since no prototype had been completed yet, financing was starting to become a problem.
In order to prevent the termintation of the project, it was now decided that the plan of getting a volunteer was to be abandoned.
Instead, it was decided that "volunteers" were to be chosen among the civilian nobodys, who no one would miss anyway, and use them insted.

The head of the research department called for a top secret meeting with the CIA and the commanding officer of the base, where they decided who would be chosen for the project.
The first one to be chosen, and in a matter of days kidnapped from his own bedroom, was an uneducated father of two children living in Nevada. Urban Gutter.
After being sent to sleep he was taken to the Nevada base, where the project finally could be completed.
After going through a thorough brainwash, the work of replacing easily damaged biological organs with metal equivalents was started.

The project was almost completed, when the accident suddenly struck.
During the final stage of the control-code uploading, Urban Gutter suddenly came to his sence. Without any security device drivers yet installed, he turned his rage against his creators, and started massacre everyone in the surgery room on a cold-blooded and completely inhumane way, before he started to head for the surface.

You are all that's left of Urban Gutter, a cyborg of no feelings, with the one and only mission to get out of the military base, and to create the worst blood bath possible on the way.
