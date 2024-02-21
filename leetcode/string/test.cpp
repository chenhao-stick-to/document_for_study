#include <iostream>
#include <string>
using namespace std;
int main() {
    string s;
    cin >> s;
    string result;
    for (int i = 0; i < s.length(); i++) {
        if ((int)s[i] >= 48 && (int)s[i] <= 57) {
            result += "number";
            continue;
        }
        result += s[i];
    }
    cout<<result;
    return 0;
}