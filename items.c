#include "raylib.h"
#include "items.h"

Item* AddItem(Item* inventory, int item_count, Item item) {
    for (int j = 0; j < item_count; j++) {
        // Check if item already exists and add to item stack (if possible)
        bool has_item = false;
        for (int i = 0; i < 16; i++) {
            if (inventory[i].id == item.id && inventory[i].count < inventory[i].max_count) {
                inventory[i].count += 1;
                has_item = true;
                break;
            }
        }

        // Check for empty slot in inventory and add to a separate stack otherwise
        if (has_item == false) {
            for (int i = 0; i < 16; i++) {
                if (inventory[i].id == -1) {
                    inventory[i] = item;
                    inventory[i].count = 1;
                    break;
                }
            }
        }
    }
    return inventory;
}

bool RemoveItem(Item* inventory, int item_count, Item item) {
    // Check for items
    int total_items = 0;
    bool success = false;

    for (int i = 0; i < 16; i++) {
        // Check for sufficient number of items that match ID
        if (inventory[i].id == item.id) {
            total_items += inventory[i].count;
        }
    }

    if (total_items >= item_count) {
        success = true;
        for (int i = 0; i < 16; i++) {
            // Check if both IDs are identical
            if (inventory[i].id == item.id) {
                for (int j = 0; j < item_count; j++) {
                    if (inventory[i].count > 0) {
                        inventory[i].count--;
                    }
                    if (inventory[i].count == 0) {
                        inventory[i] = item_air;
                    }
                }
                break;
            }
        }
    }
    
    return success;
}

Item* DeleteItem(Item* inventory, int slot) {
    // Obliterate item in specified slot
    inventory[slot] = item_air;

    return inventory;
}

void CraftItem(Item* inventory, Item* materials, Item item) {
    bool success = RemoveItem(inventory, 1, materials[0]);
    RemoveItem(inventory, materials[0].count, materials[0]);
    if (success == true) {
        AddItem(inventory, item.count, item_rock);
    }
}

void DrawItems(Item* inventory, Texture* items) {
    for (int i = 0; i < 16; i++) {
		if (inventory[i].id != -1) {
            DrawTexture(items[inventory[i].id], 1.0f + 31 * i, GetScreenHeight() - 31, RAYWHITE);
        }
		if (inventory[i].count > 1) {
			DrawText(TextFormat("%d", inventory[i].count), 1.0f + 31 * i + 19, GetScreenHeight() - 13, 10, BLACK);
		}
	}
}