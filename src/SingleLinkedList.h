#ifndef SINGLELINKEDLIST_H
#define SINGLELINKEDLIST_H

#include "node.h"

class SingleLinkedList {
private:
	node* m_head;
	node** m_tail;

	node** find_Position(int _data);
	node** find_Node(int _data);

public:
	SingleLinkedList();
	~SingleLinkedList();

	bool insertAtBegin(int _data);
	bool insertAtEnd(int _data);
	bool insertInOrder(int _data);
	bool remove(int _data);

	bool find(int _data);
	node* findNode(int _data);

	void print() const;
	bool isEmpty() const;
	int size() const;
	void clear();
};

#endif