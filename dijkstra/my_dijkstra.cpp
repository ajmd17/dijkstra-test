#include <unordered_map>
#include <vector>
#include <limits>
#include <algorithm>
#include <iostream>
#include <set>

using namespace std;

struct AdjacentNode;

struct Node {
    char id;
    int dist;
    Node *parent;
    AdjacentNode *adj[16];
};

struct AdjacentNode {
    Node *node;
    int edge_length;
};

class Graph
{
    unordered_map<char, const unordered_map<char, int>> vertices;

public:
    void add_vertex(char name, const unordered_map<char, int> &edges)
    {
        vertices.insert(unordered_map<char, const unordered_map<char, int>>::value_type(name, edges));
    }

    vector<char> shortest_path(char start, char finish)
    {
        
        vector<Node*> nodes;
        vector<char> path;
        auto comparator = [&](Node *a, Node *b) { return a->dist > b->dist; };

        unordered_map<decltype(Node::id), Node *> ptr_map;

        for (auto &it : vertices) {
            int dist = 0;

            if (it.first != start) {
                dist = std::numeric_limits<int>::max();
            }

            Node *node_ptr = new Node{ it.first, dist, nullptr, { nullptr } };
            ptr_map[it.first] = node_ptr;

            nodes.push_back(node_ptr);
            std::push_heap(nodes.begin(), nodes.end(), comparator);
        }

        for (auto &it : vertices) {
            Node *node_ptr = ptr_map[it.first];

            size_t index = 0;

            for (auto &adj : it.second) {
                node_ptr->adj[index++] = new AdjacentNode{ ptr_map[adj.first], adj.second };
            }
        }

        while (!nodes.empty()) {
            std::pop_heap(nodes.begin(), nodes.end(), comparator);

            Node *current = nodes.back();
            nodes.pop_back();

            if (current->id == finish) {
                Node *parent = current;

                while (parent != nullptr) {
                    path.push_back(parent->id);
                    parent = parent->parent;
                }

                break;
            }
            
            size_t index = 0;

            while (index < std::size(current->adj) && current->adj[index] != nullptr) {
                auto new_distance = current->dist + current->adj[index]->edge_length;

                if (new_distance < current->adj[index]->node->dist) {
                    current->adj[index]->node->dist = new_distance;

                    current->adj[index]->node->parent = current;
                    std::make_heap(nodes.begin(), nodes.end(), comparator);
                }

                index++;
            }
        }

        std::reverse(path.begin(), path.end());

        for (auto &it : ptr_map) {
            auto *node = it.second;

            AdjacentNode *adj = node->adj[0];

            for (size_t j = 0; j < std::size(node->adj) && adj != nullptr; adj = node->adj[++j]) {
                delete adj;
            }

            delete node;
        }

        return path;
    }
};

int main()
{
    Graph g;
    g.add_vertex('A', { {'B', 7}, {'C', 8} });
    g.add_vertex('B', { {'A', 7}, {'F', 2} });
    g.add_vertex('C', { {'A', 8}, {'F', 6}, {'G', 4} });
    g.add_vertex('D', { {'F', 8} });
    g.add_vertex('E', { {'H', 1} });
    g.add_vertex('F', { {'B', 2}, {'C', 6}, {'D', 8}, {'G', 9}, {'H', 3} });
    g.add_vertex('G', { {'C', 4}, {'F', 9} });
    g.add_vertex('H', { {'E', 1}, {'F', 3} });

    for (char vertex : g.shortest_path('A', 'H'))
    {
        cout << vertex << endl;
    }

    return 0;
}