#pragma once
#include "memtab.h"
#include<vector>
#include "kvstore_api.h"
#include "FileBuf.h"
struct SST_Status
{
	
	uint64_t Mink,Maxk,Timetag;
	string addr;
	SST_Status()
	{
		Mink=(1<<30),Maxk=-1;
		Timetag=-1;
	}
	SST_Status(uint64_t a,uint64_t b,uint64_t c):Mink(a),Maxk(b),Timetag(c){}
	SST_Status(uint64_t a,uint64_t b,uint64_t c,string d):Mink(a),Maxk(b),Timetag(c),addr(d){}
};
struct SST_Layer
{
	int Maxcnt;
	enum LayerMode{Tiering,Leveling} mode;
	
	int Maxid; 
	vector<SST_Status>SSTStatus;
	SST_Layer()
	{
		Maxcnt=0;
		Maxid=0;
		SSTStatus.clear();
	}
};

class KVStore : public KVStoreAPI {
	// You can add your implementation here
private:
	vector<SST>sltSST;
	SST SSTBUF; 
public:
	int verbose=0; 
	memTable memtab;
	string Dir;
	vector<SST_Layer>SSTLayer;
	void Genfile();
	void getconf();
	void getload();
	void genload();
	void prep4test();
	bool LayerCompact(int x);
	void __TEST_SSTStatus();
	void __TEST_Getfile(int x,int y);
	
	KVStore(const std::string &dir);

	~KVStore();

	void put(uint64_t key, const std::string &s) override;

	std::string get(uint64_t key) override;

	bool del(uint64_t key) override;

	void reset() override;

	void scan(uint64_t key1, uint64_t key2, std::list<std::pair<uint64_t, std::string> > &list) override;
};
