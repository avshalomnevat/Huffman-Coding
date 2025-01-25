#include <memory>
#include <vector>
#include <unordered_map>
#include <stdexcept>
#include <cmath>
#include <functional>

#define PARENT_INDEX(i) (((i) - 1) / 2)
#define TEMP_NODE_CHAR '~'

struct Node { 
    char character;
    int frequency;
    
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
};

struct Tree {
    unsigned int size;
    unsigned int capacity;
    
    std::vector<std::unique_ptr<Node>> nodes;
};

std::unique_ptr<Node> new_node(char c, int f) {
    auto node = std::make_unique<Node>();
    node->character = c;
    node->frequency = f;
    node->left = nullptr;
    node->right = nullptr;
    return node;
}

std::unique_ptr<Tree> initialize_tree(unsigned int c) {
    auto tree = std::make_unique<Tree>();
    tree->size = 0;
    tree->capacity = c;
    tree->nodes.reserve(c);
    return tree;
}

bool compare_frequencies(const std::unique_ptr<Node>& a, const std::unique_ptr<Node>& b) {
    return a->frequency < b->frequency;
}

void min_heapify(std::unique_ptr<Tree>& t, size_t index) {
    if (index >= t->size) {
        return;
    }

    size_t smallest = index;
    size_t left = 2 * index + 1;
    size_t right = 2 * index + 2;

    if (left < t->size && compare_frequencies(t->nodes[left], t->nodes[smallest])) {
        smallest = left;
    }

    if (right < t->size && compare_frequencies(t->nodes[right], t->nodes[smallest])) {
        smallest = right;
    }

    if (smallest != index) {
        std::swap(t->nodes[index], t->nodes[smallest]);
        min_heapify(t, smallest);
    }
}

std::unique_ptr<Node> get_min(std::unique_ptr<Tree>& t) {
    if (t->size == 0) {
        throw std::runtime_error("Attempting to get min from empty tree");
    }
    
    std::unique_ptr<Node> min_node = std::move(t->nodes[0]);
    
    t->nodes[0] = std::move(t->nodes[t->size - 1]);
    t->nodes.pop_back();
    t->size--;
    
    if (t->size > 0) {
        min_heapify(t, 0);
    }
    
    return min_node;
}

void insert_node(std::unique_ptr<Tree>& t, std::unique_ptr<Node>& n) {
    if (t->size >= t->capacity) {
        throw std::runtime_error("Tree capacity exceeded");
    }
	
    t->nodes.push_back(std::move(n));
    t->size++;
    
    int index = t->size - 1;
    
    while (index > 0) {
        int parent = PARENT_INDEX(index);
        
        if (compare_frequencies(t->nodes[index], t->nodes[parent])) {
            std::swap(t->nodes[index], t->nodes[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

void build_tree(std::unique_ptr<Tree>& t) {
    int index = t->size - 1;
	
	for (int i = PARENT_INDEX(index); i >= 0; i--) { 
		min_heapify(t, i);
	}
}

std::unique_ptr<Tree> create_tree(std::vector<std::pair<char, int>> f, int s) {
	auto tree = initialize_tree(s);
	tree->size = s;
	
	for (int i = 0; i < s; i++) {
        tree->nodes.push_back(new_node(f[i].first, f[i].second));
	}
	
	build_tree(tree);
	
	return tree;
}