#include <iostream>
#include <string>
#include <sstream>

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left, *right;
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
};

void serialize(TreeNode* root, string& s) {
    if (!root) {
        s += "# ";
        return;
    }
    s += to_string(root->val) + " ";
    serialize(root->left, s);
    serialize(root->right, s);
}

TreeNode* deserialize(stringstream& ss) {
    string s;
    ss >> s;
    if (s == "#")
        return nullptr;
    TreeNode* root = new TreeNode(stoi(s));
    root->left = deserialize(ss);
    root->right = deserialize(ss);
    return root;
}

int main() {
    string s = "1 2 # # 3 # # ";
    stringstream ss(s);
    TreeNode* root = deserialize(ss);
  
    string res;
    serialize(root, res);
    cout << res << endl;
}
