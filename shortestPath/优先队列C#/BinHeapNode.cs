using System;
/// <summary>
/// 二项堆用法   主要功能  Insert,delete,extractMin,decreasekey,link等 
/// </summary>



public class BinHeapNode
{
    public int key;
    public int degree;
    public BinHeapNode parent;
    public BinHeapNode leftChild;
    public BinHeapNode sibling;

    public BinHeapNode()
    {
        degree = 0;
        parent = null;
        sibling = null;
        leftChild = null;
    }

    public BinHeapNode(int k)
    {
        key = k;
        degree = 0;
        parent = null;
        sibling = null;
        leftChild = null;
    }
}


public class BinHeap
{
      //根表头结点

      BinHeapNode head;

      public BinHeap() 
      {
          this.head = null;
          this.min = null;
      }
      
      //用数组初始化堆
      public BinHeap(int[] keys,int n)
     {
        BinHeapNode newHeap = new BinHeapNode();

        for (int i = 0; i < n; i++)
        {           
            newHeap.key = keys[i];
            if (this.head==null)
            {
                this.heap = newHeap;
            }
            else
            {
                this.head = BinHeapUnion(this.head, newHeap);
                newHeap = null;
            }
         }
     }
     

    //插入值
      public void BinHeapInsert(int k) 
      {
          BinHeapNode newHeap = new BinHeapNode(k);
            if (this.head==null)
            {
                this.heap = newHeap;
            }
            else
            {
                this.head = BinHeapUnion(this.head, newHeap);
                newHeap = null;
            }
      
      }


    //返回最小根节点
    public  BinHeapNode BinHeapMin()
    {
        BinHeapNode y = null, x = this.head;
        int min = int.MaxValue;
        while (x != null) {
            if (x.key < min) 
            {
                min = x.key;
                y = x;
            }
            x = x.sibling;
        }
        return y;
    }


    //抽取有最小关键字的结点,并删除得到更新后的堆
 
    public  BinHeapNode BinHeapExtractMin() 
   {  
        BinHeapNode pre_y = null, y = null, x = this.head;  
        int min =int.MaxValue;  
        while (x != null) 
        {  
            if (x.key < min) 
            {  
                min = x.key;  
                pre_y = y;  
                y = x;  
            }  
            x = x.sibling;  
        }  
   
        if (y==null)
        {  
            return null;  
        }  
   
        if (pre_y == null)
        {  
            this.head =this.head.sibling;  
        } 
        else 
        {  
            pre_y.sibling = y.sibling;  
        }  
   
        //将y的子结点指针顺序反转 
        BinHeapNode H2 = null, p = null;  
        x = y.leftChild;  
        while (x != null)
        {  
            p = x;            
            p.sibling = H2;  
            H2 = p;  
            p.parent = null;
            x = x.sibling;  
        }  
        
        //根据两个头指针进行合并，两个子堆进行合并
        this.head = BinHeapUnion(this.head, H2);  
    }
  

      //两个堆合并  
      public  BinHeap BinHeapUnion(BinHeapNode H1, BinHeapNode H2) 
     {  
            BinHeapNode heap = null, pre_x =null, x = null, next_x = null;    //heap代表新的head
            heap = BinHeapMerge(H1, H2);  
            if (heap == null)
            {  
                return heap;  
            }  
   
            pre_x = null;  
            x = heap;  
            next_x = x.sibling;  
   
            while (next_x != null)
            {  
                if ((x.degree != next_x.degree) ||//Cases 1 and 2  
                    ((next_x.sibling != null) && (next_x.degree == next_x.sibling.degree))) 
                {  
                        pre_x = x;  
                        x = next_x;  
                }
                else if (x.key <= next_x.key)
                {//Cases 3  
                    x.sibling = next_x.sibling;  
                    BinLink(next_x, x);  
                }
                else
                {//Cases 4  
                    if (pre_x == null) 
                    {  
                        heap = next_x;  
                    } 
                    else 
                    {  
                        pre_x->sibling = next_x;  
                    }  
                    BinLink(x, next_x);  
                    x = next_x;  
                }  
                next_x = x->sibling;  
            }  
            return heap;  
        }  
   
       //将H1, H2的根表合并成一个按度数的单调递增次序排列的链表  
       public  BinHeapNode BinHeapMerge(BinHeapNode H1, BinHeapNode H2) 
       {  
            //heap->堆的首地址，H3为指向新堆根结点  
            BinHeapNode heap = null, firstHeap = null, secondHeap = null,  
                pre_H3 = null, H3 = null;  
   
            if (H1 != null&& H2 != null)
            {  
                firstHeap = H1;  
                secondHeap = H2;  
                //整个while，firstHeap, secondHeap, pre_H3, H3都在往后顺移  
                while (firstHeap != null && secondHeap != null) 
                {  
                    if (firstHeap.degree <= secondHeap.degree) 
                    {  
                        H3 = firstHeap;  
                        firstHeap = firstHeap.sibling;  
                    }
                    else
                    {  
                        H3 = secondHeap;  
                        secondHeap = secondHeap.sibling;  
                    }  
   
                    if (pre_H3 == null)
                    {  
                        pre_H3 = H3;  
                        heap = H3;             //新的head节点  
                    }
                    else
                    {  
                        pre_H3.sibling = H3;  
                        pre_H3 = H3;  
                    }  
                    if (firstHeap != null) 
                    {  
                        H3.sibling = firstHeap;  
                    }
                    else
                    {  
                        H3.sibling = secondHeap;  
                    }  
                }//while  
          } 
            else if (H1 != null) 
            {  
                heap = H1;  
            }
            else 
            {  
                heap = H2;  
            }  
            H1 = H2 = null;  
            return heap;  
        }  
   
        //使H2成为H1的父节点  
        public  void BinLink(BinHeapNode H1, BinHeapNode H2) 
        {  
            H1.parent = H2;  
            H1.sibling = H2.leftChild;  
            H2.leftChild = H1;  
            H2.degree++;  
        }


        //减少关键字的值  
        public void BinHeapDecreaseKey(BinHeapNode x, int key)
        {
            if (key >= x.key)
            {
                exit(1); //不为降键  
            }
            x.key = key;

            BinHeapNode z = null, y = null;
            y = x;
            z = x.parent;
            while (z != null && z.key > y.key)
            {
                swap(z.key, y.key);
                y = z;
                z = y.parent;
            }
        }
        
        //交换数字
        public void swap(ref int num1,ref int num2) 
        {
            int z = num1;
            num1 = num2;
            num2 = z;        
        }


        public void BinHeapDelete(BinHeapNode x)
        {
            if (x != null)
            {
                BinHeapDecreaseKey(x, x.Minvalue);
                this.head=BinHeapExtractMin(heap);
            }  
                
        }
       
        //删除一个关键字,转换为最小值进行删除
        public BinHeapNode BinHeapDelete(int key) 
       {  
            BinHeapNode x = null;  
            x = BinHeapFind(heap, key);  
            if (x != null) 
            {  
                BinHeapDecreaseKey(x,x.Minvalue);  
                return BinHeapExtractMin(heap);  
            }  
            return x;  
        }  
   
        //找出一个关键字  
       public  BinHeapNode BinHeapFind(int key) 
       {  
            Position p = null, x = null;  
            p = this.head;  
            while (p != null) 
            {  
                if (p.key == key)
                {  
                    return p;  
                }
                else
                {  
                    if((x =BinHeapFind(p.leftChild, key)) != null)
                    {  
                        return x;  
                    }  
                    p = p.sibling;  
                }  
            }  
            return null;  
       }  

}
