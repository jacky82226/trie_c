#include <iostream>
#include <fstream>
#include<vector>
#include<string.h>
#include<sstream>
#include<stdio.h>
#include<algorithm>
#include<set>
using namespace std;
typedef struct TrieNode
{
   struct TrieNode *children[128];
   void *data;
} TrieNode;

class email
{
    public:
    email()
    {
        from="";
        to="";
        time=0;
    }
    void clear()
    {
        from="";
        to="";
        time=0;
    }
    bool empty()
    {
        if(from==""&&to==""&&time==0&&content.empty())
        return true;
        return false;
    }
    string from;
    string to;
    int time;
    set<string>content;
};
string split(string str,int num);
string spacesplit(string str,int num);
int count(string str);
void getword(string str,int start);
void search(int id);
void contentsplit(string s, int idnum);
vector<string>word;
vector<email>mail(200000);
vector<int>ans;
vector<int>idnum;
void check();
bool longfirst(string a,string b);
bool lookup(string str,int idnum);
void add(char* s,int idnum);
int main(void)
{
    int i,id,before,after,tempnum;
    string str,tmp,searchfrom,searchto,searchbefore,searchafter,tempfrom,tempdate,date,printstr,tempstr;
    FILE* fptr=fopen("output","w");
    while(!cin.eof())
    {
        getline(cin,str);
        if(str.substr(0,3)=="add")
        {
            ifstream file(spacesplit(str,1).c_str());
            while(file!=NULL)
            {
                getline(file, str);
                if(str.substr(0,10)=="Message-ID")
                {
                    sscanf(split(str,2).c_str(),"%d",&id);
                    for(i=1;split(tempdate,i)!="\0";i++)
                    {
                        date+=split(tempdate,i);
                        date+=" ";
                    }
                    idnum.push_back(id);
                    mail[id].from=tempfrom;
                    mail[id].time=count(date);
                    date="";
                }
                else if(str.substr(0,4)=="From")
                    tempfrom=split(str,1);
                else if(str.substr(0,2)=="To")
                    mail[id].to=split(str,1);
                else if(str.substr(0,4)=="Date")
                    tempdate=str;
                else
                {
                    if(str.substr(0,7)=="Content")
                    {
                        getline(file, str);
                        contentsplit(str,id);
                    }
                    else if(str.substr(0,7)=="Subject")
                        contentsplit(str.substr(9),id);
                    else
                    {
                        if(str.empty())
                            continue;
                        else
                            contentsplit(str,id);
                    }
                }
            }
            fprintf(fptr,"Mail %d added, you have %d mails\n",id,idnum.size());
        }
        else if(str.substr(0,6)=="remove")
        {
            sscanf(str.c_str(),"remove %d",&id);
            if(mail[id].empty())
                fprintf(fptr,"Mail does not exist, you have %d mails\n",idnum.size());
            else
            {
                 for(i=0;i<idnum.size();i++)
                 {
                     if(idnum[i]==id&&!mail[id].empty())
                     {
                         mail[id].clear();
                         idnum.erase(idnum.begin()+i);
                         fprintf(fptr,"Mail %d removed, you have %d mails\n",id,idnum.size());
                         break;
                     }
             }
            }
        }
        else if(str.substr(0,6)=="search")
        {
            searchfrom=spacesplit(str,2);
            searchto=spacesplit(str,4);
            if(spacesplit(str,6)!="-")
            {
                for(i=6;i<11;i++)
                {
                    searchbefore+=spacesplit(str,i);
                    searchbefore+=" ";
                }
                before=count(searchbefore);
                if(spacesplit(str,12)!="-")
                {
                    for(i=12;i<17;i++)
                    {
                        searchafter+=spacesplit(str,i);
                        searchafter+=" ";
                    }
                    after=count(searchafter);
                    getword(str,17);
                }
                else
                {
                    getword(str,13);
                    searchafter="-";
                }
            }
            else
            {
                searchbefore="-";
                if(spacesplit(str,8)!="-")
                {
                    for(i=8;i<13;i++)
                    {
                        searchafter+=spacesplit(str,i);
                        searchafter+=" ";
                    }
                    after=count(searchafter);
                    getword(str,13);
                }
                else
                {
                    searchafter="-";
                    getword(str,9);
                }
            }
            sort(word.begin(),word.end(),longfirst);
            for(i=0;i<idnum.size();i++)
            {
                if(searchfrom=="-"||searchfrom==mail[idnum[i]].from)
                if(searchto=="-"||searchto==mail[idnum[i]].to)
                if(searchbefore=="-"||before>=mail[idnum[i]].time)
                if(searchafter=="-"||after<=mail[idnum[i]].time)
                        search(idnum[i]);
            }
            if(ans.empty())
            {
                fprintf(fptr,"-1\n");
            }
            else
            {
                sort(ans.begin(),ans.end());
                for(i=0;i<ans.size();i++)
                {
                    if(i!=ans.size()-1)
                         fprintf(fptr,"%d ",ans[i]);

                    else
                         fprintf(fptr,"%d\n",ans[i]);
                }
            }
            searchfrom="";
            searchto="";
            searchbefore="";
            searchafter="";
            ans.clear();
            word.clear();
        }
    }
    fclose(fptr);
    return 0;
}
string split(string str,int num)
{
    int k=0,j;
    string error("\0");
    if(str.empty())
    return error;
    if(!isalnum(str[0])&&num==0)
    {
        for(k=0;!isalnum(str[k]);k++);
        for(j=k;isalnum(str[j]);j++);
        return str.substr(k,j-k);
    }
    for(int i=0;i<num;i++)
    {
        for(;isalnum(str[k]);k++)
            if(str[k]=='\0'&&i<num)
                return error;
        for(;!isalnum(str[k]);k++)
            if(str[k]=='\0'&&i<num)
                return error;
    }
    for(j=k;isalnum(str[j]);j++);
    return str.substr(k,j-k);
}
string spacesplit(string str,int num)
{
    int k=0,j;
    string error("\0");
    for(int i=0;i<num;i++)
    {
        for(;str[k]!=' ';k++)
            if(str[k]=='\0'&&i<num)
                return error;
        for(;str[k]==' ';k++)
            if(str[k]=='\0'&&i<num)
                return error;
    }
    for(j=k;str[j]!=' '&&str[j]!='\0';j++);
    return str.substr(k,j-k);
}
int count(string str)
{
    stringstream stream;
    int time,count=0;
    sscanf(split(str,0).c_str(),"%d",&time);
    count+=time*24*60;
    if(split(str,1)=="January");
    else if(split(str,1)=="February")
        count+=31*24*60;
    else if(split(str,1)=="March")
        count+=59*24*60;
    else if(split(str,1)=="April")
        count+=90*24*60;
    else if(split(str,1)=="May")
        count+=120*24*60;
    else if(split(str,1)=="June")
        count+=151*24*60;
    else if(split(str,1)=="July")
        count+=181*24*60;
    else if(split(str,1)=="August")
        count+=212*24*60;
    else if(split(str,1)=="September")
        count+=243*24*60;
    else if(split(str,1)=="October")
        count+=273*24*60;
    else if(split(str,1)=="November")
        count+=304*24*60;
    else if(split(str,1)=="December")
        count+=334*24*60;
    sscanf(split(str,2).c_str(),"%d",&time);
    count+=(time-1)*365*24*60;
    if(split(str,1)!="January"&&split(str,1)!="February"&&time%4==0)
    {
        count+=24*60;
        if(time%100==0)
        {
            count-=24*60;
            if(time%400==0)
            {
                count+=24*60;
                if(time%4000==0)
                    count-=24*60;
            }
        }
    }
    sscanf(split(str,4).c_str(),"%d",&time);
    count+=time*60;
    sscanf(split(str,5).c_str(),"%d",&time);
    count+=time;
    return count;
}
void getword(string str,int start)
{
    for(int i=start;spacesplit(str,i)!="\0";i++)
            word.push_back(spacesplit(str,i));
}
void search(int id)
{
    for(int i=0;i<word.size();i++)
    {
        //if(mail[id].has(word[i]));
        if(lookup(word[i],id));
        else
            return;
    }
    ans.push_back(id);
}
void contentsplit(string s,int idnum){
    int i=0;
    char* a=(char*)s.c_str();
    char* b=new char[200];
    while(true){
        if(isalnum(*a))
            b[i++]=*a;
        else{
            b[i]='\0';
            if(i){
                add(b,idnum);
            }
            i=0;
        }
        if(!*a) break;
        a++;
    }
}
void add(char* s,int idnum)
{
    TrieNode* p = root;
    for (; *s; ++s)
    {
        if (!p->l[*s]) p->l[*s] = new TrieNode();
        p = p->l[*s];
    }
    p->hasid.insert(idnum);
}
bool lookup(string str,int idnum)
{
    TrieNode* p = root;
    char* s=(char*)str.c_str();
    for (; *s; ++s)
    {
        if (!p->l[*s]) return false;
        p = p->l[*s];
    }
    return p->hasid.find(idnum)!=p->hasid.end();
}
void release(TrieNode* p = root)
{
    for (int i=0; i<26; ++i)
        if (p->l[i])
            release(p->l[i]);
    delete p;
}
bool longfirst(string a,string b)
{
    return a.size()>b.size();
}
