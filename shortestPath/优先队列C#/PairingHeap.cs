 using System;

 public class PairingHeap
 {

        PairHeapNode root;


        /// <summary>
        /// ///////主要功能：  insert,merge,extract min,decrease key
        /// </summary>

        public  class PairHeapNode
        {
	        int	  key;
            PairingHeapNode child;        //最左边孩子
	        PairingHeapNode	sibling;
            PairingHeapNode prev;         //左兄弟，第一个孩子的左兄弟是父节点
            public PairHeapNode(int k) 
            {
                key = k;
                child = null;
                sibling = null;
                pre = null;
            }

        }

         public void PairHeap_insert(int k)   //添加key为k的node 
        {
            PairHeap_insert(new PairHeapNode(k));                
        }   

        public  void PairHeap_insert(PairHeap p)     //向堆中添加节点p
        {
	        if(p == null)
		        root=root;
	        else
		        root= merge_subheaps(this.root, p);
        }

        //静态函数版本
        public static PairHeapNode PairHeap_insert(PairHeapNode p,int key)  //向p中添加key值为key的点 
        {
            PairHeapNode node;  
            node.key = key;  
            node.child = node.sibling = node.prev = null;  
  
            if(p == null)  
                return node;  
            else  
                return merge_subheaps(p, node);  
        
        
        }

        public PairHeapNode merge_subheaps(PairHeapNode p)  //将root与p合并
        {
            this.root = this.merge_subheaps(this.root, p);  
        }

        public  static PairHeap merge_subheaps(PairHeap p, PairHeap q)   //两个子堆的合并
        {
	        if(q == null)
		        return p;
            if (p == null)
                return q;
            if (p.key <= q.key)        //q合并到p中
            {
                q.prev = p;
                p.sibling = q.sibling;          //这块不理解
                if (p.sibling != null)
                    p.sibling.prev = p;

                q.sibling = p.child;
                if (q.sibling != null)
                    q.sibling.prev = q;
                p.child = q;
                return p;
            }
            else                          //p合并到q中
            {
                //q.prev = p.prev;
                p.prev = q;
                p.sibling = q.child;
                if (p.sibling != null)
                    p.sibling.prev = p;

                q.child = p;
                return q;
            }
        }


        public void PairHeap_DecreaseKey(PairHeapNode pos, int newkey)
        {
	        if(newkey>=this.root.key)
		        return 0;

	        if(this.root == pos)
            {
                this.root.key = newkey;
		        return 0;
            }

	        if(pos.sibling != null)
		        pos.sibling.prev = pos.prev;

	        if(pos.prev.child == pos)
		        pos.prev.child = pos.sibling;
	        else
		        pos.prev.sibling = pos.sibling;

	        pos.sibling = null;
            pos.prev = null;

            return merge_subheaps(pos);
        }

        //得到最小点并生成新的堆
        public  PairHeapNode PairHeap_ExtractMin()
        {
	        PairHeapNode  p=this.root;

	        if(p == null)
		        return null;
	        else
	        {
                if (p.child != null)
                    this.root = combine_siblings(p.child);
                else
                    this.root = null;
	        }
	        return p;
        }

        //合并所有根节点的子节点

        private PairHeapNode  combine_siblings(PairHeapNode p)
        {
	        list<PairHeapNode> tree_array=new list<PairHeapNode>();
	        int i, count;

	        if(p.sibling == null)
		        return p;

	        for(count = 0; p != null; count++)
	        {
		        tree_array[count] = p;
		        p.prev.sibling= null;
		        p = p.sibling;
	        }
	        tree_array[count] = null;

	        for(i = 1; i < count; i++)
		        tree_array[i] = merge_subheaps(tree_array[i-1], tree_array[i]);

	        return tree_array[count-1];
        }

      



























	        
        }
