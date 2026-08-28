// Simple in-game examiner GUI.
// This file is intentionally self-contained; the actual player-selection backend
// can be connected to the examiner camera bridge when the multiplayer player list
// is available.
#include "examiner_gui.hpp"

#include <string>
#include <vector>

namespace examiner_gui {

static bool visible = false;
static int selected_player = -1;

void toggle() {
    visible = !visible;
}

bool is_visible() {
    return visible;
}

void select_player(int player_id) {
    selected_player = player_id;
}

int selected() {
    return selected_player;
}

} // namespace examiner_gui
