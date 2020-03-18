#include <iostream>
#include <algorithm>
#include <ctime>
#include <vector>
#include <fstream>
#include <string>
#include <climits>
#include <cfloat>
#include <array>
#include <cmath>
#include <map>
#include <random>
#include <iomanip>

using namespace std;

/*============================Seperate=Line===========================================*/
/*Variables*/

vector<double> sepal_l_sep;
vector<double> sepal_w_sep;
vector<double> petal_l_sep;
vector<double> petal_w_sep;
array<string,3> clazz_names={"Iris-setosa","Iris-versicolor","Iris-virginica"};
map<string,double> sepPointsMap;

struct node 	//Node of Decision Tree									
{
	string splitOn;											
	string label;
	double sepValue;										
	bool isLeaf;																	
	vector<node*> children;									
};

class data_set
{
public:
	float sepal_l, sepal_w, petal_l, petal_w;
	string clazz;
	friend ostream& operator<<(ostream& os, const data_set d)
	{
		os << d.sepal_l << "\t" << d.sepal_w << "\t" << d.petal_l << "\t" << d.petal_w << "\t" << d.clazz;
		return os;
	}
	float& operator[](string index)
	{
		if(index=="sepal_l")
			return sepal_l;
		else if(index=="sepal_w")
			return sepal_w;
		else if(index=="petal_l")
			return petal_l;
		else
			return petal_w;
	}
    const float operator[](string index) const
    {
    	if(index=="sepal_l")
			return sepal_l;
		else if(index=="sepal_w")
			return sepal_w;
		else if(index=="petal_l")
			return petal_l;
		else
			return petal_w;
    }
};

/*============================Seperate=Line===========================================*/
/*Utility Functions*/

bool comp0(const data_set & d1, const data_set & d2){return (d1.sepal_l < d2.sepal_l);}
bool comp1(const data_set & d1, const data_set & d2){return (d1.sepal_w < d2.sepal_w);}
bool comp2(const data_set & d1, const data_set & d2){return (d1.petal_l < d2.petal_l);}
bool comp3(const data_set & d1, const data_set & d2){return (d1.petal_w < d2.petal_w);}

void sort_f(vector<data_set> &table,int index)
{
	switch(index)
	{
		case 0: sort(table.begin(),table.end(),comp0);return;
		case 1: sort(table.begin(),table.end(),comp1);return;
		case 2: sort(table.begin(),table.end(),comp2);return;
		case 3: sort(table.begin(),table.end(),comp3);return;
	}
}

void pushSepPoints(int index,double value)
{
	switch(index)
	{
		case 0: sepal_l_sep.push_back(value);
		case 1: sepal_w_sep.push_back(value);
		case 2: petal_l_sep.push_back(value);
		case 3: petal_w_sep.push_back(value);
	}
}

int myrandom (int i) //random function for shuffle
{
    return rand()%i;
}

vector<data_set> getFile(string filename)
{
	ifstream ifile(filename);
	data_set input;
	vector<data_set> ivector;
	char tmp;
	while(ifile >> input.sepal_l)
	{
		ifile >> tmp >> input.sepal_w >> tmp >> input.petal_l >> tmp >> input.petal_w >> tmp >> input.clazz;
		ivector.push_back(input);
	}
	srand(time(0));
	random_shuffle(ivector.begin(),ivector.end(),myrandom);
	ifile.close();
	return ivector;
}

bool allSameClass(vector<data_set> &table) //check if the table is pure
{
	for(int i=0;i<table.size()-1;i++)
	{
		if(table[i].clazz!=table[i+1].clazz)
			return false;
	}
	return true;
}

string mostCommonClazz(vector<data_set> table)
{
	string mostCommon;
	int maxV=-1;
	map<string,int> m;
	for(int i=0;i<table.size();i++)
	{
		m[table[i].clazz]++;
		if(m[table[i].clazz]>maxV)
			mostCommon = table[i].clazz;
	}
	return mostCommon;
}

void clear_vec()
{
	if(!sepal_l_sep.empty())
		sepal_l_sep.clear();
	if(!sepal_w_sep.empty())
		sepal_w_sep.clear();
	if(!petal_l_sep.empty())
		petal_l_sep.clear();
	if(!petal_w_sep.empty())
		petal_w_sep.clear();
	if(!sepPointsMap.empty())
		sepPointsMap.clear();
}

/*============================Seperate=Line===========================================*/
/*Setter Getter Generator*/

vector<string> setAttributeName()
{
	vector<string> attribute;
	attribute.push_back("sepal_l");
	attribute.push_back("sepal_w");
	attribute.push_back("petal_l");
	attribute.push_back("petal_w");
	return attribute;
}

void setSepPoints(vector<data_set> &table)
{
	clear_vec();
	vector<string> atr = setAttributeName();
	for(int j=0;j<4;j++)
	{
		sort_f(table,j);
		for(int i=1;i<table.size();i++)
		{
			if(table[i].clazz!=table[i-1].clazz)
			{
				pushSepPoints(j,(table[i][atr[j]]+table[i-1][atr[j]])/2);
			}
		}
	}
	
}

vector<double> getSepPoints(string atr) //return seperate points vector
{
	if(atr=="sepal_l")
		return sepal_l_sep;
	else if(atr=="sepal_w")
		return sepal_w_sep;
	else if(atr=="petal_l")
		return petal_l_sep;
	else
		return petal_w_sep;
}

vector<data_set> getTestData(int i,vector<data_set> &input)
{
	vector<data_set> v;
	for(int j=i*input.size()/5;j<(i+1)*input.size()/5;j++)
	{
		v.push_back(input[j]);
	}
	return v;
}

vector<data_set> getTrainData(int i,vector<data_set> &input)
{
	vector<data_set> v;
	for(int j=0;j<5;j++)
	{
		if(j==i)
			continue;
		else
		{
			for(int k=j*input.size()/5;k<(j+1)*input.size()/5;k++)
			{
				v.push_back(input[k]);
			}
		}
	}
	return v;
}

vector<data_set> generateNewTable(vector<data_set> &table,int index,string splitAtr)
{
	vector<data_set> v0,v1;
	for(int i=0;i<table.size();i++)
	{
		if(table[i][splitAtr]>sepPointsMap[splitAtr])
			v1.push_back(table[i]);
		else
			v0.push_back(table[i]);
	}
	if(index==0)
		return v0;
	else
		return v1;
}

/*============================Seperate=Line===========================================*/
/*Calculate Functions*/

double calEntropy(vector<data_set> &table) //calculate Entropy
{
	int size = table.size();
	if(size==0)
		return 0.0;
	map<string,int> c;
	for(int i=0;i<3;i++)
	{
		c[clazz_names[i]]=0;
	}
	for(int i=0;i<size;i++)
	{
		c[table[i].clazz]++;
	}
	double result=0;
	for(int i=0;i<3;i++)
	{
		if(c[clazz_names[i]]==0)
			continue;
		result=result-(c[clazz_names[i]]/(double)size)*log2(c[clazz_names[i]]/(double)size);
	}
	return result;
}

double calRem(vector<data_set> &table, int index, vector<string> &attribute)//calculate Rem
{
	double minRem=DBL_MAX;
	vector<double> sepPoints = getSepPoints(attribute[index]);
	for(int i=0;i<sepPoints.size();i++)
	{
		double sep=sepPoints[i];
		vector<data_set> tempTable0,tempTable1;
		for(int j=0;j<table.size();j++)
		{
			if(table[j][attribute[index]]>sep)
				tempTable1.push_back(table[j]);
			else
				tempTable0.push_back(table[j]);
		}
		if(tempTable0.empty()||tempTable1.empty())
			continue;
		double e0=calEntropy(tempTable0),e1=calEntropy(tempTable1);
		double rem=((tempTable0.size()*e0)+(tempTable1.size()*e1))/table.size();
		if(rem<minRem)
		{
			minRem=rem;
			sepPointsMap[attribute[index]]=sep;
		}

	}
	return minRem;
}

double calInfoGain(vector<data_set> &table, int index, vector<string> &attribute)//calculate InfoGain
{
	return calEntropy(table)-calRem(table,index,attribute);
}

double calAccuracy(map<string,map<string,int> > &m,int size)
{
	double sum=0;
	for(int i=0;i<3;i++)
	{
		sum+=m[clazz_names[i]][clazz_names[i]];
	}
	return sum/size;
}

double calPercision(map<string,map<string,int> > &m,int index)
{
	string clazz=clazz_names[index];
	double sum=0;
	for(int i=0;i<3;i++)
	{
		sum+=m[clazz][clazz_names[i]];
	}
	return m[clazz][clazz]/sum;
}

double calRecall(map<string,map<string,int> > &m,int index)
{
	string clazz=clazz_names[index];
	double sum=0;
	for(int i=0;i<3;i++)
	{
		sum+=m[clazz_names[i]][clazz];
	}
	return m[clazz][clazz]/sum;
}

/*============================Seperate=Line===========================================*/
/*Build Decision Tree*/

string decideSplitAttribute(vector<data_set> &table, vector<string> &attribute)
{
	double infoGainMax = -1.0;
	int max=0;
	for(int i=0;i<attribute.size();i++)
	{
		double infoGain =calInfoGain(table,i,attribute);
		if(infoGain==0)
			continue;
		if(infoGain > infoGainMax)
		{
			infoGainMax = infoGain;
			max = i;
		}
	}
	return attribute[max];
}

node* buildDecisionTree(vector<data_set> &table, node* nodePtr, vector<string> attribute)
{
	if(table.empty())
		return NULL;
	if(allSameClass(table))
	{
		nodePtr->isLeaf = true;
		nodePtr->label = table[0].clazz;
		return nodePtr;
	}
	else
	{
		string splitAtr = decideSplitAttribute(table,attribute);
		nodePtr->splitOn = splitAtr;
		nodePtr->sepValue = sepPointsMap[splitAtr];
		for(int i=0;i<2;i++)
		{
			node* newNode = (node*) new node;
			newNode->label=i;
			newNode->isLeaf=false;
			vector<data_set> newTable = generateNewTable(table,i,splitAtr);
			if(newTable.size()==0)
				continue;
			nodePtr->children.push_back(buildDecisionTree(newTable, newNode, attribute));
		}
	}
	return nodePtr;
}

void printDecisionTree(node* root)//debug
{
	cout << "split on : " << root->splitOn << " ";
	cout << "split value : " << root->sepValue << endl;
	cout << "label : " << root->label << endl;
	cout << "isLeaf : " << root->isLeaf << endl;
	for(int i=0;i<root->children.size();i++)
	{
		cout << "go to children " << i << endl;
		printDecisionTree(root->children[i]);
		cout << endl;
	}
}

/*============================Seperate=Line===========================================*/
/*Predict Funtions*/

string predictClazz(data_set &input,node* root)
{
	if(root->isLeaf)
		return root->label;
	if(root->children.empty())
		return "NULL";
	if(input[root->splitOn]>root->sepValue)
		return predictClazz(input,root->children[1]);
	else
		return predictClazz(input,root->children[0]);
}

void printAPR(map<string,map<string,int> > &matrix,int size)
{
	cout << fixed << setprecision(4);
	cout << calAccuracy(matrix,size) << endl;
	cout << calPercision(matrix,0) << " " << calRecall(matrix,0) << endl;
	cout << calPercision(matrix,1) << " " << calRecall(matrix,1) << endl;
	cout << calPercision(matrix,2) << " " << calRecall(matrix,2) << endl;
}

/*============================Seperate=Line===========================================*/
/*Main Funtion*/

int main()
{
	vector<data_set> ivector=getFile("Iris.data");
	vector<string> atr=setAttributeName();
	map<string,map<string,int> > matrix;
	for(int i=0;i<5;i++)
	{
		vector<data_set> test_data=getTestData(i,ivector),train_data=getTrainData(i,ivector);
		setSepPoints(train_data);
		node* root=new node;
		root=buildDecisionTree(train_data,root,atr);
		for(int j=0;j<test_data.size();j++)
		{
			string p_clazz = predictClazz(test_data[j],root);
			if(p_clazz=="NULL")
				p_clazz=mostCommonClazz(train_data);
			matrix[test_data[j].clazz][p_clazz]++;
		}
	}
	printAPR(matrix,ivector.size());
	return 0;
}

