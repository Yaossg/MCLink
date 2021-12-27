#pragma once
// C++ standard library
#include <chrono>
#include <filesystem>
#include <memory>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <queue>
#include <map>
#include <set>
#include <random>
#include <functional>
#include <thread>

// easyx & windows library
#define NOMINMAX
#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "WINMM.LIB")

using namespace std::chrono;
using namespace std::literals;

using std::wstring;
using std::vector;

using std::greater;
using std::pair;
using std::map;
using std::multiset;

using std::min;
using std::max;

using std::to_wstring;
using std::wifstream;
using std::wofstream;
using std::wstringstream;
using std::setw;
using std::left;

using std::random_device;
using std::uniform_int_distribution;
using std::default_random_engine;

using std::function;
using std::thread;

// remove easyx colors except BLACK & WHITE

#undef	BLUE
#undef	GREEN
#undef	CYAN
#undef	RED
#undef	MAGENTA
#undef	BROWN
#undef	LIGHTGRAY
#undef	DARKGRAY
#undef	LIGHTBLUE
#undef	LIGHTGREEN
#undef	LIGHTCYAN
#undef	LIGHTRED
#undef	LIGHTMAGENTA
#undef	YELLOW

// extend easyx begin

struct BatchDrawer {
	BatchDrawer() { BeginBatchDraw(); }
	~BatchDrawer() { EndBatchDraw(); }
};

inline POINT origin;
inline void translate(int x, int y) {
	setorigin(origin.x = x, origin.y = y);
}

void renderImage(int x, int y, IMAGE const* img);
void renderText(int x, int y, wstring text);
void renderTextCentered(int left, int top, int right, int bottom, wstring text);

// extend easyx end

// animation begin

struct Animation {
	function<void(Animation*)> renderer;
	int duration;
	int timer;

	Animation(function<void(Animation*)> renderer, int duration): 
		renderer(renderer), duration(duration), timer(0) {}
};

struct Animator {
	using clock = system_clock;
	static constexpr int TPS = 100;
	static constexpr milliseconds TAU = 10ms;

	Animator() { reset(); }

	void add(function<void(Animation*)> renderer, int duration) {
		animations.push_back({renderer, duration});
	}

	void resume() {
		now = clock::now();
	}

	void reset() {
		resume();
		lag = milliseconds::zero();
		animations.clear();
	}

	void animate() {
		milliseconds dt = duration_cast<milliseconds>(clock::now() - now);
		resume();
		lag += dt;
		bool tick = false;
		if (lag > TAU) {
			tick = true;
			lag -= TAU;
		}
		for (auto it = animations.begin(); it != animations.end(); ) {
			it->renderer(&*it);
			if (tick && ++it->timer >= it->duration)   {
				it = animations.erase(it);
			} else ++it;
		}
	}
private:
	vector<Animation> animations;
	clock::time_point now;
	milliseconds lag;
};

// animation end

// sound begin

inline void playsound(wstring name) {
	PlaySound((L"assets/sound/" + name + L".wav").c_str(), 0, SND_FILENAME | SND_ASYNC);
}

inline void mci(wstring command) {
	mciSendString(command.c_str(), 0, 0, 0);
}

// sound end

// assets begin

void loadScaledAsset(IMAGE* dst, wstring name);

namespace SharedAssets {
	inline IMAGE font;
	inline IMAGE button;
	inline IMAGE selector;
	inline IMAGE widget;
	inline IMAGE logo;
	inline IMAGE background;
	inline IMAGE tnt;
	inline IMAGE sugarcane;
}

constexpr int FONT_SIZE = 24;

inline void loadSharedAssets() {
	loadScaledAsset(&SharedAssets::font, L"ascii");
	loadScaledAsset(&SharedAssets::button, L"button");
	loadScaledAsset(&SharedAssets::logo, L"minecraft");
	loadScaledAsset(&SharedAssets::widget, L"widget");
	loadScaledAsset(&SharedAssets::background, L"background");
	loadScaledAsset(&SharedAssets::tnt, L"tnt");
	loadScaledAsset(&SharedAssets::sugarcane, L"sugarcane");
	loadimage(&SharedAssets::selector, L"assets/selector.png");
}

inline void initFont() {
	settextstyle(FONT_SIZE, 0, L"ºÚÌå");
	settextcolor(WHITE);
	setbkmode(TRANSPARENT);
}

inline void titleZH(function<void()> action) {
	settextstyle(FONT_SIZE * 2.5, 0, L"ºÚÌå");
	settextcolor(BLACK);
	action();
	settextcolor(WHITE);
	settextstyle(FONT_SIZE, 0, L"ºÚÌå");
}

inline void newInterface(int width, int height) {
	initgraph(width, height);
	initFont();
	translate(0, 0);
}

// assets end

// button begin

struct Button {
	void draw() const {
		renderImage(left, top, &SharedAssets::button);
		renderTextCentered(left, top, right, bottom, text);
	}
	Button(int x, int y, wstring text, function<void()> func, bool repaint): 
		left(x), top(y), right(x + 200 * 3), bottom(y + 20 * 3), 
		text(text), func(func), repaint(repaint) {}
	int left, top, right, bottom;
	wstring text;
	function<void()> func;

	bool repaint;
};

struct ButtonManager {
	void add(Button const& button) {
		buttons.push_back(button);
	}
	void draw() const {
		BatchDrawer drawer;
		for (Button button : buttons)
			button.draw();
	}
	bool exec() const {
		if (MouseHit()) {
			MOUSEMSG msg = GetMouseMsg();
			switch (msg.uMsg) {
			case WM_LBUTTONUP: {
				for (Button button : buttons) {
					if (button.left <= msg.x - origin.x
						&& button.right >= msg.x - origin.x
						&& button.top <= msg.y - origin.y
						&& button.bottom >= msg.y - origin.y) {
						playsound(L"click");
						FlushMouseMsgBuffer();
						button.func();
						repaint = button.repaint;
						return true;
					}
				}break;
			}
			}
		}
		return false;
	}
	mutable bool repaint = true;
private:
	std::vector<Button> buttons;
};

// button end

// utilities begin

using dist_t = uniform_int_distribution<size_t>;

template<typename E>
struct WeightedList {
	WeightedList(vector<pair<E, size_t>> elements): 
		elements(elements), 
		sum(0), 
		RandomEngine(random_device{}()) 
	{
		if (elements.empty()) throw 0;
		for(auto&& [element, weight] : elements) {
			if (!weight) throw 0;
			items.push_back(element);
			stairs.push_back(sum += weight);
		}
		dist = {0, sum - 1};
	}

	E pick() const {
		size_t roll = dist(RandomEngine);
		for (size_t i = 0; i < stairs.size(); ++i) {
			if (roll < stairs[i]) {
				return items[i];
			}
		}
		throw 0; // impossible to reach here
	}
private:
	vector<pair<E, size_t>> elements;
	vector<E> items;
	vector<size_t> stairs;
	size_t sum;
	mutable default_random_engine RandomEngine;
	mutable dist_t dist;
};

template<typename E>
struct IndexedArray {
	IndexedArray(): min(0), max(0), size(0) {}
	IndexedArray(int min, int max, E element = E()): 
		min(min), 
		max(max), 
		size(max - min + 1), 
		elements(size, element) 
	{
		if (min > max) throw 0;
	}

	E& operator[](int index) {
		return elements[index - min];
	}
	E const& operator[](int index) const {
		return elements[index - min];
	}
private:
	int min, max, size;
	vector<E> elements;
};

using Route = vector<POINT>;

using ScoreListElement = pair<int, wstring>;
using ScoreList = multiset<ScoreListElement, greater<ScoreListElement>>;

inline POINT& operator+=(POINT& lhs, POINT rhs) {
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	return lhs;
}

template<typename Numeric>
inline POINT& operator*=(POINT& lhs, Numeric rhs) {
	lhs.x *= rhs;
	lhs.y *= rhs;
	return lhs;
}

wstring askName(wstring title, wstring fallback);

// utilities end