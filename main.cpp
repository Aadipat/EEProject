#include<iostream>
#include "cube.h"
using namespace std;

int main(){
    Cube c(3);
    cout << c;
    c.scramble(5);
    cout << c;
    // c.solve();
    // cout << c;
}