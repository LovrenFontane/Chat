#1
/*
#include <iostream>
using namespace std;
int main() {
	int a, b;
	int result1, result2, result3, result4;
	
	cin >> a >> b;
	result1 = a + b;
	result2 = a - b;
	result3 = a * b;
	result4 = a / b;
	
	cout << a << " + " << b << " = " << result1 << endl;
	cout << a << " - " << b << " = " << result2 << endl;
	cout << a << " * " << b << " = " << result3 << endl;
	cout << a << " / " << b << " = " << result4 << endl;
	
	return 0;
}
*/
#2
/*
#include <iostream>
using namespace std;
double computeArea(double r) {
    const double PI = 3.14159;
    double area;
    area = PI * r * r;
    return area;
}
int main() {
    double inside, outside, areaIn, areaOut;
    cin >> outside >> inside;
    areaIn = computeArea(inside);
    areaOut = computeArea(outside);
    cout << areaOut - areaIn << endl;
    return 0;
}*/
#3
/*
#include <iostream>
#include <cmath>
using namespace std;
double computeArea(double side) {
    double area;
    area = 3 * sqrt(3) / 2 * side * side;
    return area;
}
int main() {
    double side, area;
    cin >> side;
    area = computeArea(side);
    cout << area << endl;
    return 0;
}*/
#4
/*
#include <iostream>
using namespace std;
double computeArea(double r) {
    const double PI = 3.14159;
    double area;
    area = PI * r * r;
    return area;
}
int main() {
    double r, h, volume;
    cin >> r >> h;
    volume = computeArea(r) * h;
    cout << volume << endl;
    return 0;
}*/

#5
/*
#include <iostream>
using namespace std;
int main() {
	cout << "Welcome to C++!" << endl;
	
	return 0;
}
*/

#6
/*
#include <iostream>
using namespace std;
int main() {
	cout << "****" << endl;
	cout << "***" << endl;
	cout << "**" << endl;
	cout << "*" << endl;
	
	return 0;
}*/
#7
/*
#include <iostream>
using namespace std;
int main() {
	double faHeight, moHeight, girlHeight, boyHeight;
	cin >> faHeight >> moHeight;
	boyHeight = (faHeight + moHeight) * 1.08 / 2;
	girlHeight = (faHeight + 0.923 * moHeight) / 2;
	
	cout << boyHeight << " " << girlHeight << endl;
	
	return 0;
}
*/
#8
/*
#include <iostream>
using namespace std;
int main() {
	int a, b, c, result;
	cin >> a;
	cin >> b;
	cin >> c;
	result = b * b - 4 * a * c;
	cout << result << endl;
	
	return 0;
}*/
#9
/*
#include <iostream>
#include <cmath>
using namespace std;
double computeArea(int r) {
    double PI = 3.14159;
    double area;
    area = PI * r * r;
    return area;
}
int main() {
    int r, h;
    double volume;
    int num;
    cin >> h >> r;
    volume = computeArea(r) * h;
    num = ceil(20 * 1000 / volume);
    cout << num << endl;
    return 0;
}*/
#10 
#include <iostream>
using namespace std;
int main() {
	double f, c;
	cin >> c;
	f = 9 / 5.0 * c + 32;
	cout << f << endl;
	
	return 0;
}

