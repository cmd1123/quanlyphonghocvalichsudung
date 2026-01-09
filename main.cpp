#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <windows.h>
#include <conio.h>
#include <iomanip>
#include <sstream>

using namespace std;

// ======================== CẤU TRÚC DỮ LIỆU ========================
struct Booking {
    string date;
    int slot; 
    string user;
};

struct Room {
    string id, name;
    int capacity;
    vector<Booking> bookings;
};

vector<Room> dsPhong;

// ======================== HÀM HỖ TRỢ GIAO DIỆN ========================
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x; coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void setColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}

// Giữ nguyên hàm printTypewriter nhưng thay đổi tham số x khi gọi
void printTypewriter(string text, int x, int y, int delay) {
    gotoxy(x, y);
    for (char c : text) {
        cout << c;
        if (delay > 0) Sleep(2); 
    }
    if (delay > 0) Sleep(delay);
}

void drawBox(int x, int y, int width, int height, int color) {
    setColor(color);
    for (int i = 0; i < width; i++) {
        gotoxy(x + i, y); cout << (char)205;
        gotoxy(x + i, y + height); cout << (char)205;
    }
    for (int i = 0; i < height; i++) {
        gotoxy(x, y + i); cout << (char)186;
        gotoxy(x + width, y + i); cout << (char)186;
    }
    gotoxy(x, y); cout << (char)201; gotoxy(x + width, y); cout << (char)187;
    gotoxy(x, y + height); cout << (char)200; gotoxy(x + width, y + height); cout << (char)188;
}

void clearRightFrame() {
    for (int i = 2; i < 24; i++) {
        gotoxy(41, i); cout << string(82, ' ');
    }
}

// ======================== XỬ LÝ DỮ LIỆU & FILE ========================
void saveAllData() {
    ofstream fRooms("rooms.txt");
    for (auto &p : dsPhong) {
        fRooms << p.id << "|" << p.name << "|" << p.capacity << "|" << p.bookings.size();
        for (auto &b : p.bookings) fRooms << "|" << b.date << "," << b.slot << "," << b.user;
        fRooms << endl;
    }
    fRooms.close();

    ofstream fHist("history.txt");
    fHist << "================== LICH SU SU DUNG PHONG HOC ==================" << endl;
    for (auto &p : dsPhong) {
        for (auto &b : p.bookings) {
            fHist << p.id << " | " << b.date << " | " << (b.slot == 1 ? "Sang" : "Chieu") << " | " << b.user << endl;
        }
    }
    fHist.close();
}

void loadData() {
    ifstream f("rooms.txt");
    if (!f) return;
    string line;
    while (getline(f, line)) {
        stringstream ss(line);
        Room p; string tmp;
        getline(ss, p.id, '|'); getline(ss, p.name, '|');
        getline(ss, tmp, '|'); p.capacity = stoi(tmp);
        getline(ss, tmp, '|'); int bCount = stoi(tmp);
        for (int i = 0; i < bCount; i++) {
            Booking b; string bStr; getline(ss, bStr, '|');
            stringstream ssB(bStr);
            getline(ssB, b.date, ','); getline(ssB, tmp, ','); b.slot = stoi(tmp);
            getline(ssB, b.user); p.bookings.push_back(b);
        }
        dsPhong.push_back(p);
    }
}

// ======================== GIAO DIỆN TIÊU ĐỀ (ĐÃ CĂN PHẢI) ========================
void veTieuDe(bool effect) {
    int d = effect ? 200 : 0;
    int xPos = 40.5; // Vị trí bắt đầu để nằm gọn trong khung phải
    
    setColor(10);
    printTypewriter(" ____   _    _    __    __   _  _      __    __   _____  _    _  ____  _    _______ ",xPos, 5, d);
    printTypewriter("| |  | | |  | |  /  \\  |  \\| | | |     \\ \\_/ /   | |__) | |__| | |  | |  \\| | |  __ ",xPos, 6, d);
    printTypewriter("| |  | | |  | | / /\\ \\ | . ` | | |      \\   /    |  ___/|  __  | |  | | . ` | | |_ |",xPos, 7, d);
    printTypewriter("| |__| | |__| |/ ____ \\| |\\  | | |____   | |     | |    | |  | | |__| | |\\  | |__| |",xPos, 8, d);
    printTypewriter("\\___\\_\\\\____/ /_/    \\_\\_| \\_| |______|  |_|     |_|    |_|  |_|\\____/|_| \\_|\\_____|",xPos, 9, d);

    setColor(11);
    printTypewriter(" _      _____ _____ _    _    _____ _    _    _____  _    _  _   _  ____ ",xPos, 13, d);
    printTypewriter("| |    |_   _/ ____| |  | |  / ____| |  | |  |  __ \\| |  | || \\ | |/ ___|",xPos, 14, d);
    printTypewriter("| |      | || |    | |__| | | (___ | |  | |  | |  | | |  | ||  \\| | |  _ ",xPos, 15, d);
    printTypewriter("| |      | || |    |  __  |  \\___ \\| |  | |  | |  | | |  | || . ` | | |_ |",xPos, 16, d);
    printTypewriter("| |____ _| || |____| |  | |  ____) | |__| |  | |__| | |__| || |\\  | |__| |",xPos, 17, d);
    printTypewriter("|______|_____\\_____|_|  |_| |_____/ \\____/   |_____/ \\____/ |_| \\_|\\____|",xPos, 18, d);
    
    if(effect) {
        setColor(14);
        gotoxy(100, 22); cout << "Nhom thuc hien: Nhom 7";
    }
}

// ======================== CHỨC NĂNG CHÍNH ========================
void datPhong() {
    clearRightFrame();
    string id, date, user; int slot;
    gotoxy(45, 3); setColor(11); cout << ">>> DAT PHONG (CHECK TRUNG LICH)";
    gotoxy(45, 5); setColor(7); cout << "Ma phong: "; cin >> id;
    for (auto &p : dsPhong) {
        if (p.id == id) {
            gotoxy(45, 6); cout << "Ngay (dd/mm): "; cin >> date;
            gotoxy(45, 7); cout << "Ca (1:Sang, 2:Chieu): "; cin >> slot;
            for (auto &b : p.bookings) {
                if (b.date == date && b.slot == slot) {
                    gotoxy(45, 9); setColor(12); cout << "LOI: Phong da co nguoi dat vao thoi diem nay!"; return;
                }
            }
            gotoxy(45, 8); cout << "Nguoi su dung: "; cin.ignore(); getline(cin, user);
            p.bookings.push_back({date, slot, user});
            saveAllData();
            gotoxy(45, 10); setColor(10); cout << "THANH CONG: Da cap nhat lich su dat phong!"; return;
        }
    }
    gotoxy(45, 6); setColor(12); cout << "LOI: Ma phong khong ton tai!";
}

void thongKe() {
    clearRightFrame();
    gotoxy(45, 3); setColor(11); cout << ">>> PHAN TICH HIEU QUA VA TAN SUAT";
    gotoxy(45, 5); setColor(7);
    cout << left << setw(8) << "ID" << setw(15) << "Tan suat" << "Danh gia";
    gotoxy(45, 6); cout << string(50, '-');
    int row = 0;
    for (auto &p : dsPhong) {
        int ts = p.bookings.size();
        gotoxy(45, 7 + row++);
        cout << left << setw(8) << p.id << setw(15) << ts;
        if (ts == 0) { setColor(12); cout << "KEM (0 LUOT)"; }
        else if (ts < 3) { setColor(14); cout << "TRUNG BINH"; }
        else { setColor(10); cout << "HIEU QUA CAO"; }
        setColor(7);
    }
}

int main() {
    system("mode con cols=125 lines=32");
    loadData();
    veTieuDe(true);
    Sleep(500);

    vector<string> menu = {"XEM DANH SACH PHONG", "THEM PHONG MOI    ", "SUA THONG TIN PHONG", "XOA PHONG HOC     ", "DAT PHONG (BOOKING)", "THONG KE HIEU QUA ", "XUAT BAO CAO (.TXT)", "THOAT CHUONG TRINH"};
    int pointer = 0;

    while (true) {
        system("cls");
        veTieuDe(false);
        drawBox(2, 1, 35, 6, 11); drawBox(2, 8, 35, 17, 11); drawBox(40, 1, 82, 25, 11);
        setColor(11); gotoxy(13, 2); cout << "HUONG DAN";
        gotoxy(4, 3); setColor(7); cout << "- Mui ten: Di chuyen";
        gotoxy(4, 4); cout << "- ENTER: Chon menu";
        gotoxy(4, 5); cout << "- ESC: Thoat an toan";
        setColor(11); gotoxy(15, 9); cout << "MENU";

        while (true) {
            for (int i = 0; i < menu.size(); i++) {
                gotoxy(5, 11 + i);
                if (pointer == i) { setColor(240); cout << " > " << menu[i] << " "; }
                else { setColor(10); cout << "   " << menu[i] << "  "; }
            }
            char key = _getch();
            if (key == 72) pointer = (pointer == 0) ? menu.size() - 1 : pointer - 1;
            else if (key == 80) pointer = (pointer == menu.size() - 1) ? 0 : pointer + 1;
            else if (key == 13) break;
            else if (key == 27) return 0;
        }

        switch (pointer) {
            case 0: {
                clearRightFrame(); gotoxy(45, 3); setColor(11); cout << ">>> DANH SACH PHONG HOC HIEN TAI";
                for(int i=0; i<dsPhong.size(); i++) {
                    gotoxy(45, 5+i); setColor(7); cout << dsPhong[i].id << " - " << dsPhong[i].name << " (" << dsPhong[i].capacity << " seats)";
                }
                break;
            }
            case 1: {
                clearRightFrame(); string id;
                gotoxy(45, 3); setColor(11); cout << ">>> THEM PHONG MOI";
                gotoxy(45, 5); setColor(7); cout << "Nhap ID: "; cin >> id;
                bool exists = false;
                for(auto &p : dsPhong) if(p.id == id) exists = true;
                if(exists) { gotoxy(45, 7); setColor(12); cout << "LOI: ID phong da ton tai!"; }
                else {
                    Room p; p.id = id;
                    gotoxy(45, 6); cout << "Ten phong: "; cin.ignore(); getline(cin, p.name);
                    gotoxy(45, 7); cout << "Suc chua: "; cin >> p.capacity;
                    dsPhong.push_back(p); saveAllData();
                    gotoxy(45, 9); setColor(10); cout << "THANH CONG: Da luu vao he thong!";
                }
                break;
            }
            case 4: datPhong(); break;
            case 5: thongKe(); break;
            case 6: saveAllData(); clearRightFrame(); gotoxy(45, 5); setColor(10); cout << "DA XUAT BAO CAO CHI TIET RA FILE 'history.txt'!"; break;
            case 7: return 0;
        }
        gotoxy(45, 22); setColor(14); cout << "Bam phim bat ky de quay lai..."; _getch();
    }
}