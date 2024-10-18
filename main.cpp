#ifndef __PROGTEST__

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <cmath>
#include <cassert>
#include <typeinfo>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <memory>
#include <algorithm>
#include <type_traits>

using namespace std;

class CRect {
      public:
	CRect(double x,
	      double y,
	      double w,
	      double h)
		: m_X(x),
		  m_Y(y),
		  m_W(w),
		  m_H(h) {
	}

	friend ostream &operator<<(ostream &os,
				   const CRect &x) {
		return os << '(' << x.m_X << ',' << x.m_Y << ',' << x.m_W << ',' << x.m_H << ')';
	}

	double m_X;
	double m_Y;
	double m_W;
	double m_H;
};

#endif /* __PROGTEST__ */

class CItem {
      public:
	CItem(const int & id): m_id(id){}
	virtual ~CItem() = default;
	friend ostream & operator<<(ostream & os, const CItem & src);
	virtual void print(ostream & os, int format) const = 0;
	int m_id;
};

ostream &operator<<(ostream &os, const CItem &src) {
	src.print(os, 0);
	return os;
}


class CRelativePos : public CItem {
      public:
	CRelativePos(const int & id, const CRect & relPos) :
		CItem(id),
		m_relPos(relPos),
		m_absPos(0,0,0,0) {}
	virtual CRelativePos * clone() const = 0;
	void recalculate(const CRect & src);
	friend ostream &operator<<(ostream &out, const CRelativePos &item);

	CRect m_relPos;
	CRect m_absPos;
};

void CRelativePos::recalculate(const CRect &src) {
	this -> m_absPos.m_X = (src.m_W * this -> m_relPos.m_X) + src.m_X;
	this -> m_absPos.m_Y = (src.m_H * this -> m_relPos.m_Y) + src.m_Y;
	this -> m_absPos.m_H = src.m_H *  this -> m_relPos.m_H;
	this -> m_absPos.m_W = src.m_W *  this -> m_relPos.m_W;
}

ostream &operator<<(ostream &out, const CRelativePos &src) {
	src.print(out, 0);
	return out;
}

class CWindow : public CItem {
      public:
	CWindow(int id, const string &title, const CRect &absPos);
	CWindow(const CWindow & src);

	CWindow & add (const CRelativePos & src);

	CRelativePos *search(int id);
	CWindow & setPosition(CRect absPos);
	CWindow & operator=(const CWindow & src);
	void print(ostream &os, int format) const override;
	vector<unique_ptr<CRelativePos>> vect;
	string m_title;
	CRect m_absPos;
};

CWindow::CWindow(int id, const string &title, const CRect &absPos) : CItem(id) , m_title(title), m_absPos(absPos){}



CWindow::CWindow(const CWindow &src): CItem(src.m_id) , m_title(src.m_title), m_absPos(src.m_absPos) {
	for (auto & it : src.vect) {
		this -> add(*it);
	}
}

CWindow &CWindow::operator=(const CWindow &src) {
	if (&src == this)
		return *this;
	this->m_id = src.m_id;
	this->m_title = src.m_title;
	this->m_absPos = src.m_absPos;
	CWindow copy(src);
	swap (vect, copy.vect);
	return *this;

}

CWindow &CWindow::add(const CRelativePos & src) {
	vect.emplace_back(src.clone());

	vect.back() -> m_absPos.m_X = (this -> m_absPos.m_W * vect.back() -> m_relPos.m_X) + this -> m_absPos.m_X;
	vect.back() -> m_absPos.m_Y = (this -> m_absPos.m_H * vect.back() -> m_relPos.m_Y) + this -> m_absPos.m_Y;

	vect.back() -> m_absPos.m_H = this -> m_absPos.m_H * vect.back() -> m_relPos.m_H;
	vect.back() -> m_absPos.m_W = this -> m_absPos.m_W * vect.back() -> m_relPos.m_W;

	return *this;
}

void CWindow::print(ostream &os, int format) const {
	os << "[" << m_id << "] Window \"" << m_title << "\" " << m_absPos << endl;
	format = 2;


	for (auto &it : vect) {
		if (it == vect.back())
			format = 1;
		os << "+- ";
		it->print(os, format);
	}
}

CWindow &CWindow::setPosition(CRect absPos) {
	m_absPos = absPos;
	for (auto & it : vect) {
		it->recalculate(absPos);
	}
	return *this;
}

CRelativePos *CWindow::search(int id) {
	for (auto &it : vect) {
		if (it->m_id == id)
			return it.get();
	}
	return nullptr;
}

//"[0] Window \"Sample window\" (10,10,600,480)\n"

class CButton : public CRelativePos {
      public:
	CButton(const int id, const CRect &relPos, const string &name);
	CButton(const CButton & src);
	~CButton() override = default;
	CButton &operator = (const CButton & src);

	CRelativePos *clone() const override;
	void print(ostream &os, int format) const override;

	string m_name;
};

CButton::CButton(const int id, const CRect &relPos, const string &name): CRelativePos(id, relPos), m_name(name) {}

CButton::CButton(const CButton &src): CRelativePos(src.m_id, src.m_relPos), m_name(src.m_name){
	this -> m_absPos = src.m_absPos;
}

CButton &CButton::operator=(const CButton &src) {
	if (this == &src)
		return *this;

	this -> m_id = src.m_id;
	this -> m_relPos = src.m_relPos;
	this -> m_absPos = src.m_absPos;
	this -> m_name = src.m_name;
	return *this;
}

CRelativePos *CButton::clone() const {
	return new CButton(*this);
}

void CButton::print(ostream &os, int format) const {
	os << "[" << m_id << "] Button \"" << m_name << "\" " << m_absPos << endl;
}


class CInput : public CRelativePos {
      public:
	CInput(int id, const CRect &relPos, const string &value);
	CInput(const CInput & src);
	~CInput() = default;
	CInput &operator=(const CInput &src);
	void setValue(const string &value);
	string getValue();
	void print(ostream &os, int format) const override;
	CRelativePos *clone() const override;
	string m_value;
};

CInput::CInput(int id, const CRect &relPos, const string &value): CRelativePos(id, relPos), m_value(value){}

CInput::CInput(const CInput &src): CRelativePos(src.m_id, src.m_relPos), m_value(src.m_value){
	this -> m_absPos = src.m_absPos;
}

CInput &CInput::operator=(const CInput &src) {
	if (this == &src)
		return *this;

	this -> m_id = src.m_id;
	this -> m_relPos = src.m_relPos;
	this -> m_absPos = src.m_absPos;
	this -> m_value = src.m_value;
	return *this;
}

void CInput::setValue(const string &value) {
	m_value = value;
}

string CInput::getValue() {
	return this->m_value;
}




CRelativePos *CInput::clone() const {
	return new CInput(*this);
}

void CInput::print(ostream &os, int format) const {
	os << "[" << m_id << "] Input \"" << m_value << "\" " << m_absPos << endl;
}





class CLabel : public CRelativePos {
      public:
	CLabel(int id, const CRect &relPos, const string &label);
	CLabel(const CLabel & src);
	~CLabel() = default;
	CLabel &operator=(const CLabel &src);
	CRelativePos *clone() const override;
	void print(ostream &os, int format) const override;

	string m_label;
};

CLabel::CLabel(int id, const CRect &relPos, const string &label) : CRelativePos(id, relPos), m_label(label){}

CLabel::CLabel(const CLabel &src): CRelativePos(src.m_id, src.m_relPos), m_label(src.m_label){
	this -> m_absPos = src.m_absPos;
}

CLabel &CLabel::operator=(const CLabel &src) {
	if (this == &src)
		return *this;

	this -> m_id = src.m_id;
	this -> m_relPos = src.m_relPos;
	this -> m_absPos = src.m_absPos;
	this -> m_label = src.m_label;
	return *this;
}

CRelativePos *CLabel::clone() const {
	return new CLabel(*this);
}

void CLabel::print(ostream &os, int format) const {
	os << "[" << m_id << "] Label \"" << m_label << "\" " << m_absPos << endl;
}



class CComboBox : public CRelativePos{
      public:
	CComboBox(int id, const CRect &relPos);
	CComboBox(const CComboBox &src);
	~CComboBox() = default;
	CComboBox &operator=(const CComboBox &src);
	CComboBox &add(const string &src);
	void setSelected(int x);
	int getSelected() const;
	void print(ostream &os, int format) const override;
	CRelativePos *clone() const override;

	int m_select = 0;
	vector<string> m_boxes;
};

CComboBox::CComboBox(int id, const CRect &relPos): CRelativePos(id, relPos){}

CComboBox::CComboBox(const CComboBox &src): CRelativePos(src.m_id, src.m_relPos), m_select(src.m_select){
	this -> m_absPos = src.m_absPos;

	for (auto &it : src.m_boxes) {
		this -> add(it);
	}
}

CComboBox &CComboBox::add(const string &src) {
	m_boxes.emplace_back(src);
	return *this;
}


CComboBox &CComboBox::operator=(const CComboBox &src) {
	if (this == &src)
		return *this;

	this -> m_id = src.m_id;
	this -> m_relPos = src.m_relPos;
	this -> m_absPos = src.m_absPos;
	this -> m_select = src.m_select;
	m_boxes = src.m_boxes;
	return *this;
}

void CComboBox::setSelected(int x)
{
	m_select = x;
}

int CComboBox::getSelected() const
{
	return m_select;
}

CRelativePos *CComboBox::clone() const {
	return new CComboBox(*this);
}

void CComboBox::print(ostream &os, int format) const {
	os << "[" << m_id << "] ComboBox " << m_absPos << endl;
	int i = 0;
	for (auto it : m_boxes) {
		if (format == 2) {
			os << "|  ";
		}
		if (format == 1) {
			os << "   ";
		}

		if (i == m_select)
			os << "+->" << it << "<" << endl;
		else
			os << "+- " << it << endl;
		i++;
	}
}






// output operators

#ifndef __PROGTEST__

template<typename _T>
string toString(const _T &x) {
	ostringstream oss;
	oss << x;
	return oss.str();
}

int main(void) {
	//CWindow(int id, const string &title, const CRect &absPos);
	//CWindow win(1, "okno", CRect(1,1,1,1));
	//CButton but(2, CRect(0.5, 0.5, 0.25, 0.25), "butna");
	//CInput inp(2, CRect(0.5, 0.5, 0.25, 0.25),"input" );
	//win.add(but);
	//win.add(inp);

	//cout << win << endl;


	assert(sizeof(CButton) - sizeof(string) < sizeof(CComboBox) - sizeof(vector < string > ));
	assert(sizeof(CInput) - sizeof(string) < sizeof(CComboBox) - sizeof(vector < string > ));
	assert(sizeof(CLabel) - sizeof(string) < sizeof(CComboBox) - sizeof(vector < string > ));
	CWindow a(0, "Sample window", CRect(10, 10, 600, 480));
	a.add(CButton(1, CRect(0.1, 0.8, 0.3, 0.1), "Ok")).add(CButton(2, CRect(0.6, 0.8, 0.3, 0.1), "Cancel"));
	a.add(CLabel(10, CRect(0.1, 0.1, 0.2, 0.1), "Username:"));
	a.add(CInput(11, CRect(0.4, 0.1, 0.5, 0.1), "chucknorris"));
	a.add(CComboBox(20, CRect(0.1, 0.3, 0.8, 0.1)).add("Karate").add("Judo").add("Box").add("Progtest"));
	assert(toString(a) ==
	       "[0] Window \"Sample window\" (10,10,600,480)\n"
	       "+- [1] Button \"Ok\" (70,394,180,48)\n"
	       "+- [2] Button \"Cancel\" (370,394,180,48)\n"
	       "+- [10] Label \"Username:\" (70,58,120,48)\n"
	       "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
	       "+- [20] ComboBox (70,154,480,48)\n"
	       "   +->Karate<\n"
	       "   +- Judo\n"
	       "   +- Box\n"
	       "   +- Progtest\n");
	CWindow b = a;
	assert(toString(*b.search(20)) ==
	       "[20] ComboBox (70,154,480,48)\n"
	       "+->Karate<\n"
	       "+- Judo\n"
	       "+- Box\n"
	       "+- Progtest\n");
	assert(dynamic_cast<CComboBox &> ( *b.search(20)).getSelected() == 0);
	dynamic_cast<CComboBox &> ( *b.search(20)).setSelected(3);
	assert(dynamic_cast<CInput &> ( *b.search(11)).getValue() == "chucknorris");
	dynamic_cast<CInput &> ( *b.search(11)).setValue("chucknorris@fit.cvut.cz");
	b.add(CComboBox(21, CRect(0.1, 0.5, 0.8, 0.1)).add("PA2").add("OSY").add("Both"));
	assert(toString(b) ==
	       "[0] Window \"Sample window\" (10,10,600,480)\n"
	       "+- [1] Button \"Ok\" (70,394,180,48)\n"
	       "+- [2] Button \"Cancel\" (370,394,180,48)\n"
	       "+- [10] Label \"Username:\" (70,58,120,48)\n"
	       "+- [11] Input \"chucknorris@fit.cvut.cz\" (250,58,300,48)\n"
	       "+- [20] ComboBox (70,154,480,48)\n"
	       "|  +- Karate\n"
	       "|  +- Judo\n"
	       "|  +- Box\n"
	       "|  +->Progtest<\n"
	       "+- [21] ComboBox (70,250,480,48)\n"
	       "   +->PA2<\n"
	       "   +- OSY\n"
	       "   +- Both\n");
	assert(toString(a) ==
	       "[0] Window \"Sample window\" (10,10,600,480)\n"
	       "+- [1] Button \"Ok\" (70,394,180,48)\n"
	       "+- [2] Button \"Cancel\" (370,394,180,48)\n"
	       "+- [10] Label \"Username:\" (70,58,120,48)\n"
	       "+- [11] Input \"chucknorris\" (250,58,300,48)\n"
	       "+- [20] ComboBox (70,154,480,48)\n"
	       "   +->Karate<\n"
	       "   +- Judo\n"
	       "   +- Box\n"
	       "   +- Progtest\n");
	b.setPosition(CRect(20, 30, 640, 520));
	assert(toString(b) ==
	       "[0] Window \"Sample window\" (20,30,640,520)\n"
	       "+- [1] Button \"Ok\" (84,446,192,52)\n"
	       "+- [2] Button \"Cancel\" (404,446,192,52)\n"
	       "+- [10] Label \"Username:\" (84,82,128,52)\n"
	       "+- [11] Input \"chucknorris@fit.cvut.cz\" (276,82,320,52)\n"
	       "+- [20] ComboBox (84,186,512,52)\n"
	       "|  +- Karate\n"
	       "|  +- Judo\n"
	       "|  +- Box\n"
	       "|  +->Progtest<\n"
	       "+- [21] ComboBox (84,290,512,52)\n"
	       "   +->PA2<\n"
	       "   +- OSY\n"
	       "   +- Both\n");
	return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
