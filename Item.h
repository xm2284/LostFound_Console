#pragma once

#define MAX_ITEMS 100

struct Item_7047
{
    int type_7047;                  // 0=丢失，1=捡到
    char name_7047[40];             // 物品名称
    int category_7047;              // 类别编号0~6
    char description_7047[200];     // 颜色、品牌等特征
    char location_7047[60];         // 丢失或捡到地点
    char date_7047[9];              // YYYYMMDD，8个数字加结尾\0
    char personName_7047[20];       // 失主或上交人姓名
    char phone_7047[20];            // 联系电话
    int status_7047;                // 0=待处理，1=已完成
};
