#include <memory>
#include <iostream>
using namespace std;
int main() {
    int * temp = new int(10);
    std::shared_ptr<int> s1(temp);
    std::shared_ptr<int> s2(temp);
    cout<<s1.use_count()<<" "<<s2.use_count()<<endl;
    std::shared_ptr<char> s3 = std::make_shared<char>();
    std::shared_ptr<char> s4(s3);
    std::shared_ptr<char> s5 = s4;
    cout<<s3.use_count()<<" "<<s4.use_count()<<" "<<s5.use_count()<<endl;
    return 0;
}