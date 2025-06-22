//build method 1: gcc exam_CheckLeftRight.c ../matrixCalcBase.c -o exam -lm -g
/* --- Check Vec2 lib --- 
 * Author: Le Quoc Hai __devh__
 * Edit: Jun 1 2025
 * License: MIT
 *
*/

#include "../matrixCalcBase.h"
#define get_relative_direction(vecA, vecB) CHECK_RELATIVE_DIRECTION(vecA, vecB)
int main() {
    Vec2 a = {1.0f, 0.0f};
    Vec2 b = {0.0f, 1.0f};
    Vec2 c = {-1.0f, 0.0f};

    eDir3s dir1 = get_relative_direction(a, b);
    eDir3s dir2 = get_relative_direction(a, c);
    eDir3s dir3 = get_relative_direction(a, a);
    eDir3s dir4 = get_relative_direction(b, c);
    eDir3s dir5 = CHECK_RELATIVE_DIRECTION(b, c);

    return 0;
}

