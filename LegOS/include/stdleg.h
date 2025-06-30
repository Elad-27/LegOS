/* 		LegOS Copyright(C) 2025 Elad Yaakobi
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

//This file is for the standard C library of LegOS: stdleg

#ifndef STDLEG_H
#define STDLEG_H

#include "ps2_keyboard.h"
#include "fonts.h"
#include "VGA_driver.h"
#include "pit.h"
#include "shapes.h"
#include "stringlib.h"

typedef struct window
{
    unsigned short max_height;
    unsigned short max_width;
    unsigned char font;
    unsigned char text_color;
    unsigned char background_color;
}window_t;

typedef struct user
{
    unsigned char* username;
    unsigned char* password;
}user_t;


unsigned char GetNextIOStream();
unsigned char* GetLastNChars(unsigned char n, unsigned char* result);
void textwindow_init();
void NewWindow(unsigned short x, unsigned short y, window_t window);
unsigned char GetKeyWidth();
unsigned char GetKeyLength();
void SetUsrName(unsigned char* username);
void SetUsrPass(unsigned char* password);
void ReturnDefaultWindow();
void set_color(unsigned char colour);
void set_BGcolor(unsigned char colour);
unsigned char GetColor();
unsigned char GetBGColor();
void set_font(unsigned char new_font);
void clear_window();
bool read_key();
void write_char(unsigned char character);
void write_string(unsigned char* str);
unsigned char GetChar();
unsigned char* Read_Line();
void BumpX(int x);

#endif