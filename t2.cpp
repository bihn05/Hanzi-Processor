#include <iostream>
#include <graphics.h>
#include <windows.h>

#define MENU_TYPE_ENTRY 8
#define BH_MAX 50
#define PT_BUF_LEN 2000
#define KEY_DOWN(VK_NONAME) ((GetKeyState(VK_NONAME) & 0x8000) ? 1:0)

ExMessage m;

bool is_inside(int x1, int y1, int x2, int y2) {
	int xx = (x1 > x2) ? x1: x2;
	int yx = (y1 > y2) ? y1: y2;
	int xm = (x1 < x2) ? x1: x2;
	int ym = (y1 < y2) ? y1: y2;
	return (m.x >= xm && m.x <= xx && m.y >= ym && m.y <= yx);
}

class MENU {
private:
	int x;
	int y;
	int l;
	int n;
	const wchar_t** str;
	int dx;
	int* lv;
	bool open;
public:
	void init(int _x, int _y, int _l, int _n, const wchar_t** _str, int* _lv) {
		x = _x;
		y = _y;
		l = _l;
		n = _n;
		str = _str;
		open = 0;
		lv = _lv;
	}
	void set_lvptr(int* _lv) {
		lv = _lv;
	}
 	void update() {
		if (is_inside(x, y, x + l, y + 17 * n + 20)) {
			setlinecolor(0xffff00);
			rectangle(m.x, m.y - 4, m.x + 5, m.y - 2);
			if (is_inside(x, y, x + l, y + 19) && m.message == WM_LBUTTONDOWN) {
				open ^= 1;
			}
			if (is_inside(x, y + 18, x + l, y + 17 * n + 18) && open) {
				for (int i = 0; i < n; i++) {
					if (is_inside(x, y + i * 17 + 18, x + l, y + i * 17 + 28)) {
						dx = i;
					}
				}
				if (m.message == WM_LBUTTONDOWN) {
					*lv = dx;
					open = 0;
				}
			}
		}
	}
	void draw() {
		setlinecolor(0x00ff00);
		rectangle(x, y, x + l, y + 18);
		settextstyle(16, 8, _T("PMingLiU"));
		if (!open) {
			setlinecolor(0x00ff00);
			settextcolor(0x00cf00);
			outtextxy(x+2, y+2, str[*lv]);
		} else {
			setlinecolor(0xffff00);
			settextcolor(0x00ff00);
			rectangle(x, y + 18, x + l, y + 18 + 17 * n);
			outtextxy(x + 2, y + 2, str[*lv]);
			for (int i = 0; i < n; i++) {
				outtextxy(x + 2, y + 19 + i * 17, str[i]);
			}
		}
	}
};
class BUTTON_STATE {
private:
	int x;
	int y;
	int n;
	int p;
public:
	void init(int _x, int _y, int _n, int _p) {
		x = _x;
		y = _y;
		n = _n;
		p = _p;
	}
	void update() {
		if (is_inside(x,y,x+30+30*n,y+30)) {
			setlinecolor(0xffff00);
			rectangle(m.x, m.y - 4, m.x + 5, m.y - 2);
			for (int i = 0; i < n; i++) {
				if (is_inside(x + i * 30, y, x + 30 + i * 30, y + 30) && m.message == WM_LBUTTONDOWN)p = i;
			}
		}
	}
	void draw() {
		setlinecolor(0x00ff00);
		for (int i = 0; i < n; i++) {
			rectangle(x + i * 30, y, x + 30 + i * 30, y + 30);
		}
		setlinecolor(0xffff00);
		rectangle(x + p * 30, y, x + 30 + p * 30, y + 30);
	}
	int get_value() {
		return p;
	}
};
class SLICE {
private:
	int x;
	int y;
	int sz;
	double* value;
	double value_min;
	double value_max;
	const wchar_t* name;
public:
	void init(int _x, int _y, int l, double* i, double mm, double mx, const wchar_t* nm) {
		x = _x;
		y = _y;
		sz = l;
		value = i;
		value_max = mx;
		value_min = mm;
		name = nm;
	}
	void set_limit(double mn, double mx) {
		value_min = mn;
		value_max = mx;
	}
	void set_vlptr(double* i) {
		value = i;
	}
	void update() {
		if (is_inside(x, y, x + sz, y + 15)) {
			setlinecolor(0xffff00);
			rectangle(m.x, m.y - 4, m.x + 5, m.y - 2);
			TCHAR s[9];
			settextcolor(0x7fff00);
			_stprintf(s, _T("%5.2f"), *value);
			settextstyle(12, 6, _T("PMingLiU"));
			outtextxy(m.x,m.y-16,s);
			if (KEY_DOWN(VK_LBUTTON)) {
				*value = value_min + (double)(m.x - x) / sz * (value_max - value_min);
			}
		}
	}
	void draw() {
		setlinecolor(0x00ff00);
		settextcolor(0x00ff00);
		if (is_inside(x, y, x + sz, y + 15))settextcolor(0xffff00);
		settextstyle(12, 6, _T("PMingLiU"));
		outtextxy(x + 2, y + 17, name);

		setlinecolor(0xffff00);
		line(x, y, x + sz, y);
		line(x, y + 15, x + sz, y + 15);
		line(x, y, x, y + 15);
		line(x + sz, y, x + sz, y + 15);

		setlinecolor(0x00ff00);
		line(x + (*value - value_min) / (value_max - value_min) * sz, y, x + (*value - value_min) / (value_max - value_min) * sz, y + 15);
	}
};
class SWITCH {
private:
	int x;
	int y;
	int* v;
	int mn;
	int mx;
public:
	void init(int _x, int _y, int* _v, int _mn, int _mx) {
		x = _x;
		y = _y;
		v = _v;
		mn = _mn;
		mx = _mx;
	}
	void update() {
		if (is_inside(x, y, x + 30, y + 60) && m.message == WM_LBUTTONDOWN)*v = mn;
		if (is_inside(x + 30, y, x + 60, y + 60) && m.message == WM_LBUTTONDOWN)*v = *v - 1;
		if (is_inside(x + 90, y, x + 120, y + 60) && m.message == WM_LBUTTONDOWN)*v = *v + 1;
		if (is_inside(x + 120, y, x + 150, y + 60) && m.message == WM_LBUTTONDOWN)*v = mx;
		if (*v < mn)*v = mn;
		if (*v > mx)*v = mx;
	}
	void draw() {
		setlinecolor(0xffff00);
		rectangle(x, y, x + 150, y + 60);
		setlinecolor(0x00ff00);
		line(x + 30, y, x + 30, y + 60);
		line(x + 60, y, x + 60, y + 60);
		line(x + 90, y, x + 90, y + 60);
		line(x + 120, y, x + 120, y + 60);
		TCHAR s[9];
		settextcolor(0x7fff00);
		_stprintf(s, _T("%d"), *v);
		settextstyle(12, 6, _T("PMingLiU"));
		outtextxy(x + 12, y + 25, _T("<<"));
		outtextxy(x + 42, y + 25, _T(" -"));
		outtextxy(x + 72, y + 25, s);
		outtextxy(x + 102, y + 25, _T("+"));
		outtextxy(x + 132, y + 25, _T(">>"));
	}
};
class BUTTON {
private:
	int x;
	int y;
	int w;
	int h;
	const wchar_t* name;
	bool en;
public:
	void init(int nx, int ny, int nw, int nh, const wchar_t* nname) {
		x = nx;
		y = ny;
		w = nw;
		h = nh;
		name = nname;
	}
	bool get_stt() {
		return en;
	}
	void set_off() {
		en = 0;
	}
	void update() {
		if (is_inside(x, y, x + w, y + h) && m.message == WM_LBUTTONDOWN) {
			en = 1;
		}
	}
	void draw() {
		setlinecolor(0x00ff00);
		if (en == 1) {
			setlinecolor(0x00ffff);
		}
		rectangle(x, y, x + w, y + h);
	}
};

struct PT {
	double x;
	double y;
	PT operator ^ (const double k) const {
		return PT{ k * x,k * y };
	}
	PT operator * (const PT b) const {
		return PT{ x * b.x,y * b.y };
	}
	PT operator + (const PT b) const {
		return PT{ x + b.x,y + b.y };
	}
	PT operator - (const PT b) const {
		return PT{ x - b.x,y - b.y };
	}
	bool operator == (const PT b) const {
		if (b.x == x && b.y == y) {
			return 1;
		}
		else {
			return 0;
		}
	}
	bool operator != (const PT b) const {
		if (b.x == x && b.y == y) {
			return 0;
		}
		else {
			return 1;
		}
	}
};
bool is_zero(PT p) {
	return (p.x == 0 && p.y == 0);
}
PT pt_mul(PT p1, PT p2) {
	return PT{ p1.x * p2.x , p1.y * p2.y };
}
POINT _dkru(PT p) {
	return POINT{int(p.x),int(p.y)};
}

POINT polybuf[200];
int polylen;

PT ptbuf[PT_BUF_LEN];
int ptptps; //stack top or stack length

SWITCH swt_bhindex;
SLICE la, lb, lc, lx, ly, lz;
BUTTON_STATE stbut_type;
MENU menu;
BUTTON
const wchar_t* menu_str[MENU_TYPE_ENTRY]{
	L"無" , 
	L"橫 常規" , 
	L"橫 無襯綫" , 
	L"竪 常規" , 
	L"竪 無頭" , 
	L"竪 無尾" , 
	L"竪 無襯綫" ,
	L"點 常規" 
};

void addpoint(PT p) {
	ptbuf[ptptps++] = p;
}
void ptbufdisplay(PT offset, PT scale) {
	setlinecolor(0x003300);
	rectangle(offset.x - scale.x, offset.y - scale.y, offset.x + scale.x, offset.y + scale.y);
	rectangle(offset.x - scale.x * 0.7, offset.y - scale.y * 0.7, offset.x + scale.x * 0.7, offset.y + scale.y * 0.7);

	polylen = 0;
	for (int i = 0; i < ptptps; i++) {
		if (is_zero(ptbuf[i])) {
			if (polylen > 2) {
				setlinecolor(0x00ff00);
				polygon(polybuf, polylen);
			}
			polylen = 0;
		} else {
			polybuf[polylen++] = _dkru(offset + (scale * ptbuf[i]));
			putpixel(polybuf[polylen - 1].x, polybuf[polylen - 1].y, 0x007f00);
		}
//		putpixel(offset.x, offset.y, 0xffffff);
	}
}
struct BIHW {
	PT p1;
	PT p2;
	PT p3;
	int type;
	double va;
	double vb;
	double vc;
	double vx;
	double vy;
	double vz;

	void _xxru() {
		switch (type) {
		case 1: {
			addpoint(p1 + PT{ 0,-va });
			addpoint(p2 + PT{ +vb,-va });
			addpoint(p2 + PT{ 0,+vc });
			addpoint(p2 + PT{ -vb,+va });
			addpoint(p1 + PT{ 0,+va });
			addpoint(PT{ 0,0 });
			break;
		}
		case 2: {
			addpoint(p1 + PT{ 0,-va });
			addpoint(p2 + PT{ 0,-va });
			addpoint(p2 + PT{ 0,+va });
			addpoint(p1 + PT{ 0,+va });
			addpoint(PT{ 0,0 });
			break;
		}
		case 6: {
			addpoint(p1 + PT{ -va,0});
			addpoint(p2 + PT{ -va,0 });
			addpoint(p2 + PT{ +va,0 });
			addpoint(p1 + PT{ +va,0 });
			addpoint(PT{ 0,0 });
			break;
		}
		default: {
			addpoint(PT{ 0,0 });
			break;
		}
		}
	}
};

BIHW bh[BH_MAX];
int crt_bh;
PT pt_buf[4096];

void obj_init() {
	crt_bh = 0;
	swt_bhindex.init(620, 20, &crt_bh, 0, 100);
	menu.init(620, 100, 150, MENU_TYPE_ENTRY, menu_str, &bh[crt_bh].type);
	bh[crt_bh].p1 = PT{-0.5,0};
	bh[crt_bh].p2 = PT{+0.5,0};
	bh[crt_bh].va = 0.02;
	bh[crt_bh].type = 1;
	stbut_type.init(620, 350, 4, 0);
	settextstyle(12, 6, _T("PMingLiU"));
	la.init(620, 400, 150, &bh[crt_bh].va, 0, 1, L"VALUE_A");
	lb.init(620, 430, 150, &bh[crt_bh].vb, -1, 1, L"VALUE_B");
	lc.init(620, 460, 150, &bh[crt_bh].vc, -1, 1, L"VALUE_C");
	lx.init(620, 490, 150, &bh[crt_bh].vx, -1, 1, L"VALUE_X");
	ly.init(620, 520, 150, &bh[crt_bh].vy, -1, 1, L"VALUE_Y");
	lz.init(620, 550, 150, &bh[crt_bh].vz, -1, 1, L"VALUE_Z");
}
void obj_update() {
	swt_bhindex.update();
	menu.update();
	stbut_type.update();
	la.update();
	lb.update();
	lc.update();
	lx.update();
	ly.update();
	lz.update();

	if (stbut_type.get_value() == 1) {
		setlinecolor(0xffff00);
		circle(300 + bh[crt_bh].p1.x * 256, 300 - bh[crt_bh].p1.y * 256, 2);
		if (is_inside(0, 0, 600, 600) && KEY_DOWN(VK_LBUTTON)) {
			if (KEY_DOWN(VK_SHIFT)) {
				switch (bh[crt_bh].type) {
				case 1:
				case 2: {
					bh[crt_bh].p1.x = (double)(m.x - 300) / 256;
					bh[crt_bh].p1.y = bh[crt_bh].p2.y;
					break;
				}
				case 3:
				case 4:
				case 5:
				case 6: {
					bh[crt_bh].p1.x = bh[crt_bh].p2.x;
					bh[crt_bh].p1.y = (double)(300 - m.y) / 256;
					break;
				}

				}
			}
			else {
				bh[crt_bh].p1.x = 1.0 * (m.x - 300) / 256;
				bh[crt_bh].p1.y = -1.0 * (m.y - 300) / 256;
			}
		}
	}
	if (stbut_type.get_value() == 2) {
		setlinecolor(0xffff00);
		circle(300 + bh[crt_bh].p2.x * 256, 300 - bh[crt_bh].p2.y * 256, 2);
		if (is_inside(0, 0, 600, 600) && KEY_DOWN(VK_LBUTTON)) {
			bh[crt_bh].p2.x = 1.0 * (m.x - 300) / 256;
			bh[crt_bh].p2.y = -1.0 * (m.y - 300) / 256;
		}
	}
}
void obj_bond() {
	menu.set_lvptr(&bh[crt_bh].type);
	la.set_vlptr(&bh[crt_bh].va);
	lb.set_vlptr(&bh[crt_bh].vb);
	lc.set_vlptr(&bh[crt_bh].vc);
	lx.set_vlptr(&bh[crt_bh].vx);
	ly.set_vlptr(&bh[crt_bh].vy);
	lz.set_vlptr(&bh[crt_bh].vz);

	ptptps = 0;
	for (int i = 0; i < BH_MAX; i++) {
		bh[i]._xxru();
	}
}
void draw_framework() {
	swt_bhindex.draw();
	menu.draw();
	stbut_type.draw();
	la.draw();
	lb.draw();
	lc.draw();
	lx.draw();
	ly.draw();
	lz.draw();

	setlinecolor(0x00ff00);
	line(600, 0, 600, 600);
	circle(m.x, m.y, 2);
	
	settextstyle(12, 6, _T("PMingLiU"));
	outtextxy(6, 3, L"SHIFT");
	outtextxy(45, 3, L"CTRL");
	setlinecolor(0x003300);
	if (KEY_DOWN(VK_SHIFT)) {
		setlinecolor(0x00ff00);
	}
	rectangle(2, 2, 40, 14);
	if (m.message == WM_LBUTTONDOWN) {
		setlinecolor(0x0000ff);
		circle(m.x, m.y, 4);
	}
	if (m.message == WM_RBUTTONDOWN) {
		setlinecolor(0xff00ff);
		circle(m.x, m.y, 6);
	}
}
int main() {
	initgraph(800, 600);
	setbkcolor(0x000000);
	setfillcolor(0xffffff);
	setlinecolor(0xffffff);
	settextstyle(12, 6, _T("PMingLiU"));

	obj_init();

	int timer1 = 0;

	for (;;) {
		BeginBatchDraw();
		if (timer1 == 1) {
			cleardevice();
			timer1 = 0;
		}
		timer1++;
		obj_update();
		obj_bond();
		ptbufdisplay(PT{ 300,300 }, PT{ 256,-256 });
		draw_framework();
		FlushBatchDraw();
		m = getmessage(EX_MOUSE);
	}
	EndBatchDraw();
	return 0;
}