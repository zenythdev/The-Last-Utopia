#include "raylib.h"
#include "items.h"

Item* AddItem(Item* inventory, int item_count, Item item) {
    // Check if item already exists and add to item stack (if possible)
    bool has_item = false;
    for (int i = 0; i < 16; i++) {
        if (inventory[i].id == item.id && inventory[i].count < inventory[i].max_count) {
            inventory[i].count += item_count;
            has_item = true;
            break;
        }
    }

    // Check for empty slot in inventory and add to a separate stack otherwise
    if (has_item == false) {
        for (int i = 0; i < 16; i++) {
            if (inventory[i].id == -1) {
                inventory[i] = item;
                inventory[i].count = item_count;
                break;
            }
        }
    }

    return inventory;
}