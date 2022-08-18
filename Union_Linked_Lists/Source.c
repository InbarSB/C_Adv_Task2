#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "md5.h" //using void md5String(const char* input, char* output);
#include "md4.h" //using void md4String(const char* input, char* output);

#define MD4_HASH 0
#define MD5_HASH 1
#define True 1
#define False 0
typedef unsigned char HashType;
typedef unsigned char Byte;
typedef char Bool;

typedef struct msgWithHash
{
	unsigned int id;
	char* text;
	union
	{
		Byte md4[16];
		Byte md5[16];
	} hashDigest;
	HashType type;
}msgWithHash;

typedef struct msgListNode msgListNode;

struct msgListNode
{
	msgWithHash* data;
	msgListNode* next;
};

void print_hash(const unsigned char* p);  //Done
msgWithHash* createMessage(unsigned int id, const char* text, HashType type); //Done
void printMessage(const msgWithHash* message); //Done
msgListNode* addMessage(msgListNode* head, msgWithHash* data); //Done
int findMsgByHashIterative(const msgListNode* head, Byte hash[16]); //Done
int findMsgByHashRecursive(const msgListNode* head, Byte hash[16]); //Done
msgListNode* deleteMessage(msgListNode* head, unsigned int id); //Done
Bool verifyIntegrity(msgListNode* head, unsigned int id, Byte compareHash[16]); //Done
msgListNode* reverseMsgList(msgListNode* head);  //Done
void freeMsgList(msgListNode* head);  //Done
void printlist(msgListNode* head);

int main() {
	int result = 0;
	msgListNode* head = NULL;
	Bool integ_result = 0;

	//creating new messages 
	msgWithHash* msg1 = createMessage(87651233, "", MD4_HASH);
	msgWithHash* msg2 = createMessage(3, "banana1", MD5_HASH);
	msgWithHash* msg3 = createMessage(275, "banana2!", MD4_HASH);
	msgWithHash* msg4 = createMessage(342234, "Hello World", MD4_HASH);
	msgWithHash* msg5 = createMessage(685, "Hi", MD5_HASH);

	//print out messages
	printMessage(msg1);
	printMessage(msg2);
	printMessage(msg3);
	printMessage(msg4);

	//create List of messages and print out the list
	head = addMessage(head, msg3);
	head = addMessage(head, msg2);
	head = addMessage(head, msg4);
	head = addMessage(head, msg1);
	printf("\nMessages List:\n--------------------------------------------\n");
	printlist(head);

	//reverse List order  and print out the list
	head = reverseMsgList(head);
	printf("\nReverse Messages List:\n--------------------------------------------\n");
	printlist(head);

	//find message by Hash (Iterative) and print message accordng to result
	result = findMsgByHashIterative(head, msg3->hashDigest.md4);
	if (result == -1)
		printf("\nfindMsgByHashIterative - Message was not found\n");
	else if (result == -2)
		printf("\nfindMsgByHashIterative - Error was accure\n");
	else
		printf("\nfindMsgByHashIterative - Message was found - message index is: %d\n", result);

	//find message by Hash (Recursive) and print message accordng to result
	result = findMsgByHashRecursive(head, msg3->hashDigest.md4);
	if (result == -1)
		printf("\nfindMsgByHashRecursive - Message was not found\n");
	else if (result == -2)
		printf("\nfindMsgByHashRecursive - Error was accure\n");
	else
		printf("\nfindMsgByHashRecursive - Message was found - message index is: %d\n", result);

	//find message by Hash (Recursive) and print message accordng to result
	result = findMsgByHashRecursive(head, msg5->hashDigest.md5);
	if (result == -1)
		printf("\nfindMsgByHashRecursive - Message was not found\n");
	else if (result == -2)
		printf("\nfindMsgByHashRecursive - Error was accure\n");
	else
		printf("\nfindMsgByHashRecursive - Message was found - message index is: %d\n", result);

	//find message by id, compare given Hash for Validatin and print message accordng to result
	integ_result = verifyIntegrity(head, 275, msg2->hashDigest.md5);
	if (integ_result == -1)
		printf("\nverifyIntegrity - Message was not found\n");
	else if (integ_result == 0)
		printf("\nverifyIntegrity - Message was found, but Hash is not match\n");
	else
		printf("\nverifyIntegrity - Message was found, and Hash is matched - Verify pass\n");

	//find message by id, compare given Hash for Validatin and print message accordng to result
	integ_result = verifyIntegrity(head, 275, msg3->hashDigest.md4);
	if (integ_result == -1)
		printf("\nverifyIntegrity - Message was not found\n");
	else if (integ_result == 0)
		printf("\nverifyIntegrity - Message was found, but Hash is not match\n");
	else
		printf("\nverifyIntegrity - Message was found, and Hash is matched - Verify pass\n");

	//find message by id, compare given Hash for Validatin and print message accordng to result
	integ_result = verifyIntegrity(head, 685, msg5->hashDigest.md5);
	if (integ_result == -1)
		printf("\nverifyIntegrity - Message was not found\n");
	else if (integ_result == 0)
		printf("\nverifyIntegrity - Message was found, but Hash is not match\n");
	else
		printf("\nverifyIntegrity - Message was found, and Hash is matched - Verify pass\n");

	//delete specific message
	head = deleteMessage(head, 342234);
	printf("\nMessages List (after delete message):\n--------------------------------------------\n");
	printlist(head);

	//release message list - free all memory allocations
	freeMsgList(head);
}

void print_hash(const unsigned char* p) {
	for (unsigned int i = 0; i < 16; ++i) {
		printf("%02x", p[i]);
	}
	printf("\n");
}

msgWithHash* createMessage(unsigned int id, const char* text, HashType type) {
	Byte temp_hash[16] = { 0 };
	msgWithHash* new_msg = malloc(sizeof(msgWithHash));	//new message memory allocation
	if (!new_msg) { puts("allocation failed"); exit(0); }
	new_msg->text = (char*)malloc(strlen(text) + 1);	//string text memory allocation
	if (!new_msg->text) { puts("allocation failed"); exit(0); }
	new_msg->id = id;	//insert id to id field
	strcpy(new_msg->text, text);	//copy string text to text field allocated above
	new_msg->type = type;	//insert message type to type field
	if (new_msg->type == MD4_HASH) {	//Hash the string according to Hash Type
		md4String(new_msg->text, new_msg->hashDigest.md4);
		return new_msg;
	}
	else if (new_msg->type == MD5_HASH) {
		md5String(new_msg->text, new_msg->hashDigest.md5);
		return new_msg;
	}
	else
		return NULL;
}

void printMessage(const msgWithHash* message) {
	printf("Message ID:  %d\n", message->id);		//print message id
	printf("Message Test:  %s\n", message->text);		//print message string test
	if (message->type == MD4_HASH) {	//print message Hash according to relevant Hash
		printf("Message Hash MD4:  ");
		print_hash(message->hashDigest.md4);
	}
	else {
		printf("Message Hash MD5:  ");
		print_hash(message->hashDigest.md5);
	}
	printf("\n");
}

msgListNode* addMessage(msgListNode* head, msgWithHash* data) {
	msgListNode* tmp = head;
	msgListNode* new_msg_add = malloc(sizeof(msgListNode));	//nnew Node memory allocation 
	if (!new_msg_add) { puts("allocation failed"); exit(0); }
	new_msg_add->data = data;	//connect msg to data field
	new_msg_add->next = NULL;	//connect next pointer to NULL

	//sort the messages order by id - if new msg id is less than the one head is pointing on
	//this will be the 1st node and head pointer will be replace
	//if not - find the place the new node need to be placed in and place it (this includes add node at the end)
	if (head == NULL || new_msg_add->data->id < tmp->data->id) {
		new_msg_add->next = head;
		head = new_msg_add;
	}
	else
	{
		while (tmp->next != NULL && new_msg_add->data->id > tmp->next->data->id)
			tmp = tmp->next;
		new_msg_add->next = tmp->next;
		tmp->next = new_msg_add;
	}
	return head;
}

int findMsgByHashIterative(const msgListNode* head, Byte hash[16]) {
	msgListNode* tmp = head;
	int count = 0, index = 1;
	if (hash == NULL)	//if Hash is Empty - return -2 - error flag
		return -2;
	//compare node Hash with given Hash - if matched counter will be 16, and the index will be sent back.
	//if not matched, index will be increased, and look for next node,. if Hash not found -> return -1
	while (tmp != NULL) {
		for (int i = 0; i < 16; i++)
		{
			if ((tmp->data->hashDigest.md4[i]) != hash[i])
				break;
			else
				count++;	//count==16 if all chars in Hash are matched
		}
		if (count == 16)
			return index;	//message index
		index++;
		tmp = tmp->next;
	}
	return -1;
}

int findMsgByHashRecursive(const msgListNode* head, Byte hash[16]) {
	msgListNode* tmp = head;
	int count = 0, result = 0;;
	if (hash == NULL)	//if Hash is Empty - return -2 - error flag
		return -2;
	if (head == NULL)	//check if this is the end of the list
		return -1;
	else {
		for (int i = 0; i < 16; i++) {		//compare the node Hash with the given Hash
			{
				if ((tmp->data->hashDigest.md4)[i] != hash[i])
					break;
				else
					count++;
			}
		}
		if (count == 16)		//count==16 if all chars in Hash are matched
			return 1;
		result = findMsgByHashRecursive(head->next, hash);		//Recursive call for the function with Head points to next node
		if (result == -1)		//if no node is matched - -1 will be returned
			return -1;
		else if (result == -2)		//if error accure is matched - -2 will be returned
			return -2;
		else
			return 1 + result;		//calculate the matched node index (sum up all nodes checked until found and add 1)
	}

}

msgListNode* deleteMessage(msgListNode* head, unsigned int id) {
	msgListNode* tmp = head, * tmp2 = NULL;
	if (head->data->id == id) {		//check if the message at the begin of the list, release all memory allocated from message
		head = head->next;			//release memory allocation from node, return new head pointer to the list
		free(tmp->data->text);
		free(tmp->data);
		free(tmp);
	}
	else {
		while (tmp->next != NULL && tmp->data->id != id) { //find the message location - release all memory allocated from message and node
			tmp2 = tmp;
			tmp = tmp->next;
		}
		if (tmp->next == NULL && tmp->data->id != id)		//check if message id was not found
			printf("\nSorry, we didn't found message to delete\n");
		else if (tmp->next == NULL && tmp->data->id == id)		//message id was found at the end of the list - 
		{														//release all memory allocated from message and node
			tmp2->next = NULL;
			free(tmp->data->text);
			free(tmp->data);
			free(tmp);
		}
		else  //(tmp->next != NULL && tmp->data->id == id)
		{											//message was found (not 1st or last node) - release all memory allocated from message and node
			tmp2->next = tmp->next;
			free(tmp->data->text);
			free(tmp->data);
			free(tmp);
		}
	}
	return head;
}

Bool verifyIntegrity(msgListNode* head, unsigned int id, Byte compareHash[16]) {
	msgListNode* tmp = head;
	int count = 0;
	while (tmp != NULL && tmp->data->id != id)		//find message by compare message id - if not found return -1
		tmp = tmp->next;
	if (tmp == NULL)
		return -1;
	else {											//find message by compare message id - if found  - compare with given Hash
		for (int i = 0; i < 16; i++) {
			{
				if ((tmp->data->hashDigest.md4)[i] != compareHash[i])
					break;
				else
					count++;
			}
		}
		if (count == 16)
			return 1;								//if count == 16 -> all 16 chars are matched - Hash is matched
		else
			return 0;								//if count != 16 -> not all 16 chars are matched - Hash is not matched
	}
}

msgListNode* reverseMsgList(msgListNode* head) {
	msgListNode* tmp1 = head, * tmp2 = NULL;
	if (head->next == NULL)			//check if list have 1 node - return head - no need to reverse
		return head;
	tmp2 = tmp1->next;
	head = tmp2->next;
	if (head->next == NULL)			//check if list have only 2 nodes - switch nodes places
	{
		tmp1->next = NULL;
		tmp2->next = tmp1;
		head->next = tmp2;
	}
	else {									// we have list with at least 3 nodes
		tmp1->next = NULL;					//check tmp1 to point NULL (this will move from 1st node to last one)
		while (head->next != NULL)			// 3 pointers - tmp1 points on 1st node, tmp2 points on 2nd node, head points on 3rd node
		{
			tmp2->next = tmp1;				//2nd node pointer will point on the 1st node
			tmp1 = tmp2;					//tmp1 will move to 2nd node
			tmp2 = head;					//tmp2 will move to 3rd node
			head = head->next;				//head pointer will move to 4th node and so..
		}
		tmp2->next = tmp1;					//will make sure the last 2 nodes (that will now be the 1st and 2nd nodes)
		head->next = tmp2;					//pointers will move
	}
	return head;
}

//free all memory allocation - message text, message and node
void freeMsgList(msgListNode* head) {
	msgListNode* tmp = head;
	while (head != NULL) {
		head = head->next;
		free(tmp->data->text);
		free(tmp->data);
		free(tmp);
		tmp = head;
	}
}

//print full list in - order
void printlist(msgListNode* head) {
	msgListNode* tmp = head;
	while (tmp != NULL) {
		printMessage(tmp->data);
		tmp = tmp->next;
	}
}