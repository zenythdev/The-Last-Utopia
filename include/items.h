#include "raylib.h"

typedef struct {
    char* type;
    int damage_low;
    int damage_high;
    int crit_chance;
} WeaponStats;

typedef struct {
    int id;
    int count;
    int max_count;
    char* name;
    char* description;
    WeaponStats combat;
} Item;

static Item item_air = {-1, 0, 0, "", "" }; // Creator: Zenyth
static Item item_log = { 0, 1, 30, "Log", "Don't be alarmed, it's all bark and no bite." }; // Creator: Zenyth
static Item item_rock = { 1, 1, 30, "Rock", "A solid lump of random ores and minerals." }; // Creator: Zenyth
static Item item_woodsword = { 2, 1, 1, "Wood Sword", "A wooden broadsword.", (WeaponStats){ "sword", 3, 10, 7 } }; // Creator: Zenyth
static Item item_bluesteel_sabre = { 35, 1, 1, "Bluesteel Sabre", "Might also protect you a little.", (WeaponStats){ "sword_light", 33, 46, 10 } }; // Creator: Zenyth
static Item item_malice = { 200, 1, 1, "Malice", "" }; // Creator: Zenyth
static Item item_otis_smg = { 201, 1, 1, "Otis' SMG", "The heirloom of the Jenkins family; a rusty submachine gun. Handle it with care." }; // Creator: Zenyth

Item* AddItem(Item* inventory, int item_count, Item item);
bool RemoveItem(Item* inventory, int item_count, Item item);
Item* DeleteItem(Item* inventory, int slot);
void DrawItems(Item* inventory, Texture* items);
void CraftItem(Item* inventory, Item* materials, Item item);