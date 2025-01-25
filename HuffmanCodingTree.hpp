#include "MinHeapTree.hpp"

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

void generate_huffman_codes(std::unique_ptr<Node> root, std::string current_code, std::unordered_map<char, std::string>& codes) {
	if (!root) return;
    
    if (root->left == nullptr && root->right == nullptr) {
        codes[root->character] = current_code;
        return;
    }

    generate_huffman_codes(std::move(root->left), current_code + "0", codes);
    generate_huffman_codes(std::move(root->right), current_code + "1", codes);
}

std::string compress(std::string text, std::unordered_map<char, std::string>& codes) {
	std::string encoded;
	
	for (auto const c : text) {
		encoded += codes[c];
	}
	
	return encoded;
}