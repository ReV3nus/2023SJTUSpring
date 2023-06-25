#include "skiplist.h"
#include<list>
#include<string>
#include<map>
using namespace std;
class memTable
{
	private:
		int maxl;
	public:
		int verbose=0;
		uint64_t timetag,cntkv,mink=(1ull<<63),maxk;
		string dir;
		int siz;
		SkipListNode * slhead;
		memTable();
		void setdir(const std::string &pdir);
		void Clear();
		void Insert(const Cell &num);
//		double Delete(const uint64_t &key);
		void Scan(uint64_t key1, uint64_t key2, list<pair<uint64_t,string>> &list,map<uint64_t,uint64_t> &Maxtime);
		string Search(const uint64_t &key);
		void GenFile(int id);
		~memTable();
		void Display();
		int GetSize(){return siz;}
		void getload();
		void genload();
};
