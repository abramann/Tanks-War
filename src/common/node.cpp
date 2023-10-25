#include "node.h"

bool Node::operator<(const Node& node) const
{
	bool same = isSame(node);
	if (isSame(node))
		return false;
	else if (node.fcost == fcost)
		fcost += 0.1f;

	return (fcost < node.fcost);
}