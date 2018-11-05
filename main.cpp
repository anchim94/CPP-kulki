#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <list>
#include <iostream>
#include <map>
#include "winbgi2.h"

class kulka {
public:
	double x;
	double y;
	double vx;
	double vy;
	double fx;
	double fy;
	double const dt;
	double m;
	double color;
public:
	kulka();
	kulka(double x_, double y_, double vx_, double vy_);
	void przesun();
	void rysuj();
	void sily();
	void przyspiesz();
	//void rozszczep();
};

kulka::kulka() : dt(1) {
	x = 400.0 * std::rand() / RAND_MAX + 50;
	y = 400.0 * std::rand() / RAND_MAX + 50;
	vx = 20.0 * std::rand() / RAND_MAX - 10;
	vy = 20.0 * std::rand() / RAND_MAX - 10;
	fx = 0;
	fy = 0;
	m = 10;
	color = 0.1;
}

kulka::kulka(double x_, double y_, double vx_, double vy_) : dt(1)
{
	x = x_;
	y = y_;
	vx = vx_;
	vy = vy_;
	color = 0.2;
}

void kulka::przesun()
{
	x += vx * dt;
	y += vy * dt;
	fx = 0;
	fy = 0;
	if ((x >= 450) || (x <= 50)) vx = -vx;
	if ((y >= 450) || (y <= 50)) vy = -vy;
}
void kulka::rysuj()
{
	setcolor(color);
	circle(x, y, 5);
	line(x, y, x + vx, y + vy);
	setcolor(0.9);
	rectangle(45, 45, 455, 455);
}

void kulka::sily()
{
	/*if (y >= 450) fy = -100;
	if (y <= 50) fy = 100;
	if (x >= 450) fx = -100;
	if (x <= 50) fx = 100; */
}

void kulka::przyspiesz()
{
	vx += (fx / m) * dt;
	vy += (fy / m) * dt;
}

const double dx = 50.0;
const double dy = 50.0;

class Box {
public:
	int ix, iy;
	//Box();
	Box(int ix, int iy);
	bool operator <(const Box& B2) const {
		if (ix < B2.ix) return true;
		if (ix > B2.ix) return false;
		if (iy < B2.iy) return true;
		if (iy > B2.iy) return false;
		return false;
	}
	bool operator >(const Box& B2) const {
		if (ix > B2.ix) return true;
		if (ix < B2.ix) return false;
		if (iy > B2.iy) return true;
		if (iy < B2.iy) return false;
		return false;
	}
};

Box::Box(int ix_, int iy_)
{
	ix = ix_;
	iy = iy_;
	setcolor(0.9);
	rectangle(ix*dx, iy*dy, ix*dx + dx, iy*dy + dy);
}

typedef std::list<kulka> Kulki;
typedef std::list<Kulki::iterator> LI;
typedef std::map<Box, LI> Index;

void main()
{
	graphics(500, 500);
	int liczba_kulek = 10;
	//double dt = 1.0;
	Kulki tab;
	for (int i = 0; i < liczba_kulek; i++) tab.push_back(kulka());
	while (animate(15)) {
		clear();
		Index index;
		int j = 0;

		for (Kulki::iterator it = tab.begin(); it != tab.end(); it++)
		{
			it->przesun();
			it->sily();
			it->przyspiesz();
			it->rysuj();
			//if (it->x < 50) tab.push_back(kulka());
			for (Kulki::iterator jt = it;;)
			{
				jt++; if (jt == tab.end()) break;
				//std::cout << jt->x << "--" << it->x << "--" << jt->y << "--" << it->y << std::endl;
				if (((((jt->x) - (it->x))*((jt->x) - (it->x))) + (((jt->y) - (it->y)) * ((jt->y) - (it->y)))) <= 100.0) {
					j++;
					it->color = 0.9;
					jt->color = 0.9;
					std::cout << "ALL" << jt->x << "--" << it->x << "--" << jt->y << "--" << it->y << std::endl;
					//break;
				}
			}
			if (((double)std::rand() / RAND_MAX) < 0.002) tab.push_back(kulka(it->x, it->y, -(it->vx), -(it->vy)));//
		}
		//std::cout << j << std::endl;

		for (Kulki::iterator it = tab.begin(); it != tab.end(); it++)
		{
			Box b(it->x / dx, it->y / dy);
			index[b].push_back(it);
		}
		int k = 0;
		for (Kulki::iterator it = tab.begin(); it != tab.end(); it++)
		{
			Box b(it->x / dx, it->y / dy);
			for (LI::iterator LIT = index[b].begin(); LIT != index[b].end(); LIT++)
			{
				Kulki::iterator jt = *LIT;
				if (jt != it) {
					if (((((jt->x) - (it->x))*((jt->x) - (it->x))) + (((jt->y) - (it->y)) * ((jt->y) - (it->y)))) <= 100.0) {
						k++;
						it->color = 0.8;
						jt->color = 0.8;
						std::cout << "BOX" << jt->x << "--" << it->x << "--" << jt->y << "--" << it->y << std::endl;
					}
				}

			}
		}
		char buffer[10];
		sprintf(buffer, "BLISKO: %d %d", j, k/2);
		outtextxy(380, 460, buffer);
		wait();
	}
	wait();
}

