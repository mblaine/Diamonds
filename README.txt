                                   Diamonds

In this Breakout-style game, guide the bouncing ball left and right, clearing
away blocks to reach the next level.

Diamonds is a tribute to the original game, created by Oliver Dreer and 
released by Varcon Systems in 1992.

>>> How to play

    Controls

       Left arrow   - move the ball left
       Right arraow - move the ball right
       F2           - start a new game
       P            - pause/unpause
       R            - restart the current level
       Q            - quit the current game
       Escape       - exit the program (doesn't alwasy work, try Game->Exit)

    While the ball constantly bounces up and down, the only control you
    really have is with the left and right arrow keys. The goal is to see
    how far you can make it before running out of lives, lost when the ball
    makes contact with a skull block.

    To complete a level, clear away all the diamond blocks on the screen by
    bouncing the ball off of them. However, before the diamond blocks will
    be destructable, all the color blocks must be destroyed. To remove a
    color block, the ball must hit it while matching the block's color. To 
    change the color of the ball, run it into a paintbrush block first.

    Light blue blocks must be cleared first because there are no light blue
    paintbrush blocks. The only time the ball is light blue is when first
    starting a level, or right after losing a life.

    Also, there are lock and key blocks. These aren't required to be 
    eliminated to complete a level, but probably do prevent access to color
    or diamond blocks which must be cleared. The ball has to be orange to
    pick up a key or to use a key to destroy a lock.

    Hints

    Again, when starting a new level avoid paintbrush blocks until all the
    light blue blocks are gone.

    Watch out for reverse blocks, they swap the left and right keys for the
    rest of the current level or until a skull block is hit.

    Be sure and check out the "More Diamonds" levelset from the Levelset
    menu!

>>> License

    This program is free software: you can redistribute it and/or modify it 
    under the terms of the GNU General Public License as published by the
    Free Software Foundation, either version 3 of the License, or (at your 
    option) any later version.

    A copy of the GNU General Public License can be found in the file
    COPYING.txt wherever Diamonds is installed, or at gnu.org/licenses/.

>>> Source

    The source code for Diamonds is freely available at
    github.com/mblaine/Diamonds. The source for Diamonds' dependencies can be 
    found at the urls listed in the Dependencies section below.

>>> Change log

	Marcth 27, 2012
       Moved to repository on GitHub

    May 5, 2008 (v0.4)
       First public release (on SourceForge at diamonds.sourceforge.net)

>>> Installation

    If on Windows, you can download and run the installer from 
    github.com/mblaine/Diamonds/downloads. Either way, you can unzip a build
    archive wherever you want and then run diamonds.exe.

    Every file Diamonds reads or writes is in the data directory and is 
    refered to by that relative path.

>>> Building

    Be sure and see the dependencies section below.

    If building using MinGW on Windows, see BUILD-WIN32-MINGW.txt in the
    build-win32-mingw directory extracted from the source archive.

    Unfortunately Diamonds has not been built using any other compiler on any
    archive operating system yet.

>>> Dependencies

    SDL       - libsdl.org/download-1.2.php
    SDL_image - libsdl.org/projects/SDL_image/
    SDL_mixer - libsdl.org/projects/SDL_mixer/
    SDL_ttf   - libsdl.org/projects/SDL_ttf/

    Which as used by Diamonds depend on
       libpng        - libpng.org/pub/png/libpng.html
       libogg        - xiph.org/downloads/
       libvorbis     - xiph.org/downloads/
       libvorbisfile - xiph.org/downloads/
       libfreetype   - sourceforge.net/projects/freetype/
       zlib          - zlib.net/

    And if building
       wxWidgets - wxwidgets.org/downloads/