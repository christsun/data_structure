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

/*�ڵ�x�ĺ���ǣ�����x��ֵ����С��ֵ�ڵ�.
* 1,���x�������������Ǹ���������������С�ڵ�
* 2,���xû����������x�ĺ������͵����ȣ����Ҹ����ȵ�����Ҳ��x������*/
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

/*�ڵ�x��ǰ���ǣ�С��x��ֵ������ֵ�ڵ�
* 1,���X����������ǰ���Ǹ����������ڵ�
* 2�����xû����������ǰ������͵����ȣ��Ҹ����ȵ��Һ���Ҳ��x������
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
	/*�Ƚϼ�ֵ�����²��ҵ�nullptr�ڵ�*/
	while (tmp) {
		tmpParent = tmp;
		if (tmp->fKey > newNode->fKey)
			tmp = tmp->fL;
		else
			tmp = tmp->fR;
	}
	/*�ҵ�nullptr�ڵ㣬�ṩ���½ڵ㸸�ڵ�λ��*/
	newNode->fP = tmpParent;
	if (!tmpParent)
		fRoot = newNode;
	/*�жϷŵ���߻����ұ�*/
	else if (tmpParent->fKey > newNode->fKey)
		tmpParent->fL = newNode;
	else
		tmpParent->fR = newNode;
}

/*��replacer�����滻orig����*/
void SimpleBSTree::transplant(Node *orig, Node *replacer) {
	/*orig�ĸ��ڵ������null����relacerΪroot*/
	if (!orig->fP)
		fRoot = replacer;
	/*�����orig���丸�ڵ�����������Ǿ�Ǩ��replacer����������*/
	else if (orig->fP->fL == orig)
		orig->fP->fL = replacer;
	/*��֮���ŵ�������*/
	else
		orig->fP->fR = replacer;
	/*����replacer�ĸ��ڵ�*/
	if (replacer)
		replacer->fP = orig->fP;
}

/*ɾ���������нڵ�Ǩ��
* 1,��ɾ���ڵ�ֻ��һ����������Ǩ������������ɾ���ڵ�λ��
* 2,��ɾ���ڵ�ֻ��һ����������Ǩ������������ɾ���ڵ�λ��
* 3,��ɾ���ڵ����������д��ڣ���ʱ��Ҫ�ú��Ǩ�Ƶ��ڵ�λ��
*   ��1����̵ĸ��ڵ��Ǵ�ɾ���ڵ㣬ʹ����滻��ɾ���ڵ㣬ʹ��ɾ���ڵ����ڵ���Ϊ��̵���ڵ�
*   ��2����̵ĸ��ڵ㲻�Ǵ�ɾ���ڵ㣬�ú�̵��ҽڵ������̵�λ�ã��ú�������ɾ���ڵ��λ��
*   NOTE: ����ֻ�Ǵ�����ɾ���ṹ����û���ͷ��ڴ�*/
void SimpleBSTree::remove(Node *removing) {
	if (!removing->fL)
		transplant(removing, removing->fR);
	else if (!removing->fR)
		transplant(removing, removing->fL);
	else {
		Node *replacer = getMinNode(removing->fR);
		if (replacer->fP != removing) {
			transplant(replacer, replacer->fR);//�Ѻ���ҽڵ�ŵ����λ��
			replacer->fR = removing->fR;//����ҽڵ���Ϊɾ���ڵ��ҽڵ�
			replacer->fR->fP = replacer;//��ɾ���ҽڵ�ĸ��ڵ���Ϊ���
		}
		transplant(removing, replacer);//����滻����ɾ���ڵ�λ��
		replacer->fL = removing->fL;//�����ڵ���Ϊ��ɾ���ڵ���ڵ�
		replacer->fL->fP = replacer;//��ɾ����ڵ�ĸ������Ϊ���
	}
}

void SimpleBSTree::insertWithAttr(Node *newNode) {
	Node *tmpParent = nullptr;
	Node *tmp = fRoot;
	/*�Ƚϼ�ֵ�����²��ҵ�nullptr�ڵ�*/
	while (tmp) {
		tmpParent = tmp;
		if (tmp->fKey > newNode->fKey)
			tmp = tmp->fL;
		else
			tmp = tmp->fR;
		newNode->attr.updateLayer();
	}
	/*�ҵ�nullptr�ڵ㣬�ṩ���½ڵ㸸�ڵ�λ��*/
	newNode->fP = tmpParent;
	if (!tmpParent) {
		fRoot = newNode;
		newNode->attr.setTpye("root");
	}
	/*�жϷŵ���߻����ұ�*/
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

/*��replacer�����滻orig����*/
void SimpleBSTree::transplantWithAttr(Node *orig, Node *replacer) {
	/*orig�ĸ��ڵ������null����relacerΪroot*/
	if (!orig->fP) {
		fRoot = replacer;
		if (replacer) {
			replacer->attr.setTpye("root");
		}
	}
	/*�����orig���丸�ڵ�����������Ǿ�Ǩ��replacer����������*/
	else if (orig->fP->fL == orig) {
		orig->fP->fL = replacer;
		if (replacer) {
			replacer->attr.setTpye("left");
		}
	}
	/*��֮���ŵ�������*/
	else {
		orig->fP->fR = replacer;
		if (replacer) {
			replacer->attr.setTpye("right");
		}
	}
	/*����replacer�ĸ��ڵ�*/
	if (replacer) {
		replacer->fP = orig->fP;
	}
}

void SimpleBSTree::removeWithAttr(Node *removing) {
	if (!removing->fL) {
		/*��ɾ���ڵ���ڵ㲻���ڣ���ֱ�����ҽڵ�Ǩ�Ƶ���ɾ���ڵ�*/
		transplantWithAttr(removing, removing->fR);
		/*Ǩ��֮����´�ɾ���ڵ����������нڵ�layer number*/
		if (removing->fR) {
			int step = removing->fR->attr.layer - removing->attr.layer;
			updateSubTreeLayer(removing->fR, step);
		}
	}
	else if (!removing->fR) {
		/*��ɾ���ڵ��ҽڵ㲻���ڣ���ֱ������ڵ�Ǩ�Ƶ���ɾ���ڵ�*/
		transplantWithAttr(removing, removing->fL);
		/*Ǩ��֮����´�ɾ���ڵ����������нڵ�layer number*/
		if (removing->fL) {
			int step = removing->fL->attr.layer - removing->attr.layer;
			updateSubTreeLayer(removing->fL, step);
		}
	}
	else {
		Node *replacer = getMinNode(removing->fR);
		/*��ɾ���ڵ����ӽڵ㶼���ڣ������ɾ���ڵ��̵ĸ��ڵ㲻�Ǵ�ɾ���ڵ�*/
		if (replacer->fP != removing) {
			transplantWithAttr(replacer, replacer->fR);//�Ѻ���ҽڵ�ŵ����λ�ã�������ҽڵ���¸��ڵ�
			/*���º�����������нڵ�layer number*/
			if (replacer->fR) {
				int step = replacer->fR->attr.layer - replacer->attr.layer;
				updateSubTreeLayer(replacer->fR, step);
			}
			/*��̸����µ��ҽڵ�*/
			replacer->fR = removing->fR;//����ҽڵ���Ϊɾ���ڵ��ҽڵ�
			replacer->fR->fP = replacer;//��ɾ���ҽڵ�ĸ��ڵ���Ϊ���

			/*�Ѻ��Ǩ�Ƶ���ɾ���ڵ�*/
			transplantWithAttr(removing, replacer);//����滻����ɾ���ڵ�λ��
			/*ֻ���º�̽ڵ�layer number*/
			replacer->attr.setLayer(removing->attr.layer);

			replacer->fL = removing->fL;//�����ڵ���Ϊ��ɾ���ڵ���ڵ�
			replacer->fL->fP = replacer;//��ɾ����ڵ�ĸ������Ϊ���
		}
		else {
			/*��ɾ���ڵ����ӽڵ㶼���ڣ������ɾ���ڵ��̵ĸ��ڵ��Ǵ�ɾ���ڵ�*/
			transplantWithAttr(removing, replacer);//����滻����ɾ���ڵ�λ��
			/*���º�����нڵ�layer number*/
			int step = replacer->attr.layer - removing->attr.layer;
			updateSubTreeLayer(replacer, step);
			replacer->fL = removing->fL;//�����ڵ���Ϊ��ɾ���ڵ���ڵ�
			replacer->fL->fP = replacer;//��ɾ����ڵ�ĸ������Ϊ���
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