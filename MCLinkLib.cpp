#include "MCLinkLib.h"

constexpr int ASSET_SCALE = 3;

void loadScaledAsset(IMAGE* dst, wstring name) {
	IMAGE src;
	loadimage(&src, (L"assets/" + name + L".png").c_str());
	int w = src.getwidth(), h = src.getheight();
	dst->Resize(w * ASSET_SCALE, h * ASSET_SCALE);
	DWORD *srcbuf = GetImageBuffer(&src), *dstbuf = GetImageBuffer(dst);
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			DWORD pixel = srcbuf[i * w + j];
			for (int u = 0; u < ASSET_SCALE; ++u) {
				for (int v = 0; v < ASSET_SCALE; ++v) {
					dstbuf[(i * ASSET_SCALE + v) * ASSET_SCALE * w + (j * ASSET_SCALE + u)] = pixel;
				}
			}
		}
	}
}

void renderImage(int x, int y, IMAGE const* img) {

	struct ARGB {
		BYTE b, g, r, a;
	};

	x += origin.x; y += origin.y;
	DWORD const* src = GetImageBuffer((IMAGE*) img);
	DWORD* dst = GetImageBuffer();
	int ww = getwidth(), wh = getheight();
	int w = img->getwidth(), h = img->getheight();
	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			int u = x + j, v = y + i;
			if (u >= 0 && v >= 0 && u < ww && v < wh) { 
				int dstIndex = u + v * ww;
				ARGB* dstColor = (ARGB*)&dst[dstIndex];
				int srcIndex = i * w + j;
				ARGB* srcColor = (ARGB*)&src[srcIndex];
				if (BYTE a = srcColor->a) {
					if (a == 255) *dstColor = *srcColor;
					else {
						dstColor->r = srcColor->r * a / 255 + dstColor->r * (255 - a) / 255;
						dstColor->g = srcColor->g * a / 255 + dstColor->g * (255 - a) / 255;
						dstColor->b = srcColor->b * a / 255 + dstColor->b * (255 - a) / 255;
					}
				}
			}
		}
	}
}

constexpr int asciiwidth = FONT_SIZE * 6 / 8;

int renderTextWidth(wstring text) {
	int r = 0;
	for (wchar_t ch : text) {
		r += ch < 256 ? asciiwidth : textwidth(ch);
	}
	return r;
}

void renderText(int x, int y, wstring text) {
	int r = 0;
	for (wchar_t ch : text) {
		if (ch < 256) {
			IMAGE character;
			SetWorkingImage(&SharedAssets::font);
			getimage(&character, ch % 16 * FONT_SIZE, ch / 16 * FONT_SIZE, FONT_SIZE, FONT_SIZE);
			SetWorkingImage();
			renderImage(x + r, y, &character);
			r += asciiwidth;
		} else {
			outtextxy(x + r, y, ch);
			r += textwidth(ch);
		}
	}
}

void renderTextCentered(int left, int top, int right, int bottom, wstring text) {
	renderText((right + left - renderTextWidth(text)) / 2, (bottom + top - FONT_SIZE) / 2, text);
}

#define BUF_LEN 20
#define NAME_LEN 10
bool valid(wstring name) {
	if (name.length() > NAME_LEN) return false;
	for (int i = 0; i < name.length(); ++i) {
		if (!isalnum(name[i]) && name[i] != '_') {
			return false;
		}
	}
	return true;
}

wstring askName(wstring title, wstring fallback) {
	wstring name;
	wchar_t buf[BUF_LEN];
	while (true) {
		InputBox(buf, BUF_LEN, (L"请输入昵称\n" + to_wstring(NAME_LEN) 
			+ L" 个字符以内，仅包含字母、数字和下划线" 
			+ fallback).c_str(), title.c_str());
		name = buf;
		if (valid(name)) break;
		MessageBox(GetHWnd(), L"您输入的昵称非法，请重新输入", L"昵称非法", MB_OK);
	}
	return name;
}