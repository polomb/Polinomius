#ifndef __Black_Red_Tree__
#define __Black_Red_Tree__

template<typename Tkey, typename Tvalue>
class BlackRedTree
{
	struct Node
	{
		Tkey key;
        Tvalue value;
        char color;
		Node* left = nullptr;
		Node* right = nullptr;
        Node* parent = nullptr;
		//
        Node(Tkey k, Tvalue val) : key(k), value(val), color('r') {}
	};
	Node* root = nullptr;
    //
    void clear(Node* cur)
    {
        if (!cur) return;
        clear(cur->left);
        clear(cur->right);
        delete cur;
    }
    Node* copy(Node* otherNode, Node* parentNode = nullptr)
    {
        if (!otherNode) return nullptr;
        Node* newNode = new Node(otherNode->key);
        newNode->value = otherNode->value;
        newNode->color = otherNode->color;
        newNode->parent = parentNode;
        newNode->left = copy(otherNode->left, newNode);
        newNode->right = copy(otherNode->right, newNode);
        return newNode;
    }
    int height(Node* cur)
    {
        if (!cur) return 0;
        return 1 + std::max(height(cur->left), height(cur->right));
    }
    void print(Node* cur) const
    {
        if (!cur) return;
        print(cur->left);
        std::cout << cur->key << " ";
        print(cur->right);
    }
    Node* findNode(Node* cur, Tkey key)
    {
        if (!cur) return nullptr;
        if (cur->key == key) return cur;
        if (key < cur->key)
        {
            return findNode(cur->left, key);
        }
        else
        {
            return findNode(cur->right, key);
        }
    }
    Node* findMax(Node* cur)
    {
        if (!cur) return nullptr;
        while (cur->right)
        {
            cur = cur->right;
        }
        return cur;
    }
    Node* rotateLeft(Node* x)
    {
        if (!x || !x->right) return x;
        Node* y = x->right;
        x->right = y->left;
        if (y->left)
            y->left->parent = x;
        y->parent = x->parent;
        if (x->parent)
        {
            if (x->parent->left == x)
                x->parent->left = y;
            else
                x->parent->right = y;
        }
        y->left = x;
        x->parent = y;
        return y;
    }
    Node* rotateRight(Node* x)
    {
        if (!x || !x->left) return x;
        Node* y = x->left;
        x->left = y->right;
        if (y->right)
            y->right->parent = x;
        y->parent = x->parent;
        if (x->parent)
        {
            if (x->parent->left == x)
                x->parent->left = y;
            else
                x->parent->right = y;
        }
        y->right = x;
        x->parent = y;
        return y;
    }
    Node* P(Node* cur)
    {
        if (!cur || !cur->parent) return nullptr;
        return cur->parent;
    }
    Node* G(Node* cur)
    {
        if (!cur || !cur->parent) return nullptr;
        if (!cur->parent->parent) return nullptr;
        return cur->parent->parent;
    }
    Node* U(Node* cur)
    {
        if(!cur) return nullptr;
        if (!cur->parent) return nullptr;
        if (!cur->parent->parent) return nullptr;
        Node* grandparent = cur->parent->parent;
        Node* parent = cur->parent;
        if (grandparent->left == parent)
            return grandparent->right;
        else
            return grandparent->left;
    }
    void balanceInsert(Node* x)
    {
        if (x->parent == nullptr)
        {
            x->color = 'b';
            return;
        }
        if (P(x)->color == 'b') return;
        if (U(x) && U(x)->color == 'r')
        {
            P(x)->color = 'b';
            U(x)->color = 'b';
            G(x)->color = 'r';
            balanceInsert(G(x));
            return;
        }
        if (P(x) == G(x)->left)
        {
            if (x == P(x)->right)
                rotateLeft(P(x));
            P(x)->color = 'b';
            G(x)->color = 'r';
            rotateRight(G(x));
        }
        else
        {
            if (x == P(x)->left)
                rotateRight(P(x));
            P(x)->color = 'b';
            G(x)->color = 'r';
            rotateLeft(G(x));
        }
    }
    Node* insert(Node* cur, Tkey key, Tvalue val)
    {
        if (!cur) return new Node(key, val);
        if (cur->key == key) return cur;
        if (key < cur->key)
        {
            cur->left = insert(cur->left, key, val);
            if (cur->left)
                cur->left->parent = cur;
        }
        else
        {
            cur->right = insert(cur->right, key, val);
            if (cur->right)
                cur->right->parent = cur;
        }
        return cur;
    }
    void balanceErase(Node* x)
    {
        if (!x) return;
        if (x->parent == nullptr)
        {
            x->color = 'b';
            return;
        }
        if (!U(x)) return;
        if (U(x)->color == 'r')
        {
            P(x)->color = 'r';
            U(x)->color = 'b';
            if (x == P(x)->left)
                rotateLeft(P(x));
            else
                rotateRight(P(x));
        }
        if (!U(x)) return;
        if (P(x)->color == 'b' && U(x)->color == 'b' && (!U(x)->left || U(x)->left->color == 'b') && (!U(x)->right || U(x)->right->color == 'b'))
        {
            U(x)->color = 'r';
            balanceErase(P(x));
            return;
        }
        if (P(x)->color == 'r' && U(x)->color == 'b' && (!U(x)->left || U(x)->left->color == 'b') && (!U(x)->right || U(x)->right->color == 'b'))
        {
            U(x)->color = 'r';
            P(x)->color = 'b';
            return;
        }
        if (x == P(x)->left)
        {
            if (U(x)->left && U(x)->left->color == 'r' && (!U(x)->right || U(x)->right->color == 'b'))
            {
                U(x)->color = 'r';
                U(x)->left->color = 'b';
                rotateRight(U(x));
            }
            U(x)->color = P(x)->color;
            P(x)->color = 'b';
            U(x)->right->color = 'b';
            rotateLeft(P(x));
        }
        else
        {
            if (U(x)->right && U(x)->right->color == 'r' && (!U(x)->left || U(x)->left->color == 'b'))
            {
                U(x)->color = 'r';
                U(x)->right->color = 'b';
                rotateLeft(U(x));
            }
            U(x)->color = P(x)->color;
            P(x)->color = 'b';
            U(x)->left->color = 'b';
            rotateRight(P(x));
        }
    }
    Node* erase(Tkey key, Node* cur)
    {
        if (!cur) return nullptr;
        if (key == cur->key)
        {
            Node* max = findMax(cur->left);
            if (!max)
            {
                Node* right = cur->right;
                if (right) right->parent = cur->parent;
                delete cur;
                return right;
            }
            else
            {
                cur->key = max->key;
                cur->value = max->value;
                cur->left = erase(max->key, cur->left);
                if (cur->left) cur->left->parent = cur;
                return cur;
            }
        }
        if (key > cur->key)
        {
            cur->right = erase(key, cur->right);
            if (cur->right) cur->right->parent = cur;
            return cur;
        }
        else
        {
            cur->left = erase(key, cur->left);
            if (cur->left) cur->left->parent = cur;
            return cur;
        }
    }
public:
    BlackRedTree() = default;
    //5
    BlackRedTree(const BlackRedTree& other)
    {
        if (!other.empty())
        {
            root = copy(other.root);
        }
    }
    BlackRedTree& operator=(const BlackRedTree& other)
    {
        if (this != &other)
        {
            clear();
            if (!other.empty())
            {
                root = copy(other.root);
            }
        }
        return *this;
    }
    ~BlackRedTree()
    {
        clear();
    }
    BlackRedTree(BlackRedTree&& other) noexcept : root(other.root)
    {
        other.root = nullptr;
    }
    BlackRedTree& operator=(BlackRedTree&& other) noexcept
    {
        if (this != &other)
        {
            clear();
            root = other.root;
            other.root = nullptr;
        }
        return *this;
    }
    //
    void clear()
    {
        clear(root);
        root = nullptr;
    }
    bool empty() const
    {
        return (root == nullptr);
    }
    int height()
    {
        return height(root);
    }
    void print() const
    {
        print(root);
        std::cout << std::endl;
    }
    //
	class Iterator
	{
	public:
		Node* iter;
        //
		Iterator() = default;
		Iterator(Node* ptr) : iter(ptr) {}
        //
		Tkey& operator*() const
		{
			return iter->key;
		}
		Tkey* operator->() const
		{
			return &(iter->key);
		}
        //
        Iterator& operator++()
        {
            if (!iter) return *this;
            if (iter->right)
            {
                iter = iter->right;
                while (iter->left)
                {
                    iter = iter->left;
                }
            }
            else
            {
                Node* current = iter;
                Node* parent = iter->parent;
                while (parent && parent->right == current)
                {
                    current = parent;
                    parent = parent->parent;
                }
                iter = parent;
            }
            return *this;
        }
		Iterator operator++(int)
		{
			Iterator temp = *this;
			++(*this);
			return temp;
		}
        //
		bool operator==(const Iterator& other) const
		{
			return iter == other.iter;
		}
		bool operator!=(const Iterator& other) const
		{
			return iter != other.iter;
		}
        //
		operator Node* () const
		{
			return iter;
		}
	};
    Iterator begin()
    {
        if (!root) return Iterator(nullptr);
        Node* minNode = root;
        while (minNode->left)
        {
            minNode = minNode->left;
        }
        return Iterator(minNode);
    }
    Iterator end()
    {
        return Iterator(nullptr);
    }
    //
    Iterator find(const Tkey& key)
    {
        return Iterator(findNode(root, key));
    }
    Tvalue& operator[](const Tkey& key)
    {
        Iterator it = find(key);
        if (it != end())
            return it.iter->value;
        else
        {
            insert(key, Tvalue());
            return find(key).iter->value;
        }
    }
    Tvalue& at(const Tkey& key)
    {
        Iterator it = find(key);
        if (it != end())
            return it.iter->value;
        else
            throw std::out_of_range("-_-");
    }
    //
    Iterator insert(Tkey key, Tvalue val)
    {
        root = insert(root, key, val);
        Node* InsertedNode = findNode(root, key);
        if (InsertedNode)
        {
            balanceInsert(InsertedNode);
            while (root->parent)
            {
                root = root->parent;
            }
        }
        return Iterator(InsertedNode);
    }
    Iterator erase(Tkey key)
    {
        Node* deleteNode = findNode(root, key);
        if(!deleteNode)
            throw std::out_of_range("-_-");
        Node* deleteParent = deleteNode->parent;
        char deleteColor = deleteNode->color;
        root = erase(key, root);
        if (root)
        {
            if (deleteColor == 'b')
            {
                balanceErase(deleteParent);
            }
            while (root->parent)
            {
                root = root->parent;
            }
        }
        return Iterator(root);
    }
};

#endif

