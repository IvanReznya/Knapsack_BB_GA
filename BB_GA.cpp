#include "BB_GA.hpp"

using namespace std;


    
Knapsack::Knapsack(const string &filename){
    this -> filename = filename;
    ifstream file(filename);
    if(!file.is_open()){
        cout << "Cannot open file: " + filename << endl;
    }
    file >> this -> N >> this -> W;
    values.resize(N);
    weights.resize(N);
    for (int i = 0; i < N; i++){
        file >> values[i] >> weights[i];
    }
    
    file.close();
    
}
    
double Knapsack::bound(const Node &node){
    if(node.weight >= W){
        return 0;
    }
    
    double curr_bound = node.curr;
    int i = node.level + 1;
    double totalweight = node.weight;
    
    while (i < N && totalweight + weights[i] <= W) {
        totalweight += weights[i];
        curr_bound += values[i];
        i++;
    }
    if(i < N){
        curr_bound += (W - totalweight) * (values[i] / weights[i]);
    }
    return curr_bound;
}


void Knapsack::BranchAndBound(){
    vector<int> order;
    order.resize(N);
    for (int i = 0; i < N; i++){
        order[i] = i;
    }
    
    sort(order.begin(), order.end(), [&](int a, int b){return values[a]/weights[a] > values[b]/weights[b];});
    
    vector<double> v;
    vector<double> w;
    v.resize(N);
    w.resize(N);
    
    for(int i = 0; i < N; i++){
        v[i] = values[order[i]];
        w[i] = weights[order[i]];
    }
    values = v;
    weights = w;
    
    queue<Node> Queue;
    
    Node curr_node, next_node;
    
    curr_node.level = -1;
    curr_node.curr = 0;
    curr_node.weight = 0;
    
    double currvalues = 0;
    double currweight = 0;
    
    Queue.push(curr_node);
    
    while (!Queue.empty()) {
        curr_node = Queue.front();
        Queue.pop();
        if(curr_node.level == N-1){
            continue;
        }
        next_node.level = curr_node.level + 1;
        next_node.weight = curr_node.weight + weights[next_node.level];
        next_node.curr = curr_node.curr + values[next_node.level];
        
        if(next_node.weight <= W && next_node.curr > currvalues){
            currvalues = next_node.curr;
            currweight = next_node.weight;
        }
        next_node.bound = bound(next_node);
        if(next_node.bound > currvalues){
            Queue.push(next_node);
        }
        next_node.weight = curr_node.weight;
        next_node.curr = curr_node.curr;
        next_node.bound = bound(next_node);
        if(next_node.bound > currvalues){
            Queue.push(next_node);
        }
    }
    
    cout << "Решение задачи о рюкзаке методом ветвей и границ (файл: " << filename << ")" << endl;
    cout << "Общая ценность: " << currvalues << endl;
    cout << "Общий вес: " << currweight << endl;
    
    
}

double Knapsack::price(const vector<int>& chromosom){
    double totalvalue = 0;
    double totalweight = 0;
    for (int  i = 0; i < N; i++){
        if(chromosom[i]){
            totalvalue += values[i];
            totalweight += weights[i];
        }
    }
    if(totalweight > W){
        return 0;
    }
    return totalvalue;
}

vector<int> Knapsack::RAndomChromosom(){
    vector<int> chromosom;
    chromosom.resize(N);
    for (int i = 0; i < N; i++){
        chromosom[i] = rand() % 2;
    }
    return chromosom;
}

vector<int> Knapsack::Crossbreeding(const vector<int> child1, const vector<int> child2){
    vector<int> child3;
    child3.resize(N);
    int p1 = rand() % N;
    int p2 = rand() % N;
    if(p1 > p2){
        swap(p1, p2);
    }
    for (int i = 0; i < N; i++){
        if(i < p1){
            child3[i] = child1[i];
        }
        else if(i < p2){
            child3[i] = child2[i];
        }
        else{
            child3[i] = child1[i];
        }
    }
    return child3;
}

void Knapsack::mutate(vector<int>& chromosom){
    int position = rand() % N;
    chromosom[position] ^= 1;
}

void Knapsack::GeneticAlgotitm(){
    srand((unsigned int)time(0));

    int populations = 100;
    int iter = 100;

    vector<vector<int>> population;

    for(int i = 0; i < populations; i++){
        population.push_back(RAndomChromosom());
    }

    double bestvalue = 0;
    double bestweight = 0;

    for(int n = 0; n < iter; n++){

        sort(population.begin(), population.end(), [&](const vector<int>& a, const vector<int>& b){ return price(a) > price(b);});

        double val = price(population[0]);
        if(val > bestvalue){
            bestvalue = val;
            double w = 0;
            for(int i = 0; i < N; i++){
                if(population[0][i]){
                    w += weights[i];
                }
            }
            bestweight = w;
        }

        vector<vector<int>> newPopulation;

        for(int i = 0; i < 10; i++){
            newPopulation.push_back(population[i]);
        }
        while(newPopulation.size() < populations){

            vector<int> p1 = population[rand() % population.size()];
            vector<int> p2 = population[rand() % population.size()];

            vector<int> child = Crossbreeding(p1, p2);

            mutate(child);

            newPopulation.push_back(child);
        }

        population = newPopulation;
    }

    cout << "Решение задачи о рюкзаке генетическим алгоритмом (файл: " << filename << ")" << endl;
    cout << "Общая ценность: " << bestvalue << endl;
    cout << "Общий вес: " << bestweight << endl;
}



