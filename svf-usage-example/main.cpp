#include <iostream>

// Base class
class Animal {
public:
    virtual void makeSound() {
        std::cout << "The animal makes a sound" << std::endl;
    }
};

// Derived class
class Dog : public Animal {
public:
    void makeSound() override {
        std::cout << "The dog barks" << std::endl;
    }
};

// Another derived class
class Cat : public Animal {
public:
    void makeSound() override {
        std::cout << "The cat meows" << std::endl;
    }
};

int main() {
    Animal* animal1 = new Dog();
    Animal* animal2 = new Cat();

    animal1->makeSound(); // Outputs: The dog barks
    animal2->makeSound(); // Outputs: The cat meows

    delete animal1;
    delete animal2;

    return 0;
}

/*
cd .example
clang -emit-llvm -c main.cpp -o main.bc
clang -emit-llvm -S main.cpp -o main.ll
/home/SVF-tools/SVF/Debug-build/bin/wpa -ander main.ll -dump-json result.json
/home/SVF-tools/SVF/Debug-build/bin/wpa -ander -read-json result.json
*/