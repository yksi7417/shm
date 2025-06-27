#include <cassert>
#include "shm/add.h"

int main() {
    return add(2, 3) == 5 ? 0 : 1;
}
