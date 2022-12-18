#include <iostream>
#include <algorithm>
#include <map>
#include <memory>
#include "InputManager.h"

struct Vertex {
    int start;
    int end;
    int cost;
};

struct Node {
    int id;
    int rate;

    std::vector<Vertex> vertices{};
    bool active = false;
};

template<int max_time>
int find_max_pressure_release(std::map<int, Node> &nodes, Node &node, int time) {
    if (time >= max_time) return 0;

    int max_release = 0;

    for (auto &vertex: node.vertices) {
        Node &target_node = nodes[vertex.end];
        //std::cout << vertex.end - vertex.start << '\n';
        if (!target_node.active && time + vertex.cost + 1 < max_time) {
            target_node.active = true;
            max_release = std::max(max_release,
                                   find_max_pressure_release<max_time>(nodes, target_node, time + vertex.cost + 1));
            target_node.active = false;
        }
    }

    return max_release + (max_time - time) * node.rate;
}

void remove_duplicates(Node &node) {
    // remove duplicate vertices
    std::sort(node.vertices.begin(), node.vertices.end(), [](const Vertex &a, const Vertex &b) {
        return a.end == b.end ? a.cost < b.cost : a.end < b.end;
    });
    // since vertices are sorted first by their end and then their cost
    // we just need the first vertex to a node, since it is the cheapest one
    for (int i = 0; i < node.vertices.size() - 1; ++i) {
        if (node.vertices[i].end == node.vertices[i + 1].end) {
            node.vertices.erase(node.vertices.begin() + i + 1);
            --i;
        }
    }
}

// calculate costs for every node to get to every other node
// THIS IS NO EFFICIENT IMPLEMENTATION, but it does the job
void build_direct_connections(std::map<int, Node> &nodes) {
    // we need at most n-1 steps to get from one note to any other
    // if a path exists
    for (int i = 0; i < nodes.size() - 1; ++i) {
        for (auto &[src_id, src_node]: nodes) {
            // generate new vertices
            std::vector<Vertex> new_vertices;
            for (auto &src_vertex: src_node.vertices) {
                for (auto &target_vertex: nodes[src_vertex.end].vertices) {
                    if (target_vertex.end != src_node.id)
                        new_vertices.push_back(
                                Vertex{src_node.id, target_vertex.end, src_vertex.cost + target_vertex.cost});
                }
            }
            //src_node.vertices.insert(src_node.vertices.end(), new_vertices.begin(), new_vertices.end());
            for (auto &vertex: new_vertices) src_node.vertices.push_back(vertex);
            remove_duplicates(src_node);
        }
    }
}

// eliminate nodes with rate 0
void eliminate_nodes(std::vector<Node> &nodes) {
    while (true) {
        // find node with rate 0
        Node current{-1, -1, {}};
        for (auto iterator = nodes.begin(); iterator != nodes.end(); ++iterator) {
            Node &node = *iterator;
            // find node with rate 0 that isn't the start node
            if (node.rate == 0 && node.id != 0) {
                current = node;
                nodes.erase(iterator);
                break;
            }
        }
        // no node with rate 0 found => we are done eliminating nodes with cost 0
        if (current.id == -1) break;

        for (auto &node: nodes) {
            bool add_new_vertices = false;
            int cost;
            for (auto iterator = node.vertices.begin(); iterator != node.vertices.end(); ++iterator) {
                auto &vertex = *iterator;
                // vertex found that needs to be changed
                if (vertex.end == current.id) {
                    cost = vertex.cost;
                    node.vertices.erase(iterator);
                    add_new_vertices = true;
                    break;
                }
            }

            // add new vertices
            if (add_new_vertices) {
                for (auto &old_vertex: current.vertices) {
                    if (old_vertex.end != node.id)
                        node.vertices.push_back(Vertex{node.id, old_vertex.end, old_vertex.cost + cost});
                }

                remove_duplicates(node);
            }

        }
    }
}

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);

    std::vector<Node> nodes;

    // read data from file
    while (input.next_line()) {
        // skip "Valve "
        input.skip(6);
        int id = (int) (input.get_char() - 'A') * 26 + (int) (input.get_char() - 'A');
        // skip " has flow rate="
        input.skip(15);
        int rate = input.num("; ");
        input.substring("valve");
        input.substring(" ");

        Node node{id, rate};
        while (!input.empty()) {
            int other_id = (int) (input.get_char() - 'A') * 26 + (int) (input.get_char() - 'A');
            node.vertices.push_back(Vertex{id, other_id, 1});
            input.skip(2);
        }
        nodes.push_back(node);
    }

    eliminate_nodes(nodes);

    // convert list of nodes to map
    std::map<int, Node> node_map;
    for (auto &node: nodes) node_map[node.id] = node;

    build_direct_connections(node_map);
    for (auto &[id, node]: node_map)
        for (auto it = node.vertices.begin(); it != node.vertices.end(); ++it)
            if (it->end == it->start && it->end == node.id) {
                node.vertices.erase(it);
                std::cout << "ERASE\n";
                break;
            }

    int release = find_max_pressure_release<30>(node_map, node_map[0], 0);
    std::cout << release << std::endl;
}