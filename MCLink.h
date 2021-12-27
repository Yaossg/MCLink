#pragma once

#include "MCLinkLib.h"

#define CELL 48

#define SCORELISTSIZE 10

namespace Env {
	inline int mwidth = 18;
	inline int mheight = 12;
}

#define WWIDTH (Env::mwidth + 6)
#define WHEIGHT (Env::mheight + 2)//4)

#define MENU_WIDTH 819
#define MENU_HEIGHT 600

#define TIMELINE_X (CELL * Env::mwidth + 4 * CELL)

struct Sound;
struct Item;
struct Block;
struct Theme;

struct Sound {
	wstring name;
	int id;
	size_t variant;
	mutable default_random_engine rng;

	inline static vector<Sound*> sounds;

	Sound(wstring name, size_t variant): name(name), 
		id(sounds.size()), variant(variant), rng(random_device{}())
	{ sounds.push_back(this); }

	void play() const {
		if (id) {
			if (variant < 2) {
				playsound(name);
			} else {
				playsound(name + to_wstring(dist_t{1, variant}(rng)));
			}
		}
	}
};
struct Item {
	wstring name;
	int id;
	IMAGE img;

	inline static vector<Item*> items;

	Item(wstring name): name(name), 
		id(items.size()), img(CELL, CELL) 
	{ items.push_back(this); }
};

struct Block {
	wstring name;
	int id;
	IMAGE img;
	Item* drop;
	Sound* sound;

	inline static vector<Block*> blocks;

	Block(wstring name, Item* drop, Sound* sound): 
		name(name), drop(drop), sound(sound), id(blocks.size())
	{ blocks.push_back(this); }
};

struct Theme {
	wstring theme_name;
	wstring texture_name;
	int id;
	IMAGE img;
	WeightedList<Block*> blocks;

	inline static vector<Theme*> themes;

	Theme(wstring theme_name, wstring texture_name, WeightedList<Block*> blocks):
		theme_name(theme_name), texture_name(texture_name), 
		blocks(blocks), id(themes.size())
	{ themes.push_back(this); }

};

namespace Env {
	inline Theme const* theme;

	inline auto comparator = [](Item const* lhs, Item const* rhs) { return lhs->name < rhs->name; };
	inline map<Item const*, int, decltype(comparator)> collected(comparator); 

	inline IndexedArray<IndexedArray<Block const*>> gamezone;

	inline void renewzone() {
		gamezone = {0, mheight - 1, {0, mwidth - 1}};
	}

	inline POINT selected = { -1, -1 };

	inline void setblock(int x, int y, Block const* block) {
		gamezone[y][x] = block;
	}
	inline Block const* getblock(int x, int y) {
		return gamezone[y][x];
	}

	inline default_random_engine rng(random_device{}());

	inline Animator animator;

	inline int score = 0;
	inline int sum = 0;
	inline bool timeout = false;
	inline wstring name;

	using Assemble = map<Block const*, Route/* points */>;

	inline Assemble assemble() {
		Assemble assembled;
		for (int i = 0; i < mheight; ++i) {
			for (int j = 0; j < mwidth; ++j) {
				if (Block const* block = getblock(j, i); block->id) {
					assembled[block].push_back({j, i});
				}
			}
		}
		return assembled;
	}

	namespace Search {
		// partially derived from ::Env
		using Env::mheight;
		using Env::mwidth;
		using Env::gamezone;

		enum struct SearchElement {
			AIR, BLOCK, TARGET, PATH
		};

		inline IndexedArray<IndexedArray<SearchElement>> searchzone;

		inline void renewsearchzone() {
			searchzone = {-1, mheight, {-1, mwidth}};
		}

		inline bool insearchzone(POINT point) {
			return point.x >= -1 && point.y >= -1 && point.x <= mwidth && point.y <= mheight;
		}

		inline SearchElement get(POINT point) {
			return searchzone[point.y][point.x];
		}
		inline void set(POINT point, SearchElement element) {
			searchzone[point.y][point.x] = element;
		}

		constexpr POINT right = {1, 0}, left = {-1, 0}, down = {0, 1}, up = {0, -1};
		constexpr POINT directions[] = {right, left, down, up};
		
		inline Route route;
	}
}

void init();

void newLevel(Theme* theme);

Route search(POINT first, POINT second);
bool link(int x, int y);
void select(int x, int y);
Route solve();

void render();
void renderSelector(POINT point);
void renderRoute(Animation* animation);
void renderTimeline(Animation* animation);

bool update();
void game();

void villagerStore();

void menu();
void showInfo();
void tryExit();

void initScoreList();
void putScoreList();
void getScoreList();
void showScoreList();

void MCLink();