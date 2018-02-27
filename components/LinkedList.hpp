#pragma once

#include <cstdlib>
#include <cstdint>

class LinkedList
{
public:
	LinkedList()
	{
		head = NULL;
		tail = NULL;
	}
private:
	node *head;
	node *tail;
};

struct node
{
	int32_t data;
	node *next;
};

void LinkedList:CreateNode(int32_t value)
// Preconditions: Value passed to member functionmust be of type 
//                int32_t, and called on existing LinkedList object.
//
// Postconditions: A new node is created and added to the list as
//                 the head if list is empty, otherwise added to tail
//
{
	node *temp = new node;
	temp->data = value;
	temp->next = NULL;

	// Linked list is empty
	if (head == NULL)
	{
		head = temp;
		tail = temp;
		temp = NULL;
	}
	else
	{
		tail->next = temp;
		tail = temp;
	}

}

void LinkedList:InsertAtHead(int32_t value)
// Preconditions: Value passed to member function must be of type 
//				  int32_t, and called on existing LinkedList object.
//
// Postconditions: A new node is created and added to the list as 
//				   the head of the linked list
//
{
	node *temp = new node; 
	temp->data = value;
	temp->next = NULL;

	// Linked list is empty
	if (head == NULL)
	{
		head = temp;
		tail = temp;
		temp = NULL;
	}
	// Insert temp before head
	else
		temp->next = head;
}

void LinkedList:InsertAtTail(int32_t value)
// Preconditions: Value passed to member function must be of type 
//				  int32_t, and called on existing LinkedList object.
//
// Postconditions: A new node is created and added to the list as 
//				   the tail of the linked list
//
{
	node *temp = new node;
	temp->data = value;
	temp->next = NULL;

	// Linked list is empty
	if (tail == NULL)
	{
		head = temp;
		tail = temp;
		temp = NULL;
	}
	// Insert temp as tail's next value
	else
		tail->next = temp;
}

void LinkedList:InsertAtIndex(int32_t value, int32_t index)
// Preconditions: Value and index must be of type int32_t and index
//				  must be within the range of the existing linked list
//
// Postconditions: A new node is created and added to the list at
//				   the head if empty, otherwise at the specified index
//				   given it is no more than 1 index larger the list length
// 
{
	node *current = new node;
	node *previous = new node;
	node *temp = new node;
	cur = head;
	for (int i = 1; i<index; i++)
	{
		previous = current;
		current = current->next;
	}
	temp->data = value;
	previous->next = temp;
	temp->next = current;
}

void LinkedList : DeleteFirst()
// Preconditions: Member function called on existing and filled LinkedList object.
//
// Postconditions: Head node deleted, and new head set
//
{
	node *temp = new node;
	temp = head;
	head = head->next;
	delete temp;
}

void LinkedList : DeleteLast()
// Preconditions: Member function called on existing and filled LinkedList object.
//
// Postconditions: Tail node delete, and new tail set
//
{
	node *current = new node;
	node *previous = new node;
	current = head;

	while (current->next != NULL)
	{
		previous = current;
		current = current->next;
	}
	tail = previous;
	previous->next = NULL;
	delete current;
}

void LinkedList : DeleteAtIndex(int32_t index)
// Preconditions: Member function called on existing and filled LinkedList object.
//
// Postconditions: Node at specified index deleted
//
{
	node *current = new node;
	node *previous = new node;
	current = head;
	for (int i = 1; i<index; i++)
	{
		previous = current;
		current = current->next;
	}
	previous->next = current->next;
}