#include "memtab.h"
#include "FileBuf.h"
#include<stdlib.h>
#include<cmath>
#include<string>
using namespace std;
int GetSkipListLevel(const int &N)
{
	double jud=0.5;
	double tmp=rand()%10000/10000.0;
	int maxL=(int)(log(N)/log(2)+2);
	for(int i=1;i<maxL;i++)
		if(tmp>jud)return i;
		else jud/=2.0;
	return maxL;
}
memTable::memTable()
{
	srand(1145148*timetag+1919810);
	slhead=NULL,cntkv=0,maxl=0,timetag=1;
	mink=(1ull<<63),maxk=0;
	siz=32+10240;
}
void memTable::setdir(const std::string &pdir)
{
	dir=pdir;
}
void memTable::Clear()
{
	srand(1145148*timetag+1919810);
	cntkv=0,maxl=0;
	siz=32+10240;
	mink=(1ull<<63),maxk=0;
	FreeSL(slhead);
	slhead=NULL;
	timetag++;
}
void memTable::Insert(const Cell &num)
{
//	if(siz+num.v.length()+13>2097152)
//	{
//		GenFile();
//	}
	cntkv++;
	int tmplev=GetSkipListLevel(cntkv);
	int ReplaceFlag=-99999999;
//	printf("%d\n",num.k);
	if(tmplev>maxl)
	{
		maxl++,tmplev=maxl;
		SkipListNode * np=new SkipListNode(Cell(0,""),maxl);
		if(slhead!=NULL)slhead->upp=np;
		np->downp=slhead,slhead=np;
		np->num.k=-1;
	}
	slhead->insertnum(slhead,num,tmplev,&ReplaceFlag);
	mink=min(mink,num.k);
	maxk=max(maxk,num.k);
	if(ReplaceFlag!=-99999999)
		siz+=ReplaceFlag;
	else
		siz+=num.v.length()+1+12;
}
string memTable::Search(const uint64_t &key)
{
	return slhead->searchnum(slhead,key,1);
}
void memTable::Scan(uint64_t key1, uint64_t key2, list<pair<uint64_t,string>> &list,map<uint64_t,uint64_t> &Maxtime)
{
	SkipListNode * head=slhead;
	if(head==NULL)return;
	while(head)
	{
		if(head->num.k==key1)
		{
			while(head->downp!=NULL)head=head->downp;
			break;
		}
		if(head->nxtp==NULL||head->nxtp->num.k>key1)
		{
			if(head->downp==NULL)break;
			head=head->downp;
		}
		else head=head->nxtp;
	}
	while(head&&head->num.k<=key2)
	{
		if(head->num.k>=key1&&head->num.k<=key2&&head->num.v!="~DELETED~")list.push_back(pair<uint64_t,string>(head->num.k,head->num.v)),Maxtime[head->num.k]=timetag;
		head=head->nxtp;
	}
	return;
}
//double memTable::Delete(const uint64_t &key)
//{
//	return slhead->deletenum(slhead,key,1);
//}
void memTable::GenFile(int id)
{
	string path=dir+"/level-0/Table"+to_string(id)+".sst";
	SST sst_out;
	sst_out.addr=path;
	sst_out.timetag=timetag;
	SkipListNode * tmp=slhead;
	while(tmp->downp!=NULL)tmp=tmp->downp;
	tmp=tmp->nxtp;
	while(tmp)sst_out.addCell(tmp->num),tmp=tmp->nxtp;
	sst_out.cntkv=cntkv,sst_out.maxk=maxk,sst_out.mink=mink;
	sst_out.output();
	
//	if(verbose)
//		printf("Finished Generate at %s!\n",path.c_str());
	Clear();
}
memTable::~memTable()
{
	Clear();
}
void memTable::Display()
{
	slhead->output(slhead);
}
void memTable::getload()
{
	if(verbose)printf("loading memtable...\n");
	FILE* sav=fopen("memtab.sav","r");
	int n;
	char ch;
	fscanf(sav,"%d\n",&timetag);
	while(fscanf(sav,"%d",&n)!=EOF)
	{
		string tmp="";
		ch=fgetc(sav);
		while(ch==' '||ch=='\n')ch=fgetc(sav);
		while(ch!=' '&&ch!='\n'&&!feof(sav))
		{
			tmp+=ch;
			ch=fgetc(sav);
		}
		Insert(Cell(n,tmp));
		if(feof(sav))break;
	}
	fclose(sav);
	if(verbose)printf("finished memtable.\n");
}
void memTable::genload()
{
	if(verbose)printf("saving memtable...\n");
	FILE* sav=fopen("memtab.sav","w");
	fprintf(sav,"%d\n",timetag);
	SkipListNode * tmpl=slhead;
	if(tmpl==NULL)return;
	while(tmpl->downp!=NULL)tmpl=tmpl->downp;
	tmpl=tmpl->nxtp;
	while(tmpl)
		fprintf(sav,"%d %s\n",(*tmpl).num.k,(*tmpl).num.v.c_str()),tmpl=tmpl->nxtp;
	fclose(sav);
}


