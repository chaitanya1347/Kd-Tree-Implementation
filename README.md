KDTreeNode Class: This class represents a node in the K-D Tree, storing a point and pointers to left and right child nodes.

KDTree Class:
- buildTree: Recursively builds a balanced K-D Tree by sorting points and selecting the median.
- insert: Allows dynamic insertion of points into the K-D Tree.
- rangeSearch: Performs an efficient range search by pruning branches that do not fall within the given bounds.


Main Function:

Initializes the KDTree with 2 dimensions.
Builds the tree with given points.
Defines the range bounds.
Searches for points within the specified range and prints the number of points found and their coordinates.


This implementation ensures that the K-D Tree is balanced by using the median to split the points, leading to efficient range queries
