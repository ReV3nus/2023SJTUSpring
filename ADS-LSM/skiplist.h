#include<stdio.h>
#include<string>
using namespace std;

#ifndef CELL
#define CELL
struct Cell
{
	uint64_t k;
	string v;
	Cell(const uint64_t &K,const string &V):k(K),v(V){}
	Cell(){};
};
#endif

class SkipListNode
{
	private:
		int level;
	public:
		Cell num;
		SkipListNode * nxtp=NULL;
		SkipListNode * downp=NULL;
		SkipListNode * upp=NULL;
		SkipListNode(const Cell &num,const int &level):num(num),level(level){}
		void  insertnum(SkipListNode * head,const Cell &num,const int &lev,int *flag);
//		double deletenum(SkipListNode * head,const unsigned long long &key,const bool &is_head);
		string searchnum(const SkipListNode * head,const unsigned long long &key,const bool &is_head);
		void output(const SkipListNode *head);
};

void FreeSL(SkipListNode *head);
