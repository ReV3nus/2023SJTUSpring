#include "kvstore.h"
#include<pthread.h> 
#include<mutex>
#define sec 2200000000
using namespace std;
static inline uint64_t rdtsc()
{
uint32_t low, high;
asm volatile ("rdtsc" : "=a" (low), "=d" (high));
return ((uint64_t)high << 32) | low;
}
int main()
{
	srand(rdtsc());
	KVStore kvs("data");
	kvs.verbose=0;
	kvs.memtab.verbose=0;
	printf("KVS constructed success\n");
	for(int i=0;i<kvs.SSTLayer.size();i++)
	{
		printf("Layer %d Maxcnt=%d Mode=",i,kvs.SSTLayer[i].Maxcnt);
		printf(kvs.SSTLayer[i].mode==0?"Tiering\n":"Leveling\n");
	}
	while(1)
	{
		string opt;
		opt.clear();
		cin>>opt;
		if(opt=="TEST")
		{
			kvs.reset();
			uint64_t timer=rdtsc();
			int i=0,cnt=0,second=0;
//			FILE* opf=fopen("test.txt","w");
			for(int i=0;i<1024*16;i++)
			{
				kvs.put(i,string(i+1,'s'));
				kvs.get(i);
			}
			for(int i=0;i<1024*16;i++)
				kvs.get(i);
			for(int i=0;i<1024*16;i+=2)
				kvs.del(i);
			for(int i=0;i<1024*16;i++)
				kvs.get(i);
			timer=rdtsc()-timer;
			printf("time=%lf",timer*1.0/sec);
			return 0;
		}
		if(opt=="PUT")
		{
			string v;
			uint64_t k;
			cin>>k>>v;
			kvs.put(k,v);
			kvs.memtab.Display();
		}
		else if(opt=="GET")
		{
			uint64_t v;
			cin>>v;
			cout<<kvs.get(v)<<endl;
		}
		else if(opt=="SCAN")
		{
			uint64_t x,y;
			cin>>x>>y;
			list<pair<uint64_t,string>>List;
			List.clear();
			kvs.scan(x,y,List);
			for(list<pair<uint64_t,string>>::iterator it=List.begin();it!=List.end();it++)
			{
				printf("(%d,%s)\n",(*it).first,(*it).second.c_str());
			}
		}
		else if(opt=="RESET")
		{
			kvs.reset();
		}
		else if(opt=="DEL")
		{
			uint64_t k;
			cin>>k;
			cout<<kvs.del(k)<<endl;
			kvs.memtab.Display();
		}
		else if(opt=="MEMTAB")
		{
			kvs.memtab.Display();
		}
		else if(opt=="SIZE")
		{
			cout<<kvs.memtab.GetSize()<<endl;
		}
		else if(opt=="GENFILE")
		{
			kvs.Genfile();
			kvs.memtab.Display();
		}
		else if(opt=="STATUS")
		{
			kvs.__TEST_SSTStatus();
		}
		else if(opt=="GETFILE")
		{
			int x,y;
			cin>>x>>y;
			kvs.__TEST_Getfile(x,y);
		}
		else if(opt=="INIT")
		{
			kvs.reset();
			for(int i=0;i<10240;i++)
				kvs.put(i,string(i+1,'s'));
			return 0;
		}
		else if(opt=="SHUTDOWN")
		{
			return 0;
		}
	}
}
