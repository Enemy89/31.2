#include <iostream>
#include <string>

class Toy {
    std::string toyName;

public:
    Toy(std::string inName) : toyName(inName) {};
    Toy() : toyName("SomeToy") {};

    ~Toy() {
        std::cout << "Toy " << toyName << " was dropped " << std::endl;
    }
};

class shared_ptr_toy {
    Toy *ptrToy;
    int *refCount;

public:
    shared_ptr_toy(Toy *ptrToy) : ptrToy(ptrToy), refCount(new int(1)) {}

    shared_ptr_toy(const shared_ptr_toy &other) : ptrToy(other.ptrToy), refCount(other.refCount) {
        (*refCount)++;
    }

    shared_ptr_toy &operator=(const shared_ptr_toy &other) {
        if (this != &other) {
            if (--(*refCount) == 0) {
                delete ptrToy;
                delete refCount;
            }
            ptrToy = other.ptrToy;
            refCount = other.refCount;
            (*refCount)++;
        }
        return *this;
    }

    int get_ref_count() const {
        return *refCount;
    }

    void reset(Toy *newPtrToy = nullptr) {
        if (refCount && --(*refCount) == 0) {
            delete ptrToy;
            delete refCount;
        }
        ptrToy = newPtrToy;
        refCount = new int(1);
    }

    bool isSameToy(const shared_ptr_toy &other) const {
        return ptrToy == other.ptrToy;
    }

    ~shared_ptr_toy() {
        if (--(*refCount) == 0) {
            delete ptrToy;
            delete refCount;
        }
    }
};

shared_ptr_toy make_shared_toy(const std::string &name) {
    return shared_ptr_toy(new Toy(name));
}

shared_ptr_toy make_shared_toy() {
    return shared_ptr_toy(new Toy());
}

class Dog {
    std::string dogName;
    int age;
    shared_ptr_toy lovelyToy;

public:
    Dog(std::string inName, shared_ptr_toy inLovelyToy, int inAge)
            : dogName(inName), lovelyToy(inLovelyToy), age(0) {
        if (inAge >= 0 && inAge < 30) {
            age = inAge;
        }
    }
    Dog() : dogName("Snow"), lovelyToy(make_shared_toy()), age(0) {}
    Dog(std::string inName, int inAge) : dogName(inName), lovelyToy(make_shared_toy()), age(0) {
        if (inAge >= 0 && inAge < 30) {
            age = inAge;
        }
    }
    Dog(std::string inName) : dogName(inName), lovelyToy(make_shared_toy()), age(0) {}
    Dog(int inAge) : dogName("Snow"), lovelyToy(make_shared_toy()), age(0) {
        if (inAge >= 0 && inAge < 30) {
            age = inAge;
        }
    }

    void getToy(const shared_ptr_toy &toy) {
        if (lovelyToy.isSameToy(toy)) {
            std::cout << "I already have this toy." << std::endl;
        } else if (toy.get_ref_count() > 1) {
            std::cout << "Another dog is playing with this toy." << std::endl;
        } else {
            lovelyToy = toy;
            std::cout << "My darling!" << std::endl;
        }
    }

    void dropToy() {
        if (lovelyToy.get_ref_count() > 1) {
            lovelyToy.reset();
        } else {
            std::cout << "Nothing to drop." << std::endl;
        }
    }
};

int main() {
    shared_ptr_toy bone = make_shared_toy("Bone");
    shared_ptr_toy ball = make_shared_toy("Ball");

    // std::cout<<bone.get_ref_count()<<std::endl;

    Dog a("Bobik", bone, 12);
    Dog b("Richi", 7);
    Dog c("Mino", 2);
    Dog d("Princess", 5);

    a.getToy(bone);
    // std::cout<<bone.get_ref_count()<<std::endl;
    b.getToy(bone);
    // std::cout<<bone.get_ref_count()<<std::endl;
    a.dropToy();
    // std::cout<<bone.get_ref_count()<<std::endl;
    b.getToy(bone);
    // std::cout<<bone.get_ref_count()<<std::endl;
    c.getToy(bone);
    c.dropToy();

    return 0;
}
