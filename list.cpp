#include "list.h"

List::~List()
{
	Node* cur = first;
	while (size)
	{
		cur = first->next;
		delete first->shp;
		delete first;
		first = cur;
		size--;
	}
}

void List::push_back(std::string name)
{
	Node* cur = last;
	if (cur)
	{
		cur = last->next;
		cur = new Node(name);
		cur->prev = last;
		last->next = cur;
		last = cur;
	}
	else
	{
		first = new Node(name);
		last = first;
		current = first;
	}
	size++;
}

void List::push_front(std::string name)
{
	Node* cur = first;
	if (cur)
	{
		cur = first->prev;
		cur = new Node(name);
		cur->next = first;
		first->prev = cur;
		first = cur;
	}
	else
	{
		first = new Node(name);
		last = first;
		current = first;
	}
	size++;
}

Node* List::pop_back()
{
	Node* p = new Node(last->name);
	del(last);
	return p;
}

Node* List::pop_front()
{
	Node* p = new Node(first->name);
	del(first);
	return p;
}

bool List::del(Node* ptr)
{
	if (ptr)
	{
		if (ptr->prev)	ptr->prev->next = ptr->next;
		if (ptr->next)	ptr->next->prev = ptr->prev;
		if (ptr == first)
			first = ptr->next;
		if (ptr == last)
			last = ptr->prev;
		delete ptr->shp;
		delete ptr;
		size--;
		return true;
	}
	else
		return false;
}

CShaderProgram* List::operator[](int i)
{
	int j = 0;
	Node* cur = nullptr;
	if (first)
	{
		cur = first;
		while (i != j && cur)
			cur = cur->next;
	}
	if (cur)
		return cur->shp;
	else
		return nullptr;
}

bool List::find(std::string name)
{
	Node* cur = first;
	Node* tmp = nullptr;
	while (cur)
	{
		if (cur->name == name)
		{
			current = cur;
			return true;
		}
		for (int i = 0; i < cur->name.length(); ++i)
		{
			if (cur->name[i] == name[0])
			{
				bool b = true;
				for (int j = 0; j < name.length() && i + j < cur->name.length(); ++j)
					if (name[j] != cur->name[i + j])
					{
						b = false;
						break;
					}
				if (b)
					tmp = cur;
			}

		}
		cur = cur->next;
	}
	if (tmp)
	{
		current = tmp;
		return true;
	}
	return false;
}