/*
 * Picture.h
 *
 *  Created on: 2014/1/16
 *      Author: AdamChen
 */

#ifndef PICTURE_H_
#define PICTURE_H_

#include <ostream>

using namespace std;

class P_Node;
class Picture {

	friend ostream& operator<<(ostream&, const Picture&);
	friend Picture frame(const Picture&);
	friend Picture reframe(const Picture&, char, char, char);
	friend Picture operator&(const Picture&, const Picture&);
	friend Picture operator|(const Picture&, const Picture&);

	friend class String_Pic;
	friend class Frame_Pic;
	friend class VCat_Pic;
	friend class HCat_Pic;


public:
	Picture();
	Picture(const char* const*, int);
	Picture(const Picture&);
	~Picture();

	Picture& operator=(const Picture&);

private:
	Picture(P_Node*);
	int height() const;
	int width() const;
	void display(ostream&, int, int) const;
	P_Node* p;

};


class P_Node {
	friend class Picture;
	friend Picture reframe(const Picture&, char, char, char);
	virtual Picture reframe(char, char, char) = 0;
protected:
	P_Node();
	virtual ~P_Node();
	virtual int height() const = 0;
	virtual int width() const = 0;
	virtual void display(ostream&, int, int) const = 0;

	static int max(int, int);

private:
	int use;
};

class String_Pic : public P_Node {
	friend class Picture;
	String_Pic(const char* const*, int);
	~String_Pic();
	int height() const;
	int width() const;
	void display(ostream&, int, int) const;
	Picture reframe(char, char, char);

	char** data;
	int size;
};
class Frame_Pic : public P_Node {
	friend Picture frame(const Picture&);
	Frame_Pic(const Picture&, char = '+', char = '|', char = '-');
	int height() const;
	int width() const;
	void display(ostream&, int, int) const;
	Picture reframe(char, char, char);

	Picture p;
	char corner;
	char sideborder;
	char topborder;
};
class VCat_Pic : public P_Node {
	friend Picture operator&(const Picture&, const Picture&);
	VCat_Pic(const Picture&, const Picture&);
	int height() const;
	int width() const;
	void display(ostream&, int, int) const;
	Picture reframe(char, char, char);

	Picture top, bottom;
};
class HCat_Pic : public P_Node {
	friend Picture operator|(const Picture&, const Picture&);
	HCat_Pic(const Picture&, const Picture&);
	int height() const;
	int width() const;
	void display(ostream&, int, int) const;
	Picture reframe(char, char, char);

	Picture left, right;
};

static void pad(ostream&, int, int);

#endif /* PICTURE_H_ */
