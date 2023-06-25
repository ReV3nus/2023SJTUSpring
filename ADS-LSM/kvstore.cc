#include "kvstore.h"
#include <string>
#include<cstring>
#include<iostream>
#include<cstdio>
#include<algorithm>
#include<io.h>
#include<map>
#define MAXSSTSIZE 2097152
using namespace std;
void KVStore::getconf()
{
	if(verbose)printf("getting config...\n");
	FILE* conf=fopen("default.conf","r");
	int n,m;
	char mstring[11];
	while(fscanf(conf,"%d %d %s",&n,&m,mstring)!=EOF)
	{
		while(SSTLayer.size()<n+1)SSTLayer.push_back(SST_Layer());
		SSTLayer[n].Maxcnt=m;
		SSTLayer[n].mode=(SST_Layer::LayerMode)(!memcmp(mstring,"Leveling",8));
	}
}
void KVStore::getload()
{
	if(verbose)printf("loading KVS...\n");
	FILE* sav=fopen("KVS.sav","r");
	int n,m;
	char readinstring[64];
	//Layers
	fscanf(sav,"%d",&n);
	for(int i=0;i<n;i++)
	{
		if(i==SSTLayer.size())
		{
			SSTLayer.push_back(SST_Layer());
			SSTLayer[i].Maxcnt=SSTLayer[i-1].Maxcnt*2;
			SSTLayer[i].mode=SSTLayer[i-1].mode;
		}
		fscanf(sav,"%d %d",&SSTLayer[i].Maxid,&m);
		for(int j=0;j<m;j++)
		{
			SSTLayer[i].SSTStatus.push_back(SST_Status());
			fscanf(sav,"%d %d %d %s",&SSTLayer[i].SSTStatus[j].Mink,&SSTLayer[i].SSTStatus[j].Maxk,&SSTLayer[i].SSTStatus[j].Timetag,readinstring);
			SSTLayer[i].SSTStatus[j].addr=readinstring;
		}
	}
	fclose(sav);
	if(verbose)printf("finished loading kvs.\n");
}
void KVStore::genload()
{
	if(verbose)printf("saving KVS...\n");
	FILE* sav=fopen("KVS.sav","w");
	int n,m;
	fprintf(sav,"%d\n",SSTLayer.size());
	for(int i=0;i<SSTLayer.size();i++)
	{
		fprintf(sav,"%d %d\n",SSTLayer[i].Maxid,SSTLayer[i].SSTStatus.size());
		for(int j=0;j<SSTLayer[i].SSTStatus.size();j++)
		{
			fprintf(sav,"%d %d %d %s\n",SSTLayer[i].SSTStatus[j].Mink,SSTLayer[i].SSTStatus[j].Maxk,SSTLayer[i].SSTStatus[j].Timetag,SSTLayer[i].SSTStatus[j].addr.c_str());
		}
	}
	fclose(sav);
}
KVStore::KVStore(const std::string &dir): KVStoreAPI(dir)
{
	if(verbose)printf("Starting kvs construction.\n");
	srand(114514810);
	Dir=dir;
	memtab.setdir(dir);
	getconf();
	getload();
	memtab.getload();
}

KVStore::~KVStore()
{
	memtab.genload();
	genload();
}

void KVStore::put(uint64_t key, const std::string &s)
{
	if(memtab.siz+s.length()+13>MAXSSTSIZE)
		Genfile();
	memtab.Insert(Cell(key,s));
}
void KVStore::prep4test()
{
//	for(int i=0;i<SSTLayer.size();i++)
//	{
//		for(int j=0;j<SSTLayer[i].SSTStatus.size();j++)
//		{
//			SST tmpsst(SSTLayer[i].SSTStatus[j].addr);
//			tmpsst.input();
//			memcpy(SSTLayer[i].SSTStatus[j].bloomfilter,tmpsst.bloomfilter,sizeof(tmpsst.bloomfilter));
//		}
//	}
}
std::string KVStore::get(uint64_t key)
{
//	printf("getting %d\n",key);
	string ret=memtab.Search(key);
	if(ret=="~DELETED~")return "";
	if(ret!="")return ret;
	for(int i=0;i<SSTLayer.size();i++)
	{
		for(int j=0;j<SSTLayer[i].SSTStatus.size();j++)
		{
			if(key<SSTLayer[i].SSTStatus[j].Mink||key>SSTLayer[i].SSTStatus[j].Maxk)continue;
			
			SST tmpsst(SSTLayer[i].SSTStatus[j].addr);
			ret=tmpsst.Find(key);
			if(ret=="~DELETED~")return "";
			if(ret!="")return ret;
		}
	}
	return "";

//bad get
	 
//	string ret=memtab.Search(key);
//	if(ret=="~DELETED~")return "";
//	if(ret!="")return ret;
//	for(int i=0;i<SSTLayer.size();i++)
//	{
//		_finddata_t file;
//		long lf;
//		string path=Dir+"/level-"+to_string(i)+"/*";
//		if((lf=_findfirst(path.c_str(),&file))==-1)
//		{
//			printf("no found in %s\n",path.c_str());
//			continue;
//		}
//		else
//		{
//			do
//			{
//				SST tmpsst(Dir+"/level-"+to_string(i)+"/"+file.name);
//				ret=tmpsst.Find(key);
//				if(ret!="")return ret;
//			}while(_findnext(lf,&file)==0);
//		}
//	}
//	return "";
}

bool KVStore::del(uint64_t key)
{
	if(get(key)=="")return 0;
	put(key,"~DELETED~");
	return 1; 
}

/**
 * This resets the kvstore. All key-value pairs should be removed,
 * including memtable and all sstables files.
 */
void KVStore::reset()
{
	memtab.Clear();
	for(int i=0;i<SSTLayer.size();i++)
	{
		for(int j=0;j<SSTLayer[i].SSTStatus.size();j++)
			remove(SSTLayer[i].SSTStatus[j].addr.c_str());
		SSTLayer[i].SSTStatus.clear();
	}
	SSTLayer.clear();
	getconf();
}

/**
 * Return a list including all the key-value pair between key1 and key2.
 * keys in the list should be in an ascending order.
 * An empty string indicates not found.
 */
bool CMPLIST(pair<uint64_t, std::string>A,pair<uint64_t, std::string>B)
{
	return A.first<B.first;
}
void KVStore::scan(uint64_t key1, uint64_t key2, std::list<std::pair<uint64_t, std::string> > &list)
{	
	map<uint64_t,uint64_t>Maxtime;
	Maxtime.clear();
	memtab.Scan(key1,key2,list,Maxtime);
	for(int i=0;i<SSTLayer.size();i++)
	{
		for(int j=0;j<SSTLayer[i].SSTStatus.size();j++)
		{
			SST tmpsst(SSTLayer[i].SSTStatus[j].addr);
			tmpsst.Scan(key1,key2,list,Maxtime);
		}
	}
	list.sort(CMPLIST);
}
void KVStore::Genfile()
{
	SSTLayer[0].SSTStatus.push_back(SST_Status(memtab.mink,memtab.maxk,memtab.timetag,Dir+"/level-0/Table"+to_string(SSTLayer[0].Maxid)+".sst"));
	if(verbose)printf("generated SST(%d,%d,%d,%s)\n",memtab.mink,memtab.maxk,memtab.timetag,(Dir+"/level-0/Table"+to_string(SSTLayer[0].Maxid)+".sst").c_str());
//	if(verbose)printf("?????? %d %s\n",SSTLayer[0].SSTStatus[SSTLayer[0].SSTStatus.size()-1].Timetag,SSTLayer[0].SSTStatus[SSTLayer[0].SSTStatus.size()-1].addr.c_str());
	memtab.GenFile(SSTLayer[0].Maxid++);
	if(SSTLayer[0].SSTStatus.size()>SSTLayer[0].Maxcnt)
	{
//		__TEST_Getfile(0,0);
		int tmp=0;
		while(LayerCompact(tmp))tmp++;
	}
}
void KVStore::__TEST_SSTStatus()
{
	for(int i=0;i<SSTLayer.size();i++)
	{
		printf("Layer %d Maxcnt=%d mode=%s Maxid=%d Status:\n",i,SSTLayer[i].Maxcnt,(SSTLayer[i].mode==0?"Tiering\n":"Leveling\n"),SSTLayer[i].Maxid);
		for(int j=0;j<SSTLayer[i].SSTStatus.size();j++)
		{
			printf("SST %d: Mink=%d Maxk=%d Timetag=%d addr=%s\n",j,SSTLayer[i].SSTStatus[j].Mink,SSTLayer[i].SSTStatus[j].Maxk,SSTLayer[i].SSTStatus[j].Timetag,SSTLayer[i].SSTStatus[j].addr.c_str());
		}
	}
}
void KVStore::__TEST_Getfile(int x,int y)
{
	SST sst(Dir+"/level-"+to_string(x)+"/Table"+to_string(y)+".sst");
	if(verbose)printf("GETTING SST from %s\n",sst.addr.c_str());
	sst.input();
	if(sst.lk==-114514)return;
//	printf("mink=%d maxk=%d timetag=%d lk=%d\n",sst.mink,sst.maxk,sst.timetag,sst.lk);
//	for(int i=0;i<sst.lk;i++)
//		printf("%d %s\n",sst.kv[i].k,sst.kv[i].v.c_str());
}

bool SSTStatus_CMP(SST_Status A,SST_Status B)
{
	if(A.Timetag==B.Timetag)return A.Mink>B.Mink;
	return A.Timetag>B.Timetag;
}
bool sltSST_CMP(const SST &A,const SST &B)
{
	return A.timetag<B.timetag;
}
bool KVStore::LayerCompact(int x)
{
	uint64_t MINK=1<<30,MAXK=0;
	uint64_t MAXTIMETAG=0;
	
	verbose=0;
	if(verbose)printf("Compacting at Layer %d\n",x);
	if(x==SSTLayer.size()-1)
	{
		SSTLayer.push_back(SST_Layer());
		SSTLayer[x+1].Maxid=0,SSTLayer[x+1].Maxcnt=SSTLayer[x].Maxcnt*2;
		SSTLayer[x+1].mode=SSTLayer[x].mode,SSTLayer[x+1].SSTStatus.clear();
	}
	sltSST.clear();
	for(int i=0;i<SSTLayer[x].SSTStatus.size();i++)
	{
		for(int j=0;j<i;j++)
		{
			if(SSTLayer[x].SSTStatus[i].Timetag==SSTLayer[x].SSTStatus[j].Timetag)
			{
				if(SSTLayer[x].SSTStatus[j].Mink<=SSTLayer[x].SSTStatus[i].Mink)
					swap(SSTLayer[x].SSTStatus[i],SSTLayer[x].SSTStatus[j]);
			}
			else if(SSTLayer[x].SSTStatus[j].Timetag<=SSTLayer[x].SSTStatus[i].Timetag)
				swap(SSTLayer[x].SSTStatus[i],SSTLayer[x].SSTStatus[j]);
		}
	}
//	sort(SSTLayer[x].SSTStatus.begin(),SSTLayer[x].SSTStatus.end(),SSTStatus_CMP);
//	if(verbose)
//	{
//		for(int i=0;i<SSTLayer[x].SSTStatus.size();i++)
//		{
//			printf("SST %d: Mink=%d Maxk=%d Timetag=%d addr=%s\n",i,SSTLayer[x].SSTStatus[i].Mink,SSTLayer[x].SSTStatus[i].Maxk,SSTLayer[x].SSTStatus[i].Timetag,SSTLayer[x].SSTStatus[i].addr.c_str());
//		}
//	}
	if(SSTLayer[x].mode==0)
	{
		int tmplen=SSTLayer[x].SSTStatus.size();
		for(int i=0;i<tmplen;i++)
		{
			sltSST.push_back(SST(SSTLayer[x].SSTStatus[i].addr));
			MINK=min(MINK,SSTLayer[x].SSTStatus[i].Mink);
			MAXK=min(MAXK,SSTLayer[x].SSTStatus[i].Maxk);
		}
		SSTLayer[x].SSTStatus.clear();
	}
	else
	{
		while(SSTLayer[x].SSTStatus.size()>SSTLayer[x].Maxcnt)
		{
			sltSST.push_back(SST(SSTLayer[x].SSTStatus[SSTLayer[x].SSTStatus.size()-1].addr));
			MINK=min(MINK,SSTLayer[x].SSTStatus[SSTLayer[x].SSTStatus.size()-1].Mink);
			MAXK=min(MAXK,SSTLayer[x].SSTStatus[SSTLayer[x].SSTStatus.size()-1].Maxk);
			SSTLayer[x].SSTStatus.pop_back();
		}
	}
	if(SSTLayer[x+1].mode==1)
	{
		for(int i=0;i<SSTLayer[x+1].SSTStatus.size();i++)
		{
			int tmpmin=SSTLayer[x+1].SSTStatus[i].Mink,tmpmax=SSTLayer[x+1].SSTStatus[i].Maxk;
			if(tmpmax<MINK||tmpmin>MAXK)continue;
			sltSST.push_back(SST(SSTLayer[x+1].SSTStatus[i].addr));
			SSTLayer[x+1].SSTStatus.erase(SSTLayer[x+1].SSTStatus.begin()+i);
			i--;
		}
	}
	for(int i=0;i<sltSST.size();i++)
	{
		if(verbose)printf("Reading Selected SST at %s\n",sltSST[i].addr.c_str());
		sltSST[i].input();
		remove(sltSST[i].addr.c_str());
		if(verbose)printf("Finished Reading SST with timetag=%d.\n",sltSST[i].timetag);
		int tmpj=i-1;
		while(tmpj>=0&&sltSST[tmpj].timetag>sltSST[tmpj+1].timetag)swap(sltSST[tmpj],sltSST[tmpj+1]),tmpj--;
	}
//	if(verbose)
//	{
//		printf("testing..\n");
//		printf("siz=%d\n",sltSST.size());
//			printf("Head Status:");
//			printf("addr=%s ",(*sltSST.begin()).addr.c_str());
//			printf("timetag=%s \n",(*sltSST.begin()).timetag);
//			printf("End Status:");
//			printf("addr=%s ",(*(sltSST.begin()+sltSST.size()-1)).addr.c_str());
//			printf("timetag=%s \n",(*(sltSST.begin()+sltSST.size()-1)).timetag);
//		
//	}
	if(verbose)printf("Finished Sorting.\n");
	if(verbose)printf("Inserting to memtab\n");
	memtab.Clear();
	for(int i=0;i<sltSST.size();i++)
	{
		if(verbose)printf("Now Inserting sltSST %d/%d \n",i+1,sltSST.size());
		for(int j=0;j<sltSST[i].kv.size();j++)
		{
//			if(verbose)printf("inserting %d/%d\n",sltSST[i].kv[j].k+1,sltSST[i].kv.size());
			memtab.Insert(sltSST[i].kv[j]);
		}
		MAXTIMETAG=max(MAXTIMETAG,sltSST[i].timetag);
		if(verbose)printf("Now memtab cntkv=%d\n",memtab.cntkv);
	}
	if(verbose)printf("Finised Insert.\n");
	SkipListNode * slp=memtab.slhead;
	while(slp->downp!=NULL)slp=slp->downp;
	slp=slp->nxtp;
	int tmpsize=32+10240;
	SSTBUF.Clear();
	if(verbose)printf("Finished output Preparation.\n");
	while(slp!=NULL)
	{
		int tmpprelen=(*slp).num.v.length()+12;
		if(tmpsize+tmpprelen>MAXSSTSIZE)
		{
			string tmppath=Dir+"/level-"+to_string(x+1)+"/Table"+to_string(SSTLayer[x+1].Maxid++)+".sst";
			SSTBUF.addr=tmppath;
			SSTBUF.timetag=MAXTIMETAG;
			SSTLayer[x+1].SSTStatus.push_back(SST_Status(SSTBUF.mink,SSTBUF.maxk,SSTBUF.timetag,SSTBUF.addr));
			SSTBUF.output();
			tmpsize=32+10240;
			SSTBUF.Clear();
		}
		tmpsize+=tmpprelen;
		SSTBUF.addCell((*slp).num);
		SSTBUF.maxk=max(SSTBUF.maxk,(*slp).num.k);
		SSTBUF.mink=min(SSTBUF.mink,(*slp).num.k);
		slp=slp->nxtp;
	}
	if(tmpsize>10240+32)
	{
		string tmppath=Dir+"/level-"+to_string(x+1)+"/Table"+to_string(SSTLayer[x+1].Maxid++)+".sst";
		SSTBUF.addr=tmppath;
		SSTBUF.timetag=MAXTIMETAG;
		SSTLayer[x+1].SSTStatus.push_back(SST_Status(SSTBUF.mink,SSTBUF.maxk,SSTBUF.timetag,SSTBUF.addr));
		SSTBUF.output();
	}
	if(verbose)printf("Finished SST output.\n");
	memtab.Clear();
	if(verbose)
	{
		printf("Finished Layer %d compaction.\n",x);
		printf("Next Layer size=%d",SSTLayer[x+1].SSTStatus.size());
		printf(" Maxcnt=%d\n",SSTLayer[x+1].Maxcnt);
	}
	verbose=0;
	if(SSTLayer[x+1].SSTStatus.size()>SSTLayer[x+1].Maxcnt)return 1;
	return 0;
}
