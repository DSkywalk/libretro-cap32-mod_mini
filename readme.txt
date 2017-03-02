		
	CAPRICE32 4.2 LIBRETRO 

	 
A quick port of caprice32-4.2.0 to LIBRETRO.

	
All the credit of the caprice32 Emulator to Ulrich Doewich 
Also 6128.h & amsdos.h (and psg for ps3 )files taken from wiituka (author:dskywalk)
http://code.google.com/p/wiituka/

And of course for the RetroArch/Libretro team : "http://www.libretro.com"


You can download the caprice32 original source code here :
	http://sourceforge.net/projects/caprice32/


Remember ,Everything not working well, so expect bugs.

Usage: launch an TAPE (cdt) or DSK , and game should autostart.

Core options:
  Autorun - 	disable/enable
        If enabled a RUN the first bas/bin found in DSK
  	
Knows Bugs: 
	- Everything not working well .


**********************************************************************************
 SKY NES MINI MOD - v1
**********************************************************************************

- still need rework onscreen kbd and clean previous libretro code.

 Core options:
  Autorun - disabled/enabled. If enabled, the core will run the first bas/bin found in DSK
  Resolution - Internal resolution; 384x272|800x600. 800x600 have a better KeyboardOnScreen.
  Pad X CFG - Configure pad; cursors|joystick|qaop|sxkl. See below.

 Controls:
	- JoyConfig "cursors": [STR] K, [A] ENTER, [B] F1,   [X] N, [Y] Y, DIR CURSORS.
	- JoyConfig "joystick":[STR] J, [A] FIRE1, [B] FIRE2,[X] N, [Y] Y, DIR JOYDIR.
	- JoyConfig "qaop":    [STR] R, [A] SPACE, [B] 1,    [X] 3, [Y] 2, DIR QAOP.
	- JoyConfig "sxkl":    [STR] QR,[A] UP,    [B] Z,    [X] N, [Y] SPACE, DIR SXKL.

	- JoyConfig common: [L2] CTRL, [R2] COPY, [L] ENTER, [R] SHIFT, [SEL] ** ALT MODE - KEEP PRESSED **.
    - Alternative mode: [STR] ** KeyboardOnScreen **, [UP] # RUN"DISC, [DW] # RUN"DISK,[LF] # CAT, [RG] # |CPM
	
 To start a game without autorun:
	Caprice32 emulates a real CPC, which means that to run a game you have to do the same thing as you would with a real CPC.

	To play a game, you must insert a disk image and type two basic commands:

		- You have to know what files the disc image has. It's easy, just press [SELECT] and [DPAD-LEFT]. This will then type CAT on screen.
		- You'll see a list of files. The important filetypes are BIN, BAS or ones without the extension, ex: ABADIA.BAS, GRYZOR.BIN, TETRIS.
		- With JoyConfig-cursors, use [R] and [R2] to copy the game's filename.
		- To run the game, place the cursor at the beginning of the name, press [L2]+[DPAD-LEFT] buttons and press [L] while holding them. This will put RUN" in front of the file name and launch the game.
		- For some games, you don't have to type the  RUN" command, they load with the |CPM command. Those are easily detectable. If you get an error when you executing the CAT command (Ignore, Cancel), these games are protected. Don't worry about it, they are even easier to execute, just press [SELECT] and [DPAD-RIGHT].


 Rec. CPC Games:
=================


SPORTS
-----------
Fernando Martin Basket Master
Match Day 2
Tenis 3D
Colossus Chess
Cyrus II Chess
Summer Games
Winter Games
Emlyn Hughes
Tie Break
Perico Delgado



ADVENTURES
---------------
La Abadia del Crimen (Best ever 8bits game Based on "Name of the Rose" - ONLY SPANISH)
Head Over Heels
Batman 3D (John Ritman)
Rambo 3
Dustin
The Great Escape
Knight Lore
Pijamarama
Myth: History Making
3 weeks in paradise
Last Ninja 2
Livingstone Supongo
Sir Fred
Inside Outing 
Fairlight
Navy moves
Shadow of the Beast


ARCADE
---------
Gryzor (Aka Contra)
Turrican II
Renegade I y II
Super Sprint
Zona 0
Year Die Kung Fu
Kungfu Master
Ikari Warrirors
The Way of the Exploding Fist
Commando
Shinobi
Strider 1 y 2
Psycho Pig Uxb
Oh! Mummy
Gunfright
Tempest
Barbarian I
Prohibition


STRATEGY - PUZZLES
-------------------
Lords of Midnight
North and South
Elite
Sentinel
Archon 2
Deflektor
Puzznic
Night Shift
High Steel
Spindizzy
Boulder Dash
Spherical
Solomons Key


PLATFORMS
------------------
Rick Dangerous
Manic Miner
Abu Simbel
Vampire: Phantomas 2
Prince of Persia
Prehistorik II
Antiriad
Jet Set Willy 
Rodland Series
Bomb jack
Bubble Bobble
Rainbow Islands


SHOOTERS
--------------
Defend or Die (Aka defender)
Nemesis
R-type
Zynaps
Psyborg
1942
Afterburner
Thanatos
Empire Strikes Back
Army Moves
virus
Space Harrier 2
Silk Worm
Monty Phyton's Fliying Circus


RPGs
--------
Bloodwych
Bard's Tale
Heroes of the Lance
Hero Quest
Space Crusader
Rogue


MISC.
-------
Teenage queen
Sherman M4
Fighter Bomber
Total Eclipse
Castle Master
Dark Side



 Where get downloads CPC games and more info:
==============================================

http://www.cpc-power.com/ (original disks, games, covers, ... best source!)
http://www.cantrell.org.uk/mirrors/ftp.nvg.ntnu.no/pub/cpc/games/ (European CPC Server)
http://cpcgamereviews.com/a/index.html (CPC Reviews, Coments, Screens...)

See you and enjoy with your nes mini!!
	D_Skywalk

http://david.dantoine.org


