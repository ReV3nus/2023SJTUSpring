#include<stdio.h>
#include<stdlib.h>
#include<cmath>
#include "skiplist.h"

using namespace std;

void FreeSL(SkipListNode *head)
{
	if(head==NULL)return;
	while(head)
	{
		SkipListNode * nowpoint=head->nxtp;
		while(nowpoint)
		{
			SkipListNode * nxtpoint=nowpoint->nxtp;
			delete nowpoint;
			nowpoint=nxtpoint;
		}
		nowpoint=head->downp;
		delete head;
		head=nowpoint;
	}
}

void  SkipListNode::insertnum(SkipListNode * head,const Cell &num,const int &lev,int *flag)
{
	if(head==NULL)return;
	SkipListNode * oldpoint=NULL;
	while(head)
	{
//		printf("inserting %d at %d,level %d\n",num.k,head->num.k,head->level);
		if(head->nxtp==NULL||head->nxtp->num.k>num.k)
		{
			if(head->level>lev)head=head->downp;
			else
			{
				if(head->num.k==num.k)
				{
//					printf("****replacing at (%d,%d)\n",head->num.k,head->level);
					if(oldpoint)oldpoint->downp=head,head->upp=oldpoint;
					(*flag)=num.v.length()-head->num.v.length();
					head->num.v=num.v;
					SkipListNode * nowpoint=head->upp;
					SkipListNode * nxtpoint;
					while(nowpoint)
					{
						nowpoint->num.v=num.v;
						nxtpoint=nowpoint->upp;
						nowpoint=nxtpoint;
					}
					nowpoint=head->downp;
					while(nowpoint)
					{
						nowpoint->num.v=num.v;
						nxtpoint=nowpoint->downp;
						nowpoint=nxtpoint;
					}
//					insertnum(head->downp,num,lev,flag);
						{
							while(1)
							{
//								printf("testing: now head is (%d,%d)\n",head->num.k,head->level);
								if(head->upp==NULL)break;
								head=head->upp;
							}
							while(1)
							{
//								printf("testing: now head is (%d,%d)\n",head->num.k,head->level);
								if(head->downp==NULL)break;
								head=head->downp;
							}
							
						}
					return;
				}
				else
				{
					SkipListNode * newpoint=new SkipListNode(num,head->level);
					newpoint->nxtp=head->nxtp;
					head->nxtp=newpoint;
					if(oldpoint)
					{
						oldpoint->downp=newpoint,newpoint->upp=oldpoint;
//						printf("connect (%d,%d),(%d,%d)\n",oldpoint->num.k,oldpoint->level,newpoint->num.k,newpoint->level);
					}
					oldpoint=newpoint;
					head=head->downp;
				}
			}
		}
		else head=head->nxtp;
	}
	
//	if(head->level>lev)return insertnum(head->downp,num,lev,flag);
////	if(is_head||head->num<num)
//	if(head->num.k==num.k)
//	{
//		(*flag)=num.v.length()-head->num.v.length();
//		head->num.v=num.v;
//		SkipListNode * nowpoint=head;
//		while(nowpoint&&nowpoint->upp!=NULL&&nowpoint->upp->num.v!=num.v)nowpoint=nowpoint->upp,nowpoint->num.v=num.v;
//		insertnum(head->downp,num,lev,flag);
//		return head;
//	} 
//	if(head->nxtp==NULL||head->nxtp->num.k>num.k)
//	{
//		SkipListNode * newpoint=new SkipListNode(num,head->level);
//		newpoint->nxtp=head->nxtp;
//		head->nxtp=newpoint;
//		newpoint->downp=insertnum(head->downp,num,lev,flag);
//		if(newpoint->downp!=NULL)newpoint->downp->upp=newpoint;
//		return newpoint;
//	}
//	else return insertnum(head->nxtp,num,lev,flag);
}

string SkipListNode::searchnum(const SkipListNode *head, const uint64_t &key,const bool &is_head)
{
	if(head==NULL)return "";
	while(head)
	{
//		printf("now searching at %d level %d\n",head->num.k,head->level);
		if(head->num.k==key)return head->num.v;
		if(head->nxtp==NULL||head->nxtp->num.k>key)head=head->downp;
		else head=head->nxtp;
	}
	return "";
}
//double SkipListNode::deletenum(SkipListNode *head, const uint64_t &key,const bool &is_head)
//{
//	if(head==NULL)return 0;
//	if(is_head||head->num.k<key)
//	{
//		if(head->nxtp==NULL||head->nxtp->num.k>key)return deletenum(head->downp,key,is_head);
//		else if(head->nxtp->num.k==key)
//		{
//			if(head->nxtp->num.v=="~DELETED~")return 0;
//			SkipListNode *tmp=head->nxtp;
//			head->nxtp=tmp->nxtp;
//			delete(tmp);
//			deletenum(head->downp,key,is_head);
//			return 1;
//		}
//		else return deletenum(head->nxtp,key,0);
//	}
//}

void SkipListNode::output(const SkipListNode * head)
{
	if(head==NULL)return;
	SkipListNode * nowpoint=head->nxtp;
	printf("Level:%d  head  ",head->level);
	while(nowpoint!=NULL)
	{
		printf("--->  (%d,%s)  ",nowpoint->num.k,nowpoint->num.v.c_str());
		nowpoint=nowpoint->nxtp;
	}
	printf("\n");
	output(head->downp);
}
