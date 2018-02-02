#include <stdio.h>
#include "SimpleBSTree.h"

int SimpleBSTree::getMaxDepth(Node *root) {
	if (!root)
		return 0;
	int leftDepth = getMaxDepth(root->fL);
	int rightDepth = getMaxDepth(root->fR);
	if (leftDepth > rightDepth)
		return leftDepth + 1;
	else
		return rightDepth + 1;
}

int SimpleBSTree::getMaxWidth() {
	return getMaxDepth() == 0 ? 0 : 1 << getMaxDepth();
}

SimpleBSTree::Node *SimpleBSTree::getMinNode(Node *tmp) {
	while (tmp->fL)
		tmp = tmp->fL;
	return tmp;
}

SimpleBSTree::Node *SimpleBSTree::getMaxNode(Node *tmp) {
	while (tmp->fR)
		tmp = tmp->fR;
	return tmp;
}

/*traverse*/
void SimpleBSTree::inorderWalk() {
	inorderWalk(fRoot);
	printf("\n");
}

void SimpleBSTree::preOrderWalk() {
	preOrderWalk(fRoot);
	printf("\n");
}

void SimpleBSTree::postOrderWalk() {
	postOrderWalk(fRoot);
	printf("\n");
}
void SimpleBSTree::inorderWalk(Node *root) {
	if (!root)
		return;
	inorderWalk(root->fL);
	printf("%d ", root->fKey);
	inorderWalk(root->fR);
}

void SimpleBSTree::preOrderWalk(Node *root) {
	if (!root)
		return;
	printf("%d ", root->fKey);
	preOrderWalk(root->fL);
	preOrderWalk(root->fR);
}

void SimpleBSTree::postOrderWalk(Node *root) {
	if (!root)
		return;

	postOrderWalk(root->fL);
	postOrderWalk(root->fR);
	printf("%d ", root->fKey);
}

void SimpleBSTree::postOrderRelease(Node *root) {
	if (!root)
		return;
	postOrderRelease(root->fL);
	postOrderRelease(root->fR);
	printf("delete %d\n", root->fKey);
	delete root;
}

/*print tree*/
void SimpleBSTree::printTreeBF() {
	std::queue<Node *> nodes;
	nodes.push(fRoot);
	Node *tmp;
	while (!nodes.empty()) {
		tmp = nodes.front();
		nodes.pop();
		printf("%d ", tmp->fKey);
		if (tmp->fL)
			nodes.push(tmp->fL);
		else if (tmp->fR)
			nodes.push(tmp->fR);
	}
	printf("\n");
}

void SimpleBSTree::printTreeBF1() {
	std::queue<Node *> nodes;
	nodes.push(fRoot);
	Node *tmp;
	bool terminate = false;
	while (!nodes.empty()) {
		tmp = nodes.front();
		nodes.pop();
		while (!tmp) {
			printf("Na ");
			/*if queue has become empty, continue to pop can cause the queue become non-empty.
			* now need to terminate push node*/
			if (!nodes.empty()) { //last node & terminate push node
				tmp = nodes.front();
				nodes.pop();
			}
			else {
				terminate = true;
				break;
			}
		}

		if (terminate)
			break;
		printf("%d ", tmp->fKey);
		if (tmp->fL)
			nodes.push(tmp->fL);
		else if (!tmp->fL)
			nodes.push(nullptr);
		if (tmp->fR)
			nodes.push(tmp->fR);
		else if (!tmp->fR)
			nodes.push(nullptr);
	}
	printf("\n");
}

void SimpleBSTree::printTreeBFByLayer() {
	std::queue<Node *> nodes;
	nodes.push(fRoot);
	Node *tmp;
	int lastLayerNum = 0;
	while (!nodes.empty()) {
		tmp = nodes.front();
		nodes.pop();

		if (lastLayerNum != tmp->attr.layer) {
			printf("\n");
			printf("layer %d ", tmp->attr.layer);
			lastLayerNum = tmp->attr.layer;
		}
		if (tmp->fP)
			printf("[%s %d(parent %d)] ", tmp->attr.type.c_str(), tmp->fKey, tmp->fP->fKey);
		else {
			printf("layer %d ", tmp->attr.layer);
			printf("[%s %d] ", tmp->attr.type.c_str(), tmp->fKey);
		}


		if (tmp->fL)
			nodes.push(tmp->fL);
		if (tmp->fR)
			nodes.push(tmp->fR);
	}
	printf("\n");
}

/*节点x的后继是，大于x键值的最小键值节点.
* 1,如果x有右树，则后继是该右树的左树的最小节点
* 2,如果x没有右树，则x的后继是最低的祖先，并且该祖先的左孩子也是x的祖先*/
SimpleBSTree::Node *SimpleBSTree::getSuccessor(Node *tmp) {
	if (tmp->fR)
		return getMinNode(tmp->fR);
	Node *parent = tmp->fP;
	while (parent && parent->fR == tmp) {
		tmp = parent;
		parent = parent->fP;
	}
	return parent;
}

/*节点x的前驱是，小于x键值的最大键值节点
* 1,如果X有左树，则前驱是该左树的最大节点
* 2，如果x没有左树，则前驱是最低的祖先，且该祖先的右孩子也是x的祖先
* */
SimpleBSTree::Node *SimpleBSTree::getPredecessor(Node *tmp) {
	if (tmp->fL)
		return getMaxNode(tmp->fL);
	Node *parent = tmp->fP;
	while (parent && tmp == parent->fL) {
		tmp = parent;
		parent = parent->fP;
	}
	return parent;
}

SimpleBSTree::Node *SimpleBSTree::search(int key) {
	Node *tmp = fRoot;
	while (tmp && tmp->fKey != key) {
		if (tmp->fKey > key)
			tmp = tmp->fL;
		else
			tmp = tmp->fR;
	}
	return tmp;
}

void SimpleBSTree::insert(Node *newNode) {
	Node *tmpParent = nullptr;
	Node *tmp = fRoot;
	/*比较键值，向下查找到nullptr节点*/
	while (tmp) {
		tmpParent = tmp;
		if (tmp->fKey > newNode->fKey)
			tmp = tmp->fL;
		else
			tmp = tmp->fR;
	}
	/*找到nullptr节点，提供给新节点父节点位置*/
	newNode->fP = tmpParent;
	if (!tmpParent)
		fRoot = newNode;
	/*判断放到左边还是右边*/
	else if (tmpParent->fKey > newNode->fKey)
		tmpParent->fL = newNode;
	else
		tmpParent->fR = newNode;
}

/*将replacer子树替换orig子树*/
void SimpleBSTree::transplant(Node *orig, Node *replacer) {
	/*orig的父节点如果是null，那relacer为root*/
	if (!orig->fP)
		fRoot = replacer;
	/*如果是orig是其父节点的左子树，那就迁移replacer到左子树上*/
	else if (orig->fP->fL == orig)
		orig->fP->fL = replacer;
	/*反之，放到右子树*/
	else
		orig->fP->fR = replacer;
	/*设置replacer的父节点*/
	if (replacer)
		replacer->fP = orig->fP;
}

/*删除导致树中节点迁移
* 1,待删除节点只有一个右子树，迁移右子树到待删除节点位置
* 2,待删除节点只有一个左子树，迁移左子树到待删除节点位置
* 3,待删除节点两颗子树有存在，此时需要用后继迁移到节点位置
*   （1）后继的父节点是待删除节点，使后继替换待删除节点，使待删除节点的左节点作为后继的左节点
*   （2）后继的父节点不是待删除节点，用后继的右节点替代后继的位置，用后继替代待删除节点的位置
*   NOTE: 这里只是从树中删除结构，并没有释放内存*/
void SimpleBSTree::remove(Node *removing) {
	if (!removing->fL)
		transplant(removing, removing->fR);
	else if (!removing->fR)
		transplant(removing, removing->fL);
	else {
		Node *replacer = getMinNode(removing->fR);
		if (replacer->fP != removing) {
			transplant(replacer, replacer->fR);//把后继右节点放到后继位置
			replacer->fR = removing->fR;//后继右节点变更为删除节点右节点
			replacer->fR->fP = replacer;//待删除右节点的父节点变更为后继
		}
		transplant(removing, replacer);//后继替换到待删除节点位置
		replacer->fL = removing->fL;//后继左节点变更为待删除节点左节点
		replacer->fL->fP = replacer;//待删除左节点的父检点变更为后继
	}
}

void SimpleBSTree::insertWithAttr(Node *newNode) {
	Node *tmpParent = nullptr;
	Node *tmp = fRoot;
	/*比较键值，向下查找到nullptr节点*/
	while (tmp) {
		tmpParent = tmp;
		if (tmp->fKey > newNode->fKey)
			tmp = tmp->fL;
		else
			tmp = tmp->fR;
		newNode->attr.updateLayer();
	}
	/*找到nullptr节点，提供给新节点父节点位置*/
	newNode->fP = tmpParent;
	if (!tmpParent) {
		fRoot = newNode;
		newNode->attr.setTpye("root");
	}
	/*判断放到左边还是右边*/
	else if (tmpParent->fKey > newNode->fKey) {
		tmpParent->fL = newNode;
		newNode->attr.setTpye("left");
	}
	else {
		tmpParent->fR = newNode;
		newNode->attr.setTpye("right");
	}
}

void SimpleBSTree::updateSubTreeLayer(Node *sub, int step) {
	if (!sub)
		return;
	sub->attr.setLayer(sub->attr.layer - step);
	updateSubTreeLayer(sub->fL, step);
	updateSubTreeLayer(sub->fR, step);
}

/*将replacer子树替换orig子树*/
void SimpleBSTree::transplantWithAttr(Node *orig, Node *replacer) {
	/*orig的父节点如果是null，那relacer为root*/
	if (!orig->fP) {
		fRoot = replacer;
		if (replacer) {
			replacer->attr.setTpye("root");
		}
	}
	/*如果是orig是其父节点的左子树，那就迁移replacer到左子树上*/
	else if (orig->fP->fL == orig) {
		orig->fP->fL = replacer;
		if (replacer) {
			replacer->attr.setTpye("left");
		}
	}
	/*反之，放到右子树*/
	else {
		orig->fP->fR = replacer;
		if (replacer) {
			replacer->attr.setTpye("right");
		}
	}
	/*设置replacer的父节点*/
	if (replacer) {
		replacer->fP = orig->fP;
	}
}

void SimpleBSTree::removeWithAttr(Node *removing) {
	if (!removing->fL) {
		/*待删除节点左节点不存在，则直接拿右节点迁移到待删除节点*/
		transplantWithAttr(removing, removing->fR);
		/*迁移之后更新待删除节点右子树所有节点layer number*/
		if (removing->fR) {
			int step = removing->fR->attr.layer - removing->attr.layer;
			updateSubTreeLayer(removing->fR, step);
		}
	}
	else if (!removing->fR) {
		/*待删除节点右节点不存在，则直接拿左节点迁移到待删除节点*/
		transplantWithAttr(removing, removing->fL);
		/*迁移之后更新待删除节点左子树所有节点layer number*/
		if (removing->fL) {
			int step = removing->fL->attr.layer - removing->attr.layer;
			updateSubTreeLayer(removing->fL, step);
		}
	}
	else {
		Node *replacer = getMinNode(removing->fR);
		/*待删除节点两子节点都存在，如果待删除节点后继的父节点不是待删除节点*/
		if (replacer->fP != removing) {
			transplantWithAttr(replacer, replacer->fR);//把后继右节点放到后继位置，即后继右节点更新父节点
			/*更新后继右子树所有节点layer number*/
			if (replacer->fR) {
				int step = replacer->fR->attr.layer - replacer->attr.layer;
				updateSubTreeLayer(replacer->fR, step);
			}
			/*后继更新新的右节点*/
			replacer->fR = removing->fR;//后继右节点变更为删除节点右节点
			replacer->fR->fP = replacer;//待删除右节点的父节点变更为后继

			/*把后继迁移到待删除节点*/
			transplantWithAttr(removing, replacer);//后继替换到待删除节点位置
			/*只更新后继节点layer number*/
			replacer->attr.setLayer(removing->attr.layer);

			replacer->fL = removing->fL;//后继左节点变更为待删除节点左节点
			replacer->fL->fP = replacer;//待删除左节点的父检点变更为后继
		}
		else {
			/*待删除节点两子节点都存在，如果待删除节点后继的父节点是待删除节点*/
			transplantWithAttr(removing, replacer);//后继替换到待删除节点位置
			/*更新后继所有节点layer number*/
			int step = replacer->attr.layer - removing->attr.layer;
			updateSubTreeLayer(replacer, step);
			replacer->fL = removing->fL;//后继左节点变更为待删除节点左节点
			replacer->fL->fP = replacer;//待删除左节点的父检点变更为后继
		}

	}
}

int SimpleBSTreeUT::input[11] = { 15, 6, 18, 3, 7, 17, 20, 2, 4, 13, 9 };

/*                          15
*            6                           18
*      3             7             17            20
*  2      4      Na     13     Na      Na    Na      Na
*Na Na  Na Na  Na Na  Na  9
**/
void SimpleBSTreeUT::create() {
	for (int i = 0; i < 11; i++) {
		bst.insertWithAttr(new SimpleBSTree::Node(input[i]));
	}
	bst.printTreeBFByLayer();
}

void SimpleBSTreeUT::removeNode(int key) {
	printf("remvoe %d\n", key);
	SimpleBSTree::Node *node = bst.search(key);
	bst.removeWithAttr(node);
	bst.printTreeBFByLayer();
}