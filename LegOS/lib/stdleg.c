/* 		LegOS Copyright(C) 2025 Elad Yaakobi
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

/**This file is for the standard C library of LegOS: stdleg
 **it includes but not limited to: 
 * functions to echo and recive data,
 * an i/o stream
 * Methods to control GUI properties such as: 
 * window hight / width, 
 * font size, 
 * color schemes   
*/

#include "../include/stdleg.h"

unsigned char iostream[KEYBOARD_BUFFER_SIZE]; 
int iostream_head = 0;
int iostream_tail = 0;
// this is a stack keeping track of the *ascii chars* (not raw codes) a user is typing, and used to retrieve data from.
// this might seem redundant to have 3 different variables keeping track of the keyboard, but the different queues and stacks are as similar as they are dissimilar;
// they are used for different things and goals and sperating them like this avoids overly complex logic and more curated data for said goals.

user_t global_user;

unsigned char GetNextIOStream() {
    if (iostream_head == iostream_tail) {
        return 0; // no key
    }
    
    unsigned char character = iostream[iostream_tail];
    iostream_tail = (iostream_tail + 1) % KEYBOARD_BUFFER_SIZE;

    return character;
}

unsigned char* GetLastNChars(unsigned char n, unsigned char* result) {
    if (n > iostream_head - iostream_tail) {
        return (unsigned char*)"exceeded size limit";
    }
    unsigned char* temp = result;
    unsigned char data = 0;

    for (unsigned char i = 0; i < n; i++)
    {
        data = GetNextIOStream();
        if (data == '\b' && result - temp != 0)
        {
            result--;
        }
        
        if (data >= 32)
        {
            *result = data;
            result++;
        }        
    }
    *result = '\0';

    return temp;
}

void Empty_Stream() {
    while (iostream_tail < iostream_head)
    {
        GetNextIOStream();
    }
    
}

// as per VGA 13h mode:
static const unsigned char MAX_WINDOW_HEIGHT = 200;
static const unsigned short MAX_WINDOW_WIDTH = 320;

unsigned char dynamic_window_height;
unsigned short dynamic_window_width;

unsigned char start_y;
unsigned short start_x;
unsigned char window_row;
unsigned short window_column;
unsigned char text_color;
unsigned char background_color;
unsigned char* font;
unsigned char key_length;
unsigned char key_width;

window_t default_window = {
    MAX_WINDOW_HEIGHT,
    MAX_WINDOW_WIDTH,
    2,
    vga13h_White,
    vga13h_Black
};

void textwindow_init() {
    dynamic_window_height = MAX_WINDOW_HEIGHT;    
    dynamic_window_width = MAX_WINDOW_WIDTH;
    window_row = 0;
    window_column = 0;
    start_y = 0;
    start_x = 0;
    font = "6x8";
    key_width = 6;
    key_length = 8;
    text_color = vga13h_White;
    background_color = 0;
    clear_screen();
}

void NewWindow(unsigned short x, unsigned short y, window_t window) {
    dynamic_window_height = window.max_height;
    dynamic_window_width = window.max_width;
    window_row = y;
    start_y = y;
    window_column = x;
    start_x = x;
    set_font(window.font);
    text_color = window.text_color;
    background_color = window.background_color;
}

unsigned char GetKeyWidth() {
    return key_width;
}

unsigned char GetKeyLength() {
    return key_length;
}

void SetUsrName(unsigned char* username) {
    global_user.username = username;   
}

void SetUsrPass(unsigned char* password) {
    global_user.password = password;   
}

void ReturnDefaultWindow() {
    textwindow_init();
}

void set_color(unsigned char colour) {
    text_color = colour;
}

void set_BGcolor(unsigned char colour) {
    background_color = colour;
}

unsigned char GetColor() {
    return text_color;
}

unsigned char GetBGColor() {
    return background_color;
}

void set_font(unsigned char new_font) {
//                       value:  |0 =   |1 =    |2 =   |3 =    |4 =    |5 =     |6 = 
    unsigned char* font_list[] = {"4x6", "5x12", "6x8", "7x12", "8x12", "10x18", "12x16"};
    unsigned char format[][2] = {{4, 6}, {5, 12}, {6, 8}, {7, 12}, {8, 12}, {10, 18}, {12, 16}};
    font = font_list[new_font % 7];
    key_width = *(format[new_font]);
    key_length = *(format[new_font] + 1);
}

void clear_window() {
    for (int x = 0; x < dynamic_window_width; x++)
    {
        for (int y = 0; y < dynamic_window_height; y++)
        {
            draw_pixel(x, y, background_color);
        }
    }
    window_column = start_x;
    window_row = start_y;
}

// function to read the area currently the curser is on from the screen and returns true if a char was typed there, and false if not. 
bool read_key() {
    for (int y = window_row; y < window_row + key_length; y++)
    {
        for (int x = window_column; x < window_column + key_width; x++)
        {
            if (read_pixel(x, y) == text_color) {
                return true;
            }
        }
    }
    return false;    
}

void write_char(unsigned char character) {
    if (character == '\n') {
        window_row += key_length + 2;
        window_column = start_x;
    }

    if (character == '\t') {
        for (unsigned char i = 0; i < 4; i++)
        {
            draw_char(window_column % dynamic_window_width, window_row % dynamic_window_height, font, ' ', text_color);
            window_column += key_width;
            if (window_column + key_width - start_x >= dynamic_window_width) {
                window_column = start_x;
                if (window_row + 2 * key_length - start_y < dynamic_window_height)
                {
                    window_row += key_length + 2;
                }        
            }
        }
    }

    if (character == '\b') {
        if (window_column == start_x && window_row > start_y)
        {
            window_row -= key_length + 2;
            window_column = dynamic_window_width - key_width - dynamic_window_width % key_width;
            while (!read_key())
            {
                window_column -= key_width;
                if(window_column <= start_x) {
                    window_column = start_x;
                    break;
                }
            }
            if (read_key())
            {
                window_column += key_width;
            }            
            
        }
        else if (window_column != start_x)
        {
            window_column =  (window_column - key_width);
            draw_rectangle(window_column, window_row, key_width, key_length, background_color);  
        }
    }

    if (character >= 32 && window_column + key_width - start_x < dynamic_window_width && window_row + key_length + 2 - start_y < dynamic_window_height) {
        draw_char(window_column, window_row, font, character, text_color);
        window_column += key_width;
    }

    if (window_column + key_width - start_x >= dynamic_window_width) {
        window_column = start_x;
        if (window_row + 2 * key_length - start_y < dynamic_window_height)
        {
            window_row += key_length + 2;
        }        
    }
}

void write_string(unsigned char* str) {
    while (*str)
    {
        write_char(*str);
        *str++;
    }
}

unsigned char GetChar() {
    unsigned char character = ScancodeToAscii(get_last_keycode());
    if (character)
    {
        int next = (iostream_head + 1) % KEYBOARD_BUFFER_SIZE;
        if (next != iostream_tail) {
            iostream[iostream_head] = character;
            iostream_head = next;
        }
        return character;
    }

    return character;
}


unsigned char* Read_Line() {
    Empty_Stream();

    unsigned char* result = "test";
    unsigned char counter = 0;
    unsigned char character = GetChar();
    unsigned char colors[2] = {text_color, background_color};
    unsigned char index = 0;
    unsigned int target = GetTicks() + 50;
    while (character != '\n')
    {
        // basically, to make a blinking curser I just print the underscore character _ , wait around 50 ms and then print it again with the bg color.
        // because we don't want any delay while typing we can use the PIT to count ticks, since the PIT runs in the backgronud we can use it instead of manually waiting or counting. 
        draw_char(window_column + 1, window_row, font, '_', colors[index % 2]); 
        if (GetTicks() >= target)
        {
            draw_char(window_column + 1, window_row, font, '_', colors[index % 2]);
            index++;
            target = GetTicks() + 50;
        }
        character = GetChar();
        if (character && character != '\n') {
            draw_char(window_column + 1, window_row, font, '_', background_color);
            counter++;
            write_char(character);
        }
        else
        {
            asm("hlt");
        }
    }
    draw_char(window_column + 1, window_row, font, '_', background_color);
    if (character == '\n') {
        write_char(character);
    }    

    result = GetLastNChars(counter, "");
    return result;
}

//function to "bump" the dynamic width of the current window by a certain size;
//this can be used to seemlessly change between widths such that the system writes to places then 
//adjusts itself so the user wouldn't be able to type on the same area.
//for example: the "username/$: |___" part of the shell 
void BumpX(int x) {
    start_x += x;
    window_column = start_x;
}