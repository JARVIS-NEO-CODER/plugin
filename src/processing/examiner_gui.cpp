#if defined(_WIN32)

#include "examiner_gui.hpp"
#include "examiner_camera_bridge.hpp"
#include "prism/traffic/game_traffic.hpp"
#include "prism/traffic/objects/traffic_player_vehicle.hpp"

#include <Windows.h>
#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>

namespace examiner_gui {
namespace {
constexpr wchar_t kClassName[] = L"ETS2ExaminerOverlay";
constexpr int kWidth = 460;
constexpr int kHeight = 620;
HWND g_overlay = nullptr;
HWND g_game = nullptr;
bool g_visible = false;
int g_selected_player = -1;
ULONGLONG g_last_toggle = 0;
struct PlayerEntry { int id; std::uintptr_t vehicle; std::wstring name; };
std::vector<PlayerEntry> g_players;

void set_click_through(bool click_through) {
    if (!g_overlay) return;
    LONG_PTR ex = GetWindowLongPtrW(g_overlay, GWL_EXSTYLE);
    if (click_through) ex |= WS_EX_TRANSPARENT | WS_EX_NOACTIVATE;
    else ex &= ~WS_EX_TRANSPARENT;
    SetWindowLongPtrW(g_overlay, GWL_EXSTYLE, ex);
}
void position_overlay() {
    if (!g_overlay || !g_game || !IsWindow(g_game)) return;
    RECT r{}; if (!GetClientRect(g_game, &r)) return;
    POINT p{r.left, r.top}; ClientToScreen(g_game, &p);
    SetWindowPos(g_overlay, HWND_TOPMOST, p.x, p.y, r.right-r.left, r.bottom-r.top, SWP_NOACTIVATE|SWP_SHOWWINDOW);
}
void draw_text(HDC dc, int x, int y, const std::wstring& text, int size=18) {
    HFONT font=CreateFontW(size,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY,DEFAULT_PITCH|FF_DONTCARE,L"Segoe UI");
    HFONT old=(HFONT)SelectObject(dc,font); SetBkMode(dc,TRANSPARENT); SetTextColor(dc,RGB(235,240,245));
    TextOutW(dc,x,y,text.c_str(),static_cast<int>(text.size())); SelectObject(dc,old); DeleteObject(font);
}
void draw_button(HDC dc, RECT row, const std::wstring& text, bool active) {
    HBRUSH b=CreateSolidBrush(active?RGB(45,80,110):RGB(30,36,44)); FillRect(dc,&row,b); DeleteObject(b); draw_text(dc,row.left+12,row.top+9,text,15);
}
void refresh_players() {
    std::vector<PlayerEntry> fresh;
    auto* traffic=ets2la_plugin::prism::game_traffic_u::get(); if (!traffic) return;
    int id=0;
    for(auto* player:traffic->traffic_player_vehicles_1) {
        if(!player || !player->traffic_player_object) continue;
        PlayerEntry e{}; e.id=id++; e.vehicle=reinterpret_cast<std::uintptr_t>(player); e.name=L"Joueur "+std::to_wstring(e.id+1); fresh.push_back(std::move(e));
    }
    if(g_selected_player>=static_cast<int>(fresh.size())) { g_selected_player=-1; ets2la_plugin::examiner_camera_bridge::clear_player_vehicle(); }
    g_players.swap(fresh);
}
void select_entry(int index) {
    if(index<0 || index>=static_cast<int>(g_players.size())) return;
    g_selected_player=g_players[index].id; ets2la_plugin::examiner_camera_bridge::set_player_vehicle(g_players[index].vehicle);
    if(g_overlay) InvalidateRect(g_overlay,nullptr,FALSE);
}
void set_mode(ets2la_plugin::examiner_camera_bridge::camera_mode mode) {
    ets2la_plugin::examiner_camera_bridge::set_camera_mode(mode); if(g_overlay) InvalidateRect(g_overlay,nullptr,FALSE);
}
const wchar_t* mode_name(ets2la_plugin::examiner_camera_bridge::camera_mode mode) {
    using M=ets2la_plugin::examiner_camera_bridge::camera_mode;
    switch(mode){case M::cabin:return L"1  Cabine";case M::rear:return L"2  Derrière";case M::free:return L"3  Extérieure";case M::overhead:return L"4  Dessus";} return L"";
}

LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch(msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps{}; HDC dc=BeginPaint(hwnd,&ps); RECT r{}; GetClientRect(hwnd,&r);
        HBRUSH bg=CreateSolidBrush(RGB(18,22,28)); FillRect(dc,&r,bg); DeleteObject(bg);
        draw_text(dc,24,20,L"EXAMINATEUR",26); draw_text(dc,24,58,L"Sélectionner un joueur Convoy",17);
        int y=105;
        if(g_players.empty()) { draw_text(dc,24,y,L"Aucun joueur détecté",17); y+=38; draw_text(dc,24,y,L"Rejoins un Convoy pour afficher",14); y+=24; draw_text(dc,24,y,L"les véhicules des autres joueurs.",14); y+=55; }
        else for(size_t i=0;i<g_players.size();++i){ RECT row{20,y,440,y+44}; draw_button(dc,row,g_players[i].name,g_players[i].id==g_selected_player); if(g_players[i].id==g_selected_player) draw_text(dc,350,y+11,L"ACTIF",12); y+=52; }
        y+=12; draw_text(dc,24,y,L"CAMÉRA",18); y+=34;
        using M=ets2la_plugin::examiner_camera_bridge::camera_mode; auto current=ets2la_plugin::examiner_camera_bridge::get_camera_mode();
        const M modes[]={M::cabin,M::rear,M::free,M::overhead};
        for(M m:modes){ RECT row{20,y,440,y+40}; draw_button(dc,row,mode_name(m),current==m); y+=47; }
        draw_text(dc,24,r.bottom-62,L"F8  •  ouvrir / fermer",14); draw_text(dc,24,r.bottom-38,L"Échap  •  fermer",14); draw_text(dc,24,r.bottom-16,L"1-4  •  changer de caméra",13);
        EndPaint(hwnd,&ps); return 0;
    }
    case WM_LBUTTONDOWN: {
        int y=GET_Y_LPARAM(lp); if(y>=105){ int player_count=static_cast<int>(g_players.size()); if(player_count>0 && y<105+player_count*52){ select_entry((y-105)/52); return 0; }
            int camera_start=105+std::max(player_count,1)*52+12+34; int index=(y-camera_start)/47;
            if(y>=camera_start && index>=0 && index<4){ using M=ets2la_plugin::examiner_camera_bridge::camera_mode; set_mode(static_cast<M>(index+1)); return 0; }
        } return 0;
    }
    case WM_KEYDOWN:
        if(wp==VK_ESCAPE){ toggle(); return 0; }
        if(wp>=VK_NUMPAD1 && wp<=VK_NUMPAD4){ using M=ets2la_plugin::examiner_camera_bridge::camera_mode; set_mode(static_cast<M>(wp-VK_NUMPAD1+1)); return 0; }
        break;
    case WM_DESTROY: g_overlay=nullptr; return 0;
    }
    return DefWindowProcW(hwnd,msg,wp,lp);
}
}

bool init(){
    if(g_overlay) return true; g_game=FindWindowW(nullptr,L"Euro Truck Simulator 2"); if(!g_game) return false;
    WNDCLASSW wc{}; wc.lpfnWndProc=wnd_proc; wc.hInstance=GetModuleHandleW(nullptr); wc.lpszClassName=kClassName; wc.hCursor=LoadCursor(nullptr,IDC_ARROW); RegisterClassW(&wc);
    g_overlay=CreateWindowExW(WS_EX_TOOLWINDOW|WS_EX_NOACTIVATE,kClassName,L"ETS2 Examiner",WS_POPUP,0,0,kWidth,kHeight,nullptr,nullptr,GetModuleHandleW(nullptr),nullptr);
    if(!g_overlay) return false; position_overlay(); ShowWindow(g_overlay,SW_HIDE); set_click_through(false); refresh_players(); return true;
}
void shutdown(){ ets2la_plugin::examiner_camera_bridge::clear_player_vehicle(); if(g_overlay){DestroyWindow(g_overlay);g_overlay=nullptr;} g_game=nullptr;g_visible=false;g_players.clear();g_selected_player=-1; }
void tick(){
    if(!g_overlay){init();return;} if(g_game&&!IsWindow(g_game)) g_game=FindWindowW(nullptr,L"Euro Truck Simulator 2");
    if(GetAsyncKeyState(VK_F8)&0x8000){ULONGLONG now=GetTickCount64();if(now-g_last_toggle>300){g_last_toggle=now;toggle();}}
    if(g_visible){refresh_players();position_overlay();ShowWindow(g_overlay,SW_SHOWNOACTIVATE);
        if(GetAsyncKeyState('1')&0x8000) set_mode(ets2la_plugin::examiner_camera_bridge::camera_mode::cabin);
        else if(GetAsyncKeyState('2')&0x8000) set_mode(ets2la_plugin::examiner_camera_bridge::camera_mode::rear);
        else if(GetAsyncKeyState('3')&0x8000) set_mode(ets2la_plugin::examiner_camera_bridge::camera_mode::free);
        else if(GetAsyncKeyState('4')&0x8000) set_mode(ets2la_plugin::examiner_camera_bridge::camera_mode::overhead);
    }
}
void toggle(){if(!g_overlay&&!init())return;g_visible=!g_visible;if(g_visible){refresh_players();position_overlay();set_click_through(false);ShowWindow(g_overlay,SW_SHOWNOACTIVATE);InvalidateRect(g_overlay,nullptr,FALSE);}else ShowWindow(g_overlay,SW_HIDE);}
bool is_visible(){return g_visible;} void select_player(int player_id){for(size_t i=0;i<g_players.size();++i)if(g_players[i].id==player_id){select_entry(static_cast<int>(i));return;}}
int selected(){return g_selected_player;}
}
#else
#include "examiner_gui.hpp"
namespace examiner_gui{bool init(){return false;}void shutdown(){}void tick(){}void toggle(){}bool is_visible(){return false;}void select_player(int){}int selected(){return -1;}}
#endif
