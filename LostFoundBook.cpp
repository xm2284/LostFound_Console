#include "LostFoundBook.h"
#include <iostream>
#include <fstream>
#include <string.h>
#include <string>
#include <cctype>
#include <windows.h>

using namespace std;

char g_categoryNames[7][20] = {
    "电子产品", "证件卡片", "书籍文具", "衣物饰品",
    "生活用品", "运动器材", "其他"
};

// 数据文件固定放在exe旁边，避免从不同目录启动时出现多份数据。
static string getDataFilePath()
{
    char modulePath[MAX_PATH] = { 0 };
    GetModuleFileNameA(NULL, modulePath, MAX_PATH);
    string path = modulePath;
    size_t pos = path.find_last_of("\\/");
    if (pos != string::npos)
        path = path.substr(0, pos + 1);
    return path + "lostfound_console.dat";
}

static int getCharacterCount(const char text[])
{
    int length = MultiByteToWideChar(CP_ACP, 0, text, -1, NULL, 0);
    if (length <= 0)
        return 0;
    return length - 1;
}

LostFoundBook_7047::LostFoundBook_7047()
{
    cnt_7047 = 0;
    loadBook_7047(); // 启动程序时恢复上次保存的数据
}

void LostFoundBook_7047::loadBook_7047()
{
    cnt_7047 = 0;
    ifstream infile(getDataFilePath(), ios::in | ios::binary);

    // 兼容旧版本：exe旁没有数据时，再尝试原来的工作目录。
    bool loadedLegacyFile = false;
    if (!infile)
    {
        infile.clear();
        infile.open("lostfound_console.dat", ios::in | ios::binary);
        loadedLegacyFile = infile.good();
    }
    if (!infile)
        return;

    infile.seekg(0, ios::end);
    streamoff fileSize = infile.tellg();
    if (fileSize < 0 || fileSize % sizeof(Item_7047) != 0 ||
        fileSize / sizeof(Item_7047) > MAX_ITEMS)
    {
        cout << "数据文件损坏，已停止加载！" << endl;
        infile.close();
        return;
    }
    infile.seekg(0, ios::beg);

    int fileCount = (int)(fileSize / sizeof(Item_7047));
    for (int i = 0; i < fileCount; i++)
    {
        if (!infile.read((char*)&all_7047[i], sizeof(Item_7047)))
        {
            cnt_7047 = 0;
            cout << "数据文件读取失败！" << endl;
            infile.close();
            return;
        }

        // 损坏文件可能没有字符串结尾，先强制补\0再校验字段。
        all_7047[i].name_7047[39] = '\0';
        all_7047[i].description_7047[199] = '\0';
        all_7047[i].location_7047[59] = '\0';
        all_7047[i].date_7047[8] = '\0';
        all_7047[i].personName_7047[19] = '\0';
        all_7047[i].phone_7047[19] = '\0';
        if (!isValidItem_7047(all_7047[i]))
        {
            cnt_7047 = 0;
            cout << "数据文件内容不合法，已停止加载！" << endl;
            infile.close();
            return;
        }
        cnt_7047++;
    }
    infile.close();

    // 旧数据加载成功后保存到exe旁，以后从任意目录启动都使用同一份数据。
    if (loadedLegacyFile)
        saveBook_7047();
}

bool LostFoundBook_7047::saveBook_7047()
{
    string filePath = getDataFilePath();
    string tempPath = filePath + ".tmp";
    ofstream outfile(tempPath, ios::out | ios::binary);
    if (!outfile)
    {
        cout << "文件保存失败！" << endl;
        return false;
    }

    // 只保存0到cnt_7047-1这些有效元素，数组后面的空位置不写入文件。
    for (int i = 0; i < cnt_7047; i++)
        outfile.write((char*)&all_7047[i], sizeof(Item_7047));

    bool success = outfile.good();
    outfile.close();
    if (!success || !MoveFileExA(tempPath.c_str(), filePath.c_str(),
        MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
    {
        DeleteFileA(tempPath.c_str());
        cout << "文件保存失败！" << endl;
        return false;
    }
    return true;
}

void LostFoundBook_7047::showOneItem_7047(int pos)
{
    // 数组下标从0开始，但给用户看的编号从1开始，更符合使用习惯。
    cout << "编号:" << pos + 1;
    if (all_7047[pos].type_7047 == 0)
        cout << "  类型:丢失";
    else
        cout << "  类型:捡到";

    if (all_7047[pos].status_7047 == 0)
        cout << "  状态:待处理";
    else
        cout << "  状态:已完成";
    cout << endl;

    cout << "名称:" << all_7047[pos].name_7047;
    cout << "  类别:";
    if (all_7047[pos].category_7047 >= 0 && all_7047[pos].category_7047 <= 6)
        cout << g_categoryNames[all_7047[pos].category_7047];
    else
        cout << g_categoryNames[6];
    cout << "  日期:" << all_7047[pos].date_7047 << endl;

    cout << "描述:" << all_7047[pos].description_7047
        << "  地点:" << all_7047[pos].location_7047 << endl;

    cout << "姓名:" << all_7047[pos].personName_7047
        << "  电话:" << all_7047[pos].phone_7047 << endl;
}

void LostFoundBook_7047::showLostItem_7047()
{
    bool found = false;
    cout << "\n********** 丢失物品 **********" << endl;
    for (int i = 0; i < cnt_7047; i++)
    {
        if (all_7047[i].type_7047 == 0)
        {
            showOneItem_7047(i);
            found = true;
        }
    }
    if (!found)
        cout << "暂无丢失物品记录。" << endl;
}

void LostFoundBook_7047::showFoundItem_7047()
{
    bool found = false;
    cout << "\n********** 捡到物品 **********" << endl;
    for (int i = 0; i < cnt_7047; i++)
    {
        if (all_7047[i].type_7047 == 1)
        {
            showOneItem_7047(i);
            found = true;
        }
    }
    if (!found)
        cout << "暂无捡到物品记录。" << endl;
}

int LostFoundBook_7047::inputCategory_7047()
{
    int category;
    while (true)
    {
        cout << "类别(0电子产品 1证件卡片 2书籍文具 3衣物饰品 "
            << "4生活用品 5运动器材 6其他):";

        if (cin >> category && category >= 0 && category <= 6)
            return category;

        cin.clear();
        cin.ignore(10000, '\n');
        cout << "类别必须是0到6之间的数字，请重新输入！" << endl;
    }
}

void LostFoundBook_7047::inputText_7047(const char prompt[], char value[], int capacity)
{
    while (true)
    {
        cout << prompt;
        cin >> ws;
        cin.getline(value, capacity);
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "输入过长，最多允许" << capacity - 1 << "个字节，请重新输入！" << endl;
            continue;
        }

        // 去掉行尾空格，防止只输入空格也被当作有效内容。
        int length = (int)strlen(value);
        while (length > 0 && isspace((unsigned char)value[length - 1]))
            value[--length] = '\0';
        if (length == 0)
        {
            cout << "内容不能为空，请重新输入！" << endl;
            continue;
        }
        return;
    }
}

bool LostFoundBook_7047::isDateFormat_7047(char date[])
{
    if (strlen(date) != 8)
        return false;

    for (int i = 0; i < 8; i++)
    {
        if (date[i] < '0' || date[i] > '9')
            return false;
    }

    int year = (date[0] - '0') * 1000 + (date[1] - '0') * 100 +
        (date[2] - '0') * 10 + date[3] - '0';
    int month = (date[4] - '0') * 10 + date[5] - '0';
    int day = (date[6] - '0') * 10 + date[7] - '0';
    if (month < 1 || month > 12)
        return false;

    int daysOfMonth[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };
    bool leapYear = (year % 400 == 0) || (year % 4 == 0 && year % 100 != 0);
    if (leapYear)
        daysOfMonth[1] = 29;
    return day >= 1 && day <= daysOfMonth[month - 1];
}

bool LostFoundBook_7047::isValidItem_7047(Item_7047 item)
{
    if (item.type_7047 < 0 || item.type_7047 > 1 ||
        item.category_7047 < 0 || item.category_7047 > 6 ||
        item.status_7047 < 0 || item.status_7047 > 1)
        return false;
    if (item.name_7047[0] == '\0' || item.description_7047[0] == '\0' ||
        item.location_7047[0] == '\0' || item.personName_7047[0] == '\0' ||
        item.phone_7047[0] == '\0')
        return false;
    return isDateFormat_7047(item.date_7047);
}

void LostFoundBook_7047::addLostItem_7047()
{
    if (cnt_7047 >= MAX_ITEMS)
    {
        cout << "记录已满，不能继续添加！" << endl;
        return;
    }

    Item_7047 item = {}; // 先把整个结构体清零，避免字符数组中出现垃圾数据。
    item.type_7047 = 0;
    item.status_7047 = 0; // 新登记的记录默认等待处理。

    inputText_7047("名称:", item.name_7047, 40);
    item.category_7047 = inputCategory_7047();
    inputText_7047("特征描述:", item.description_7047, 200);
    inputText_7047("丢失地点:", item.location_7047, 60);
    do
    {
        inputText_7047("丢失日期(YYYYMMDD):", item.date_7047, 9);
        if (!isDateFormat_7047(item.date_7047))
            cout << "日期无效，请输入真实的8位日期，例如20260716！" << endl;
    } while (!isDateFormat_7047(item.date_7047));
    inputText_7047("失主姓名:", item.personName_7047, 20);
    inputText_7047("联系电话:", item.phone_7047, 20);

    all_7047[cnt_7047] = item; // 尾部追加：新记录放到第cnt_7047个位置。
    cnt_7047++;           // 实际记录数增加1。
    if (!saveBook_7047())
    {
        cnt_7047--;
        return;
    }
    cout << "登记丢失物品成功！" << endl;
}

void LostFoundBook_7047::addFoundItem_7047()
{
    if (cnt_7047 >= MAX_ITEMS)
    {
        cout << "记录已满，不能继续添加！" << endl;
        return;
    }

    Item_7047 item = {}; // 先清零，再逐个接收用户输入。
    item.type_7047 = 1;
    item.status_7047 = 0;

    inputText_7047("名称:", item.name_7047, 40);
    item.category_7047 = inputCategory_7047();
    inputText_7047("特征描述:", item.description_7047, 200);
    inputText_7047("捡到地点:", item.location_7047, 60);
    do
    {
        inputText_7047("捡到日期(YYYYMMDD):", item.date_7047, 9);
        if (!isDateFormat_7047(item.date_7047))
            cout << "日期无效，请输入真实的8位日期，例如20260716！" << endl;
    } while (!isDateFormat_7047(item.date_7047));
    inputText_7047("上交人姓名(不详请输入无):", item.personName_7047, 20);
    inputText_7047("联系电话(不详请输入无):", item.phone_7047, 20);

    all_7047[cnt_7047] = item;
    cnt_7047++;
    if (!saveBook_7047())
    {
        cnt_7047--;
        return;
    }
    cout << "登记捡到物品成功！" << endl;
}

void LostFoundBook_7047::modifyItem_7047()
{
    showLostItem_7047();
    showFoundItem_7047();

    if (cnt_7047 == 0)
    {
        cout << "当前没有可以修改的记录。" << endl;
        return;
    }

    int number;
    cout << "请输入要修改的编号:";
    if (!(cin >> number))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "编号必须是数字！" << endl;
        return;
    }

    // 用户编号从1开始，减1后才是数组下标。
    int pos = number - 1;
    if (pos < 0 || pos >= cnt_7047)
    {
        cout << "编号不存在，请输入上面列表中显示的编号！" << endl;
        return;
    }

    // 先复制原记录，因此没有重新赋值的type和status可以保留下来。
    Item_7047 item = all_7047[pos];
    inputText_7047("新名称:", item.name_7047, 40);
    item.category_7047 = inputCategory_7047();
    inputText_7047("新特征描述:", item.description_7047, 200);
    inputText_7047("新地点:", item.location_7047, 60);
    do
    {
        inputText_7047("新日期(YYYYMMDD):", item.date_7047, 9);
        if (!isDateFormat_7047(item.date_7047))
            cout << "日期无效，请输入真实的8位日期，例如20260716！" << endl;
    } while (!isDateFormat_7047(item.date_7047));
    inputText_7047("新姓名:", item.personName_7047, 20);
    inputText_7047("新电话:", item.phone_7047, 20);

    Item_7047 oldItem = all_7047[pos];
    all_7047[pos] = item;
    if (!saveBook_7047())
    {
        all_7047[pos] = oldItem;
        return;
    }
    cout << "修改成功！" << endl;
}

void LostFoundBook_7047::delItem_7047()
{
    showLostItem_7047();
    showFoundItem_7047();

    if (cnt_7047 == 0)
    {
        cout << "当前没有可以删除的记录。" << endl;
        return;
    }

    int number;
    cout << "请输入要删除的编号:";
    if (!(cin >> number))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "编号必须是数字！" << endl;
        return;
    }

    int pos = number - 1;
    if (pos < 0 || pos >= cnt_7047)
    {
        cout << "编号不存在，请输入上面列表中显示的编号！" << endl;
        return;
    }

    Item_7047 deletedItem = all_7047[pos];
    // 删除数组中间元素：后面的每一条都向左移动一格。
    for (int i = pos; i < cnt_7047 - 1; i++)
        all_7047[i] = all_7047[i + 1];

    cnt_7047--;
    if (!saveBook_7047())
    {
        for (int i = cnt_7047; i > pos; i--)
            all_7047[i] = all_7047[i - 1];
        all_7047[pos] = deletedItem;
        cnt_7047++;
        return;
    }
    cout << "删除成功！" << endl;
}

void LostFoundBook_7047::queryItem_7047()
{
    char keyword[60];
    inputText_7047("请输入名称或地点关键词:", keyword, 60);

    bool found = false;
    for (int i = 0; i < cnt_7047; i++)
    {
        // strstr不为NULL表示字段中包含关键词，不要求完全相等。
        if (strstr(all_7047[i].name_7047, keyword) != NULL ||
            strstr(all_7047[i].location_7047, keyword) != NULL)
        {
            showOneItem_7047(i);
            found = true;
        }
    }

    if (!found)
        cout << "未找到相关物品。" << endl;
}

bool LostFoundBook_7047::isRelated_7047(char a[], char b[])
{
    // 单字符和占位词不参加包含匹配，避免“1”匹配“17号楼”。
    if (getCharacterCount(a) < 2 || getCharacterCount(b) < 2 ||
        strcmp(a, "无") == 0 || strcmp(b, "无") == 0 ||
        strcmp(a, "未知") == 0 || strcmp(b, "未知") == 0 ||
        strcmp(a, "不详") == 0 || strcmp(b, "不详") == 0)
        return false;

    // a里面包含b
    if (strstr(a, b) != NULL)
        return true;

    // b里面包含a
    if (strstr(b, a) != NULL)
        return true;

    // 两个方向都不包含
    return false;
}

int LostFoundBook_7047::getMatchScore_7047(int lostPos, int foundPos)
{
    // 类别是硬条件：类别不同，后面的名称、地点和日期不再比较。
    if (all_7047[lostPos].category_7047 != all_7047[foundPos].category_7047)
        return 0;

    // 类别相同先得1分，名称、地点、日期每符合一项再加1分。
    int score = 1;
    if (isRelated_7047(all_7047[lostPos].name_7047, all_7047[foundPos].name_7047))
        score++;
    if (isRelated_7047(all_7047[lostPos].location_7047, all_7047[foundPos].location_7047))
        score++;
    if (strcmp(all_7047[lostPos].date_7047, all_7047[foundPos].date_7047) == 0)
        score++;
    return score;
}

void LostFoundBook_7047::matchItem_7047()
{
    bool hasPendingLost = false;
    cout << "\n********** 待匹配的丢失物品 **********" << endl;
    for (int i = 0; i < cnt_7047; i++)
    {
        // 已完成的丢失记录不能再次认领，因此这里不显示。
        if (all_7047[i].type_7047 == 0 && all_7047[i].status_7047 == 0)
        {
            showOneItem_7047(i);
            hasPendingLost = true;
        }
    }

    if (!hasPendingLost)
    {
        cout << "当前没有待匹配的丢失物品。" << endl;
        return;
    }

    int lostNumber;
    cout << "请输入上面要匹配的丢失物品编号:";
    if (!(cin >> lostNumber))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "编号必须是数字！" << endl;
        return;
    }

    int lostPos = lostNumber - 1;
    if (lostPos < 0 || lostPos >= cnt_7047 ||
        all_7047[lostPos].type_7047 != 0 || all_7047[lostPos].status_7047 != 0)
    {
        cout << "编号无效，请输入上面显示的待匹配丢失物品编号！" << endl;
        return;
    }

    int foundPosList[MAX_ITEMS];
    int scoreList[MAX_ITEMS];
    int foundCount = 0;
    cout << "\n********** 匹配候选 **********" << endl;
    for (int i = 0; i < cnt_7047; i++)
    {
        // 所有“待处理的捡到记录”都列出来。
        // 匹配分只用于排序和提示，不能代替值班室人员的人工确认。
        if (all_7047[i].type_7047 == 1 && all_7047[i].status_7047 == 0)
        {
            int score = getMatchScore_7047(lostPos, i);
            foundPosList[foundCount] = i;
            scoreList[foundCount] = score;
            foundCount++;
        }
    }
    if (foundCount == 0)
    {
        cout << "当前没有待处理的捡到物品。" << endl;
        return;
    }

    // 与GUI保持一致：先显示分数高的候选。
    for (int i = 0; i < foundCount - 1; i++)
    {
        for (int j = i + 1; j < foundCount; j++)
        {
            if (scoreList[j] > scoreList[i])
            {
                int temp = scoreList[i];
                scoreList[i] = scoreList[j];
                scoreList[j] = temp;
                temp = foundPosList[i];
                foundPosList[i] = foundPosList[j];
                foundPosList[j] = temp;
            }
        }
    }
    for (int i = 0; i < foundCount; i++)
    {
        showOneItem_7047(foundPosList[i]);
        cout << "匹配分:" << scoreList[i] << endl;
    }

    cout << "匹配推荐结束；匹配分仅供参考，确认归还请使用主菜单的‘确认认领’。" << endl;
}

void LostFoundBook_7047::claimItem_7047()
{
    bool hasPendingFound = false;
    cout << "\n********** 待确认的捡到物品 **********" << endl;
    for (int i = 0; i < cnt_7047; i++)
    {
        if (all_7047[i].type_7047 == 1 && all_7047[i].status_7047 == 0)
        {
            showOneItem_7047(i);
            hasPendingFound = true;
        }
    }
    if (!hasPendingFound)
    {
        cout << "当前没有待处理的捡到物品。" << endl;
        return;
    }

    int foundNumber;
    cout << "请输入要确认的捡到物品编号（必填）:";
    if (!(cin >> foundNumber))
    {
        cin.clear();
        cin.ignore(10000, '\n');
        cout << "编号必须是数字！" << endl;
        return;
    }
    int foundPos = foundNumber - 1;
    if (foundPos < 0 || foundPos >= cnt_7047 ||
        all_7047[foundPos].type_7047 != 1 || all_7047[foundPos].status_7047 != 0)
    {
        cout << "请选择上面显示的待处理捡到物品编号！" << endl;
        return;
    }

    bool hasPendingLost = false;
    cout << "\n********** 可选的丢失物品 **********" << endl;
    for (int i = 0; i < cnt_7047; i++)
    {
        if (all_7047[i].type_7047 == 0 && all_7047[i].status_7047 == 0)
        {
            showOneItem_7047(i);
            hasPendingLost = true;
        }
    }

    int lostNumber = 0;
    if (hasPendingLost)
    {
        cout << "请输入对应的丢失物品编号（没有登记请输入0）:";
        if (!(cin >> lostNumber))
        {
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "编号必须是数字！" << endl;
            return;
        }
    }

    int lostPos = -1;
    if (lostNumber != 0)
    {
        lostPos = lostNumber - 1;
        if (lostPos < 0 || lostPos >= cnt_7047 ||
            all_7047[lostPos].type_7047 != 0 || all_7047[lostPos].status_7047 != 0)
        {
            cout << "请选择上面显示的待处理丢失物品编号，或输入0表示没有登记！" << endl;
            return;
        }
    }

    claimItem_7047(lostPos, foundPos);
}

void LostFoundBook_7047::claimItem_7047(int lostPos, int foundPos)
{
    if (foundPos < 0 || foundPos >= cnt_7047 ||
        all_7047[foundPos].type_7047 != 1 || all_7047[foundPos].status_7047 != 0)
    {
        cout << "请选择一条待处理的捡到物品！" << endl;
        return;
    }

    if (lostPos >= 0 && (lostPos >= cnt_7047 ||
        all_7047[lostPos].type_7047 != 0 || all_7047[lostPos].status_7047 != 0))
    {
        cout << "请选择一条待处理的丢失物品，或输入0表示没有登记！" << endl;
        return;
    }

    cout << "捡到物品:" << endl;
    showOneItem_7047(foundPos);
    if (lostPos >= 0)
    {
        cout << "对应的丢失物品:" << endl;
        showOneItem_7047(lostPos);
    }
    else
    {
        cout << "当前没有对应的丢失登记，本次只处理捡到物品。" << endl;
    }

    char confirm;
    cout << "确认认领请输入Y:";
    cin >> confirm;
    if (confirm != 'Y' && confirm != 'y')
    {
        cout << "已取消认领。" << endl;
        return;
    }

    // 有对应丢失记录时同时完成两边；没有时只完成捡到记录。
    if (lostPos >= 0)
        all_7047[lostPos].status_7047 = 1;
    all_7047[foundPos].status_7047 = 1;
    if (!saveBook_7047())
    {
        if (lostPos >= 0)
            all_7047[lostPos].status_7047 = 0;
        all_7047[foundPos].status_7047 = 0;
        return;
    }
    cout << "认领成功！" << endl;
}

void LostFoundBook_7047::clearBook_7047()
{
    int oldCount = cnt_7047;
    cnt_7047 = 0;
    if (!saveBook_7047())
    {
        cnt_7047 = oldCount;
        return;
    }
    cout << "已经清空全部记录。" << endl;
}

void LostFoundBook_7047::exitBook_7047()
{
    saveBook_7047();
}
