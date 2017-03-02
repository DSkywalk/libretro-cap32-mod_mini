/* nuklear - v1.00 - public domain */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <math.h>
#include <limits.h>
#include <time.h>

#include <libretro.h>
#include <libretro-core.h>

extern void filebrowser_init();
extern void filebrowser_free();

extern int snapshot_save (char *pchFileName);
extern void play_tape();
extern void Screen_SetFullUpdate(int scr);
extern void vkbd_key(int key,int pressed);

extern long GetTicks(void);

extern retro_input_poll_t input_poll_cb;
extern retro_input_state_t input_state_cb;

void update_joy_cfg(int playerID);

extern char RPATH[512];

//EMU FLAGS
int SHOWKEY=-1;
int SHIFTON=-1;
int KBMOD=-1;
int RSTOPON=-1;
int CTRLON=-1;
int NUMDRV=1;
int NPAGE=-1;
int KCOL=1;
int LOADCONTENT=-1;
int STATUTON=-1;
int LDRIVE=8;
int SND=1;
int vkey_pressed;
unsigned char MXjoy[2]; // joy
char LCONTENT[512];
char Core_Key_Sate[512];
char Core_old_Key_Sate[512];

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_IMPLEMENTATION

#define NK_RETRO_SOFT_IMPLEMENTATION

#include "nuklear.h"
#include "nuklear_retro_soft.h"

static RSDL_Surface *screen_surface;
extern void restore_bgk();
extern void save_bkg();

/* macros */

#define UNUSED(a) (void)a
#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) < (b) ? (b) : (a))
#define LEN(a) (sizeof(a)/sizeof(a)[0])

/* Platform */

float bg[4];
struct nk_color background;
/* GUI */
struct nk_context *ctx;

static nk_retro_Font *RSDL_font;

#include "style.c"
#include "filebrowser.c"
#include "gui_sky.c"

void retro_key_down(int key);
void retro_key_up(int key);
void app_vkb_handle();
void kbd_buf_feed(char *s);
int retro_disk_auto();

int current_w, current_h;

int app_init(int width, int height)
{
    current_w = width;
    current_h = height;

#ifdef M16B
    screen_surface=Retro_CreateRGBSurface16(current_w,current_h,16,0,0,0,0);
#else
    screen_surface=Retro_CreateRGBSurface32(current_w,current_h,32,0,0,0,0);
#endif
    Retro_Screen=(PIXEL_TYPE *)screen_surface->pixels;

    RSDL_font = (nk_retro_Font*)calloc(1, sizeof(nk_retro_Font));
    RSDL_font->width = 8;
    RSDL_font->height = 8;
    if (!RSDL_font)
        return -1;

    /* GUI */
    ctx = nk_retro_init(RSDL_font,screen_surface,current_w,current_h);

    /* style.c */
    /* THEME_BLACK, THEME_WHITE, THEME_RED, THEME_BLUE, THEME_DARK */
    set_style(ctx, THEME_DARK);

    /* icons */

    filebrowser_init();
    sprintf(LCONTENT,"%s%c",RPATH,'\0');

	memset(Core_Key_Sate,0,512);
	memset(Core_old_Key_Sate ,0, sizeof(Core_old_Key_Sate));

    printf("Init nuklear %ux%u\n", current_w, current_h);

 return 0;
}

int app_free()
{
   //FIXME: memory leak here
   if (RSDL_font)
      free(RSDL_font);
   RSDL_font = NULL;
   filebrowser_free();
   nk_retro_shutdown();

   Retro_FreeSurface(screen_surface);
   if (screen_surface)
      free(screen_surface);
   screen_surface = NULL;

   return 0;
}

unsigned int message_timer = 0;
char message_data[256] = {'\0'};
void app_printf(const char* format, ...){
    va_list args;
    va_start (args, format);

    message_timer = GetTicks() + 500000;
    vsnprintf(message_data, 256, format, args);
    message_data[255] = '\0';
    va_end (args);
}


int app_event(int poll)
{
	nk_input_begin(ctx);
	nk_retro_handle_event(poll);
	nk_input_end(ctx);

	return 0;
}

int app_render(int poll)
{
	if( poll==0 && SHOWKEY==-1 )return 1;

	if(poll==0)
		app_vkb_handle();
	else
		restore_bgk();

	app_event(poll);

    gui(&browser,ctx, current_w, current_h);

    /* Draw */
    //nk_color_fv(bg, background);
	nk_retro_render(nk_rgba(0,0,0,0));

    return 0;
}

void app_vkb_handle()
{
    static int oldi=-1;
    int i;

    if(oldi!=-1) {
        vkbd_key(oldi,0);
        oldi=-1;
    }

    if(vkey_pressed==-1)
        return;

	i=vkey_pressed;
	vkey_pressed=-1;

    switch (i) {
        case -2:
            NPAGE=-NPAGE;
        case -1:
            oldi=-1;
            break;
        case -3:
            //KDB bgcolor
            KCOL=-KCOL;
            oldi=-1;
            break;
        case -4:
            //autorun
            retro_disk_auto();
            oldi=-1;
            break;
        case -5:
            //RESET CPC
            emu_reset();
            oldi=-1;
            break;
        case -8:
            //PLAY TAPE
            kbd_buf_feed("|TAPE\nRUN\"\n^");
            //play_tape();
            oldi=-1;
            break;
        case -10:
            kbd_buf_feed("RUN\"DISC\n");
            oldi=-1;
            break;
        case -11:
            kbd_buf_feed("RUN\"DISK\n");
            oldi=-1;
            break;
        case -12:
            kbd_buf_feed("CAT\n");
            oldi=-1;
            break;
        case -13:
            kbd_buf_feed("|CPM\n");
            oldi=-1;
            break;
        case -14:
            update_joy_cfg(ID_PLAYER1);
            oldi=-1;
            break;
        case -15:
            update_joy_cfg(ID_PLAYER2);
            oldi=-1;
            break;
        case 0x25:
            SHIFTON=-SHIFTON;
            oldi=-1;
            break;
        case 0x27:
            CTRLON=-CTRLON;
            oldi=-1;
        default:
            oldi=i;
            vkbd_key(oldi,1);

    }
}

/* SKY NES MINI MOD! */
extern uint8_t keyboard_matrix[16];
const uint8_t bit_values[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

unsigned int last_input[MAX_INPUTS] = {0,0};
unsigned int padnum = 0, last_event = 0, padcfg[MAX_INPUTS] = {0,0};

const uint8_t translatePAD[MAX_PADCFG][MAX_BUTTONS] = {
    {
    0x15, // B      -- F1
    0x53, // Y      -- Y
    0xff, // SELECT ** ALT MODE
    0x62, // START  -- K
    0x00, // DUP    -- CUP
    0x02, // DDOWN  -- CDW
    0x10, // DLEFT  -- CLF
    0x01, // DRIGHT -- CRG
    0x06, // A      -- ENTER
    0x71, // X      -- N
          //------------------
    0x06, // L      -- ENTER
    0x25, // R      -- SHIFT
    0x27, // L2     -- CTRL
    0x11, // R2     -- COPY
    },
    {
    0x95, // B        -- FIRE2
    0x53, // Y        -- Y
    0xff, // SELECT   ** ALT MODE 1
    0x55, // START    -- J
    0x90, // DUP      -- JUP
    0x91, // DDOWN    -- JDW
    0x92, // DLEFT    -- JLF
    0x93, // DRIGHT   -  JRG
    0x94, // A        -- FIRE1
    0x56, // X        -- N
          //------------------
    0x22, // L        -- RETURN
    0x25, // R        -- SHIFT
    0x27, // L2       -- CTRL
    0x11, // R2       -- COPY
    },
    {
    0x80, // B      -- 1
    0x81, // Y      -- 2
    0xff, // SELECT ** ALT MODE
    0x62, // START  -- R
    0x83, // DUP    -- Q
    0x85, // DDOWN  -- A
    0x42, // DLEFT  -- O
    0x01, // DRIGHT -- P
    0x57, // A      -- SPACE
    0x71, // X      -- 3
          //------------------
    0x06, // L        -- ENTER
    0x25, // R        -- SHIFT
    0x27, // L2       -- CTRL
    0x11, // R2       -- COPY
    },
    {
    0x87, // B      -- Z
    0x57, // Y      -- SPACE
    0xff, // SELECT ** ALT MODE
    0x83|0x62, // START  -- QR
    0x74, // DUP    -- S
    0x77, // DDOWN  -- X
    0x45, // DLEFT  -- L
    0x44, // DRIGHT -- K
    0x90, // A      -- JUP
    0x56, // X      -- N
          //------------------
    0x06, // L      -- ENTER
    0x25, // R      -- SHIFT
    0x27, // L2     -- CTRL
    0x11, // R2     -- COPY
    }
};

char pad_strings[4][5] = { {"curs"}, {"joys"}, {"qaop"}, {"sxkl"}};

unsigned int special_events()
{
    unsigned int pressed = 0;
    if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_B)) {
        pressed = 1 << 0;
        if(pressed != last_event)
            kbd_buf_feed("3\n");
    } else if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_Y)) {
        pressed = 1 << 1;
        if(pressed != last_event) {
            kbd_buf_feed("1\nY\n");
        }
    } else if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_START)) {
        pressed = 1 << 3;
        if(pressed != last_event) {
            printf("SHOW KEYB\n");
            SHOWKEY=-SHOWKEY;
            memset(keyboard_matrix, 0xff, sizeof(keyboard_matrix)); // clear CPC keyboard matrix
            //memset(Retro_Screen, 0, 1024);
            Screen_SetFullUpdate(0);
        }
    } else if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_UP)) {
        pressed = 1 << 4;
        if(pressed != last_event)
            kbd_buf_feed("RUN\"DISC\n");
    } else if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_DOWN)) {
        pressed = 1 << 5;
        if(pressed != last_event)
            kbd_buf_feed("RUN\"DISK\n");
    } else if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_LEFT)) {
        pressed = 1 << 6;
        if(pressed != last_event)
            kbd_buf_feed("CAT\n");
    } else if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_RIGHT)) {
        pressed = 1 << 7;
        if(pressed != last_event)
            kbd_buf_feed("|CPM\n");
    } else if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_A)) {
        pressed = 1 << 8;
        if(pressed != last_event)
            kbd_buf_feed("4\n");
    } else if (input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_X)) {
        pressed = 1 << 9;
        if(pressed != last_event)
            kbd_buf_feed("2\nN\n");
    }

    return pressed;
}

void process_joy(int playerID){
    uint8_t * pad = (uint8_t*) &translatePAD[padcfg[playerID]];
    register int value = 0;
    for (int i = 0; i < MAX_BUTTONS; i++) {
        value = 1 << i;
        if (input_state_cb(playerID, RETRO_DEVICE_JOYPAD, 0, i)) {
            if(!(last_input[playerID] & value)) {
                keyboard_matrix[*(pad+i) >> 4] &= ~bit_values[*(pad+i) & 7]; // key is being held down
                last_input[playerID] |= value;
            }
        }
        else if (last_input[playerID] & value) {
            keyboard_matrix[*(pad+i) >> 4] |= bit_values[*(pad+i) & 7]; // key is being held down
            last_input[playerID] &= ~value;
        }
    }

}

void update_joy_cfg(int playerID){
    padcfg[playerID]++;
    if(padcfg[playerID] > 3)
        padcfg[playerID] = 0;

    app_printf("pad %u updated to: %s", playerID + 1, pad_strings[padcfg[playerID]]);
}

void gui_input()
{
    if(SHOWKEY < 0)
        return;

    input_poll_cb(); // retroarch get keys

    if( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT)) {
        last_event = special_events();
        return;
    }

    if(message_timer) {
        nk_retro_render_text(10, 10, 100, 100, message_data);
        unsigned int current = GetTicks();
        printf("time: %u, %u\n", message_timer, current);
        if(message_timer < current)
            message_timer = 0;
    }
}

void poll_input()
{
    if(SHOWKEY > 0)
        return;

    input_poll_cb(); // retroarch get keys
    if( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, RETRO_DEVICE_ID_JOYPAD_SELECT)) {
        last_event = special_events();
        return;
    }
    //Core_Processkey();
    process_joy(ID_PLAYER1);
    process_joy(ID_PLAYER2);
}

#if 0

// Core input Key(not GUI)
void Core_Processkey()
{
	int i;

	for(i=0;i<320;i++)
        	Core_Key_Sate[i]=input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0,i) ? 0x80: 0;

	if(memcmp( Core_Key_Sate,Core_old_Key_Sate , sizeof(Core_Key_Sate) ) )
	 	for(i=0;i<320;i++)
			if(Core_Key_Sate[i] && Core_Key_Sate[i]!=Core_old_Key_Sate[i]  )
        	{
				if(i==RETROK_F12){
					//play_tape();
					continue;
				}

				if(i==RETROK_LALT){
					//KBMOD=-KBMOD;
					printf("Modifier alt pressed %d \n",KBMOD);
					continue;
				}
				//printf("press: %d \n",i);
				retro_key_down(i);

        	}
        	else if ( !Core_Key_Sate[i] && Core_Key_Sate[i]!=Core_old_Key_Sate[i]  )
        	{
				if(i==RETROK_F12){
      				kbd_buf_feed("|tape\nrun\"\n^");
					continue;
				}
/*
				if(i==RETROK_RCTRL){
					CTRLON=-CTRLON;
					printf("Modifier crtl released %d \n",CTRLON);
					continue;
				}
				if(i==RETROK_RSHIFT){
					SHIFTON=-SHIFTON;
					printf("Modifier shift released %d \n",SHIFTON);
					continue;
				}
*/
				if(i==RETROK_LALT){
					KBMOD=-KBMOD;
					printf("Modifier alt released %d \n",KBMOD);
					continue;
				}
				//printf("release: %d \n",i);
				retro_key_up(i);

        	}

	memcpy(Core_old_Key_Sate,Core_Key_Sate , sizeof(Core_Key_Sate) );

}

// Core input (not GUI)
int Core_PollEvent_old()
{
	//   RETRO        B    Y    SLT  STA  UP   DWN  LEFT RGT  A    X    L    R    L2   R2   L3   R3
    //   INDEX        0    1    2    3    4    5    6    7    8    9    10   11   12   13   14   15
    //   AMSTRAD      RUN  VKB  M/J  RTRN UP   DWN  LEFT RGT  B1   B2   CAT  RST  STAT TAPE ?    ?

   int i;
   static int jbt[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
   static int vbt[16]={0x1C,0x39,0x01,0x3B,0x01,0x02,0x04,0x08,0x80,0x40,0x15,0x31,0x24,0x1F,0x6E,0x6F};
   static int kbt[4]={0,0,0,0};

   MXjoy[0]=0;

   input_poll_cb();

   int mouse_l;
   int mouse_r;
   int16_t mouse_x,mouse_y;
   mouse_x=mouse_y=0;

   if(SHOWKEY==-1 && pauseg==0)Core_Processkey();

   // F9 vkbd
   i=0;
   if (input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_F9) && kbt[i]==0){
      kbt[i]=1;
   }
   else if ( kbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_F9) ){
      kbt[i]=0;
      SHOWKEY=-SHOWKEY;
   }
   // F10 GUI
   i=1;
   if (input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_F10) && kbt[i]==0){
      kbt[i]=1;
   }
   else if ( kbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_KEYBOARD, 0, RETROK_F10) ){
      kbt[i]=0;
      pauseg=1;
      save_bkg();
      printf("enter gui!\n");
   }

/*
if(amstrad_devices[0]==RETRO_DEVICE_AMSTRAD_JOYSTICK){

    i=2;//mouse/joy toggle
   if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && jbt[i]==0 )
      jbt[i]=1;
   else if ( jbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
      jbt[i]=0;
      retro_evt->MOUSE_EMULATED=-retro_evt->MOUSE_EMULATED;
   }

}
*/

if(pauseg==0){ // if emulation running

	  //Joy mode

      for(i=4;i<11;i++)
      {
         if( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i)){
           printf("vbt[%i] = %x\n", i, vbt[i]);
            MXjoy[0] |= vbt[i]; // Joy press
         }
      }

      if(SHOWKEY==-1)retro_joy0(MXjoy[0]);


if(amstrad_devices[0]==RETRO_DEVICE_AMSTRAD_JOYSTICK){
   //shortcut for joy mode only

   i=1;//show vkbd toggle
   if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && jbt[i]==0 )
      jbt[i]=1;
   else if ( jbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) )
   {
      jbt[i]=0;
      SHOWKEY=-SHOWKEY;
   }

   i=3;//type ENTER
   if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && jbt[i]==0 )
      jbt[i]=1;
   else if ( jbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) )
   {
      jbt[i]=0;
	  kbd_buf_feed("\n");
   }

   i=0;//type RUN"
   if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && jbt[i]==0 )
      jbt[i]=1;
   else if ( jbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
      jbt[i]=0;
	  kbd_buf_feed("RUN\"");
   }

   i=10;//Type CAT\n
   if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && jbt[i]==0 )
      jbt[i]=1;
   else if ( jbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
      jbt[i]=0;
	  kbd_buf_feed("CAT\n");
      //Screen_SetFullUpdate();
   }

   i=12;//show/hide statut
   if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && jbt[i]==0 )
      jbt[i]=1;
   else if ( jbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
      jbt[i]=0;
      STATUTON=-STATUTON;
     // Screen_SetFullUpdate();
   }

   i=13;//auto load tape
   if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && jbt[i]==0 )
      jbt[i]=1;
   else if ( jbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
      jbt[i]=0;
      kbd_buf_feed("|tape\nrun\"\n^");
   }

   i=11;//reset
   if ( input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) && jbt[i]==0 )
      jbt[i]=1;
   else if ( jbt[i]==1 && ! input_state_cb(0, RETRO_DEVICE_JOYPAD, 0, i) ){
      jbt[i]=0;
      emu_reset();
   }

}//if amstrad_devices=joy


}// if pauseg=0

return 1;

}
#endif
