#pragma once
#include "Item.h"

class LostFoundBook_7047
{
public:
    LostFoundBook_7047();           // 构造对象时自动读取数据文件

    // 菜单直接调用的功能
    void showLostItem_7047();       // 显示全部丢失物品记录
    void showFoundItem_7047();      // 显示全部捡到物品记录
    void addLostItem_7047();        // 登记一条新的丢失物品记录
    void addFoundItem_7047();       // 登记一条新的捡到物品记录
    void modifyItem_7047();         // 根据列表中显示的编号修改一条记录
    void delItem_7047();            // 根据列表中显示的编号删除一条记录
    void queryItem_7047();          // 按名称或地点关键词查询记录
    void matchItem_7047();          // 只显示匹配推荐，不改变记录状态
    void claimItem_7047();          // 捡到记录必选，丢失记录可选，由工作人员确认认领
    void clearBook_7047();          // 清空内存中的全部记录并保存
    void exitBook_7047();      // 退出前保存当前数据

private:
    // 只在类内部使用的辅助功能
    void loadBook_7047();           // 从数据文件中读取全部记录
    bool saveBook_7047();           // 保存成功返回true，失败返回false
    void showOneItem_7047(int pos); // 显示下标pos的记录，用户编号显示为pos+1
    int inputCategory_7047();       // 输入并检查类别编号，返回0～6
    void inputText_7047(const char prompt[], char value[], int capacity); // 安全读取一行文本
    bool isDateFormat_7047(char date[]); // 检查日期是否为YYYYMMDD格式
    bool isValidItem_7047(Item_7047 item); // 检查从文件读取的记录是否合法
    bool isRelated_7047(char a[], char b[]); // 判断两个字符串是否互相包含
    int getMatchScore_7047(int lostPos, int foundPos); // 计算两条记录的匹配分
    void claimItem_7047(int lostPos, int foundPos); // lostPos为-1时只完成捡到记录

    Item_7047 all_7047[MAX_ITEMS];       // 固定长度数组，存放全部记录
    int cnt_7047;                   // 当前实际记录数，不是最大容量
};
