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
#include <atomic>
#include <thread>
#include <mutex>

namespace examiner_gui { namespace {
constexpr wchar_t kClassName[] = L"ETS2ExaminerOverlay";
constexpr int kWidth = 460, kHeight = 620;
constexpr UINT WM_EXAMINER_REFRESH = WM_APP + 10;
constexpr UINT WM_EXAMINER_TOGGLE = WM_APP + 11;
constexpr int kHotkeyId = 0x4554;

std::atomic<HWND> g_overlay{nullptr};
std::atomic<HWND> g_game{nullptr};
std::atomic<DWORD> g_gui_thread_id{0};
std::atomic<bool> g_visible{false};
std::atomic<bool> g_stop{false};
std::thread g_gui_thread;
std::mutex g_players_mutex;
int g_selected_player = -1;
ULONGLONG g_last_toggle = 0;

struct PlayerEntry { int id; std::uintptr_t vehicle; std::wstring name; };
std::vector<PlayerEntry> g_players;

void position_overlay() {
    HWND overlay = g_overlay.load();
    HWND game = g_game.load();
    if (!overlay || !game || !IsWindow(overlay) || !IsWindow(game)) return;
    RECT r{};
    if (!GetClientRect(game, &r)) return;
    POINT p{r.left, r.top};
    ClientToScreen(game, &p);
    SetWindowPos(overlay, HWND_TOPMOST, p.x, p.y,
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
    std::lock_guard<std::mutex> lock(g_players_mutex);
    g_players.swap(fresh);
}

void select_entry(int index) {
    std::uintptr_t vehicle = 0;
    int id = -1;
    {
        std::lock_guard<std::mutex> lock(g_players_mutex);
        if (index < 0 || index >= (int)g_players.size()) return;
        id = g_players[index].id;
        vehicle = g_players[index].vehicle;
    }
    g_selected_player = id;
    ets2la_plugin::examiner_camera_bridge::set_player_vehicle(vehicle);
    HWND overlay = g_overlay.load();
    if (overlay) InvalidateRect(overlay, nullptr, FALSE);
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

void toggle_on_gui_thread() {
    HWND overlay = g_overlay.load();
    if (!overlay) return;
    bool next = !g_visible.load();
    g_visible.store(next);
    if (next) {
        HWND foreground = GetForegroundWindow();
        if (foreground && foreground != overlay) g_game.store(foreground);
        refresh_players();
        position_overlay();
        ShowWindow(overlay, SW_SHOWNOACTIVATE);
        InvalidateRect(overlay, nullptr, FALSE);
    } else {
        ShowWindow(overlay, SW_HIDE);
    }
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
        {
            std::lock_guard<std::mutex> lock(g_players_mutex);
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
        int index = -1;
        {
            std::lock_guard<std::mutex> lock(g_players_mutex);
            int pc = (int)g_players.size();
            if (y >= 105 && pc > 0 && y < 105 + pc * 52)
                index = (y - 105) / 52;
        }
        if (index >= 0) select_entry(index);
        return 0;
    }
    case WM_KEYDOWN:
        if (wp == VK_ESCAPE) {
            toggle_on_gui_thread();
            return 0;
        }
        break;
    case WM_EXAMINER_TOGGLE:
        toggle_on_gui_thread();
        return 0;
    case WM_EXAMINER_REFRESH:
        if (g_visible.load()) {
            position_overlay();
            InvalidateRect(hwnd, nullptr, FALSE);
        }
        return 0;
    case WM_DESTROY:
        g_overlay.store(nullptr);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

void gui_thread_main() {
    g_gui_thread_id.store(GetCurrentThreadId());
    HINSTANCE instance = GetModuleHandleW(nullptr);
    WNDCLASSW wc{};
    wc.lpfnWndProc = wnd_proc;
    wc.hInstance = instance;
    wc.lpszClassName = kClassName;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClassW(&wc);

    HWND overlay = CreateWindowExW(
        WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE,
        kClassName, L"ETS2 Examiner", WS_POPUP,
        40, 40, kWidth, kHeight, nullptr, nullptr, instance, nullptr);
    if (!overlay) {
        g_gui_thread_id.store(0);
        return;
    }
    g_overlay.store(overlay);

    // The GUI thread owns the Win32 message loop and the F8 hotkey.
    // This avoids depending on the SCS telemetry callback for Windows input.
    RegisterHotKey(nullptr, kHotkeyId, 0, VK_F8);

    MSG msg{};
    while (!g_stop.load()) {
        BOOL result = GetMessageW(&msg, nullptr, 0, 0);
        if (result <= 0) break;
        if (msg.message == WM_HOTKEY && msg.wParam == kHotkeyId) {
            toggle_on_gui_thread();
            continue;
        }
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    UnregisterHotKey(nullptr, kHotkeyId);
    if (g_overlay.load()) DestroyWindow(g_overlay.load());
    g_overlay.store(nullptr);
    g_gui_thread_id.store(0);
}
}

bool init() {
    if (g_gui_thread.joinable()) return g_overlay.load() != nullptr;
    g_stop.store(false);
    g_visible.store(false);
    g_gui_thread = std::thread(gui_thread_main);
    for (int i = 0; i < 100 && !g_overlay.load() && !g_stop.load(); ++i)
        Sleep(10);
    return g_overlay.load() != nullptr;
}

void shutdown() {
    g_stop.store(true);
    DWORD tid = g_gui_thread_id.load();
    if (tid) PostThreadMessageW(tid, WM_QUIT, 0, 0);
    if (g_gui_thread.joinable()) g_gui_thread.join();
    ets2la_plugin::examiner_camera_bridge::clear_player_vehicle();
    g_game.store(nullptr);
    g_visible.store(false);
    std::lock_guard<std::mutex> lock(g_players_mutex);
    g_players.clear();
    g_selected_player = -1;
}

void tick() {
    HWND overlay = g_overlay.load();
    if (!overlay) return;
    if (g_visible.load()) {
        // Telemetry thread updates player data; Win32 painting/input stays on GUI thread.
        refresh_players();
        DWORD tid = g_gui_thread_id.load();
        if (tid) PostThreadMessageW(tid, WM_EXAMINER_REFRESH, 0, 0);
    }
}

void toggle() {
    HWND overlay = g_overlay.load();
    if (!overlay) return;
    DWORD tid = g_gui_thread_id.load();
    if (tid == GetCurrentThreadId()) {
        toggle_on_gui_thread();
    } else if (tid) {
        PostThreadMessageW(tid, WM_EXAMINER_TOGGLE, 0, 0);
    }
}

bool is_visible() { return g_visible.load(); }
void select_player(int id) {
    int index = -1;
    {
        std::lock_guard<std::mutex> lock(g_players_mutex);
        for (size_t i = 0; i < g_players.size(); ++i)
            if (g_players[i].id == id) { index = (int)i; break; }
    }
    if (index >= 0) select_entry(index);
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
