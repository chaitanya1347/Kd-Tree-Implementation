#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

class KDTreeNode {
public:
    vector<double> point;
    KDTreeNode* left;
    KDTreeNode* right;

    KDTreeNode(vector<double> point) : point(point), left(nullptr), right(nullptr) {}
};

class KDTree {
private:
    int k;
    KDTreeNode* root;

    KDTreeNode* buildTree(vector<vector<double>>& points, int depth) {
        if (points.empty()) return nullptr;

        int axis = depth % k;
        sort(points.begin(), points.end(), [axis](const vector<double>& a, const vector<double>& b) {
            return a[axis] < b[axis];
        });

        int median = points.size() / 2;
        KDTreeNode* node = new KDTreeNode(points[median]);

        vector<vector<double>> leftPoints(points.begin(), points.begin() + median);
        vector<vector<double>> rightPoints(points.begin() + median + 1, points.end());

        node->left = buildTree(leftPoints, depth + 1);
        node->right = buildTree(rightPoints, depth + 1);

        return node;
    }

    KDTreeNode* insert(KDTreeNode* node, vector<double>& point, int depth) {
        if (node == nullptr) return new KDTreeNode(point);

        int axis = depth % k;
        if (point[axis] < node->point[axis]) {
            node->left = insert(node->left, point, depth + 1);
        } else {
            node->right = insert(node->right, point, depth + 1);
        }
        return node;
    }

    void rangeSearch(KDTreeNode* node, const vector<double>& lowerBound, const vector<double>& upperBound, int depth, vector<vector<double>>& result) {
        if (node == nullptr) return;

        bool inside = true;
        for (int i = 0; i < k; ++i) {
            if (node->point[i] < lowerBound[i] || node->point[i] > upperBound[i]) {
                inside = false;
                break;
            }
        }

        if (inside) {
            result.push_back(node->point);
        }

        int axis = depth % k;
        if (lowerBound[axis] <= node->point[axis]) {
            rangeSearch(node->left, lowerBound, upperBound, depth + 1, result);
        }
        if (upperBound[axis] >= node->point[axis]) {
            rangeSearch(node->right, lowerBound, upperBound, depth + 1, result);
        }
    }

    void radiusSearch(KDTreeNode* node, const vector<double>& center, double radius, int depth, vector<vector<double>>& result) {
        if (node == nullptr) return;

        double distance = 0.0;
        for (int i = 0; i < k; ++i) {
            distance += (node->point[i] - center[i]) * (node->point[i] - center[i]);
        }
        distance = sqrt(distance);

        if (distance <= radius) {
            result.push_back(node->point);
        }

        int axis = depth % k;
        if (center[axis] - radius <= node->point[axis]) {
            radiusSearch(node->left, center, radius, depth + 1, result);
        }
        if (center[axis] + radius >= node->point[axis]) {
            radiusSearch(node->right, center, radius, depth + 1, result);
        }
    }

public:
    KDTree(int k) : k(k), root(nullptr) {}

    void build(vector<vector<double>>& points) {
        root = buildTree(points, 0);
    }

    void insert(vector<double>& point) {
        root = insert(root, point, 0);
    }

    vector<vector<double>> rangeSearch(const vector<double>& lowerBound, const vector<double>& upperBound) {
        vector<vector<double>> result;
        rangeSearch(root, lowerBound, upperBound, 0, result);
        return result;
    }

    vector<vector<double>> radiusSearch(const vector<double>& center, double radius) {
        vector<vector<double>> result;
        radiusSearch(root, center, radius, 0, result);
        return result;
    }
};

int main() {
    KDTree tree(2);  // Assuming 2-dimensional space (latitude, longitude)

    vector<vector<double>> points = {
        {40.748817, -73.985428},  // Restaurant 1
        {40.748947, -73.987563},  // Restaurant 2
        {40.749102, -73.987683},  // Restaurant 3
        // Add more restaurant coordinates as needed
    };

    tree.build(points);

    // Define the lower and upper bounds of the range (latitude, longitude)
    vector<double> lowerBound = {40.748000, -73.990000};
    vector<double> upperBound = {40.750000, -73.980000};

    vector<vector<double>> restaurantsInRange = tree.rangeSearch(lowerBound, upperBound);
    cout << "Number of restaurants in range: " << restaurantsInRange.size() << endl;
    for (const auto& point : restaurantsInRange) {
        cout << "(" << point[0] << ", " << point[1] << ")" << endl;
    }

    // Define the center point and radius for the radius search
    vector<double> center = {40.748817, -73.985428};
    double radius = 0.001;  // Radius in degrees

    vector<vector<double>> restaurantsInRadius = tree.radiusSearch(center, radius);
    cout << "Number of restaurants within radius: " << restaurantsInRadius.size() << endl;
    for (const auto& point : restaurantsInRadius) {
        cout << "(" << point[0] << ", " << point[1] << ")" << endl;
    }

    return 0;
}
