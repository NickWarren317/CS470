#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <vector>                          
#include <random>

using namespace std;


struct node{
    int index;
    bool visited = false;
    bool first = false;
};

struct test_report{
    vector<node> list;
    int value;
};

class city_map{
    int num_cities;
    int path_length;
    int start_index;


    vector<vector<int>> map;
    vector<node> node_list;
    vector<node> ideal_path;


    public:
    city_map(vector<vector<int>> cities){
        num_cities = cities.size();
        path_length = 0;
        vector<int> temp_vec;
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
        int min = 999999999;
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
        path_length = selection.value;
    }

    struct test_report lazy_path(int start){
        int count = 0;
        int min_path_length = 0;
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
            int min = 9999999; //big numbah

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

    void printMatrix(){
        printf("     ");
        for(int i = 0; i < num_cities; i++){
            printf("%3d ", i + 1);
        }   
        printf("\n");
        printf("\n");
        for(int x = 0; x < num_cities; x++){
            for(int y = 0; y < num_cities; y++){
                if(y == 0){
                    printf("%3d  ",x + 1);
                }
                printf("%3d ", map[x][y]);
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
        printf("Path length of: %d", path_length);
    }


};

//start at node k, find the farthest node from k, make a path there using half the remaining nodes, make a path back with the remaining.


vector<vector<int>> random_map_generator(int size){
    vector<int> new_row;
    vector<vector<int>> ans;

    for(int i = 0; i < size; i++){
        if(i == 0){
            new_row.push_back(0);
        } else {
            new_row.push_back(rand() % 500 + 1);
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
                new_row.push_back(rand() % 500 + 1);
            } 
        }
        ans.push_back(new_row);
        new_row.clear();
    }

    return ans;
}


int main(int argc, char **argv){

    srand(time(NULL));
    vector<vector<int>> input = {{0,  3,  9,  5,  14, 32, 13},
                                 {3,  0,  13, 18, 5,  31, 6},
                                 {9,  13, 0,  4,  6,  12, 5},
                                 {5,  18, 4,  0,  9,  15, 2},
                                 {14, 5,  6,  9,  0,  1,  8},
                                 {32, 31, 12, 15, 1,  0,  10},
                                 {13, 6,  5,  2,  8,  10, 0}};

    vector<vector<int>> input2 = { { 0, 10, 15, 20 },
                                   { 10, 0, 35, 25 },
                                   { 15, 35, 0, 30 },
                                   { 20, 25, 30, 0 } };
    city_map map = city_map(random_map_generator(30));
    map.solve_TSP();
    map.print_path_and_length(); 
    return 0;
}