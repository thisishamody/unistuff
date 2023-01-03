#include <algorithm>
#include <iostream>
#include <set>
#include <vector>
#include <chrono>

using namespace std;

// function to generate all possible combinations
vector<string> generator(int n) {
    // we take the input n
    // we generate all possible combinations
    vector<string> strings;
    for (int i = 0; i < (1 << n); i++) {
        string s;
        for (int j = n - 1; j >= 0; j--) {
            s += (i >> j) & 1 ? '1' : '0';
        }
        // .push_back adds the 0/1 to the end of the string/vector etc.
        strings.push_back(s);
    }
    // print the binary strings, for understanding and troubleshooting purpose.
    /* for (const string& s : strings) {
        cout << s << endl;
    }*/
    return strings;
}

// here we compute the hamming distance between the two vectors
int hamming_distance(const string& a, const string& b) {
    int distance = 0;
    for (int i = 0; i < a.size(); i++) {
        if (a[i] != b[i]) {
            distance++;
        }
    }
    return distance;
}

// this function builds the hamming graph, it uses sets to store the vertices, it iterates through the vertices and
// calls hamming_distance function to compute the distance
vector<set<int>> graph_maker(const vector<string>& vertices, int d) {
    int n = vertices.size();
    vector<set<int>> graph(n);
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (hamming_distance(vertices[i], vertices[j]) >= d) {
                // here we insert the index of the vertices that have a distance >=d toward eachother
                // [0] 1, 2, 3
                // [1], 0, 3
                // etc.
                graph[i].insert(j);
                graph[j].insert(i);
            }
        }
    }
    return graph;
}

// Here we try to find the maximal clique by interating through the graph
//
int maximum_clique(const vector<set<int>>& graph){
    int max_size = 0;
    //This loop iterates over all the vertices in the graph. The graph size is 2^n. so the iterations will be
    // 2^n, checking the connections.
    cout << "point: " << graph.size() << endl;
    for (int i = 0; i < graph.size(); i++) {
        // here we initialize a another set to store the connected vertices
        set<int> clique{i};
        // we iterate
        for (int j: graph[i]) {
            bool can_add = true;
            for (int k: clique) {
                if (!graph[j].count(k)) {
                    can_add = false;
                    break;
                }
            }
            if (can_add) {
                clique.insert(j);
            }
        }
        // std::max is defined in the header file <algorithm> and is used to find out
        // the largest of the number passed to it. It returns the first of them
        max_size = max(max_size, (int) clique.size());
        cout << clique.size() << endl;
    }
    return max_size;
}


// Our main function
// what we do is, first generate the combinations, then make the hamming graph, then calculate the maximal.
// we also have a timer to check our run time.
int main() {
    // getting the input
    int N, d;
    cin >> N >> d;
    cout << "Starting..." << endl;
    // starting the timer
    std::chrono::time_point<std::chrono::steady_clock> start = std::chrono::steady_clock::now();
    // we generate the vertices
    vector<string> vertices = generator(N);

    // build the Hamming graph
    vector<set<int>> graph = graph_maker(vertices, d);
    // we print the graph, for understanding and troubleshooting purpose.
    for (int i = 0; i < graph.size(); i++) {
            for (auto x : graph[i])
                cout << x << " ";
            cout << endl;
        }

    // compute the size of the maximum clique in the graph
    int size = maximum_clique(graph);
    cout << "Size of maximum clique: " << size << endl;

    // the rest of the timer code. It outputs in milliseconds
    std::chrono::time_point<std::chrono::steady_clock> end = std::chrono::steady_clock::now();
    std::chrono::duration<double, std::milli> fp_ms = end - start;
    std::chrono::duration<unsigned long long, std::milli> int_ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "The iterative function took(milliseconds): " << int_ms.count() << "\n";

    return 0;
}