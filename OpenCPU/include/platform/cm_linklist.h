/**
 *  @file    cm_linklist.h
 *  @brief   双端链表实现
 *  @copyright copyright © 2021 China Mobile IOT. All rights reserved.
 *  @author by ZXW
 *  @date 2021/4/28
 *  @date 2021/6/29,增加互斥锁(降低锁的粒度),by zxw
 */


#ifndef __CM_LINKLIST_H__
#define __CM_LINKLIST_H__


/** 双端链表元素定义*/
typedef struct _linklist_element_t
{
     void* content;                     /*!< 节点内容*/
     int size;                          /*!< content长度*/
     struct _linklist_element_t* pre;   /*!< 前驱节点*/
     struct _linklist_element_t* next;  /*!< 后继节点*/
} linklist_element_t;

/** 双端链表结构定义*/
typedef struct {
    linklist_element_t* head;           /*!< 表头*/
    linklist_element_t* tail;           /*!< 表尾*/
    int count;                          /*!< 节点个数*/
    int size;                           /*!< 链表占用内存*/
    void* mutex;                        /*!< 互斥锁，用于多线程时链表数据同步*/
} linklist_t;


/**
 *  \brief 链表节点content比较方法
 *  
 *  \param [in] src_content 源content 
 *  \param [in] dst_content 目标content
 *  \return 相等返回0，不相等返回其他值
 *  
 *  \details More details
 */
typedef int (*linklist_content_compare)(void* src_content, void* dst_content);

/**
 *  \brief 创建一个空的双端链表
 *  
 *  \return 返回双端链表指针
 *  
 *  \details More details
 */
linklist_t* linklist_create(void);

/**
 *  \brief 在表尾添加一个节点
 *  
 *  \param [in] list 链表
 *  \param [in] content 节点内容
 *  \param [in] size 节点长度
 *  \return 返回当前节点
 *  
 *  \details More details
 */
linklist_element_t* linklist_append_to_tail(linklist_t* list, void* content, int size);

/**
 *  \brief 在表头添加一个节点
 *  
 *  \param [in] list 链表
 *  \param [in] content 节点内容
 *  \param [in] size 节点长度
 *  \return 返回当前节点
 *  
 *  \details More details
 */
linklist_element_t* linklist_append_to_head(linklist_t* list, void* content, int size);

/**
 *  \brief 在index节点前插入一个节点
 *  
 *  \param [in] list 链表
 *  \param [in] index 被插入节点
 *  \param [in] content 节点内容
 *  \param [in] size 节点长度
 *  \return 返回当前节点
 *  
 *  \details More details
 */
linklist_element_t* linklist_insert_before_index(linklist_t* list, linklist_element_t* index, void* content, int size);

/**
 *  \brief 在index节点后插入一个节点
 *  
 *  \param [in] list 链表
 *  \param [in] index 被插入节点
 *  \param [in] content 节点内容
 *  \param [in] size 节点长度
 *  \return 返回当前节点
 *  
 *  \details More details
 */
linklist_element_t* linklist_insert_after_index(linklist_t* list, linklist_element_t* index, void* content, int size);

/**
 *  \brief 移除表头节点
 *  
 *  \param [in] list 链表
 *  \return 成功返回0，失败返回-1
 *  
 *  \details More details
 */
int linklist_remove_head(linklist_t* list);

/**
 *  \brief 移除表尾节点
 *  
 *  \param [in] list 链表
 *  \return 成功返回0，失败返回-1
 *  
 *  \details More details
 */
int linklist_remove_tail(linklist_t* list);

/**
 *  \brief 取出表头节点内容，不释放内存
 *  
 *  \param [in] list 链表
 *  \return 成功返回content，失败返回NULL
 *  
 *  \details More details
 */
void* linklist_pop_head(linklist_t* list);

/**
 *  \brief 取出表尾节点内容，不释放内存
 *  
 *  \param [in] list 链表
 *  \return 成功返回content，失败返回NULL
 *  
 *  \details More details
 */
void* linklist_pop_tail(linklist_t* list);

/**
 *  \brief 删除content对应的节点
 *  
 *  \param [in] list 链表
 *  \param [in] content 节点内容
 *  \param [in] cb 节点比较方法
 *  \return 成功返回0，失败返回-1
 *  
 *  \details More details
 */
int linklist_remove_content(linklist_t* list, void* content, linklist_content_compare cb);

/**
 *  \brief 删除节点
 *  
 *  \param [in] list 链表
 *  \param [in] element 节点
 *  \return 成功返回0，失败返回-1
 *  
 *  \details More details
 */
int linklist_remove_element(linklist_t* list, linklist_element_t* element);

/**
 *  \brief 正向迭代
 *  
 *  \param [in] list 链表
 *  \param [in] pos 当前位置
 *  \return 下一个节点
 *  
 *  \details 起始pos传入NULL，从表头迭代
 */
linklist_element_t* linklist_next_element(linklist_t* list, linklist_element_t** pos);

/**
 *  \brief 反向迭代
 *  
 *  \param [in] list 链表
 *  \param [in] pos 当前位置
 *  \return 前一个节点
 *  
 *  \details 起始pos传入NULL，从表尾迭代
 */
linklist_element_t* linklist_pre_element(linklist_t* list, linklist_element_t** pos);

/**
 *  \brief 根据content查找对应的节点
 *  
 *  \param [in] list 链表
 *  \param [in] content 节点内容
 *  \param [in] cb 节点比较方法
 *  \return 成功返回0，失败返回-1
 *  
 *  \details More details
 */
linklist_element_t* linklist_find_element(linklist_t* list, void* content, linklist_content_compare cb);

/**
 *  \brief 销毁链表
 *  
 *  \param [in] list 链表
 *  \return None
 *  
 *  \details 销毁后链表将置为NULL
 */
void linklist_destroy(linklist_t** list);

#endif
