#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <ctime>
#include <random>

using namespace std;

class Knapsack{
private:
    string filename;
    vector<double> values;
    vector<double> weights;
    double W;
    int N;
    
    struct Node{
        int level;
        double curr;
        double weight;
        double bound;
    };
public:
    
    Knapsack(const string &filename);
    double bound(const Node &node);
    void BranchAndBound();
    
    double price(const vector<int>& chromosom);
    vector<int> RAndomChromosom();
    vector<int> Crossbreeding(const vector<int> child1, const vector<int> child2);
    void mutate(vector<int>& chromosom);
    void GeneticAlgotitm();
};

