#include "MCLink.h"

// resources begin

namespace Sounds {
#define DECL_SOUND(name, variant) Sound name(L#name, variant)
	DECL_SOUND(air, 0);

	DECL_SOUND(stone, 4);
	DECL_SOUND(grass, 4);

	DECL_SOUND(challenge_complete, 1);
	DECL_SOUND(explode, 4);
#undef DECL_SOUND
}

namespace Items {
#define DECL_ITEM(name) Item name(L#name)
	DECL_ITEM(air);

	DECL_ITEM(coal);
	DECL_ITEM(copper_ingot);
	DECL_ITEM(iron_ingot);
	DECL_ITEM(gold_ingot);
	DECL_ITEM(lapis_lazuli);
	DECL_ITEM(redstone);
	DECL_ITEM(diamond);
	DECL_ITEM(emerald);

	DECL_ITEM(oak_sapling);
	DECL_ITEM(birch_sapling);
	DECL_ITEM(spruce_sapling);
	DECL_ITEM(jungle_sapling);
	DECL_ITEM(acacia_sapling);
	DECL_ITEM(dark_oak_sapling);

	DECL_ITEM(dandelion);
	DECL_ITEM(poppy);
	DECL_ITEM(blue_orchid);
	DECL_ITEM(allium);
	DECL_ITEM(azure_bluet);
	DECL_ITEM(red_tulip);
	DECL_ITEM(orange_tulip);
	DECL_ITEM(white_tulip);
	DECL_ITEM(pink_tulip);
	DECL_ITEM(oxeye_daisy);
	DECL_ITEM(cornflower);
	DECL_ITEM(lily_of_the_valley);
#undef DECL_ITEM
}

namespace Blocks {
#define DECL_BLOCK(name, drop, sound) Block name(L#name, &Items::drop, &Sounds::sound)
#define DECL_BLOCK_ITEM(name, sound) Block name(L#name, &Items::name, &Sounds::sound)
	DECL_BLOCK_ITEM(air, air);

	DECL_BLOCK(coal_ore, coal, stone);
	DECL_BLOCK(copper_ore, copper_ingot, stone);
	DECL_BLOCK(iron_ore, iron_ingot, stone);
	DECL_BLOCK(gold_ore, gold_ingot, stone);
	DECL_BLOCK(lapis_ore, lapis_lazuli, stone);
	DECL_BLOCK(redstone_ore, redstone, stone);
	DECL_BLOCK(diamond_ore, diamond, stone);
	DECL_BLOCK(emerald_ore, emerald, stone);

	DECL_BLOCK(deepslate_coal_ore, coal, stone);
	DECL_BLOCK(deepslate_copper_ore, copper_ingot, stone);
	DECL_BLOCK(deepslate_iron_ore, iron_ingot, stone);
	DECL_BLOCK(deepslate_gold_ore, gold_ingot, stone);
	DECL_BLOCK(deepslate_lapis_ore, lapis_lazuli, stone);
	DECL_BLOCK(deepslate_redstone_ore, redstone, stone);
	DECL_BLOCK(deepslate_diamond_ore, diamond, stone);
	DECL_BLOCK(deepslate_emerald_ore, emerald, stone);

	DECL_BLOCK_ITEM(oak_sapling, grass);
	DECL_BLOCK_ITEM(birch_sapling, grass);
	DECL_BLOCK_ITEM(spruce_sapling, grass);
	DECL_BLOCK_ITEM(jungle_sapling, grass);
	DECL_BLOCK_ITEM(acacia_sapling, grass);
	DECL_BLOCK_ITEM(dark_oak_sapling, grass);

	DECL_BLOCK_ITEM(dandelion, grass);
	DECL_BLOCK_ITEM(poppy, grass);
	DECL_BLOCK_ITEM(blue_orchid, grass);
	DECL_BLOCK_ITEM(allium, grass);
	DECL_BLOCK_ITEM(azure_bluet, grass);
	DECL_BLOCK_ITEM(red_tulip, grass);
	DECL_BLOCK_ITEM(orange_tulip, grass);
	DECL_BLOCK_ITEM(white_tulip, grass);
	DECL_BLOCK_ITEM(pink_tulip, grass);
	DECL_BLOCK_ITEM(oxeye_daisy, grass);
	DECL_BLOCK_ITEM(cornflower, grass);
	DECL_BLOCK_ITEM(lily_of_the_valley, grass);
#undef DECL_BLOCK_ITEM
#undef DECL_BLOCK
}


namespace Themes {
	using namespace Blocks;
	Theme cave(L"cave", L"stone", {{
		{&air, 35}, 
		{&coal_ore, 14}, {&copper_ore, 8},
		{&iron_ore, 10}, {&gold_ore, 4}, 
		{&lapis_ore, 3}, {&redstone_ore, 5},
		{&diamond_ore, 2}, {&emerald_ore, 1}
		}});
	Theme deep_cave(L"deep_cave", L"deepslate", {{
		{&air, 35}, 
		{&deepslate_coal_ore, 14}, {&deepslate_copper_ore, 8},
		{&deepslate_iron_ore, 10}, {&deepslate_gold_ore, 4}, 
		{&deepslate_lapis_ore, 3}, {&deepslate_redstone_ore, 5},
		{&deepslate_diamond_ore, 2}, {&deepslate_emerald_ore, 1}
		}});
	Theme saplings(L"saplings", L"grass_block", {{
		{&air, 8}, 
		{&oak_sapling, 1}, {&birch_sapling, 1},
		{&spruce_sapling, 1}, {&jungle_sapling, 1},
		{&acacia_sapling, 1}, {&dark_oak_sapling, 1}
		}});
	Theme flowers(L"flowers", L"grass_block", {{
		{&air, 14}, 
		{&dandelion, 1}, {&poppy, 1}, {&blue_orchid, 1}, {&allium, 1}, {&azure_bluet, 1},
		{&red_tulip, 1}, {&orange_tulip, 1}, {&white_tulip, 1}, {&pink_tulip, 1},
		{&oxeye_daisy, 1}, {&cornflower, 1},{&lily_of_the_valley, 1},
		}});
}

void loadResources() {
	for (Item* item : Item::items) {
		if (item->id) {
			loadScaledAsset(&item->img, item->name);
		}
	}
	for (Block* block : Block::blocks) {
		if (block->id) {
			loadScaledAsset(&block->img, block->name);
		}
	}
	for (Theme* theme : Theme::themes) {
		loadScaledAsset(&theme->img, theme->texture_name);
	}
	loadSharedAssets();
}

// resources end

// initialization begin

namespace Env::Menu {
	ButtonManager bm;
	const wchar_t info[] = 
		LR"(MCLink by Yaossg
Developed in C++17 with EasyX
Resources from Minecraft)";
}

namespace Env::Score {
	ButtonManager bm;
	ScoreList scorelist;
}

void init() {
	initgraph(0, 0); // to initialize easyx environment
	loadResources();
	closegraph();
	{
		using Env::Menu::bm;
		bm.add(Button(110, 4.5 * CELL, L"新游戏", game, true));
		bm.add(Button(110, 6.5 * CELL, L"排行榜", showScoreList, true));
		bm.add(Button(110, 8.5 * CELL, L"关于", showInfo, false));
		bm.add(Button(110, 10.5 * CELL, L"退出", tryExit, false));
	}
	{
		using Env::Score::bm;
		bm.add(Button(110, 10.5 * CELL, L"返回", [] {}, true));
	}
}

// initialization end

void newLevel(Theme* theme) {
	Env::theme = theme;
	using namespace Env;
	sum += score;
	score = 0;
	timeout = false;
	renewzone();
	for (int i = 0; i < mheight; ++i) {
		for (int j = 0; j < mwidth; ++j) {
			setblock(j, i, theme->blocks.pick());
		}
	}
	for (auto&& [block, points] : assemble()) {
		if (points.size() & 1) {
			POINT point = points[dist_t {0, points.size() - 1}(rng)];
			setblock(point.x, point.y, &Blocks::air);
		}
	}
	collected.clear();
	animator.reset();
	Search::route.clear();
	newInterface(WWIDTH * CELL, WHEIGHT * CELL);
	animator.add(renderTimeline, INT_MAX);
}

/******************************************
 * Search algorithm of MCLink by Yaossg.  *
 * Copyright © Yaossg All rights reserved.*
 ******************************************/
Route search(POINT first, POINT second) {
	using namespace Env::Search;
	renewsearchzone();
	Route route;
	// step 1: migrate
	for (int i = 0; i < mheight; ++i) {
		for (int j = 0; j < mwidth; ++j) {
			searchzone[i][j] = gamezone[i][j]->id ? SearchElement::BLOCK : SearchElement::AIR;
		}
	}
	set(first, SearchElement::TARGET);
	set(second, SearchElement::TARGET);
	// step 2: first point
	for (POINT direction : directions) {
		POINT point = first;
		bool running = true;
		while (running) {
			point += direction;
			if (!insearchzone(point)) running = false;
			else switch (get(point)) {
			case SearchElement::TARGET:
				return { first, second };
			case SearchElement::AIR:
				set(point, SearchElement::PATH);
				break;
			case SearchElement::BLOCK:
				running = false;
				break;
			default:
				throw 0; // impossible to reach here
			}
		}
	}
	// step 3: second point
	for (POINT direction : directions) {
		POINT point = second;
		bool running = true;
		while (running) {
			point += direction;
			if (!insearchzone(point)) running = false;
			else switch (get(point)) {
			case SearchElement::AIR:
				set(point, SearchElement::PATH);
				break;
			case SearchElement::BLOCK:
				running = false;
				break;
			case SearchElement::PATH:
				return { first, point, second };
			default:
				throw 0; // impossible to reach here
			}
		}
	}
	int minx = min(first.x, second.x);
	int maxx = max(first.x, second.x);
	int miny = min(first.y, second.y);
	int maxy = max(first.y, second.y);
	int avex = (minx + maxx) / 2;
	int avey = (miny + maxy) / 2;
	// step 4: row bridge
	if (minx != maxx)
	for (int r = 1; r <= 2 * mheight + 1; ++r) {
		int bridgey = avey + (r >> 1) * (r & 1 ? -1 : 1);
		if (bridgey >= -1 && bridgey <= mheight && bridgey != first.y && bridgey != second.y &&
			get({minx, bridgey}) == SearchElement::PATH && get({maxx, bridgey}) == SearchElement::PATH) {
			bool blocked = false;
			for (int k = minx + 1; k < maxx; ++k) {
				if (get({k, bridgey}) == SearchElement::BLOCK) {
					blocked = true;
					break;
				}
			}
			if (!blocked) {
				return { first, {first.x, bridgey}, {second.x, bridgey}, second };
			}
		}
	}
	// step 5: col bridge
	if (miny != maxy)
	for (int r = 1; r <= 2 * mwidth + 1; ++r) {
		int bridgex = avex + (r >> 1) * (r & 1 ? -1 : 1);
		if (bridgex >= -1 && bridgex <= mwidth && bridgex != first.x && bridgex != second.x &&
			get({bridgex, miny}) == SearchElement::PATH && get({bridgex, maxy}) == SearchElement::PATH) {
			bool blocked = false;
			for (int k = miny + 1; k < maxy; ++k) {
				if (get({bridgex, k}) == SearchElement::BLOCK) {
					blocked = true;
					break;
				}
			}
			if (!blocked) {
				return { first, {bridgex, first.y}, {bridgex, second.y}, second };
			}
		}
	}
	// step 6: failure
	return {};
}

// link begin

bool link(int x, int y) {
	using namespace Env;
	if (selected.x < 0 && selected.y < 0) return false;
	if (getblock(x, y) != getblock(selected.x, selected.y)) return false;
	Route route = search({x, y}, selected);
	bool found = !route.empty();
	if (found) {
		Search::route = move(route);
		++collected[getblock(x, y)->drop];
		getblock(x, y)->sound->play();
		getblock(selected.x, selected.y)->sound->play();
		setblock(x, y, &Blocks::air);
		setblock(selected.x, selected.y, &Blocks::air);
		Env::score++;
		selected.x = -1;
		selected.y = -1;
		animator.add(renderRoute, Animator::TPS);
	}
	return found;
}

void select(int x, int y) {
	using namespace Env;
	if ((x < 0 || y < 0 || x >= mwidth || y >= mheight) 
		|| selected.x == x && selected.y == y 
		|| !getblock(x, y)->id) {
		selected.x = -1;
		selected.y = -1;
	} else if (!link(x, y)) {
		selected.x = x;
		selected.y = y;
	}
}

Route solve() {
	using namespace Env;
	for (auto&& [block, points] : assemble()) {
		if (points.size() < 2) continue;
		for (int i = 0; i < points.size() - 1; ++i) {
			for (int j = i + 1; j < points.size(); ++j) {
				if (Route route = search(points[i], points[j]); !route.empty()) {
					return route;
				}
			}
		}
	}
	return {};
}

// link end

// render begin

void render() {
	using namespace Env;
	BatchDrawer drawer;
	translate(0, 0);
	for (int i = 0; i < WHEIGHT; ++i) {
		for (int j = 0; j < WWIDTH; ++j) {
			putimage(j * CELL, i * CELL, &theme->img);
		}
	}
	translate(CELL, CELL);
	for (int i = 0; i < mheight; ++i) {
		for (int j = 0; j < mwidth; ++j) {
			if (Block const* block = getblock(j, i); block->id) {
				renderImage(j * CELL, i * CELL, &block->img);
			}
		}
	}
	if (selected.x >= 0 && selected.y >= 0) {
		renderSelector(selected);
	}
	//translate(CELL, mheight * CELL);
	//renderImage(CELL * 3.5, CELL * 2, &SharedAssets::widget);
	translate(0, 0);
	renderImage(TIMELINE_X, 0, &SharedAssets::tnt);
	int r = 0;
	for (auto&& [item, count] : collected) {
		translate(CELL * mwidth + 2 * CELL, CELL + r * CELL);
		renderImage(0, 0, &item->img);
		int offset = CELL * 0.6;
		renderText(offset, offset, to_wstring(count).c_str());
		++r;
	}
	animator.animate();
}

void renderSelector(POINT point) {
	translate(CELL, CELL);
	renderImage(point.x * CELL - 2, point.y * CELL - 2, &SharedAssets::selector);
}

void renderRoute(Animation* animation) {
	using namespace Env::Search;
	if (!route.empty()) {
		translate(CELL, CELL);
		setlinecolor(HSLtoRGB(animation->timer * 3.6, 1, 0.7));
		setlinestyle(PS_SOLID, 7);
		POINT semicell = { CELL, CELL };
		semicell *= 0.5;
		for (size_t i = 0; i < route.size() - 1; ++i) {
			POINT first = route[i], second = route[i + 1];
			first *= CELL;
			second *= CELL;
			first += semicell;
			second += semicell;
			line(first.x, first.y, second.x, second.y);
		}
	}
}

void renderTimeline(Animation* animation) {
	translate(TIMELINE_X, 0);
	for (int i = 0; i < WHEIGHT; ++i) {
		int y = (WHEIGHT + i) * CELL - animation->timer + Env::score * 100;
		if (y > getheight()) break;
		if (y < CELL) Env::timeout = true;
		renderImage(0, y, &SharedAssets::sugarcane);
	}
}

// render end

// game begin

int id = 0;
bool skip = false;
bool update() {
	Route solution = solve();
	if (skip || solution.empty()) {
		Sounds::challenge_complete.play();
		villagerStore();
		newLevel(Theme::themes[id = (id + 1) % Theme::themes.size()]);
		skip = false;
	}
	if (Env::timeout) {
		Sounds::explode.play();
		MessageBox(GetHWnd(), L"你失败了", L"游戏结束", MB_OK | MB_ICONINFORMATION);
		return false;
	}
	if (_kbhit()) {
		switch(_getch()) {
		case VK_ESCAPE:
			if (MessageBox(GetHWnd(), L"您确定要退出吗?", L"退出", MB_YESNO | MB_ICONWARNING) == IDYES) 
				return false;
			Env::animator.resume();
			break;
		case VK_SPACE:
			if (!solution.empty()) {
				Env::animator.add([
					first = *solution.begin(), 
					second = *solution.rbegin()
				] (Animation* animation) {
					if (animation->timer % 40 < 20) {
						renderSelector(first);
						renderSelector(second);
					}
				}, Animator::TPS);
			}break;
		case VK_TAB:
			skip = true;
			break;
		}
	}
	if (MouseHit()) {
		MOUSEMSG msg = GetMouseMsg();
		switch(msg.uMsg) {
		case WM_LBUTTONDOWN:
			select(msg.x / CELL - 1, msg.y / CELL - 1);
			break;
		case WM_RBUTTONDOWN:
			using Env::selected;
			selected.x = selected.y = -1;
			break;
		}
	}
	render();
	return true;
}

void game() {
	using namespace Env;
	sum = score = 0;
	newLevel(&Themes::cave);
	while (update());
	MessageBox(GetHWnd(), (L"您的得分：" + to_wstring(sum += score)).c_str(), L"游戏结束", MB_OK | MB_ICONINFORMATION);
	Env::name = askName(L"计入高分榜", L"\n若昵称为空则放弃本次成绩");
	if (!Env::name.empty())
		putScoreList();
}

// game end

void villagerStore() {
	newInterface(819, 600);
	renderImage(0, 0, &SharedAssets::background);
	translate(CELL, CELL);
	//renderText(0, 0, L"村民商店");
	//renderText(0, CELL, L"未完成");
	renderText(0, 0, L"恭喜你完成本关!");
	renderText(0, CELL, L"当前分数:" + to_wstring(Env::sum + Env::score));
	renderText(0, 2 * CELL, L"按任意键继续...");
	_getch();
}

// menu begin

void menu() {
	using Env::Menu::bm;
	while (true) {
		newInterface(MENU_WIDTH, MENU_HEIGHT);
		renderImage(0, 0, &SharedAssets::background);
		renderImage(0, 0, &SharedAssets::logo);
		titleZH([] { renderTextCentered(0, 140, MENU_WIDTH + 1, 160, L"连连看"); });
		bm.draw();
		do
			while( !bm.exec());
		while (!bm.repaint);
	}
}

void showInfo() {
	MessageBox(GetHWnd(), Env::Menu::info, L"关于", MB_OK | MB_ICONINFORMATION);
}

void tryExit(){
	if (MessageBox(GetHWnd(), L"您确定要退出吗?", L"退出游戏", MB_YESNO | MB_ICONWARNING) == IDYES) {
		closegraph();
		exit(EXIT_SUCCESS);
	}
}

// menu end

// scorelist begin

void initScoreList() {
	wofstream file("ScoreList.txt");
	for (int i = 0; i < SCORELISTSIZE; i++) {
		file << L"- 0\n";
	}
}

void putScoreList() {
	using Env::Score::scorelist;
	getScoreList();
	wofstream file("ScoreList.txt");
	scorelist.insert({ Env::sum, Env::name });
	scorelist.erase(--scorelist.end());
	for (ScoreListElement const& e : scorelist) {
		file << e.second << L' ' << e.first << L'\n';
	}
}

void getScoreList() {
	using Env::Score::scorelist;
	wifstream file("ScoreList.txt");
	if (file.fail()) {
		initScoreList();
		file.open("ScoreList.txt");
	}
	scorelist.clear();
	for (int i = 0; i < SCORELISTSIZE; i++)  {
		wstring name; int score; file >> name >> score;
		scorelist.insert({ score, name });
	}
}

void showScoreList() {
	using namespace Env::Score;
	newInterface(MENU_WIDTH, MENU_HEIGHT);
	getScoreList();
	translate(0, 0);
	renderImage(0, 0, &SharedAssets::background);
	titleZH([] { renderTextCentered(0, 0, MENU_WIDTH + 1, 40, L"高分榜"); });
	translate(CELL * 5, CELL * 3);
	int i = 1;
	for (ScoreListElement const& e : scorelist)  {
		renderText(0, FONT_SIZE * i, (
			wstringstream() << left
			<< setw(4) << i++
			<< setw(12) << e.second
			<< setw(8) << e.first
			).str().c_str());
	}
	translate(0, 0);
	bm.draw();
	while (!bm.exec());
}

// scorelist end

void MCLink() {
	thread music([] {
		while(true)
			mci(L"play assets/sound/cat.wav");
	});
	init();
	menu();
}