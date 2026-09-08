#include <iostream>
#include "LostFoundBook.h"

using namespace std;

void showMenu()
{
    // 这个函数只负责显示菜单，不处理任何业务数据。
    cout << "\n****************************************" << endl;
    cout << "  校园失物招领匹配与管理系统" << endl;
    cout << "****************************************" << endl;
    cout << " 1. 登记丢失物品" << endl;
    cout << " 2. 登记捡到物品" << endl;
    cout << " 3. 查看全部物品" << endl;
    cout << " 4. 匹配推荐" << endl;
    cout << " 5. 确认认领" << endl;
    cout << " 6. 查询物品" << endl;
    cout << " 7. 修改物品" << endl;
    cout << " 8. 删除物品" << endl;
    cout << " 9. 清空全部记录" << endl;
    cout << " 0. 退出" << endl;
    cout << "****************************************" << endl;
    cout << "请选择[0-9]:";
}

int main()
{
    LostFoundBook_7047 book;       // 构造函数会自动执行loadBook_7047。
    bool running = true;

    while (running)
    {
        showMenu();
        int code;
        // 如果用户输入字母，cin会进入失败状态，必须清除后才能继续输入。
        if (!(cin >> code))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "请输入数字！" << endl;
            continue;
        }

        // switch只负责“菜单编号 -> 成员函数”的分发。
        switch (code)
        {
        case 1:
            book.addLostItem_7047();
            break;
        case 2:
            book.addFoundItem_7047();
            break;
        case 3:
            book.showLostItem_7047();
            book.showFoundItem_7047();
            break;
        case 4:
            book.matchItem_7047();
            break;
        case 5:
            book.claimItem_7047();
            break;
        case 6:
            book.queryItem_7047();
            break;
        case 7:
            book.modifyItem_7047();
            break;
        case 8:
            book.delItem_7047();
            break;
        case 9:
            book.clearBook_7047();
            break;
        case 0:
            book.exitBook_7047();
            running = false;
            break;
        default:
            cout << "输入无效，请重新选择！" << endl;
            break;
        }
    }
    return 0;
}
