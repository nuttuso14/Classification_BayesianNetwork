#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

string outputClass[4] = {"Basic","Normal","Silver","Gold"};
int sizeOfMap = 4;
string missingValue="??";

string trim(const string& str)
{
    size_t first = str.find_first_not_of(' ');
    if (string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

double computeMean(vector<map<int,string>> train,int key,int index){
    int N=0;
    double trainvalue = 0;
    for(map<int,string> t:train)
    {
        if(outputClass[index]==t[2]){
            if(t[key]!=missingValue)
            {
                trainvalue+=stod(t[key]);
                N++;
            }
        }
    }

    if(N==0)
    {
         return -1;
    }
       
    return trainvalue/(double)N;
}

double computeVariance(vector<map<int,string>> train,int key,int index,double mean){
    
    int N=0;
    vector<int> noMissingIndex;
    int x=0;
    for(map<int,string> t:train)
    {
        if(outputClass[index]==t[2]){
            if(t[key]!=missingValue)
            {
                N++;
                noMissingIndex.push_back(x);
            }
        }
        x++;
    }

    double delta = 0;
    if(N<2)
    {
        return -1;
    }
    else
    {
        
        for(int i:noMissingIndex)
        {
            map<int,string> tr = train[i];
            double xi = stod(tr[key]);
            delta+=pow((xi-mean),2);
        }
    }

    double var = delta/((double)(N-1));

    return var;
}

double computeProb(){
    return 0;
}

double computeProb(int type){
    return 0;
}

int FindMaxIndex(double list[],int size){
    double max = list[0];
    int index = 0;
    for(int i=0;i<size;i++)
    {
        if(max<list[i]){
            max = list[i];
            index = i;
        }
    }
    return index;
}

vector<int> getListofKeyname(map<int,string> mp){
    vector<int> key;
    for(map<int,string>::iterator it = mp.begin(); it != mp.end(); ++it) {
        key.push_back(it->first);
        //cout << it->first << "\n";
    }
    return key;
}

int computeOutputClass(vector<map<int,string>> train, map<int,string> test){
 
    vector<int> key = getListofKeyname(test);
    double popcase[sizeOfMap];
    fill_n(popcase,sizeOfMap,1);

    /*for(map<int,string>::iterator it = test.begin(); it != test.end(); ++it) {
        key.push_back(it->first);
        //cout << it->first << "\n";
    }*/

    for(int k:key)
    {
        if(k!=2)
        {
            switch(k)
            {
                case 0:{ // cal by discreate values
                
                    int n_classifier[sizeOfMap]={0};
                    int n_condition[sizeOfMap]={0};
                    for(int i=0;i<sizeOfMap;i++)
                    {
                        for(map<int,string> t:train)
                        {
                            if(t[2]==outputClass[i])
                            {
                                n_classifier[i]++;
                            } 
                        }
                    }

                    for(int i=0;i<sizeOfMap;i++)
                    {
                        if(n_classifier[i]!=0)
                        {
                            for(map<int,string> t:train)
                            {
                                if( (t[k]==test[k]) && (t[2]==outputClass[i]))
                                {
                                    n_condition[i]++;
                                }
                            }
                        }
                        else
                        {
                            popcase[i]=0;
                        }
                        
                    }

                    for(int i=0;i<sizeOfMap;i++)
                    {
                        if(n_classifier[i]!=0)
                        {
                            popcase[i]*=((double)n_condition[i])/((double)n_classifier[i]);
                        }
                        
                    }
                    break;
                }
                default:{ // cal by continuous values
                     
                     for(int i=0;i<sizeOfMap;i++)
                     {
                         double mean = computeMean(train,k,i);
                         //cout << "mean:" << mean<<endl;
                         if(mean!=-1){
                             double var = computeVariance(train,k,i,mean);
                            // cout << "var:" << var<<endl;
                             if(var!=-1){
                                 double power = -1*((pow((stod(test[k]))-mean,2))/(2*var));
                                 double expo = exp(power);
                                 popcase[i]*=expo/sqrt(2*M_PI*var);
                             }
                             else
                             {
                                popcase[i]=0;
                             }
                         }
                         else{
                             popcase[i]=0;
                         }
                     }
                     break; 
                } 
     
            }
        }
    }
    /*int x=0;
    for(double p:popcase){
        cout << x << ":" << p <<endl;
        x++;
    }*/

    return FindMaxIndex(popcase,sizeOfMap);
}

vector<string> list(string str,string delimiter){
    vector<string> lists;
    size_t pos = 0;
    string token;
    str = trim(str);
    while ((pos = str.find(delimiter)) != string::npos) {
        token = str.substr(0, pos);
           // cout << "token:"<<token<<endl;
            lists.push_back(token);
            //cout <<"Token:"<< token << endl;
            str.erase(0, pos + delimiter.length());
    }
    if(!str.empty()){
        lists.push_back(str);
    }
 
    return lists;
}

vector<vector<string>> fileToLineItems(string input_name){
    vector<vector<string>> vs;
    string line;
    ifstream myfile (input_name);
    vector<string> lineitems;

    string banket[2] ={"{","}"};
    if (myfile.is_open())
    {
        while (getline(myfile,line,'\n'))
        {
            size_t found1 = line.find(banket[0]);
               
            if (found1!=string::npos){
                int i1 = static_cast<int>(found1);
                line.erase(line.begin()+i1); 
            }
            size_t found2 = line.find(banket[1]);
            if (found2!=string::npos){
                int i2 = static_cast<int>(found2);
                line.erase(line.begin()+i2); 
            }
            //cout << line << '\n';
            lineitems.push_back(line);
        }
        myfile.close();
    }
    else cout << "Unable to open file"; 

    for(string ss: lineitems){
         vector<string> v = list(ss,",");
         vs.push_back(v);
    }

    return vs;
}

vector<map<int,string>> mapTransaction(string input_name, int type){
    
    if(type==0){
        cout << "Trainning Data ..." <<endl;
    }
    else{
        cout << "Test Data ..." <<endl;
    }

    vector<map<int,string>> vms;
    vector<vector<string>> vs = fileToLineItems(input_name);

    for(vector<string> ss:vs)
    {

        int mem[sizeOfMap]={0};
        map<int,string> mp;
        for(string s:ss)
        {

            vector<string> att = list(s," ");
            int index = stoi(att[0]);
            mp.insert(make_pair(index,att[1]));
            mem[index]++;
        }

        if(type==0)
        {
            for(int i=0;i<sizeOfMap;i++)
            {
                if(mem[i]==0)
                {
                    switch(i) 
                    {
                    case 2:
                        mp.insert(make_pair(i,"Basic"));
                        mem[i]++;
                            break;
                    default:
                        mp.insert(make_pair(i,"??"));
                        mem[i]++;
                            break;
                    }
                }
            }
        }
        vms.push_back(mp);
    }

    return vms;
}

int main(int argc, char *argv[]) {

    cout << "Homework2 : Classification by using Bayesian Network " << endl;
    cout << "Load input File ... " <<endl;


    vector<map<int,string>> vms = mapTransaction("training.txt",0);
    ofstream outfile;
    outfile.open("output.txt"); // overwrite

    

    string content;
    vector<map<int,string>> tests = mapTransaction("test.txt",-1);
    for(map<int,string> test:tests)
    {
        //cout << "{";
        content+="{";
        vector<int> keys = getListofKeyname(test);
        for(int i=0;i<keys.size();i++)
        {
             //cout << keys[i] << ":" << test[keys[i]];
             content+=to_string(keys[i])+ ":"+test[keys[i]];
             if(i!=keys.size()-1)
             {
                //cout <<",";
                content+=",";
             } 
        }
       /* for(int key:keys){
            cout << key << ":" << test[key] <<",";
        }*/
        //cout << "}->";
        content+="}->";
        int chosenIndex = computeOutputClass(vms,test);
        //cout << outputClass[chosenIndex]<<endl;
        content+=outputClass[chosenIndex]+"\n";
    }

    outfile << content; 
    outfile.close();
    cout << "Classification Complete, please see output.txt"<<endl;
  
    return 0;
}
