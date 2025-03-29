/*
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

*/
#ifndef __VIRTUAL_KEYBOARD_H
#define __VIRTUAL_KEYBOARD_H

#define VIRTUAL_KEYBOARD_NEWLINE "__newline__"
#include <imgui.h>

#ifndef VIRTUAL_KEYBOARD_NO_STD
#include <string>
#endif
namespace ImGui{

namespace VirtualKeyboard{

static const char *layout_en_lower[] = {
"q","w","e","r","t","y","u","i","o","p",VIRTUAL_KEYBOARD_NEWLINE,
"a","s","d","f","g","h","j","k","l",VIRTUAL_KEYBOARD_NEWLINE,
"z","x","c","v","b","n","m",
NULL
}; // It should always end with null and 
static const char *layout_en_upper[] = {
"Q","W","E","R","T","Y","U","I","O","P",VIRTUAL_KEYBOARD_NEWLINE,
"A","S","D","F","G","H","J","K","L",VIRTUAL_KEYBOARD_NEWLINE,
"Z","X","C","V","B","N","M",
NULL
};
static const char *layout_ru_lower[] = {
u8"й",u8"ц",u8"у",u8"к",u8"е",u8"н",u8"г",u8"ш",u8"щ",u8"з",u8"х",VIRTUAL_KEYBOARD_NEWLINE,
u8"ф",u8"ы",u8"в",u8"а",u8"п",u8"р",u8"о",u8"л",u8"д",u8"ж",u8"э",VIRTUAL_KEYBOARD_NEWLINE,
u8"я",u8"ч",u8"с",u8"м",u8"и",u8"т",u8"ь",u8"б",u8"ю",
NULL
};
static const char *layout_ru_upper[] = {
u8"Й",u8"Ц",u8"У",u8"К",u8"Е",u8"Н",u8"Г",u8"Ш",u8"Щ",u8"З",u8"Х",VIRTUAL_KEYBOARD_NEWLINE,
u8"Ф",u8"Ы",u8"В",u8"А",u8"П",u8"Р",u8"О",u8"Л",u8"Д",u8"Ж",u8"Э",VIRTUAL_KEYBOARD_NEWLINE,
u8"Я",u8"Ч",u8"С",u8"М",u8"И",u8"Т",u8"Ь",u8"Б",u8"Ю",
NULL
};
static const char *layout_123[] = {
"1","2","3","4","5","6","7","8","9","0",VIRTUAL_KEYBOARD_NEWLINE,
"@","#","$","%","&","-","+","(",")",VIRTUAL_KEYBOARD_NEWLINE,
"*","\"","\'",":",";","!","?",NULL
};
/* Here your layout should be declared as:
   {{layout_lower,layout_upper},{layout_lower2,layout_upper2},
   {NULL,NULL}}
*/
struct layoutArray
{
    const char**lower,**upper;
    const char **operator [](bool upc)
    {
        return upc?upper:lower;
    }
};
static layoutArray layouts[] = {
{(layout_en_lower),(layout_en_upper)},
{(layout_ru_lower),(layout_ru_upper)},
{(layout_123),(layout_123)},
{NULL,NULL} // Don't forget to finish it
};
enum{
    LAYOUT_EN=0,
    LAYOUT_RU,
    LAYOUT_123,
    LAYOUT_NONE
};
#ifndef VIRTUAL_KEYBOARD_NO_STD
extern void SetInputPtr(std::string *input);
#endif
extern void SetInputPtr(char **text,int len);
extern bool Keyboard(int setlayout = LAYOUT_EN,float kbd_height=0.40);
extern void Toggle(bool toggle); // In case you want to hide the keyboard, draw it when an item is clicked, call this function
};

};


#endif