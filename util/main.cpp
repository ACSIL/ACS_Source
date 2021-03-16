
 #include<iostream>

	
void f(std::string &s = "hello"){
    std::cout << s;
}

 int main(){
     f();
     return 0;
 }