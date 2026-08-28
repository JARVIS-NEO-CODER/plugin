#if defined(_WIN32)

#include "examiner_gui.hpp"

#include <Windows.h>
#include <string>
#include <vector>
#include <algorithm>

namespace examiner_gui {
namespace {

constexpr wchar_t kClassName[] = L"ETS2LAExaminerOverlay";
constexpr int kWidth = 420;
constexpr int kHeight = 520;
constexpr UINT kToggleTimer = 1;

HWND g_overlay = nullptr;
HWND g_game = nullptr;
bool g_visible = false;
int g_selected_player = -1;
ULONGLONG g_last_toggle = 0;

struct PlayerEntry {
    int id;
    std::wstring name;
};

// The rendering/input layer is deliberately independent from the camera bridge.
// A real Convoy player provider can feed this list later; we do not fabricate
// player names when the game does not expose them through the telemetry SDK.
std::vector<PlayerEntry> g_players;

void set_click_through(bool click_through) {
    if (!g_overlay) return;
    LONG_PTR ex = GetWindowLongPtrW(g_overlay, GWL_EXSTYLE);
    if (click_through)
        ex |= WS_EX_TRANSPARENT | WS_EX_NOACTIVATE;
    else
        ex &= ~WS_EX_TRANSPARENT;
    SetWindowLongPtrW(g_overlay, GWL_EXSTYLE, ex);
}

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
                             DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                             CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY,
                             DEFAULT_PITCH | FF_DONTCARE, L"Segoe UI");
    HFONT old = (HFONT)SelectObject(dc, font);
    SetBkMode(dc, TRANSPARENT);
    SetTextColor(dc, RGB(235, 240, 245));
    TextOutW(dc, x, y, text.c_str(), static_cast<int>(text.size()));
    SelectObject(dc, old);
    DeleteObject(font);
}

LRESULT CALLBACK wnd_proc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps{};
        HDC dc = BeginPaint(hwnd, &ps);
        RECT r{};
        GetClientRect(hwnd, &r);

        // Transparent-ish dark panel. The layered window is only used as an
        // overlay surface; no external process or ETS2LA UI is required.
        HBRUSH bg = CreateSolidBrush(RGB(18, 22, 28));
        FillRect(dc, &r, bg);
        DeleteObject(bg);

        draw_text(dc, 24, 20, L"EXAMINATEUR", 26);
        draw_text(dc, 24, 58, L"Sélectionner un joueur", 17);

        if (g_players.empty()) {
            draw_text(dc, 24, 108, L"Aucun joueur détecté", 17);
            draw_text(dc, 24, 140, L"La source Convoy n'est pas encore exposée", 14);
            draw_text(dc, 24, 164, L"par le SDK télémétrie de cette DLL.", 14);
        } else {
            int y = 105;
            for (const auto& player : g_players) {
                RECT row{20, y, 400, y + 48};
                HBRUSH row_brush = CreateSolidBrush(
                    player.id == g_selected_player ? RGB(45, 80, 110) : RGB(30, 36, 44));
                FillRect(dc, &row, row_brush);
                DeleteObject(row_brush);
                draw_text(dc, 34, y + 12, player.name, 16);
                y += 56;
            }
        }

        draw_text(dc, 24, r.bottom - 58, L"F8  •  ouvrir / fermer", 14);
        draw_text(dc, 24, r.bottom - 34, L"Échap  •  fermer", 14);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_LBUTTONDOWN: {
        int y = GET_Y_LPARAM(lp);
        if (!g_players.empty() && y >= 105) {
            int index = (y - 105) / 56;
            if (index >= 0 && index < static_cast<int>(g_players.size())) {
                select_player(g_players[index].id);
                InvalidateRect(hwnd, nullptr, FALSE);
            }
        }
        return 0;
    }
    case WM_KEYDOWN:
        if (wp == VK_ESCAPE) {
            toggle();
            return 0;
        }
        break;
    case WM_TIMER:
        if (wp == kToggleTimer) {
            if (GetAsyncKeyState(VK_F8) & 0x8000) {
                ULONGLONG now = GetTickCount64();
                if (now - g_last_toggle > 250) {
                    g_last_toggle = now;
                    toggle();
                }
            }
        }
        break;
    case WM_DESTROY:
        g_overlay = nullptr;
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

} // namespace

bool init() {
    if (g_overlay) return true;

    g_game = FindWindowW(nullptr, L"Euro Truck Simulator 2");
    if (!g_game) return false;

    WNDCLASSW wc{};
    wc.lpfnWndProc = wnd_proc;
    wc.hInstance = GetModuleHandleW(nullptr);
    wc.lpszClassName = kClassName;
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
    RegisterClassW(&wc);

    g_overlay = CreateWindowExW(
        WS_EX_TOOLWINDOW | WS_EX_NOACTIVATE,
        kClassName, L"ETS2 Examiner",
        WS_POPUP,
        0, 0, kWidth, kHeight,
        nullptr, nullptr, GetModuleHandleW(nullptr), nullptr);

    if (!g_overlay) return false;

    SetTimer(g_overlay, kToggleTimer, 50, nullptr);
    position_overlay();
    ShowWindow(g_overlay, SW_HIDE);
    set_click_through(false);
    return true;
}

void shutdown() {
    if (g_overlay) {
        KillTimer(g_overlay, kToggleTimer);
        DestroyWindow(g_overlay);
        g_overlay = nullptr;
    }
    g_game = nullptr;
    g_visible = false;
}

void tick() {
    if (!g_overlay) {
        init();
        return;
    }
    if (g_game && !IsWindow(g_game)) {
        g_game = FindWindowW(nullptr, L"Euro Truck Simulator 2");
    }
    if (g_visible) position_overlay();
}

void toggle() {
    if (!g_overlay && !init()) return;
    g_visible = !g_visible;
    if (g_visible) {
        position_overlay();
        set_click_through(false);
        ShowWindow(g_overlay, SW_SHOWNOACTIVATE);
        InvalidateRect(g_overlay, nullptr, FALSE);
    } else {
        ShowWindow(g_overlay, SW_HIDE);
    }
}

bool is_visible() { return g_visible; }

void select_player(int player_id) {
    g_selected_player = player_id;
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
