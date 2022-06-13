#include <iostream>
#include <vector>

using namespace std;

template<typename T>
class SegmentTree
{
    friend ostream& operator << (ostream& os, const SegmentTree& aSegmentTree) // перегрузка оператора вывода
    {
      os << "-------------------------------------------\n";
      for (size_t i = 1; i < aSegmentTree.tree.size(); ++i)
      {
        os << "at " << i << "st vertex " << aSegmentTree.tree[i];
        if (i < aSegmentTree.treePower)
        {
          os << " children " << aSegmentTree.tree[i * 2] << " " << aSegmentTree.tree[i * 2 + 1];
        }
        os << endl;
      }
      return os;
    }
    public:
        SegmentTree(const vector<T>& v) // конструктор дерева
        {
            size_t arrSize = v.size();
            treePower = 1; // номер первой першины последнего слоя
            while (treePower < arrSize) {
                treePower *= 2;
            }
            tree.resize(treePower * 2);
            subTree.resize(treePower * 2);
            for (size_t i = 0; i < arrSize; ++i) 
            {
                tree[i + treePower] = v[i];
            }
            for (int i = treePower - 1; i > 0; --i) 
            {
                tree[i] = tree[i * 2] + tree[i * 2 + 1];
            }   
        }
        inline void relax(int v)
        {
          tree[v] += subTree[v];
          if (v < (int)treePower)
          {
            subTree[v * 2] += subTree[v] / 2;
            subTree[v * 2 + 1] += subTree[v] / 2;
          }
          subTree[v] = 0;
        }
        inline T getSum(int l, int r) // для обращения из вне
        {
          return getSum(l, r, 0, treePower - 1, 1);
        }
        T getSum(int l, int r, int tl, int tr, int v) // получить сумму на отрезке
        { 
          relax(v);
          if (r == tr && l == tl)
          {
            return tree[v];
          }
          int sum = 0;
          int tm = (tl + tr) / 2;
          if (l <= tm)
          {
            sum += getSum(l, min(tm, r), tl, tm, v * 2);
          }
          if (r > tm)
          {
            sum += getSum(max(tm + 1, l), r, tm + 1, tr, v * 2 + 1);
          }
          return sum;
        }
        
        inline void addOnSeg(int l, int r, T add) // для обращения из вне
        {
          return addOnSeg(l, r, add, 0, treePower - 1, 1);
        }
        void addOnSeg(int l, int r, T add, int tl, int tr, int v)  // прибавить на отрезке
        {   
          relax(v);
          if (tr == r && tl == l)
          {
            subTree[v] = (r - l + 1) * add;
            relax(v);
            return;
          }
          int tm = (tl + tr) / 2;
          if (l <= tm)
          {
            addOnSeg(l, min(tm, r), add, tl, tm, v * 2);
          }
          if (r > tm)
          {
            addOnSeg(max(tm + 1, l), r, add, tm + 1, tr, v * 2 + 1);
          }
          tree[v] = tree[v * 2] + tree[v * 2 + 1];
        }
    private:
        size_t treePower;
        vector<T> tree;
        vector<T> subTree;
};

int main() 
{
  ios_base::sync_with_stdio(0); cin.tie(0);
  int n; cin >> n;
  vector<long long> v(n);
  for (long long& i : v) cin >> i;
  SegmentTree<long long> myTree(v);
  int q; cin >> q;
  while (q--)
  {
    char op; cin >> op;
    switch (op)
    {
    case 'g':
      int num; cin >> num; num--;
      cout << myTree.getSum(num, num) << " ";
      break;
    case 'a':
      int l, r, x; cin >> l >> r >> x; l--; r--;
      myTree.addOnSeg(l, r, x);
    default:
      break;
    }
  }
  // cout << myTree;
  // myTree.addOnSeg(0, 3, 1);
  // cout << myTree;
  // myTree.addOnSeg(0, 3, 1);
  // myTree.addOnSeg(0, 3, 1);
  // cout << myTree;
  // cout << myTree.getSum(0, 0);
  return 0;
}
