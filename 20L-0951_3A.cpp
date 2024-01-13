#include<iostream>
using namespace std;
#include<fstream>
#include<queue>

class node
{
public:
	int frequency;
	node* left;
	node* right;
	string chr;

	node()
	{
		left = nullptr;
		right = nullptr;
	}
	node(int f, string str)
	{
		frequency = f;
		chr = str;
		left = nullptr;
		right = nullptr;
	}
	void insert(int f, char str)
	{
		frequency = f;
		chr = str;
		left = nullptr;
		right = nullptr;
	}
	
};
class BinaryTree           
{
	
	node* root;
	void print_leaves(node* curr)       //for testing purposes
	{
		if (curr == nullptr)
			return;
		print_leaves(curr->left);
		if (curr->right == nullptr && curr->left == nullptr)
			cout << curr->chr << " ";
		print_leaves(curr->right);
	}
	void generate_code(string c, string temp_codes,string &codes ,node* curr)    //generates the code for a character using recursion 
	{
		if (curr == nullptr)
			return;
		
		
	    generate_code(c, temp_codes+"0", codes, curr->left);

		if (curr->chr == c)
		{
			codes = temp_codes;
		}
		
		generate_code(c, temp_codes+"1", codes, curr->right);
	}
	void write_to_file(ofstream& fout, node* curr)            //writes to file in preorder
	{
		if (curr == nullptr)
		{
			fout << "-1"<<endl;
			return;
		}

		fout << curr->frequency << " " << curr->chr << "|";         //assuming that data wont include '|' char
		write_to_file(fout, curr->left);
		write_to_file(fout, curr->right);
	}
	node* read_file(ifstream& fin, node* curr)           //reads the file and reconstructs the tree
	{
		
		if (fin.eof())
			cout << "done";
			int f;
			string c;
			curr = nullptr;
			fin >> f;
			if (f != -1)
			{
				char* chr = new char[f+1];
				char extra_space;
				fin.get(extra_space);           //to read the extra space
				fin.getline(chr,f+1,'|');
				c = chr;
				delete[]chr;
				curr = new node(f, c);
				curr->left=read_file(fin, curr->left);
				curr->right=read_file(fin,curr->right);
			}
			
				return curr;

		
	}
public:
	BinaryTree()
	{
		root = nullptr;
	}
	node* insert_both(node* n1,node*n2)                         //takes two nodes and connects them to root
	{
		root = new node;
		root->frequency = n1->frequency + n2->frequency;
		root->chr = n1->chr + n2->chr;
		root->left = n1;
		root->right = n2;
		return root;
	}
	void print_leaves()
	{
		print_leaves(root);
	}
	string generate_code(string c)
	{
		string temp_codes;
		string codes;
		generate_code(c, temp_codes,codes, root);
		return codes;
	}
	void LevelOrder()        //for testing purposes
	{
		queue<node*> q;
		q.push(root);
		node* curr;
		while (!q.empty())
		{
			curr = q.front();
			q.pop();
			cout << curr->chr << " " << curr->frequency << endl;
			if (curr->left != nullptr)
				q.push(curr->left);
			if (curr->right != nullptr)
				q.push(curr->right);

		}
	}
	void store(char* filename)          //stores the file
	{
		ofstream fout;
		fout.open(filename);
		write_to_file(fout, root);
		fout.close();
	}
	void restore(char* filename) //reads the file
	{
		ifstream fin;
		fin.open(filename);
		if (!fin)
		{
			cout << "cannot open the file";
			return;
		}
		root=read_file(fin, this->root);
		
	}
	friend void Decode(BinaryTree& tree, char* bin_file, char* decompressed_file);
	
};

class minheap
{
	node** arr;
	int capacity;
	int totalcount;
	void heapify(int i)
	{
		node* temp;
		if (i == 1)
			return;        //no need to heapify when only one element is passed
		if (arr[i / 2]->frequency > arr[i]->frequency)
		{
			while (i != 1)
			{
				if (arr[i / 2]->frequency > arr[i]->frequency)
				{
					temp = arr[i / 2];
					arr[i / 2] = arr[i];
					arr[i] = temp;
				}
				i = i / 2;
			}
		}
	}

public:
	minheap()
	{
		arr = nullptr;
		capacity = 0;
		totalcount = 0;
	}
	minheap(int cap)
	{
		capacity = cap + 1;
		arr = new node*[capacity];
		for (int i = 0; i < capacity; i++)
			arr[i] = nullptr;
		totalcount = 0;
		arr[0] = nullptr;
	}
	void insert(node* x)
	{
		if (totalcount == capacity)
		{
			int size = capacity * 2;
			node** arr2 = new node*[size];
			for (int i = 0; i < size; i++)
				arr2[i] = nullptr;
			for (int i = 0; i < capacity; i++)
				arr2[i] = arr[i];
			delete[] arr;
			arr = arr2;
		}
		arr[totalcount + 1] = x;
		totalcount++;
		heapify(totalcount);
	}
	
	void Delete()
	{
		node* temp = arr[1];
		arr[1] = arr[totalcount];
		arr[totalcount] = temp;
		totalcount--;
		int i = 1;
		while (i < totalcount)
		{
			if ((i * 2 < totalcount))
			{
				if (arr[i * 2]->frequency < arr[i]->frequency)
				{
					temp = arr[i * 2];
					arr[i * 2] = arr[i];
					arr[i] = temp;
				}
				if (arr[i * 2 + 1]->frequency < arr[i]->frequency)
				{
					temp = arr[i * 2 + 1];
					arr[i * 2 + 1] = arr[i];
					arr[i] = temp;
				}

			}
			i = i * 2;
		}
	}
	bool isEmpty()
	{
		if (totalcount == 0)
			return true;
		return false;
	}
	~minheap()
	{
		while (!isEmpty())
		{
			Delete();
		}
	}
	node* getmin()
	{
		if (!isEmpty())
			return arr[1];
	}

	void print()
	{
		for (int i = 1; i < totalcount; i++)
			cout << arr[i]->frequency << " ";
	}
	int get_totalcount()
	{
		return totalcount;
	}

};

void unique_characters(char* filename, string& all_chars)   //finds all the unique characters in a file
{
	ifstream fin;
	fin.open(filename);
	if (!fin)
	{
		cout << "cannot open file";
		return;
	}
	
	char c;
	bool already = false;
	while (!fin.eof())
	{
		fin.get(c);
		for (int i = 0; i < all_chars.size(); i++)
		{
			if (all_chars[i] == c)
				already = true;
		}
		if (!already)
			all_chars = all_chars + c;
		already = false;
	}
	fin.close();
}
void calculate_freq(char*filename,string all_chars, int* &arr,int&size)      //calculates all the frequencies of unique characters
{
	 size = all_chars.size();
	arr = new int[size];
	for (int i = 0; i < size; i++)
		arr[i] = 0;
	ifstream fin;
	fin.open(filename);
	if (!fin)
	{
		cout << "cannot open file";
		return;
	}
	char c;
	string whole_file;
	while (!fin.eof())
	{
		fin.get(c);
		whole_file = whole_file + c;
	}

	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < whole_file.size() - 1; j++)
		{
			if (all_chars[i] == whole_file[j])
				arr[i]++;
		}

	}
	fin.close();
}
void Encode(BinaryTree tree, char* filename,char*compressed)     //encodes the file
{
	ifstream fin;
	fin.open(filename);
	if (!fin)
	{
		cout << "cannot open file";
		return;
	}
	char c;
	string str;
	string code;
	while (!fin.eof())
	{
		fin.get(c);
		str = c;
		code = code + tree.generate_code(str);     //generating code of str which contains a single character
	}
	fin.close();
	ofstream fout;
	fout.open(compressed);
	fout << code;
	fout.close();
	
}

void Decode(BinaryTree& tree, char* bin_file,char*decompressed_file)     
{
	ifstream fin;
	fin.open(bin_file);
	if (!fin)
	{
		cout << "\n cannot open file\n";
		return;
	}
	string binary;
	fin >> binary;
	fin.close();
	node* curr = tree.root;
	if (curr == nullptr)
		return;
	ofstream fout;
	fout.open(decompressed_file);
	for (int i = 0; i < binary.size(); i++)
	{
		if (curr->left == nullptr && curr->right == nullptr)
		{
			fout << curr->chr;
			curr = tree.root;
		}
		if (binary[i] == '0')
			curr = curr->left;
		else if (binary[i] == '1')
			curr = curr->right;

	}
	fout.close();

}

// dont forget to give extensions while entering the filenames
void main()
{
	
	string all_chars;                               //unique chars
	cout << "Enter the file name to compress: ";
	char c[50];
	cin.getline(c, 50);
	unique_characters(c, all_chars);                //place all unique character in all_chars 
	int* freq=nullptr;
	int size;
	calculate_freq(c, all_chars, freq, size);       //calculate the frequency of chars in all_chars and place it in freq
	node *nodes= new node[size];
	for (int i = 0; i < size; i++)
	{
		nodes[i].insert(freq[i], all_chars[i]);     //making nodes    
	}
	minheap heap(size);
	for (int i = 0; i < size; i++)
		heap.insert(&nodes[i]);                     //inserting nodes in minheap
	node* n1;
	node* n2;
	node* n3;
	BinaryTree tree;
	while (heap.get_totalcount() > 1)              
	{
		n1 = heap.getmin();
		heap.Delete();
		n2 = heap.getmin();
		heap.Delete();
		n3=tree.insert_both(n1, n2);
		heap.insert(n3);
	}
	//tree.LevelOrder();
	char f[50] = "tree.txt";                       //filename to save the tree
	tree.store(f);
	//cout<<strlen(f)
	char comp[50];
	cout << "Enter the name of compreesed version with .bin as extension: ";
	cin.getline(comp, 50);
	Encode(tree, c,comp);                          //encodes and saves the file
	cout << "SUCCESSFULLY COMPRESSED\n";
	
	BinaryTree tree2;                              //creating a sencond tree
	tree2.restore(f);                              //restoring all the data from "tree.txt" and constructing tree2
	char decomp[50];
	cout << "Enter the name of decompreesed version with .txt as extension: ";
	cin.getline(decomp, 50);
	Decode(tree2, comp,decomp);                    //using tree2 and compressed file to decode the file
	cout << "SUCCESSFULLY DECOMPRESSED";
	
}