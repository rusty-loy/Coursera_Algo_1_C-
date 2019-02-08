#include "pch.h"
#include "Percolation.h"
#include <numeric>
#include <iostream>
#include <random>
#include <ctime>

CQuickUnion::CQuickUnion(int size)
{
	m_parentIndices.resize(size);
	std::iota(m_parentIndices.begin(), m_parentIndices.end(), 0);

	m_weights.resize(size);
	std::fill(m_weights.begin(), m_weights.end(), 0);
}

int CQuickUnion::GetRoot(int nodeIndex)
{
	auto parentIndex = nodeIndex;
	while (parentIndex != m_parentIndices[parentIndex])
	{
		m_parentIndices[parentIndex] = m_parentIndices[m_parentIndices[parentIndex]];
		parentIndex = m_parentIndices[parentIndex];
	}

	return parentIndex;
}

bool CQuickUnion::IsConnected(int nodeIndex1, int nodeIndex2)
{
	return GetRoot(nodeIndex1) == GetRoot(nodeIndex2);
}

void CQuickUnion::Union(int nodeIndex1, int nodeIndex2)
{
	auto root1 = GetRoot(nodeIndex1);
	auto root2 = GetRoot(nodeIndex2);

	if (root1 != root2)
	{
		// Always place the smaller tree under the larger tree to keep the trees balanced
		if (m_weights[root1] < m_weights[root2])
		{
			m_parentIndices[root1] = root2;
			m_weights[root1] += m_weights[root2];
		}
		else
		{
			m_parentIndices[root2] = root1;
			m_weights[root2] += m_weights[root1];
		}
	}
}

CPercolation::CPercolation(int gridSize) : m_nodes((gridSize * gridSize) + 2)
{
	m_gridSize = gridSize;

	m_openNodes.resize(gridSize * gridSize);
	std::fill(m_openNodes.begin(), m_openNodes.end(), false);

	// Special nodes to check if the top is connected to the bottom
	m_topIndex = gridSize * gridSize;
	m_bottomIndex = m_topIndex + 1;
}

int CPercolation::GetGridPos(int row, int col) const
{
	return (row * m_gridSize) + col;
}

void CPercolation::Open(int row, int col)
{
	auto pos = GetGridPos(row, col);
	if (!m_openNodes[pos])
	{
		m_openNodes[pos] = true;
		for (auto neighborPos : { pos + 1, pos - 1,
								  pos + m_gridSize, pos - m_gridSize  })
		{
			if ((neighborPos >= 0) && (neighborPos < m_topIndex))
			{
				if (m_openNodes[neighborPos])
					m_nodes.Union(pos, neighborPos);
			}
		}

		// Connect the top and bottom rows to their respective special node
		if (row == 0)
			m_nodes.Union(pos, m_topIndex);
		if (row == (m_gridSize - 1))
			m_nodes.Union(pos, m_bottomIndex);
	}
}

int CPercolation::GetNumberOpenSites() const
{
	return std::count(m_openNodes.begin(), m_openNodes.end(), true);
}

bool CPercolation::Percolates()
{
	return m_nodes.IsConnected(m_topIndex, m_bottomIndex);
}

void CPercolation::RunPercolate(int gridSize)
{
	auto start = std::clock();

	auto grid = CPercolation(gridSize);
	std::uniform_int_distribution<int> rand_gen(0, gridSize - 1);
	std::minstd_rand R;

	while (!grid.Percolates())
		grid.Open(rand_gen(R), rand_gen(R));

	std::cout << grid.GetNumberOpenSites() << "\n";
	std::cout << grid.GetNumberOpenSites() / float(gridSize * gridSize) << "\n";

	std::cout << (std::clock() - start) / (double)CLOCKS_PER_SEC << "\n";
}