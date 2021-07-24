#include "skipList.h"
int main() {
    skipList<std::string,std::string> sl(10);
    sl.loadFile();
    sl.display();
    return 0;
}
