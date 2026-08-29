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

namespace examiner_gui { namespace {
constexpr wchar_t kClassName[] = L"ETS2ExaminerOverlay";
constexpr int kWidth = 460, kHeight = 620;
HWND g_overlay = nullptr, g_game = nullptr;
bool g_visible = false;
int g_selected_player = -1;
ULONGLONG g_last_toggle = 0;

struct PlayerEntry { int id; std::uintptr_t vehicle; std::wstring name; };
std::vector<PlayerEntry> g_players;

void position_overlay() {
    if (!g_overlay || !g_game || !IsWindow(g_game)) return;
    RECT r{};
    if (!GetClientRect(g_game, &r)) return;
    POINT p{r.left, r.top};
    ClientToScreen(g_game, &p);
    SetWindowPos(g_overlay, HWND_TOPMOST, p.x, p.y,
                 r.right - r.left, r.bottom - r.top,
                 SWP_NOACTIVATE | SWP_SHOWWINDOW);
}

void draw_text(HDC dc, int x, int y, const std::wstring& text, int size = 18) {
    HFONT font = CreateFontW(size, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
        DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY, DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
    HFONT old = (HFONT)SelectObject(dc, font);
    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, RGB(235, 240, 245));
    TextOutW(dc, x, y, text.c_str(), (int)text.size());
    SelectObject(dc, old);
    DeleteObject(font);
}

void draw_button(HDC dc, RECT row, const std::wstring& text, bool active) {
    HBRUSH brush = CreateSolidBrush(active ? RGB(45, 80, 110) : RGB(30, 36, 44));
    FillRect(dc, &row, brush);
    DeleteObject(brush);
    draw_text(dc, row.left + 12, row.top + 9, text, 15);
}

void refresh_players() {
    std::vector<PlayerEntry> fresh;
    auto* traffic = ets2la_plugin::prism::game_traffic_u::get();
    if (!traffic) return;
    int id = 0;
    for (auto* p : traffic->traffic_player_vehicles_1) {
        if (!p || !p->traffic_player_object) continue;
        PlayerEntry e{};
        e.id = id++;
        e.vehicle = (std::uintptr_t)p;
        e.name = L"Joueur " + std::to_wstring(e.id + 1);
        fresh.push_back(std::move(e));
    }
    g_players.swap(fresh);
}

void select_entry(int index) {
    if (index < 0 || index >= (int)g_players.size()) return;
    g_selected_player = g_players[index].id;
    ets2la_plugin::examiner_camera_bridge::set_player_vehicle(g_players[index].vehicle);
    InvalidateRect(g_overlay, nullptr, FALSE);
}

void set_mode(ets2la_plugin::examiner_camera_bridge::camera_mode mode) {
    ets2la_plugin::examiner_camera_bridge::set_camera_mode(mode);
    InvalidateRect(g_overlay, nullptr, FALSE);
}

const wchar_t* mode_name(ets2la_plugin::examiner_camera_bridge::camera_mode mode) {
    using M = ets2la_plugin::examiner_camera_bridge::camera_mode;
    switch (mode) {
        case M::cabin: return L"1  Cabine";
        case M::rear: return L"2  Derrière";
        case M::free: return L"3  Extérieure";
        case M::overhead: return L"4  Dessus";
    }
    return L"";
}

LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps{};
        HDC dc = BeginPaint(hwnd, &ps);
        RECT r{};
        GetClientRect(hwnd, &r);
        HBRUSH bg = CreateSolidBrush(RGB(18, 22, 28));
        FillRect(dc, &r, bg);
        DeleteObject(bg);
        draw_text(dc, 24, 20, L"EXAMINATEUR", 26);
        draw_text(dc, 24, 58, L"Sélectionner un joueur Convoy", 17);
        int y = 105;
        if (g_players.empty()) {
            draw_text(dc, 24, y, L"Aucun joueur détecté", 17);
            y += 38;
        } else {
            for (size_t i = 0; i < g_players.size(); ++i) {
                RECT row{20, y, 440, y + 44};
                draw_button(dc, row, g_players[i].name, g_players[i].id == g_selected_player);
                y += 52;
            }
        }
        y += 12;
        draw_text(dc, 24, y, L"CAMÉRA", 18);
        y += 34;
        using M = ets2la_plugin::examiner_camera_bridge::camera_mode;
        M modes[] = {M::cabin, M::rear, M::free, M::overhead};
        auto current = ets2la_plugin::examiner_camera_bridge::get_camera_mode();
        for (M mode : modes) {
            RECT row{20, y, 440, y + 40};
            draw_button(dc, row, mode_name(mode), current == mode);
            y += 47;
        }
        draw_text(dc, 24, r.bottom - 62, L"F8  •  ouvrir / fermer", 14);
        draw_text(dc, 24, r.bottom - 38, L"Échap  •  fermer", 14);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_LBUTTONDOWN: {
        int y = (int)(short)HIWORD(lp);
        int pc = (int)g_players.size();
        if (y >= 105 && pc > 0 && y < 105 + pc * 52) {
            select_entry((y - 105) / 52);
            return 0;
        }
        return 0;
    }
    case WM_KEYDOWN:
        if (wp == VK_ESCAPE) { toggle(); return 0; }
        break;
    case WM_DESTROY:
        g_overlay = nullptr;
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

bool create_overlay() {
    if (g_overlay) return true;
    HINSTANCE instance = GetModuleHandleW(nullptr);
    WNDCLASSW wc{};
    wc.lpfnWndProc = wnd_proc;
    wc.hInstance = instance;
    wc.lpszClassName = kClassName;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassW(&wc);
    g_overlay = CreateWindowExW(
        WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE,
        kClassName, L"ETS2 Examiner", WS_POPUP,
        40, 40, kWidth, kHeight, nullptr, nullptr, instance, nullptr);
    return g_overlay != nullptr;
}
}

bool init() {
    if (g_overlay) return true;
    g_game = GetForegroundWindow();
    return create_overlay();
}

void shutdown() {
    ets2la_plugin::examiner_camera_bridge::clear_player_vehicle();
    if (g_overlay) DestroyWindow(g_overlay);
    g_overlay = nullptr;
    g_game = nullptr;
    g_visible = false;
    g_players.clear();
    g_selected_player = -1;
}

void tick() {
    if (!g_overlay) create_overlay();
    if (!g_game || !IsWindow(g_game)) g_game = GetForegroundWindow();
    if (GetAsyncKeyState(VK_F8) & 0x8000) {
        ULONGLONG now = GetTickCount64();
        if (now - g_last_toggle > 300) {
            g_last_toggle = now;
            toggle();
        }
    }
    if (g_visible) {
        refresh_players();
        position_overlay();
        ShowWindow(g_overlay, SW_SHOWNOACTIVATE);
    }
}

void toggle() {
    if (!g_overlay && !init()) return;
    g_visible = !g_visible;
    if (g_visible) {
        refresh_players();
        position_overlay();
        ShowWindow(g_overlay, SW_SHOWNOACTIVATE);
        InvalidateRect(g_overlay, nullptr, FALSE);
    } else {
        ShowWindow(g_overlay, SW_HIDE);
    }
}

bool is_visible() { return g_visible; }
void select_player(int id) {
    for (size_t i = 0; i < g_players.size(); ++i)
        if (g_players[i].id == id) { select_entry((int)i); return; }
}
int selected() { return g_selected_player; }

} // namespace examiner_gui
#else
#include "examiner_gui.hpp"
namespace examiner_gui {
bool init() { return false; }
void shutdown() {}
void tick() {}
void toggle() {}
bool is_visible() { return false; }
void select_player(int) {}
int selected() { return -1; }
}
#endif
