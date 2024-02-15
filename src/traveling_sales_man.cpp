#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <vector>                          
#include <random>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <thread>
#include <unordered_set>


using namespace std;
int number = 0;

struct node{
    int index;
    bool visited = false;
    bool first = false;
};

struct point{
    float x;
    float y;
};

struct test_report{
    vector<node> list;
    float value;
};

class city_map{
    int num_cities;
    int start_index;

    float brute_min_path;
    float nn_path_length;

    vector<vector<float>> map;
    vector<node> node_list;
    vector<node> ideal_path;

    vector<int> brute_ideal_path;

    public:

    city_map(vector<vector<float>> cities){
        num_cities = cities[0].size();
        nn_path_length = 0.0;
        brute_min_path = 0.0;
        vector<float> temp_vec;
        int count = 0;
        for(int x = 0; x < num_cities; x++){
            node new_node;
            new_node.index = x;
            node_list.push_back(new_node);
            for(int y = 0; y < num_cities; y++){
                //add to adjacency matrix
                temp_vec.push_back(cities[x][y]);
            }
            //push to map vector
            map.push_back(temp_vec);
            temp_vec.clear();
        }
    }
    vector<int> get_nn_path(){
        vector<int> path;
        for(int i = 0; i < num_cities; i++){
            path.push_back(ideal_path[i].index);
        }
        return path;
    }
    bool verify_path_length(){
        bool valid = true;
        float cur_len = 0.0;
        for(int i = 1; i < num_cities; i++){
            cur_len += map[ideal_path[i].index][ideal_path[i-1].index];
        }
        cur_len += map[ideal_path[num_cities-1].index][ideal_path[0].index];
        if(cur_len != nn_path_length) valid = false;
        printf("   %f actual length\n", cur_len);
        return valid;
    }
    float get_nn_length(){
        return nn_path_length;
    }
    int cities(){
        return num_cities;
    }
    void solve_TSP(){ //does naive method starting on every node.
        float min = 999999999;
        struct test_report canidate;
        struct test_report selection;
        for(int i = 0; i < num_cities; i++){
            canidate = lazy_path(i);
            if(canidate.value < min){
                selection = canidate;
                min = canidate.value;
            }
        }

        ideal_path = selection.list;
        nn_path_length = selection.value;
    }
    vector<int> run_nn(int start){
        test_report run = lazy_path(start);
        vector<int> path;
        nn_path_length = run.value;
        ideal_path = run.list;
        for(int i = 0; i < num_cities; i++){
            path.push_back(run.list[i].index);
        }
        return path;
    }
    struct test_report lazy_path(int start){
        int count = 0;
        float min_path_length = 0.0;
        node * current_city;

        current_city = &node_list[start];

        vector<node> current_path;

        for(int i = 0; i < num_cities; i++){
            node_list[i].visited = false;
        }
        struct test_report test;
        while(count < num_cities){
            //find closest city
            current_city->visited = true;
            current_path.push_back(*current_city);
            int mindex; //index of next city
            float min = 9999999; //big numbah

            for(int x = 0; x < num_cities; x++){
                //crazy if statement
                if(map[x][current_city->index] < min //gets new min.
                && node_list[x].visited == false)  
                    //|| count == num_cities)  //checks if visited.
                //&& map[x][current_city->index] != 0){ //0 represents path to self.
                {
                        mindex = x;
                        min = map[x][current_city->index];
                }
            }
            min_path_length += map[current_city->index][mindex];
            current_city = &node_list[mindex];
            //increment cities visited
            count++;
        }

        min_path_length += map[current_path[0].index][current_city->index];


        //struct test_report test;
        test.list = current_path;
        test.value = min_path_length;

        current_path.clear();

        return test;

    }

    void brute_force(){
        //initialize big numbers
        float length = 99999999.0;
        bool cities[num_cities] = {false};
        brute_min_path = length;
        //start at 0 (1)
        vector<int> end;
        end.push_back(0);
        //call helper function to get the party started
        brute_force_helper(0,1, end, 0);
        //print path and min value
        printf("Brute Min Path: %3.2f\n", brute_min_path);

        for(int i = 0; i < num_cities; i++) printf("%d ", brute_ideal_path[i] + 1);
        printf("1"); //prints start node (always 1).
        printf("\n");
        
    }

    //path legnth, path distance, remaining path, current path
    void brute_force_helper(float length, int path_length, vector<int> current_path, int last_node){
        if(length > nn_path_length || length > brute_min_path) return;
        //check if path to current node is greater than the min path to said node?
        vector<int> temp = current_path;
        bool remaining_cities[num_cities] = {false};
        for(int i = 0; i < path_length; i++){
            remaining_cities[temp[i]] = true;
        }

        if(path_length < num_cities){
            for(int i = 0; i < num_cities; i++){
                if(remaining_cities[i] == false && map[last_node][i] != 0){
                    temp.push_back(i);
                    brute_force_helper(length + map[last_node][i], path_length + 1, temp, i);
                    temp.pop_back();
                }
            }
        } else {
            if(length + map[last_node][0] < brute_min_path){
                brute_min_path = length + map[last_node][0];
                brute_ideal_path = current_path;
            }
        }
    }

    void printMatrix(){
        printf("     ");
        for(int i = 0; i < num_cities; i++){
            printf("%3d  ", i + 1);
        }   
        printf("\n");
        printf("\n");
        for(int x = 0; x < num_cities; x++){
            for(int y = 0; y < num_cities; y++){
                if(y == 0){
                    printf("%3d   ",x + 1);
                }
                printf("%3.2f ", map[x][y]);
            }
            printf("\n");
        }
        printf("\n");
    }

    void print_path_and_length(){
        for(int i = 0; i < num_cities; i++){
            printf("%d ", ideal_path[i].index + 1);
        }
        printf("%d ", ideal_path[0].index + 1);
        printf("\n");
        printf("Path length of: %.2f", nn_path_length);
    }
    
    void get_max_distance(int start, int end, int iter){
        int min = 0;
        int max = 0;
        //choose the median route
        for(int y = 0; y < num_cities; y++){
            for(int x = y; x < num_cities; x++){
                if(map[y][x] > max){
                    max = map[y][x];
                    start = x;
                    end = y;
                }
            }                                           
                                                            
        } 

        printf("%d and %d are %d away!", start, end, max);                                                                                                                          
    } 

    /*
    
        ACCESS FUNCTIONS
    
    */

    //returns nearest neighbor path
    vector<int> pop_path_nn(){
        vector<int> path;
        for(int i = 0; i < ideal_path.size(); i++){
            path.push_back(ideal_path[i].index);
        }
        return path;
    }        

    //returns number of nodes                                                      
    int get_cities(){
        return num_cities;
    }
    //returns distance between nodes
    float dist(int x, int y){
        return map[x][y];
    }

};


//start at node k, find the farthest node from k, make a path there using half the remaining nodes, make a path back with the remaining.
/*
    find max distance between cities, use as 0 and n;
        lg(n)

    for nodes adjactent to n:
        get max
    farthest node = max
        n^2

    find shortest path from start to max,
    find shortest path from max to start.

    traverse both paths:
        at n cities from start, try to swap the distances to see if the path can be optimized.

*/

//maybe a merge for finding the path? 0->n->0  0->n/2->n->3n/2->0 ...
struct organism{
    vector<int> path;
    float length;

    bool operator < (const organism& rhs) const {
        return length < rhs.length;
    }
    organism(vector<int> p, float l){
        path = p;
        length = l;
    }
    organism(){

    }

};
class environment{
    public:
    int num_nodes;

    city_map * graph;
    int pop_size;

    vector<organism> population;
    vector<vector<int>> int_pop;
    int generations;

    vector<int> min_path;
    float min_length;

    
    environment(city_map * map){
        generations = 0;
        graph = map;
        min_length = 99999999.99;
        num_nodes = map->cities();
    }

    void initialize_population(){
        //run nearest neighbor on each point collecting top n% paths 
        if(num_nodes < 10){
            pop_size = num_nodes;
        } else if(num_nodes < 100){
            pop_size = num_nodes / 5;
        } else{
            pop_size = 50;
        }
        struct organism init;
        if(num_nodes <= 1000){
            graph->solve_TSP();
            init.length = graph->get_nn_length();
            init.path = graph->get_nn_path();
            population.push_back(init);
            vector<organism> batch = reproduce(init, pop_size);
            for(int j = 0; j < batch.size(); j++){
                population.push_back(batch[j]);
            }
        } else {
            for(int i = 0; i < pop_size; i++){
                graph->run_nn(i);
                init.length = graph->get_nn_length();
                init.path = graph->pop_path_nn();
                population.push_back(init);
            }
        }
        printf("Population Initialized!\n");
    }
    vector<organism> reproduce(const organism &p1, const organism &p2, int num_children){
        vector<organism> ans;
        vector<int> child_path;
        vector<int> temp2;
        
        for(int c = 0; c < num_children; c++){
            int n1, n2;
            n1 = rand() % p1.path.size();
            n2 = rand() % p1.path.size();

            int start_gene = min(n1,n2);
            int end_gene = max(n1,n2);

            
            //partition out gene from parent 1.
            vector<int> temp1 = {p1.path.begin() + start_gene, p1.path.begin() + end_gene};
            
            //Append non existing nodes back from parent 2 in the order they are in.
            for(int i = 0; i < p2.path.size(); i++){
                    if(find(temp1.begin(), temp1.end(), p2.path[i]) == temp1.end()){
                        //printf("%d ", p2.path[i]);
                        temp2.push_back(p2.path[i]);
                    } 
                
            }
            //printf("\n");
            //combine into new.
            child_path.insert(child_path.end(), temp1.begin(), temp1.end());
            child_path.insert(child_path.end(), temp2.begin(), temp2.end());

            int mut = rand() % 20;

            if(mut == 3) {
                n1 = rand() % child_path.size();
                n2 = rand() % child_path.size();
                int temp;
                temp = child_path[n1];
                child_path[n1] = child_path[n2];
                child_path[n2] = temp;   
            }

            struct organism child;
            child.length = get_length(child_path);
            child.path = child_path;
            ans.push_back(child);

            child_path.clear();
            temp1.clear();
            temp2.clear();
            }
            return ans;
    }
    vector<organism> opt_reproduce(const organism& p1, const organism& p2, int num_children) {
    vector<organism> ans;
    unordered_set<int> temp1_set(p1.path.begin(), p1.path.end());

    for (int c = 0; c < num_children; c++) {
        vector<int> child_path;
        vector<int> temp2;

        // Randomly select a segment from parent 1's path
        int n1 = rand() % p1.path.size();
        int n2 = rand() % p1.path.size();
        int start_gene = min(n1, n2);
        int end_gene = max(n1, n2);

        // Copy the selected segment to child path
        child_path.insert(child_path.end(), p1.path.begin() + start_gene, p1.path.begin() + end_gene);

        // Append non-existing nodes from parent 2 to the child path
        for (int i : p2.path) {
            if (temp1_set.find(i) == temp1_set.end()) {
                temp2.push_back(i);
            }
        }
        child_path.insert(child_path.end(), temp2.begin(), temp2.end());

        // Perform mutation with a probability of 5%
        if (rand() % 20 == 3) {
            int mut_pos1 = rand() % child_path.size();
            int mut_pos2 = rand() % child_path.size();
            std::swap(child_path[mut_pos1], child_path[mut_pos2]);
        }

        ans.push_back(organism(child_path, get_length(child_path)));
    }
    return ans;
}
    vector<organism> reproduce(struct organism p1, int num_children){
        //printf("bEgins\n");
        vector<vector<int>> common_pairs; 
        common_pairs.clear();
        vector<organism> ans;
        //printf("Getting pairs!\n");
        //common_pairs = get_common_pairs(p1.path, p2.path);
        //printf("got pairs\n");
        //vector<int> free_nodes;
        //printf("init\n");
        //hash map for which nodes are held in place.
        //bool held[num_nodes] = {false};
        //for(int i = 0; i < common_pairs.size(); i++){
        //    held[common_pairs[i][0]] = true;
        //    held[common_pairs[i][1]] = true;
        //}
        //for(int i = 0; i < num_nodes; i++){
        //    if(!held[i]){
        //        free_nodes.push_back(i);
        //    } 
        //}
        //printf("Here! %d\n",common_pairs.size());
        //reinsert nodes/pairs at random, keep pairs paired up.
        vector<int> child_path;

        vector<int> temp1;
        vector<int> temp2;
        vector<int> temp3;
        int temp;

        for(int c = 0; c < num_children; c++){
            child_path = p1.path;
            
            int num1 = child_path.size() / 3;
            int num2 = 2 * child_path.size() / 3;

            if(num2 < num1){
                temp = num1;
                num1 = num2;
                num2 = temp;
            }
            
            //partition into 3 groups
            temp1 = {child_path.begin(), child_path.begin() + num1};
            temp2 = {child_path.begin() + num1, child_path.begin() + num2};
            temp3 = {child_path.begin() + num2, child_path.end()};
            //Shuffle each group
            
            auto rng = std::default_random_engine {};
            shuffle(temp1.begin(),temp1.end(), rng);
            shuffle(temp2.begin(), temp2.end(), rng);
            shuffle(temp3.begin(), temp3.end(), rng);


            temp1.insert(temp1.end(),child_path.begin() + num1, child_path.end());

            temp2.insert(temp2.begin(),child_path.begin(), child_path.begin() + num1);
            temp2.insert(temp2.end(),child_path.begin() + num2, child_path.end());

            temp3.insert(temp3.begin(),child_path.begin(), child_path.begin() + num2);

            //append the non shuffled groups
            //temp1 = mutate(temp1);
            //temp2 = mutate(temp2);
            //temp3 = mutate(temp3);

            vector<organism> offspring;
            vector<float> sizes;
            sizes.push_back(get_length(temp1));
            sizes.push_back(get_length(temp2));
            sizes.push_back(get_length(temp3));


            struct organism child1, child2, child3;
            child1.length = sizes[0];
            child1.path = temp1;

            child2.length = sizes[1];
            child2.path = temp2;

            child3.length = sizes[2];
            child3.path = temp3;
        
            ans.push_back(child1);
            ans.push_back(child2);
            ans.push_back(child3);

            child_path.clear();
            temp1.clear();
            temp2.clear();
            temp3.clear();
            //printf("%.2f\n",get_length(p1.path));
            }
            //printf("Clearing Child\n");
            return ans;
    }
    float get_length(vector<int> path){
        float child_size = 0.0;
        for(int j = 1; j < path.size();j++){
                //printf("%d\n",i);
                child_size += graph->dist(path[j],path[j-1]);
        }
        child_size+=graph->dist(path[0], path[path.size()-1]);
        return child_size;
    }

    void run_generation(){
        vector<organism> batch, batch2;
        for(int i = 1; i < pop_size / 2; i++){
            
            batch = reproduce(population[i], population[i-1], 10);

            for(int j = 0; j < batch.size(); j++){
                population.push_back(batch[j]);
            }
        }
    }

    void thin_herd(){
        float min;
        //printf("Thinning Herd!\n");
        std::sort(population.begin(),population.end());
        population.erase(population.begin() + pop_size,population.end());
        min = population[0].length;
        if(min < min_length){
            printf("Improvement!: %.2f -> %.2f\n",min_length, min);
            min_length = min;
        }
    } 
    vector<int> mutate(vector<int> gene){
        //swaps two random places.
        int swap1 = rand() % num_nodes;
        int swap2 = rand() % num_nodes;
        
        int temp;
        temp = gene[swap1];
        gene[swap1] = gene[swap2];
        gene[swap2] = temp;

        return gene;
    }

    vector<vector<int>> get_common_pairs(vector<int> p1, vector<int> p2){
        int num1, num2;
        vector<vector<int>> ans;
        vector<int> common_pair;
        ans.clear();
        common_pair.clear();
        bool used_map[p1.size()] = {false};
        bool match;
        num1 = num2 = 0;
        for(int i = 1; i < p1.size(); i++){
            num1 = p1[i-1];
            num2 = p1[i];
            match = false;
            common_pair.clear();
            for(int j = 1; j < p2.size(); j++){
                if((p2[j] == num2 && p2[j-1] == num1 || p2[j-1] == num2 && p2[j] == num1)
                    && (!used_map[p2[j]] && !used_map[p2[j-1]])){
                    match = true;
                    break;
                }
            }

            if(match){
                common_pair.push_back(num1);
                common_pair.push_back(num2);
                used_map[num1] = true;
                used_map[num2] = true;
                ans.push_back(common_pair);
            }
        }
        /*
        if(p1[0] == p2[0] && p1[num_nodes] == p2[num_nodes]){
            common_pair.push_back(p1[0]);
            common_pair.push_back(p2[num_nodes]);
            printf("Pair %d %d found!\n",p1[0],p2[num_nodes]);
            ans.push_back(common_pair);
        }
        */
        return ans;
        
    }

    void print_population(){
        for(int x = 0; x < population.size(); x++){
            printf("%d : %.2f |",x,population[x].length);
            for(int y = 0; y < population[x].path.size(); y++){
                printf("%d ", population[x].path[y]);
            }
            printf("\n");
        }
    }

    void run_genetic_algorithm(int generations){
        initialize_population();
        thin_herd();

        int init_size;
        for(int i = 1; i < generations; i++){
            run_generation();
            thin_herd();
            //printf("Min is: %f\n",min_length);
        }
        //print_population();
        for(int i = 0; i < population[0].path.size(); i++){
            printf("%d ", population[0].path[i]);
        }
        
        printf("%d\n", population[0].path[0]);
        printf("%d nodes\n", population[0].path.size());
        printf("Genetic Path length of: %f\n", population[0].length);
    }
};

struct point point_generator(int m){
    float x, y;
    struct point n_point;
    x = y = 0;
    x = ((float) rand() / (RAND_MAX)) * m;
    y = ((float) rand() / (RAND_MAX)) * m;
    n_point.x = x;
    n_point.y = y;
    return n_point;
};

vector<vector<float>> generate_map(struct point points[], int num_points){
    vector<vector<float>> ret_map;
    vector<float> new_row;

    float dist = 0;
    float x1,x2,y1,y2;
    x1 = x2 = y1 = y2 = 0;
    for(int y = 0; y < num_points; y++){
        x1 = points[y].x;
        y1 = points[y].y;
        for(int x = 0; x < num_points; x++){
            if(x < y){
                new_row.push_back(ret_map[x][y]);
            } else if(x == y){
                new_row.push_back(0.0);
            } else {
                //d = sqrt((abs(x2 - x1))^2+(abs(y2 - y2))^2)
                x2 = points[x].x;
                y2 = points[x].y;
                dist = sqrt(pow(x2-x1,2) + pow(y2-y1,2));
                new_row.push_back(dist);
            } 
        }
        ret_map.push_back(new_row);
        new_row.clear();
        
    }

    return ret_map;
}

vector<vector<float>> random_map_generator(int size, int max_distance){
    vector<float> new_row;
    vector<vector<float>> ans;

    for(int i = 0; i < size; i++){
        if(i == 0){
            new_row.push_back(0);
        } else {
            new_row.push_back(((float) rand() / (RAND_MAX)) * max_distance + 1);
        }
        
    }

    ans.push_back(new_row);
    new_row.clear();
    
    for(int y = 1; y < size; y++){
        for(int x = 0; x < size; x++){
            if(x < y){
                new_row.push_back(ans[x][y]);
            } else if(x == y){
                new_row.push_back(0);
            } else {
                new_row.push_back(((float) rand() / (RAND_MAX))*max_distance + 1);
            } 
        }
        ans.push_back(new_row);
        new_row.clear();
    }

    return ans;
}

vector<vector<float>> ingest_file(string name){
    fstream file;
    file.open(name);
    string nums;
    char c;
    float numbah = 0.0;
    int num_rows = 0;
    vector<float> row;
    vector<vector<float>> ans;
    while(file >> numbah){
        if(numbah == 0.0){
            row.push_back(0.0);
            ans.push_back(row);
            row.clear();
            num_rows++;
        } else {
            row.push_back(numbah);
        }
    }
    file.close();
    vector<vector<float>> real_ans(num_rows, vector<float>(num_rows,0.0f));
    int n = num_rows;
     for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            real_ans[i][j] = ans[i][j];
            real_ans[j][i] = ans[i][j];
        }
    }
    return real_ans;
}

int main(int argc, char ** argv){
    srand(time(NULL));
    string file_name = "Size1000(1).graph";

    city_map map = city_map(random_map_generator(500, 10000));
    //map.printMatrix();
    environment env(&map);
    env.run_genetic_algorithm(100);
    //map.solve_TSP();
    //env.run_genetic_algorithm(1);
    //env.print_population();
    //map.print_path_and_length();
    //map.verify_path_length();

    //("\nThingy: %.2f \n", env.get_length(env.population[0].path));

}