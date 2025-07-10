#include "SingleLinkedList.h"
#include <iostream>

SingleLinkedList::SingleLinkedList() : m_head(nullptr) {
	m_tail = &m_head;
}

SingleLinkedList::~SingleLinkedList() {
	clear();
}

node** SingleLinkedList::find_Position(int _data) {
	node** pointer = &m_head;
	while (*pointer && (*pointer)->m_data < _data) {
		pointer = &((*pointer)->m_next);
	}
	return pointer;
}

node** SingleLinkedList::find_Node(int _data) {
	node** pointer = &m_head;
	while (*pointer && (*pointer)->m_data != _data) {
		pointer = &((*pointer)->m_next);
	}
	return pointer;
}

bool SingleLinkedList::insertAtBegin(int _data) {
	node* newNode = new node(_data);
	newNode->m_next = m_head;
	m_head = newNode;
	if (m_tail== &m_head) {
		m_tail = &newNode->m_next;
	}
	return true;
}

bool SingleLinkedList::insertAtEnd(int _data) {
	node* newNode = new node(_data);
	*m_tail = newNode;
	m_tail = &newNode->m_next;
	newNode->m_next = nullptr;
	return true;
}

bool SingleLinkedList::insertInOrder(int _data) {
	node** pointer = find_Position(_data);
	if (*pointer && (*pointer)->m_data == _data) {
		return false;
	}

	node* newNode = new node(_data);
	newNode->m_next = *pointer;
	*pointer = newNode;
	if (newNode->m_next == nullptr) {
		m_tail = &newNode->m_next;
	}
	return true;
}

bool SingleLinkedList::remove(int _data) {
	node** pointer = find_Node(_data);
	if (*pointer == nullptr) {
		return false;
	}

	node* nodeRemove = *pointer;
	*pointer = nodeRemove->m_next;
	if (m_tail == &nodeRemove->m_next) {
		m_tail = pointer;
	}
	delete nodeRemove;
	return true;
}

bool SingleLinkedList::find(int _data) {
	node* current = m_head;
	while (current) {
		if (current->m_data == _data) {
			return true;
		}
		current = current->m_next;
	}
	return false;
}

node* SingleLinkedList::findNode(int _data) {
	node* current = m_head;
	while (current) {
		if (current->m_data == _data) {
			return current;
		}
		current = current->m_next;
	}
	return nullptr;
}

void SingleLinkedList::print() const {
	node* current = m_head;
	if (!current) {
		std::cout << "Empty" << std::endl;
		return;
	}

	while (current) {
		std::cout <<current->m_data << " ";
		current = current->m_next;
	}
	std::cout << std::endl;
}

bool SingleLinkedList::isEmpty() const {
	return m_head == nullptr;
}

int SingleLinkedList::size() const {
	int count = 0;
	node* current = m_head;
	while(current) {
		count++;
		current = current->m_next;
	}
	return count;
}

void SingleLinkedList::clear() {
	node* current = m_head;
	while (current) {
		node* temp = current;
		current = current->m_next;
		delete temp;
	}
	m_head = nullptr;
	m_tail = &m_head;
}
