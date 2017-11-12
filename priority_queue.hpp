#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"



namespace sjtu {
    
    template<typename T, class Compare = std::less<T>>
    class priority_queue {
    private:
        //template<typename T, class Compare = std::less<T>>
        //先建立一个基本的节点类，left代表了这个点的最左边的儿子，next代表了这个点右兄弟，degree是度数，parent是这个点的父亲节点。
        struct priority_queueNode{
            T key;
            int degree;
            priority_queueNode *left;
            priority_queueNode *parent;
            priority_queueNode *next;
            
            priority_queueNode(T value, int deg = 0):key(value),degree(deg),left(NULL),parent(NULL),next(NULL){}
        };
        priority_queueNode *head;
        int Size; //Size纪录了二项堆中有几个节点，主要是方便最后计数。
        //    int f(int num){
        //      if(num == 1) return 0;
        //    else return 2 * f(num-1);
        //}
        //这个函数可以来计算总节点数目。
        void delete_priority_queue(priority_queueNode* head){
            if(head){
                //         std::cerr << "jkfjks" << std::endl;
                if(head->next) delete_priority_queue(head->next);
                if(head->left) delete_priority_queue(head->left);
                //          std::cerr << "jsekfjjfskfjks" << std::endl;
                delete head;
            }
        }
        // std::less<T>(a,b)
        priority_queueNode* copy(priority_queueNode* other){
            if(!other) {std::cerr << "hehehe" << std::endl; return NULL;}
            //    std::cerr << other->degree << std::endl;
            priority_queueNode* node = new priority_queueNode(other->key,other->degree);
            if(other->left) node->left = copy(other->left);
            if(other->next) node->next = copy(other->next);
            return node;
        }
        priority_queueNode* copytree(priority_queueNode* other){
            priority_queueNode* tmp = new priority_queueNode(other->key,other->degree);
            if(other->left) tmp->left = copy(other->left);
            return tmp;
        }
        void minimun(priority_queueNode* &y, priority_queueNode* &pre_y) const {
            priority_queueNode *x, *pre_x;
            if(!head) return;
            pre_x = head;
            x = head->next;
            pre_y = NULL;
            y = head;
            while(x){
                if(!Compare()(x->key,y->key)){
                    y = x;
                    pre_y = pre_x;
                }
                pre_x = x;
                x = x->next;
            }
        }
    public:
        priority_queue(){head = NULL; Size = 0;}
        
        ~priority_queue(){
            delete_priority_queue(head);
        };
        
        priority_queue<T, Compare> &operator=(const priority_queue<T, Compare> &other){
            if(other.head == head) return *this;
            delete_priority_queue(head);
            head = copy(other.head);
            Size = other.Size;
            return *this;
        }
        
        priority_queue(const priority_queue<T, Compare> &other){
            //     std::cerr << "sehf" <<std::endl;
            if(other.head) head = copy(other.head);
            Size = other.Size;
        }
        
        void link(priority_queueNode*h1, priority_queueNode*h2){
            h1->parent = h2;
            h1->next = h2->left;
            h2->left = h1;
            h2->degree++;
            //      std::cerr << "h2->degree" << std::endl;
        }
        

        priority_queueNode* merge(priority_queueNode*h1, priority_queueNode*h2){
            //  std::cerr << "fuckf" << std::endl;
            //       if(h1 && h2) std::cerr << "h1->key = " << h1->key << " " << "h2->key = " << h2->key << std::endl;
            priority_queueNode *tmp = NULL, *prev = NULL, *root = NULL;
            int num = 0;
            while(h1 && h2){
                if(h1->degree < h2->degree){
                    prev = tmp;
                    tmp = h1;
                    if (prev)
                        prev->next = tmp;
                    h1 = h1->next;
                }
                else{
                    prev = tmp;
                    tmp = h2;
                    if (prev)
                        prev->next = tmp;
                    h2 = h2->next;
                }
                if(!num) root = tmp;
                num++;
            }
            if(h1)
            {
                prev = tmp;
                tmp = h1;
                if (prev)
                    prev->next = tmp;
            }
            if(h2)
            {
                prev = tmp;
                tmp = h2;
                if (prev)
                    prev->next = tmp;
            }
            if(!num) root = tmp;
            //      if(root->next) std::cerr << "root->next = " << root->next->key << std::endl;
            return root;
        }
        
        
        
        priority_queueNode* combine(priority_queueNode*h1, priority_queueNode*h2){
            //      if(h1 && h2)std::cerr << "h1->key = " << h1->key << " " << "h2->key = " << h2->key << std::endl;
            priority_queueNode* head = merge(h1, h2);
            //    if(head) std::cerr << "head = " << head->key << std::endl;
            //   if(head->next) std::cerr << "head->next = " << head->next->key << std::endl;
            // if(head->next && head->next->next) std::cerr << "fuck" <<std::endl;
            priority_queueNode *pre_x, *x, *next_x;
            if(!head) return NULL;
            pre_x = NULL;
            x = head;
            next_x = x->next;
            //    priority_queueNode* tmp = head;
            while(next_x!=NULL){
                if(x->degree != next_x->degree || (next_x->next != NULL && next_x->degree == next_x->next->degree)){
                    pre_x = x;
                    x = next_x;
                }
                else if(!Compare()(x->key,next_x->key)){
                    x->next = next_x->next;
                    link(next_x,x);
                }
                else{
                    if(!pre_x){
                        head = next_x;
                    }
                    else{
                        pre_x->next = next_x;
                    }
                    link(x,next_x);
                    x = next_x;
                }
                next_x = x->next;
            }
            //      if(head->left) std::cerr << "head->left = " << head->left->key << std::endl;
            //      std::cerr << head->key << std::endl;
            return head;
        }
        
        void combine (priority_queue &other){
            if(other.head){
                head = combine(head, other.head);
                other.head = NULL;
                Size += other.Size;
            }
        }
        //一开始感觉直接扫一遍得到最小值就可以，后来发现自己naive（pop的时候不会写了。。），这里存下最小key的前一个节点，是为了方便删除这个节点。。。
        T& Minkey() const {
            if(!head) throw container_is_empty();
            priority_queueNode *y, *pre_y;
            minimun(y,pre_y);
            return y->key;
        }
        
        const T & top() const {
            //      std::cerr << "hehehe" << std::endl;
            return  Minkey();
        }
        
        void push(const T &e) {
            //     std::cerr << "ha" << std::endl;
            
            //       if(head)std::cerr << "head->key = " << head->key << std::endl;
            
            priority_queueNode *tmp = new priority_queueNode(e);
            
            //   std::cerr << "tmp->key = " << tmp->key << std::endl;
            
            // if(head)std::cerr << "head->key = " << head->key << std::endl;
            
            //  std::cerr << "ha" << std::endl;
            head = combine(head, tmp);
            //  if(head)std::cerr << "head->key = " << head->key << std::endl;
            //  if(head->left) std::cerr << "head->left = " << head->left->key << std::endl;
            //  if(!head->next && !head->left)  std::cerr << "fjskfjklskfls" << std::endl;
            ++Size;
        }
        
        void pop() {
            if(!head) throw container_is_empty();
            priority_queueNode *y, *pre_y;
            minimun(y, pre_y);
            if(!pre_y) head = head->next; //删除这个节点
            else pre_y->next = y->next;
            //       priority_queueNode<T,Compare> *other = reverse(y->left); //把删掉最小节点下面的子树重新挂到链表上。
            //     head = combine(head,child);
            priority_queueNode *p = y->left; //依次合并这个节点的各科子树。
            while(p != NULL){
                //        std::cerr << "travel " << p->key << std::endl;
                priority_queueNode* next = p->next;
                p->next = NULL;
                head = combine(head, p);
                p = next;
                //         std::cerr << "finish" << std::endl;
                
            }
            delete y;
            Size--;
        }
        size_t size() const {
            return (size_t) Size;
        }
        bool empty() const {
            return Size == 0;
        }
        void print(){
            priority_queueNode *p;
            if(head == NULL) {return;}
            std::cerr << "== 二项堆(";
            p = head;
            while(p != NULL){
                std::cerr << "B" << p->degree << "";
                p = p->next;
            }
            std::cerr << ")的详细信息" << std::endl;
        }
    };
    
}

#endif