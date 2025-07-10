#ifndef NODE_H
#define NODE_H

class node {
public:
	int m_data;
	node* m_next;

	node(int data);
	~node();
	
};

#endif