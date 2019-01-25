#include "tree.h"

sTree::~sTree()
{
	this->clear();
}

void sTree::clear()
{
	this->clear(root);
	root = nullptr;
	levels = 0;
}

void sTree::clear(sNode* st)
{
	if (st)
	{
		if (st->right)
			clear(st->right);
		if (st->left)
			clear(st->left);
		delete st;
	}
}

void sTree::add(CEntity* value, float key)
{
	if (!find(key))
	{
		root = add(root, value, key);
	}
	else
		root = add(root, value, key+0.001f);
}

sNode* sTree::add(sNode* st, CEntity* value, float key)
{
	if (!st)
		return new sNode(value, key);
	if (key < st->key)
		st->left = add(st->left, value, key);
	else
		st->right = add(st->right, value, key);
	return balance(st);
}

sNode* sTree::find(float key)
{
	if (root)
	{
		sNode* cur = root;
		while (cur)
		{
			if (cur->key == key)
				if (!cur->deleted)
					return cur;
				else
					return nullptr;
			else if (key > cur->key)
				cur = cur->right;
			else
				cur = cur->left;
		}
		return cur;
	}
	return nullptr;
}

void sTree::del(CEntity* value)
{
	root = del(root, value);
}

sNode* sTree::del(sNode* st, CEntity* value)
{
	if (!st)
		return nullptr;
	if (value < st->value)
		st->left = del(st->left, value);
	else if (value > st->value)
		st->right = del(st->right, value);
	else
	{
		sNode* l = st->left;
		sNode* r = st->right;
		delete st;
		if (!r)
			return l;
		sNode* min = findMin(r);
		min->right = removeMin(r);
		min->left = l;
		return balance(min);
	}
	return balance(st);
}

sNode* sTree::findMin(sNode* st)
{
	return st->left ? findMin(st->left) : st;
}

sNode* sTree::removeMin(sNode* st)
{
	if (st->left == nullptr)
		return st->right;
	st->left = removeMin(st->left);
	return balance(st);
}

void sTree::print()
{
	printST(root);
}

int sTree::height(sNode* st)
{
	return st ? st->height : 0;
}

int sTree::bfactor(sNode* st)
{
	return height(st->right) - height(st->left);
}

void sTree::fixHeight(sNode* st)
{
	int hl = height(st->left);
	int hr = height(st->right);
	st->height = (hl > hr ? hl : hr) + 1;
}

sNode* sTree::rotateRight(sNode* st)
{
	sNode* q = st->left;
	st->left = q->right;
	q->right = st;
	fixHeight(st);
	fixHeight(q);
	return q;
}

sNode* sTree::rotateLeft(sNode* st)
{
	sNode* q = st->right;
	st->right = q->left;
	q->left = st;
	fixHeight(st);
	fixHeight(q);
	return q;
}

sNode* sTree::balance(sNode* st)
{
	fixHeight(st);
	if (bfactor(st) == 2)
	{
		if (bfactor(st->right) < 0)
			st->right = rotateRight(st->right);
		return rotateLeft(st);
	}
	if (bfactor(st) == -2)
	{
		if (bfactor(st->left) > 0)
			st->left = rotateLeft(st->left);
		return rotateRight(st);
	}
	return st;
}

void sTree::printST(sNode* st)
{
	levels = st->height;
	std::cout << "Levels: " << levels << "\n";
	if (st)
	{
		std::queue<sNode*> queue;
		queue.push(st);
		int lvl = 1;
		int i = 1;
		while (lvl <= levels)
		{
			if (queue.front() != nullptr)
			{
				std::cout << queue.front()->value << "  ";
				queue.push(queue.front()->left);
				queue.push(queue.front()->right);
			}
			else
			{
				std::cout << "nullptr  ";
				queue.push(nullptr);
				queue.push(nullptr);
			}
			queue.pop();
			i++;
			if (i > pow(2, lvl - 1))
			{
				lvl++;
				i = 1;
				std::cout << "\n";
			}
		}
	}
}

void sTree::unload(std::vector<CEntity*>* vec)
{
	if (root)
	{
		if (root->right)unload(vec, root->right);
		vec->push_back(root->value);
		if (root->left)unload(vec, root->left);
	}
}

void sTree::unload(std::vector<CEntity*>* vec, sNode* st)
{
	if (st)
	{
		if (st->right)unload(vec, st->right);
		vec->push_back(st->value);
		if (st->left)unload(vec, st->left);
	}
}