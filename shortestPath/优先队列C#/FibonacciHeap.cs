using System;




    public class Node
    {
        Node parent;
        Node child;    //指向任意子女指针
        Node left;
        Node right;
        int value;
        int degree;
        bool marked;

        public Node()
        {
            degree = 0;
            mark = false;
            leftNode = this;
            rightNode = this;
            parent = null;
            child = null;
            left = null;
            right = null;
        }
    }


    public class FibonacciHeap
    {
        Node min;       //最小堆根节点
        int num;     

        public FibonacciHeap()
	    {
            this.min = null;
            this.num = 0;
            this.maxDegree = 0;
            maxDegreeNode = null;
	    }

        //堆结点x插入heap中  
        public void Insert(Node x)
        {
            if (this.num==0)
            {
                this.minNode = x;
            }
            else
            {
                FibNodeAdd(x, this.min);
                x.parentNode = null;
                if (x.value< this.minNode.value)
                {
                    this.minNode = x;
                }
            }
            this.num++;
        } 
 

        //将数组内的值插入Fibonacci Heap  
        void FibHeapInsertKeys(int[] keys, int keyNum)
        {  
           for (int i = 0; i < keyNum; i++) 
           {  
              FibHeapInsertKey(keys[i]);  
           }  
        }  
   
        //将值插入Fibonacci Heap  
        static void HeapInsertKey(int key)
        {
            Node x = new Node();
            x.value = key;
            FibHeapInsert(x);  
        }  

        public Node HeapExtractMin() 
        {
            Node x = null;  
            Node z=this.min;  
            if (z != null)
            {     
                //删除z的每一个孩子  
              while (z.child!=null) 
              {  
                x = z.child;  
                this.FibNodeRemove(x);  
                if (x.right == x) 
                {  
                    z.child = null;  
                } 
                else
                {  
                    z.child = x.right;  
                }  
                FibNodeAdd(x, z);                     //add x to the root list heap  
                x.parent = null;  
              }  
   
             this.FibNodeRemove(z); 
 
             if (z.rightNode == z)
             {  
                this.minNode = null;  
             }
             else
             {  
                this.minNode = z.rightNode;  
                FibHeapConsolidate();  
             }  
             this.num--;  
           }  
           return z;  
       }

        //合并左右相同度数的二项树  
        public  void FibHeapConsolidate() 
        {  
             int D, d;  
             Node w = this.minNode,x =null,y = null;
             
             double goldratio = (Math.sqrt(5) + 1) / 2.0;   //黄金分割率
             int maxdegree = (int)(Math.log(n) / Math.log(goldratio));  

             D = this.maxDegree + 1;  
             Node[]  A=new Node[D+1]();
             for (int i = 0; i < D+1; i++) 
             {  
                A[i]=null;  
             }  
   
             //合并相同度的根节点，使每个度数的二项树唯一  
            while (w!=null)
            {
                Node prev = w.left;
                Node next = w.right;
                x = w;  
                d = x.degree;

                while (A[d] != null) 
                {
                    y = A[d];
                    if (x.value > y.value)
                    {
                        x=FibHeapLink(x, y);
                    }
                    else
                    {
                        x=FibHeapLink(y, x);
                    }
                    A[d] = null;
                    d++;
                }
                A[d] = x;
                //remove w from root list  
                if (prev == w)
                {
                    //w is the only node in root list  
                    w = null; //set condition to exit while  
                }
                else
                {
                    prev.right = next;
                    next.left = prev;
                    w = next;
                }  
          
            }  
   
             heap.min = null;//原有根表清除  
            //将heap->cons中结点都重新加到根表中，且找出最小根  
            for (int i = 0; i < D; i++) 
            {        
                if(A[i]!=null)
                {
                   FibNodeAdd(A[i], this.min);
                
                }
            }
        }

         //x加到y旁边
         public void FibNodeAdd(Node x,Node y)
         {
            x.left = y.left;
            y.left.right= x;
            x.right = y;
            y.left= x;
         }
         
        //从根表中移除节点
         public void FibNodeRemove(Node x) 
         {
           x.left.right = x.right;
           x.right.left = x.left;
         }

        //将x根结点链接到y根结点
         public Node FibHeapLink(Node x, Node y)
         {          
            FibNodeRemove(x);
            if (y.child==null)
            {
                y.child = x;
            }
            else
            {
                FibNodeAdd(x, y.child);
            }
            x.parent = y;
            y.degree++;
            x.marked = false;
            return y;
        }

       //// //将堆的最小结点移出，并指向其右兄弟
       ////public  Node FibHeapMinRemove() 
       ////{
       ////     Node min = this.min;
       ////     if (this.min == min.rightNode)
       ////     {
       ////         this.minNode = null;
       ////     }
       ////     else
       ////     {
       ////         FibNodeRemove(min);
       ////         this.min = min.rightNode;
       ////     }
       ////     min.left = min.right = min;
       ////     return min;
       //// }
 

       //减小一个关键字
        public void FibHeapDecrease(Node x, int key) 
        {
            Node y = x.parent;
            if (x.value < key) {
                puts("new key is greater than current key!");
                exit(1);
            }
            x.value = key;
            if (y!=null&& x.value < y.value)
            {
                //破坏了最小堆性质,需要进行剪切操作
                FibHeapCut(x, y);
                FibHeapCascadingCut(y);   //如果父节点被标记过，则需要进行级联切断操作
            }
            if (x.value < this.min.value)
            {
                this.min = x;
            }
        }
 
        //切断x与父节点y之间的链接，使x成为一个根
        public void FibHeapCut(Node  x, Node  y) 
        {
            FibNodeRemove(x);
            renewDegree(y, x.degree);
            if (x == x.right)  //y只有x一个孩子节点
            {
                y.child = null;
            }
            else 
            {
                y.child = x.right;
            }
            x.parent = null;
            x.left = x.right= x;
            x.marked = false;
            FibNodeAdd(x,this.min);
        }
 
        //级联剪切
       public void FibHeapCascadingCut(Node y) 
       {
            Node  z = y.parent;
            if (z!=null) 
            {
                if (y.marked == false) 
                {
                    y.marked = true;
                }
                else  //
                {
                    FibHeapCut(y, z);
                    FibHeapCascadingCut(z);
                }
            }
        }
 
        //修改度数
        public  void renewDegree(Node  parent, int degree) 
        {
            parent.degree -= degree;
            if (parent.parent!= null)
            {
                renewDegree(parent.parent,degree);
            }
        }
 
        //删除结点
        void FibHeapDelete(Node  x) 
        {
            FibHeapDecrease(x, int.MinValue);
            FibHeapExtractMin();
        }
 
        //堆内搜索关键字,从minNode开始搜索
        public  Node  FibHeapSearch(int key) 
        {
           return FibNodeSearch(this.minNode, key);
        }
 
          //被FibHeapSearch调用
        public  Node  FibNodeSearch(Node x, int key) 
        {
          Node  w = x, y = null;
          if (x != null) 
          {
            do {
                 if (w.value == key) 
                     {
                        y = w;
                        break;
                     } 
                    else if (null!=(y = FibNodeSearch(w.childNode, key))) 
                    {
                        break;
                    }
                    w = w.rightNode;
              } 
            while (w != x);
           }
          return y;
        }
 
}  
   




  