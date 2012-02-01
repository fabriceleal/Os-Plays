/**
 * @file monitor.c
 * @author Fabrice Leal <fabrice [dot]leal[dot]ch[at]gmail[dot]com>
 *
 * @section LICENSE
 *
 * < ... goes here ... >
 *
 * @section DESCRIPTION
 * 
 * < ... goes here ... >
 *
 */

#include "monitor.h"

/*

*/
#define make_char_attribute(backcolor,forecolor) ((((backcolor) & 0xF) <<  4) | ((forecolor) & 0xF))
#define make_char(backcolor,forecolor,character) ((((backcolor) & 0xF) << 12) | (((forecolor) & 0xF) << 8) | ((character) & 0xFF))

static u16int cursor_y = 0;
static u16int cursor_x = 0;
static e_color backcolor = e_color_black;
static e_color forecolor = e_color_white;

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25
#define tr_to_linear(x,y) (((y) * SCREEN_WIDTH) + (x))

static u16int * video_memory = (u16int*)0xB8000; // Start of VGA buffer

void monitor_get_backcolor(e_color *result)
{
	*result = backcolor;
}

void monitor_get_forecolor(e_color *result)
{
	*result = forecolor;
}

void monitor_set_backcolor(e_color new_value)
{
	backcolor = new_value;
}

void monitor_set_forecolor(e_color new_value)
{
	forecolor = new_value;
}

// We don't actually need the cursor to actually write in the monitor's buffer, but it is cute to see the blink'in
static void move_cursor()
{
   // The screen is 80 characters wide
   u16int cursorLocation = tr_to_linear(cursor_x, cursor_y); // cursor_y * 80 + cursor_x;

   outb(0x3D4, 14);                  // Tell the VGA board we are setting the high cursor byte.
   outb(0x3D5, cursorLocation >> 8); // Send the high cursor byte.
   outb(0x3D4, 15);                  // Tell the VGA board we are setting the low cursor byte.
   outb(0x3D5, cursorLocation);      // Send the low cursor byte.
}

// Scrolls the text on the screen up by one line.
static void scroll()
{

   // Get a space character with the default colour attributes.
   //u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
   //u16int blank = 0x20 /* space */ | (attributeByte << 8);
	u16int blank = make_char(backcolor, forecolor, (int) ' ');

   // Row 25 is the end, this means we need to scroll up
   if(cursor_y >= SCREEN_HEIGHT)
   {
#define FIRST_ROW_INDEX 0 // Obviously! :-|
#define LAST_ROW_INDEX (SCREEN_HEIGHT-1)

       // Move the current text chunk that makes up the screen
       // back in the buffer by a line
       int i;
       for (i = FIRST_ROW_INDEX*SCREEN_WIDTH ; i < LAST_ROW_INDEX*SCREEN_WIDTH ; i++)
       {
           video_memory[i] = video_memory[i+SCREEN_WIDTH];
       }

       // The last line should now be blank. Do this by writing
       // 80 spaces to it.
       for (i = LAST_ROW_INDEX*SCREEN_WIDTH ; i < SCREEN_HEIGHT*SCREEN_WIDTH ; i++)
       {
           video_memory[i] = blank;
       }
       // The cursor should now be on the last line.
       cursor_y = LAST_ROW_INDEX;

#undef FIRST_ROW_INDEX
#undef LAST_ROW_INDEX
   }
}

// Writes a single character out to the screen.
void monitor_put(char c)
{
   // The background colour is black (0), the foreground is white (15).
   //u8int backColour = 0;
   //u8int foreColour = 15;

   // The attribute byte is made up of two nibbles - the lower being the
   // foreground colour, and the upper the background colour.
   //u8int  attributeByte = (backColour << 4) | (foreColour & 0x0F);
   // The attribute byte is the top 8 bits of the word we have to send to the
   // VGA board.
   //u16int attribute = attributeByte << 8;
	
	u16int typeme = make_char(backcolor, forecolor, (int)c);
   u16int *location;
	

   // Handle a backspace, by moving the cursor back one space
   if (c == 0x08 && cursor_x)
   {
       cursor_x--;
   }

   // Handle a tab by increasing the cursor's X, but only to a point
   // where it is divisible by 8.
   else if (c == 0x09)
   {
       cursor_x = (cursor_x+8) & ~(8-1); // :S
   }

   // Handle carriage return
   else if (c == '\r')
   {
       cursor_x = 0;
   }

   // Handle newline by moving cursor back to left and increasing the row
   else if (c == '\n')
   {
       cursor_x = 0;
       cursor_y++;
   }
   // Handle any other printable character.
   else if(c >= ' ')
   {
       location = video_memory + tr_to_linear(cursor_x, cursor_y); //(cursor_y*80 + cursor_x);
       //*location = c | attribute;
		 *location = typeme;
       cursor_x++;
   }

   // Check if we need to insert a new line because we have reached the end
   // of the screen.
   if (cursor_x >= SCREEN_WIDTH)
   {
       cursor_x = 0;
       cursor_y ++;
   }

   // Scroll the screen if needed.
   scroll();
   // Move the hardware cursor.
   move_cursor();
} 

// Clears the screen, by copying lots of spaces to the framebuffer.
void monitor_clear()
{
   // Make an attribute byte for the default colours
   //u8int attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
   //u16int blank = 0x20 /* space */ | (attributeByte << 8);

	u16int blank = make_char(backcolor, forecolor, (int)' ');

   int i;
   for (i = 0; i < SCREEN_WIDTH*SCREEN_HEIGHT; i++)
   {
       video_memory[i] = blank;
   }

   // Move the hardware cursor back to the start.
   cursor_x = 0;
   cursor_y = 0;

   move_cursor();
}

// Outputs a null-terminated ASCII string to the monitor.
void monitor_write(char *c)
{
   int i = 0;
   while (c[i])
   {
       monitor_put(c[i++]);
   }
}


void monitor_write_hex(u32int n)
{
    s32int tmp;

    monitor_write("0x");

    char noZeroes = 1;

    int i;
    for (i = 28; i > 0; i -= 4)
    {
        tmp = (n >> i) & 0xF;
        if (tmp == 0 && noZeroes != 0)
        {
            continue;
        }
    
        if (tmp >= 0xA)
        {
            noZeroes = 0;
            monitor_put (tmp-0xA+'a' );
        }
        else
        {
            noZeroes = 0;
            monitor_put( tmp+'0' );
        }
    }
  
    tmp = n & 0xF;
    if (tmp >= 0xA)
    {
        monitor_put (tmp-0xA+'a');
    }
    else
    {
        monitor_put (tmp+'0');
    }

}

void monitor_write_dec(u32int n)
{

    if (n == 0)
    {
        monitor_put('0');
        return;
    }

    s32int acc = n;
    char c[32];
    int i = 0;
    while (acc > 0)
    {
        c[i] = '0' + acc%10;
        acc /= 10;
        i++;
    }
    c[i] = 0;

    char c2[32];
    c2[i--] = 0;
    int j = 0;
    while(i >= 0)
    {
        c2[i--] = c[j++];
    }
    monitor_write(c2);

}

void printf(){
	// Refer to vsprintf at the linux0.1 code
	// TODO: ...
	return;
}
