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
#include <ui/VirtualKeyboard.h>
#include <emscripten/console.h>

namespace ImGui{
namespace VirtualKeyboard{
        int layout = LAYOUT_NONE;
        bool layout_changed = true; // We need to pre-calculate number of items on every keyboard line
        bool shift_toggled = false;
        bool shift_toggled_once = false;
        bool toggled = true;
        int width,height;

        std::string *input_str = NULL;

        int win_height;
        int win_width;

        ImU32 ButtonColor = IM_COL32(71,71,71,255);
        ImU32 ButtonClickedColor = IM_COL32(124,124,124,255);
        ImU32 BgColor = IM_COL32_BLACK;
        ImU32 LabelColor = IM_COL32_WHITE;
        float ButtonRounding = 7.0f;
        #define KBD_MARGIN 10

        int numkeys[3];
        int getNumLayouts()
        {
            int i;
            for(i=0;layouts[i][0];i++);
            return i;
        }
        void Toggle(bool toggle)
        {
            toggled = toggle;
        }
        #define KBD_LAYOUT layouts[layout][shift_toggled||shift_toggled_once]
        void RecalculateNumKeys()
        {
            static int i;
            #define KBD_NEXT_ROW (!strcmp(KBD_LAYOUT[i],VIRTUAL_KEYBOARD_NEWLINE))
            #define KBD_LAST_ROW (!KBD_LAYOUT[i])
            numkeys[0] = numkeys[1] = numkeys[2] = 0;


            for(i=0;!KBD_NEXT_ROW;i++)numkeys[0]++;
            for(++i;!KBD_NEXT_ROW;i++)numkeys[1]++;
            for(++i;!KBD_LAST_ROW;i++)numkeys[2]++;
        }
        inline bool KeyButton(const char *key,int x,int y,int w,int h,ImDrawList* draw_list,bool labelleft = false)
        {
            static bool clicked;
            static bool smalltext;
            clicked = false;
            if(w<h)labelleft = true;
            if(IsMouseClicked(ImGuiMouseButton_Left))
            {
                ImVec2 mpos = GetMousePos();
                clicked = (mpos.x>=x&&mpos.y>=y&&mpos.x<=(x+w)&&mpos.y<=(y+h));
            }
            draw_list->AddRectFilled(ImVec2(x,y),ImVec2(x+w,y+h),
                clicked?ButtonClickedColor:ButtonColor,
                ButtonRounding,
                ImDrawFlags_RoundCornersAll);
            draw_list->AddText(ImGui::GetFont(),labelleft?(h/2):h,ImVec2(labelleft?x+KBD_MARGIN/2:(x+w/2-KBD_MARGIN/2),!labelleft?y:(y+h/2-KBD_MARGIN/2)),LabelColor,key);
            return clicked;
        }
        void KeyRow(int row,ImDrawList* draw_list)
        {
            int offs = 0;
            for(int i=0;i<row;i++)offs += numkeys[i]+1;
            int xoffset = 0; // Offset for special keys. Comment out the next switch section to disable it.
            switch(row){
                case(0):xoffset = 0;break;
                case(1):xoffset = width * 0.1;break;
                case(2):xoffset = width * 0.18;break;
            }
            //emscripten_console_log(std::string("Offset:" + std::to_string(offs)).c_str());
            int btnwidth = (width-xoffset)/numkeys[row]-KBD_MARGIN/2;
            int btnheight=height/4-KBD_MARGIN/2;
            int ycursor = win_height - height + (KBD_MARGIN/2 + btnheight) * row;
            /*std::string s = "btnwidth:" + std::to_string(btnwidth) + "btnheight:" + std::to_string(btnheight) + " layouts: " + std::to_string(getNumLayouts());
            emscripten_console_log(s.c_str());*/
            for(int i=offs;i<offs+numkeys[row];i++)
            {
                int xcursor = i - offs;
                if(KeyButton(KBD_LAYOUT[i],xcursor*btnwidth + KBD_MARGIN/2+xoffset,ycursor,btnwidth-KBD_MARGIN/2,btnheight,draw_list))
                {
                    if(input_str)*input_str += KBD_LAYOUT[i];
                    ImGui::GetIO().AddInputCharactersUTF8(KBD_LAYOUT[i]);
                    if(shift_toggled_once)shift_toggled_once = false;
                }
            }
            //emscripten_console_log(std::string("Row render complete"+std::to_string(row)).c_str());
        }
        void input_pop_back(std::string* utf8) // Delete last UTF-8 character (NOT BYTE)
        {
            int len = utf8->length();
            short del_bytes = 1; // Minimal value
            const char * s = utf8->c_str();
            if((s[len-1]&128)) // The last character(ignoring null-terminated symbol) is not a single-byte character: bit mask 10000000
            {
                if(len>1&&(s[len-2]&192)) // Two bytes character: bit mask of the first byte is 110
                    del_bytes = 2;
                else if(len>2&&(s[len-3]&224))del_bytes = 3; // Three bytes character: 1110
                else if(len>3&&(s[len-4]&240))del_bytes = 4; // Four bytes character 11110
            }
            for(int i=0;i<del_bytes;i++)utf8->pop_back();
        }

        void FnKeys(ImDrawList* draw_list)
        {
            if(KeyButton(u8"↑",KBD_MARGIN/2,win_height-height+height/4,width*0.1-KBD_MARGIN/2,height/4-KBD_MARGIN/2,draw_list))shift_toggled=!shift_toggled;
            
            if(KeyButton(u8"⌫",(width/numkeys[0]-KBD_MARGIN/2)*numkeys[0]+KBD_MARGIN/2,win_height-height,width-((width/numkeys[0]-KBD_MARGIN/2)*numkeys[0]+KBD_MARGIN/2)-KBD_MARGIN/2,height/4-KBD_MARGIN/2,draw_list)&&input_str&&input_str->length())input_pop_back(input_str);
            if(KeyButton("Shift",KBD_MARGIN/2,win_height-height/2,width*0.15,height/4,draw_list,true)){shift_toggled_once = true;}
            if(KeyButton("<>",width *0.15,win_height-height/4,width*0.1-KBD_MARGIN/2,height/4-KBD_MARGIN/2,draw_list))
            {
                layout++; // If the last layout reached, layout will be reset on the next cycle
                if(layout<0||layout>=LAYOUT_123)layout = LAYOUT_NONE;
                layout_changed = true;
            }
            if(KeyButton("123",width*0.75+KBD_MARGIN/2,win_height-height/4,width*0.1-KBD_MARGIN/2,height/4-KBD_MARGIN/2,draw_list,true))
            {
                layout = LAYOUT_123;
                layout_changed = true;
            }
            if(KeyButton(",",KBD_MARGIN/2,win_height-height/4,width*0.075-KBD_MARGIN/2,height/4-KBD_MARGIN/2,draw_list)){}
            if(KeyButton(".",width*0.075+KBD_MARGIN/2,win_height-height/4,width*0.075-KBD_MARGIN/2,height/4-KBD_MARGIN/2,draw_list)){}
            if(KeyButton(" ",width*0.25,win_height-height/4,width*0.5,height/4-KBD_MARGIN/2,draw_list))*input_str += " ";
        }
        void SetInputPtr(std::string *input)
        {
            input_str = input;
        }
        bool Keyboard(int setlayout,float kbd_height)
        {
            if(!toggled||!input_str)return false; // we want to hide the keyboard
            static int num_layouts = getNumLayouts(); // We don't expect it to change since the initialization
            if(layout==LAYOUT_NONE)
            {
                layout_changed = true;
                layout = setlayout;
            }
            if(layout<0||layout>=num_layouts)
            {
                layout = LAYOUT_EN;
                layout_changed = true;
            }
            win_width = ImGui::GetWindowWidth();
            win_height = ImGui::GetWindowHeight();
            width = win_width - KBD_MARGIN;
            height = win_height * kbd_height - KBD_MARGIN;
            if(layout_changed)
            {
                RecalculateNumKeys();
                layout_changed = false;
            }
            // Drawing
            ImDrawList* draw_list = GetWindowDrawList();
            draw_list->AddRectFilled(
                ImVec2(0,win_height-height-KBD_MARGIN/2),  ImVec2(width,win_height),//coords
                BgColor, // Color ImU32(000000000000=Black)
                0.0f, // Rounding
                0 // Draw flags
            );
            KeyRow(0,draw_list);
            KeyRow(1,draw_list);
            KeyRow(2,draw_list);
            FnKeys(draw_list);
            return false;
        }
    }
}