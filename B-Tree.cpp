//Mohammad Naeimi 9720973

#include <iostream> 
#include <string>
using namespace std;
#define HTSize 3 //Hash Table Size
#define tNum 2 // Minimum Degree 't'

class medic {
public:
	string name;
	double dosage;
	medic* next;
	medic(string _name, double _dosage) {
		name = _name;
		dosage = _dosage;
		next = NULL;
	}
};

class symptom {
public:
	string name;
	int code;
	symptom* next;
	symptom(string _name, int _num) {
		name = _name;
		code = _num;
		next = NULL;
	}
};

class disease {
public:
	string name;
	symptom* symps;
	int sympKey;
	medic* medics;
	disease(string _name) {
		name = _name;
		medics = NULL;
		symps = NULL;
		sympKey = 1;
	}
	void addSymp(int _num) {
		if (_num == 1) this->addSymp("Tab", 2);
		if (_num == 2) this->addSymp("Sargijeh", 3);
		if (_num == 3) this->addSymp("Sardard", 5);
		if (_num == 4) this->addSymp("Sorfeh", 7);
		if (_num == 5) this->addSymp("Atseh", 11);
		if (_num == 6) this->addSymp("Deldard", 13);
		if (_num == 7) this->addSymp("Tangi Nafas", 17);
		if (_num == 8) this->addSymp("Dobini", 19);
		if (_num == 9) this->addSymp("Khastegi va Kerekhti", 23);
	}
	void addSymp(string _name, int _num) {
		symptom* tmp = symps;
		if (symps) {
			for (; tmp->next != NULL; tmp = tmp->next);
			tmp->next = new symptom(_name, _num);
		}
		else symps = new symptom(_name, _num);
		sympKey *= _num;
	}
	void addMed(string _name, int _dose) {
		medic* tmp = medics;
		if (medics) {
			for (; tmp->next != NULL; tmp = tmp->next);
			tmp->next = new medic(_name, _dose);
		}
		else medics = new medic(_name, _dose);
	}
	int getSympKey() {
		return sympKey;
	}
};

class nodeIn {
public:
	disease* data;
	int key;
	nodeIn(disease* _data = NULL, int _key = 0) {
		data = _data;
		if (key) key = _key;
		else key = data->getSympKey();
	}
	void show() {
		cout << "Disease's Name: " << this->data->name << endl;
		cout << "Symptom(s):\n";
		for (symptom* i = this->data->symps; i != NULL; i = i->next) cout << "\t" << i->code << ". " << i->name << endl;
		cout << "Medication(s):\n";
		for (medic* i = this->data->medics; i != NULL; i = i->next) cout << "\t" << i->name << " -> Dosage: " << i->dosage << endl << endl;
	}
};

class bTreeNode {
public:
	nodeIn* nodes[2 * tNum - 1]; // An array of keys 
	bTreeNode* childs[2 * tNum]; // An array of child pointers 
	int occP;	 // Current number of keys 
	bool isLeaf; // Is true when node is leaf. Otherwise false 
	int level;

	bTreeNode(int _level = 0, bool _leaf = 1) {
		isLeaf = _leaf;
		for (int i = 0; i < 2 * tNum - 1; i++) nodes[i] = NULL;
		for (int i = 0; i < 2 * tNum; i++) childs[i] = NULL;
		level = _level;
		occP = 0;
	}

	//Part1
	void traverse() {
		int i;
		for (i = 0; i < occP; i++) {
			if (!isLeaf) childs[i]->traverse();
			cout << nodes[i]->key << "- " << nodes[i]->data->name << ".  ";
		}
		if (!isLeaf) childs[i]->traverse();
	}

	nodeIn* findNode(int k) {
		int i = 0;
		for (; i < occP && k > nodes[i]->key; i++);
		if (nodes[i]->key == k) return nodes[i];
		else if (isLeaf) return NULL;
		else return childs[i]->findNode(k);
	}

	bool isFull() {
		if (occP == 2 * tNum - 1) return 1;
		return 0;
	}

	//Part2
	void split(int i, bTreeNode* left) {
		bTreeNode* right = new bTreeNode(left->isLeaf);
		left->occP = tNum - 1;
		right->occP = tNum - 1;
		for (int j = 0; j < tNum - 1; j++) {
			right->nodes[j] = left->nodes[j + tNum];
			left->nodes[j + tNum] = NULL;
		}
		if (!(left->isLeaf)) {
			for (int j = 0; j < tNum; j++) {
				right->childs[j] = left->childs[j + tNum];
				left->childs[j + tNum] = NULL;
			}
		}
		for (int j = occP; i + 1 <= j; j--) childs[j + 1] = childs[j];
		childs[i + 1] = right;
		for (int j = occP - 1; i <= j; j--)	nodes[j + 1] = nodes[j];
		nodes[i] = left->nodes[tNum - 1];
		left->nodes[tNum - 1] = NULL;
		occP++;
	}

	void addNode(nodeIn* _node) {
		int i = occP - 1;
		if (isLeaf) {
			for (; i >= 0 && nodes[i]->key > _node->key; i--) nodes[i + 1] = nodes[i];
			nodes[i + 1] = _node;
			occP++;
		}
		else {
			for (; 0 <= i && _node->key < nodes[i]->key; i--);
			if (childs[i + 1]->occP == 2 * tNum - 1) {
				split(i + 1, childs[i + 1]);
				if (nodes[i + 1]->key < _node->key) i++;
			}
			childs[i + 1]->addNode(_node);
		}
	}

	//Part3
	void removeDown(int i) {
		nodeIn* k = nodes[i];
		if (tNum <= childs[i]->occP) {
			bTreeNode* cur = childs[i];
			for (; !cur->isLeaf; cur = cur->childs[cur->occP]);
			nodeIn* pred = cur->nodes[cur->occP - 1];
			delete nodes[i];
			nodes[i] = pred;
			childs[i]->delNode(pred->key);
		}
		else if (tNum <= childs[i + 1]->occP) {
			bTreeNode* cur = childs[i + 1];
			for (; !cur->isLeaf; cur = cur->childs[0]);
			nodeIn* succ = cur->nodes[0];
			delete nodes[i];
			nodes[i] = succ;
			childs[i + 1]->delNode(succ->key);
		}
		else {
			merge(i);
			childs[i]->delNode(k->key);
		}
		return;
	}

	void delNode(int _key) {
		int i = 0;
		for (; i < occP && nodes[i]->key < _key; ++i);
		if (i < occP && nodes[i]->key == _key) {
			if (isLeaf) {
				delete nodes[i];
				for (int j = i + 1; j < occP; j++) nodes[j - 1] = nodes[j];
				occP--;
			}
			else removeDown(i);
		}
		else {
			if (isLeaf) {
				cout << "There isn't Such Disease!\n";
				return;
			}
			bool flag = ((i == occP) ? true : false);
			if (childs[i]->occP < tNum) fill(i);
			if (flag && i > occP) childs[i - 1]->delNode(_key);
			else childs[i]->delNode(_key); //EDIT
		}
		return;
	}

	void fill(int i) {
		if (i != 0 && tNum <= childs[i - 1]->occP) {
			bTreeNode* myNode = childs[i];
			bTreeNode* prvNode = childs[i - 1];
			for (int j = myNode->occP - 1; 0 <= j; j--) myNode->nodes[j + 1] = myNode->nodes[j];
			if (!myNode->isLeaf) {
				for (int j = myNode->occP; 0 <= j; j--) myNode->childs[j + 1] = myNode->childs[j];
			}
			myNode->nodes[0] = nodes[i - 1];
			if (!myNode->isLeaf) myNode->childs[0] = prvNode->childs[prvNode->occP];
			nodes[i - 1] = prvNode->nodes[prvNode->occP - 1];
			myNode->occP++;
			prvNode->occP--;
		}
		else if (i != occP && tNum <= childs[i + 1]->occP) {
			bTreeNode* myNode = childs[i];
			bTreeNode* nxtNode = childs[i + 1];
			myNode->nodes[(myNode->occP)] = nodes[i];
			if (!(myNode->isLeaf)) myNode->childs[(myNode->occP) + 1] = nxtNode->childs[0];
			nodes[i] = nxtNode->nodes[0];
			for (int j = 1; j < nxtNode->occP; j++)	nxtNode->nodes[j - 1] = nxtNode->nodes[j];
			if (!nxtNode->isLeaf) {
				for (int j = 1; j <= nxtNode->occP; j++) nxtNode->childs[j - 1] = nxtNode->childs[j];
			}
			myNode->occP++;
			nxtNode->occP--;
		}
		else {
			if (i != occP) merge(i);
			else merge(i - 1);
		}
		return;
	}

	void merge(int i) {
		bTreeNode* lNode = childs[i];
		bTreeNode* rNode = childs[i + 1];
		lNode->nodes[tNum - 1] = nodes[i];
		for (int j = 0; j < rNode->occP; j++) lNode->nodes[j + tNum] = rNode->nodes[j];
		if (!lNode->isLeaf) {
			for (int j = 0; j <= rNode->occP; j++) lNode->childs[j + tNum] = rNode->childs[j];
		}
		for (int j = i + 1; j < occP; j++) nodes[j - 1] = nodes[j];
		for (int j = i + 2; j <= occP; j++) childs[j - 1] = childs[j];
		lNode->occP += rNode->occP + 1;
		occP--;
		delete(rNode);
		return;
	}

	friend class bTree;
};

class bTree {
public:
	bTreeNode* root; // Pointer to Root 

	bTree() {
		root = NULL;
	}

	//Part1
	void traverse() {
		if (root != NULL) root->traverse();
	}

	nodeIn* findNode(int k) {
		if (root) return root->findNode(k);
		else return NULL;
	}

	//Part2
	void addNode(nodeIn* _node) {
		if (root) {
			if (root->isFull()) {
				bTreeNode* tmp = new bTreeNode(0, 0);
				tmp->childs[0] = root;
				tmp->split(0, root);
				if (_node < tmp->nodes[0]) tmp->childs[0]->addNode(_node);
				else tmp->childs[1]->addNode(_node);
				root = tmp;
			}
			else root->addNode(_node);
		}
		else {
			root = new bTreeNode(1);
			root->nodes[0] = _node;
			root->occP = 1;
		}
	}

	//Part3
	void delNode(int _key) {
		if (root) {
			root->delNode(_key);
			if (root->occP == 0) {
				bTreeNode* tmp = root;
				if (root->isLeaf) root = NULL;
				else root = root->childs[0];
				delete tmp;
			}
		}
		else cout << "Disease Not Found!\n";
		return;
	}
};

class hashTable {
public:
	bTree nodesHT[HTSize];

	hashTable() {
		for (int i = 0; i < HTSize; i++) nodesHT[i].root = NULL;
	}
	void traverse() {
		for (int i = 0; i < HTSize; i++) {
			cout << "\n\nHash to " << i << ":\n";
			nodesHT[i].traverse();
		}
	}
	void addNode(nodeIn* _node) {
		nodesHT[_node->key % HTSize].addNode(_node);
	}
	nodeIn* findNode(int _key) {
		return nodesHT[_key % HTSize].findNode(_key);
	}
	void delNode(int _key) {
		nodesHT[_key % HTSize].delNode(_key);
	}
};

hashTable mainTable;

int toKey(int n) {
	if (n == 1) return 2;
	else if (n == 2) return 3;
	else if (n == 3) return 5;
	else if (n == 4) return 7;
	else if (n == 5) return 11;
	else if (n == 6) return 13;
	else if (n == 7) return 17;
	else if (n == 8) return 19;
	else if (n == 9) return 21;
}

// Driver program to test above functions 
int main() {
	
	//Test1
	int tmpint1 = 1, tmpint2 = 1;
	string input;
	cout << "Start!!!\n";
	while (true) {
		cout << "--------------------------------------------------\nWhat You Wanna Do? (0. Exit, 1. Add, 2. Delete, 3. Find) ";
		cin >> tmpint1;
		if (tmpint1 == 0) break; //Exit
		else if (tmpint1 == 1) { //Add
			cout << "Disease's Name: ";
			cin >> input;
			disease* dtmp = new disease(input);
			cout << "Possible Symptoms:\n 1. Tab\n 2. Sargijeh\n 3. Sardard\n 4. Sorfeh\n 5. Atseh\n 6. Deldard\n 7. Tangi nafas\n 8. Dobini\n 9. Khastegi va Kerekhti";
			cout << "\nSymptom: ";
			cin >> tmpint2;
			dtmp->addSymp(tmpint2);
			while (true) {
				cout << "Any Other Symptom? (0. No) ";
				cin >> tmpint2;
				if (tmpint2 == 0) {
					cout << "Medication: ";
					cin >> input;
					cout << "Dosage: ";
					cin >> tmpint2;
					dtmp->addMed(input, tmpint2);
					while (true) {
						cout << "Any Other Medication? (0. No) ";
						cin >> input;
						if (input == "0") break;
						cout << "Dosage: ";
						cin >> tmpint2;
						dtmp->addMed(input, tmpint2);
					}
					nodeIn* tmp = new nodeIn(dtmp, dtmp->getSympKey());
					mainTable.addNode(tmp);
					break;
				}
				dtmp->addSymp(tmpint2);
			}
		}
		else if (tmpint1 == 2) { //Delete
			tmpint2 = 1;
			cout << "Possible Symptoms:\n 1. Tab\n 2. Sargijeh\n 3. Sardard\n 4. Sorfeh\n 5. Atseh\n 6. Deldard\n 7. Tangi nafas\n 8. Dobini\n 9. Khastegi va Kerekhti";
			cout << "\nSymptom: ";
			cin >> tmpint1;
			tmpint2 *= toKey(tmpint1);
			while (true) {
				cout << "Any Other Symptom? (0. No) ";
				cin >> tmpint1;
				if (tmpint1 == 0) {
					cout << "\nSympKey: " << tmpint2 << endl;
					break;
				}
				tmpint2 *= toKey(tmpint1);
			}
			nodeIn* tmp = mainTable.findNode(tmpint2);
			if (tmp) {
				tmp->show();
				cout << "Do you Wanna Delete It? (0. No, 1. Yes) ";
				cin >> tmpint1;
				if (tmpint1 == 1) mainTable.delNode(tmpint2);
			}
			else cout << "Disease Not Found!\n";
		}
		else if (tmpint1 == 3) { //Find
			tmpint2 = 1;
			cout << "Possible Symptoms:\n 1. Tab\n 2. Sargijeh\n 3. Sardard\n 4. Sorfeh\n 5. Atseh\n 6. Deldard\n 7. Tangi nafas\n 8. Dobini\n 9. Khastegi va Kerekhti";
			cout << "\nSymptom: ";
			cin >> tmpint1;
			tmpint2 *= toKey(tmpint1);
			while (true) {
				cout << "Any Other Symptom? (0. No) ";
				cin >> tmpint1;
				if (tmpint1 == 0) {
					cout << "\nSympKey: " << tmpint2 << endl;
					break;
				}
				tmpint2 *= toKey(tmpint1);
			}
			nodeIn* tmp = mainTable.findNode(tmpint2);
			if (tmp) tmp->show();
			else cout << "Disease Not Found!\n";
		}
	}
	
	
	/*
	//Test2
	disease* dtmp;
	nodeIn* ntmp;

	for (int i = 1; i <= 8; i++) {
		dtmp = new disease("Disease" + to_string(i));
		dtmp->addSymp("Symptom", i);
		dtmp->addMed("Medicate", 20);
		ntmp = new nodeIn(dtmp, dtmp->sympKey);
		mainTable.addNode(ntmp);
	}

	for (int i = 2; i < 4; i++) mainTable.delNode(i);

	mainTable.traverse();

	for (int i = 0; i < 100; i += 12) {
		ntmp = mainTable.findNode(i);
		if (ntmp) cout << "\n\nfind: " << ntmp->data->name;
		else cout << "\n\nNode " << i << " Not Found!";
	}
	*/

	/*
	//Test3
	disease* dtmp = new disease("Cold");
	dtmp->addSymp("Tab", 1);
	dtmp->addSymp("Atseh", 5);
	dtmp->addSymp("Sorfeh", 4);
	dtmp->addMed("Adult Cold", 50);
	nodeIn* ntmp = new nodeIn(dtmp, dtmp->sympKey);
	mainTable.addNode(ntmp);

	dtmp = new disease("Corona");
	dtmp->addSymp("Tab", 1);
	dtmp->addSymp("Tangi Nafas", 7);
	dtmp->addSymp("Sorfeh", 4);
	dtmp->addMed("Remdesivir?", 15);
	ntmp = new nodeIn(dtmp, dtmp->sympKey);
	mainTable.addNode(ntmp);

	dtmp = new disease("Migren");
	dtmp->addSymp("Sardard", 3);
	dtmp->addSymp("Sargijeh", 2);
	dtmp->addSymp("Dobini", 9);
	dtmp->addMed("Jelophen", 20);
	ntmp = new nodeIn(dtmp, dtmp->sympKey);
	mainTable.addNode(ntmp);
	*/


	return 0;
}


