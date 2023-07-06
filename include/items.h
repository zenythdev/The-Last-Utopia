#include "raylib.h"

typedef struct {
    int id;
    int count;
    int max_count;
    char* name;
    char* description;
} Item;

static Item item_air = {-1, 0, 0, "", "" }; // Creator: Zenyth
static Item item_log = { 0, 1, 30, "Log", "Don't be alarmed, it's all bark and no bite." }; // Creator: Zenyth
static Item item_rock = { 1, 1, 30, "Rock", "" }; // Creator: Zenyth
static Item item_bluesteel_sabre = { 35, 1, 1, "Bluesteel Sabre", "" }; // Creator: Zenyth
static Item item_malice = { 200, 1, 1, "Malice", "" }; // Creator: Zenyth

Item* AddItem(Item* inventory, int item_count, Item item);
Item* RemoveItem(Item* inventory, int item_count, Item item);
Item* DeleteItem(Item* inventory, int item);