#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <vector>                          
#include <random>
#include <algorithm>


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
                min = selection.value;
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
        while(count < num_cities){
            //find closest city
            current_city->visited = true;
            current_path.push_back(*current_city);
            int mindex; //index of next city
            float min = 9999999; //big numbah

            for(int x = 0; x < num_cities; x++){
                //crazy if statement
                if(map[x][current_city->index] < min //gets new min.
                && (node_list[x].visited == false  
                    || count == num_cities)  //checks if visited.
                && map[x][current_city->index] != 0){ //0 represents path to self.
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


        struct test_report test;
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

};
class environment{
    int num_nodes;

    city_map * graph;
    int pop_size;

    vector<organism> population;
    vector<vector<int>> int_pop;
    int generations;

    vector<int> min_path;
    float min_length;

    public:
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
        float value;
        graph->solve_TSP();
        struct organism init;
        init.length = graph->get_nn_length();
        init.path = graph->get_nn_path();
        population.push_back(init);
        for(int i = 1; i < pop_size; i++){
            graph->lazy_path(i);
            vector<int> path = graph->run_nn(i);
            value = graph->get_nn_length();
            int_pop.push_back(path);

            float init_size = 0;
            for(int i = 0; i < num_nodes;i++){
                //printf("%d\n",i);
                init_size += graph->dist(path[i],path[i-1]);
            }
            init_size+=graph->dist(path[0], path[num_nodes-1]);
            if(init_size < min_length) min_length = init_size;
            struct organism new_guy;
            new_guy.length = init_size;
            new_guy.path = path;
            population.push_back(new_guy);
            
        }

    }

    vector<organism> reproduce(struct organism p1, struct organism p2, int num_children){
        //printf("bEgins\n");
        vector<vector<int>> common_pairs; 
        common_pairs.clear();
        vector<organism> ans;
        //printf("Getting pairs!\n");
        common_pairs = get_common_pairs(p1.path, p2.path);
        //printf("got pairs\n");
        vector<int> free_nodes;
        //printf("init\n");
        //hash map for which nodes are held in place.
        bool held[num_nodes] = {false};
        for(int i = 0; i < common_pairs.size(); i++){
            held[common_pairs[i][0]] = true;
            held[common_pairs[i][1]] = true;
        }
        for(int i = 0; i < num_nodes; i++){
            if(!held[i]){
                free_nodes.push_back(i);
            } 
        }
        //printf("Here! %d\n",common_pairs.size());
        //reinsert nodes/pairs at random, keep pairs paired up.
        vector<int> child_path;
        vector<int> temp1;
        vector<vector<int>> temp2;
        float child_size = 0.0;
        int selection = 0;
        for(int c = 0; c < num_children; c++){
            /*
            selection = 1 + rand() % 2;
            if(selection == 1){
                child_path = p1.path;
            } else {
                child_path = p2.path;
            }
            */
            temp1 = free_nodes;
            temp2 = common_pairs;
            for(int n = 0; n < num_nodes - common_pairs.size(); n++){
                //randomly append pairs and free nodes to the solution.
                selection = 1 + rand() % 2;
                //printf("Here!\n");
                if(selection == 1 && temp2.size() == 0) selection = 2;
                if(selection == 2 && temp1.size() == 0) selection = 1;

                //printf("pairs remaining: %d, free rem: %d, selection %d \n", temp2.size(), temp1.size(), selection);

                switch (selection)
                {
                    //append random pair
                    case 1:{
                        if(temp1.size() == 0 && temp2.size() == 0) break;
                        int pair_selection = rand() % temp2.size();
                        child_path.push_back(temp2[pair_selection][0]);
                        child_path.push_back(temp2[pair_selection][1]);
                        //printf("%d %d \n", temp2[pair_selection][0],temp2[pair_selection][1]);
                        //printf("%d\n", pair_selection);
                        temp2.erase(temp2.begin() + pair_selection);
                        //n++;
                        break;
                    }
                    //append free node
                    case 2: {
                        if(temp1.size() == 0 && temp2.size() == 0) break;
                        int node_selection = rand() % temp1.size();
                        child_path.push_back(temp1[node_selection]);
                        //printf("%d \n", temp1[node_selection]);
                        //printf("%d\n", node_selection);
                        temp1.erase(temp1.begin() + node_selection);            
                        break;
                    }
                    }
                }
            //printf("Done path!\n");
           int mutation = rand() % 3;
           if(mutation == 2){
                //printf("MUTATION!\n");
                child_path = mutate(child_path);
            }
            
            for(int i = 0; i < num_nodes;i++){
                //printf("%d\n",i);
                child_size += graph->dist(child_path[i],child_path[i-1]);
            }
            child_size+=graph->dist(child_path[0], child_path[num_nodes-1]);
            //printf("%f \n",child_size);
            

            struct organism child;
            child.length = child_size;
            child.path = child_path;

            ans.push_back(child);
            //printf("Clearing Child\n");
            child_size = 0.0;
            child_path.clear();
        }
        temp1.clear();
        temp2.clear();
        common_pairs.clear();
        free_nodes.clear();
        //
        return ans;

    }
    void run_generation(){
        //printf("Running Generation!\n");
        vector<organism> next_generation;
        vector<organism> batch;
        int num, num1;
        for(int i = 0; i < population.size()/2; i++){
            int num = rand() % pop_size;
            
            if(i + 1 == population.size()){
                num1 = num - 1;
            } else {
                num1 = num + 1;
            }
            
            batch = reproduce(population[num],population[num1],10);
            for(int j = 0; j < batch.size(); j++){
                next_generation.push_back(batch[j]);
            }
        }
        for(int i = 0; i < next_generation.size(); i++){
            population.push_back(next_generation[i]);
        }
    }

    void thin_herd(){
        //printf("Thinning Herd!\n");
        std::sort(population.begin(),population.end());
        population.erase(population.begin() + int(num_nodes*.2),population.end());
        min_length = population[0].length;
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
            printf("%d : ",x);
            for(int y = 0; y < population[x].path.size(); y++){
                printf("%d ", population[x].path[y]);
            }
            printf("\n");
        }
    }

    void run_genetic_algorithm(int generations){
        initialize_population();
        int init_size;
        for(int i = 1; i < generations; i++){
            run_generation();
            thin_herd();
            //printf("Min is: %f\n",min_length);
        }
        for(int i = 0; i < population[0].path.size(); i++){
            printf("%d ", population[0].path[i]);
        }
        printf("%d\n", population[0].path[0]);
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


int main(int argc, char ** argv){
    srand(time(NULL));

    city_map map = city_map(random_map_generator(300,100));

    vector<int> path1 = {0,1,2,3,4,5,6,8,7};
    vector<int> path2 ={1,3,2,0,4,5,7,6,8};

    struct organism p1, p2;

    p1.path = path1;
    p2.path = path2;

    p1.length = 10;
    p2.length = 8;

    environment env(&map);
    env.run_genetic_algorithm(25);

    
    map.solve_TSP();
    map.print_path_and_length();
    map.verify_path_length();
    
   



}