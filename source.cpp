#include<stdio.h>
#include<iostream>
#include<stdlib.h>
#include<string.h>
using namespace std;


typedef struct _Project
{
	int no;//the number of project
	int *stuArray;//the array of the student arrangment
	int size;//the size of the array
	//int size_left;//the left size of the array
}Project,*pProject; 
typedef struct _Student
{
	int no;//the number of the student
	int *proArray;//the array of the projects that he chose
	int size;//
	int size_left;//
}Student,*pStudent;

class Match
{
public:
	Match(const char *filename,int _M,int _N);
private:
	void Init();
	void LoadDataFromFile(FILE* fp);
	void StartMatch();
	void StoreResult(FILE *fp);
	int M,N;//the number of projects and students
	pProject mpProjects;//the total projects 
	pStudent mpStudents;//the total students
};
Match::Match(const char *filename,int _M,int _N):M(_M),N(_N)
{
	Init();
	if(filename==NULL) return;
	FILE *fp;
	if((fp=fopen(filename,"r"))==NULL)
	{
		cout<<"open file failed!"<<endl;
		return;
	}
	LoadDataFromFile(fp);
	fclose(fp);
	StartMatch();
	if((fp=fopen("out.txt","wt"))==NULL)
	{
		cout<<"open file failed!"<<endl;
		return;
	}
	StoreResult(fp);
	fclose(fp);
}
void Match::Init()
{
	mpProjects=new Project[M];
	int i,j;
	for(i=0;i<M;i++)
	{
		mpProjects[i].no=i+1;
		mpProjects[i].stuArray=new int[3*N];
		mpProjects[i].size=0;
		//mpProjects[i].size_left=0;	
		for(j=0;j<3*N;j++)
		{
			mpProjects[i].stuArray[j]=-1;//initial
		}
	}
	mpStudents=new Student[N];
	for(i=0;i<N;i++)
	{
		mpStudents[i].no=i+1;
		mpStudents[i].proArray=new int[M];
		mpStudents[i].size=0;
		mpStudents[i].size_left=0;
		for(j=0;j<M;j++)
		{
			mpStudents[i].proArray[j]=-1;//
		}
	}
}
void Match::LoadDataFromFile(FILE *fp)
{
	
	char ch;
	int p=-1,s=-1;
	bool change=false;
	printf("input data:\n");
	while((ch=fgetc(fp))!=EOF)
	{
		if(ch=='0') break;
		if(ch=='\n')//set data 
		{
			int k=0;
			while(mpStudents[s-1].proArray[k]!=-1) k++;
			mpStudents[s-1].proArray[k]=p-1;
			mpStudents[s-1].size++;
			mpStudents[s-1].size_left++;	
			printf("%d,%d\n",s,p);
			s=-1,p=-1;
			change=false;	
		}
		else
		{	if(ch>='0'&&ch<='9')
			{
				if(!change)//input the first number whitch represents student
				{
					if(s==-1) s=ch-'0';
					else s=s*10+ch-'0';
				}else//input the second number whitch represent project
				{
					if(p==-1) p=ch-'0';
					else p=p*10+ch-'0';	
				}
			}else if(ch==' ')
			{
				change=true;
			}
		}
	}
}
void Match::StartMatch()
{
	int i;
	for(i=0;i<N;i++)//through N students
	{
		int order=0;
		int match=0;
		const int S=M;
		int chose[S];
		memset(chose,0,sizeof(int)*S);
		while(mpStudents[i].size_left>0)
		{
			int j;
			match=0;
			for(j=0;j<mpStudents[i].size;j++)
			{
				if(mpStudents[i].proArray[j]==-2) continue;//already arranged
				int pro=mpStudents[i].proArray[j];
				if(mpProjects[pro].stuArray[order]==-1)
				{
					chose[match++]=pro;
				}
			}
			if(match>0)//find match projects
			{
				if(match==1)//only one project match
				{
					int p=chose[0];
					mpProjects[p].stuArray[order]=i;
					mpProjects[p].size++;
					int k=0;
					while(mpStudents[i].proArray[k]!=p) k++;
					mpStudents[i].proArray[k]=-2;//arrange this project,set 0
					mpStudents[i].size_left--;
				}else//more than one matched,chose the one whitch left_size is max
				{
					int i_min=0;
					int k,min=0;	
					for(k=0;k<match;k++)
					{
						int c=chose[k];
						if(mpProjects[c].size<min) i_min=k;	
					}
					int p=chose[i_min];
					mpProjects[p].stuArray[order]=i;
					mpProjects[p].size++;
					k=0;
					while(mpStudents[i].proArray[k]!=p) k++;
					mpStudents[i].proArray[k]=-2;//arrange this project,set 0
					mpStudents[i].size_left--;
				}
			}
			order++;
		}
	}
}
void Match::StoreResult(FILE *fp)
{
	printf("result :\n");
	for(int i=0;i<M;i++)
	{
		printf("project %d:,",i+1);
		int j;
		char *buf=new char[20];
		memset(buf,'\0',20);
		char *str=new char[5];
		memset(str,'\0',5);
		for(j=0;j<mpProjects[i].size;j++)
		{
			printf("%d ",mpProjects[i].stuArray[j]+1);
			sprintf(str,"%d ",mpProjects[i].stuArray[j]+1);
			strcat(buf,str);
		}
		strcat(buf,"\n");
		fputs(buf,fp);	
		printf("\n");
	}
	printf("write data to file.\n");
}
int main(int argc,char *argv[])
{
	Match m("data.txt",4,4);
	return 0;
}
