/* 		LegOS Copyright(C) 2025 Elad Yaakobi
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

//This file is for the memory maneger of LegOS: memleg

#include "../include/memleg.h"  

#define MAX_HEAP_SIZE 0x100000
// a basic heap tool- create a variabe in a big size you can write data to.
unsigned char heap[MAX_HEAP_SIZE];

static unsigned int heap_index = 0;


// a basic function to allocate memory from the heap: you pass a certain size you want to allocate,
// point to the first free area in the heap using the heap index, then increese the index by the size,
// now you have a pointer you can store data to in the heap, and said data is marked by the undex as occupied.
void* kmalloc(unsigned int size) {
    if(heap_index + size >= MAX_HEAP_SIZE) {
        return 0; // Out of memory
    }
    void* ptr = &heap[heap_index];
    heap_index += size;
    return ptr;
}

