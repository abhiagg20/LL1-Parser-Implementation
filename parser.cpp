#include <bits/stdc++.h>

using namespace std;

class parser
{
public:
    bool system;                
    int n_prod;                      
    string start_symbol;        
	vector<string> p_str;        
    vector<string> glhs[100];   
    vector<string> grhs[100];  
    vector<string> variable;      
    vector<string> terminal;      
    vector<string> pre_first[100];  
    vector<string> pre_follow[100]; 
    vector<int> ptable[50][50]; 

    bool check_terminal (string);  
    bool check_variable (string);   
    void read_grammar ();           

    parser (int);                 
    parser ();                
    ~parser ();                  
};

parser::parser(int x)
{
    n_prod = x;
    system=true;
}

parser::parser()        
{
    n_prod = 0;
    system=true;
}

parser::~parser()  
{

}
bool parser::check_terminal (string x)
{
    bool answer = true;
    for(string::iterator i = x.begin() ; i != x.end() ; i++ )
    {
        if(isupper(*i))
        {
            answer = false;
            break;
        }
    }
    return answer;
}

bool parser::check_variable (string x)
{
    bool answer = true;
    for(std::string::iterator i = x.begin() ; i != x.end() ; i++ )
    {
        if(!isupper(*i))
        {
            answer = false;
            break;
        }
    }
    return answer;
}

void parser::read_grammar()
{
    char temp[1000];
    int startsym=1;
    FILE *fp;
    fp = fopen("grammar3.txt","r");
    fscanf(fp,"%d",&n_prod);
    cout << n_prod << endl;
    for(int i = 0; i < n_prod; i++)
    {
        if(system)
            fscanf(fp," %[^\n]s",temp);
        else
            fscanf(fp," %[^\r]s",temp);
        char c=temp[0];
        int curr_pos=0;
        string lt_var;
        while(c!=' ')
        {
            lt_var.push_back(c);
            curr_pos++;
            c=temp[curr_pos];
        }
        if(check_variable(lt_var))
        {
            glhs[i].push_back(lt_var);
            if(startsym)
            {
                start_symbol=lt_var;
                startsym=0;
            }
            int flag=0;
            for(int k=0;k<variable.size();k++)
            {
                if(variable[k].compare(lt_var)==0)
                {
                    flag=1;
                    break;
                }
            }
            if(flag==0)
            {
                variable.push_back(lt_var);
            }
        }
        else
        {
            cout<<"ERROR: Grammar not according to the READ_ME File norms1\n Aborting...\n";
            exit(1);
        }
        curr_pos+=3;
        c=temp[curr_pos];
        string rt_alpha;
        lab:
        while(c==' ')
        {
            curr_pos++;
            c=temp[curr_pos];
        }
        rt_alpha.clear();
        while(c!=' '&&c!='\0'&&c!='\n'&&c!='\r')
        {
            rt_alpha.push_back(c);
            curr_pos++;
            c=temp[curr_pos];
        }
        if(check_terminal(rt_alpha)||check_variable(rt_alpha))
        {
            grhs[i].push_back(rt_alpha);
            if(rt_alpha.compare("null")!=0 && check_terminal(rt_alpha))
            {
                int flag=0;
                for(int k=0;k<terminal.size();k++)
                {
                    if(terminal[k].compare(rt_alpha)==0)
                    {
                        flag=1;
                        break;
                    }
                }
                if(flag==0)
                {
                    terminal.push_back(rt_alpha);
                }
            }
        }
        else
        {
            cout<<"ERROR: Grammar not according to the READ_ME File norms2\nAborting...\n";
            exit(1);
        }
        while(c==' ')
        {
            curr_pos++;
            c=temp[curr_pos];
        }
        if(c=='\0'||c=='\n'||c=='\r')
            continue;
        else
            goto lab;
    }
    fclose(fp);
}
vector<string> answer;
vector<string> dp;
void find_first(parser p,string alpha,int* locnull)
{
    if(p.check_terminal(alpha)&&alpha.compare("null")!=0)
    {
        vector<string>::iterator it=find(answer.begin(),answer.end(),alpha);
        vector<string>::iterator itt=find(p.terminal.begin(),p.terminal.end(),alpha);
        if(it==answer.end()&&itt!=p.terminal.end())
            answer.push_back(alpha);
        *locnull=0;
        return;
    }
    if(alpha.compare("null")==0)
    {
        answer.push_back(alpha);
        *locnull=1;
        return;
    }
    for(int i=0;i<p.n_prod;i++)
    {
        if(p.glhs[i][0].compare(alpha)==0)
        {
            for(vector<string>::iterator j=p.grhs[i].begin(); j!=p.grhs[i].end(); j++)
            {
                if(p.check_terminal(*j) && (*j).compare("null")!=0)
                {
                    vector<string>::iterator it=find(answer.begin(),answer.end(),*j);
                    vector<string>::iterator itt=find(p.terminal.begin(),p.terminal.end(),*j);
                    if(it==answer.end() && itt!=p.terminal.end())
                        answer.push_back(*j);
                    *locnull=0;
                    break;
                }
                else if(p.check_variable(*j))
                {

                    vector<string>::iterator itt=find(dp.begin(),dp.end(),*j);
                    if(itt==dp.end())
                    {
                        dp.push_back(*j);
                        find_first(p,*j,locnull);

                        vector<string>::iterator ittt=find(dp.begin(),dp.end(),*j);
                        dp.erase(ittt);
                    }
                    else
                    {
                        break;
                    }
                    if(*locnull==0)
                        break;
                    *locnull=0;
                    if(j+1!=p.grhs[i].end())
                    {
                        vector<string>::iterator it=find(answer.begin(),answer.end(),"null");
                        if(it!=answer.end())
                            answer.erase(it);
                    }
                    else
                    {
                        int it=i+1,flag=0;
                        for(    ;it<p.n_prod;it++)
                        {
                            if(p.glhs[it][0].compare(p.glhs[i][0])==0)
                            {
                                flag=1;
                                break;
                            }
                        }
                        if(flag==1)
                        {
                            vector<string>::iterator itt=find(answer.begin(),answer.end(),"null");
                            if(itt!=answer.end())
                                answer.erase(itt);

                        }
                        else
                        {
                            return;
                        }
                    }
                }
                else
                {
                    *locnull=1;
                    answer.push_back("null");
                    break;
                }
            }
        }
    }
    return;
}

void find_first_util(parser p,vector<string> s)
{
    answer.clear();
    int loc_null=0;

    for(vector<string>::iterator i=s.begin();i!=s.end();i++)
    {
        loc_null=0;
        dp.clear();
        dp.push_back(*i);
        find_first(p,*i,&loc_null);
        vector<string>::iterator itt=find(answer.begin(),answer.end(),"null");
        if(itt!=answer.end() && i+1!=s.end())
        {
            answer.erase(itt);
        }
        else
            break;
    }

}

void populate_first(parser* p) 
{
    int j=0;
    for(vector<string>::iterator i=p->variable.begin(); i!=p->variable.end();i++)
    {
        answer.clear();
        int loc_null=0;
        dp.clear();
        dp.push_back(*i);
        find_first(*p,*i,&loc_null);
        for(vector<string>::iterator it=answer.begin();it!=answer.end();it++)
        {
            p->pre_first[j].push_back(*it);
        }
        j++;
    }
    answer.clear();
}

vector<string> foll_sol;
vector<string> foll_pro;
void find_follow(string s,parser p)
{
    //cout<<s<<"\n";
    if(p.start_symbol.compare(s)==0)
    {
        vector<string>::iterator itt=find(foll_sol.begin(),foll_sol.end(),"$");
        if(itt==foll_sol.end())
            foll_sol.push_back("$");
    }
    //cout<<"gotcha ";
    for(int i=0;i<p.n_prod;i++)
    {
        for(vector<string>::iterator it=p.grhs[i].begin();it!=p.grhs[i].end();it++)
        {
            if(s.compare(*it)==0 && it+1 != p.grhs[i].end())
            {
                //cout<<p.glhs[i][0]<<" ";
                vector<string> temp;
                it++;
                while(it!=p.grhs[i].end())
                {
                    temp.push_back(*it);
                    it++;
                }
                answer.clear();
                find_first_util(p,temp);
                int flag=0;
                for(vector<string>::iterator itt=answer.begin();itt!=answer.end();itt++)
                {
                    if((*itt).compare("null")==0)
                    {
                        flag=1;
                    }
                    else
                    {
                        vector<string>::iterator ittt=find(foll_sol.begin(),foll_sol.end(),*itt);
                        if(ittt==foll_sol.end())
                            foll_sol.push_back(*itt);
                    }
                }
                answer.clear();
                if(flag==0)
                {
                    break;
                }
                else
                {
                    vector<string>::iterator ittt=find(foll_pro.begin(),foll_pro.end(),p.glhs[i][0]);
                    if(p.glhs[i][0].compare(s)!=0 )  //to remove direct and indirect left recursion
                    {
                        if(ittt==foll_pro.end())
                        {
                            foll_pro.push_back(p.glhs[i][0]);
                            find_follow(p.glhs[i][0],p);
                            ittt=find(foll_pro.begin(),foll_pro.end(),p.glhs[i][0]);
                            if(ittt!=foll_pro.end())
                                foll_pro.erase(ittt);
                        }
                        else
                        {
                            break;
                        }
                    }

                    break;
                }
            }
            else if(s.compare(*it)==0 && it+1 == p.grhs[i].end())
            {
                //cout<<p.glhs[i][0]<<" ";
                vector<string>::iterator ittt=find(foll_pro.begin(),foll_pro.end(),p.glhs[i][0]);
                if(p.glhs[i][0].compare(s)!=0 )  //to remove direct and indirect left recursion
                    {
                        if(ittt==foll_pro.end())
                        {
                            foll_pro.push_back(p.glhs[i][0]);
                            find_follow(p.glhs[i][0],p);
                            ittt=find(foll_pro.begin(),foll_pro.end(),p.glhs[i][0]);
                            if(ittt!=foll_pro.end())
                                foll_pro.erase(ittt);
                        }
                        else
                        {
                            break;
                        }
                    }
                break;
            }
        }
    }
    return;
}

void populate_follow(parser *p)
{
    for(int i=0; i<p->variable.size(); i++)
    {
        foll_sol.clear();
        foll_pro.clear();
        foll_pro.push_back(p->variable[i]);
        find_follow(p->variable[i],*p);
        for(vector<string>::iterator it=foll_sol.begin(); it!=foll_sol.end(); it++)
        {
            p->pre_follow[i].push_back(*it);
        }
        foll_pro.clear();
        foll_sol.clear();
    }
}

void compute_table(parser *p)
{
    for(int i=0;i<p->n_prod;i++)
    {
        //cout<<i<<"  -> ";
        vector<string> temp;
        temp.clear();
        for(int j=0;j<p->grhs[i].size();j++)
        {
            temp.push_back(p->grhs[i][j]);
        }
        find_first_util(*p,temp);
        int fflag=0,i_ind;
        vector<string>::iterator it=find(answer.begin(),answer.end(),"null");
        if(it!=answer.end())
        {
            fflag=1;
            answer.erase(it);
        }
        it=find(p->variable.begin(),p->variable.end(),p->glhs[i][0]);
        i_ind=it-p->variable.begin();
        for(it=answer.begin();it!=answer.end();it++)
        {
            vector<string>::iterator itt=find(p->terminal.begin(),p->terminal.end(),*it);
            int j_ind=itt-p->terminal.begin();
            p->ptable[i_ind][j_ind].push_back(i);
        }
        if(fflag==1)
        {

            for(int j=0;j<p->pre_follow[i_ind].size();j++)
            {
                vector<string>::iterator itt=find(p->terminal.begin(),p->terminal.end(),p->pre_follow[i_ind][j]);
                if(itt==p->terminal.end())
                {
                    p->ptable[i_ind][p->terminal.size()].push_back(i);
                }
                else
                {
                    int j_ind=itt-p->terminal.begin();
                    p->ptable[i_ind][j_ind].push_back(i);
                }
            }
        }
        answer.clear();
    }
}

void input_parsing_string(parser *p)
{
	FILE *fp;
	fp=fopen("string.txt","r");
	char str[10000];
	if(p->system)
        fscanf(fp," %[^\n]s",str);                  
	else
        fscanf(fp," %[^\r]s",str);
	int curr=0;
	char c=str[curr];
	string temp;
	lab:
	temp.clear();
	while(c==' ')
	{
		curr++;
		c=str[curr];
	}
	while(c!=' '&&c!='\r'&&c!='\0'&&c!='\n')
	{
		temp.push_back(c);
		curr++;
		c=str[curr];
	}
	p->p_str.push_back(temp);
	if(c=='\r' || c=='\0' || c=='\n')
		return;
	goto lab;
}

void parse(parser *p)
{
	stack<string> st;
	st.push("$");
	st.push(p->start_symbol);
	bool fail=false;
	for(vector<string>::iterator i=p->p_str.begin();i!=p->p_str.end();i++)
    {
        vector<string>::iterator temp=find(p->terminal.begin(),p->terminal.end(),*i);
        if(temp==p->terminal.end())
        {
            cout<<"Input String Contains Invalid Token ->> "<<*i<<endl;
            fail=true;
        }
    }
    if(fail)
    {
        cout<<"Hence, Parsing Failed\nTerminating...\n";
        return;
    }
	p->p_str.push_back("$");
	int parsed=0;
	vector<string>::iterator inp_ptr=p->p_str.begin();
	cout<<"Stack\t\t\t\t\t\tINPUT\t\t\t\t\t\tACTION\n";
	while(1)
	{
	    stack<string> t_st;
	    while(!st.empty())
        {
            t_st.push(st.top());
            st.pop();
        }
        while(!t_st.empty())
        {
            st.push(t_st.top());
            cout<<t_st.top()<<" ";
            t_st.pop();
        }
        cout<<"\t\t\t\t   ->> ";
        vector<string>::iterator itt=inp_ptr;
        while(itt!=p->p_str.end())
            cout<<*itt<<" ",itt++;
        cout<<"\t\t\t\t  ->> ";
		if(st.top().compare(*inp_ptr)==0 && st.top().compare("$")==0)
		{
			cout<<"Successfull parsing\n";
			parsed=1;
			break;
		}
		else if(st.top().compare(*inp_ptr)==0)
		{
			st.pop();
			inp_ptr++;
			cout<<"Match\n";
		}
		else if(p->check_variable(st.top())==true)
		{
			vector<string>::iterator var=find(p->variable.begin(),p->variable.end(),st.top());
			int v_ind=var-p->variable.begin();
			var=find(p->terminal.begin(),p->terminal.end(),*inp_ptr);
			int t_ind=var-p->terminal.begin();
			if(p->ptable[v_ind][t_ind].size()!=1)
			{
			    if(p->ptable[v_ind][t_ind].size()==0)
				cout<<"ERROR::\nThe input string is not valid\n";
				else
                cout<<"ERROR::\nThe parsing table is ambiguous\n";
				break;
			}
			int production=p->ptable[v_ind][t_ind][0];
			st.pop();
			cout<<p->glhs[production][0]<<" -> ";
			for(int j=0;j<p->grhs[production].size();j++)     
            {
                cout<<p->grhs[production][j]<<" ";
            }
            cout<<endl;
			for(int j=p->grhs[production].size()-1;j>=0;j--)       
			{
			    if(p->grhs[production][j].compare("null")!=0)
                    st.push(p->grhs[production][j]);
			}
		}
	}
	if(parsed==1)
        cout<<"\n\n\tRESULT: String Successfully parsed\n\n";
    else
        cout<<"\n\n\tRESULT: ERROR\n\n";
}

int main()
{
    freopen("output.txt", "w", stdout);
    parser p;      
    int choice = 0;
    if(choice==1)
        p.system=false;
    p.read_grammar();      
    cout<<"\t\tNo. of productions in grammar: "<<p.n_prod<<endl<<endl;
    cout<<"\t\tStart Symbol->> "<<p.start_symbol<<endl<<endl;
    cout<<"_______________________________________________________________________________________\n";
    cout<<endl<<"\n\t\tGrammar:\n\n";
    for(int i=0;i<p.n_prod;i++)     
    {
        cout<<"\t\t"<<*p.glhs[i].begin()<<" -> ";
        for(vector<string>::iterator j=p.grhs[i].begin(); j!=p.grhs[i].end(); j++)
        {
            cout<<*j<<" ";
        }
        cout<<endl;
    }
    cout<<"_______________________________________________________________________________________\n";
    populate_first(&p);                
    cout<<"\n\tFIRST() sets of non-terminals:\n\n";
    for(int j=0;j<p.variable.size();j++)
    {
        cout<<"\tFIRST of "<<p.variable[j]<<" ->>\t\t";
        for(vector<string>::iterator it=p.pre_first[j].begin(); it!=p.pre_first[j].end(); it++)
        {
            cout<<*it<<"  ";
        }
        cout<<endl;
    }
    cout<<endl;
    cout<<"_______________________________________________________________________________________\n";
    populate_follow(&p);       
    cout<<"\n\tFOLLOW() sets of non-terminals:\n\n";
    for(int i=0;i<p.variable.size();i++)
    {
        cout<<"\tFollow of "<<p.variable[i]<<" ->>\t\t";
        for(vector<string>::iterator it=p.pre_follow[i].begin(); it!=p.pre_follow[i].end(); it++)
            cout<<*it<<" ";
        cout<<endl;
    }
    cout<<"_______________________________________________________________________________________\n";
    compute_table(&p);		
    cout<<"\n\n\t\t\t\tPARSING TABLE\n\n";
    bool mul_entry=false;
    cout<<" \t";
    for(int i=0;i<p.terminal.size();i++)
        cout<<p.terminal[i]<<"  \t";
    cout<<"$\n\n";
    for(int i=0;i<p.variable.size();i++)
    {
        cout<<p.variable[i]<<"\t";
        for(int j=0;j<=p.terminal.size();j++)
        {
            if(p.ptable[i][j].size()==0)
            {
                cout<<" -----  ";
                continue;
            }
            if(p.ptable[i][j].size()>1)
            {
                mul_entry=true;
                cout<<"*";
            }
            int index=p.ptable[i][j][0];
            cout<<p.glhs[index][0]<<"->";
            for(vector<string>::iterator it=p.grhs[index].begin();it!=p.grhs[index].end();it++)
            {
                cout<<*it;
            }
            cout<<"  ";
        }
        cout<<endl<<endl;
    }
    if(mul_entry)
    {
        cout<<"\nNOTE: Parsing table has multiple entries in the same blocks which are marked by asterisk(*)\n\n";
        cout<<"This multiple entries is due to either ambiguous grammar or left recursion or Common prefixes\n\n";
        cout<<"Thus parsing of string is not possible due to ambiguity\n\nTerminating...\n\n";
        exit(1);
    }
    cout<<"_______________________________________________________________________________________\n";

	input_parsing_string(&p);	
	cout<<"\n\tString to be Parsed:\n\n\t";
	for(vector<string>::iterator it=p.p_str.begin();it!=p.p_str.end();it++)
		cout<<*it<<" ";
	cout<<endl;
	cout<<"_______________________________________________________________________________________\n";

	cout<<"\n\tSteps involved in Parsing:\n\n";
	parse(&p);						
	cout<<"_______________________________________________________________________________________\n";
    return 0;
}
