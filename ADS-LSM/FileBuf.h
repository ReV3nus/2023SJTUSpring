#include<cstdio>
#include<cstring>
#include<fstream>
#include<iostream>
#include<vector>
#include<list>
#include<map>
#include "MurmurHash3.h"
using namespace std;

#define MAXBFSIZE 81920
#define MAXSSTSIZE 2097152

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

struct SST
{
	uint64_t timetag,cntkv,mink,maxk;
	int lk=0;
	string addr;
	bool bloomfilter[MAXBFSIZE];
	vector<Cell>kv;
	SST();
	SST(string adr);
	void Clear();
	void addCell(const Cell &num);
	void output();
	void input();
	void Scan(uint64_t key1, uint64_t key2, list<pair<uint64_t,string>> &list, map<uint64_t,uint64_t> &Maxtime);
	string Find(uint64_t key);
		
};
