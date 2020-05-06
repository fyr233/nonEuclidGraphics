#pragma once

#include <initializer_list>
#include <assert.h>
#include <vector>
#include <core/mat.h>

namespace cgcore
{

    //本来为了普遍性，这里可以实现一个任意阶的张量类型，和爱因斯坦求和。
    //但是在这个程序中需要的非常有限，只实现shape = (3,3,3) 的特例tensor更容易，并且运算也更快


    struct tensorf333
    {
        float* const data = new float[27];
        const size_t size = 27;
        
        tensorf333() {}

        tensorf333(const tensorf333& t2)
        {
            for (float* p1 = data, *p2 = t2.data, *pe = data + size; p1 < pe; p1++, p2++)
                *p1 = *p2;
        }

        tensorf333(const std::initializer_list<float>& elem_list)
        {
            assert(elem_list.size() <= size);
            size_t i = 0;
            for (float it : elem_list)
                data[i++] = it;
        }

        ~tensorf333() { delete[] data; }

        float& operator()(size_t i, size_t j, size_t k)
        {
            return data[9*i + 3*j + k];
        }

        //连续点乘 TODO
        vecf3 dot_s(const vecf3& v1, const vecf3& v2)
        {

            float ret = 0;
            for (size_t j = 0; j < 3; j++)
            {
                for (size_t k = 0; k < 3; k++)
                {
                    ret += v1.data[j] * v2.data[k];
                }
            }
        }
    };
}