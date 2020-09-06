#include <iostream>
#include "console.h"
#include <conio.h>
#include <vector>
#include <time.h>
#include <string>
#include <fstream>
#include <algorithm>
using namespace std;

#define consoleWidth 60
#define consoleHeight 29

enum status{LEFT, RIGHT};
struct HIGHSCORE
{
    string name;
    int score;
};

int score;
int heart;

struct tank
{
    int x, y;
};

void Init(tank &t)
{
    t.x = (consoleWidth - 4) / 2;
    t.y = consoleHeight - 4;
}

void InitEnemyTank(vector<tank> &enemy) //Khoi tao xe tang doi phuong
{
    tank t;
    t.x = 0;
    t.y = 0;
    bool RunAgain = true;

    srand((unsigned)time(NULL));
    while (RunAgain)
    {
        do
        {
            t.x = 4 + rand() % (consoleWidth - 4);
        } while ((t.x - 4) % 5 != 0);   //Xe tang co do rong bang 5, tao khoang cach giua cac xe deu nhau

        if (enemy.size() == 0)
            break;
        else
        {
            for (int i = enemy.size() - 1; i >= 0; i--) //Tranh truong hop 2 xe lien tiep nhau cung mot cot
            {
                if (abs(t.x - enemy[i].x) == 0)
                    break;
                else
                    RunAgain = false;
            }
        }
    }
    t.y = 0;

    enemy.push_back(t);
}

void DrawEnemyTank(tank t)  //Ve xe tang cua doi thu
{
    gotoXY(t.x, t.y + 1);
    cout << char(178) << char(95) << char(95) << char(95) << char(178);
    gotoXY(t.x, t.y + 2);
    cout << char(178) << char(219) << char(219) << char(219) << char(178);
    gotoXY(t.x + 2, t.y + 3);
    cout << char(219);
}

void DrawTank(tank t)   //Ve xe tang cua minh
{
    gotoXY(t.x + 3, t.y + 1);
    cout << char(219);
    gotoXY(t.x + 1, t.y + 2);
    cout << char(178) << char(219) << char(219) << char(219) <<char(178) ;
    gotoXY(t.x + 1, t.y + 3);
    cout << char(178) << char(223) << char(223) << char(223) << char(178) ;
}

struct Bullet //Dan
{
    int x, y;
};

void DrawBullet(Bullet b)   //Ve dan
{
    gotoXY(b.x, b.y);
    cout << char(219);
}

void BulletManage(vector<Bullet> &bArr, Bullet b)   //Quan ly duong di cua dan
{
    for (size_t i = 0; i < bArr.size(); i++)
    {
        DrawBullet(bArr[i]);
        bArr[i].y -= 2;
        if (bArr[i].y < 0)//Xu ly dan bay het man hinh, xoa khoi mang
        {
            bArr.erase(bArr.begin() + i);
            i--;
        }            
    }
}

void XuLyXeTangTrungDan(vector<Bullet>& bArr, vector<tank> &enemy)
{
    for (size_t i = 0; i < bArr.size(); i++)
        for (size_t j = 0; j < enemy.size(); ++j)
        {
            if (bArr[i].x == enemy[j].x + 2 && bArr[i].y <= enemy[j].y + 3)
            {
                bArr.erase(bArr.begin() + i);
                enemy.erase(enemy.begin() + j);
                score += 100;
                return;
            }
        }
}

void InMangSong()
{
    gotoXY(0, 0);
    if (heart == 3)
        cout << char(233) << char(233) << char(233);
    else if (heart == 2)
        cout << char(233) << char(233);
    else if (heart == 1)
        cout << char(233);
    else if (heart == 0)
    {
        throw "GAME OVER";
    }
}

void XuLyMangSong(vector<tank>& enemy)
{
    for (size_t i = 0; i < enemy.size(); i++)
    {
        if ((enemy[i].y + 3) > (consoleHeight - 4))//Neu vi tri xe tang dich co y lon hon y cua xe tang minh
        {
            enemy.erase(enemy.begin() + i);
            heart--;
            break;
        }
    }
}

void MainRun(tank &t)
{
    vector<Bullet> bArr;//Quan ly duong di cua vien dan
    vector<tank> enemy; //Quan ly xe tang doi thu
    bArr.reserve(30);
    enemy.reserve(20);
    int enemySpeed = 0;

    bool shooted = false;//true: ban dan, false: khong ban dan
    Bullet b;
    //tank enemyTank;

    try
    {
        while (true)
        {
            clrscr();
            InMangSong();

            DrawTank(t);    //Khoi tao xe tank
            b.x = t.x + 3;  //Vi tri cua vien dan phu thuoc vao dau xe tank
            b.y = t.y;

            if (enemySpeed % 18 == 0)
                InitEnemyTank(enemy); //Toc do khoi tao xe tang doi phuong
            for (size_t i = 0; i < enemy.size(); i++)
            {
                DrawEnemyTank(enemy[i]);
                if (enemySpeed % 5 == 0)
                    enemy[i].y += 1;    //Toc do di chuyen cua xe tang doi phuong
            }


            if (_kbhit())
            {
                char key = _getch();
                if (key == 75 || key == 'A' || key == 'a')
                {
                    t.x -= 5;
                    if (t.x < 0)    //Xu ly cham bien
                        t.x += 5;
                }  
                else if (key == 77 || key == 'D' || key == 'd')
                {
                    t.x += 5;
                    if (t.x > consoleWidth) //Xu ly cham bien
                        t.x -= 5;
                }    
                else if (key == 32) //Phim space: ban dan
                {
                    shooted = true;
                    bArr.push_back(b);
                }
            }

            //Xu ly dan bay
            if (shooted == true)
            {
                BulletManage(bArr, b);
                if (bArr.size() == 0)
                    shooted = false;
            }

            //Xu ly xe tang trung dan
            XuLyXeTangTrungDan(bArr, enemy);

            //Xu ly mang song
            XuLyMangSong(enemy);

            enemySpeed++;
            Sleep(50);
        }
    }
    catch (const char* endGame)
    {
        gotoXY((consoleWidth - 9) / 2,2);
        cout << endGame << endl;
        gotoXY((consoleWidth - 15) / 2, 4);
        cout <<"YOUR SCORE: "<< score << endl;
        system("pause");
    }
}

void GhiVaoFile(string name, int score)
{
    int fileSize;
    fstream outFile("HighScore.txt");
    HIGHSCORE diem[20];

    if (!outFile)
    {
        cout << "Khong mo duoc file\n";
        exit(1);
    }

    outFile >> fileSize;

    for (int i = 0; i < fileSize; i++)
    {
        outFile >> diem[i].name;
        outFile >> diem[i].score;
    }

    fileSize++; //Tang kich thuoc mang luu tru
    outFile.seekp(ios::beg);//Di chuyen den dau file de ghi kich thuoc
    outFile << fileSize << endl;

    //Cap nhat du lieu moi vao mang
    diem[fileSize - 1].name = name;
    diem[fileSize - 1].score = score;

    //Ghi du lieu len file
    for (int i = 0; i < fileSize; i++)
    {
        outFile << diem[i].name << " " << diem[i].score;
        outFile << endl;
    }

    outFile.close();
}

void DocFile()
{
    HIGHSCORE diem[20];
    int fileSize;


    ifstream inFile("HighScore.txt");
    if (!inFile)
    {
        cout << "Khong mo duoc file de doc!\n";
        exit(1);
    }

    inFile >> fileSize;

    HIGHSCORE h;

    for (int i = 0; i < fileSize; i++)
    {
        inFile >> diem[i].name;
        inFile >> diem[i].score;
    }

    //Sap xep
    for (int i = 0; i < fileSize - 1; i++)
        for (int j = fileSize - 1; j > i; j--)
            if (diem[j].score > diem[j - 1].score)
            {
                HIGHSCORE tmp = diem[j];
                diem[j] = diem[j - 1];
                diem[j - 1] = tmp;
            }

    for (int i = 0; i < fileSize; i++)
        cout << diem[i].name << " " << diem[i].score << endl;

    inFile.close();
}

void menu()
{
    char selection;
    string name;

    do
    {
        do
        {
            system("cls");
            cout << "1. NEW GAME\n";
            cout << "2. HIGH SCORE\n";
            cout << "3. EXIT GAME\n";
            cin >> selection;
        } while (selection != 49 && selection != 50 && selection != 51);

        switch (selection)
        {
        case 49:
            score = 0;
            heart = 3;
            tank t;
            Init(t);

            system("cls");
            cout << "YOUR NAME: ";
            cin.ignore();
            getline(cin, name);
            MainRun(t);
            GhiVaoFile(name, score);
            break;
        case 50:
            system("cls");
            DocFile();
            system("pause");
            break;
        case 51:
            system("cls");
            gotoXY((consoleWidth - 13) / 2, 2);
            cout << "SEE YOU AGAIN\n\n\n";
            system("pause");
        }
    } while (selection != 51);

}

int main()
{
    menu();
    return 0;
}

