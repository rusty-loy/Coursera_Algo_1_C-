#pragma once
#include <vector>

// Supports union/connection of two nodes.  Quickly finds if two nodes are connected.
class CQuickUnion
{
private:
	std::vector<int> m_parentIndices;
	std::vector<int> m_weights;

public:
	CQuickUnion(int size);
	// Returns True if two nodes are connected (part of the same set)
	bool IsConnected(int nodeIndex1, int nodeIndex2);
	// Connect two nodes.  All nodes that were previously union are maintained
	void Union(int nodeIndex1, int nodeIndex2);

private:
	int GetRoot(int nodeIndex);
};

// Grid that finds if there is an open path from the top row to the bottom row.
class CPercolation
{
private:
	CQuickUnion m_nodes;
	std::vector<bool> m_openNodes;
	int m_gridSize;
	int m_topIndex;
	int m_bottomIndex;

public:
	CPercolation(int gridSize);
	// Make grid passable at zero indexed row and column
	void Open(int row, int col);
	int GetNumberOpenSites() const;
	// If the grid has a path from the top to the bottom
	bool Percolates();

public:
	static void RunPercolate(int gridSize);

private:
	int GetGridPos(int row, int col) const;
};
