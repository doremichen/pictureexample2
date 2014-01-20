/*
 * Picture.cpp
 *
 *  Created on: 2014/1/16
 *      Author: AdamChen
 */
#include <string.h>
#include "Picture.h"
//Picture class
Picture::Picture(const char* const* str, int n) :
	p(new String_Pic(str, n)) {}

Picture::Picture(const Picture& orig) : p(orig.p)
{
	orig.p->use++;
}

Picture::Picture(P_Node* p_node) : p(p_node) {}

Picture::~Picture()
{
	if(--p->use == 0)
		delete p;
}



Picture& Picture::operator=(const Picture& orig)
{
	orig.p->use++;

	if(--p->use == 0)
		delete p;

	p = orig.p;
	return *this;
}

int Picture::height() const
{
	return p->height();
}

int Picture::width() const
{
	return p->width();
}

void Picture::display(ostream& o, int x, int y) const
{
	p->display(o, x, y);
}

//friend function
ostream& operator<<(ostream& os, const Picture& picture)
{
	int ht =  picture.height();

	for(int i = 0; i < ht; ++i) {
		picture.display(os, i, 0);
		os << endl;
	}

	return os;
}

Picture reframe(const Picture& pic, char c, char s, char t)
{
	return pic.p->reframe(c, s, t);
}

//P_Node class
P_Node::P_Node() : use(1) {}
P_Node::~P_Node() {}

int P_Node::max(int x, int y)
{
	return x > y ? x : y;
}

//String_Pic class
String_Pic::String_Pic(const char* const* p, int n) :
		data(new char* [n]), size(n)
{
	for(int i = 0; i < n; ++i) {
		data[i] = new char[strlen(p[i])+1];
		strcpy(data[i], p[i]);
	}
}

String_Pic::~String_Pic()
{
	for(int i = 0; i < size; ++i) {
		delete [] data[i];
	}

	delete [] data;
}

int String_Pic::height() const
{
	return size;
}

int String_Pic::width() const
{
	int n = 0;

	for(int i = 0; i < size; ++i) {
		n =  max(n, strlen(data[i]));
	}

	return n;
}

void String_Pic::display(ostream& os, int row, int width) const
{
	int start = 0;
	if(row >=0 && row < height()) {
		os << data[row];
		start = strlen(data[row]);
	}

	pad(os, start, width);
}

Picture String_Pic::reframe(char c, char s, char t)
{
	return this;
}

//Frame_Pic class
Frame_Pic::Frame_Pic(const Picture& pic, char c, char s, char t) :
		p(pic), corner(c), sideborder(s), topborder(t)
{

}

int Frame_Pic::height() const
{
	return p.height() + 2;
}

int Frame_Pic::width() const
{
	return p.width() + 2;
}

void Frame_Pic::display(ostream& os, int row, int wd) const
{
	if(row < 0 || row >= height()) {
		pad(os, 0, wd);
	}
	else {
		if(row == 0 || row ==height()-1) {
			os << "+";
			int i = p.width();
			while(--i >= 0) {
				os << "-";
			}
			os << "+";
		}
		else {
			os << "|";
			p.display(os, row - 1, p.width());
			os << "|";
		}

		pad(os, width(), wd);
	}
}

Picture Frame_Pic::reframe(char c, char s, char t)
{
	return new Frame_Pic(::reframe(p, c, s, t), c, s, t);
}

//friend function
Picture frame(const Picture& pic)
{
	return new Frame_Pic(pic);
}

//VCat_Pic class
VCat_Pic::VCat_Pic(const Picture& t, const Picture& b) :
		top(t), bottom(b)
{

}

int VCat_Pic::height() const
{
	return top.height() + bottom.height();
}

int VCat_Pic::width() const
{
	return max(top.width(), bottom.width());
}

void VCat_Pic::display(ostream& os, int row, int wd) const
{
	if(row >=0 && row < top.height()) {
		top.display(os, row, wd);
	}
	else if(row < top.height() + bottom.height()) {
		bottom.display(os, row-top.height(), wd);
	}
	else {
		pad(os, 0, wd);
	}
}

Picture VCat_Pic::reframe(char c, char s, char t)
{
	return new VCat_Pic(
			::reframe(top, c, s, t),
			::reframe(bottom, c, s, t));
}

//friend function
Picture operator&(const Picture& t, const Picture& b)
{
	return new VCat_Pic(t, b);
}

//HCat_Pic class
HCat_Pic::HCat_Pic(const Picture& l, const Picture& r) :
		left(l), right(r)
{

}

int HCat_Pic::height() const
{
	return max(left.height(), right.height());
}

int HCat_Pic::width() const
{
	return left.width() + right.width();
}

void HCat_Pic::display(ostream& os, int row, int wd) const
{
	left.display(os, row, left.width());
	right.display(os, row, right.width());
	pad(os, width() , wd);
}

Picture HCat_Pic::reframe(char c, char s, char t)
{
	return new HCat_Pic(
				::reframe(left, c, s, t),
				::reframe(right, c, s, t));
}

//friend function
Picture operator|(const Picture& l, const Picture& r)
{
	return new HCat_Pic(l, r);
}

//common function
static void pad(ostream& os, int x, int y)
{
	for(int i = x; i < y; ++i) {
		os << " ";
	}
}
