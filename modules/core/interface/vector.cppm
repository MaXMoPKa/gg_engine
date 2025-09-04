module;

#include <stdexcept>

export module core.vector;

import core.types;

export namespace gg {

template<class T> struct vec2;
template<class T> struct vec3;
template<class T> struct vec4;

template<typename T, int SIZE, int... INDEXES>
struct swizzle {
    T v[SIZE];
    static constexpr int indexes[] = {INDEXES...};

    template<int RSIZE, int... RINDEXES>
    inline auto& operator=(const swizzle<T, RSIZE, RINDEXES...>& rhs) {
        static_assert(SIZE == RSIZE, "error: assigning swizzle of different dimensions");
        constexpr int rindexes[] = {RINDEXES...};

        for(int i = 0; i < SIZE; ++i) {
            v[indexes[i]] = rhs.v[rindexes[i]];
        }

        return *this;
    }

    inline auto& operator=(const swizzle& rhs) {
        for(int i = 0; i < SIZE; ++i) {
            v[indexes[i]] = rhs.v[indexes[i]];
        }
        return *this;
    }
};

template<typename T, int SIZE, int... SWIZZLES>
struct swizzle2 : public swizzle<T, SIZE, SWIZZLES...> {
    inline swizzle<T, SIZE, SWIZZLES...>& operator=(const vec2<T>& l) {
        static_assert(SIZE == 2, "error: assigning swizzle2 not from vec2");
        this->v[this->indexes[0]] = l.x;
        this->v[this->indexes[1]] = l.y;
        return *this;
    }

    inline operator vec2<T>() const {
        static_assert(SIZE > 1, "error: no data that convert to vec2");
        return vec2<T>{this->v[this->indexes[0]], this->v[this->indexes[1]]};
    }

    inline const T& operator[](size_t i) const {
        static_assert(SIZE == 2, "error: subscript swizzle2 not from vec2");
        return this->v[this->indexes[i]];
    }

    inline const T& at(size_t i) const
    {
        static_assert(SIZE == 2, "error: at method of swizzle2 not from vec2");

        if(i >= 2)
        {
            throw std::out_of_range("error: swizzle2 index out of range");
        }

        return this->v[this->indexes[i]];
    }
};

template<typename T, int SIZE, int... SWIZZLES>
struct swizzle3 : public swizzle<T, SIZE, SWIZZLES...> {
    inline swizzle<T, SIZE, SWIZZLES...>& operator=(const vec3<T>& l) {
        static_assert(SIZE == 3, "error: assigning swizzle3 not from vec3");
        this->v[this->indexes[0]] = l.x;
        this->v[this->indexes[1]] = l.y;
        this->v[this->indexes[2]] = l.z;
        return *this;
    }

    inline operator vec3<T>() const {
        static_assert(SIZE > 2, "error: no data that convert to vec3");
        return vec3<T>{this->v[this->indexes[0]], this->v[this->indexes[1]], this->v[this->indexes[2]]};
    }

    inline const T& operator[](size_t i) const {
        static_assert(SIZE == 3, "error: subscript swizzle3 not from vec3");
        return this->v[this->indexes[i]];
    }

    inline const T& at(size_t i) const
    {
        static_assert(SIZE == 3, "error: at method of swizzle3 not from vec3");

        if(i >= 3)
        {
            throw std::out_of_range("error: swizzle3 index out of range");
        }

        return this->v[this->indexes[i]];
    }
};

template<typename T, int SIZE, int... SWIZZLES>
struct swizzle4 : public swizzle<T, SIZE, SWIZZLES...> {
    inline swizzle<T, SIZE, SWIZZLES...>& operator=(const vec4<T>& l) {
        static_assert(SIZE == 4, "error: assigning swizzle4 not from vec4");
        this->v[this->indexes[0]] = l.x;
        this->v[this->indexes[1]] = l.y;
        this->v[this->indexes[2]] = l.z;
        this->v[this->indexes[3]] = l.w;
        return *this;
    }

    inline operator vec4<T>() const {
        static_assert(SIZE > 3, "error: no data that convert to vec4");
        return vec4<T>{this->v[this->indexes[0]], this->v[this->indexes[1]], this->v[this->indexes[2]], this->v[this->indexes[3]]};
    }

    inline const T& operator[](size_t i) const {
        static_assert(SIZE == 4, "error: subscript swizzle4 not from vec4");
        return this->v[this->indexes[i]];
    }

    inline const T& at(size_t i) const
    {
        static_assert(SIZE == 4, "error: at method of swizzle4 not from vec4");

        if(i >= 4)
        {
            throw std::out_of_range("error: swizzle4 index out of range");
        }

        return this->v[this->indexes[i]];
    }
};

constexpr int swizzle_idx__(const char* x, int offset) {
    switch(*(x + offset)) {
        case 'x': return 0;
        case 'y': return 1;
        case 'z': return 2;
        case 'w': return 3;
    }
    return -1;
}

template<class T>
struct vec2 {
    union {
        struct {T x, y;};

        swizzle2<T, 2, 0, 0> xx;
        swizzle2<T, 2, 0, 1> xy;
        swizzle2<T, 2, 1, 0> yx;
        swizzle2<T, 2, 1, 1> yy;
    };
};

template<class T>
struct vec3 {
    union {
        struct {T x, y, z;};

        swizzle2<T, 2, 0, 0> xx;
        swizzle2<T, 2, 0, 1> xy;
        swizzle2<T, 2, 0, 2> xz;
        swizzle2<T, 2, 1, 0> yx;
        swizzle2<T, 2, 1, 1> yy;
        swizzle2<T, 2, 1, 2> yz;
        swizzle2<T, 2, 2, 0> zx;
        swizzle2<T, 2, 2, 1> zy;
        swizzle2<T, 2, 2, 2> zz;

        swizzle3<T, 3, 0, 0, 0> xxx;
        swizzle3<T, 3, 0, 0, 1> xxy;
        swizzle3<T, 3, 0, 0, 2> xxz;
        swizzle3<T, 3, 0, 1, 0> xyx;
        swizzle3<T, 3, 0, 1, 1> xyy;
        swizzle3<T, 3, 0, 1, 2> xyz;
        swizzle3<T, 3, 0, 2, 0> xzx;
        swizzle3<T, 3, 0, 2, 1> xzy;
        swizzle3<T, 3, 0, 2, 2> xzz;

        swizzle3<T, 3, 1, 0, 0> yxx;
        swizzle3<T, 3, 1, 0, 1> yxy;
        swizzle3<T, 3, 1, 0, 2> yxz;
        swizzle3<T, 3, 1, 1, 0> yyx;
        swizzle3<T, 3, 1, 1, 1> yyy;
        swizzle3<T, 3, 1, 1, 2> yyz;
        swizzle3<T, 3, 1, 2, 0> yzx;
        swizzle3<T, 3, 1, 2, 1> yzy;
        swizzle3<T, 3, 1, 2, 2> yzz;

        swizzle3<T, 3, 2, 0, 0> zxx;
        swizzle3<T, 3, 2, 0, 1> zxy;
        swizzle3<T, 3, 2, 0, 2> zxz;
        swizzle3<T, 3, 2, 1, 0> zyx;
        swizzle3<T, 3, 2, 1, 1> zyy;
        swizzle3<T, 3, 2, 1, 2> zyz;
        swizzle3<T, 3, 2, 2, 0> zzx;
        swizzle3<T, 3, 2, 2, 1> zzy;
        swizzle3<T, 3, 2, 2, 2> zzz;
    };
};

template<class T>
struct vec4 {
    union {
        struct {T x, y, z, w;};

        swizzle2<T, 2, 0, 0> xx;
        swizzle2<T, 2, 0, 1> xy;
        swizzle2<T, 2, 0, 2> xz;
        swizzle2<T, 2, 0, 3> xw;
        swizzle2<T, 2, 1, 0> yx;
        swizzle2<T, 2, 1, 1> yy;
        swizzle2<T, 2, 1, 2> yz;
        swizzle2<T, 2, 1, 3> yw;
        swizzle2<T, 2, 2, 0> zx;
        swizzle2<T, 2, 2, 1> zy;
        swizzle2<T, 2, 2, 2> zz;
        swizzle2<T, 2, 2, 3> zw;
        swizzle2<T, 2, 3, 0> wx;
        swizzle2<T, 2, 3, 1> wy;
        swizzle2<T, 2, 3, 2> wz;
        swizzle2<T, 2, 3, 3> ww;

        swizzle3<T, 3, 0, 0, 0> xxx;
        swizzle3<T, 3, 0, 0, 1> xxy;
        swizzle3<T, 3, 0, 0, 2> xxz;
        swizzle3<T, 3, 0, 0, 3> xxw;
        swizzle3<T, 3, 0, 1, 0> xyx;
        swizzle3<T, 3, 0, 1, 1> xyy;
        swizzle3<T, 3, 0, 1, 2> xyz;
        swizzle3<T, 3, 0, 1, 3> xyw;
        swizzle3<T, 3, 0, 2, 0> xzx;
        swizzle3<T, 3, 0, 2, 1> xzy;
        swizzle3<T, 3, 0, 2, 2> xzz;
        swizzle3<T, 3, 0, 2, 3> xzw;
        swizzle3<T, 3, 0, 3, 0> xwx;
        swizzle3<T, 3, 0, 3, 1> xwy;
        swizzle3<T, 3, 0, 3, 2> xwz;
        swizzle3<T, 3, 0, 3, 3> xww;

        swizzle3<T, 3, 1, 0, 0> yxx;
        swizzle3<T, 3, 1, 0, 1> yxy;
        swizzle3<T, 3, 1, 0, 2> yxz;
        swizzle3<T, 3, 1, 0, 3> yxw;
        swizzle3<T, 3, 1, 1, 0> yyx;
        swizzle3<T, 3, 1, 1, 1> yyy;
        swizzle3<T, 3, 1, 1, 2> yyz;
        swizzle3<T, 3, 1, 1, 3> yyw;
        swizzle3<T, 3, 1, 2, 0> yzx;
        swizzle3<T, 3, 1, 2, 1> yzy;
        swizzle3<T, 3, 1, 2, 2> yzz;
        swizzle3<T, 3, 1, 2, 3> yzw;
        swizzle3<T, 3, 1, 3, 0> ywx;
        swizzle3<T, 3, 1, 3, 1> ywy;
        swizzle3<T, 3, 1, 3, 2> ywz;
        swizzle3<T, 3, 1, 3, 3> yww;

        swizzle3<T, 3, 2, 0, 0> zxx;
        swizzle3<T, 3, 2, 0, 1> zxy;
        swizzle3<T, 3, 2, 0, 2> zxz;
        swizzle3<T, 3, 2, 0, 3> zxw;
        swizzle3<T, 3, 2, 1, 0> zyx;
        swizzle3<T, 3, 2, 1, 1> zyy;
        swizzle3<T, 3, 2, 1, 2> zyz;
        swizzle3<T, 3, 2, 1, 3> zyw;
        swizzle3<T, 3, 2, 2, 0> zzx;
        swizzle3<T, 3, 2, 2, 1> zzy;
        swizzle3<T, 3, 2, 2, 2> zzz;
        swizzle3<T, 3, 2, 2, 3> zzw;
        swizzle3<T, 3, 2, 3, 0> zwx;
        swizzle3<T, 3, 2, 3, 1> zwy;
        swizzle3<T, 3, 2, 3, 2> zwz;
        swizzle3<T, 3, 2, 3, 3> zww;

        swizzle3<T, 3, 3, 0, 0> wxx;
        swizzle3<T, 3, 3, 0, 1> wxy;
        swizzle3<T, 3, 3, 0, 2> wxz;
        swizzle3<T, 3, 3, 0, 3> wxw;
        swizzle3<T, 3, 3, 1, 0> wyx;
        swizzle3<T, 3, 3, 1, 1> wyy;
        swizzle3<T, 3, 3, 1, 2> wyz;
        swizzle3<T, 3, 3, 1, 3> wyw;
        swizzle3<T, 3, 3, 2, 0> wzx;
        swizzle3<T, 3, 3, 2, 1> wzy;
        swizzle3<T, 3, 3, 2, 2> wzz;
        swizzle3<T, 3, 3, 2, 3> wzw;
        swizzle3<T, 3, 3, 3, 0> wwx;
        swizzle3<T, 3, 3, 3, 1> wwy;
        swizzle3<T, 3, 3, 3, 2> wwz;
        swizzle3<T, 3, 3, 3, 3> www;

        swizzle4<T, 4, 0, 0, 0, 0> xxxx;
        swizzle4<T, 4, 0, 0, 0, 1> xxxy;
        swizzle4<T, 4, 0, 0, 0, 2> xxxz;
        swizzle4<T, 4, 0, 0, 0, 3> xxxw;
        swizzle4<T, 4, 0, 0, 1, 0> xxyx;
        swizzle4<T, 4, 0, 0, 1, 1> xxyy;
        swizzle4<T, 4, 0, 0, 1, 2> xxyz;
        swizzle4<T, 4, 0, 0, 1, 3> xxyw;
        swizzle4<T, 4, 0, 0, 2, 0> xxzx;
        swizzle4<T, 4, 0, 0, 2, 1> xxzy;
        swizzle4<T, 4, 0, 0, 2, 2> xxzz;
        swizzle4<T, 4, 0, 0, 2, 3> xxzw;
        swizzle4<T, 4, 0, 0, 3, 0> xxwx;
        swizzle4<T, 4, 0, 0, 3, 1> xxwy;
        swizzle4<T, 4, 0, 0, 3, 2> xxwz;
        swizzle4<T, 4, 0, 0, 3, 3> xxww;

        swizzle4<T, 4, 0, 1, 0, 0> xyxx;
        swizzle4<T, 4, 0, 1, 0, 1> xyxy;
        swizzle4<T, 4, 0, 1, 0, 2> xyxz;
        swizzle4<T, 4, 0, 1, 0, 3> xyxw;
        swizzle4<T, 4, 0, 1, 1, 0> xyyx;
        swizzle4<T, 4, 0, 1, 1, 1> xyyy;
        swizzle4<T, 4, 0, 1, 1, 2> xyyz;
        swizzle4<T, 4, 0, 1, 1, 3> xyyw;
        swizzle4<T, 4, 0, 1, 2, 0> xyzx;
        swizzle4<T, 4, 0, 1, 2, 1> xyzy;
        swizzle4<T, 4, 0, 1, 2, 2> xyzz;
        swizzle4<T, 4, 0, 1, 2, 3> xyzw;
        swizzle4<T, 4, 0, 1, 3, 0> xywx;
        swizzle4<T, 4, 0, 1, 3, 1> xywy;
        swizzle4<T, 4, 0, 1, 3, 2> xywz;
        swizzle4<T, 4, 0, 1, 3, 3> xyww;

        swizzle4<T, 4, 0, 2, 0, 0> xzxx;
        swizzle4<T, 4, 0, 2, 0, 1> xzxy;
        swizzle4<T, 4, 0, 2, 0, 2> xzxz;
        swizzle4<T, 4, 0, 2, 0, 3> xzxw;
        swizzle4<T, 4, 0, 2, 1, 0> xzyx;
        swizzle4<T, 4, 0, 2, 1, 1> xzyy;
        swizzle4<T, 4, 0, 2, 1, 2> xzyz;
        swizzle4<T, 4, 0, 2, 1, 3> xzyw;
        swizzle4<T, 4, 0, 2, 2, 0> xzzx;
        swizzle4<T, 4, 0, 2, 2, 1> xzzy;
        swizzle4<T, 4, 0, 2, 2, 2> xzzz;
        swizzle4<T, 4, 0, 2, 2, 3> xzzw;
        swizzle4<T, 4, 0, 2, 3, 0> xzwx;
        swizzle4<T, 4, 0, 2, 3, 1> xzwy;
        swizzle4<T, 4, 0, 2, 3, 2> xzwz;
        swizzle4<T, 4, 0, 2, 3, 3> xzww;

        swizzle4<T, 4, 0, 3, 0, 0> xwxx;
        swizzle4<T, 4, 0, 3, 0, 1> xwxy;
        swizzle4<T, 4, 0, 3, 0, 2> xwxz;
        swizzle4<T, 4, 0, 3, 0, 3> xwxw;
        swizzle4<T, 4, 0, 3, 1, 0> xwyx;
        swizzle4<T, 4, 0, 3, 1, 1> xwyy;
        swizzle4<T, 4, 0, 3, 1, 2> xwyz;
        swizzle4<T, 4, 0, 3, 1, 3> xwyw;
        swizzle4<T, 4, 0, 3, 2, 0> xwzx;
        swizzle4<T, 4, 0, 3, 2, 1> xwzy;
        swizzle4<T, 4, 0, 3, 2, 2> xwzz;
        swizzle4<T, 4, 0, 3, 2, 3> xwzw;
        swizzle4<T, 4, 0, 3, 3, 0> xwwx;
        swizzle4<T, 4, 0, 3, 3, 1> xwwy;
        swizzle4<T, 4, 0, 3, 3, 2> xwwz;
        swizzle4<T, 4, 0, 3, 3, 3> xwww;

        swizzle4<T, 4, 1, 0, 0, 0> yxxx;
        swizzle4<T, 4, 1, 0, 0, 1> yxxy;
        swizzle4<T, 4, 1, 0, 0, 2> yxxz;
        swizzle4<T, 4, 1, 0, 0, 3> yxxw;
        swizzle4<T, 4, 1, 0, 1, 0> yxyx;
        swizzle4<T, 4, 1, 0, 1, 1> yxyy;
        swizzle4<T, 4, 1, 0, 1, 2> yxyz;
        swizzle4<T, 4, 1, 0, 1, 3> yxyw;
        swizzle4<T, 4, 1, 0, 2, 0> yxzx;
        swizzle4<T, 4, 1, 0, 2, 1> yxzy;
        swizzle4<T, 4, 1, 0, 2, 2> yxzz;
        swizzle4<T, 4, 1, 0, 2, 3> yxzw;
        swizzle4<T, 4, 1, 0, 3, 0> yxwx;
        swizzle4<T, 4, 1, 0, 3, 1> yxwy;
        swizzle4<T, 4, 1, 0, 3, 2> yxwz;
        swizzle4<T, 4, 1, 0, 3, 3> yxww;

        swizzle4<T, 4, 1, 1, 0, 0> yyxx;
        swizzle4<T, 4, 1, 1, 0, 1> yyxy;
        swizzle4<T, 4, 1, 1, 0, 2> yyxz;
        swizzle4<T, 4, 1, 1, 0, 3> yyxw;
        swizzle4<T, 4, 1, 1, 1, 0> yyyx;
        swizzle4<T, 4, 1, 1, 1, 1> yyyy;
        swizzle4<T, 4, 1, 1, 1, 2> yyyz;
        swizzle4<T, 4, 1, 1, 1, 3> yyyw;
        swizzle4<T, 4, 1, 1, 2, 0> yyzx;
        swizzle4<T, 4, 1, 1, 2, 1> yyzy;
        swizzle4<T, 4, 1, 1, 2, 2> yyzz;
        swizzle4<T, 4, 1, 1, 2, 3> yyzw;
        swizzle4<T, 4, 1, 1, 3, 0> yywx;
        swizzle4<T, 4, 1, 1, 3, 1> yywy;
        swizzle4<T, 4, 1, 1, 3, 2> yywz;
        swizzle4<T, 4, 1, 1, 3, 3> yyww;

        swizzle4<T, 4, 1, 2, 0, 0> yzxx;
        swizzle4<T, 4, 1, 2, 0, 1> yzxy;
        swizzle4<T, 4, 1, 2, 0, 2> yzxz;
        swizzle4<T, 4, 1, 2, 0, 3> yzxw;
        swizzle4<T, 4, 1, 2, 1, 0> yzyx;
        swizzle4<T, 4, 1, 2, 1, 1> yzyy;
        swizzle4<T, 4, 1, 2, 1, 2> yzyz;
        swizzle4<T, 4, 1, 2, 1, 3> yzyw;
        swizzle4<T, 4, 1, 2, 2, 0> yzzx;
        swizzle4<T, 4, 1, 2, 2, 1> yzzy;
        swizzle4<T, 4, 1, 2, 2, 2> yzzz;
        swizzle4<T, 4, 1, 2, 2, 3> yzzw;
        swizzle4<T, 4, 1, 2, 3, 0> yzwx;
        swizzle4<T, 4, 1, 2, 3, 1> yzwy;
        swizzle4<T, 4, 1, 2, 3, 2> yzwz;
        swizzle4<T, 4, 1, 2, 3, 3> yzww;

        swizzle4<T, 4, 1, 3, 0, 0> ywxx;
        swizzle4<T, 4, 1, 3, 0, 1> ywxy;
        swizzle4<T, 4, 1, 3, 0, 2> ywxz;
        swizzle4<T, 4, 1, 3, 0, 3> ywxw;
        swizzle4<T, 4, 1, 3, 1, 0> ywyx;
        swizzle4<T, 4, 1, 3, 1, 1> ywyy;
        swizzle4<T, 4, 1, 3, 1, 2> ywyz;
        swizzle4<T, 4, 1, 3, 1, 3> ywyw;
        swizzle4<T, 4, 1, 3, 2, 0> ywzx;
        swizzle4<T, 4, 1, 3, 2, 1> ywzy;
        swizzle4<T, 4, 1, 3, 2, 2> ywzz;
        swizzle4<T, 4, 1, 3, 2, 3> ywzw;
        swizzle4<T, 4, 1, 3, 3, 0> ywwx;
        swizzle4<T, 4, 1, 3, 3, 1> ywwy;
        swizzle4<T, 4, 1, 3, 3, 2> ywwz;
        swizzle4<T, 4, 1, 3, 3, 3> ywww;

        swizzle4<T, 4, 2, 0, 0, 0> zxxx;
        swizzle4<T, 4, 2, 0, 0, 1> zxxy;
        swizzle4<T, 4, 2, 0, 0, 2> zxxz;
        swizzle4<T, 4, 2, 0, 0, 3> zxxw;
        swizzle4<T, 4, 2, 0, 1, 0> zxyx;
        swizzle4<T, 4, 2, 0, 1, 1> zxyy;
        swizzle4<T, 4, 2, 0, 1, 2> zxyz;
        swizzle4<T, 4, 2, 0, 1, 3> zxyw;
        swizzle4<T, 4, 2, 0, 2, 0> zxzx;
        swizzle4<T, 4, 2, 0, 2, 1> zxzy;
        swizzle4<T, 4, 2, 0, 2, 2> zxzz;
        swizzle4<T, 4, 2, 0, 2, 3> zxzw;
        swizzle4<T, 4, 2, 0, 3, 0> zxwx;
        swizzle4<T, 4, 2, 0, 3, 1> zxwy;
        swizzle4<T, 4, 2, 0, 3, 2> zxwz;
        swizzle4<T, 4, 2, 0, 3, 3> zxww;

        swizzle4<T, 4, 2, 1, 0, 0> zyxx;
        swizzle4<T, 4, 2, 1, 0, 1> zyxy;
        swizzle4<T, 4, 2, 1, 0, 2> zyxz;
        swizzle4<T, 4, 2, 1, 0, 3> zyxw;
        swizzle4<T, 4, 2, 1, 1, 0> zyyx;
        swizzle4<T, 4, 2, 1, 1, 1> zyyy;
        swizzle4<T, 4, 2, 1, 1, 2> zyyz;
        swizzle4<T, 4, 2, 1, 1, 3> zyyw;
        swizzle4<T, 4, 2, 1, 2, 0> zyzx;
        swizzle4<T, 4, 2, 1, 2, 1> zyzy;
        swizzle4<T, 4, 2, 1, 2, 2> zyzz;
        swizzle4<T, 4, 2, 1, 2, 3> zyzw;
        swizzle4<T, 4, 2, 1, 3, 0> zywx;
        swizzle4<T, 4, 2, 1, 3, 1> zywy;
        swizzle4<T, 4, 2, 1, 3, 2> zywz;
        swizzle4<T, 4, 2, 1, 3, 3> zyww;

        swizzle4<T, 4, 2, 2, 0, 0> zzxx;
        swizzle4<T, 4, 2, 2, 0, 1> zzxy;
        swizzle4<T, 4, 2, 2, 0, 2> zzxz;
        swizzle4<T, 4, 2, 2, 0, 3> zzxw;
        swizzle4<T, 4, 2, 2, 1, 0> zzyx;
        swizzle4<T, 4, 2, 2, 1, 1> zzyy;
        swizzle4<T, 4, 2, 2, 1, 2> zzyz;
        swizzle4<T, 4, 2, 2, 1, 3> zzyw;
        swizzle4<T, 4, 2, 2, 2, 0> zzzx;
        swizzle4<T, 4, 2, 2, 2, 1> zzzy;
        swizzle4<T, 4, 2, 2, 2, 2> zzzz;
        swizzle4<T, 4, 2, 2, 2, 3> zzzw;
        swizzle4<T, 4, 2, 2, 3, 0> zzwx;
        swizzle4<T, 4, 2, 2, 3, 1> zzwy;
        swizzle4<T, 4, 2, 2, 3, 2> zzwz;
        swizzle4<T, 4, 2, 2, 3, 3> zzww;

        swizzle4<T, 4, 2, 3, 0, 0> zwxx;
        swizzle4<T, 4, 2, 3, 0, 1> zwxy;
        swizzle4<T, 4, 2, 3, 0, 2> zwxz;
        swizzle4<T, 4, 2, 3, 0, 3> zwxw;
        swizzle4<T, 4, 2, 3, 1, 0> zwyx;
        swizzle4<T, 4, 2, 3, 1, 1> zwyy;
        swizzle4<T, 4, 2, 3, 1, 2> zwyz;
        swizzle4<T, 4, 2, 3, 1, 3> zwyw;
        swizzle4<T, 4, 2, 3, 2, 0> zwzx;
        swizzle4<T, 4, 2, 3, 2, 1> zwzy;
        swizzle4<T, 4, 2, 3, 2, 2> zwzz;
        swizzle4<T, 4, 2, 3, 2, 3> zwzw;
        swizzle4<T, 4, 2, 3, 3, 0> zwwx;
        swizzle4<T, 4, 2, 3, 3, 1> zwwy;
        swizzle4<T, 4, 2, 3, 3, 2> zwwz;
        swizzle4<T, 4, 2, 3, 3, 3> zwww;

        swizzle4<T, 4, 3, 0, 0, 0> wxxx;
        swizzle4<T, 4, 3, 0, 0, 1> wxxy;
        swizzle4<T, 4, 3, 0, 0, 2> wxxz;
        swizzle4<T, 4, 3, 0, 0, 3> wxxw;
        swizzle4<T, 4, 3, 0, 1, 0> wxyx;
        swizzle4<T, 4, 3, 0, 1, 1> wxyy;
        swizzle4<T, 4, 3, 0, 1, 2> wxyz;
        swizzle4<T, 4, 3, 0, 1, 3> wxyw;
        swizzle4<T, 4, 3, 0, 2, 0> wxzx;
        swizzle4<T, 4, 3, 0, 2, 1> wxzy;
        swizzle4<T, 4, 3, 0, 2, 2> wxzz;
        swizzle4<T, 4, 3, 0, 2, 3> wxzw;
        swizzle4<T, 4, 3, 0, 3, 0> wxwx;
        swizzle4<T, 4, 3, 0, 3, 1> wxwy;
        swizzle4<T, 4, 3, 0, 3, 2> wxwz;
        swizzle4<T, 4, 3, 0, 3, 3> wxww;

        swizzle4<T, 4, 3, 1, 0, 0> wyxx;
        swizzle4<T, 4, 3, 1, 0, 1> wyxy;
        swizzle4<T, 4, 3, 1, 0, 2> wyxz;
        swizzle4<T, 4, 3, 1, 0, 3> wyxw;
        swizzle4<T, 4, 3, 1, 1, 0> wyyx;
        swizzle4<T, 4, 3, 1, 1, 1> wyyy;
        swizzle4<T, 4, 3, 1, 1, 2> wyyz;
        swizzle4<T, 4, 3, 1, 1, 3> wyyw;
        swizzle4<T, 4, 3, 1, 2, 0> wyzx;
        swizzle4<T, 4, 3, 1, 2, 1> wyzy;
        swizzle4<T, 4, 3, 1, 2, 2> wyzz;
        swizzle4<T, 4, 3, 1, 2, 3> wyzw;
        swizzle4<T, 4, 3, 1, 3, 0> wywx;
        swizzle4<T, 4, 3, 1, 3, 1> wywy;
        swizzle4<T, 4, 3, 1, 3, 2> wywz;
        swizzle4<T, 4, 3, 1, 3, 3> wyww;

        swizzle4<T, 4, 3, 2, 0, 0> wzxx;
        swizzle4<T, 4, 3, 2, 0, 1> wzxy;
        swizzle4<T, 4, 3, 2, 0, 2> wzxz;
        swizzle4<T, 4, 3, 2, 0, 3> wzxw;
        swizzle4<T, 4, 3, 2, 1, 0> wzyx;
        swizzle4<T, 4, 3, 2, 1, 1> wzyy;
        swizzle4<T, 4, 3, 2, 1, 2> wzyz;
        swizzle4<T, 4, 3, 2, 1, 3> wzyw;
        swizzle4<T, 4, 3, 2, 2, 0> wzzx;
        swizzle4<T, 4, 3, 2, 2, 1> wzzy;
        swizzle4<T, 4, 3, 2, 2, 2> wzzz;
        swizzle4<T, 4, 3, 2, 2, 3> wzzw;
        swizzle4<T, 4, 3, 2, 3, 0> wzwx;
        swizzle4<T, 4, 3, 2, 3, 1> wzwy;
        swizzle4<T, 4, 3, 2, 3, 2> wzwz;
        swizzle4<T, 4, 3, 2, 3, 3> wzww;

        swizzle4<T, 4, 3, 3, 0, 0> wwxx;
        swizzle4<T, 4, 3, 3, 0, 1> wwxy;
        swizzle4<T, 4, 3, 3, 0, 2> wwxz;
        swizzle4<T, 4, 3, 3, 0, 3> wwxw;
        swizzle4<T, 4, 3, 3, 1, 0> wwyx;
        swizzle4<T, 4, 3, 3, 1, 1> wwyy;
        swizzle4<T, 4, 3, 3, 1, 2> wwyz;
        swizzle4<T, 4, 3, 3, 1, 3> wwyw;
        swizzle4<T, 4, 3, 3, 2, 0> wwzx;
        swizzle4<T, 4, 3, 3, 2, 1> wwzy;
        swizzle4<T, 4, 3, 3, 2, 2> wwzz;
        swizzle4<T, 4, 3, 3, 2, 3> wwzw;
        swizzle4<T, 4, 3, 3, 3, 0> wwwx;
        swizzle4<T, 4, 3, 3, 3, 1> wwwy;
        swizzle4<T, 4, 3, 3, 3, 2> wwwz;
        swizzle4<T, 4, 3, 3, 3, 3> wwww;
    };
};

using fvec2 = vec2<float>;
using fvec3 = vec3<float>;
using fvec4 = vec4<float>;

using dvec2 = vec2<double>;
using dvec3 = vec3<double>;
using dvec4 = vec4<double>;

using u8vec2 = vec2<u8>;
using u8vec3 = vec3<u8>;
using u8vec4 = vec4<u8>;

using u16vec2 = vec2<u16>;
using u16vec3 = vec3<u16>;
using u16vec4 = vec4<u16>;

using u32vec2 = vec2<u32>;
using u32vec3 = vec3<u32>;
using u32vec4 = vec4<u32>;

using u64vec2 = vec2<u64>;
using u64vec3 = vec3<u64>;
using u64vec4 = vec4<u64>;

using i8vec2 = vec2<i8>;
using i8vec3 = vec3<i8>;
using i8vec4 = vec4<i8>;

using i16vec2 = vec2<i16>;
using i16vec3 = vec3<i16>;
using i16vec4 = vec4<i16>;

using i32vec2 = vec2<i32>;
using i32vec3 = vec3<i32>;
using i32vec4 = vec4<i32>;

using i64vec2 = vec2<i64>;
using i64vec3 = vec3<i64>;
using i64vec4 = vec4<i64>;

} // namespace gg
