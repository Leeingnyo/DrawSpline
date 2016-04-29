#pragma once

namespace mymath{
    inline uint64_t Combinations(unsigned int n, unsigned int k) {
         if (k > n)
             return 0;
         uint64_t r = 1;
         for (unsigned int d = 1; d <= k; ++d)
         {
             r *= n--;
             r /= d;
         }
         return r;
    }
};
