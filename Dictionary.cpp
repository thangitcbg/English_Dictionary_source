#include <bits/stdc++.h>

using namespace std;

void debug_out() { cerr << '\n'; }
template <typename Head, typename ...Tail>
void debug_out(Head H, Tail ...T)
{
    cerr << " " << H;
    debug_out(T...);
}
#define debug(...) cerr << "[" << #__VA_ARGS__ << "]:", debug_out(__VA_ARGS__)

// IDWord has the role of linking words and their indices
map <string, int> IDWord;

// Store synonyms
vector <vector <string>> Group;

int Count_Words = 0;
vector <int> par;

int String_to_Number(string str)
{
    int res = 0;
    for (auto x : str) res = res * 10 + (x - '0');
    return res;
}

void Init()
{
    Group.assign(Count_Words + 1, vector <string> (0));
    par.assign(Count_Words + 1, 0);
}

void ImportData()
{
    // Index_File_Input is index of file INPUT

    ifstream FileIn("Data.txt");

    string Information;
    getline(FileIn, Information);
    Count_Words = String_to_Number(Information);

    Init();

    while (getline(FileIn, Information))
    {
        string a[3] = {"","",""};
        int type = 0;
        for (int i = 0; i < Information.size(); ++i)
        {
            if (isdigit(Information[i]) && Information[i-1] == ' ')
                a[type].pop_back(), ++type;
            a[type] += Information[i];
        }
        if (a[0] != "" && a[1] != "" && a[2] != "")
        {
            int Index = String_to_Number(a[1]);
            int Root = String_to_Number(a[2]);
            IDWord[a[0]] = Index;
            par[Index] = Root;
            Group[Root].push_back(a[0]);
        }
    }
    FileIn.close();
}

void ExportData()
{
    ofstream FileOut("Data.txt");

    FileOut << Count_Words << '\n';
    for (auto &x : IDWord)
        FileOut << x.first << " " << x.second << " " << par[x.second] << '\n';
    FileOut.close();
}

bool Check_Word(string str)
{
    for (auto &x : str)
        if (!islower(x) && !isupper(x) && x != ' ')
            return 0;
    return 1;
}

int EnterOption()
{
    cout << "Options:\n";
    cout << "   1.Check whether the word has been added before or not\n";
    cout << "   2.Add a new word\n";
    cout << "   3.Add a word to group of synonyms\n";
    cout << "   4.Show synonyms of a word\n";
    cout << "   0.Exit program\n";

    int Selection = 0;
    do
    {
        if (Selection)
        {
            cout << "Please enter your selection again!\n";
            cout << "(It must be between 1 and 4)\n";
        }
        cin >> Selection;
        cin.ignore(1);
    } while (Selection < 0 || Selection > 4);
    return Selection;
}

string EnterWord()
{
    cout << "Type the word: ";
    string word = "";
    do
    {
        if (word.size())
        {
            cout << "\n-The word can only consist of letters! ('A'-'Z' and 'a'-'z')\n";
            cout << "Please enter your word again!\n";
            cout << "Type the word: ";
        }
        getline(cin, word);
    } while (!Check_Word(word));
    for (auto &x : word) if (isupper(x)) x = x + 32;
    return word;
}

void Add_a_Word(string word)
{
    IDWord[word] = ++Count_Words;
    Group.push_back(vector <string> ({word}));
    par.push_back(Count_Words); // add a node
}

int root(int x)
{
    while (x != par[x])
        x = par[x] = par[par[x]];
    return x;
}

void Join(int x, int y)
{
    x = root(x); y = root(y);
    if (x == y) return;
    if (Group[x].size() < Group[y].size())
        swap(x, y);
    par[y] = x;
    for (auto &idword : Group[y])
        Group[x].push_back(idword);
    Group[y].clear();
}

void MainProgram()
{
    bool Continue = 0;
    do
    {
        int Selection = EnterOption();
        if (!Selection) break;

        string word = EnterWord();

        int GroupID = ((IDWord.find(word) != IDWord.end()) ? root(IDWord[word]) : 0);

        switch(Selection)
        {
            case 1:
            {
                if (GroupID)
                {
                    cout << "-The word is in your dictionary!\n";
                    cout << "-ID of group synonyms: " << GroupID << '\n';
                    cout << "-Size of group synonyms: " << Group[GroupID].size() << '\n';
                }
                else
                    cout << "-The word is NOT in your dictionary!" << '\n';

                break;
            }
            case 2:
            {
                if (GroupID) { cout << "-The word has been added before.\n"; break; }

                Add_a_Word(word);
                cout << "-Added!\n";
                break;
            }
            case 3:
            {
                bool Found = 0;
                do
                {
                    string other_word;
                    cout << "Type the synonym: ";
                    getline(cin, other_word);

                    if (IDWord.find(other_word) != IDWord.end() || GroupID)
                    {
                        Found = 1;

                        if (!GroupID)
                            Add_a_Word(word);
                        if (IDWord.find(other_word) == IDWord.end())
                            Add_a_Word(other_word);

                        Join(IDWord[other_word], IDWord[word]);

                        cout << "-The words have been merged!\n";
                        break;
                    }
                    else
                    {
                        cout << "-Can't find the synonym.\n";
                        cout << "\nDo you want to try again?\n";
                        cout << "0.No\n1.Yes\n";

                        bool Again;
                        cin >> Again;
                        cin.ignore(1);

                        cout << '\n';
                        if (!Again) break;
                    }
                } while (!Found);

                break;
            }
            case 4:
            {
                if (GroupID)
                {
                    cout << "\nList of synonyms:\n";
                    for (auto &x : Group[GroupID])
                        cout << '-' << x << '\n';
                }
                else
                    cout << "-Can't find the word!\n";
                break;
            }
        }

        cout << "\nDo you want to enter ANOTHER word?\n";
        cout << "0.No\n1.Yes\n";
        cin >> Continue;
        cin.ignore(1);

        system("cls");
    } while (Continue);
}

int main()
{
    ImportData();
    MainProgram();
    ExportData();

    return 0;
}
