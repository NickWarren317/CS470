#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <vector>                          
#include <random>

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

template <class keytype>
class city_map{
    int num_cities;
    int start_index;

    keytype brute_min_path;
    keytype nn_path_length;

    vector<vector<keytype>> map;
    vector<node> node_list;
    vector<node> ideal_path;

    vector<int> brute_ideal_path;

    public:
    city_map(vector<vector<keytype>> cities){
        num_cities = cities[0].size();
        nn_path_length = 0.0;
        brute_min_path = 0.0;
        vector<keytype> temp_vec;
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

    struct test_report lazy_path(int start){
        int count = 0;
        float min_path_length = 0;
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
            min_path_length+=map[current_city->index][mindex];
            current_city = &node_list[mindex];
            //increment cities visited
            count++;
        }

        min_path_length += map[current_city->index][start];


        struct test_report test;
        test.list = current_path;
        test.value = min_path_length;

        current_path.clear();

        return test;

    }

    void brute_force(){
        //initialize big numbers
        keytype length = 99999999;
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
    void brute_force_helper(keytype length, int path_length, vector<int> current_path, int last_node){
        if(length > nn_path_length || length > brute_min_path) return;
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
        } else if(path_length >= num_cities){
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

    void mid_path_search(){
        int start, end;
        int max = 0;
        start = end = num_cities + 5; //both are inpossible
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
    int size = atoi(argv[2]);
    struct point p[size];
    for(int i = 0; i < size; i++){
       p[i] = point_generator(atoi(argv[1]));
    }
    city_map<float> map = city_map(generate_map(p,size));

    map.printMatrix();
    map.solve_TSP();
    map.brute_force();
    map.print_path_and_length(); 
    return 0;
}