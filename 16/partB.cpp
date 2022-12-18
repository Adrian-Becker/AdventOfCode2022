#include <iostream>
#include <algorithm>
#include <map>
#include <memory>
#include <chrono>
#include <complex>
#include "InputManager.h"

//#define test_data

#ifdef test_data
#define result_size 7
#else
#define result_size 16
#endif

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

struct Result {
    int pressure;
    std::vector<int> path;
};

template<int max_time>
Result find_max_pressure_release(std::map<int, Node> &nodes, Node &node, int time) {
    if (time >= max_time) return {0, {}};

    int max_release = 0;
    std::vector<int> path;

    for (auto &vertex: node.vertices) {
        Node &target_node = nodes[vertex.end];
        if (!target_node.active && time + vertex.cost + 1 < max_time) {
            target_node.active = true;
            Result result = find_max_pressure_release<max_time>(nodes, target_node, time + vertex.cost + 1);
            if (result.pressure > max_release) {
                max_release = result.pressure;
                path = result.path;
            }
            target_node.active = false;
        }
    }

    path.push_back(node.id);

    return {
            max_release + (max_time - time) * node.rate,
            path
    };
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

void save_permutations(std::vector<Node *> &nodes, std::array<int, 1 << (result_size + 1)> &results, int code,
                       Result &result, int offset = 0) {
    if (offset >= nodes.size()) {
        results[code] = result.pressure;
    } else {
        if (nodes[offset]->active) {
            save_permutations(nodes, results, code + (1 << offset), result, offset + 1);
        } else if (std::find(result.path.begin(), result.path.end(), nodes[offset]->id) != result.path.end())
            save_permutations(nodes, results, code, result, offset + 1);
        else {
            save_permutations(nodes, results, code + (1 << offset), result, offset + 1);
            save_permutations(nodes, results, code, result, offset + 1);
        }
    }
}

int count_iterations = 0;
std::chrono::steady_clock::time_point start_time;

void build_permutations(std::vector<Node *> &nodes, std::map<int, Node> &node_map, int offset,
                        std::array<int, 1 << (result_size + 1)> &results, int code) {
    if (offset >= nodes.size()) {
        // check that result hasn't been computed yet
        if (results[code] >= 0) return;

        Result result = find_max_pressure_release<26>(node_map, node_map[0], 0);
        save_permutations(nodes, results, 0, result, 0);
    } else {
        build_permutations(nodes, node_map, offset + 1, results, code);
        nodes[offset]->active = true;
        build_permutations(nodes, node_map, offset + 1, results, code + (1 << offset));
        nodes[offset]->active = false;

        if (offset == 6) {
            ++count_iterations;
            auto end = std::chrono::steady_clock::now();
            long time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start_time).count();
            time *= 64;
            time /= count_iterations;
            std::cout << "Finished part " << count_iterations << " / 32, " << (time / 1000) << "s remaining!"
                      << std::endl;
        }
    }
}

std::array<int, 1 << (result_size + 1)> build_permutations(std::vector<Node *> &nodes, std::map<int, Node> &node_map) {
    // not optimal since we technically only need every second item of this array
    std::array<int, 1 << (result_size + 1)> results{};
    std::fill_n(results.begin(), 1 << (result_size + 1), -1);

    build_permutations(nodes, node_map, 1, results, 0);

    return results;
}

int find_max(std::array<int, 1 << (result_size + 1)> &results, int code1, int code2, int offset, int nodes) {
    if (offset >= nodes) {
        return results[code1] + results[code2];
    }

    return std::max(
            find_max(results, code1 + (1 << offset), code2, offset + 1, nodes),
            find_max(results, code1, code2 + (1 << offset), offset + 1, nodes)
    );
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

    std::vector<Node *> nodes_ptr;
    for (auto &[id, node]: node_map) nodes_ptr.push_back(&node);
    std::sort(nodes_ptr.begin(), nodes_ptr.end(), [](Node *a, Node *b) { return a->id < b->id; });

    std::cout << "== Building Permutations & calculating their costs ==" << std::endl;
    start_time = std::chrono::steady_clock::now();
    auto results = build_permutations(nodes_ptr, node_map);
    auto end = std::chrono::steady_clock::now();
    std::cout << "Took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start_time).count() << "ms"
              << std::endl;

    std::cout << "== Finding best combination ==" << std::endl;
    start_time = std::chrono::steady_clock::now();
    int result = find_max(results, 0, 0, 1, nodes.size());
    end = std::chrono::steady_clock::now();
    std::cout << "Maximum pressure release: " << result << std::endl;
    std::cout << "Took: " << std::chrono::duration_cast<std::chrono::milliseconds>(end - start_time).count() << "ms"
              << std::endl;
}