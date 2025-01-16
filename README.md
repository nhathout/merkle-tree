# merkle-tree

This repository contains an implementation of a 4-ary Merkle Tree for verifying data integrity. The tree uses FNV1a hashing for consistent results across platforms. Each leaf node contains the hash of individual data points, and internal nodes contain hashes of their child nodes. The root node serves as the fingerprint for the entire tree.

Features:
- Constructors:
  - merkleTree(): Creates an empty Merkle Tree.
  - merkleTree(vector<int>& data): Builds a tree from a given vector of integers.
- verify: Checks the integrity of the data by comparing the stored root with the recomputed root.
- overwrite: Updates a data value and recalculates affected node hashes.
- printTree: Visualizes the structure of the Merkle Tree.
- printRoot: Outputs the root hash.
