#include <iostream>
#include <array>
#include <climits>
#include "InputManager.h"

struct Entry {
    std::string name;
    long size;
    std::vector<Entry> sub_entries;
    Entry *parent;
};

long find_smallest_fitting_directory(Entry *source, long min_size) {
    if (source->size >= min_size && !(source -> sub_entries).empty()) {
        long size = source -> size;
        for (auto &child : source -> sub_entries) {
            size = std::min(size, find_smallest_fitting_directory(&child, min_size));
        }
        return size;
    } else {
        return LLONG_MAX;
    }
}

int main(int argc, char *argv[]) {
    InputManager input(argc, argv);
    input.next_line();

    Entry source{"", 0, {}, nullptr};
    Entry *current = &source;
    bool next_line = true;
    while (next_line) {
        if (input.starts_with("$ cd ")) {
            // skip "$ cd "
            input.skip(5);
            std::string name = input.line();

            if (name == "/") {
                // return to source directory
                current = &source;
            } else if (name == "..") {
                // move to parent directory
                current = current -> parent;
            } else {
                // check if current entry contains the folder
                // and set the current entry to it, if it exists
                bool found = false;
                for (Entry &entry: current->sub_entries) {
                    if (entry.name == name) {
                        current = &entry;
                        found = true;
                        break;
                    }
                }

                // create the folder if it doesn't exist
                if (!found) {
                    (current->sub_entries).push_back(
                            {
                                    name,
                                    0,
                                    {},
                                    current
                            }
                    );
                    current = &(current->sub_entries).back();
                }
            }

            next_line = input.next_line();
        } else if (input.starts_with("$ ls")) {
            // add information of all added files
            while ((next_line = input.next_line()) && !input.starts_with("$ ")) {
                // skip directories
                if (!input.starts_with("dir")) {
                    long size = (long) input.num(" ");
                    std::string name = input.line();

                    // check that file hasn't already been indexed
                    bool found = false;
                    for (auto &entry: current->sub_entries)
                        if (entry.name == name) {
                            found = true;
                            break;
                        }
                    if (!found) {
                        // add entry
                        (current->sub_entries).push_back(
                                {
                                        name,
                                        size,
                                        {},
                                        current
                                }
                        );
                        // add folder size to all parents folders
                        Entry *parent = current;
                        while (parent != nullptr) {
                            parent->size += size;
                            parent = parent->parent;
                        }
                    }
                }
            }
        }
    }

    long update_size = 30000000;
    long filesystem_size = 70000000;
    long free_space = filesystem_size - source.size;
    long needed_space = update_size - free_space;

    std::cout << find_smallest_fitting_directory(&source, needed_space) << '\n';
}