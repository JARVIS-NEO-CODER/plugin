#pragma once

namespace examiner_gui {

// Starts/stops the in-game Win32 overlay used by the examiner.
bool init();
void shutdown();

// Must be called from the plugin tick. Handles the toggle hotkey and
// refreshes the overlay position to the ETS2 window.
void tick();

void toggle();
bool is_visible();
void select_player(int player_id);
int selected();

} // namespace examiner_gui
