#pragma once
#include "types.h"

struct Node : public Space
{
	Node() : parent(nullptr), fcost(0) {}
	Node(const Space& space) : parent(nullptr), fcost(0)
	{
		memcpy(this, &space, sizeof(Space));
	};

	Node(const Space& space, const std::vector<V3>& vertexList) : parent(nullptr), fcost(0)
	{
		memcpy(this, &space, sizeof(Space));
		for (auto vertex : vertexList)
			fcost += vertex.distance(getCenter());
	}

	Node(const Node& node, const std::vector<V3>& vertexList) : parent(nullptr)
	{
		memcpy(this, &node, sizeof(Node));
		fcost = 0;
		for (auto vertex : vertexList)
			fcost += vertex.distance(getCenter());
	}

	bool operator<(const Node& node) const;

	void setSpace(const Space& s)
	{
		v1 = s.v1, v2 = s.v2, v3 = s.v3, v4 = s.v4;
	}

	mutable float fcost;
	const Node* parent;

private:
};