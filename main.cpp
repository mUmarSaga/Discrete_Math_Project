#include <cmath>
#include <iostream>
#include <windows.h>
#include <stack>
#include <queue>
#include <sstream>
#include <conio.h>
#include <map>
#define WIDTH 149
using namespace std;
void printHeading();
void mainMenu(int &choice);
void printMenu(string menu[],int rows,int selected);
void parseExpression(int c);
vector<string> uniqueVariables(queue<string> q);
void createTruthTable(queue<string> q);
void NANDSynthesis(queue<string> q);
void drawDecorativeGate(int x, int y);
void printColoredWord(string text, int colorCode);
void NORSynthesis(queue<string> q);
void generateColors(int k);
void gotoxy(int x, int y);
int countNANDGates(queue<string> q);
int countNORGates(queue<string> q);
void compareNANDvsNOR(queue<string> q);
void clearInputBuffer();
void clearStage();
void setFullScreen();
void clearStackDisplay();
void hideCursor();
int getMainChoice(int &choice,string s[],int rows);
int main(){
    int mainChoice = 0;
    string menu[] = {"[1] Synthesize New Boolean Expression (AND/OR/NOT -> NAND)","[2] Synthesize New Boolean Expression (AND/OR/NOT -> NOR)","[3] Compare NAND VS NOR GATE COUNT","[4] Generate Truth Table","[5] Exit"};
    SetConsoleOutputCP(CP_UTF8);
    hideCursor();
    setFullScreen();
    printHeading();
    // drawDecorativeGate(3,13);
    while(mainChoice != 4) {
        printMenu(menu,5,0);
        mainMenu(mainChoice);
        if (mainChoice == 0) {
            parseExpression(1);
        }else if (mainChoice == 1) {
            parseExpression(2);
        }else if (mainChoice == 2) {
            parseExpression(3);
        }else if (mainChoice == 3) {
            parseExpression(4);
        }
        clearStage();
    }
    return 0;
}
void printHeading() {
    system("cls");
    generateColors(14);
    const int x = 32;
    int y = 2;
    string heading[6] = {
        "███╗   ██╗ █████╗ ███╗   ██╗██████╗     ███████╗██╗   ██╗███╗   ██╗████████╗██╗  ██╗",
        "████╗  ██║██╔══██╗████╗  ██║██╔══██╗    ██╔════╝╚██╗ ██╔╝████╗  ██║╚══██╔══╝██║  ██║",
        "██╔██╗ ██║███████║██╔██╗ ██║██║  ██║    ███████╗ ╚████╔╝ ██╔██╗ ██║   ██║   ███████║",
        "██║╚██╗██║██╔══██║██║╚██╗██║██║  ██║    ╚════██║  ╚██╔╝  ██║╚██╗██║   ██║   ██╔══██║",
        "██║ ╚████║██║  ██║██║ ╚████║██████╔╝    ███████║   ██║   ██║ ╚████║   ██║   ██║  ██║",
        "╚═╝  ╚═══╝╚═╝  ╚═╝╚═╝  ╚═══╝╚═════╝     ╚══════╝   ╚═╝   ╚═╝  ╚═══╝   ╚═╝   ╚═╝  ╚═╝"
    };
    for (const auto & i : heading) {
        gotoxy(x, y);
        cout << i;
        y++;
    }
    generateColors(11);
    cout << "\n  ";
gotoxy(10,8);
    for(int i = 0; i < 125; i++) cout << "═";
    generateColors(15);
    gotoxy(x + 8, y + 1);
    cout << ">> UNIVERSAL GATE SYNTHESIS & LOGIC OPTIMIZATION ENGINE <<";
    generateColors(11);
    cout << "\n  ";
    gotoxy(10,10);
    for(int i = 0; i < 125; i++) cout << "═";
    generateColors(7);
    cout << "\n\n";
}
void drawDecorativeGate(int x, int y) {
    gotoxy(x, y);     cout << "A \xCD\xCD\xCD\xCD\xCD\xCD\xBB";           // A ══════╗
    gotoxy(x, y + 1); cout << "        \xBA  \xDB\xDB\xDB\xDB\xDB\xDB\xBB";   // ║  ██████╗
    gotoxy(x, y + 2); cout << "        \xCC\xCD\xCD\xDB\xDB\xC9\xCD\xCD\xDB\xDB\xBB\xCD\xCDo  Y"; // ╠══██╔══██╗══o  Y
    gotoxy(x, y + 3); cout << "        \xBA  \xDB\xDB\xDB\xDB\xDB\xDB\xBC";   // ║  ██████╝
    gotoxy(x, y + 4); cout << "B \xCD\xCD\xCD\xCD\xCD\xCD\xBC  \xC8\xCD\xCD\xCD\xCD\xCD\xBC";   // B ══════╝  ╚═════╝
}
int checkPrecedence(string token) {
    if (token == "AND") {
        return 2;
    }else if (token == "XOR") {
        return 2;
    }else if (token == "NOT") {
        return 3;
    }else if (token == "OR") {
        return 1;
    }
    return 0;
}
string fixSpace(const string &input) {
    string result = "";
    for (int i = 0; i < input.length(); i++) {
        if (input[i] == '(' || input[i] == ')') {
            if (i != 0) {
                if (input[i - 1] != ' ') {
                    result += " ";
                }
            }
            result += input[i];
            if (i < input.length() - 1) {
                if (input[i + 1] != ' ') {
                    result += " ";
                }
            }
        } else {
            result += input[i];
        }
    }
    return result;
}
void printStackorQueue(stack<string> st,queue<string> q) {
    clearStackDisplay();
    gotoxy(0, 0);
    generateColors(14);
    cout << "STACK:";
    generateColors(10);
    int row = 2;
    while (!st.empty()) {
        gotoxy(0, row++);
        cout << st.top();
        st.pop();
    }
    gotoxy(143, 0);
    generateColors(14);
    cout << "QUEUE:";
    generateColors(10);
    int i = 1;
    while (!q.empty()) {
        gotoxy(143, 0 + i);
        cout << q.front();
        q.pop();
        i++;
    }
}
void parseExpression(int c) {
    stack<string> st;
    queue<string> q;
    clearStage();
    clearInputBuffer();
    gotoxy(32, 13);
    generateColors(14);
    if (c == 3) {
        cout << "BOOLEAN EXPRESSION FOR COMPARISON : ";
    }else {
        cout << "BOOLEAN EXPRESSION : ";
    }

    generateColors(15);
    string input;
    getline(cin,input);
    input  = fixSpace(input);
    stringstream ss(input);
    string token;
    while (ss >> token) {
        if (token == "(") {
                st.push(token);
            }
        else if (token == "AND"|| token == "OR"||token == "NOT"|| token == "XOR") {
            while (!st.empty() && st.top() != "(" && checkPrecedence(st.top()) >= checkPrecedence(token)) {
                    q.push(st.top());
                    st.pop();
                }
                st.push(token);
            }
        else if (token >= "A" && token <= "Z") {
                q.push(token);
            }
        else if (token == ")") {
                while (st.top() != "(") {
                    q.push(st.top());
                    st.pop();
                }
                st.pop();
            }
            printStackorQueue(st,q);
            Sleep(500);
    }
    while (!st.empty()) {
        q.push(st.top());
        st.pop();
        printStackorQueue(st,q);
        Sleep(500);
    }
    if (c == 1) {
        NANDSynthesis(q);
        generateColors(14);
        cout << endl;
        cout << "\t\t\t\t";
        cout << "NAND GATE COUNT : ";
        generateColors(7);
        cout << countNANDGates(q);
    }else if (c == 2) {
        NORSynthesis(q);
        generateColors(14);
        cout << endl;
        cout << "\t\t\t\t";
        cout << "NOR GATE COUNT : ";
        generateColors(7);
        cout << countNORGates(q);
    }else if (c == 3) {
        compareNANDvsNOR(q);
    }else if (c == 4) {
        createTruthTable(q);
    }
    clearInputBuffer();
    getch();
}
vector<string> uniqueVariables(queue<string> q) {
    vector<string> vars;
    while (!q.empty()) {
        string s = q.front();
        q.pop();
        if (s.length() == 1 && s[0] >= 'A' && s[0] <= 'Z') {
            bool isFound = false;
            for (int i = 0;i < vars.size();i++) {
                if (vars[i] == s) {
                    isFound = true;
                }
            }
            if (!isFound) {
                vars.push_back(s);
            }
        }
    }
    return vars;
}
void createTruthTable(queue<string> q) {
    system("cls");
    printHeading();
    vector<string> var = uniqueVariables(q);
    queue<string> temp;
    map<char,bool> mapVar;
    int rowCount = pow(2,var.size());
    // cout <<"\t\t";
    generateColors(14);
    for (int i = 0; i < var.size(); i++) {
        cout << var[i] << "\t\t";
    }
    cout << "Result";
    generateColors(7);
    cout << endl;
    for (int i = 0;i < rowCount;i++) {
        temp = q;
        stack<bool> calcStack;
        // cout <<"\t\t";
        for (int j = 0; j < var.size(); j++) {
            mapVar[var[j][0]] = (i >> (var.size()-1-j)) & 1;
            cout << mapVar[var[j][0]] << "\t\t";
        }
        while (!temp.empty()) {
            string s = temp.front();
            if (s == "NOT") {
                bool t = calcStack.top();
                t = !t;
                calcStack.pop();
                calcStack.push(t);
            }else if (s == "AND") {
                bool op1,op2,t;
                op1 = calcStack.top();
                calcStack.pop();
                op2 = calcStack.top();
                calcStack.pop();
                bool final = op1 && op2;
                calcStack.push(final);
            }else if (s == "OR") {
                bool op1,op2,t;
                op1 = calcStack.top();
                calcStack.pop();
                op2 = calcStack.top();
                calcStack.pop();
                t = op1 || op2;
                calcStack.push(t);
            } else if (s == "XOR") {
                bool op1 = calcStack.top(); calcStack.pop();
                bool op2 = calcStack.top(); calcStack.pop();
                bool result = op1 ^ op2;
                calcStack.push(result);
            }else if (s >= "A" && s <= "Z") {
                calcStack.push(mapVar[s[0]]);
            }
            temp.pop();
        }
        if(!calcStack.empty()) {
            cout << "\t\t" << calcStack.top();
            calcStack.pop();
        }
        cout << endl;
    }
}
void printNAND(stack<string> synthesis) {
    generateColors(14);
    gotoxy(32, 14);
      cout << "NAND EXPRESSION : ";
    gotoxy(32, 15);
generateColors(10);
    while (!synthesis.empty()) {
        cout << synthesis.top();
        synthesis.pop();
    }
}
void NORSynthesis(queue<string> q) {
    stack<string> st;
    while (!q.empty()) {
        string s = q.front();
        q.pop();
        if (s == "NOT") {
            string a = st.top(); st.pop();
            string r = " ( " + a + " NOR " + a + " ) ";
            st.push(r);
        }
        else if (s == "OR") {
            string b = st.top(); st.pop();
            string a = st.top(); st.pop();

            string t = " ( " + a + " NOR " + b + " ) ";
            string r = " ( " + t + " NOR " + t + " ) ";
            st.push(r);
        }else if (s == "XOR") {
            string b = st.top(); st.pop();
            string a = st.top(); st.pop();
            string t1 = " ( " + a + " NOR " + b + " ) ";      // A NOR B
            string na = " ( " + a + " NOR " + a + " ) ";      // NOT A
            string nb = " ( " + b + " NOR " + b + " ) ";      // NOT B
            string t2 = " ( " + na + " NOR " + nb + " ) ";    // A AND B
            string r  = " ( " + t1 + " NOR " + t2 + " ) ";    // XOR
            st.push(r);
        }
        else if (s == "AND") {
            string b = st.top(); st.pop();
            string a = st.top(); st.pop();

            string na = " ( " + a + " NOR " + a + " ) ";
            string nb = " ( " + b + " NOR " + b + " ) ";
            string r  = " ( " + na + " NOR " + nb + " ) ";
            st.push(r);
        }
        else if (s >= "A" && s <= "Z") {
            st.push(s);
        }
        printNAND(st);
        Sleep(200);
    }
}
void NANDSynthesis(queue<string> q) {
    stack<string> synthesisStack;
    while (!q.empty()) {
        string s = q.front();
        if (s == "NOT") {
            string temp = synthesisStack.top();
            temp = " ( "+temp+" NAND "+temp+" ) ";

            synthesisStack.pop();
            synthesisStack.push(temp);
        }else if (s == "AND") {
            string op1,op2,temp;
            op1 = synthesisStack.top();
            synthesisStack.pop();
            op2 = synthesisStack.top();
            synthesisStack.pop();
            temp = "(( "+op2 + " NAND "+ op1+" ) "+" NAND " +" ( "+op2 + " NAND " +op1+" ))";
            temp = " ( " + op2 + " NAND "+ op1+" ) ";
            string final = " ( " + temp + " NAND " + temp + " ) ";
            synthesisStack.push(final);
        }else if (s == "XOR") {
            string op1 = synthesisStack.top(); synthesisStack.pop();
            string op2 = synthesisStack.top(); synthesisStack.pop();
            string notA = " ( " + op2 + " NAND " + op2 + " ) ";
            string notB = " ( " + op1 + " NAND " + op1 + " ) ";
            string temp1 = " ( " + op2 + " NAND " + notB + " ) ";
            string aAndNotB = " ( " + temp1 + " NAND " + temp1 + " ) ";
            string temp2 = " ( " + notA + " NAND " + op1 + " ) ";
            string notAAndB = " ( " + temp2 + " NAND " + temp2 + " ) ";
            string notLeft = " ( " + aAndNotB + " NAND " + aAndNotB + " ) ";
            string notRight = " ( " + notAAndB + " NAND " + notAAndB + " ) ";
            string result = " ( " + notLeft + " NAND " + notRight + " ) ";
            synthesisStack.push(result);
        }else if (s == "OR") {
            string op1,op2,temp;
            op1 = synthesisStack.top();
            synthesisStack.pop();
            op2 = synthesisStack.top();
            synthesisStack.pop();
            string invA = " ( " + op2 + " NAND " + op2 + " ) ";
            string invB = " ( " + op1 + " NAND " + op1 + " ) ";
            // temp = "(( "+op2 + " NAND "+ op2+" ) "+" NAND " +" ( "+op1 + " NAND " +op1+" ))";
            temp = " ( "+ invA + " NAND " + invB + " ) ";
            synthesisStack.push(temp);
        }else if (s >= "A" && s <= "Z") {
            synthesisStack.push(s);
        }
        q.pop();
        printNAND(synthesisStack);
        Sleep(200);
    }
}
void compareNANDvsNOR(queue<string> q) {
    // clearStage();
    // clearInputBuffer();
    // gotoxy(32, 13);
    // generateColors(14);
    // cout << "ENTER EXPRESSION FOR COMPARISON: ";
    // generateColors(15);
    // string input;
    // getline(cin, input);
    // input = fixSpace(input);
    int nandGates = countNANDGates(q);
    int norGates = countNORGates(q);
    gotoxy(20, 15);
    generateColors(14);
    cout << "╔════════════════════════════════════════════════════════╗";
    gotoxy(20, 16);
    cout << "║         NAND vs NOR COMPARISON                         ║";
    gotoxy(20, 17);
    cout << "╚════════════════════════════════════════════════════════╝";
    gotoxy(25, 19);
    generateColors(10);
    cout << "NAND Implementation:";
    gotoxy(25, 20);
    cout << "  Total Gates: " << nandGates;
    gotoxy(25, 22);
    generateColors(12);
    cout << "NOR Implementation:";
    gotoxy(25, 23);
    cout << "  Total Gates: " << norGates;
    gotoxy(25, 25);
    generateColors(14);
    if (nandGates < norGates) {
        cout << "✓ NAND is more efficient ("
             << ((norGates - nandGates) * 100 / norGates) << "% fewer gates)";
    } else if (norGates < nandGates) {
        cout << "✓ NOR is more efficient ("
             << ((nandGates - norGates) * 100 / nandGates) << "% fewer gates)";
    } else {
        cout << "✓ Both implementations are equally efficient";
    }
    getch();
}
int countNANDGates(queue<string> q) {
    int count = 0;
    while (!q.empty()) {
        string s = q.front();
        q.pop();
        if (s == "NOT") count += 1;
        else if (s == "AND") count += 2;
        else if (s == "OR") count += 3;
        else if (s == "XOR") count += 9;
    }
    return count;
}
int countNORGates(queue<string> q) {
    int count = 0;
    while (!q.empty()) {
        string s = q.front();
        q.pop();

        if (s == "NOT") count += 1;
        else if (s == "AND") count += 3;
        else if (s == "OR") count += 2;
        else if (s == "XOR") count += 9;
    }
    return count;
}
void printMenu(string menu[],int rows,int selected) {
    int y = 14;
    generateColors(10);
    for (int i = 0 ; i < rows ; i++) {
        gotoxy(40,y+i);
        if (i == selected) {
            printColoredWord(menu[i],122);
        }else {
            printColoredWord(menu[i],10);
        }
    }
}
int getMainChoice(int &choice,string s[],int rows) {
    choice = 0;
    while (true) {
        if (GetAsyncKeyState(VK_UP)) {
            if (choice > 0) {
                choice--;
                printMenu(s,rows,choice);
            }
        }
        if (GetAsyncKeyState(VK_DOWN)) {
            if (choice < rows-1) {
                choice++;
                printMenu(s,rows,choice);
            }
        }
        if (GetAsyncKeyState('F')) {
            return 0;
        }
        Sleep(150);
    }
}
void mainMenu(int &choice) {
    string menu[] = {"[1] Synthesize New Boolean Expression (AND/OR/NOT -> NAND)","[2] Synthesize New Boolean Expression (AND/OR/NOT -> NOR)","[3] Compare NAND VS NOR GATE COUNT","[4] Generate Truth Table","[5] Exit"};
    getMainChoice(choice,menu,5);
}
void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void hideCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = FALSE; // Set the cursor visibility to false
    SetConsoleCursorInfo(hConsole, &cursorInfo);
}
void generateColors(int k)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, k);
}
void clearInputBuffer() {
    HANDLE hStdIn = GetStdHandle(STD_INPUT_HANDLE);
    FlushConsoleInputBuffer(hStdIn);
}
void setFullScreen() {
    HWND hwnd = GetConsoleWindow();
    ShowWindow(hwnd, SW_MAXIMIZE);
}
void clearStackDisplay() {
    for (int y = 0; y < 10; y++) {
        gotoxy(0, y);
        for(int x = 0; x < 7; x++) cout << " ";
    }
    for (int y = 0; y < 10; y++) {
        gotoxy(0, y);
        for(int x = 140; x < WIDTH; x++) cout << " ";
    }
}
void clearStage() {
    int h;

    for (int y = 12; y < 33; y++) {
        gotoxy(0, y);
        for(int x = 0; x < WIDTH; x++) cout << " ";
    }
}
void printColoredWord(string text, int colorCode) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // 1. Get the current console attributes (the default color)
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    WORD savedAttributes = consoleInfo.wAttributes;

    // 2. Set the custom color (Light Red, in this case)
    SetConsoleTextAttribute(hConsole, colorCode);

    // 3. Print the text (Crucially, NO endl here)
    cout << text;

    // 4. Reset the color back to the saved default
    SetConsoleTextAttribute(hConsole, savedAttributes);
}