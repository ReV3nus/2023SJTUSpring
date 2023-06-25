#include "FileBuf.h"
#include<stdio.h>
#include <fcntl.h>
//#include <io.h>
using namespace std;
SST::SST()
{
	memset(bloomfilter,0,sizeof(bloomfilter));
	kv.clear();
	lk=0;
	mink=1<<30,maxk=0;
}
SST::SST(string adr):addr(adr)
{
	memset(bloomfilter,0,sizeof(bloomfilter));
	kv.clear();
	lk=0;
	mink=1<<30,maxk=0;
}
void SST::Clear()
{
	memset(bloomfilter,0,sizeof(bloomfilter));
	kv.clear();
	lk=0;
	mink=1<<30,maxk=0;
	cntkv=0,timetag=0;
	addr="";
}
void SST::addCell(const Cell &num)
{
	kv.push_back(num);
	unsigned int hash[4]={0};
	uint64_t key=num.k;
	MurmurHash3_x64_128(&key, sizeof(key), 1, hash);
	for(int i=0;i<4;i++)
		bloomfilter[hash[i]%MAXBFSIZE]=1;
}
fpos_t TESTFPOS;
bool __TESTCHECK=0;
void SST::output()
{
	FILE *opf=fopen(addr.c_str(),"wb");
	
//	printf("output to %s\n",addr.c_str());
//	fgetpos(opf,&TESTFPOS);
//	cout<<"****** "<<TESTFPOS<<endl;
	
	uint32_t offset=0;
	fwrite(&timetag,8,1,opf);
	fwrite(&cntkv,8,1,opf);
	fwrite(&mink,8,1,opf);
	fwrite(&maxk,8,1,opf);
	
	
//	fgetpos(opf,&TESTFPOS);
//	cout<<"****** "<<TESTFPOS<<endl;
	
	for(int i=0;i<10240;i++)
	{
		unsigned char ret=0;
		for(int j=0;j<8;j++)
			ret+=(bloomfilter[8*i+j]<<j);
		fwrite(&ret,1,1,opf);
	}
	lk=kv.size();
	offset=10272+12*lk;
	
//	fgetpos(opf,&TESTFPOS);
//	cout<<"****** "<<TESTFPOS<<endl;
	
	for(int i=0;i<lk;i++)
	{
		fwrite(&kv[i].k,8,1,opf);
		fwrite(&offset,4,1,opf);
		offset+=kv[i].v.length()+1;
	}
	for(int i=0;i<lk;i++)
	{
		char ch;
		for(int j=0;j<kv[i].v.length();j++)
		{
			ch=kv[i].v[j];
			fwrite(&ch,1,1,opf);
		}
		ch='\0';
		fwrite(&ch,1,1,opf);
	}
	
//	fgetpos(opf,&TESTFPOS);
//	cout<<"****** "<<TESTFPOS<<endl;
//	printf("finished output, with size=%d\n",filelength(fileno(opf)));
	fclose(opf);
	
	if(__TESTCHECK)
	{
		FILE *ipf=fopen(addr.c_str(),"rb");
		printf("input size=%d\n",filelength(fileno(ipf)));
		fgetpos(ipf,&TESTFPOS);
		cout<<"###### "<<TESTFPOS<<endl;
		
		fread(&timetag,8,1,ipf);
		fread(&cntkv,8,1,ipf);
		fread(&mink,8,1,ipf);
		fread(&maxk,8,1,ipf);
		
		fgetpos(ipf,&TESTFPOS);
		cout<<"###### "<<TESTFPOS<<endl;
		
		
//		unsigned char retbuf[10240];
//		cout<<
//		fread(retbuf,1,10240,ipf)
//		<<endl;
		for(int i=0;i<10240;i++)
		{
			unsigned char ret=0;
			if(fread(&ret,1,1,ipf)!=1)
			{
//				printf("ERROR read at %d\n",i);
				if(ferror(ipf))printf("END at %d with error=%d size=%d\n",i,ferror(ipf),ftell(ipf));
			}
	//	fgetpos(ipf,&TESTFPOS);
	//	cout<<"!@#!@# "<<TESTFPOS<<' '<<32+i+1<<endl;
//			for(int j=0;j<8;j++)
//			{
//	//			printf("make BloomFilter[%d]=%d\n",8*i+j,(ret&(1<<j)));
//				bloomfilter[8*i+j]=(retbuf[i]&(1<<j));
//			}
		}
		
		
		fgetpos(ipf,&TESTFPOS);
		cout<<"###### "<<TESTFPOS<<endl;
		
		kv.clear();
		kv.push_back(Cell());
		fread(&kv[0].k,8,1,ipf);
		fread(&offset,4,1,ipf);
		
		cout<<"??? "<<kv[0].k<<' '<<offset<<endl;
		
		
		lk=(offset-10272)/12;
		printf("lk=%d\n",lk);
		for(int i=1;i<lk;i++)
		{
			kv.push_back(Cell());
			fread(&kv[i].k,8,1,ipf);
			fread(&offset,4,1,ipf);
	//		printf("%d %d\n",kv[i].k,offset);
		}
		for(int i=0;i<lk;i++)
		{
			char ch;
			fread(&ch,1,1,ipf);
			while(ch!='\0')
			{
	//			printf("%c",ch);
				kv[i].v+=ch;
				fread(&ch,1,1,ipf);
			}
		}
		fclose(ipf);
	}
}
int verbose=0;
void SST::input()
{
	lk=0; 
	FILE *ipf=fopen(addr.c_str(),"rb");
	if(verbose)printf("reading from %s\n",addr.c_str());
	if(ipf==NULL)
	{
		lk=-114514;
		printf("ERROR!NOT FOUND FILE %s\n",addr.c_str());
		return;
	}
	uint32_t offset=0;
	fread(&timetag,8,1,ipf);
	fread(&cntkv,8,1,ipf);
	fread(&mink,8,1,ipf);
	fread(&maxk,8,1,ipf);
	
	
//	fgetpos(ipf,&TESTFPOS);
//	cout<<"!@#!@# "<<TESTFPOS<<endl;
//	if(verbose)printf("timetag=%d cntkv=%d mink=%d maxk=%d\n",timetag,cntkv,mink,maxk);
	
//	fgetpos(ipf,&TESTFPOS);
//	cout<<"!@#!@# "<<TESTFPOS<<endl;
	
	
	
//	unsigned char retbuf[10240];/
//	<<endl;
	for(int i=0;i<10240;i++)
	{
		unsigned char ret;
		fread(&ret,1,1,ipf);
//	fgetpos(ipf,&TESTFPOS);
//	cout<<"!@#!@# "<<TESTFPOS<<' '<<32+i+1<<endl;
		for(int j=0;j<8;j++)
		{
//			printf("make BloomFilter[%d]=%d\n",8*i+j,(ret&(1<<j)));
			bloomfilter[8*i+j]=(ret&(1<<j));
		}
	}
	
	if(verbose)printf("BloomFilter Read Success.\n");
	
//	fgetpos(ipf,&TESTFPOS);
//	cout<<"!@#!@# "<<TESTFPOS<<endl;
	
	
	kv.clear();
	kv.push_back(Cell());
	fread(&kv[0].k,8,1,ipf);
	fread(&offset,4,1,ipf);
	
//	cout<<"??? "<<kv[0].k<<' '<<offset<<endl;
	
	
	lk=(offset-10272)/12;
	if(verbose)printf("lk=%d\n",lk);
	for(int i=1;i<lk;i++)
	{
		kv.push_back(Cell());
		fread(&kv[i].k,8,1,ipf);
		fread(&offset,4,1,ipf);
//		printf("%d %d\n",kv[i].k,offset);
	}
	for(int i=0;i<lk;i++)
	{
		char ch;
		fread(&ch,1,1,ipf);
		while(ch!='\0')
		{
//			printf("%c",ch);
			kv[i].v+=ch;
			fread(&ch,1,1,ipf);
		}
	}
	fclose(ipf);
//	printf("Finished Inputing.\n");
}
string SST::Find(uint64_t key)
{
	FILE *ipf=fopen(addr.c_str(),"rb");
	if(ipf==NULL)
	{
		lk=-114514;
		printf("ERROR!NOT FOUND FILE %s\n",addr.c_str());
		return "";
	}
	uint32_t offset=0;
	fread(&timetag,8,1,ipf);
	fread(&cntkv,8,1,ipf);
	fread(&mink,8,1,ipf);
	fread(&maxk,8,1,ipf);
	for(int i=0;i<10240;i++)
	{
		unsigned char ret=0;
		fread(&ret,1,1,ipf);
		for(int j=0;j<8;j++)
			bloomfilter[8*i+j]=(ret&(1<<j));
	}
	
	unsigned int hash[4]={0};
	MurmurHash3_x64_128(&key, sizeof(key), 1, hash);
	for(int i=0;i<4;i++)
		if(!bloomfilter[hash[i]%MAXBFSIZE])
		{
			fclose(ipf);
			return "";
		}
	
	kv.clear();
	kv.push_back(Cell());
	fread(&kv[0].k,8,1,ipf);
	fread(&offset,4,1,ipf);
	lk=(offset-10272)/12;
	int L=0,R=lk-1;
	while(L<R)
	{
		int mid=(L+R)>>1;
		fseek(ipf,10272+mid*12,SEEK_SET);
		uint64_t val;
		fread(&val,8,1,ipf);
		if(val==key)L=mid,R=mid;
		else if(val>key)R=mid-1;
		else L=mid+1;
	}
	fseek(ipf,10272+L*12,SEEK_SET);
	uint64_t val;
	fread(&val,8,1,ipf);
	if(val!=key)
	{
		fclose(ipf);
		return "";
	}
	fread(&val,4,1,ipf);
	fseek(ipf,val,SEEK_SET);
	string ret="";
	char ch;
	fread(&ch,1,1,ipf);
	while(ch!='\0')
	{
		ret+=ch;
		fread(&ch,1,1,ipf);
	}
	fclose(ipf);
	return ret;

//bad find
//	FILE *ipf=fopen(addr.c_str(),"rb");
//	if(ipf==NULL)
//	{
//		lk=-114514;
//		return "";
//	}
//	uint32_t offset=0;
//	fread(&timetag,8,1,ipf);
//	fread(&cntkv,8,1,ipf);
//	fread(&mink,8,1,ipf);
//	fread(&maxk,8,1,ipf);
//	if(maxk<key||mink>key)
//	{
//		fclose(ipf);
//		return "";
//	}
//	for(int i=0;i<10240;i++)
//	{
//		unsigned char ret=0;
//		fread(&ret,1,1,ipf);
//		for(int j=0;j<8;j++)
//			bloomfilter[8*i+j]=(ret&(1<<j));
//	}
//	
//	unsigned int hash[4]={0};
//	MurmurHash3_x64_128(&key, sizeof(key), 1, hash);
//	for(int i=0;i<4;i++)
//		if(!bloomfilter[hash[i]%MAXBFSIZE])
//		{
//			fclose(ipf);
//			return "";
//		}
//	
//	kv.clear();
//	kv.push_back(Cell());
//	fread(&kv[0].k,8,1,ipf);
//	fread(&offset,4,1,ipf);
//	lk=(offset-10272)/12;
//	for(int i=0;i<lk;i++)
//	{
//		fseek(ipf,10272+i*12,SEEK_SET);
//		uint64_t val;
//		fread(&val,8,1,ipf);
//		if(val==key)
//		{
//			fread(&val,4,1,ipf);
//			fseek(ipf,val,SEEK_SET);
//			string ret="";
//			char ch;
//			fread(&ch,1,1,ipf);
//			while(ch!='\0')
//			{
//				ret+=ch;
//				fread(&ch,1,1,ipf);
//			}
//			fclose(ipf);
//			return ret;
//		}
//	}
//	fclose(ipf);
//	return "";
}
void SST::Scan(uint64_t key1, uint64_t key2, list<pair<uint64_t,string>> &list,map<uint64_t,uint64_t> &Maxtime)
{
	FILE *ipf=fopen(addr.c_str(),"rb");
	if(ipf==NULL)
	{
		lk=-114514;
		printf("ERROR!NOT FOUND FILE %s\n",addr.c_str());
		return;
	}
	uint32_t offset=0;
	fread(&timetag,8,1,ipf);
	fread(&cntkv,8,1,ipf);
	fread(&mink,8,1,ipf);
	fread(&maxk,8,1,ipf);
	if(maxk<key1||mink>key2)return;
	for(int i=0;i<10240;i++)
	{
		unsigned char ret=0;
		fread(&ret,1,1,ipf);
		for(int j=0;j<8;j++)
			bloomfilter[8*i+j]=(ret&(1<<j));
	}
	
	kv.clear();
	kv.push_back(Cell());
	fread(&kv[0].k,8,1,ipf);
	fread(&offset,4,1,ipf);
	lk=(offset-10272)/12;
	int L=0,R=lk-1;
	while(L<R)
	{
		int mid=(L+R)>>1;
		fseek(ipf,10272+mid*12,SEEK_SET);
		uint64_t val;
		fread(&val,8,1,ipf);
		if(val==key1)L=mid,R=mid;
		else if(val>key1)R=mid;
		else L=mid+1;
	}
	while(L<lk)
	{
		fseek(ipf,10272+L*12,SEEK_SET);
		uint64_t val,adr;
		fread(&val,8,1,ipf);
		if(val>key2)break;
		if(val>=key1&&val<=key2)
		{
			fread(&adr,4,1,ipf);
			fseek(ipf,adr,SEEK_SET);
			string ret="";
			char ch;
			fread(&ch,1,1,ipf);
			while(ch!='\0')
			{
				ret+=ch;
				fread(&ch,1,1,ipf);
			}
			if(ret!=""&&ret!="~DELETED~")
			{
				if(Maxtime[val]<timetag)
					list.push_back(pair<uint64_t,string>(val,ret)),Maxtime[val]=timetag;
			}
		}
		L++;
	}
	return;
}
