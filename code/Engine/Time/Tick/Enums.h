#pragma once

namespace Future {

class TickEnums {
public:
    enum Bits {
        NodeBits = 6,
        RootBits = 8,
    };

    enum Size {
        NodeSize = (1 << NodeBits),
        RootSize = (1 << RootBits),
    };

    enum Mask {
        NodeMask = NodeSize - 1,
        RootMask = RootSize - 1,
    };

    const static int TotalNode = 5;
};

}