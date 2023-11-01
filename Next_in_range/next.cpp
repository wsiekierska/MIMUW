#include "next.h"
#include <cstdlib>
#include <vector>
#include <utility>
#include <algorithm>
#include <deque>
#include <iostream>
#include <climits>
#include <algorithm>

using namespace std;

namespace {
    typedef struct node *tree;
    struct node {
        int val;
        tree left, right;
    };

    deque<tree> trees;
    vector<int> values;
    vector<int> new_x;
    vector<tree> deleting;
}


void create_tree(int a, int b, tree t) {
    t->val = INT_MAX;
    if (a == b) {
        t->left = nullptr;
        t->right = nullptr;
    } else {
        t->left = new node;
        t->right = new node;
        deleting.push_back(t->left);
        deleting.push_back(t->right);
        create_tree(a, (a + b) / 2, t->left);
        create_tree(((a + b) / 2) + 1, b, t->right);
    }
}

void update(int i, tree t, tree previous, int a, int b) {
    deleting.push_back(t);
    if (new_x[i] >= a && new_x[i] <= b) {
        t->val = i;
        if (a != b) {
            t->left = new node;
            t->right = new node;
            update(i, t->left, previous->left, a, (a+b)/2);
            update(i, t->right, previous->right, (a+b)/2 +1, b);
        }
    } else {
        t->val = previous->val;
        t->left = previous->left;
        t->right = previous->right;
    }
}

void init(const vector<int> &x) {
    values = x;
    sort(values.begin(), values.end());
    values.erase(unique(values.begin(), values.end()), values.end());
    for (auto i: x) {
        new_x.push_back(int(lower_bound(values.begin(), values.end(), i) - values.begin()));
    }
    tree biggest = new node;
    create_tree(0, int(values.size()) - 1, biggest);
    deleting.push_back(biggest);
    trees.push_front(biggest);
    tree previous = biggest;
    for (int i = int(x.size() - 1); i >= 0; i--) {
        trees.push_front(new node);
        update(i, trees.front(), previous, 0, int(values.size()) - 1);
        previous = trees.front();
    }
}

int query(int first, int second, tree d, int a, int b) {
    if (a > second || b < first) {
        return INT_MAX;
    }
    if (a >= first && b <= second) {
        return d->val;
    }
    return min(query(first, second, d->left, a, (a+b)/2), query(first, second, d->right ,(a+b)/2 +1, b));
}

int nextInRange(int i, int a, int b) {
    int first = int(lower_bound(values.begin(), values.end(), a) - values.begin());
    int second = int(upper_bound(values.begin(), values.end(), b) - values.begin()) - 1;
    if (query(first, second, trees[i], 0, int(values.size()-1)) != INT_MAX)
        return query(first, second, trees[i], 0, int(values.size())-1);
    return -1;
}

void done() {
    int n = int(deleting.size());
    for (int i = 0; i < n; i++) {
        delete deleting.back();
        deleting.pop_back();
    }
    n = int(values.size());
    for (int i = 0; i < n; i++) {
        values.pop_back();
    }
    n = int(new_x.size());
    for (int i = 0; i < n; i++) {
        new_x.pop_back();
    }
    while (!trees.empty()) {
        trees.pop_front();
    }
}
