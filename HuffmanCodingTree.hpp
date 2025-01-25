#include "MinHeapTree.hpp"
#include <queue>

std::unique_ptr<Node> build_huffman_tree(std::vector<std::pair<char, int>> f, int s) {
    if (s < 1) {
        throw std::runtime_error("No data to build the tree.");
    }

    auto tree = initialize_tree(s);
    
    for (const auto& item : f) {
        auto node = new_node(item.first, item.second);
        insert_node(tree, node);
    }

    while (tree->size > 1) {
        auto left = get_min(tree);
        auto right = get_min(tree);

        if (!left || !right) {
            throw std::runtime_error("Failed to extract nodes for tree construction.");
        }

        auto root = new_node(TEMP_NODE_CHAR, left->frequency + right->frequency); 
        root->left = std::move(left); 
        root->right = std::move(right); 

        insert_node(tree, root);
    }

    return get_min(tree);
}

