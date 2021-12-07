#include "point.hpp"
#include "path.hpp"
#include <vector>

using namespace PP;

int main(){

    Path testPath = Path(
        {
            {0.0 , 0.0},
            {24.0, 0.0},
            {24.0, 24.0}
        },
        6.0,
        0.6,
        0.001,
        25.0,
        40.0,
        4
    );

    std::cout << testPath;

}