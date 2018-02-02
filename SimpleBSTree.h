/*

* SimpleBSTree.h
*
*  Created on: 2018年2月1日
*      Author: 孙浩
*/

#ifndef SIMPLEBSTREE_H_
#define SIMPLEBSTREE_H_
#include <queue>
#include <string>


class SimpleBSTree {
public:
	SimpleBSTree() : fRoot(nullptr) {}
	~SimpleBSTree() {
		releaseAllNodes();
	}

	struct NodeAttr {
		std::string type;//root, l, r
		int layer;
		NodeAttr() : type("unknown"), layer(0) {}
		void updateLayer() { layer++; }
		void setTpye(const std::string &newType) { type = newType; }
		void setLayer(int l) { layer = l; }
	};

	struct Node {
		int fKey;
		Node *fL, *fR, *fP;
		NodeAttr attr;

		Node(int key, Node *left, Node *right, Node *parent)
			: fKey(key), fL(left), fR(right), fP(parent) {
			NodeAttr();
		}
		Node(int key) : Node(key, nullptr, nullptr, nullptr) {}

		bool isLeaf() { return fL == nullptr && fR == nullptr; }
		bool onlyHasLeft() { return fL != nullptr && fR == nullptr; }
		bool onlyHasRight() { return fL == nullptr && fR != nullptr; }
	};
	/*traverse*/
	void inorderWalk();
	void preOrderWalk();
	void postOrderWalk();
	/*print tree*/
	void printTreeBF();//breadth first
	void printTreeBF1();//breadth first with separator "Na"
	void printTreeBFByLayer();//print tree breadth-firstly with details, which looks like a tree.
	/*release all nodes memory*/
	void releaseAllNodes() {
		postOrderRelease(fRoot);
	}

	int getMaxDepth() {
		return getMaxDepth(fRoot);
	}

	int getMaxDepth(Node *root);
	int getMaxWidth();
	Node *getMinNode(Node *tmp);
	Node *getMaxNode(Node *tmp);

	/*successor and predecessor*/
	Node *getSuccessor(Node *tmp);
	Node *getPredecessor(Node *tmp);

	/*basic operation*/
	Node *search(int key);
	void insert(Node *newNode);
	void transplant(Node *orig, Node *replacer);
	void remove(Node *removing);

	/*basic operation with node attribute*/
	void insertWithAttr(Node *newNode);
	void transplantWithAttr(Node *orig, Node *replacer);
	void removeWithAttr(Node *removing);

private:
	/*traverse*/
	void inorderWalk(Node *root);
	void preOrderWalk(Node *root);
	void postOrderWalk(Node *root);
	/*release node memory*/
	void postOrderRelease(Node *root);

	/*update sub tree layer num after remove operation with node attribute*/
	void updateSubTreeLayer(Node *sub, int step);

	Node *fRoot;
};

class SimpleBSTreeUT {
public:
	static int input[11];
	SimpleBSTree &bst;

	SimpleBSTreeUT(SimpleBSTree &test) : bst(test) {}

	/*create tree*/
	void create();
	/*remove node*/
	void removeNode(int key);
	/*remove leaf node*/
	void removeLeaf() { removeNode(input[5]);}
	/*remove节点右两个孩子 后继的父节点是待删除节点*/
	void removeSuccessor1() { removeNode(input[4]); }
	/*remove节点右两个孩子  后继的父节点不是待删除节点*/
	void removeSuccessor2() { removeNode(input[0]); }
};
#endif /* SIMPLEBSTREE_H_ */