#ifndef _BTREE_H_
#define _BTREE_H_

#include "Image.h"

template<class T>
class BTree {
	
	public:
		struct node {
		  T key_value;
		  node *left;
		  node *right;
		};
		int (*UserCmpFunction)(T key1, T key2);
        BTree() {
			UserCmpFunction = 0;
			root = 0;
		}
        ~BTree();

        void insert(T key) {
		  if(root!=NULL)
			insert(key, root);
		  else
		  {
			root=new node;
			root->key_value=key;
			root->left=NULL;
			root->right=NULL;
		  }
		}
        node *search(T key) {
		  return search(key, root);
		}
        void destroy_tree() {
		  destroy_tree(root);
		}
		node* Root() {
			return root;
		}
    private:
        void destroy_tree(node *leaf) {
		  if(leaf!=NULL)
		  {
			destroy_tree(leaf->left);
			destroy_tree(leaf->right);
			delete leaf;
		  }
		}
        void insert(T key, node *leaf) {
		  if(Compare(key, leaf->key_value) < 0)
		  {
			if(leaf->left!=NULL)
			 insert(key, leaf->left);
			else
			{
			  leaf->left=new node;
			  leaf->left->key_value=key;
			  leaf->left->left=NULL;    //Sets the left child of the child node to null
			  leaf->left->right=NULL;   //Sets the right child of the child node to null
			}  
		  }
		  else if(Compare(key, leaf->key_value) >= 0)
		  {
			if(leaf->right!=NULL)
			  insert(key, leaf->right);
			else
			{
			  leaf->right=new node;
			  leaf->right->key_value=key;
			  leaf->right->left=NULL;  //Sets the left child of the child node to null
			  leaf->right->right=NULL; //Sets the right child of the child node to null
			}
		  }
		}
        node *search(T key, node *leaf){
		  if(leaf!=NULL)
		  {
			if(Compare(key, leaf->key_value) == 0)
			  return leaf;
			if(Compare(key, leaf->key_value) < 0)
			  return search(key, leaf->left);
			else
			  return search(key, leaf->right);
		  }
		  else return NULL;
		}
        
        node *root;

		int Compare(T key1, T key2) const {
			if (UserCmpFunction) {
				return UserCmpFunction(key1, key2);
			} else {
				return 0;
			} 
		}
};

#endif